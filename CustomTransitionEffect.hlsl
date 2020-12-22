Texture2D SourceTexture : register(t0);
SamplerState SourceSampler : register(s0);

Texture2D MaskTexture : register(t1);
SamplerState MaskSampler : register(s1);

Texture2D ElementTexture : register(t2);
SamplerState ElementSampler : register(s2);

Texture2D DestinationTexture : register(t3);
SamplerState DestinationSampler : register(s3);

cbuffer constants : register(b0)
{
   float progress;
};

float4 main(
   float4 clipSpaceOutput  : SV_POSITION,
   float4 sceneSpaceOutput : SCENE_POSITION,
   float4 texelSpaceInput0 : TEXCOORD0,
   float4 texelSpaceInput1 : TEXCOORD1,
   float4 texelSpaceInput2 : TEXCOORD2,
   float4 texelSpaceInput3 : TEXCOORD3
) : SV_Target
{
   float4 sourcecolor = SourceTexture.Sample(
      SourceSampler,
      float2 (texelSpaceInput0.xy));

   sourcecolor.r /= sourcecolor.a;
   sourcecolor.g /= sourcecolor.a;
   sourcecolor.b /= sourcecolor.a;

   float4 maskcolor = MaskTexture.Sample(
      MaskSampler,
   float2 (texelSpaceInput1.xy));

   float4 elementcolor = ElementTexture.Sample(
      ElementSampler,
      float2 (texelSpaceInput2.xy));

   elementcolor.r /= elementcolor.a;
   elementcolor.g /= elementcolor.a;
   elementcolor.b /= elementcolor.a;

   float4 destinationcolor = DestinationTexture.Sample(
      DestinationSampler,
      float2 (texelSpaceInput3.xy));

   destinationcolor.r /= destinationcolor.a;
   destinationcolor.g /= destinationcolor.a;
   destinationcolor.b /= destinationcolor.a;

   float4 color = sourcecolor;

   // 1. combine source and destination as background
   float4 backgroundcolor;
   backgroundcolor.r = destinationcolor.r * maskcolor.r + sourcecolor.r  * (1 - maskcolor.r);
   backgroundcolor.g = destinationcolor.g * maskcolor.r + sourcecolor.g  * (1 - maskcolor.r);
   backgroundcolor.b = destinationcolor.b * maskcolor.r + sourcecolor.b  * (1 - maskcolor.r);
   backgroundcolor.a = destinationcolor.a * maskcolor.r + sourcecolor.a  * (1 - maskcolor.r);

   //backgroundcolor *= progress;

   /*backgroundcolor.r /= backgroundcolor.a;
   backgroundcolor.g /= backgroundcolor.a;
   backgroundcolor.b /= backgroundcolor.a;*/

   // 2. element with mask
   float4 elementmaskcolor;
   elementmaskcolor.r = elementcolor.r * maskcolor.g * (1 - progress);
   elementmaskcolor.g = elementcolor.g * maskcolor.g  * (1 - progress);
   elementmaskcolor.b = elementcolor.b * maskcolor.g * (1 - progress);
   elementmaskcolor.a = elementcolor.a * maskcolor.g * (1 - progress);

   /*elementmaskcolor.r *= elementmaskcolor.a;
   elementmaskcolor.g *= elementmaskcolor.a;
   elementmaskcolor.b *= elementmaskcolor.a;*/

   if (elementmaskcolor.a <= 0.000001f)
   {
      color = backgroundcolor;

      /*color.r *= color.a;
      color.g *= color.a;
      color.b *= color.a;*/
   }
   else
   {
      /*backgroundcolor.r /= backgroundcolor.a;
      backgroundcolor.g /= backgroundcolor.a;
      backgroundcolor.b /= backgroundcolor.a;*/

      /*elementmaskcolor.r *= elementmaskcolor.a;
      elementmaskcolor.g *= elementmaskcolor.a;
      elementmaskcolor.b *= elementmaskcolor.a;*/
      backgroundcolor.a = 1.f - elementmaskcolor.a;

      color.rgb = elementmaskcolor.a * elementmaskcolor.rgb + backgroundcolor.a * backgroundcolor.rgb;
      color.a = 1.f;

      //color = (1 - progress) * elementmaskcolor + progress * backgroundcolor;
   }

      /*color.r *= color.a;
      color.g *= color.a;
      color.b *= color.a;*/


   return color;
}

//float4 main(
//   float4 clipSpaceOutput  : SV_POSITION,
//   float4 sceneSpaceOutput : SCENE_POSITION,
//   float4 texelSpaceInput0 : TEXCOORD0,
//   float4 texelSpaceInput1 : TEXCOORD1,
//   float4 texelSpaceInput2 : TEXCOORD2,
//   float4 texelSpaceInput3 : TEXCOORD3
//) : SV_Target
//{
//   float4 sourcecolor = SourceTexture.Sample(
//      SourceSampler,
//      float2 (texelSpaceInput0.xy));
//
//   sourcecolor.r /= sourcecolor.a;
//   sourcecolor.g /= sourcecolor.a;
//   sourcecolor.b /= sourcecolor.a;
//
//   float4 maskcolor = MaskTexture.Sample(
//      MaskSampler,
//   float2 (texelSpaceInput1.xy));
//
//   maskcolor.r /= maskcolor.a;
//   maskcolor.g /= maskcolor.a;
//   maskcolor.b /= maskcolor.a;
//
//   float4 elementcolor = ElementTexture.Sample(
//      ElementSampler,
//      float2 (texelSpaceInput2.xy));
//
//   elementcolor.r /= elementcolor.a;
//   elementcolor.g /= elementcolor.a;
//   elementcolor.b /= elementcolor.a;
//
//   float4 destinationcolor = DestinationTexture.Sample(
//      DestinationSampler,
//      float2 (texelSpaceInput3.xy));
//
//   destinationcolor.r /= destinationcolor.a;
//   destinationcolor.g /= destinationcolor.a;
//   destinationcolor.b /= destinationcolor.a;
//
//   float4 color = sourcecolor;
//
//   // 1. combine source and destination as background
//   float4 backgroundcolor;
//   backgroundcolor.r = destinationcolor.r * maskcolor.r + sourcecolor.r  * (1 - maskcolor.r);
//   backgroundcolor.g = destinationcolor.g * maskcolor.r + sourcecolor.g  * (1 - maskcolor.r);
//   backgroundcolor.b = destinationcolor.b * maskcolor.r + sourcecolor.b  * (1 - maskcolor.r);
//   backgroundcolor.a = destinationcolor.a * maskcolor.r + sourcecolor.a  * (1 - maskcolor.r);
//
//   backgroundcolor.r *= backgroundcolor.a;
//   backgroundcolor.g *= backgroundcolor.a;
//   backgroundcolor.b *= backgroundcolor.a;
//
//   // 2. element with mask
//   float4 elementmaskcolor;
//
//   if (maskcolor.g < 0.5f)
//   {
//      elementmaskcolor = float4(0.f, 0.f, 0.f, 0.f);
//   }
//   else
//   {
//      elementmaskcolor = elementcolor;
//   }
//
//   elementmaskcolor.r *= elementmaskcolor.a;
//   elementmaskcolor.g *= elementmaskcolor.a;
//   elementmaskcolor.b *= elementmaskcolor.a;
//
//   if (elementmaskcolor.a <= 0.000001f)
//   {
//      color = backgroundcolor;
//
//      color.r *= color.a;
//      color.g *= color.a;
//      color.b *= color.a;
//   }
//   else
//   {
//      color = (1 - progress) * elementmaskcolor + progress * backgroundcolor;
//   }
//
//   return color;
//}
