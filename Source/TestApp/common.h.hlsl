#ifdef VULKAN
#define LOCATION_0 [[vk::location(0)]]
#define LOCATION_1 [[vk::location(1)]]
#define LOCATION_2 [[vk::location(2)]]
#define LOCATION_3 [[vk::location(3)]]
#define LOCATION_4 [[vk::location(4)]]
#define LOCATION_5 [[vk::location(5)]]
#define LOCATION_6 [[vk::location(6)]]
#define LOCATION_7 [[vk::location(7)]]
#define LOCATION_8 [[vk::location(8)]]
#else
#define LOCATION_0 
#define LOCATION_1 
#define LOCATION_2 
#define LOCATION_3 
#define LOCATION_4 
#define LOCATION_5 
#define LOCATION_6 
#define LOCATION_7 
#define LOCATION_8
#endif

#ifdef VULKAN
#define BINDING_UNIFORM_0 [[vk::binding(0)]]
#define BINDING_UNIFORM_1 [[vk::binding(1)]]
#define BINDING_UNIFORM_2 [[vk::binding(2)]]
#define BINDING_UNIFORM_3 [[vk::binding(3)]]
#define BINDING_UNIFORM_4 [[vk::binding(4)]]
#define BINDING_UNIFORM_5 [[vk::binding(5)]]
#define BINDING_UNIFORM_6 [[vk::binding(6)]]
#define BINDING_UNIFORM_7 [[vk::binding(7)]]
#define BINDING_UNIFORM_8 [[vk::binding(8)]]
#define BINDING_UNIFORM_9 [[vk::binding(9)]]
#define BINDING_UNIFORM_10 [[vk::binding(10)]]
#define BINDING_UNIFORM_11 [[vk::binding(11)]]
#define BINDING_UNIFORM_12 [[vk::binding(12)]]


#define BINDING_SRV_0 [[vk::binding(0+16)]]
#define BINDING_SRV_1 [[vk::binding(1+16)]]
#define BINDING_SRV_2 [[vk::binding(2+16)]]
#define BINDING_SRV_3 [[vk::binding(3+16)]]
#define BINDING_SRV_4 [[vk::binding(4+16)]]
#define BINDING_SRV_5 [[vk::binding(5+16)]]
#define BINDING_SRV_6 [[vk::binding(6+16)]]
#define BINDING_SRV_7 [[vk::binding(7+16)]]


#define BINDING_UAV_0 [[vk::binding(0+32)]]
#define BINDING_UAV_1 [[vk::binding(1+32)]]
#define BINDING_UAV_2 [[vk::binding(2+32)]]
#define BINDING_UAV_3 [[vk::binding(3+32)]]
#define BINDING_UAV_4 [[vk::binding(4+32)]]
#define BINDING_UAV_5 [[vk::binding(5+32)]]
#define BINDING_UAV_6 [[vk::binding(6+32)]]
#define BINDING_UAV_7 [[vk::binding(7+32)]]

#define BINDING_SAMPLER_0  [[vk::binding(0+48)]]
#define BINDING_SAMPLER_1 [[vk::binding(1+48)]]
#define BINDING_SAMPLER_2 [[vk::binding(2+48)]]
#define BINDING_SAMPLER_3  [[vk::binding(3+48)]]
#else
#define BINDING_UNIFORM_0
#define BINDING_UNIFORM_1
#define BINDING_UNIFORM_2 
#define BINDING_UNIFORM_3 
#define BINDING_UNIFORM_4 
#define BINDING_UNIFORM_5 
#define BINDING_UNIFORM_6 
#define BINDING_UNIFORM_7 
#define BINDING_UNIFORM_8 
#define BINDING_UNIFORM_9 
#define BINDING_UNIFORM_10 
#define BINDING_UNIFORM_11 
#define BINDING_UNIFORM_12 

#define BINDING_UAV_0
#define BINDING_UAV_1 
#define BINDING_UAV_2
#define BINDING_UAV_3
#define BINDING_UAV_4
#define BINDING_UAV_5
#define BINDING_UAV_6 
#define BINDING_UAV_7

#define BINDING_SRV_0 
#define BINDING_SRV_1
#define BINDING_SRV_2 
#define BINDING_SRV_3 
#define BINDING_SRV_4 
#define BINDING_SRV_5 
#define BINDING_SRV_6 
#define BINDING_SRV_7 

#define BINDING_SAMPLER_0  
#define BINDING_SAMPLER_1
#define BINDING_SAMPLER_2 
#define BINDING_SAMPLER_3 
#endif

struct VS_IN
{
	LOCATION_0
    float3 position : POS;
};
struct PS_IN
{
    float4 position : SV_POSITION;
};
struct VS_TEXTURE_IN
{
	LOCATION_0   float3 position : POS;
    LOCATION_1   float2 uv : UV;
};
struct PS_TEXTURE_IN
{
    float4 position : SV_POSITION;
    LOCATION_0 float2 uv : UV;
};
struct HitInfo
{
    LOCATION_0 float4 colorAndDistance;
};
struct Attributes
{
    LOCATION_0 float2 bary;
};