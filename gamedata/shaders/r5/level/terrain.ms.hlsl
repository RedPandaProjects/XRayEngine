#define USE_DETAIL_UNIFORM
#define USE_GLOBAL_UNIFORM
#define USE_TRANSFORMATION_UNIFORM
#define MESH_SHADER
#include "common.h"


StructuredBuffer<VS_R1LMap_IN> Vertices : register(t8);

PS_TERRAIN_IN GetVertexAttributes(uint meshletIndex, uint vertexIndex)
{
    VS_R1LMap_IN input = Vertices[vertexIndex];
    PS_TERRAIN_IN res = (PS_TERRAIN_IN) 0;
    res.position = mul(VPW, float4(input.position, 1));
    res.out_position_z = mul(WV, float4(input.position, 1)).z;
#ifdef MESH_SHADER_DEBUG
    res.MeshletIndex = meshletIndex; 
#else
    uint4 normal = UnpackColor(input.normal);
    uint4 tangent = UnpackColor(input.tangent);
    uint4 binormal = UnpackColor(input.binormal);
    normal.xyz = normal.zyx;
    int4 uv = UnpackUV2(input.uv);
    float3 N = unpack_normal(float3(normal.xyz) / 255.f);
    res.color1 = v_hemi(N); // hemi
    res.color2 = v_sun(N);
	
    res.fog = calc_fogging(float4(input.position, 1));
    res.uv1 = unpack_tc_base(uv.xy, float(tangent.w) / 255.f, float(binormal.w) / 255.f);
    res.uv2 = DetailParams.x * res.uv1;
#endif
    return res;
}

[NumThreads(GROUP_SIZE, 1, 1)]
[OutputTopology("triangle")]
void main(
    uint gtid : SV_GroupThreadID,
    uint gid : SV_GroupID,
    out indices uint3 tris[64],
    out vertices PS_TERRAIN_IN verts[64]
)
{
    Meshlet m = Meshlets[gid];
    SetMeshOutputCounts(m.VertCount, m.PrimCount);

    if (gtid < m.PrimCount)
    {
        tris[gtid] = GetPrimitive(m, gtid);
    }

    if (gtid < m.VertCount)
    {
        uint vertexIndex = GetVertexIndex(m, gtid);
        verts[gtid] = GetVertexAttributes(gid, vertexIndex);
    }
}
