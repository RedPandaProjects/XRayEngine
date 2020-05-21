struct VS_R1Vertex_IN
{
	float3 position;
	uint normal;
	uint tangent;
	uint binormal;
	uint color;
	int uv;
};
struct VS_R1LMap_IN
{
	float3 position;
	uint normal;
	uint tangent;
	uint binormal;
	int2 uv;
};

int2 UnpackUV(int color)
{
	// Unpacks a 10 bits per index triangle from a 32-bit uint.
	return int2(color & 0xFFFF, (color >> 16) & 0xFFFF);
}
int4 UnpackUV2(int2 color)
{
	// Unpacks a 10 bits per index triangle from a 32-bit uint.
	return int4(color.x & 0xFFFF, (color.x >> 16) & 0xFFFF, color.y & 0xFFFF, (color.y >> 16) & 0xFFFF);
}
uint4 UnpackColor(uint color)
{
	// Unpacks a 10 bits per index triangle from a 32-bit uint.
	return uint4(color & 0xFF, (color >> 8) & 0xFF, (color >> 16) & 0xFF, (color >> 24) & 0xFF);
}