#define USE_GLOBAL_UNIFORM
#define USE_TRANSFORMATION_UNIFORM
#include "common.h"

ROOT_SIGNATURE_GRAPHICS
PS_LMAPE_IN main(VS_R1LMap_IN input)
{

    PS_LMAPE_IN output;
	
	////////////////////////////////////////////////////////////////
    output.position = mul(VPW, input.position);
#ifndef USE_BLEND
    output.out_position_z = mul(WV, input.position).z;
#endif
		
	////////////////////////////////////////////////////////////////
    input.normal.xyz = input.normal.zyx;
    output.normal = unpack_normal(float3(input.normal.xyz) / 255.f);
    output.uv = unpack_tc_base(float2(input.uv[0], input.uv[1]), float(input.tangent.w) / 255.f, float(input.binormal.w) / 255.f);
		
	////////////////////////////////////////////////////////////////
    output.uv_map1.xy = unpack_tc_lmap(float2(input.uv[2], input.uv[3]));
    output.color1.xyz = v_hemi(output.normal);
    output.color2.xyz = v_sun(output.normal);
	////////////////////////////////////////////////////////////////
    output.uv_map2 = calc_reflection(input.position.xyz, output.normal);
    output.fog = calc_fogging(input.position);
	////////////////////////////////////////////////////////////////
    return output;
}