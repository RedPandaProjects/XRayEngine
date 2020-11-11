#include "pch.h"
#include "BLenders/Blender_CLSID.h"
#include "Blenders/HUD/XRayBlenderHudMovie.h"
#include "Blenders/HUD/XRayBlenderViewportSet.h"
#include "Blenders/HUD/XRayBlenderHudCrosshair.h"
#include "Blenders/HUD/XRayBlenderHudFont.h"
#include "Blenders/HUD/XRayBlenderHudFont2.h"

#include "Blenders/Effects/XRayBlenderSky.h"
#include "Blenders/Effects/XRayBlenderCombine.h"
#include "Blenders/Effects/XRayBlenderClouds.h"
#include "Blenders/Effects/XRayBlenderSun.h"
#include "Blenders/Effects/XRayBlenderLightplanes.h"
#include "Blenders/Effects/XRayBlenderSelflight.h"
#include "Blenders/Effects/XRayBlenderBloom.h"
#include "Blenders/Level/XRayBlenderLOD.h"
#include "XRayShaderIncluder.h"
XRayResourcesManager::XRayResourcesManager()
{
	{
		BearSamplerDescription SamplerDescription;
		SamplerDescription.Filter = BearSamplerFilter::Anisotropic;
		SamplerDescription.MaxAnisotropy = 4;
		SamplerDefault = BearRenderInterface::CreateSampler(SamplerDescription);
	}
	{
		BearSamplerDescription SamplerDescription;
		SamplerDescription.AddressU = BearSamplerAddressMode::Clamp;
		SamplerDescription.AddressV = BearSamplerAddressMode::Clamp;
		SamplerDescription.AddressW = BearSamplerAddressMode::Clamp;
		SamplerDescription.Filter = BearSamplerFilter::ComparisonMinMagMipPoint;
		//	SamplerDescription.MaxAnisotropy = 16;
		SamplerNoFilter = BearRenderInterface::CreateSampler(SamplerDescription);
	}
	{
		SamplerPoint = SamplerNoFilter;
	}
	{
		BearSamplerDescription SamplerDescription;
		SamplerDescription.AddressU = BearSamplerAddressMode::Clamp;
		SamplerDescription.AddressV = BearSamplerAddressMode::Clamp;
		SamplerDescription.AddressW = BearSamplerAddressMode::Clamp;
		SamplerDescription.Filter = BearSamplerFilter::ComparisonMinMagLinearMipPoint;
		//	SamplerDescription.MaxAnisotropy = 16;
		SamplerNoFilter = BearRenderInterface::CreateSampler(SamplerDescription);
	}
	{
		BearSamplerDescription SamplerDescription;
		SamplerDescription.AddressU = BearSamplerAddressMode::Wrap;
		SamplerDescription.AddressV = BearSamplerAddressMode::Wrap;
		SamplerDescription.AddressW = BearSamplerAddressMode::Wrap;
		SamplerDescription.Filter = BearSamplerFilter::ComparisonMinMagMipLinear;
		//	SamplerDescription.MaxAnisotropy = 16;
		SamplerLinear = BearRenderInterface::CreateSampler(SamplerDescription);
	}
	{
		BearSamplerDescription SamplerDescription;
		SamplerDescription.AddressU = BearSamplerAddressMode::Clamp;
		SamplerDescription.AddressV = BearSamplerAddressMode::Clamp;
		SamplerDescription.AddressW = BearSamplerAddressMode::Clamp;
		SamplerDescription.Filter = BearSamplerFilter::ComparisonMinMagMipLinear;
		//	SamplerDescription.MaxAnisotropy = 16;
		SamplerRTLinear = BearRenderInterface::CreateSampler(SamplerDescription);
	}
}

XRayResourcesManager::~XRayResourcesManager()
{
	for (auto b = m_Blenders.begin(), e = m_Blenders.end(); b != e; b++)
	{
		xr_delete(b->second);
	}
	R_ASSERT(m_Textures.size() == 0);
}

