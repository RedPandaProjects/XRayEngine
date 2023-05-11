#include "stdafx.h"
#include "ParticleEffectDef.h"
#include "particle_manager.h"


//---------------------------------------------------------------------------
using namespace PAPI;
using namespace PS;

//------------------------------------------------------------------------------
// class CParticleEffectDef
//------------------------------------------------------------------------------
CPEDef::CPEDef()
{                                          
    m_Frame.InitDefault	();
    m_MaxParticles		= 0;
	m_fTimeLimit		= 0.f;
    // collision
    m_fCollideOneMinusFriction 	= 1.f;
    m_fCollideResilience		= 0.f;
    m_fCollideSqrCutoff			= 0.f;
    // velocity scale
    m_VelocityScale.set			(0.f,0.f,0.f);
    // align to path
    m_APDefaultRotation.set		(-PI_DIV_2,0.f,0.f);
	// flags
    m_Flags.zero		();
}

CPEDef::~CPEDef()
{
}
void CPEDef::CreateShader()
{
}
void CPEDef::DestroyShader()
{
}
void CPEDef::SetName(LPCSTR name)
{
    m_Name				= name;
}

/*
void CPEDef::pAlignToPath(float rot_x, float rot_y, float rot_z)
{
	m_Flags.set			(dfAlignToPath,TRUE);
	m_APDefaultRotation.set(rot_x,rot_y,rot_z);
}
void CPEDef::pVelocityScale(float scale_x, float scale_y, float scale_z)
{
	m_Flags.set			(dfVelocityScale,TRUE);
	m_VelocityScale.set	(scale_x, scale_y, scale_z);
}
void CPEDef::pCollision(float friction, float resilience, float cutoff, BOOL destroy_on_contact)
{
	m_fCollideOneMinusFriction 	= 1.f-friction;
	m_fCollideResilience		= resilience;
	m_fCollideSqrCutoff			= cutoff*cutoff;
	m_Flags.set					(dfCollision,TRUE);
	m_Flags.set					(dfCollisionDel,destroy_on_contact);
}

void CPEDef::pSprite(string128& sh_name, string128& tex_name)
{
	xr_free(m_ShaderName);	m_ShaderName	= xr_strdup(sh_name);
	xr_free(m_TextureName);	m_TextureName	= xr_strdup(tex_name);
	m_Flags.set	(dfSprite,TRUE);
}
void CPEDef::pFrame(BOOL random_frame, u32 frame_count, u32 tex_width, u32 tex_height, u32 frame_width, u32 frame_height)
{
	m_Flags.set			(dfFramed,TRUE);
	m_Flags.set			(dfRandomFrame,random_frame);
	m_Frame.Set			(frame_count, (float)tex_width, (float)tex_height, (float)frame_width, (float)frame_height);
}
void CPEDef::pAnimate(float speed, BOOL random_playback)
{
	m_Frame.m_fSpeed	= speed;
	m_Flags.set			(dfAnimated,TRUE);
	m_Flags.set			(dfRandomPlayback,random_playback);
}
void CPEDef::pTimeLimit(float time_limit)
{
	m_Flags.set			(dfTimeLimit,TRUE);
	m_fTimeLimit		= time_limit;
}
*/
void CPEDef::ExecuteAnimate(Particle *particles, u32 p_cnt, float dt)
{
	float speedFac = m_Frame.m_fSpeed * dt;
	for(u32 i = 0; i < p_cnt; i++){
		Particle &m = particles[i];
		float f						= (float(m.frame)/255.f+((m.flags.is(Particle::ANIMATE_CCW))?-1.f:1.f)*speedFac);
		if (f>m_Frame.m_iFrameCount)f-=m_Frame.m_iFrameCount;
		if (f<0.f)					f+=m_Frame.m_iFrameCount;
		m.frame						= (u16)iFloor(f*255.f);
	}
}

