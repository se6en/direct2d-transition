#include "stdafx.h"
#include "direct2d-transition-draw-ctrl.h"
#include "CustomEffect.h"

IMPLEMENT_DYNAMIC(CDirect2DTransitionDrawCtrl, CStatic)

CDirect2DTransitionDrawCtrl::CDirect2DTransitionDrawCtrl()
{
   m_customEffect = nullptr;
}

CDirect2DTransitionDrawCtrl::~CDirect2DTransitionDrawCtrl()
{

}

BEGIN_MESSAGE_MAP(CDirect2DTransitionDrawCtrl, CStatic)
   ON_WM_SIZE()
   ON_WM_PAINT()
END_MESSAGE_MAP()

void CDirect2DTransitionDrawCtrl::SetProgress(float fProgress)
{
   if (m_customEffect == nullptr)
   {
      return;
   }

   m_customEffect->SetValue(CUSTOM_PROP_PROGRESS, fProgress);

   Invalidate();
   UpdateWindow();
}

void CDirect2DTransitionDrawCtrl::CreateDeviceIndependentResources()
{
   IDXGIAdapter *pDxgiAdapter = nullptr;
   ID3D11Device *pD3D11Device = nullptr;
   ID3D11DeviceContext *pD3D11DeviceContext = nullptr;
   IDXGIDevice1 *pDxgiDevice = nullptr;
   IDXGIFactory2 *pDxgiFactory = nullptr;
   IDXGISurface *pDxgiBackBuffer = nullptr;

   UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
   creationFlags |= D3D11_CREATE_DEVICE_DEBUG;

   D3D_FEATURE_LEVEL featureLevels[] = {
   D3D_FEATURE_LEVEL_11_1,
   D3D_FEATURE_LEVEL_11_0,
   D3D_FEATURE_LEVEL_10_1,
   D3D_FEATURE_LEVEL_10_0,
   D3D_FEATURE_LEVEL_9_3,
   D3D_FEATURE_LEVEL_9_2,
   D3D_FEATURE_LEVEL_9_1
   };

   D3D_FEATURE_LEVEL featureLevel;

   HRESULT hr = D3D11CreateDevice(pDxgiAdapter, D3D_DRIVER_TYPE_HARDWARE, nullptr, creationFlags, featureLevels, sizeof(featureLevels) / sizeof(D3D_FEATURE_LEVEL), 
      D3D11_SDK_VERSION, &pD3D11Device, &featureLevel, &pD3D11DeviceContext);

   if (SUCCEEDED(hr))
   {
      hr = pD3D11Device->QueryInterface(__uuidof(IDXGIDevice1), (void **)&pDxgiDevice);
   }

   if (SUCCEEDED(hr))
   {
      hr = pDxgiDevice->GetAdapter(&pDxgiAdapter);
   }

   if (SUCCEEDED(hr))
   {
      hr = pDxgiAdapter->GetParent(IID_PPV_ARGS(&pDxgiFactory));
   }

   if (SUCCEEDED(hr))
   {
      DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullscreenDesc;
      fullscreenDesc.RefreshRate.Numerator = 60;
      fullscreenDesc.RefreshRate.Denominator = 1;
      fullscreenDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
      fullscreenDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
      fullscreenDesc.Windowed = FALSE;

      DXGI_SWAP_CHAIN_DESC1 swapChainDesc = { 0 };
      swapChainDesc.Height = 0;
      swapChainDesc.Width = 0;
      swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
      swapChainDesc.BufferCount = 2;
      swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
      swapChainDesc.Flags = 0;
      swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
      swapChainDesc.SampleDesc.Count = 1;
      swapChainDesc.SampleDesc.Quality = 0;
      swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
      swapChainDesc.Stereo = FALSE;
      swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

      hr = pDxgiFactory->CreateSwapChainForHwnd(pD3D11Device, m_hWnd, &swapChainDesc, nullptr, nullptr, &m_pSwapChain);
   }

   if (SUCCEEDED(hr))
   {
      hr = m_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pDxgiBackBuffer));
   }

   if (SUCCEEDED(hr))
   {
      hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory1), &m_pD2DFactory);
   }

   if (SUCCEEDED(hr))
   {
      hr = m_pD2DFactory->CreateDevice(pDxgiDevice, &m_pDevice);
   }

   if (SUCCEEDED(hr))
   {
      hr = m_pDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &m_pD2DContext);
      m_pD2DContext->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
   }

   if (SUCCEEDED(hr))
   {
      D2D1_BITMAP_PROPERTIES1 bitmapProperties = D2D1::BitmapProperties1(
         D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
         D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED),
         96.0f,
         96.0f
      );

      hr = m_pD2DContext->CreateBitmapFromDxgiSurface(pDxgiBackBuffer, &bitmapProperties, &m_pTargetBitmap);
   }

   CoInitialize(nullptr);

   if (SUCCEEDED(hr))
   {
      hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&m_pWicFactory));
   }

   // source bitmap
   if (SUCCEEDED(hr))
   {
      hr = m_pWicFactory->CreateDecoderFromFilename(
         L"TransitionSource.png",
         nullptr,
         GENERIC_READ,
         WICDecodeMetadataCacheOnDemand,
         &m_pWicSrcBitmapDecoder);
   }

   if (SUCCEEDED(hr))
   {
      hr = m_pWicSrcBitmapDecoder->GetFrame(0, &m_pWicSrcBitmapFrameDecoder);
   }

   if (SUCCEEDED(hr))
   {
      hr = m_pWicFactory->CreateFormatConverter(&m_pWicSrcFormatConverter);
   }

   if (SUCCEEDED(hr))
   {
      hr = m_pWicSrcFormatConverter->Initialize(
         m_pWicSrcBitmapFrameDecoder.Get(),
         GUID_WICPixelFormat32bppPBGRA,
         WICBitmapDitherTypeNone,
         nullptr,
         0.0f,
         WICBitmapPaletteTypeCustom
      );
   }

   if (SUCCEEDED(hr))
   {
      hr = m_pD2DContext->CreateBitmapFromWicBitmap(m_pWicSrcFormatConverter.Get(), NULL, &m_pSrcD2DBitmap);
   }

   // destination bitmap
   if (SUCCEEDED(hr))
   {
      hr = m_pWicFactory->CreateDecoderFromFilename(
         L"TransitionDestination.png",
         nullptr,
         GENERIC_READ,
         WICDecodeMetadataCacheOnDemand,
         &m_pWicDestBitmapDecoder);
   }

   if (SUCCEEDED(hr))
   {
      hr = m_pWicDestBitmapDecoder->GetFrame(0, &m_pWicDestBitmapFrameDecoder);
   }

   if (SUCCEEDED(hr))
   {
      hr = m_pWicFactory->CreateFormatConverter(&m_pWicDestFormatConverter);
   }

   if (SUCCEEDED(hr))
   {
      hr = m_pWicDestFormatConverter->Initialize(
         m_pWicDestBitmapFrameDecoder.Get(),
         GUID_WICPixelFormat32bppPBGRA,
         WICBitmapDitherTypeNone,
         nullptr,
         0.0f,
         WICBitmapPaletteTypeCustom
      );
   }

   if (SUCCEEDED(hr))
   {
      hr = m_pD2DContext->CreateBitmapFromWicBitmap(m_pWicDestFormatConverter.Get(), NULL, &m_pDestD2DBitmap);
   }

   CoUninitialize();
}

