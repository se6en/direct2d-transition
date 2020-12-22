#pragma once

#include <d2d1effectauthor.h>
#include <d2d1effecthelpers.h>
#include <math.h>
#include <initguid.h>

using namespace Microsoft::WRL;

DEFINE_GUID(GUID_CustomTransitionPixelShader, 0xd0fc1d6b, 0x5af8, 0x4152, 0x8b, 0x3e, 0x75, 0x5a, 0x99, 0x8c, 0x3f, 0x8e);// {D0FC1D6B-5AF8-4152-8B3E-755A998C3F8E}
DEFINE_GUID(CLSID_CustomTransitionEffect, 0xaf1dd7fd, 0x4de3, 0x46d7, 0xa8, 0xc1, 0x1f, 0xd6, 0x4e, 0x5c, 0xc5, 0xd4);// {AF1DD7FD-4DE3-46D7-A8C1-1FD64E5CC5D4}

enum CUSTOM_TRANSITION_PROP
{
   CUSTOM_TRANSITION_ALPHA = 0
};

class CustomTransitionEffect : public ID2D1EffectImpl, public ID2D1DrawTransform
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
   CustomTransitionEffect();
   HRESULT UpdateConstants();

   inline static float Clamp(float v, float low, float high)
   {
      return (v < low) ? low : (v > high) ? high : v;
   }

   inline static float Round(float v)
   {
      return floorf(v + 0.5f);
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