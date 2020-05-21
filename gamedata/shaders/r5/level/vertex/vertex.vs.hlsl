#define USE_TRANSFORMATION_UNIFORM
#define USE_GLOBAL_UNIFORM
#include "common.h"

ROOT_SIGNATURE_GRAPHICS
PS_BASE_IN main(VS_R1Vertex_IN input  )
{

    PS_BASE_IN output;
	
	////////////////////////////////////////////////////////////////
	output.position = input.position;
	output.position = mul(VPW, output.position);
#ifndef USE_BLEND
    output.out_position_z = mul(WV, input.position).z;
#endif
	
	////////////////////////////////////////////////////////////////
    input.normal.xyz = input.normal.zyx;
	output.normal = unpack_normal(float3(input.normal.xyz) / 255.f);
	
	
	////////////////////////////////////////////////////////////////
	output.uv = unpack_tc_base(input.uv, float(input.tangent.w) / 255.f, float(input.binormal.w) / 255.f);
	
	
	
	////////////////////////////////////////////////////////////////
	float3 L_rgb = float3(input.color.bgr)/255.f; // precalculated RGB lighting
	float3 L_hemi = v_hemi(output.normal) * (float(input.normal.w) / 255.f); // hemisphere
	float3 L_sun = v_sun(output.normal) * (float(input.color.w) / 255.f); // sun
	output.color1 =  L_rgb + L_hemi + L_sun + AmbineColor.rgb;
	
	
	////////////////////////////////////////////////////////////////
    output.fog = calc_fogging(input.position);
	
	
	////////////////////////////////////////////////////////////////
#ifdef USE_DETAIL
    output.uv_detail.xy = output.uv * DetailParams.x;
    output.uv_detail.zw = calc_detail(input.position.xyz);
#endif
	
	////////////////////////////////////////////////////////////////
	return output;
}
