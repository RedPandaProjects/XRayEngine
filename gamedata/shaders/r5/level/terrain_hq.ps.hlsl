#define USE_GLOBAL_UNIFORM
#include "common.h"
BINDING_SRV_0 Texture2D t_base: register( t0 );
BINDING_SRV_1 Texture2D t_lamp: register( t1 );
BINDING_SAMPLER_0 SamplerState sdefault: register( s0 );
BINDING_SRV_2 Texture2D t_detail: register( t2 );
/*BINDING_1 Texture2D t_mask: register( t3 );
BINDING_1 Texture2D t_r: register( t4 );
BINDING_1 Texture2D t_g: register( t5 );
BINDING_1 Texture2D t_b: register( t6 );
BINDING_1 Texture2D t_a: register( t7 );*/


ROOT_SIGNATURE_GRAPHICS
PS_BASE_OUT main(PS_TERRAIN_IN input) 
{
#ifndef MESH_SHADER_DEBUG

	float4 base = t_base.Sample(sdefault, input.uv1);
	float4 lmap = t_lamp.Sample(sdefault, input.uv1);

	/*float4 O =base;
	float4 mask = t_mask.Sample(sdefault, input.uv1);
	float4 c_r=t_r.Sample(sdefault, input.uv2)*mask.r;
	float4 c_g=t_g.Sample(sdefault, input.uv2)*mask.g;
	float4 c_b=t_b.Sample(sdefault, input.uv2)*mask.b;
	float4 c_a=t_a.Sample(sdefault, input.uv2)*mask.a;
	float4 mask_result = c_r+c_g+c_b+c_a;

	O.rgb=O.rgb*mask_result.rgb*2;

	;
	O.rgb=O.rgb*c_d.rgb*2;*/

    float3 l_base = lmap.rgb; // base light-map
	float3	l_hemi 	= input.color1.rgb * base.a;		// hemi is implicitly inside texture
	float3 	l_sun 	= input.color2.rgb * lmap.a;		// sun color
	float3	light	=AmbineColor.rgb + l_base + l_sun + l_hemi;

	float4 detail = t_detail.Sample(sdefault, input.uv2);

    float4 color = float4((light * base.rgb * 2) * detail.rgb * 2, 1.f);
	
	    PS_BASE_OUT output;
    output.color = float4(lerp(FogColor.rgb, color.rgb, input.fog), 1.f);
    output.compute.z = input.out_position_z;
    output.compute.xyw = float3(1.f, 1.f, 1.f);
    output.bloom = float4(0, 0, 0, 0);
    return output;
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
       
    output.compute.z = input.out_position_z;
    output.compute.xyw = float3(1.f, 1.f, 1.f);
    output.bloom = float4(0, 0, 0, 0);
    return output;
#endif
}