void XRayResourcesManager::LoadShaders(const char* name)
{
	string32	ID = "shENGINE";
	string32	id;
	IReader * F = FS.r_open(name);
	F->r(&id, 8);
	if (0 == strncmp(id, ID, 8))
	{
		FATAL("Unsupported blender library. Compressed?");
	}


	IReader* fs = 0;
	// Load constants
	
	fs = F->open_chunk(2);
	if (fs) {
		IReader* chunk = NULL;
		int			chunk_id = 0;

		while ((chunk = fs->open_chunk(chunk_id)) != NULL) {
			XRayBlenderDescription	desc;
			chunk->r(&desc, sizeof(desc));
			XRayBlenderCompiler* B = CreateBlender(desc.ID);
			if (0 == B)
			{
				Msg("! Renderer doesn't support blender '%s'", desc.Name);
			}
			else
			{
				if (B->GetDescription().Version != desc.Version)
				{
					Msg("! Version conflict in shader '%s'", desc.Name);
				}

				chunk->seek(0);
				B->Load(*chunk, desc.Version);
				m_Blenders.insert(std::pair<shared_str, XRayBlenderCompiler*>(desc.Name, B));
			}
#ifdef _DEBUG
			Msg(TEXT("Register blender [%s]"), desc.Name);
#endif
			chunk->close();
			chunk_id += 1;
		}
		fs->close();
	}
	FS.r_close(F);

	{
		m_Blenders.insert(std::pair<shared_str, XRayBlenderCompiler*>("r5\\combine", xr_new< XRayBlenderCombine>()));
		m_Blenders.insert(std::pair<shared_str, XRayBlenderCompiler*>("hud\\movie",xr_new< XRayBlenderHudMovie>()));
		m_Blenders.insert(std::pair < shared_str, XRayBlenderCompiler*>("hud\\crosshair", xr_new< XRayBlenderHudCrosshair>()));
		m_Blenders.insert(std::pair < shared_str, XRayBlenderCompiler*>("viewport\\set", xr_new<XRayBlenderViewportSet>()));
		m_Blenders.insert(std::pair<shared_str, XRayBlenderCompiler*>("hud\\font", xr_new< XRayBlenderHudFont>()));
		//m_Blenders.insert(std::pair<shared_str, XRayBlenderCompiler*>("font", xr_new< XRayBlenderHudFont>()));
		m_Blenders.insert(std::pair<shared_str, XRayBlenderCompiler*>("hud\\font2", xr_new< XRayBlenderHudFont2>()));
		m_Blenders.insert(std::pair<shared_str, XRayBlenderCompiler*>("sky", xr_new < XRayBlenderSky>()));
		
		ReplaceBlender("details\\lod", xr_new < XRayBlenderLOD >());
		
		ReplaceBlender("effects\\sun", xr_new < XRayBlenderSun >());
		ReplaceBlender("effects\\flare", xr_new < XRayBlenderSun >());
		ReplaceBlender("clouds", xr_new < XRayBlenderClouds >());

		ReplaceBlender("effects\\lightplanes", xr_new < XRayBlenderLightplanes >());

		ReplaceBlender("selflight", xr_new < XRayBlenderSelflight >());
		ReplaceBlender("r5\\bloom", xr_new<XRayBlenderBloom>());
		
	}

	for (auto b = m_Blenders.begin(), e = m_Blenders.end(); b != e; b++)
	{
		b->second->Initialize();
	}

	BearRenderStats::Dump();
}

void XRayResourcesManager::CompileBlender(XRayShader& Blender, const char* name, const char* textures)
{
	R_ASSERT(m_Blenders.find(name) != m_Blenders.end());
	m_Blenders[name]->Compile(Blender, textures);
	Blender.Name = name;
}

