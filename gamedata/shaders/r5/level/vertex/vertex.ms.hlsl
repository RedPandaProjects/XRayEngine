#define USE_TRANSFORMATION_UNIFORM
#define USE_GLOBAL_UNIFORM
#define MESH_SHADER
#include "common.h"



StructuredBuffer<VS_R1Vertex_IN> Vertices : register(t8);

PS_BASE_IN GetVertexAttributes(uint meshletIndex, uint vertexIndex)
{
    VS_R1Vertex_IN input = Vertices[vertexIndex];
    
    PS_BASE_IN output
#ifdef MESH_SHADER_DEBUG
  =(PS_BASE_IN)0;
#else
;
#endif
	
    
    ////////////////////////////////////////////////////////////////
    output.position = float4(input.position, 1);
    output.position = mul(VPW, output.position);
#ifndef USE_BLEND
    output.out_position_z = mul(WV, float4(input.position, 1)).z;
#endif
#ifdef MESH_SHADER_DEBUG
    output.MeshletIndex = meshletIndex; 
#else
    uint4 normal = UnpackColor(input.normal);
    uint4 tangent = UnpackColor(input.tangent);
    uint4 binormal = UnpackColor(input.binormal);
    uint4 color = UnpackColor(input.color);
    int2 uv = UnpackUV(input.uv);
	////////////////////////////////////////////////////////////////
    normal.xyz = normal.zyx;
    output.normal = unpack_normal(float3(normal.xyz) / 255.f);
	
	
	////////////////////////////////////////////////////////////////
    output.uv = unpack_tc_base(uv, float(tangent.w) / 255.f, float(binormal.w) / 255.f);
	
	
	
	////////////////////////////////////////////////////////////////
    float3 L_rgb = float3(color.bgr) / 255.f; // precalculated RGB lighting
    float3 L_hemi = v_hemi(output.normal) * (float(normal.w) / 255.f); // hemisphere
    float3 L_sun = v_sun(output.normal) * (float(color.w) / 255.f); // sun
    output.color1 = L_rgb + L_hemi + L_sun + AmbineColor.rgb;
	
	
	////////////////////////////////////////////////////////////////
    output.fog = calc_fogging(float4(input.position, 1));
	
	
	////////////////////////////////////////////////////////////////
#ifdef USE_DETAIL
    output.uv_detail.xy = output.uv * DetailParams.x;
    output.uv_detail.zw = calc_detail(input.position.xyz);
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
    out vertices PS_BASE_IN verts[64]
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
