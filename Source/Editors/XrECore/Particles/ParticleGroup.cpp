#include "stdafx.h"
#include "psystem.h"
#include "ParticleGroup.h"
#include "ParticleEffectDef.h"
using namespace PS;

//------------------------------------------------------------------------------
CPGDef::CPGDef()
{                             
    m_Flags.zero	();
    m_fTimeLimit	= 0.f;
}

CPGDef::~CPGDef()
{
    for (EffectIt it=m_Effects.begin(); it!=m_Effects.end(); it++)
		xr_delete	(*it);
    m_Effects.clear	();
}

void CPGDef::SetName(LPCSTR name)
{
    m_Name			= name;
}

#if 0
void CPGDef::Clone	(CPGDef* source)
{
	m_Name			= "<invalid_name>";
    m_Flags			= source->m_Flags;
    m_fTimeLimit	= source->m_fTimeLimit;

    m_Effects.resize(source->m_Effects.size(),0);		
    for (EffectIt d_it=m_Effects.begin(),s_it=source->m_Effects.begin(); s_it!=source->m_Effects.end(); s_it++,d_it++)
    	*d_it		= xr_new<SEffect>(**s_it);
}
#endif

//------------------------------------------------------------------------------
// I/O part
//------------------------------------------------------------------------------
BOOL CPGDef::Load(IReader& F)
{
	R_ASSERT		(F.find_chunk(PGD_CHUNK_VERSION));
	u16 version		= F.r_u16();

    if (version!=PGD_VERSION){
		Log			("!Unsupported PG version. Load failed.");
    	return FALSE;
    }

	R_ASSERT		(F.find_chunk(PGD_CHUNK_NAME));
	F.r_stringZ		(m_Name);

	F.r_chunk		(PGD_CHUNK_FLAGS,&m_Flags);

    if (F.find_chunk(PGD_CHUNK_TIME_LIMIT))
   		m_fTimeLimit= F.r_float();
    else
		m_fTimeLimit	= 0.0f;
	
	bool dont_calc_timelimit = m_fTimeLimit > 0.0f;
    if (F.find_chunk(PGD_CHUNK_EFFECTS))
	{
        m_Effects.resize(F.r_u32());
        for (EffectIt it=m_Effects.begin(); it!=m_Effects.end(); it++){
        	*it				= xr_new<SEffect>();
            F.r_stringZ		((*it)->m_EffectName);
            F.r_stringZ		((*it)->m_OnPlayChildName);
        	F.r_stringZ		((*it)->m_OnBirthChildName);
        	F.r_stringZ		((*it)->m_OnDeadChildName);
            (*it)->m_Time0 	= F.r_float();
            (*it)->m_Time1 	= F.r_float();
            (*it)->m_Flags.assign	(F.r_u32());
			
			if(!dont_calc_timelimit)
				m_fTimeLimit	= _max(m_fTimeLimit, (*it)->m_Time1);
        }
    }
    return TRUE;
}                   

BOOL CPGDef::Load2(CInifile& ini)
{
//.	u16 version						= ini.r_u16("_group", "version");
	m_Flags.assign					(ini.r_u32("_group", "flags"));


    m_Effects.resize				(ini.r_u32("_group", "effects_count"));

	u32 counter						= 0;
	string256						buff;
    for (EffectIt it=m_Effects.begin(); it!=m_Effects.end(); ++it,++counter)
	{
    	*it							= xr_new<SEffect>();

		xr_sprintf					(buff, sizeof(buff), "effect_%04d", counter);
        
		(*it)->m_EffectName			= ini.r_string	(buff, "effect_name");
        (*it)->m_OnPlayChildName	= ini.r_string	(buff, "on_play_child");
        (*it)->m_OnBirthChildName	= ini.r_string	(buff, "on_birth_child");
        (*it)->m_OnDeadChildName	= ini.r_string	(buff, "on_death_child");

        (*it)->m_Time0 				= ini.r_float(buff, "time0");
        (*it)->m_Time1 				= ini.r_float(buff, "time1");
        (*it)->m_Flags.assign		(ini.r_u32(buff, "flags"));
	}
   	m_fTimeLimit					= ini.r_float		("_group", "timelimit");
	return							TRUE;
}

void CPGDef::Save(IWriter& F)
{
	F.open_chunk	(PGD_CHUNK_VERSION);
	F.w_u16			(PGD_VERSION);
    F.close_chunk	();

	F.open_chunk	(PGD_CHUNK_NAME);
    F.w_stringZ		(m_Name);
    F.close_chunk	();

	F.w_chunk		(PGD_CHUNK_FLAGS,&m_Flags,sizeof(m_Flags));

	F.open_chunk	(PGD_CHUNK_EFFECTS);
    F.w_u32			(m_Effects.size());
    for (EffectIt it=m_Effects.begin(); it!=m_Effects.end(); it++){
    	F.w_stringZ	((*it)->m_EffectName);
    	F.w_stringZ	((*it)->m_OnPlayChildName);
    	F.w_stringZ	((*it)->m_OnBirthChildName);
    	F.w_stringZ	((*it)->m_OnDeadChildName);
    	F.w_float	((*it)->m_Time0);
    	F.w_float	((*it)->m_Time1);
    	F.w_u32		((*it)->m_Flags.get());
    }
    F.close_chunk	();

    F.open_chunk	(PGD_CHUNK_TIME_LIMIT);
   	F.w_float		(m_fTimeLimit);
    F.close_chunk	();
}

void CPGDef::Save2(CInifile& ini)
{
	ini.w_u16		("_group", "version", PGD_VERSION);

	ini.w_u32		("_group", "flags", m_Flags.get());

    ini.w_u32		("_group", "effects_count", m_Effects.size());

	u32 counter		= 0;
	string256		buff;
    for (EffectIt it=m_Effects.begin(); it!=m_Effects.end(); ++it,++counter)
	{
		xr_sprintf		(buff, sizeof(buff), "effect_%04d", counter);
    	
		ini.w_string	(buff, "effect_name",	(*it)->m_EffectName.c_str());
    	ini.w_string	(buff, "on_play_child", (*it)->m_Flags.test(SEffect::flOnPlayChild)?(*it)->m_OnPlayChildName.c_str():"");
    	ini.w_string	(buff, "on_birth_child",(*it)->m_Flags.test(SEffect::flOnBirthChild)?(*it)->m_OnBirthChildName.c_str():"");
    	ini.w_string	(buff, "on_death_child",(*it)->m_Flags.test(SEffect::flOnDeadChild)?(*it)->m_OnDeadChildName.c_str():"");
    	ini.w_float		(buff, "time0",			(*it)->m_Time0);
    	ini.w_float		(buff, "time1",			(*it)->m_Time1);
    	ini.w_u32		(buff, "flags",			(*it)->m_Flags.get());
    }

   	ini.w_float		("_group", "timelimit", m_fTimeLimit);
}
