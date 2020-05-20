#include "pch.h"
#pragma hdrstop
#include "ETextureParams.h"
// eye-params
float					r__dtex_range = 50;


inline void fix_texture_thm_name(LPSTR fn)
{
	LPSTR _ext = strext(fn);
	if (_ext &&
		(0 == stricmp(_ext, ".tga") ||
			0 == stricmp(_ext, ".thm") ||
			0 == stricmp(_ext, ".dds") ||
			0 == stricmp(_ext, ".bmp") ||
			0 == stricmp(_ext, ".ogm")))
		*_ext = 0;
}


void XRayTextureDescrManager::LoadTHM(LPCSTR initial)
{

	FS_FileSet flist;
	FS.file_list(flist, initial, FS_ListFiles, "*.thm");


	auto It = flist.begin();
	auto It_e = flist.end();
	STextureParams			tp;
	string_path				fn;
	xr_vector<float> scalars;
	for (; It != It_e; ++It)
	{

		FS.update_path(fn, initial, It->name.c_str());
		IReader* F = FS.r_open(fn);
		xr_strcpy(fn, It->name.c_str());
		fix_texture_thm_name(fn);

		R_ASSERT(F->find_chunk(THM_CHUNK_TYPE));
		F->r_u32();
		tp.Clear();
		tp.Load(*F);
		FS.r_close(F);
		if (STextureParams::ttImage == tp.type ||
			STextureParams::ttTerrain == tp.type ||
			STextureParams::ttNormalMap == tp.type)
		{
			texture_desc& desc = m_texture_details[fn];
			size_t& dts = m_detail_scalers[fn];

			if (tp.detail_name.size() &&
				tp.flags.is_any(STextureParams::flDiffuseDetail | STextureParams::flBumpDetail))
			{
				if (desc.m_assoc)
					xr_delete(desc.m_assoc);

				desc.m_assoc = xr_new<texture_assoc>();
				desc.m_assoc->detail_name = tp.detail_name;
				dts = scalars.size();;
				scalars.push_back(tp.detail_scale);
			

				desc.m_assoc->usage = 0;

				if (tp.flags.is(STextureParams::flDiffuseDetail))
					desc.m_assoc->usage |= (1 << 0);

				if (tp.flags.is(STextureParams::flBumpDetail))
					desc.m_assoc->usage |= (1 << 1);

			}
			if (desc.m_spec)
				xr_delete(desc.m_spec);

			desc.m_spec = xr_new<texture_spec>();
			desc.m_spec->m_material = tp.material + tp.material_weight;
			desc.m_spec->m_use_steep_parallax = false;

			if (tp.bump_mode == STextureParams::tbmUse)
			{
				desc.m_spec->m_bump_name = tp.bump_name;
			}
			else if (tp.bump_mode == STextureParams::tbmUseParallax)
			{
				desc.m_spec->m_bump_name = tp.bump_name;
				desc.m_spec->m_use_steep_parallax = true;
			}

		}
	}
	if(scalars.size())
	{
		GUniformAllocator->Reserve(XRayUniformAllocator::UT_DetailScalar, scalars.size());
		void* gptr = GUniformAllocator->Uniform[XRayUniformAllocator::UT_DetailScalar]->Lock();
		for(size_t i=0;i<scalars.size();i++)
		{
			R_ASSERT(GUniformAllocator->Alloc(XRayUniformAllocator::UT_DetailScalar) == i);
			u8* ptr = (u8*)gptr + GUniformAllocator->CalcOffset(XRayUniformAllocator::UT_DetailScalar,i);
			Fvector4* in_scalar = (Fvector4*)ptr;
			in_scalar->set(scalars[i], scalars[i], scalars[i], scalars[i]);
		}
		GUniformAllocator->Uniform[XRayUniformAllocator::UT_DetailScalar]->Unlock();
	}
}