//------------------------------------------------------------------------------
// I/O part
//------------------------------------------------------------------------------
BOOL CPEDef::Load(IReader& F)
{
	R_ASSERT		(F.find_chunk(PED_CHUNK_VERSION));
	u16 version		= F.r_u16();

	if (version!=PED_VERSION)
		return FALSE;

	R_ASSERT		(F.find_chunk(PED_CHUNK_NAME));
	F.r_stringZ		(m_Name);

	R_ASSERT		(F.find_chunk(PED_CHUNK_EFFECTDATA));
	m_MaxParticles	= F.r_u32();

	{
		u32 action_list	= F.find_chunk(PED_CHUNK_ACTIONLIST); 
		R_ASSERT(action_list);
		m_Actions.w		(F.pointer(),action_list);
	}

	F.r_chunk		(PED_CHUNK_FLAGS,&m_Flags);

	if (m_Flags.is(dfSprite))
	{
		R_ASSERT	(F.find_chunk(PED_CHUNK_SPRITE));
		F.r_stringZ	(m_ShaderName);
		F.r_stringZ	(m_TextureName);
	}

	if (m_Flags.is(dfFramed))
	{
		R_ASSERT	(F.find_chunk(PED_CHUNK_FRAME));
		F.r			(&m_Frame,sizeof(SFrame));
	}

	if (m_Flags.is(dfTimeLimit))
	{
		R_ASSERT(F.find_chunk(PED_CHUNK_TIMELIMIT));
		m_fTimeLimit= F.r_float();
	}

	if (m_Flags.is(dfCollision))
	{
		R_ASSERT(F.find_chunk(PED_CHUNK_COLLISION));
		m_fCollideOneMinusFriction 	= F.r_float();
		m_fCollideResilience		= F.r_float();
		m_fCollideSqrCutoff			= F.r_float();
	}

	if (m_Flags.is(dfVelocityScale))
	{
		R_ASSERT(F.find_chunk(PED_CHUNK_VEL_SCALE));
		F.r_fvector3				(m_VelocityScale); 
	}

	if (m_Flags.is(dfAlignToPath))
	{
		if (F.find_chunk(PED_CHUNK_ALIGN_TO_PATH)){
			F.r_fvector3			(m_APDefaultRotation);
		}
	}

#if 0
    if (F.find_chunk(PED_CHUNK_EDATA))
	{
		CParticleManager Manager;
        m_EActionList.resize(F.r_u32());
        for (EPAVecIt it=m_EActionList.begin(); it!=m_EActionList.end(); ++it)
		{
            PAPI::PActionEnum type		= (PAPI::PActionEnum)F.r_u32();
            (*it)						= Manager.CreateAction(type);
            (*it)->Load					(F);
        }
		Compile							(m_EActionList);
    } 
#endif

	return TRUE;
}

BOOL CPEDef::Load2(CInifile& ini)
{
//.	u16 version		= ini.r_u16("_effect", "version");
	m_MaxParticles	= ini.r_u32("_effect", "max_particles");
	m_Flags.assign	(ini.r_u32("_effect", "flags"));

	if (m_Flags.is(dfSprite))
	{
		m_ShaderName	= ini.r_string	("sprite", "shader");
		m_TextureName	= ini.r_string	("sprite", "texture");
	}

	if (m_Flags.is(dfFramed))
	{
		m_Frame.m_fTexSize		= ini.r_fvector2		("frame",	"tex_size");
		m_Frame.reserved		= ini.r_fvector2		("frame",	"reserved");
		m_Frame.m_iFrameDimX	= ini.r_s32				("frame",	"dim_x");
		m_Frame.m_iFrameCount	= ini.r_s32				("frame",	"frame_count");
		m_Frame.m_fSpeed		= ini.r_float			("frame",	"speed");
	}

	if (m_Flags.is(dfTimeLimit))
	{
		m_fTimeLimit			= ini.r_float			("timelimit", "value");
	}

	if (m_Flags.is(dfCollision))
	{
		m_fCollideOneMinusFriction	= ini.r_float		("collision", "one_minus_friction");
		m_fCollideResilience		= ini.r_float		("collision", "collide_resilence");
		m_fCollideSqrCutoff			= ini.r_float		("collision", "collide_sqr_cutoff");
	}

	if (m_Flags.is(dfVelocityScale))
	{
		m_VelocityScale				= ini.r_fvector3	("velocity_scale", "value");
	}

	if (m_Flags.is(dfAlignToPath))
	{
		m_APDefaultRotation			= ini.r_fvector3	("align_to_path", "default_rotation");
	}
#if 0
	if(pCreateEAction)
	{
		u32 count		= ini.r_u32("_effect", "action_count");
        m_EActionList.resize(count);
		u32 action_id	= 0;
        for (EPAVecIt it=m_EActionList.begin(); it!=m_EActionList.end(); ++it,++action_id)
		{
			string256					sect;
			xr_sprintf					(sect, sizeof(sect), "action_%04d", action_id);
            PAPI::PActionEnum type		= (PAPI::PActionEnum)(ini.r_u32(sect,"action_type"));
            (*it)						= pCreateEAction(type);
            (*it)->Load2				(ini, sect);
        }
		Compile							(m_EActionList);
    }
#endif

	return TRUE;
}