size_t XRayResourcesManager::GetStride(u32 VertexState)
{
	switch (VertexState)
	{
	case FVF::F_0W:
		return 36;
		break;
	case FVF::F_1W:
		return 36;
		break;
	case FVF::F_2W:
		return 44;
		break;
	case FVF::F_3W:
		return 44;
		break;
	case FVF::F_4W:
		return 40;
		break;
	case FVF::F_R1LMap:
		return 32;
		break;
	case FVF::F_R1Vert:
		return 32;
		break;
	case FVF::F_XVert:
		return 12;
		break;
	case FVF::F_MUModel:
		return 32;
	case FVF::F_LOD:
		return 76;
	default:
		break;
	}
	size_t texture_uv_count = 0;
	if ((D3DFVF_TEX1 & int(VertexState)) == D3DFVF_TEX1)
	{

		texture_uv_count = 1;

	}
	else if ((D3DFVF_TEX2 & int(VertexState)) == D3DFVF_TEX2)
	{

		texture_uv_count = 2;

	}
	else if ((D3DFVF_TEX3 & int(VertexState)) == D3DFVF_TEX3)
	{
		texture_uv_count = 3;
	}
	else if ((D3DFVF_TEX4 & int(VertexState)) == D3DFVF_TEX4)
	{

		texture_uv_count = 4;

	}
	static int StrideArray[] =
	{
#define REGISTER(name,byte,stride,a1,a2) byte,stride,
#include "D3D9VertexState.h"
#undef REGISTER
	};
	size_t Stride = 0;
	for (size_t i = 0; i < D3DFVF_COUNT; i++)
	{
		if ((StrideArray[i * 2] == D3DFVF_TEX4) && texture_uv_count >= 4)
		{
			if ((D3DFVF_TEXCOORDSIZE1(3) & int(VertexState)) == D3DFVF_TEXCOORDSIZE1(3))
			{
				Stride += sizeof(float);
			}
			else if ((D3DFVF_TEXCOORDSIZE3(3) & int(VertexState)) == D3DFVF_TEXCOORDSIZE3(3))
			{
				Stride += sizeof(float) * 3;
			}
			else if ((D3DFVF_TEXCOORDSIZE4(3) & int(VertexState)) == D3DFVF_TEXCOORDSIZE4(3))
			{
				Stride += sizeof(float) * 4;
			}
			else
			{
				Stride += sizeof(float) * 2;
			}
		}
		else  if ((StrideArray[i * 2] == D3DFVF_TEX3) && texture_uv_count >= 3)
		{
			if ((D3DFVF_TEXCOORDSIZE1(2) & int(VertexState)) == D3DFVF_TEXCOORDSIZE1(2))
			{
				Stride += sizeof(float);
			}
			else if ((D3DFVF_TEXCOORDSIZE3(2) & int(VertexState)) == D3DFVF_TEXCOORDSIZE3(2))
			{
				Stride += sizeof(float) * 3;
			}
			else if ((D3DFVF_TEXCOORDSIZE4(2) & int(VertexState)) == D3DFVF_TEXCOORDSIZE4(2))
			{
				Stride += sizeof(float) * 4;
			}
			else
			{
				Stride += sizeof(float) * 2;
			}
		}
		else  if ((StrideArray[i * 2] == D3DFVF_TEX2) && texture_uv_count >= 2)
		{
			if ((D3DFVF_TEXCOORDSIZE1(1) & int(VertexState)) == D3DFVF_TEXCOORDSIZE1(1))
			{
				Stride += sizeof(float);
			}
			else if ((D3DFVF_TEXCOORDSIZE3(1) & int(VertexState)) == D3DFVF_TEXCOORDSIZE3(1))
			{
				Stride += sizeof(float) * 3;
			}
			else if ((D3DFVF_TEXCOORDSIZE4(1) & int(VertexState)) == D3DFVF_TEXCOORDSIZE4(1))
			{
				Stride += sizeof(float) * 4;
			}
			else
			{
				Stride += sizeof(float) * 2;
			}

		} 
		else  if ((StrideArray[i * 2] == D3DFVF_TEX1) && texture_uv_count >= 1)
		{
			if ((D3DFVF_TEXCOORDSIZE1(0) & int(VertexState)) == D3DFVF_TEXCOORDSIZE1(0))
			{
				Stride += sizeof(float);
			}
			else if ((D3DFVF_TEXCOORDSIZE3(0) & int(VertexState)) == D3DFVF_TEXCOORDSIZE3(0))
			{
				Stride += sizeof(float) * 3;
			}
			else if ((D3DFVF_TEXCOORDSIZE4(0) & int(VertexState)) == D3DFVF_TEXCOORDSIZE4(0))
			{
				Stride += sizeof(float) * 4;
			}
			else
			{
				Stride += sizeof(float) * 2;
			}

		}
		else
		{

			if ((StrideArray[i * 2] & int(VertexState)) == StrideArray[i * 2])
				Stride += StrideArray[i * 2 + 1];
		}
	}
	return Stride;
}

