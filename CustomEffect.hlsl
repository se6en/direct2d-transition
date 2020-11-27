Texture2D SourceTexture : register(t0);
SamplerState SourceSampler : register(s0);

Texture2D DestinationTexture : register(t1);
SamplerState DestinationSampler : register(s1);

cbuffer constants : register(b0)
{
   float progress;
};

float4 main(
   float4 clipSpaceOutput  : SV_POSITION,
   float4 sceneSpaceOutput : SCENE_POSITION,
   float4 texelSpaceInput0 : TEXCOORD0,
   float4 texelSpaceInput1 : TEXCOORD1
) : SV_Target
{
   float4 color = SourceTexture.Sample(
      SourceSampler,          // Sampler and Texture must match for a given input.
      float2 (texelSpaceInput0.xy)
   );

   if (texelSpaceInput0.x > progress)
   {
      // source
      float distance = texelSpaceInput0.x - progress;

      color = SourceTexture.Sample(
         SourceSampler,          // Sampler and Texture must match for a given input.
         float2 (distance, texelSpaceInput0.y)
      );

   }
   else
   {
      // destination
      float distance = 1.f - progress + texelSpaceInput1.x;

      color = DestinationTexture.Sample(
         DestinationSampler,          // Sampler and Texture must match for a given input.
         float2 (distance, texelSpaceInput1.y)
      );
   }

   return color;
}
