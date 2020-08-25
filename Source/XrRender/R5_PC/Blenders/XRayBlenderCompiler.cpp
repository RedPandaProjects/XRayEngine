#include "pch.h"
void XRayBlenderCompiler::Initialize()
{
	if (HW->MeshShaderSupport() && UseMeshShader())
	{
		InitializeMesh();
	}
	else
	{
		InitializeGraphics();
	}
}

void XRayBlenderCompiler::InitializeMesh()
{
}

void XRayBlenderCompiler::InitializeGraphics()
{
}

LPCSTR XRayBlenderCompiler::getName()
{
	return Description.Name;
}

LPCSTR XRayBlenderCompiler::getComment()
{
	return "";
}

void XRayBlenderCompiler::Save(IWriter & fs)
{
	R_ASSERT(false);
}

void XRayBlenderCompiler::Load(IReader & fs, u16 version)
{
	u16	V = Description.Version;
	fs.r(&Description, sizeof(Description));
	Description.Version = V;

	xrPREAD_MARKER(fs);
	xrPREAD_PROP(fs, xrPID_INTEGER, oPriority);
	xrPREAD_PROP(fs, xrPID_BOOL, oStrictSorting);
	xrPREAD_MARKER(fs);
	xrPREAD_PROP(fs, xrPID_TEXTURE, oTName);
	xrPREAD_PROP(fs, xrPID_MATRIX, oTXform);
}

XRayBlenderCompiler::XRayBlenderCompiler()
{
	
}

XRayBlenderCompiler::~XRayBlenderCompiler()
{
}
inline  size_t GetSize(BearVertexFormat format)
{
	switch (format)
	{
	case BearVertexFormat::R32_FLOAT:
		return sizeof(float) * 1;
	case BearVertexFormat::R32G32_FLOAT:
		return sizeof(float) * 2;
	case BearVertexFormat::R32G32B32_FLOAT:
		return sizeof(float) * 3;
	case BearVertexFormat::R32G32B32A32_FLOAT:
		return sizeof(float) * 4;
	case BearVertexFormat::R32_INT:
		return sizeof(int);
	case BearVertexFormat::R8:
		return sizeof(char);
	case BearVertexFormat::R8G8:
		return sizeof(char)*2;
	case BearVertexFormat::R8G8B8A8:
		return sizeof(int);
	default:
		return 0;
	}
}
void XRayBlenderCompiler::Compile(XRayShaderElement& shader)
{

}

inline void FixTextureName(LPSTR fn)
{
	LPSTR _ext = strext(fn);
	if (_ext &&
		(0 == stricmp(_ext, ".tga") ||
			0 == stricmp(_ext, ".dds") ||
			0 == stricmp(_ext, ".bmp") ||
			0 == stricmp(_ext, ".ogm")))
		*_ext = 0;
}
void XRayBlenderCompiler::Compile(XRayShader& shader, const char* textures )
{
	m_textures.clear();
	if (textures && textures[0])
	{
		char* P = (char*)textures;
		svector<char, 128> N;

		while (*P)
		{
			if (*P == ',')
			{
				// flush
				N.push_back(0);
				strlwr(N.begin());

				FixTextureName(N.begin());
				m_textures.push_back(N.begin());
				N.clear();
			}
			else
			{
				N.push_back(*P);
			}
			P++;
		}
		if (N.size())
		{
			// flush
			N.push_back(0);
			strlwr(N.begin());

			FixTextureName(N.begin());
			m_textures.push_back(N.begin());
		}
	}
	{
		DetailName = 0;
	}
	{
		bDetail = UseDetail() && GTextureDescrManager->GetDetailTexture(m_textures[0], DetailName, DetailScalar);
	}
	bool bUseMeshShader = UseMeshShader() && HW->MeshShaderSupport();;
	for (size_t x = 0; x <2 ; x++)
	{
		IDSlot = x;
		int slot_pipeline = GetSlotPipelineForShader(x);
		if (slot_pipeline >= 0)
		{
			for (size_t i = 0; i < 5; i++)
			{
			

				IDShader = i;
				shader.E[x][i].RootSignature = RootSignature[slot_pipeline][i];
				if(!m_pipelines[slot_pipeline][i].Container.empty())
					shader.E[x][i].Pipeline = GResourcesManager->GetPipeline(m_pipelines[slot_pipeline][i]);
				Compile(shader.E[x][i]);

			}
		}
	}
	shader.MeshPipeline = bUseMeshShader;
}



