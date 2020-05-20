#include "pch.h"
#include "../../xrEngine/IGame_Persistent.h"
#include "../../xrEngine/CustomHUD.h"
#include "../../xrEngine/xrLevel.h"
#include "../../xrCore/stream_reader.h"
void XRayRenderInterface::level_Load(IReader* fs)
{
	R_ASSERT(g_pGamePersistent);
	IReader* chunk;
	{
		chunk = fs->open_chunk(fsL_SHADERS);
		R_ASSERT2(chunk, "Level doesn't builded correctly.");
		u32 count = chunk->r_u32();
		m_Shader.resize(count);
		for (u32 i = 0; i < count; i++)	// skip first shader as "reserved" one
		{
			string512				n_sh, n_tlist;
			LPCSTR			n = LPCSTR(chunk->pointer());
			chunk->skip_stringZ();
			if (0 == n[0])			continue;
			xr_strcpy(n_sh, n);
			LPSTR			delim = strchr(n_sh, '/');
			*delim = 0;
			xr_strcpy(n_tlist, delim + 1);
			Msg("Level shader:%s", n_sh);
			GResourcesManager->CompileBlender(m_Shader[i], n_sh, n_tlist);
		}
		chunk->close();
	}
	{
		g_pGamePersistent->LoadTitle("st_loading_geometry");
		g_pGamePersistent->LoadTitle();
		CStreamReader* geom = FS.rs_open("$level$", "level.geom");
		R_ASSERT2(geom, "level.geom");
		LoadBuffers(geom);
		LoadSWIs(geom);
		FS.r_close(geom);

		// Visuals
		g_pGamePersistent->LoadTitle("st_loading_spatial_db");
		g_pGamePersistent->LoadTitle();
		chunk = fs->open_chunk(fsL_VISUALS);
		LoadVisuals(chunk);
		chunk->close();
	}
	LoadSectors(fs);

	HOM.Load();
	GRenderTarget->LoadLevel();
}
#pragma pack(push,4)
struct D3DVERTEXELEMEN_D3D9
{
	u16    Stream;     // Stream index
	u16    Offset;     // Offset in the stream in bytes
	u8    Type;       // Data type
	u8    Method;     // Processing method
	u8    Usage;      // Semantics
	u8    UsageIndex; // Semantic index
};
#pragma pack(pop)
void XRayRenderInterface::LoadBuffers(CStreamReader* base_fs)
{
	if (HW->MeshShaderSupport())
	{

		// Vertex buffers
		if (base_fs->find_chunk(fsL_VB))
		{
			// Use DX9-style declarators
			auto* fs = base_fs->open_chunk(fsL_VB);
			u32 count = fs->r_u32();
			m_VertexState.resize(count);
			//m_VertexBufferForMesh.resize(count);
			for (u32 i = 0; i < count; i++)
			{
				u32					buffer_size = (64 + 1) * sizeof(D3DVERTEXELEMEN_D3D9);
				D3DVERTEXELEMEN_D3D9* dcl = (D3DVERTEXELEMEN_D3D9*)_alloca(buffer_size);
				fs->r(dcl, buffer_size);
				fs->advance(-(int)buffer_size);
				fs->advance(GetSize(dcl));

				m_VertexState[i] = ConvertFVF(dcl);
				u32 vCount = fs->r_u32();
				u32 vSize = GResourcesManager->GetStride(m_VertexState[i]);
				Msg("* [Loading VB] %d verts, %d Kb", vCount, (vCount * vSize) / 1024);

				u8* pData = xr_alloc<u8>(vCount * vSize);
				fs->r(pData, vCount * vSize);
				m_VertexBufferForMesh.emplace_back(std::move(pData));
			//	m_VertexBufferForMesh[i] = std::unique_ptr<u8>((u8*)pData);

			}
			fs->close();
		}
		else {
			FATAL("DX7-style FVFs unsupported");
		}
	}
	if (!HW->MeshShaderSupport()||HW->DevOldShaderSupport())
	{
		if (base_fs->find_chunk(fsL_VB))
		{
			// Use DX9-style declarators
			auto* fs = base_fs->open_chunk(fsL_VB);
			u32 count = fs->r_u32();
			m_VertexState.resize(count);
			m_VertexBuffer.resize(count);
			for (u32 i = 0; i < count; i++)
			{
				u32					buffer_size = (64 + 1) * sizeof(D3DVERTEXELEMEN_D3D9);
				D3DVERTEXELEMEN_D3D9* dcl = (D3DVERTEXELEMEN_D3D9*)_alloca(buffer_size);
				fs->r(dcl, buffer_size);
				fs->advance(-(int)buffer_size);
				fs->advance(GetSize(dcl));

				m_VertexState[i] = ConvertFVF(dcl);
				u32 vCount = fs->r_u32();
				u32 vSize = GResourcesManager->GetStride(m_VertexState[i]);
				Msg("* [Loading VB] %d verts, %d Kb", vCount, (vCount * vSize) / 1024);

				u8* pData = xr_alloc<u8>(vCount * vSize);
				fs->r(pData, vCount * vSize);
				m_VertexBuffer[i] = BearRenderInterface::CreateVertexBuffer();
				m_VertexBuffer[i]->Create(vSize, vCount, false, pData);
				xr_free(pData);

			}
			fs->close();
		}
		else {
			FATAL("DX7-style FVFs unsupported");
		}
	}
	// Index buffers
	if (HW->MeshShaderSupport())
	{
		if (base_fs->find_chunk(fsL_IB))
		{
			auto* fs = base_fs->open_chunk(fsL_IB);
			u32 count = fs->r_u32();
			//m_IndexBuffersForMesh.resize(count);
			for (u32 i = 0; i < count; i++)
			{
				u32 iCount = fs->r_u32();
				Msg("* [Loading IB] %d indices, %d Kb", iCount, (iCount * 2) / 1024);

				u32* pData = xr_alloc<u32>(iCount);
				fs->r(pData, iCount * 2);
				u16* srcData = (u16*)pData;

				for (u32 a = iCount; a != 0; a--)
				{
					pData[a - 1] = srcData[a - 1];
				}
				m_IndexBuffersForMesh.emplace_back(std::move(pData));
			}
			fs->close();
		}
		
	}
	if (!HW->MeshShaderSupport() || HW->DevOldShaderSupport())
	{
		if (base_fs->find_chunk(fsL_IB))
		{
			auto* fs = base_fs->open_chunk(fsL_IB);
			u32 count = fs->r_u32();
			m_IndexBuffers.resize(count);
			for (u32 i = 0; i < count; i++)
			{
				u32 iCount = fs->r_u32();
				Msg("* [Loading IB] %d indices, %d Kb", iCount, (iCount * 2) / 1024);

				u32* pData = xr_alloc<u32>(iCount);
				fs->r(pData, iCount * 2);
				u16* srcData = (u16*)pData;

				for (u32 a = iCount; a != 0; a--)
				{
					pData[a - 1] = srcData[a - 1];
				}
				m_IndexBuffers[i] = BearRenderInterface::CreateIndexBuffer();
				m_IndexBuffers[i]->Create(iCount, true, pData);
				xr_free(pData);
			}
			fs->close();
		}
	}
	
}

