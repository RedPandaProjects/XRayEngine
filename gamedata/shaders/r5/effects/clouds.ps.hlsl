#define USE_GLOBAL_UNIFORM
#include "common.h"

BINDING_SRV_0 Texture2D s_clouds0: register( t0 );
BINDING_SRV_1 Texture2D s_clouds1: register( t1 );
BINDING_SAMPLER_0 SamplerState sampler1: register( s0 );

ROOT_SIGNATURE_GRAPHICS
float4 main(PS_CLOUDS_IN input) : SV_Target
{
    float4 base0 = s_clouds0.Sample(sampler1, input.uv1);
    float4 base1 = s_clouds1.Sample(sampler1, input.uv2);
    float4 mix = input.color * (base0 + base1);
    return mix;

}