void XRayTextureDescrManager::LoadLTX()
{

	/*if (FS.ExistFile("%textures%", "textures.ltx"))
	{
		CInifile			ini("%textures%", "textures.ltx");
		if (ini.section_exist("association"))
		{
			CInifile::Sect& data = ini.r_section("association");
			CInifile::SectCIt I = data.Data.begin();
			CInifile::SectCIt E = data.Data.end();
			for (; I != E; ++I)
			{
				const CInifile::Item& item = *I;

				texture_desc& desc = m_texture_details[item.first];
				desc.m_assoc = xr_new<texture_assoc>();

				string_path				T;
				float					s;

				int res = BearString::Scanf(*item.second, "%[^,],%f", T, &s);
				R_ASSERT(res == 2);
				desc.m_assoc->detail_name = T;
				desc.m_assoc->cs = xr_new<cl_dt_scaler>(s);
				desc.m_assoc->usage = 0;
				if (strstr(item.second.c_str(), "usage[diffuse_or_bump]"))
					desc.m_assoc->usage = (1 << 0) | (1 << 1);
				else
					if (strstr(item.second.c_str(), "usage[bump]"))
						desc.m_assoc->usage = (1 << 1);
					else
						if (strstr(item.second.c_str(), "usage[diffuse]"))
							desc.m_assoc->usage = (1 << 0);
			}
		}//"association"

		if (ini.section_exist("specification"))
		{
			CInifile::Sect& sect = ini.r_section("specification");
			for (CInifile::SectCIt I2 = sect.Data.begin(); I2 != sect.Data.end(); ++I2)
			{
				const CInifile::Item& item = *I2;

				texture_desc& desc = m_texture_details[item.first];
				desc.m_spec = xr_new<texture_spec>();

				string_path				bmode;
				int res = BearString::Scanf(item.second.c_str(), "bump_mode[%[^]]], material[%f]", bmode, &desc.m_spec->m_material);
				R_ASSERT(res == 2);
				if ((bmode[0] == 'u') && (bmode[1] == 's') && (bmode[2] == 'e') && (bmode[3] == ':'))
				{
					// bump-map specified
					desc.m_spec->m_bump_name = bmode + 4;
				}
			}
		}//"specification"
#ifdef _EDITOR
		if (ini.section_exist("types"))
		{
			CInifile::Sect& data = ini.r_section("types");
			for (CInifile::SectCIt I = data.Data.begin(); I != data.Data.end(); I++)
			{
				CInifile::Item& item = *I;

				texture_desc& desc = m_texture_details[item.first];
				desc.m_type = (u16)atoi(item.second.c_str());
			}
		}//"types"
#endif
	}//file-exist*/
}

void XRayTextureDescrManager::Load()
{
#ifdef DEBUG
	BearTimer					TT;
	TT.restart();
#endif // #ifdef DEBUG

	LoadTHM("$textures$");
	if (FS.path_exist("$level$"))
		LoadTHM("$level$");
/*	if (gameVersionController->getGame() == GameVersionController::SOC)
	{
		LoadLTX();
	}*/
#ifdef DEBUG
	Msg("load time=%d ms", TT.get_elapsed_time().asmiliseconds());
#endif // #ifdef DEBUG
}

void XRayTextureDescrManager::UnLoad()
{
	auto I = m_texture_details.begin();
	auto E = m_texture_details.end();
	for (; I != E; ++I)
	{
		xr_delete(I->second.m_assoc);
		xr_delete(I->second.m_spec);
	}
	m_texture_details.clear();
}

XRayTextureDescrManager::~XRayTextureDescrManager()
{
	UnLoad();
/*	auto I = m_detail_scalers.begin();
	auto E = m_detail_scalers.end();

	for (; I != E; ++I)
		xr_delete(I->second);

	m_detail_scalers.clear();*/
}

shared_str XRayTextureDescrManager::GetBumpName(const shared_str& tex_name) const
{
	auto I = m_texture_details.find(tex_name);
	if (I != m_texture_details.end())
	{
		if (I->second.m_spec)
		{
			return I->second.m_spec->m_bump_name;
		}
	}
	return "";
}

BOOL XRayTextureDescrManager::UseSteepParallax(const shared_str& tex_name) const
{
	auto I = m_texture_details.find(tex_name);
	if (I != m_texture_details.end())
	{
		if (I->second.m_spec)
		{
			return I->second.m_spec->m_use_steep_parallax;
		}
	}
	return FALSE;
}

float XRayTextureDescrManager::GetMaterial(const shared_str& tex_name) const
{
	auto I = m_texture_details.find(tex_name);
	if (I != m_texture_details.end())
	{
		if (I->second.m_spec)
		{
			return I->second.m_spec->m_material;
		}
	}
	return 1.0f;
}

void XRayTextureDescrManager::GetTextureUsage(const shared_str& tex_name, BOOL& bDiffuse, BOOL& bBump) const
{
	auto I = m_texture_details.find(tex_name);
	if (I != m_texture_details.end())
	{
		if (I->second.m_assoc)
		{
			u8 usage = I->second.m_assoc->usage;
			bDiffuse = !!(usage & (1 << 0));
			bBump = !!(usage & (1 << 1));
		}
	}
}

BOOL XRayTextureDescrManager::GetDetailTexture(const shared_str& tex_name, LPCSTR& res, size_t& CS) const
{
	auto I = m_texture_details.find(tex_name);
	if (I != m_texture_details.end())
	{
		if (I->second.m_assoc)
		{
			texture_assoc* TA = I->second.m_assoc;
			res = TA->detail_name.c_str();
		/*	if (gameVersionController->getGame() == GameVersionController::SOC)
			{
				CS = TA->cs;
				return TRUE;
			}
			else
			{*/
				auto It2 = m_detail_scalers.find(tex_name);
				CS = It2 == m_detail_scalers.end() ? 0 : It2->second+1;//TA->cs;
				return TRUE;
			/*}*/
		}
	}
	return FALSE;
}

XRayTextureDescrManager* GTextureDescrManager = 0;