void XRayRenderInterface::LoadVisuals(IReader* fs)
{
	IReader* chunk = 0;
	u32				index = 0;
	XRayRenderVisual* V = 0;
	ogf_header		H;

	while ((chunk = fs->open_chunk(index)) != 0)
	{
		chunk->r_chunk_safe(OGF_HEADER, &H, sizeof(H));
		V = GModelPool->Instance_Create(H.type);
		V->Load(0, chunk, 0);
		m_Visuals.push_back(V);

		chunk->close();
		index++;

	}
}
#pragma pack(push,4)
struct b_portal
{
	u16				sector_front;
	u16				sector_back;
	svector<Fvector, 6>	vertices;
};
#pragma pack(pop)
void XRayRenderInterface::LoadSectors(IReader* fs)
{
	size_t size = fs->find_chunk(fsL_PORTALS);
	R_ASSERT(0 == size % sizeof(b_portal));
	size_t count = size / sizeof(b_portal);
	m_Portals.resize(count);
	for (size_t c = 0; c < count; c++)
		m_Portals[c] = xr_new<CPortal>();

	// load sectors
	IReader* S = fs->open_chunk(fsL_SECTORS);
	for (u32 i = 0; ; i++)
	{
		IReader* P = S->open_chunk(i);
		if (0 == P) break;

		CSector* __S = xr_new<CSector>();
		__S->load(*P);
		m_Sectors.push_back(__S);

		P->close();
	}
	S->close();

	// load portals
	if (count)
	{
		u32	i;
		CDB::Collector	CL;
		fs->find_chunk(fsL_PORTALS);
		for (i = 0; i < count; i++)
		{
			b_portal	P;
			fs->r(&P, sizeof(P));
			CPortal* __P = (CPortal*)m_Portals[i];
			__P->Setup(P.vertices.begin(), P.vertices.size(),
				(CSector*)getSector(P.sector_front),
				(CSector*)getSector(P.sector_back));
			for (u32 j = 2; j < P.vertices.size(); j++)
				CL.add_face_packed_D(P.vertices[0], P.vertices[j - 1], P.vertices[j], u32(i));
		}
		if (CL.getTS() < 2)
		{
			Fvector		v1, v2, v3;
			v1.set(-20000.f, -20000.f, -20000.f);
			v2.set(-20001.f, -20001.f, -20001.f);
			v3.set(-20002.f, -20002.f, -20002.f);
			CL.add_face_packed_D(v1, v2, v3, 0);
		}

		// build portal model
		rmPortals = xr_new	<CDB::MODEL>();
		rmPortals->build(CL.getV(), int(CL.getVS()), CL.getT(), int(CL.getTS()));
	}
	else {
		rmPortals = 0;
	}

	// debug
	//	for (int d=0; d<Sectors.size(); d++)
	//		Sectors[d]->DebugDump	();

	pLastSector = 0;
}

