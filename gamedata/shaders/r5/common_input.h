struct VS_R1Vertex_IN
{
	LOCATION_0 float4 position : position;
	LOCATION_1 uint4 normal : normal;
	LOCATION_2 uint4 tangent : tangent;
	LOCATION_3 uint4 binormal : binormal;
	LOCATION_4 uint4 color : color;
	LOCATION_5 int2 uv : uv1t;
};
struct VS_R1LMap_IN
{
	LOCATION_0 float4 position : position;
	LOCATION_1 uint4 normal : normal;
	LOCATION_2 uint4 tangent : tangent;
	LOCATION_3 uint4 binormal : binormal;
	LOCATION_4 int4 uv : uv1t;
};
struct VS_TL_IN
{
	LOCATION_0 float4 position : position_xyzrhw;
	LOCATION_1 uint4 diffuse : diffuse;
	LOCATION_2  float2 uv : uv1t;
};
struct VS_LIT_IN
{
	LOCATION_0 float3 position : position_xyz;
	LOCATION_1 uint4 diffuse : diffuse;
	LOCATION_2  float2 uv : uv1t;
};
struct VS_TL0uv_IN
{
	LOCATION_0 float4 position : position_xyzrhw;
	LOCATION_1 uint4 diffuse : diffuse;
};
struct VS_M_IN
{
	LOCATION_0 float3 position : position_xyz;
	LOCATION_1 float3 normal : normal;
	LOCATION_2 float2 uv : uv1t;
};
struct VS_MUModel_IN
{
	LOCATION_0 float4 position : position;
	LOCATION_1 uint4 normal : normal;
	LOCATION_2 uint4 tangent : tangent;
	LOCATION_3 uint4 binormal : binormal;
	LOCATION_4 int4 uv : uv1t;
};

struct VS_LOD_IN
{
	LOCATION_0 float3 position0 : position0t;
	LOCATION_1 float3 position1 : position1t;
	LOCATION_2 float3 normal0 : normal0t;
	LOCATION_3 float3 normal1 : normal1t;
	LOCATION_4 uint4 sun_af : sun_af;
	LOCATION_5 float2 uv0 : uv0t;
	LOCATION_6 float2 uv1 : uv1t;
	LOCATION_7 uint4 rgbh0 : rgbh0t;
	LOCATION_8 uint4 rgbh1 : rgbh1t;
};
struct	VS_BUILD_IN
{
	LOCATION_0 float4	position : position;
	LOCATION_1 float2	uv0 : uv0t;
	LOCATION_2 float2	uv1 : uv1t;
	LOCATION_3 float2 	uv2 : uv2t;
	LOCATION_4 float2	uv3 : uv3t;
};
struct	VS_FILTER_IN
{
	LOCATION_0 float4	position : position;
	LOCATION_1 float4	uv0 : uv0t;
	LOCATION_2 float4	uv1 : uv1t;
	LOCATION_3 float4 	uv2 : uv2t;
	LOCATION_4 float4	uv3 : uv3t;
	LOCATION_5 float4	uv4 : uv4t;
	LOCATION_6 float4	uv5 : uv5t;
	LOCATION_7 float4 	uv6 : uv6t;
	LOCATION_8 float4	uv7 : uv7t;
};