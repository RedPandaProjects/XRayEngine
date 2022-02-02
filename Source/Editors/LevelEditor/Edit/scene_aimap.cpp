#include "stdafx.h"
#pragma hdrstop

#include "Scene.h"
#include "..\XrGame\xrLevel.h"
#include "ESceneAIMapTools.h"
#include "..\XrECore\Engine\guid_generator.h"

const u32 UnkonnectedNode = 0xfffffff0;
const WORD	InvalidSector = 0xff;
const float	cover_distance = 30.f;
const float high_cover_height = 1.5f;
const float low_cover_height = 0.6f;
const float cover_sqr_dist = cover_distance * cover_distance;
static SAIParams g_params;



IC void CNodePositionCompressor(NodePosition& Pdest, Fvector& Psrc, hdrNODES& H)
{
	float sp = 1 / g_params.fPatchSize;
	int row_length = iFloor((H.aabb.max.z - H.aabb.min.z) / H.size + EPS_L + 1.5f);
	int pxz = iFloor((Psrc.x - H.aabb.min.x) * sp + EPS_L + .5f) * row_length + iFloor((Psrc.z - H.aabb.min.z) * sp + EPS_L + .5f);
	int py = iFloor(65535.f * (Psrc.y - H.aabb.min.y) / (H.size_y) + EPS_L);
	VERIFY(pxz < (1 << MAX_NODE_BIT_COUNT) - 1);
	Pdest.xz(pxz);
	clamp(py, 0, 65535);	Pdest.y(u16(py));
}
IC void	compress_node(NodeCompressed& Dest, SAINode* Src)
{
	Dest.light(15);//compress(Src.LightLevel,15));
	for (u8 L = 0; L < 4; ++L)
		Dest.link(L, Src->n[L]? Src->n[L]->idx:InvalidNode);
}
IC BYTE	compress(float c, int max_value)
{
	int	cover = iFloor(c * float(max_value) + .5f);
	clamp(cover, 0, max_value);
	return BYTE(cover);
}
void	Compress(NodeCompressed& Dest, SAINode* Src, hdrNODES& H)
{
	// Compress plane (normal)
	Dest.plane = pvCompress(Src->Plane.n);

	// Compress position
	CNodePositionCompressor(Dest.p, Src->Pos, H);
	//	CompressPos	(Dest.p1,Src.P1,H);

		// Sector
		// R_ASSERT(Src.sector<=255);
		// Dest.sector = BYTE(Src.sector);

		// Light & Cover
	compress_node(Dest, Src);
	//	Dest.cover[0]	= CompressCover(Src.cover[0]);
	//	Dest.cover[1]	= CompressCover(Src.cover[1]);
	//	Dest.cover[2]	= CompressCover(Src.cover[2]);
	//	Dest.cover[3]	= CompressCover(Src.cover[3]);
	Dest.high.cover0 = compress(high_cover_height, 15);
	Dest.high.cover1 = compress(high_cover_height, 15);
	Dest.high.cover2 = compress(high_cover_height, 15);
	Dest.high.cover3 = compress(high_cover_height, 15);
	Dest.low.cover0 = compress(low_cover_height, 15);
	Dest.low.cover1 = compress(low_cover_height, 15);
	Dest.low.cover2 = compress(low_cover_height, 15);
	Dest.low.cover3 = compress(low_cover_height, 15);
	//	Msg				("[%.3f -> %d][%.3f -> %d][%.3f -> %d][%.3f -> %d]",
	//		Src.cover[0],Dest.cover0,
	//		Src.cover[1],Dest.cover1,
	//		Src.cover[2],Dest.cover2,
	//		Src.cover[3],Dest.cover3
	//		);

		// Compress links
	//	R_ASSERT	(Src.neighbours.size()<64);
	//	Dest.links	= BYTE(Src.neighbours.size());
}



class CNodeRenumberer 
{


public:
	CNodeRenumberer(
		xr_vector<NodeCompressed>& nodes,
		xr_vector<u32>& sorted,
		xr_vector<u32>& renumbering
	) 
	{
		u32					N = (u32)nodes.size();
		sorted.resize(N);
		renumbering.resize(N);

		for (u32 i = 0; i < N; ++i)
			sorted[i] = i;

		std::stable_sort(sorted.begin(), sorted.end(), [&nodes](u32 vertex_id0, u32 vertex_id1)
			{
				return		(nodes[vertex_id0].p.xz() < nodes[vertex_id1].p.xz());
			});

		for (u32 i = 0; i < N; ++i)
			renumbering[sorted[i]] = i;

		for (u32 i = 0; i < N; ++i) {
			for (u32 j = 0; j < 4; ++j) {
				u32			vertex_id = nodes[i].link(u8(j));
				if (vertex_id >= N)
					continue;
				nodes[i].link(u8(j), renumbering[vertex_id]);
			}
		}

		std::stable_sort(nodes.begin(), nodes.end(),[](const NodeCompressed& vertex0, const NodeCompressed& vertex1) 
		{
			return		(vertex0.p.xz() < vertex1.p.xz());
		});
	}
};

NodeCompressed* EScene::GetAINodes()
{
    return m_AIMapNodes.data();
}

hdrNODES* EScene::GetAIHeader()
{
    return &m_AIMapHeader;
}




void EScene::BuildAIMap()
{
    ESceneAIMapTool* AIMapTool = dynamic_cast<ESceneAIMapTool*>(GetTool(OBJCLASS_AIMAP));
	g_params = AIMapTool->AIParams();

	AIMapTool->EnumerateNodes();

	size_t Index = 0;
	auto CalculateHeight = [&AIMapTool](Fbox& BB)->float
	{
		// All nodes
		BB.invalidate();

		for (u32 i = 0; i < AIMapTool->Nodes().size(); i++)
		{
			BB.modify(AIMapTool->Nodes()[i]->Pos);
		}
		return BB.max.y - BB.min.y + EPS_L;
	};

	AIMapTool->CalculateNodesBBox(m_AIMapHeader.aabb);
	m_AIMapHeader.version = XRAI_CURRENT_VERSION;
	m_AIMapHeader.count = AIMapTool->Nodes().size();
	m_AIMapHeader.size = g_params.fPatchSize;
	m_AIMapHeader.size_y = CalculateHeight(m_AIMapHeader.aabb);
	m_AIMapHeader.guid = generate_guid();
	m_AIMapNodes.erase(m_AIMapNodes.begin(), m_AIMapNodes.end());
	m_AIMapNodes.reserve(AIMapTool->Nodes().size());
    for (SAINode* Node : AIMapTool->Nodes())
    {
		NodeCompressed	NC;
		Compress(NC, Node, m_AIMapHeader);
		m_AIMapNodes.push_back(NC);
    }
	xr_vector<u32>	sorted;
	xr_vector<u32>	renumbering;
	CNodeRenumberer	A(m_AIMapNodes, sorted, renumbering);

}
