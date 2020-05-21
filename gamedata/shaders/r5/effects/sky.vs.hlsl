#define USE_TRANSFORMATION_UNIFORM
#include "common.h"
struct VS_SKY_IN
{
	LOCATION_0 float3 position : position_xyz;
	LOCATION_1 uint4 diffuse : diffuse;
	LOCATION_2  float3 uv1 : uv1t;
	LOCATION_3  float3 uv2 : uv2t;
};

ROOT_SIGNATURE_GRAPHICS
PS_SKY_IN main(VS_SKY_IN input)
{
    PS_SKY_IN output = (PS_SKY_IN) 0;
    output.position = mul(VPW, mul(float4(input.position, 1), 100.f));
    output.uv = input.uv1;
    input.diffuse.bgr = input.diffuse.rgb;
    output.color = float4(input.diffuse) / 255.f;
	return output;
}