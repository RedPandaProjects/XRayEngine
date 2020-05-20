#pragma once

struct XRaySubset
{
    u32 Offset;
    u32 Count;
};

struct XRayMeshlet
{
    u32 VertCount;
    u32 VertOffset;
    u32 PrimCount;
    u32 PrimOffset;
};

union XRayPackedTriangle
{
    struct
    {
        u32 i0 : 10;
        u32 i1 : 10;
        u32 i2 : 10;
        u32 _unused : 2;
    } indices;
    u32 packed;
};
enum EMeshShaderSlot
{
    MSS_Vertices = 8,
    MSS_Meshlets = 9,
    MSS_UniqueVertexIndices = 10,
    MSS_PrimitiveIndices = 11,
};

class XRayMeshletGenerator
{
public:
	static void  ComputeMeshlets(u32 maxVerts, u32 maxPrims,const u32* indices, u32 indexCount,const XRaySubset* indexSubsets, u32 subsetCount,const void* vertex_buffer,size_t stride, u32 vertexCount,xr_vector<XRaySubset>& meshletSubsets,xr_vector<XRayMeshlet>& meshlets,xr_vector<uint8_t>& uniqueVertexIndices,xr_vector<XRayPackedTriangle>& primitiveIndices);
};