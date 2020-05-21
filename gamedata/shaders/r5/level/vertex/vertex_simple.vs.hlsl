
#define USE_TRANSFORMATION_UNIFORM
#include "common.h"
ROOT_SIGNATURE_GRAPHICS
PS_SIMPLE_IN main(VS_R1Vertex_IN input)
{

    PS_SIMPLE_IN output;
	
	////////////////////////////////////////////////////////////////
    output.position = input.position;
    output.position = mul(VPW, output.position);
    output.out_position_z = mul(WV, input.position).z;
	
	////////////////////////////////////////////////////////////////
    output.uv = unpack_tc_base(input.uv, float(input.tangent.w) / 255.f, float(input.binormal.w) / 255.f);
	
	////////////////////////////////////////////////////////////////
    return output;
}
