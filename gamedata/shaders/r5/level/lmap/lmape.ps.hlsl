#define USE_GLOBAL_UNIFORM
#include "common.h"
BINDING_SRV_0 Texture2D t_base : register(t0);
BINDING_SRV_1 Texture2D t_lmap : register(t1);
BINDING_SRV_2 Texture2D t_hemi : register(t2);
BINDING_SRV_3 TextureCube t_env: register( t3 );
BINDING_SAMPLER_0 SamplerState s_default : register(s0);


ROOT_SIGNATURE_GRAPHICS
PS_BASE_OUT main(PS_LMAPE_IN input)
{
    float4 base = t_base.Sample(s_default, input.uv);

    float4 lmap = t_lmap.Sample(s_default, input.uv_map1.xy);
    float4 hemi = t_hemi.Sample(s_default, input.uv_map1.xy);
    
    float4 env = t_env.Sample(s_default, input.uv_map2);
	
    float3 l_base = lmap.rgb; // base light-map
    float3 l_hemi = input.color1.rgb * hemi.a; // hemi is implicitly inside texture
    float3 l_sun = input.color2.rgb * lmap.a; // sun color
    float3 light = AmbineColor.rgb + l_base + l_sun + l_hemi;

    
    float3 cbase = lerp(env.rgb, base.rgb, base.a);
    
    float3 result = light * cbase * 2;
	

	
    PS_BASE_OUT output;
    output.color = float4(lerp(FogColor.rgb, result, input.fog), base.a);
#ifndef USE_BLEND
    output.compute.z = input.out_position_z;
    output.compute.xyw = float3(1.f, 1.f, 1.f);
    output.bloom = float4(0, 0, 0, 0);
#endif
    return output;
}