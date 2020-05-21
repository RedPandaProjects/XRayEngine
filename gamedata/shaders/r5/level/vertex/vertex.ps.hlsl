#define USE_GLOBAL_UNIFORM
#include "common.h"


BINDING_SRV_0 Texture2D t_base: register( t0 );
BINDING_SAMPLER_0 SamplerState s_default: register( s0 );


#ifdef USE_DETAIL
BINDING_SRV_1 Texture2D t_detail: register( t1 );
#endif

ROOT_SIGNATURE_GRAPHICS
PS_BASE_OUT main(PS_BASE_IN input)
{
    float4 base = t_base.Sample(s_default, input.uv);
#ifdef USE_AREF
    clip(base.w-AlphaRef);
#endif
    float3 result = base.rgb * input.color1 * 2;
#ifdef USE_DETAIL
    float3 detail = t_detail.Sample(s_default, input.uv_detail.xy).rgb;
    detail = detail * input.uv_detail.z + input.uv_detail.w;
    result = result * detail * 2;
#endif
    PS_BASE_OUT output;
    output.color = float4( lerp(FogColor.rgb, result, input.fog), base.a /** input.fog * input.fog*/);
    
#ifndef USE_BLEND
    output.compute.z = input.out_position_z;
    output.compute.xyw = float3(1.f, 1.f, 1.f);
    output.bloom = float4(0, 0, 0, 0);
#endif
    return output;
}