BearFactoryPointer<BearRHI::BearRHIShader> XRayResourcesManager::GetPixelShader(shared_str name)
{
	auto item = m_PShaders.find(name);
	if (item == m_PShaders.end())
	{
		Msg(TEXT("Load shader [%s]"), name.c_str());
	
		BearFactoryPointer<BearRHI::BearRHIShader> shader = BearRenderInterface::CreatePixelShader();
		string_path					cname;
#if 0
		xr_strconcat(cname, ::Render->getShaderPath(), name.c_str(), ".ps.bin");
		FS.update_path(cname, "$game_shaders$", cname);
		IReader* file = FS.r_open(cname);
		R_ASSERT(file);
		shader->LoadAsBinary(file->pointer(), file->length());
		FS.r_close(file);
#else
		xr_string Text;

		string_path local_path;
		xr_strcpy(local_path, name.c_str());
		{
			char* ext = strrchr(local_path, '\\');
			if (ext)
			{
				*ext = 0;
			}
			else
			{
				local_path[0] = 0;
			}
		}
		{
			xr_strcpy(cname, ::Render->getShaderPath());
			xr_strcat(cname, name.c_str());
			xr_strcat(cname, ".ps.hlsl");

			FS.update_path(cname, "$game_shaders$", cname);

			// duplicate and zero-terminate
			IReader* file = FS.r_open(cname);
			R_ASSERT(file);
			Text.assign((char*)file->pointer(), file->length());
			FS.r_close(file);
		}
		BearMap<BearStringConteniar, BearStringConteniar> Defines;
		{
#ifdef DEBUG
			BearString out;
			if (XRayRenderConsole::ps_r_mesh_shader_debug&&HW->MeshShaderSupport())
			{
				Defines["MESH_SHADER_DEBUG"] = "1";
			}
			XRayShaderIncluder Includer(::Render->getShaderPath(), local_path);
			if (!shader->LoadAsText(Text.c_str(),TEXT("Main"), Defines, out,&Includer,"",""))
			{
				Msg(TEXT("------------------------------------------------------------------------"));
				Msg(*out);
				Msg(TEXT("------------------------------------------------------------------------"));
				R_ASSERT(0);
			}
			if (out.size())
			{
				Msg(TEXT("------------------------------------------------------------------------"));
				Msg(*out);
				Msg(TEXT("------------------------------------------------------------------------"));
			}
#endif
		}
#endif
		m_PShaders.insert(std::pair< shared_str, BearFactoryPointer<BearRHI::BearRHIShader>>(name, shader));
		return shader;
	}
	return item->second;
}

BearFactoryPointer<BearRHI::BearRHIShader> XRayResourcesManager::GetVertexShader(shared_str name)
{

	auto item = m_VShaders.find(name);
	string_path					cname;
	if (item == m_VShaders.end())
	{
		Msg(TEXT("Load shader [%s]"), name.c_str());
		BearFactoryPointer<BearRHI::BearRHIShader> shader = BearRenderInterface::CreateVertexShader();
#if 0
		string_path					cname;
		xr_strconcat(cname, ::Render->getShaderPath(), name.c_str(), ".vs.bin");
		FS.update_path(cname, "$game_shaders$", cname);
		IReader* file = FS.r_open(cname);
		R_ASSERT(file);
		shader->LoadAsBinary(file->pointer(), file->length());
		FS.r_close(file);
#else
		xr_string Text;
		string_path local_path;
		xr_strcpy(local_path, name.c_str());
		{
			char* ext = strrchr(local_path, '\\');
			if (ext)
			{
				*ext = 0;
			}
			else
			{
				local_path[0] = 0;
			}
		}

		{
			xr_strcpy(cname, ::Render->getShaderPath());
			xr_strcat(cname, name.c_str());
			xr_strcat(cname, ".vs.hlsl");

			FS.update_path(cname, "$game_shaders$", cname);

			// duplicate and zero-terminate
			IReader* file = FS.r_open(cname);
			R_ASSERT(file);
			Text.assign((char*)file->pointer(), file->length());
			R_ASSERT2(file, cname);
			FS.r_close(file);
		}
		BearMap<BearStringConteniar, BearStringConteniar> Defines;
		{
#ifdef DEBUG
			BearString out;
			if (XRayRenderConsole::ps_r_mesh_shader_debug && HW->MeshShaderSupport())
			{
				Defines["MESH_SHADER_DEBUG"] = "1";
			}
			XRayShaderIncluder Includer(::Render->getShaderPath(), local_path);
			if (!shader->LoadAsText(Text.c_str(), TEXT("Main"), Defines, out, &Includer, "", ""))
			{
				Msg(TEXT("------------------------------------------------------------------------"));
				Msg(*out);
				Msg(TEXT("------------------------------------------------------------------------"));
				R_ASSERT(0);
			}
			if (out.size())
			{
				Msg(TEXT("------------------------------------------------------------------------"));
				Msg(*out);
				Msg(TEXT("------------------------------------------------------------------------"));
			}
#endif
		}
#endif
		m_VShaders.insert(std::pair< shared_str, BearFactoryPointer<BearRHI::BearRHIShader>>(name, shader));
		return shader;
	}
	return item->second;
}

