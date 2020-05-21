#define USE_DETAIL_UNIFORM
#define USE_GLOBAL_UNIFORM
#define USE_TRANSFORMATION_UNIFORM
#include "common.h"

ROOT_SIGNATURE_GRAPHICS
PS_TERRAIN_IN main(VS_R1LMap_IN input)
{
    PS_TERRAIN_IN res = (PS_TERRAIN_IN) 0;
    res.position = input.position;
    res.position = mul(VPW, input.position);
    res.out_position_z = mul(WV, input.position).z;
	
    input.normal.xyz = input.normal.zyx;
	float3 N = unpack_normal(float3(input.normal.xyz)/255.f);
	res.color1		= v_hemi		(N);				// hemi
	res.color2 		= v_sun			(N);  
	
    res.fog = calc_fogging(input.position);
 	res.uv1=unpack_tc_base(input.uv.xy,float(input.tangent.w)/255.f,float(input.binormal.w)/255.f);
    res.uv2 = DetailParams.x * res.uv1;
	return res;
}