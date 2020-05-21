#define USE_TRANSFORMATION_UNIFORM
#include "common.h"

ROOT_SIGNATURE_GRAPHICS
PS_TL_IN main(VS_LIT_IN input)
{
    PS_TL_IN output;
    output.position = mul(VP, float4(input.position, 1));
    output.uv = input.uv;
    input.diffuse.argb = input.diffuse.bgra;
    output.color = float4(input.diffuse) / 255.f;
	return output;
}