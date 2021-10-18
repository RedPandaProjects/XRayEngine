#pragma once
#include "FactoryPtr.h"
#include "EnvironmentRender.h"
class IEnvironment;
class 	IEnvModifier
{
public:
	Fvector3			position;
	float				radius;
	float				power;

	float				far_plane;
	Fvector3			fog_color;
	float				fog_density;
	Fvector3			ambient;
	Fvector3			sky_color;
	Fvector3			hemi_color;
public:
	virtual void				load(IReader* fs,u32 version) = 0;
	virtual float				sum(IEnvModifier& _another, Fvector3& view) = 0;
};


class 	IEnvAmbient {
public:
	struct SEffect {
		u32 			life_time;
		ref_sound		sound;
		shared_str		particles;
		Fvector			offset;
		float			wind_gust_factor;
		float			wind_blast_in_time;
		float			wind_blast_out_time;
		float			wind_blast_strength;
		Fvector			wind_blast_direction;
	};
	DEFINE_VECTOR(SEffect, EffectVec, EffectVecIt);
public:
	virtual SEffect*			get_rnd_effect() = 0;
	virtual u32					get_rnd_effect_time() = 0;
	virtual const shared_str&	name() = 0;
};


class ENGINE_API	IEnvDescriptor
{
public:
	shared_str			lens_flare_id;
	shared_str			tb_id;

public:
	FactoryPtr<IEnvDescriptorRender>	m_pDescriptor;

	float				exec_time;
	float				exec_time_loaded;

	shared_str			sky_texture_name;
	shared_str			sky_texture_env_name;
	shared_str			clouds_texture_name;

	Fvector4			clouds_color;
	Fvector3			sky_color;
	float				sky_rotation;

	float				far_plane;

	Fvector3			fog_color;
	float				fog_density;
	float				fog_distance;

	float				rain_density;
	Fvector3			rain_color;

	float				bolt_period;
	float				bolt_duration;

	float				wind_velocity;
	float				wind_direction;

	Fvector3			ambient;
	Fvector4			hemi_color;	// w = R2 correction
	Fvector3			sun_color;
	Fvector3			sun_dir;


	IEnvAmbient* env_ambient;

#ifdef DEBUG
	shared_str			sect_name;
#endif	

	virtual void			copy(const IEnvDescriptor& src) = 0;
	virtual void			on_device_create() = 0;
	virtual void			on_device_destroy() = 0;
};

class ENGINE_API		IEnvDescriptorMixer:public virtual IEnvDescriptor {
public:

	FactoryPtr<IEnvDescriptorMixerRender>	m_pDescriptorMixer;
	float				weight;

	float				fog_near;
	float				fog_far;
public:
	virtual void lerp(IEnvironment* parent, IEnvDescriptor& A, IEnvDescriptor& B, float f, IEnvModifier& M, float m_power) = 0;
	virtual	void clear() = 0;
	virtual void destroy() = 0;
};

class ENGINE_API	IEnvironment
{
protected:
	struct str_pred : public std::binary_function<shared_str, shared_str, bool> {
		IC bool operator()(const shared_str& x, const shared_str& y) const
		{
			return xr_strcmp(x, y) < 0;
		}
	};
public:
	DEFINE_VECTOR(IEnvAmbient*, EnvAmbVec, EnvAmbVecIt);
	DEFINE_VECTOR(IEnvDescriptor*, EnvVec, EnvIt);
	DEFINE_MAP_PRED(shared_str, EnvVec, EnvsMap, EnvsMapIt, str_pred);
	static bool sort_env_pred(const IEnvDescriptor* x, const IEnvDescriptor* y)
	{
		return x->exec_time < y->exec_time;
	}
	static bool sort_env_etl_pred(const IEnvDescriptor* x, const IEnvDescriptor* y)
	{
		return x->exec_time_loaded < y->exec_time_loaded;
	}
protected:
	// clouds
	FvectorVec				CloudsVerts;
	U16Vec					CloudsIndices;
protected:
	FactoryPtr<IEnvironmentRender>	m_pRender;
	CPerlinNoise1D* PerlinNoise1D;

	float					fGameTime;
public:

	BOOL					bNeed_re_create_env;
	float					wind_strength_factor;
	float					wind_gust_factor;
	// Environments
	IEnvDescriptorMixer*CurrentEnv;
	IEnvDescriptor* Current[2];

	bool					bWFX;
	float					wfx_time;
	IEnvDescriptor* WFX_end_desc[2];

	EnvVec* CurrentWeather;
	shared_str				CurrentWeatherName;
	shared_str				CurrentCycleName;

	EnvsMap					WeatherCycles;
	EnvsMap					WeatherFXs;
	xr_vector<IEnvModifier*>	Modifiers;
	EnvAmbVec				Ambients;

	CEffect_Rain* eff_Rain;
	CLensFlare* eff_LensFlare;
	CEffect_Thunderbolt* eff_Thunderbolt;

	float					fTimeFactor;

	virtual void					SelectEnvs(float gt) = 0;

	virtual IEnvAmbient* AppendEnvAmb(const shared_str& sect) = 0;

	virtual void					Invalidate() = 0;
public:
	virtual ~IEnvironment() {}

	virtual void					load() = 0;
	virtual void					unload() = 0;

	virtual void					mods_load() = 0;
	virtual void					mods_unload() = 0;

	virtual void					OnFrame() = 0;

	virtual void					RenderSky() = 0;
	virtual void					RenderClouds() = 0;
	virtual void					RenderFlares() = 0;
	virtual void					RenderLast() = 0;

	virtual bool					SetWeatherFX(shared_str name )= 0;
	virtual bool					IsWFXPlaying() { return bWFX; }
	virtual void					SetWeather(shared_str name, bool forced = false) = 0;
	virtual shared_str				GetWeather() { return CurrentWeatherName; }
	virtual void					SetGameTime(float game_time, float time_factor) = 0;

	virtual void					OnDeviceCreate() = 0;
	virtual void					OnDeviceDestroy() = 0;

};