int XRayBlenderCompiler::GetSlotPipelineForShader(size_t slot)
{
	if(slot==0)
		return 0;
	return -1;
}

void XRayBlenderCompiler::SetInputLayout(BearPipelineGraphicsDescription& Description, u32 VertexState)
{
	switch (VertexState)
	{
	case FVF::F_0W:
	case FVF::F_1W:
	{
		Description.InputLayout.Elements[0] = BearInputLayoutElement("position", BearVertexFormat::R32G32B32A32_FLOAT, 0);
		Description.InputLayout.Elements[1] = BearInputLayoutElement("normal", BearVertexFormat::R8G8B8A8, 16);
		Description.InputLayout.Elements[2] = BearInputLayoutElement("tangent", BearVertexFormat::R8G8B8A8, 20);
		Description.InputLayout.Elements[3] = BearInputLayoutElement("binormal", BearVertexFormat::R8G8B8A8, 24);
		Description.InputLayout.Elements[4] = BearInputLayoutElement("uv1t", BearVertexFormat::R32G32_FLOAT, 28);
		Description.InputLayout.Elements[5] = BearInputLayoutElement();

	}
	return;
	break;
	case FVF::F_2W:
	{
		Description.InputLayout.Elements[0] = BearInputLayoutElement("position", BearVertexFormat::R32G32B32A32_FLOAT, 0);
		Description.InputLayout.Elements[1] = BearInputLayoutElement("normal", BearVertexFormat::R8G8B8A8, 16);
		Description.InputLayout.Elements[2] = BearInputLayoutElement("tangent", BearVertexFormat::R8G8B8A8, 20);
		Description.InputLayout.Elements[3] = BearInputLayoutElement("binormal", BearVertexFormat::R8G8B8A8, 24);
		Description.InputLayout.Elements[4] = BearInputLayoutElement("uv1t", BearVertexFormat::R32G32B32A32_FLOAT, 28);
		Description.InputLayout.Elements[5] = BearInputLayoutElement();
		return;
		break;
	}
	case FVF::F_3W:
	{
		Description.InputLayout.Elements[0] = BearInputLayoutElement("position", BearVertexFormat::R32G32B32A32_FLOAT, 0);
		Description.InputLayout.Elements[1] = BearInputLayoutElement("normal", BearVertexFormat::R8G8B8A8, 16);
		Description.InputLayout.Elements[2] = BearInputLayoutElement("tangent", BearVertexFormat::R8G8B8A8, 20);
		Description.InputLayout.Elements[3] = BearInputLayoutElement("binormal", BearVertexFormat::R8G8B8A8, 24);
		Description.InputLayout.Elements[4] = BearInputLayoutElement("uv1t", BearVertexFormat::R32G32B32A32_FLOAT, 28);
		Description.InputLayout.Elements[5] = BearInputLayoutElement();
	}
	return;
	break;
	case FVF::F_4W:
	{
		Description.InputLayout.Elements[0] = BearInputLayoutElement("position", BearVertexFormat::R32G32B32A32_FLOAT, 0);
		Description.InputLayout.Elements[1] = BearInputLayoutElement("normal", BearVertexFormat::R8G8B8A8, 16);
		Description.InputLayout.Elements[2] = BearInputLayoutElement("tangent", BearVertexFormat::R8G8B8A8, 20);
		Description.InputLayout.Elements[3] = BearInputLayoutElement("binormal", BearVertexFormat::R8G8B8A8, 24);
		Description.InputLayout.Elements[4] = BearInputLayoutElement("uv1t", BearVertexFormat::R32G32_FLOAT, 28);
		Description.InputLayout.Elements[5] = BearInputLayoutElement("indices_t", BearVertexFormat::R8G8B8A8, 36);
		Description.InputLayout.Elements[6] = BearInputLayoutElement();
	}
	return;
	break;
	case FVF::F_R1LMap:
	{
		Description.InputLayout.Elements[0] = BearInputLayoutElement("position", BearVertexFormat::R32G32B32_FLOAT, 0);
		Description.InputLayout.Elements[1] = BearInputLayoutElement("normal", BearVertexFormat::R8G8B8A8, 12);
		Description.InputLayout.Elements[2] = BearInputLayoutElement("tangent", BearVertexFormat::R8G8B8A8, 16);
		Description.InputLayout.Elements[3] = BearInputLayoutElement("binormal", BearVertexFormat::R8G8B8A8, 20);
		Description.InputLayout.Elements[4] = BearInputLayoutElement("uv1t", BearVertexFormat::R16G16B16A16_SINT, 24);
		Description.InputLayout.Elements[5] = BearInputLayoutElement();

	}
	return;
	break;
	case FVF::F_R1Vert:
	{
		Description.InputLayout.Elements[0] = BearInputLayoutElement("position", BearVertexFormat::R32G32B32_FLOAT, 0);
		Description.InputLayout.Elements[1] = BearInputLayoutElement("normal", BearVertexFormat::R8G8B8A8, 12);
		Description.InputLayout.Elements[2] = BearInputLayoutElement("tangent", BearVertexFormat::R8G8B8A8, 16);
		Description.InputLayout.Elements[3] = BearInputLayoutElement("binormal", BearVertexFormat::R8G8B8A8, 20);
		Description.InputLayout.Elements[4] = BearInputLayoutElement("color", BearVertexFormat::R8G8B8A8, 24);
		Description.InputLayout.Elements[5] = BearInputLayoutElement("uv1t", BearVertexFormat::R16G16_SINT, 28);
		Description.InputLayout.Elements[6] = BearInputLayoutElement();
	}
	return;
	break;
	case FVF::F_XVert:
	{
		Description.InputLayout.Elements[0] = BearInputLayoutElement("position", BearVertexFormat::R32G32B32_FLOAT, 0);
		Description.InputLayout.Elements[1] = BearInputLayoutElement();

	}
	return;
	break;
	case FVF::F_MUModel:
	{
		Description.InputLayout.Elements[0] = BearInputLayoutElement("position", BearVertexFormat::R32G32B32_FLOAT, 0);
		Description.InputLayout.Elements[1] = BearInputLayoutElement("normal", BearVertexFormat::R8G8B8A8, 12);
		Description.InputLayout.Elements[2] = BearInputLayoutElement("tangent", BearVertexFormat::R8G8B8A8, 16);
		Description.InputLayout.Elements[3] = BearInputLayoutElement("binormal", BearVertexFormat::R8G8B8A8, 20);
		Description.InputLayout.Elements[4] = BearInputLayoutElement("uv1t", BearVertexFormat::R16G16B16A16_SINT, 24);
		Description.InputLayout.Elements[5] = BearInputLayoutElement();

	}

	return;
	case FVF::F_LOD:
	{
		Description.InputLayout.Elements[0] = BearInputLayoutElement("position0t", BearVertexFormat::R32G32B32_FLOAT, 0);
		Description.InputLayout.Elements[1] = BearInputLayoutElement("position1t", BearVertexFormat::R32G32B32_FLOAT, 12);

		Description.InputLayout.Elements[2] = BearInputLayoutElement("normal0t", BearVertexFormat::R32G32B32_FLOAT, 24);
		Description.InputLayout.Elements[3] = BearInputLayoutElement("normal1t", BearVertexFormat::R32G32B32_FLOAT, 36);

		Description.InputLayout.Elements[4] = BearInputLayoutElement("sun_af", BearVertexFormat::R8G8B8A8, 48);

		Description.InputLayout.Elements[5] = BearInputLayoutElement("uv0t", BearVertexFormat::R32G32_FLOAT, 52);
		Description.InputLayout.Elements[6] = BearInputLayoutElement("uv1t", BearVertexFormat::R32G32_FLOAT, 60);

		Description.InputLayout.Elements[7] = BearInputLayoutElement("rgbh0t", BearVertexFormat::R8G8B8A8, 68);
		Description.InputLayout.Elements[8] = BearInputLayoutElement("rgbh1t", BearVertexFormat::R8G8B8A8, 72);
		Description.InputLayout.Elements[9] = BearInputLayoutElement();
	}

	return;
	case FVF::F_V_BUILD:
	{
		Description.InputLayout.Elements[0] = BearInputLayoutElement("position", BearVertexFormat::R32G32B32A32_FLOAT, 0);
		Description.InputLayout.Elements[1] = BearInputLayoutElement("uv0t", BearVertexFormat::R32G32_FLOAT, 16);
		Description.InputLayout.Elements[2] = BearInputLayoutElement("uv1t", BearVertexFormat::R32G32_FLOAT, 24);
		Description.InputLayout.Elements[3] = BearInputLayoutElement("uv2t", BearVertexFormat::R32G32_FLOAT, 32);
		Description.InputLayout.Elements[4] = BearInputLayoutElement("uv3t", BearVertexFormat::R32G32_FLOAT, 40);
		Description.InputLayout.Elements[5] = BearInputLayoutElement();

	}
	return;
	case FVF::F_V_FILTER:
	{
		Description.InputLayout.Elements[0] = BearInputLayoutElement("position", BearVertexFormat::R32G32B32A32_FLOAT, 0);
		Description.InputLayout.Elements[1] = BearInputLayoutElement("uv0t", BearVertexFormat::R32G32B32A32_FLOAT, 16);
		Description.InputLayout.Elements[2] = BearInputLayoutElement("uv1t", BearVertexFormat::R32G32B32A32_FLOAT, 32);
		Description.InputLayout.Elements[3] = BearInputLayoutElement("uv2t", BearVertexFormat::R32G32B32A32_FLOAT, 48);
		Description.InputLayout.Elements[4] = BearInputLayoutElement("uv3t", BearVertexFormat::R32G32B32A32_FLOAT, 64);
		Description.InputLayout.Elements[5] = BearInputLayoutElement("uv4t", BearVertexFormat::R32G32B32A32_FLOAT, 80);
		Description.InputLayout.Elements[6] = BearInputLayoutElement("uv5t", BearVertexFormat::R32G32B32A32_FLOAT, 96);
		Description.InputLayout.Elements[7] = BearInputLayoutElement("uv6t", BearVertexFormat::R32G32B32A32_FLOAT, 112);
		Description.InputLayout.Elements[8] = BearInputLayoutElement("uv7t", BearVertexFormat::R32G32B32A32_FLOAT, 128);
		Description.InputLayout.Elements[9] = BearInputLayoutElement();

	}
	return;
	default:
		break;
	};
	int Count = 0;
	struct
	{
		int bits;
		size_t size;
		BearVertexFormat type;
		const char* name;
	}

	StrideArray[] =
	{
#define REGISTER(name,byte,stride,sh_name,sh_type) {byte,stride,sh_type,sh_name},
#include "../D3D9VertexState.h"
#undef REGISTER
	};

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

	for (size_t i = 0; i < D3DFVF_COUNT; i++)
	{
		if ((StrideArray[i].bits == D3DFVF_TEX4) && texture_uv_count >= 4)
		{
			Description.InputLayout.Elements[Count].Name = StrideArray[i].name;
			if ((D3DFVF_TEXCOORDSIZE1(3) & int(VertexState)) == D3DFVF_TEXCOORDSIZE1(3))
			{
				Description.InputLayout.Elements[Count].Type = BearVertexFormat::R32_FLOAT;
			}
			else if ((D3DFVF_TEXCOORDSIZE3(3) & int(VertexState)) == D3DFVF_TEXCOORDSIZE3(3))
			{
				Description.InputLayout.Elements[Count].Type = BearVertexFormat::R32G32B32_FLOAT;
			}
			else if ((D3DFVF_TEXCOORDSIZE4(3) & int(VertexState)) == D3DFVF_TEXCOORDSIZE4(3))
			{
				Description.InputLayout.Elements[Count].Type = BearVertexFormat::R32G32B32A32_FLOAT;
			}
			else
			{
				Description.InputLayout.Elements[Count].Type = BearVertexFormat::R32G32_FLOAT;
			}
			if (Count == 0)
			{
				Description.InputLayout.Elements[Count].Offset = 0;
			}
			else
			{
				Description.InputLayout.Elements[Count].Offset = Description.InputLayout.Elements[Count - 1].Offset + GetSize(Description.InputLayout.Elements[Count - 1].Type);
			}
			Count += 1;
		}
		else if ((StrideArray[i].bits == D3DFVF_TEX3) && texture_uv_count >= 3)
		{
			Description.InputLayout.Elements[Count].Name = StrideArray[i].name;
			if ((D3DFVF_TEXCOORDSIZE1(2) & int(VertexState)) == D3DFVF_TEXCOORDSIZE1(2))
			{
				Description.InputLayout.Elements[Count].Type = BearVertexFormat::R32_FLOAT;
			}
			else if ((D3DFVF_TEXCOORDSIZE3(2) & int(VertexState)) == D3DFVF_TEXCOORDSIZE3(2))
			{
				Description.InputLayout.Elements[Count].Type = BearVertexFormat::R32G32B32_FLOAT;
			}
			else if ((D3DFVF_TEXCOORDSIZE4(2) & int(VertexState)) == D3DFVF_TEXCOORDSIZE4(2))
			{
				Description.InputLayout.Elements[Count].Type = BearVertexFormat::R32G32B32A32_FLOAT;
			}
			else
			{
				Description.InputLayout.Elements[Count].Type = BearVertexFormat::R32G32_FLOAT;
			}
			if (Count == 0)
			{
				Description.InputLayout.Elements[Count].Offset = 0;
			}
			else
			{
				Description.InputLayout.Elements[Count].Offset = Description.InputLayout.Elements[Count - 1].Offset + GetSize(Description.InputLayout.Elements[Count - 1].Type);
			}
			Count += 1;
		}
		else if ((StrideArray[i].bits  == D3DFVF_TEX2) && texture_uv_count >= 2)
		{
			Description.InputLayout.Elements[Count].Name = StrideArray[i].name;
			if ((D3DFVF_TEXCOORDSIZE1(1) & int(VertexState)) == D3DFVF_TEXCOORDSIZE1(1))
			{
				Description.InputLayout.Elements[Count].Type = BearVertexFormat::R32_FLOAT;
			}
			else if ((D3DFVF_TEXCOORDSIZE3(1) & int(VertexState)) == D3DFVF_TEXCOORDSIZE3(1))
			{
				Description.InputLayout.Elements[Count].Type = BearVertexFormat::R32G32B32_FLOAT;
			}
			else if ((D3DFVF_TEXCOORDSIZE4(1) & int(VertexState)) == D3DFVF_TEXCOORDSIZE4(1))
			{
				Description.InputLayout.Elements[Count].Type = BearVertexFormat::R32G32B32A32_FLOAT;
			}
			else
			{
				Description.InputLayout.Elements[Count].Type = BearVertexFormat::R32G32_FLOAT;
			}
			if (Count == 0)
			{
				Description.InputLayout.Elements[Count].Offset = 0;
			}
			else
			{
				Description.InputLayout.Elements[Count].Offset = Description.InputLayout.Elements[Count - 1].Offset + GetSize(Description.InputLayout.Elements[Count - 1].Type);
			}
			Count += 1;
		}
		
		else  if ((StrideArray[i].bits == D3DFVF_TEX1) && texture_uv_count >= 1)
		{

			Description.InputLayout.Elements[Count].Name = StrideArray[i].name;

			if ((D3DFVF_TEXCOORDSIZE1(0) & int(VertexState)) == D3DFVF_TEXCOORDSIZE1(0))
			{

				Description.InputLayout.Elements[Count].Type = BearVertexFormat::R32_FLOAT;
			}
			else if ((D3DFVF_TEXCOORDSIZE3(0) & int(VertexState)) == D3DFVF_TEXCOORDSIZE3(0))
			{

				Description.InputLayout.Elements[Count].Type = BearVertexFormat::R32G32B32_FLOAT;
			}
			else if ((D3DFVF_TEXCOORDSIZE4(0) & int(VertexState)) == D3DFVF_TEXCOORDSIZE4(0))
			{

				Description.InputLayout.Elements[Count].Type = BearVertexFormat::R32G32B32A32_FLOAT;
			}
			else
			{

				Description.InputLayout.Elements[Count].Type = BearVertexFormat::R32G32_FLOAT;
			}
			if (Count == 0)
			{
				Description.InputLayout.Elements[Count].Offset = 0;
			}
			else
			{
				Description.InputLayout.Elements[Count].Offset = Description.InputLayout.Elements[Count - 1].Offset + GetSize(Description.InputLayout.Elements[Count - 1].Type);
			}
			Count += 1;
		}
		else
		{
			if ((StrideArray[i].bits & int(VertexState)) == StrideArray[i].bits)
			{
				Description.InputLayout.Elements[Count].Type = StrideArray[i].type;
				Description.InputLayout.Elements[Count].Name = StrideArray[i].name;
				if (Count == 0)
					Description.InputLayout.Elements[Count].Offset = 0;
				else
				{
					Description.InputLayout.Elements[Count].Offset = Description.InputLayout.Elements[Count - 1].Offset + GetSize(Description.InputLayout.Elements[Count - 1].Type);
				}
				Count += 1;
			}
		}
		

	}
	if (Count != 16)
	{
		for (size_t i = Count; i < 16; i++)
			Description.InputLayout.Elements[i] = BearInputLayoutElement();
	}
}



