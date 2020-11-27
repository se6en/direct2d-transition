#pragma once

#include <d2d1effectauthor.h>
#include <d2d1effecthelpers.h>
#include <math.h>
#include <initguid.h>

using namespace Microsoft::WRL;

DEFINE_GUID(GUID_CustomPixelShader, 0x4973595c, 0x2b06, 0x42fe, 0xa4, 0x5b, 0xa0, 0xa3, 0x4e, 0x2, 0x78, 0x7d);// {4973595C-2B06-42FE-A45B-A0A34E02787D}
DEFINE_GUID(CLSID_CustomEffect, 0x695d21c5, 0x1758, 0x4263, 0x92, 0x57, 0xe5, 0xe8, 0xbf, 0x46, 0xee, 0xe1);// {695D21C5-1758-4263-9257-E5E8BF46EEE1}

enum CUSTOM_PROP
{
   CUSTOM_PROP_PROGRESS = 0
};

class CustomEffect : public ID2D1EffectImpl, public ID2D1DrawTransform
{
public:
   // Declare effect registration methods.
   static HRESULT Register(_In_ ID2D1Factory1* pFactory);

   static HRESULT __stdcall CreateCustomImpl(_Outptr_ IUnknown** ppEffectImpl);

   // Declare property getter/setter methods.
   HRESULT SetProgress(float spread);
   float GetProgress() const;

   // Declare ID2D1EffectImpl implementation methods.
   IFACEMETHODIMP Initialize(
      _In_ ID2D1EffectContext* pContextInternal,
      _In_ ID2D1TransformGraph* pTransformGraph
   );

   IFACEMETHODIMP PrepareForRender(D2D1_CHANGE_TYPE changeType);

   IFACEMETHODIMP SetGraph(_In_ ID2D1TransformGraph* pGraph);

   // Declare ID2D1DrawTransform implementation methods.
   IFACEMETHODIMP SetDrawInfo(_In_ ID2D1DrawInfo* pRenderInfo);

   // Declare ID2D1Transform implementation methods.
   IFACEMETHODIMP MapOutputRectToInputRects(
      _In_ const D2D1_RECT_L* pOutputRect,
      _Out_writes_(inputRectCount) D2D1_RECT_L* pInputRects,
      UINT32 inputRectCount
   ) const;

   IFACEMETHODIMP MapInputRectsToOutputRect(
      _In_reads_(inputRectCount) CONST D2D1_RECT_L* pInputRects,
      _In_reads_(inputRectCount) CONST D2D1_RECT_L* pInputOpaqueSubRects,
      UINT32 inputRectCount,
      _Out_ D2D1_RECT_L* pOutputRect,
      _Out_ D2D1_RECT_L* pOutputOpaqueSubRect
   );

   IFACEMETHODIMP MapInvalidRect(
      UINT32 inputIndex,
      D2D1_RECT_L invalidInputRect,
      _Out_ D2D1_RECT_L* pInvalidOutputRect
   ) const;

   // Declare ID2D1TransformNode implementation methods.
   IFACEMETHODIMP_(UINT32) GetInputCount() const;

   // Declare IUnknown implementation methods.
   IFACEMETHODIMP_(ULONG) AddRef();
   IFACEMETHODIMP_(ULONG) Release();
   IFACEMETHODIMP QueryInterface(_In_ REFIID riid, _Outptr_ void** ppOutput);

private:
   CustomEffect();
   HRESULT UpdateConstants();

   inline static float Clamp(float v, float low, float high)
   {
      return (v < low) ? low : (v > high) ? high : v;
   }

   inline static float Round(float v)
   {
      return floor(v + 0.5f);
   }

   // Prevents over/underflows when adding longs.
   inline static long SafeAdd(long base, long valueToAdd)
   {
      if (valueToAdd >= 0)
      {
         return ((base + valueToAdd) >= base) ? (base + valueToAdd) : LONG_MAX;
      }
      else
      {
         return ((base + valueToAdd) <= base) ? (base + valueToAdd) : LONG_MIN;
      }
   }

   // This struct defines the constant buffer of our pixel shader.
   struct
   {
      float progress;
   } m_constants;

   Microsoft::WRL::ComPtr<ID2D1DrawInfo>      m_drawInfo;
   Microsoft::WRL::ComPtr<ID2D1EffectContext> m_effectContext;
   LONG                                       m_refCount;
   D2D1_RECT_L                                m_inputRect;
   float                                      m_dpi;
};