BearFactoryPointer<BearRHI::BearRHIShader> XRayResourcesManager::GetMeshShader(shared_str name)
{
	auto item = m_MShaders.find(name);
	string_path					cname;
	if (item == m_MShaders.end())
	{
		Msg(TEXT("Load shader [%s]"), name.c_str());
		BearFactoryPointer<BearRHI::BearRHIShader> shader = BearRenderInterface::CreateMeshShader();
#if 0
		string_path					cname;
		xr_strconcat(cname, ::Render->getShaderPath(), name.c_str(), ".ms.bin");
		FS.update_path(cname, "$game_shaders$", cname);
		IReader* file = FS.r_open(cname);
		R_ASSERT(file);
		shader->LoadAsBinary(file->pointer(), file->length());
		FS.r_close(file);
#else
		xr_string Text;
		string_path local_path;
		xr_strcpy(local_path, name.c_str());
		{
			char* ext = strrchr(local_path, '\\');
			if (ext)
			{
				*ext = 0;
			}
			else
			{
				local_path[0] = 0;
			}
		}

		{
			xr_strcpy(cname, ::Render->getShaderPath());
			xr_strcat(cname, name.c_str());
			xr_strcat(cname, ".ms.hlsl");

			FS.update_path(cname, "$game_shaders$", cname);

			// duplicate and zero-terminate
			IReader* file = FS.r_open(cname);
			R_ASSERT(file);
			Text.assign((char*)file->pointer(), file->length());
			R_ASSERT2(file, cname);
			FS.r_close(file);
		}
		BearMap<BearStringConteniar, BearStringConteniar> Defines;
		if (XRayRenderConsole::ps_r_mesh_shader_debug)
		{
			Defines["MESH_SHADER_DEBUG"] = "1";
		}
		Defines["GROUP_SIZE"] = *BearString().append_printf("%d", XRayRenderConsole::ps_r_mesh_shader_thread);
		{
#ifdef DEBUG

			BearString out;
			XRayShaderIncluder Includer(::Render->getShaderPath(), local_path);
			if (!shader->LoadAsText(Text.c_str(), TEXT("Main"), Defines, out, &Includer, "", ""))
			{
				Msg(TEXT("------------------------------------------------------------------------"));
				Msg(*out);
				Msg(TEXT("------------------------------------------------------------------------"));
				R_ASSERT(0);
			}
			if (out.size())
			{
				Msg(TEXT("------------------------------------------------------------------------"));
				Msg(*out);
				Msg(TEXT("------------------------------------------------------------------------"));
			}
#endif
		}
#endif
		m_MShaders.insert(std::pair< shared_str, BearFactoryPointer<BearRHI::BearRHIShader>>(name, shader));
		return shader;
	}
	return item->second;
}

BearFactoryPointer<BearRHI::BearRHIPipeline> XRayResourcesManager::CreatePipeline(BearPipelineGraphicsDescription& Description)
{
	auto item = m_PipelinesGraphics.find(Description);
	if (item == m_PipelinesGraphics.end())
	{
		m_PipelinesGraphics.insert(std::make_pair(Description, BearRenderInterface::CreatePipelineGraphics(Description)));
		item = m_PipelinesGraphics.find(Description);
	}
	return item->second;
}

BearFactoryPointer<BearRHI::BearRHIPipeline> XRayResourcesManager::CreatePipeline(BearPipelineMeshDescription& Description)
{
	auto item = m_PipelinesMesh.find(Description);
	if (item == m_PipelinesMesh.end())
	{
		m_PipelinesMesh.insert(std::make_pair(Description, BearRenderInterface::CreatePipelineMesh(Description)));
		item = m_PipelinesMesh.find(Description);
	}
	return item->second;
}