void XRayBlenderCompiler::SetTexture(XRayShaderElement& shader, size_t id, shared_str name)
{
	shader.Textures[id] = GResourcesManager->GetTexture(GetTexture(name));
}

void XRayBlenderCompiler::SetTexture(XRayShaderElement& shader, size_t id, const char* name1, const char* name2)
{
	string512 name;
	xr_strcpy(name, GetTexture(name1).c_str());
	xr_strcat(name, name2);
	shader.Textures[id] = GResourcesManager->GetTexture(name);
}

void XRayBlenderCompiler::CreatePipeline( size_t slot, size_t shader_id, BearPipelineGraphicsDescription& Description, shared_str name_vs, shared_str name_ps, ShaderVertexDeclaration FVFType)
{
	u32 fvf = 0;
	switch (FVFType)
	{
#define ADD_SVD(x) case  SVD_ ## x: fvf = FVF::F_ ## x; break;
#include "../Resources/XRaySVDList.h"
#undef ADD_SVD	
	
	default:
		R_ASSERT(0);
		break;
	}
	/*string_path NameVS;
	sprintf(NameVS, TEXT("%s%s"), name_vs, prefix);
	xr_strlwr(NameVS);*/
	SetInputLayout(Description, fvf);
	Description.Shaders.Pixel = GResourcesManager->GetPixelShader(name_ps);
	Description.Shaders.Vertex = GResourcesManager->GetVertexShader(name_vs);
	Description.RootSignature = RootSignature[slot][shader_id];


	R_ASSERT(m_pipelines[slot][shader_id].Container.find(FVFType) == m_pipelines[slot][shader_id].Container.end());
	m_pipelines[slot][shader_id].Container[FVFType] = GResourcesManager->CreatePipeline(Description);
	/*R_ASSERT(m_pipeline[id][slot][shader_id][FVFType].empty());
	m_pipeline[id][slot][shader_id][FVFType]*/

}

