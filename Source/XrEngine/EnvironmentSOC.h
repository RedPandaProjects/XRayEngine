#ifndef EnvironmentSOCH
#define EnvironmentSOCH

// refs
class ENGINE_API	IRender_Visual;
class ENGINE_API	CInifile;

// refs - effects
class ENGINE_API	CLensFlare;	
class ENGINE_API	CEffect_Rain;
class ENGINE_API	CEffect_Thunderbolt;

class ENGINE_API	CPerlinNoise1D;

#include "EnvironmentBase.h"
#define DAY_LENGTH		86400.f

class ENGINE_API	CEnvSOCModifier: public IEnvModifier
{
public:

	virtual void				load(IReader* fs, u32 version);
	virtual float				sum(IEnvModifier& _another, Fvector3& view);
};

class ENGINE_API	CEnvSOCAmbient : public IEnvAmbient
{
protected:
	shared_str			section;
	EffectVec			effects;
	xr_vector<ref_sound>sounds;
	Fvector2			sound_dist;
	Ivector2			sound_period;
	Ivector2			effect_period;
public:
	virtual void				load				(const shared_str& section);
	virtual SEffect*			get_rnd_effect		(){return effects.empty()?0:&effects[Random.randI(effects.size())];}
	virtual ref_sound*			get_rnd_sound		(){return sounds.empty()?0:&sounds[Random.randI(sounds.size())];}
	virtual const shared_str&	name				(){return section;}
	virtual u32					get_rnd_sound_time	(){return Random.randI(sound_period.x,sound_period.y);}
	virtual float				get_rnd_sound_dist	(){return Random.randF(sound_dist.x,sound_dist.y);}
	virtual u32					get_rnd_effect_time (){return Random.randI(effect_period.x,effect_period.y);}
};

class ENGINE_API	CEnvSOCDescriptor :public virtual IEnvDescriptor
{
public:

    
#ifdef DEBUG
	shared_str			sect_name;
#endif	
	CEnvSOCDescriptor();

	virtual void				load			(LPCSTR exec_tm, LPCSTR sect, IEnvironment* parent);
	virtual void				copy			(const IEnvDescriptor& src)
	{
		float tm0		= exec_time;
		float tm1		= exec_time_loaded; 
		*this = *dynamic_cast<const CEnvSOCDescriptor*>(&src);
		exec_time		= tm0;
		exec_time_loaded= tm1;
	}

	void				on_device_create	();
	void				on_device_destroy	();
};

class ENGINE_API		CEnvSOCDescriptorMixer: public CEnvSOCDescriptor,public IEnvDescriptorMixer {
public:
	void				lerp			(IEnvironment* parent, IEnvDescriptor& A, IEnvDescriptor& B, float f, IEnvModifier& M, float m_power);
	void				clear			();
	void				destroy			();
};

class ENGINE_API	CEnvironmentSOC :public IEnvironment
{
private:
	float					NormalizeTime(float tm);
	float					TimeDiff(float prev, float cur);
	float					TimeWeight(float val, float min_t, float max_t);
	void					SelectEnvs(EnvVec* envs, IEnvDescriptor*& e0, IEnvDescriptor*& e1, float tm);
	void					SelectEnv(EnvVec* envs, IEnvDescriptor*& e, float tm);
	void					StopWFX();

public:
	// Environments
	void					SelectEnvs(float gt);

	IEnvAmbient* AppendEnvAmb(const shared_str& sect);

	void					Invalidate();
public:
	CEnvironmentSOC();
	virtual	~CEnvironmentSOC();

	virtual void					load();
	virtual  void					unload();

	virtual void					mods_load();
	virtual void					mods_unload();

	virtual 	void					OnFrame();

	virtual void					RenderSky();
	virtual void					RenderClouds();
	virtual void					RenderFlares();
	virtual void					RenderLast();

	virtual bool					SetWeatherFX(shared_str name);
	virtual void					SetWeather(shared_str name, bool forced = false);
	virtual void					SetGameTime(float game_time, float time_factor);

	virtual void					OnDeviceCreate();
	virtual void					OnDeviceDestroy();

	// editor-related
#ifdef _EDITOR
public:
	float					ed_from_time;
	float					ed_to_time;
public:
	void					ED_Reload();
	float					GetGameTime() { return fGameTime; }
#endif
};

ENGINE_API extern Flags32	psEnvFlags;
ENGINE_API extern float		psVisDistance;

#endif //EnvironmentH
