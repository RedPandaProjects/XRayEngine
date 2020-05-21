#define USE_GLOBAL_UNIFORM
#include "common.h"
BINDING_SRV_0 Texture2D s_image: register( t0 );
BINDING_SAMPLER_0 SamplerState s_default: register( s0 );

ROOT_SIGNATURE_GRAPHICS
float4 main(PS_BUILD_IN input) : SV_Target
{
    float3 s0 = s_image.Sample(s_default, input.uv0).rgb;
    float3 s1 = s_image.Sample(s_default, input.uv1).rgb;
    float3 s2 = s_image.Sample(s_default, input.uv2).rgb;
    float3 s3 = s_image.Sample(s_default, input.uv3).rgb;
    
    float3 avg = ((s0 + s1) + (s2 + s3)) / 2;
    float hi = dot(avg, 1.h) - BloomParams.x;
    return float4(avg, hi);
}