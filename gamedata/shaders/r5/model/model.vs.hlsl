#define USE_TRANSFORMATION_UNIFORM
#include "default_model.h"

ROOT_SIGNATURE_GRAPHICS
PS_BASE_IN main(MODEL_IN IN)
{
	PS_BASE_IN output = (PS_BASE_IN) 0;
	v_model input = main_model(IN);
	
#ifndef USE_BLEND
    output.out_position_z = mul(WV, input.position).z;
#endif
	output.position = mul(VPW, input.position);
	output.uv = input.uv;
	return output;

}