#define USE_TRANSFORMATION_UNIFORM
#include <common.h> 
struct VS_SKY_IN
{
	LOCATION_0 float3 position : position_xyz;
	LOCATION_1 uint4 diffuse : diffuse;
	LOCATION_2  float3 uv1 : uv1t;
	LOCATION_2  float3 uv2 : uv2t;
};

RegRootSignature(DeafultRS)
PS_SKY_IN main(VS_SKY_IN input)
{
    PS_SKY_IN output = (PS_SKY_IN) 0;
    output.position = mul(VPW, mul(float4(input.position, 1), 100));
    output.uv1 = input.uv1;
	return output;
}