BearFactoryPointer<BearRHI::BearRHIRootSignature> XRayResourcesManager::CreateRootSignature(BearRootSignatureDescription& Description)
{
	auto item = m_RootSignatures.find(Description);
	if (item == m_RootSignatures.end())
	{
		m_RootSignatures.insert(std::make_pair(Description, BearRenderInterface::CreateRootSignature(Description)));
		item = m_RootSignatures.find(Description);
	}
	return item->second;
}

XRayTexture* XRayResourcesManager::GetTexture(shared_str name)
{
	auto texture = m_Textures.find(name);
	if (texture == m_Textures.end())
	{
		Msg(TEXT("Load texture [%s]"), name.c_str());
		m_Textures.insert(std::pair< shared_str, XRayTexture*>(name, xr_new<XRayTexture>(name)));
		texture = m_Textures.find(name);
	}
	texture->second->Counter++;
	return texture->second;
}

void XRayResourcesManager::Free(XRayTexture* texture)
{
	if (!texture)return;
	texture->Counter--;
	if (texture->Counter == 0)
	{
		Msg(TEXT("Unload texture [%s]"), texture->GetName().c_str());
		m_Textures.erase(texture->GetName());
		xr_delete(texture);
	}
}

XRayPipelineRef XRayResourcesManager::GetPipeline(XRayPipeline& Pipeline)
{
	for(auto & ref: m_PipelinesRef)
	{
		auto& Сontainer1 = ref->Container;
		auto& Сontainer2 = Pipeline.Container;
		if (Сontainer1.size() == Сontainer2.size())
		{
			bool Equal = true;
			auto Begin1 = Сontainer1.begin();
			auto Begin2 = Сontainer2.begin();
			for (size_t i = 0; i < Сontainer1.size(); i++)
			{
				Equal = Equal && (*Begin1 == *Begin2);
				Begin1++; Begin2++;
			}
			if (Equal)
			{
				return ref;
			}
		}
	}
	m_PipelinesRef.push_back(XRayPipelineRef(Pipeline));
	return m_PipelinesRef.back();
}

void XRayResourcesManager::ReplaceBlender(shared_str name, XRayBlenderCompiler* new_blender)
{
	
	auto item = m_Blenders.find(name);
	if (item != m_Blenders.end())
	{
		xr_delete(item->second);
		m_Blenders.erase(name);
	}
	m_Blenders.insert(std::make_pair(name, new_blender));
}

XRayBlenderCompiler* XRayResourcesManager::CreateBlender(CLASS_ID ID)
{
	switch (ID)
	{
	case B_DEFAULT: return xr_new< XRayBlenderDefault>(false);
	case B_DEFAULT_AREF: return xr_new< XRayBlenderDefaultAref>(false);
	case B_VERT: return xr_new< XRayBlenderDefault>(true);
	case B_VERT_AREF: return xr_new< XRayBlenderDefaultAref>(true);

	case B_LmEbB: return xr_new< XRayBlenderLmEbB>();
	case B_BmmD: return xr_new< XRayBlenderBmmD>();

	case B_PARTICLE: return xr_new< XRayBlenderParticle>();

	case B_SCREEN_SET: return xr_new< XRayBlenderScreenSet>();

	case B_DETAIL:return xr_new< XRayBlenderDetailStill>();
	case B_TREE: return xr_new< XRayBlenderTree>();

	case B_MODEL: return xr_new< XRayBlenderModel>();
	case B_MODEL_EbB: return xr_new< XRayBlenderModelEbB>();

	case B_EDITOR_WIRE: return xr_new<XRayBlenderEditorSelection>();
	case B_EDITOR_SEL: return xr_new<XRayBlenderEditorWire>();

	case B_SCREEN_GRAY:		
	case B_LIGHT:			
	case B_LaEmB:			
	case B_B:				
	case B_SHADOW_TEX:		
	case B_SHADOW_WORLD:	
	case B_BLUR:			return 0;
	default:
	{
		R_ASSERT(0);
		return 0;
	}
	}
}

XRayResourcesManager* GResourcesManager = nullptr;