void XRayBlenderCompiler::CreatePipeline(size_t slot, size_t shader_id, BearPipelineMeshDescription& Description, shared_str name_ms, shared_str name_ps, ShaderVertexDeclaration FVFType)
{
	u32 fvf = 0;
	switch (FVFType)
	{
#define ADD_SVD(x) case  SVD_ ## x: fvf = FVF::F_ ## x; break;
#include "../Resources/XRaySVDList.h"
#undef ADD_SVD	

	default:
		R_ASSERT(0);
		break;
	}
	Description.Shaders.Pixel = GResourcesManager->GetPixelShader(name_ps);
	Description.Shaders.Mesh = GResourcesManager->GetMeshShader(name_ms);
	Description.RootSignature = RootSignature[slot][shader_id];


	R_ASSERT(m_pipelines[slot][shader_id].Container.find(FVFType) == m_pipelines[slot][shader_id].Container.end());
	m_pipelines[slot][shader_id].Container[FVFType] = GResourcesManager->CreatePipeline(Description);
}




inline int ParseName(LPCSTR N)
{
	if (0 == strcmp(N, "$null"))
		return -1;
	if (0 == strcmp(N, "$base0"))
		return 0;
	if (0 == strcmp(N, "$base1"))
		return 1;
	if (0 == strcmp(N, "$base2"))
		return 2;
	if (0 == strcmp(N, "$base3"))
		return 3;
	if (0 == strcmp(N, "$base4"))
		return 4;
	if (0 == strcmp(N, "$base5"))
		return 5;
	if (0 == strcmp(N, "$base6"))
		return 6;
	if (0 == strcmp(N, "$base7"))
		return 7;
	return -1;
}

shared_str XRayBlenderCompiler::GetPixelShaderName(const char* name1, bool detail, bool aref, bool blend)
{
	string512 result;
	xr_strcpy(result, name1);
	if (detail)
	{
		xr_strcat(result, "_dt");
	}
	if (aref)
	{
		xr_strcat(result, "_aref");
	}
	if (blend)
	{
		xr_strcat(result, "_blend");
	}
	return result;
}

shared_str XRayBlenderCompiler::GetVertexShaderName(const char* name1, bool detail,bool blend)
{
	string512 result;
	xr_strcpy(result, name1);
	if (detail)
	{
		xr_strcat(result, "_dt");
	}
	if (blend)
	{
		xr_strcat(result, "_blend");
	}
	return result;
}

shared_str XRayBlenderCompiler::GetTexture(shared_str name)
{
	int id = ParseName(name.c_str());
	if (id >= 0&&id<m_textures.size())
	{
		return m_textures[id];
	}
	return name;
}

/*
void XRayBlender::Compile(XRayShader & shader)
{
	R_ASSERT(false);
}
*/