void CDirect2DTransitionDrawCtrl::CreateDeviceResources()
{
   if (m_customEffect != nullptr)
   {
      return;
   }

   HRESULT hr = CustomEffect::Register(m_pD2DFactory.Get());

   if (SUCCEEDED(hr))
   {
      hr = m_pD2DContext->CreateEffect(CLSID_CustomEffect, &m_customEffect);
   }

   if (SUCCEEDED(hr))
   {
      m_customEffect->SetInput(0, m_pSrcD2DBitmap.Get());
      m_customEffect->SetInput(1, m_pDestD2DBitmap.Get());
   }
}

void CDirect2DTransitionDrawCtrl::OnSize(UINT nType, int cx, int cy)
{
   CStatic::OnSize(nType, cx, cy);

   if (m_pSwapChain == nullptr || m_pD2DContext == nullptr)
   {
      return;
   }

   CRect rcCient;
   GetClientRect(rcCient);

   if (rcCient.IsRectEmpty())
   {
      return;
   }

   ID2D1Image* pImage = nullptr;
   m_pD2DContext->GetTarget(&pImage);
   m_pD2DContext->SetTarget(nullptr);
   if (pImage)
   {
      pImage->Release();
   }

   m_pTargetBitmap = nullptr;

   IDXGISurface* pBuffer = NULL;
   m_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBuffer));
   if (pBuffer)
   {
      int i = pBuffer->Release();
      while (i > 0)
         i = pBuffer->Release();
   }
   pBuffer = NULL;
   m_pSwapChain->GetBuffer(1, IID_PPV_ARGS(&pBuffer));
   if (pBuffer)
   {
      int i = pBuffer->Release();
      while (i > 0)
         i = pBuffer->Release();
   }

   HRESULT hr = m_pSwapChain->ResizeBuffers(0, rcCient.Width(), rcCient.Height(), DXGI_FORMAT_UNKNOWN, 0);

   if (FAILED(hr))
   {
      return;
   }

   FLOAT dpiX = 96.0;
   FLOAT dpiY = 96.0;
   // Direct2D needs the dxgi version of the backbuffer surface pointer.
   ComPtr<IDXGISurface> dxgiBackBuffer;
   hr = m_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&dxgiBackBuffer));
   if (hr != S_OK)
   {
      return;
   }
   // Create a Direct2D surface (bitmap) linked to the Direct3D texture back buffer via the DXGI back buffer
   //DON'T CHANGE THESE VALUES!!! Bitmap creation will fail with anything but these values.
   D2D1_BITMAP_PROPERTIES1 bitmapProperties =
      D2D1::BitmapProperties1(D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
         D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE), dpiX, dpiY);

   // Get a D2D surface from the DXGI back buffer to use as the D2D render target.

   hr = m_pD2DContext->CreateBitmapFromDxgiSurface(
      dxgiBackBuffer.Get(),
      &bitmapProperties,
      &m_pTargetBitmap);

   if (hr != S_OK)
   {
      return;
   }
}

