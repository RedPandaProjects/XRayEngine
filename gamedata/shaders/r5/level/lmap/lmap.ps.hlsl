#define USE_GLOBAL_UNIFORM
#include "common.h"

BINDING_SRV_0 Texture2D t_base: register( t0 );
BINDING_SRV_1 Texture2D t_lmap: register( t1 );
BINDING_SRV_2 Texture2D t_hemi: register( t2 );

#ifdef USE_DETAIL
BINDING_SRV_3 Texture2D t_detail: register( t3 );
#endif

BINDING_SAMPLER_0 SamplerState s_default: register( s0 );

ROOT_SIGNATURE_GRAPHICS
PS_BASE_OUT main(PS_LMAP_IN input) 
{
#ifndef MESH_SHADER_DEBUG
    float4 base = t_base.Sample(s_default, input.uv);
#ifdef USE_AREF
    clip(base.w-AlphaRef);
#endif
    float4 lmap = t_lmap.Sample(s_default, input.uv_map.xy);
    float4 hemi = t_hemi.Sample(s_default, input.uv_map.xy);
	
	float3 l_base = lmap.rgb; // base light-map
	float3 l_hemi = input.color1.rgb * hemi.a; // hemi is implicitly inside texture
    float3 l_sun = input.color2.rgb * lmap.a; // sun color
	float3 light = AmbineColor.rgb + l_base + l_sun + l_hemi;

    float3 result = light * base.rgb * 2;
	
#ifdef USE_DETAIL
    float3 detail = t_detail.Sample(s_default, input.uv_map.zw).rgb;
    detail = detail * input.color1.a + input.color2.a;
    result = result * detail * 2;
#endif
	
	
    PS_BASE_OUT output;
    output.color = float4(lerp(FogColor.rgb, result, input.fog), base.a);
#else
    PS_BASE_OUT output;
     uint meshletIndex = input.MeshletIndex;
    if (meshletIndex == 0)
    {
        output.color = float4(1, 1, 1, 1);

    }
    else
    {
        output.color = float4(float3(
            float(meshletIndex & 1),
            float(meshletIndex & 3) / 4,
            float(meshletIndex & 7) / 8),
    1.f);
    }
#endif
#ifndef USE_BLEND
    output.compute.z = input.out_position_z;
    output.compute.xyw = float3(1.f, 1.f, 1.f);
    output.bloom = float4(0, 0, 0, 0);
#endif
    return output;
}