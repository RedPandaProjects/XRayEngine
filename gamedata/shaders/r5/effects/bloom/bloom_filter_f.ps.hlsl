#include "common.h"
BINDING_SRV_0 Texture2D s_image: register( t0 );
BINDING_SAMPLER_0 SamplerState s_default: register( s0 );

ROOT_SIGNATURE_GRAPHICS
float4 main(PS_BUILD_IN input) : SV_Target
{
    float4 s0 = s_image.Sample(s_default, input.uv0);
    float4 s1 = s_image.Sample(s_default, input.uv1);
    float4 s2 = s_image.Sample(s_default, input.uv2);
    float4 s3 = s_image.Sample(s_default, input.uv3);
    
    return ((s0 + s1) + (s2 + s3)) / 2;
}