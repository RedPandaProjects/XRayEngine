#define USE_GLOBAL_UNIFORM
#define USE_TRANSFORMATION_UNIFORM
#include "common.h"

ROOT_SIGNATURE_GRAPHICS
PS_LMAP_IN main(VS_R1LMap_IN input  )
{

    PS_LMAP_IN   output;
	
	////////////////////////////////////////////////////////////////
	output.position = mul(VPW, input.position);
#ifndef USE_BLEND
    output.out_position_z = mul(WV, input.position).z;
#endif
		
	////////////////////////////////////////////////////////////////
    input.normal.xyz = input.normal.zyx;
	output.normal = unpack_normal(float3(input.normal.xyz) / 255.f);
	output.uv = unpack_tc_base(input.uv.xy, float(input.tangent.w) / 255.f, float(input.binormal.w) / 255.f);
		
	////////////////////////////////////////////////////////////////
	output.uv_map.xy = unpack_tc_lmap(input.uv.zw);
	output.color1.xyz = v_hemi(output.normal);
    output.color2.xyz = v_sun(output.normal);
	////////////////////////////////////////////////////////////////
#ifdef MESH_SHADER_DEBUG
    output.MeshletIndex = 0; 
#else
    output.fog = calc_fogging(input.position);
	////////////////////////////////////////////////////////////////
#ifdef USE_DETAIL
    output.uv_map.zw = output.uv * DetailParams.x;
    float2 dt = calc_detail(input.position.xyz);
    output.color1.a = dt.x;
    output.color2.a = dt.y;
#endif
#endif
	////////////////////////////////////////////////////////////////
	return output;
}