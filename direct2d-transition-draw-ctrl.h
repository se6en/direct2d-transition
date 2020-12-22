#pragma once

using namespace Microsoft::WRL;

class CDirect2DTransitionDrawCtrl : public CStatic
{
   DECLARE_DYNAMIC(CDirect2DTransitionDrawCtrl)

public:
   CDirect2DTransitionDrawCtrl();
   virtual ~CDirect2DTransitionDrawCtrl();

   afx_msg void OnSize(UINT nType, int cx, int cy);
   afx_msg void OnPaint();

   virtual void PreSubclassWindow();

   DECLARE_MESSAGE_MAP()

public:
   void SetProgress(float fProgress);

private:
   void CreateDeviceIndependentResources();
   void CreateDeviceResources();

private:
   ComPtr<ID2D1Factory1>               m_pD2DFactory;
   ComPtr<ID2D1Device>                 m_pDevice;
   ComPtr<ID2D1DeviceContext>          m_pD2DContext;
   ComPtr<IDXGISwapChain1>             m_pSwapChain;
   ComPtr<ID2D1Bitmap1>                m_pTargetBitmap;

   ComPtr<IWICImagingFactory2>         m_pWicFactory;

   ComPtr<IWICBitmapDecoder>           m_pWicSrcBitmapDecoder;
   ComPtr<IWICBitmapFrameDecode>       m_pWicSrcBitmapFrameDecoder;
   ComPtr<IWICFormatConverter>         m_pWicSrcFormatConverter;
   ComPtr<ID2D1Bitmap1>                m_pSrcD2DBitmap;

   ComPtr<IWICBitmapDecoder>           m_pWicDestBitmapDecoder;
   ComPtr<IWICBitmapFrameDecode>       m_pWicDestBitmapFrameDecoder;
   ComPtr<IWICFormatConverter>         m_pWicDestFormatConverter;
   ComPtr<ID2D1Bitmap1>                m_pDestD2DBitmap;

   ComPtr<IWICBitmapDecoder>           m_pWicMaskBitmapDecoder;
   ComPtr<IWICBitmapFrameDecode>       m_pWicMaskBitmapFrameDecoder;
   ComPtr<IWICFormatConverter>         m_pWicMaskFormatConverter;
   ComPtr<ID2D1Bitmap1>                m_pMaskD2DBitmap;

   ComPtr<IWICBitmapDecoder>           m_pWicElementBitmapDecoder;
   ComPtr<IWICBitmapFrameDecode>       m_pWicElementBitmapFrameDecoder;
   ComPtr<IWICFormatConverter>         m_pWicElementFormatConverter;
   ComPtr<ID2D1Bitmap1>                m_pElementD2DBitmap;

   ComPtr<ID2D1Effect>                 m_effectTransitionMask;
};