void XRayRenderInterface::LoadSWIs(CStreamReader* base_fs)
{
	// allocate memory for portals
	if (base_fs->find_chunk(fsL_SWIS))
	{
		CStreamReader* fs = base_fs->open_chunk(fsL_SWIS);
		u32 item_count = fs->r_u32();



		m_SWIs.clear();

		m_SWIs.resize(item_count);
		for (u32 c = 0; c < item_count; c++) {
			FSlideWindowItem& swi = m_SWIs[c];
			swi.reserved[0] = fs->r_u32();
			swi.reserved[1] = fs->r_u32();
			swi.reserved[2] = fs->r_u32();
			swi.reserved[3] = fs->r_u32();
			swi.count = fs->r_u32();
			VERIFY(NULL == swi.sw);
			swi.sw = xr_alloc<FSlideWindow>(swi.count);
			fs->r(swi.sw, sizeof(FSlideWindow) * swi.count);
		}

		fs->close();
	}
}



void XRayRenderInterface::level_Unload()
{
	GRenderTarget->UnloadLevel();
	HOM.Unload();
	uLastLTRACK = 0;
	xr_delete(rmPortals);
	pLastSector = 0;
	for (size_t i = 0; i < m_Sectors.size(); i++)	xr_delete(m_Sectors[i]);
	m_Sectors.clear();
	// 3.
	for (size_t i = 0; i < m_Portals.size(); i++)	xr_delete(m_Portals[i]);
	m_Portals.clear();
	for (size_t i = 0; i < m_Visuals.size(); i++)
	{
		m_Visuals[i]->Release();
		xr_delete(m_Visuals[i]);
	}
	m_Visuals.clear();
	m_IndexBuffers.clear();
	m_VertexBuffer.clear();
	m_IndexBuffersForMesh.clear();
	m_VertexBufferForMesh.clear();
	m_Shader.clear();
	m_VertexState.clear();
	auto it = m_SWIs.begin();
	auto it_e = m_SWIs.end();

	for (; it != it_e; ++it)
		xr_free((*it).sw);
}