void CPEDef::Save2(CInifile& ini)
{
	ini.w_u16		("_effect", "version",			PED_VERSION);
//.	ini.w_string	("_effect", "name",				m_Name.c_str());
	ini.w_u32		("_effect", "max_particles",	m_MaxParticles);
//.!!	F.w				(m_Actions.pointer(),m_Actions.size());
	ini.w_u32		("_effect", "flags",				m_Flags.get());

	if (m_Flags.is(dfSprite))
	{
		ini.w_string	("sprite", "shader", m_ShaderName.c_str());
		ini.w_string	("sprite", "texture", m_TextureName.c_str());
	}

	if (m_Flags.is(dfFramed))
	{
		ini.w_fvector2		("frame",	"tex_size", 	m_Frame.m_fTexSize);
		ini.w_fvector2		("frame",	"reserved", 	m_Frame.reserved);
		ini.w_s32			("frame",	"dim_x", 		m_Frame.m_iFrameDimX);
		ini.w_s32			("frame",	"frame_count",	m_Frame.m_iFrameCount);
		ini.w_float			("frame",	"speed",		m_Frame.m_fSpeed);
	}

	if (m_Flags.is(dfTimeLimit))
	{
		ini.w_float		("timelimit", "value", m_fTimeLimit);
	}

	if (m_Flags.is(dfCollision))
	{
		ini.w_float		("collision", "one_minus_friction", m_fCollideOneMinusFriction);	
		ini.w_float		("collision", "collide_resilence", m_fCollideResilience);
		ini.w_float		("collision", "collide_sqr_cutoff", m_fCollideSqrCutoff);
	}

	if (m_Flags.is(dfVelocityScale))
	{
		ini.w_fvector3	("velocity_scale", "value", m_VelocityScale);
	}

	if (m_Flags.is(dfAlignToPath))
	{
		ini.w_fvector3	("align_to_path", "default_rotation", m_APDefaultRotation);
	}
#if 0
    ini.w_u32			("_effect", "action_count", m_EActionList.size());
    u32					action_id = 0;
	for (EPAVecIt it=m_EActionList.begin(); it!=m_EActionList.end(); ++it,++action_id)
	{
		string256		sect;
		xr_sprintf		(sect, sizeof(sect), "action_%04d", action_id);
		ini.w_u32		(sect, "action_type", (*it)->type);
    	(*it)->Save2	(ini, sect);
    }
#endif
}

void CPEDef::Save(IWriter& F)
{
	F.open_chunk	(PED_CHUNK_VERSION);
	F.w_u16			(PED_VERSION);
	F.close_chunk	();

	F.open_chunk	(PED_CHUNK_NAME);
	F.w_stringZ		(m_Name);
	F.close_chunk	();

	F.open_chunk	(PED_CHUNK_EFFECTDATA);
	F.w_u32			(m_MaxParticles);
	F.close_chunk	();

	F.open_chunk	(PED_CHUNK_ACTIONLIST);
	F.w				(m_Actions.pointer(),m_Actions.size());
	F.close_chunk	();

	F.w_chunk		(PED_CHUNK_FLAGS,&m_Flags,sizeof(m_Flags));

	if (m_Flags.is(dfSprite))
	{
		F.open_chunk	(PED_CHUNK_SPRITE);
		F.w_stringZ		(m_ShaderName);
		F.w_stringZ		(m_TextureName);
		F.close_chunk	();
	}

	if (m_Flags.is(dfFramed))
	{
		F.open_chunk	(PED_CHUNK_FRAME);
		F.w				(&m_Frame,sizeof(SFrame));
		F.close_chunk	();
	}

	if (m_Flags.is(dfTimeLimit))
	{
		F.open_chunk	(PED_CHUNK_TIMELIMIT);
		F.w_float		(m_fTimeLimit);
		F.close_chunk	();
	}

	if (m_Flags.is(dfCollision))
	{
		F.open_chunk	(PED_CHUNK_COLLISION);
		F.w_float		(m_fCollideOneMinusFriction);	
		F.w_float		(m_fCollideResilience);
		F.w_float		(m_fCollideSqrCutoff);
		F.close_chunk	();
	}

	if (m_Flags.is(dfVelocityScale))
	{
		F.open_chunk	(PED_CHUNK_VEL_SCALE);
		F.w_fvector3	(m_VelocityScale);
		F.close_chunk	();
	}

	if (m_Flags.is(dfAlignToPath))
	{
		F.open_chunk	(PED_CHUNK_ALIGN_TO_PATH);
		F.w_fvector3	(m_APDefaultRotation);
		F.close_chunk	();
	}
#if 0
	F.open_chunk	(PED_CHUNK_EDATA);
    F.w_u32			(m_EActionList.size());
    for (EPAVecIt it=m_EActionList.begin(); it!=m_EActionList.end(); it++){
        F.w_u32		((*it)->type);
    	(*it)->Save	(F);
    }
	F.close_chunk	();
#endif
}

#if 0
void PS::CPEDef::Compile(EPAVec& v)
{
	m_Actions.clear	();
    m_Actions.w_u32	(v.size());
    int cnt			= 0;
	EPAVecIt it		= v.begin();
	EPAVecIt it_e	= v.end();
    
	for(; it!=it_e; ++it)
	{
		if ((*it)->flags.is(ParticleAction::m_Flags::flEnabled))
		{
    	    (*it)->Compile(m_Actions);
            cnt++;
        }
    }
    m_Actions.seek	(0);
    m_Actions.w_u32 (cnt);
}
#endif

