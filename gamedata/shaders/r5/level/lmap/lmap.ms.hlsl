#define USE_GLOBAL_UNIFORM
#define USE_TRANSFORMATION_UNIFORM
#define MESH_SHADER
#include "common.h"



StructuredBuffer<VS_R1LMap_IN> Vertices : register(t8);

PS_LMAP_IN GetVertexAttributes(uint meshletIndex, uint vertexIndex)
{
    VS_R1LMap_IN input = Vertices[vertexIndex];
    
    PS_LMAP_IN output
#ifdef MESH_SHADER_DEBUG
  =(PS_LMAP_IN)0;
#else
;
#endif
	
	////////////////////////////////////////////////////////////////
    output.position = mul(VPW, float4(input.position, 1));
#ifndef USE_BLEND
    output.out_position_z = mul(WV, float4(input.position, 1)).z;
#endif
#ifdef MESH_SHADER_DEBUG
    output.MeshletIndex = meshletIndex; 
#else
    uint4 normal = UnpackColor(input.normal);
    uint4 tangent = UnpackColor(input.tangent);
    uint4 binormal = UnpackColor(input.binormal);
    int4 uv = UnpackUV2(input.uv);
	////////////////////////////////////////////////////////////////
    normal.xyz = normal.zyx;
    output.normal = unpack_normal(float3(normal.xyz) / 255.f);
    output.uv = unpack_tc_base(input.uv.xy, float(tangent.w) / 255.f, float(tangent.w) / 255.f);
		
	////////////////////////////////////////////////////////////////
    output.uv_map.xy = unpack_tc_lmap(uv.zw);
    output.color1.xyz = v_hemi(output.normal);
    output.color2.xyz = v_sun(output.normal);
	////////////////////////////////////////////////////////////////
	
    output.fog = calc_fogging(float4(input.position, 1));
	////////////////////////////////////////////////////////////////
#ifdef USE_DETAIL
    output.uv_map.zw = output.uv * DetailParams.x;
    float2 dt = calc_detail(input.position.xyz);
    output.color1.a = dt.x;
    output.color2.a = dt.y;
#endif
#endif
	////////////////////////////////////////////////////////////////
    return output;
}

[NumThreads(GROUP_SIZE, 1, 1)]
[OutputTopology("triangle")]
void main(
    uint gtid : SV_GroupThreadID,
    uint gid : SV_GroupID,
    out indices uint3 tris[64],
    out vertices PS_LMAP_IN verts[64]
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
