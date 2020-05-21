#define USE_GLOBAL_UNIFORM
#include "common.h"

BINDING_SRV_0 TextureCube t_base0: register( t0 );
BINDING_SRV_1 TextureCube t_base1: register( t1 );
BINDING_SAMPLER_0 SamplerState sampler1: register( s0 );

ROOT_SIGNATURE_GRAPHICS
float4 main(PS_SKY_IN input) : SV_Target
{
    float4 base0 = t_base0.Sample(sampler1, input.uv);
    float4 base1 = t_base1.Sample(sampler1, input.uv);
    float3 sky = input.color.rgb * lerp(base0.rgb, base1.rgb, input.color.w);
    return float4(sky, 1.f);

}