void CDirect2DTransitionDrawCtrl::OnPaint()
{
   CPaintDC dc(this);

   CreateDeviceResources();

   CRect rcClient;
   GetClientRect(rcClient);

   D2D1_SIZE_F rectTargetBitmap = m_pTargetBitmap->GetSize();
   D2D1_SIZE_F rectDestBitmap = m_pDestD2DBitmap->GetSize();

   FLOAT fLeft = (rectTargetBitmap.width - rectDestBitmap.width) / 2.f;
   FLOAT fTop = (rectTargetBitmap.height - rectDestBitmap.height) / 2.f;

   D2D1_RECT_F rectDestination = D2D1::RectF(fLeft, fTop, rectDestBitmap.width, rectDestBitmap.height);
   D2D1_RECT_F rectSource = D2D1::RectF(0.f, 0.f, rectDestBitmap.width, rectDestBitmap.height);

   m_pD2DContext->BeginDraw();
   m_pD2DContext->SetTarget(m_pTargetBitmap.Get());

   m_pD2DContext->Clear(D2D1::ColorF(D2D1::ColorF::Black/*CornflowerBlue*/));

   // draw dest bitmap
   //m_pD2DContext->DrawBitmap(m_pDestD2DBitmap.Get(), rectDestination, 1.f, D2D1_INTERPOLATION_MODE_LINEAR, rectSource);
   if (m_customEffect != nullptr)
   {
      m_pD2DContext->DrawImage(m_customEffect.Get());
   }

   // We ignore D2DERR_RECREATE_TARGET here. This error indicates that the device
   // is lost. It will be handled during the next call to Present.
   HRESULT hr = m_pD2DContext->EndDraw();
   DXGI_PRESENT_PARAMETERS parameters = { 0 };
   parameters.DirtyRectsCount = 0;
   parameters.pDirtyRects = nullptr;
   parameters.pScrollRect = nullptr;
   parameters.pScrollOffset = nullptr;

   hr = m_pSwapChain->Present1(1, 0, &parameters);
}

void CDirect2DTransitionDrawCtrl::PreSubclassWindow()
{
   CreateDeviceIndependentResources();

   CStatic::PreSubclassWindow();
}
