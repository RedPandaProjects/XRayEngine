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

struct VS_IN
{
	LOCATION_0
    float3 position : POS;
};
struct PS_IN
{
    float4 position : SV_POSITION;
};
