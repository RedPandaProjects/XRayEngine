#define USE_GLOBAL_UNIFORM
#include "common.h"
BINDING_SRV_0 Texture2D s_base: register( t0 );
BINDING_SRV_1 Texture2D s_bloom: register( t1 );
//BINDING_0 Texture2D s_sky: register( t2 );
BINDING_SAMPLER_0 SamplerState s_default1: register( s0 );
BINDING_SAMPLER_1 SamplerState s_default2: register( s1 );
ROOT_SIGNATURE_GRAPHICS
float4 main(PS_TL_IN input) : SV_Target
{
    float4 base = s_base.Sample(s_default1, input.uv);
    float4 bloom = s_bloom.Sample(s_default2, input.uv);
    return float4(base.rgb + bloom.rgb * bloom.a, 1);

}