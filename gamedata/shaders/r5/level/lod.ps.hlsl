#define USE_GLOBAL_UNIFORM

#include "common.h"

BINDING_SRV_0 Texture2D t_base: register( t0 );
BINDING_SRV_1 Texture2D t_hemi: register( t1 );
BINDING_SAMPLER_0 SamplerState sdefault: register( s0 );


ROOT_SIGNATURE_GRAPHICS
PS_BASE_OUT main(PS_LOD_IN input)
{
    float4 base0 = t_base.Sample(sdefault, input.uv0);
    float hemi0 = t_hemi.Sample(sdefault, input.uv0).a;
    
    float4 base1 = t_base.Sample(sdefault, input.uv1);
    float hemi1 = t_hemi.Sample(sdefault, input.uv1).a;
    
    float4 base = lerp(base0, base1, input.factor) * input.color0;
    clip(base.a - (96.f / 255.f));
    
    float hemi = lerp(hemi0, hemi1, input.factor);
    
    float3 color = base.rgb * 2 * (0.5 + 0.5 * hemi);
    
    PS_BASE_OUT output;
    output.color = float4( lerp(FogColor.rgb, color.rgb, input.fog), 1);
    output.compute.z = input.out_position_z;
    output.compute.xyw = float3(1.f, 1.f, 1.f);
    output.bloom = float4(0, 0, 0, 0);
    return output;
}