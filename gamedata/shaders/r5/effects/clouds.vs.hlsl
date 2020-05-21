#define USE_TRANSFORMATION_UNIFORM
#define USE_GLOBAL_UNIFORM
#include "common.h"
struct VS_CLOUDS_IN
{
	LOCATION_0 float3 position : position_xyz;
	LOCATION_1 uint4 dir : diffuse;
	LOCATION_2 uint4 color : specular;
};

#define CLOUD_TILE0        (0.7f)
#define CLOUD_SPEED0       (2*0.05)//(0.033f)
#define CLOUD_TILE1        (2.8)//(2.5f)
#define CLOUD_SPEED1       (2*0.025)//(0.033f)

ROOT_SIGNATURE_GRAPHICS
PS_CLOUDS_IN main(VS_CLOUDS_IN input)
{
    PS_CLOUDS_IN output = (PS_CLOUDS_IN) 0;
    output.position = mul(VPW, float4(input.position, 1));
    input.color.bgr = input.color.rgb;
    input.dir.bgr = input.dir.rgb;
    
    float2 d0 = (float2(input.dir.xy) / 255.f) * 2 - 1;
    float2 d1 = (float2(input.dir.wz) / 255.f) * 2 - 1;
    output.uv1 = input.position.xz * CLOUD_TILE0 + d0 * Timer.z * CLOUD_SPEED0;
    output.uv2 = input.position.xz * CLOUD_TILE1 + d1 * Timer.z * CLOUD_SPEED1;

    output.color = float4(input.color) / 255.f;
    output.color.w *= pow(abs(input.position.y), 25);
	return output;
}