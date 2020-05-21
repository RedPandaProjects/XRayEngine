/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct	PS_BUILD_IN
{
	LOCATION_0 float4	position : SV_Position;
	LOCATION_1 float2	uv0 : uv0t;
	LOCATION_2 float2	uv1 : uv1t;
	LOCATION_3 float2 	uv2 : uv2t;
	LOCATION_4 float2	uv3 : uv3t;
};
struct	PS_FILTER_IN
{
	LOCATION_0 float4	position : SV_Position;
	LOCATION_1 float4	uv0 : uv0t;
	LOCATION_2 float4	uv1 : uv1t;
	LOCATION_3 float4 	uv2 : uv2t;
	LOCATION_4 float4	uv3 : uv3t;
	LOCATION_5 float4	uv4 : uv4t;
	LOCATION_6 float4	uv5 : uv5t;
	LOCATION_7 float4 	uv6 : uv6t;
	LOCATION_8 float4	uv7 : uv7t;
};
struct PS_TL_IN
{
	float4 position : SV_POSITION;
	LOCATION_0 float2 uv : UV;
	LOCATION_1 float4 color : COLOR;
};
struct PS_V_IN
{
	float4 position : SV_POSITION;
	LOCATION_0 float2 uv : UV;
};
struct PS_TL0uv_IN
{
	float4 position : SV_POSITION;
	LOCATION_1 float4 color : COLORA;
};

#ifdef USE_DETAIL
struct PS_BASE_IN
{
	float4 position : SV_POSITION;
	LOCATION_0 float2 uv : UV;
	LOCATION_1 float3 normal : NORMAL;
	LOCATION_2 float3 color1 : COLOR1;
	LOCATION_3 float fog : FOG;
	LOCATION_4 float4 uv_detail : UVDETAIL;
#ifndef USE_BLEND
	LOCATION_5 float out_position_z : OUT_POSITIONZ;
#endif
#ifdef MESH_SHADER_DEBUG
	LOCATION_6 uint MeshletIndex : MESHLETINDEX;
#endif
};
#else
struct PS_BASE_IN
{
	float4 position : SV_POSITION;
	LOCATION_0 float2 uv : UV;
	LOCATION_1 float3 normal : NORMAL;
	LOCATION_2 float3 color1 : COLOR1;
	LOCATION_3 float fog : FOG;
#ifndef USE_BLEND
	LOCATION_4 float out_position_z : OUT_POSITIONZ;
#endif
#ifdef MESH_SHADER_DEBUG
	LOCATION_5 uint MeshletIndex : MESHLETINDEX;
#endif
};
#endif

#ifdef USE_DETAIL
struct PS_LMAP_IN
{
	float4 position : SV_POSITION;
	LOCATION_0 float2 uv : UV;
	LOCATION_1 float3 normal : NORMAL;
	LOCATION_2 float4 color1 : COLOR1;
	LOCATION_3 float4 color2 : COLOR2;
	LOCATION_4 float4 uv_map : UVMAP;
	LOCATION_5 float fog : FOG;
#ifndef USE_BLEND
	LOCATION_6 float out_position_z : OUT_POSITIONZ;
#endif
#ifdef MESH_SHADER_DEBUG
	LOCATION_7 uint MeshletIndex : MESHLETINDEX;
#endif
};
#else
struct PS_LMAP_IN
{
	float4 position : SV_POSITION;
	LOCATION_0 float2 uv : UV;
	LOCATION_1 float3 normal : NORMAL;
	LOCATION_2 float3 color1 : COLOR1;
	LOCATION_3 float3 color2 : COLOR2;
	LOCATION_4 float2 uv_map : UVMAP;
	LOCATION_5 float fog : FOG;
#ifndef USE_BLEND
	LOCATION_6 float out_position_z : OUT_POSITIONZ;
#endif
#ifdef MESH_SHADER_DEBUG
	LOCATION_7 uint MeshletIndex : MESHLETINDEX;
#endif
};
#endif

struct PS_LMAPE_IN
{
	float4 position : SV_POSITION;
	LOCATION_0 float2 uv : UV;
	LOCATION_1 float3 normal : NORMAL;
	LOCATION_2 float3 color1 : COLOR1;
	LOCATION_3 float3 color2 : COLOR2;
	LOCATION_4 float2 uv_map1 : UVMAP1;
	LOCATION_4 float3 uv_map2 : UVMAP2;
	LOCATION_5 float fog : FOG;
#ifndef USE_BLEND
	LOCATION_6 float out_position_z : OUT_POSITIONZ;
#endif
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct PS_LIGHTPLANES_IN
{
	float4 position : SV_POSITION;
	LOCATION_0 float2 uv : UV;
	LOCATION_1 float color : COLOR;
	LOCATION_2 float out_position_z : OUT_POSITIONZ;
};
struct PS_LOD_IN
{
	float4 position : SV_POSITION;
	LOCATION_0 float2 uv0 : UV0;
	LOCATION_1 float2 uv1 : UV1;
	LOCATION_2 float4 color0 : COLOR0;
	LOCATION_3 float factor : COLOR1;
	LOCATION_4 float fog : FOG;
	LOCATION_5 float out_position_z : OUT_POSITIONZ;
};
struct PS_SIMPLE_IN
{
	float4 position : SV_POSITION;
	LOCATION_0 float2 uv : UV;
	LOCATION_2 float out_position_z : OUT_POSITIONZ;
};
struct PS_TERRAIN_IN
{
	float4 position : SV_POSITION;
	LOCATION_0 float2 uv1 : UV1;
	LOCATION_1 float2 uv2 : UV2;
	LOCATION_2 float3 color1 : COLOR1;
	LOCATION_3 float3 color2 : COLOR2;
	LOCATION_4 float fog : FOG;
	LOCATION_5 float out_position_z : OUT_POSITIONZ;
#ifdef MESH_SHADER_DEBUG
	LOCATION_6 uint MeshletIndex : MESHLETINDEX;
#endif
};
struct PS_SKY_IN
{
	float4 position : SV_POSITION;
	LOCATION_0  float3 uv : UV;
	LOCATION_1  float4 color : COLOR;
};
struct PS_CLOUDS_IN
{
	float4 position : SV_POSITION;
	LOCATION_0  float2 uv1 : UV1;
	LOCATION_1  float2 uv2 : UV2;
	LOCATION_2  float4 color : COLOR;
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef USE_BLEND
struct PS_BASE_OUT
{
	float4 color : SV_Target0;
};
#else
struct PS_BASE_OUT
{
	float4 color : SV_Target0;
	float4 bloom : SV_Target1;
	float4 compute : SV_Target2;
};
#endif