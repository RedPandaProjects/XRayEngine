#define USE_BLEND

#include "common.h"
BINDING_SRV_0 Texture2D t_base: register( t0 );
BINDING_SAMPLER_0 SamplerState s_default: register( s0 );


ROOT_SIGNATURE_GRAPHICS
PS_BASE_OUT main( PS_LIGHTPLANES_IN input)
{
    float4 base = t_base.Sample(s_default, input.uv);
    PS_BASE_OUT output;
    output.color = float4(base.rgb, base.a * input.color /** input.fog * input.fog*/);
    return output;
}