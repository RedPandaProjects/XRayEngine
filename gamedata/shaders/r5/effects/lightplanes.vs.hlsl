#define USE_TRANSFORMATION_UNIFORM
#define USE_GLOBAL_UNIFORM
#include "common.h"
ROOT_SIGNATURE_GRAPHICS
PS_LIGHTPLANES_IN main(VS_R1Vertex_IN input)
{
    PS_LIGHTPLANES_IN output;
	
	////////////////////////////////////////////////////////////////
    output.position = input.position;
    output.position = mul(VPW, output.position);
    output.out_position_z = mul(WV, input.position).z;
	
	
	////////////////////////////////////////////////////////////////
    output.uv = unpack_tc_base(input.uv, float(input.tangent.w) / 255.f, float(input.binormal.w) / 255.f);
    
	////////////////////////////////////////////////////////////////
    input.normal.xyz = input.normal.zyx;
	
	////////////////////////////////////////////////////////////////
    float3 dir_v = normalize(mul(WV, input.position).xyz);
    float3 norm_v = normalize(mul(WV, float4(unpack_normal(float3(input.normal.xyz) / 255.f), 1)).xyz);
    output.color  = abs(dot(dir_v, norm_v));
	////////////////////////////////////////////////////////////////
    return output;
}
