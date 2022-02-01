#ifndef EnvironmentH
#define EnvironmentH

// refs
class ENGINE_API	IRender_Visual;
class ENGINE_API	CInifile;
class ENGINE_API 	CEnvironment;

// refs - effects
class ENGINE_API	CEnvironment;
class ENGINE_API	CLensFlare;
class ENGINE_API	CEffect_Rain;
class ENGINE_API	CEffect_Thunderbolt;

class ENGINE_API	CPerlinNoise1D;

struct SThunderboltDesc;
struct SThunderboltCollection;
class CLensFlareDescriptor;

#define DAY_LENGTH		86400.f

#include "FactoryPtr.h"
#include "EnvironmentBase.h"
#include "EnvironmentRender.h"

#ifdef INGAME_EDITOR
#	define	INGAME_EDITOR_VIRTUAL	virtual
#else // #ifdef INGAME_EDITOR
#	define	INGAME_EDITOR_VIRTUAL
#endif // #ifdef INGAME_EDITOR

// t-defs
class ENGINE_API	CEnvModifier:public IEnvModifier
{
public:
	Flags16				use_flags;

	virtual void				load		(IReader* fs, u32 version);
	virtual float				sum			(IEnvModifier&	_another, Fvector3& view);
};

class ENGINE_API	CEnvAmbient : public IEnvAmbient
{
public:
	DEFINE_VECTOR(SEffect*,EffectVec,EffectVecIt);
	struct SSndChannel
	{
		shared_str				m_load_section;
		Fvector2				m_sound_dist;
		Ivector4				m_sound_period;

		typedef xr_vector<ref_sound>	sounds_type;

		virtual	void					load					(CInifile& config, LPCSTR sect);
		virtual ref_sound&				get_rnd_sound			()	{return sounds()[Random.randI(sounds().size())];}
		virtual u32						get_rnd_sound_time		()	{return (m_sound_period.z < m_sound_period.w) ? Random.randI(m_sound_period.z,m_sound_period.w) : 0;}
		virtual u32						get_rnd_sound_first_time()	{return (m_sound_period.x < m_sound_period.y) ? Random.randI(m_sound_period.x,m_sound_period.y) : 0;}
		virtual float					get_rnd_sound_dist		()	{return (m_sound_dist.x < m_sound_dist.y) ? Random.randF(m_sound_dist.x, m_sound_dist.y) : 0;}
		INGAME_EDITOR_VIRTUAL	~SSndChannel			()	{}
		inline INGAME_EDITOR_VIRTUAL sounds_type& sounds()  {return m_sounds;}

	protected:
		xr_vector<ref_sound>	m_sounds;
	};
	DEFINE_VECTOR(SSndChannel*,SSndChannelVec,SSndChannelVecIt);
protected:
	shared_str				m_load_section;

	EffectVec				m_effects;
	Ivector2				m_effect_period;

	SSndChannelVec			m_sound_channels;
	shared_str              m_ambients_config_filename;

public:
	IC const shared_str&	name				()	{return m_load_section;}
	IC const shared_str&	get_ambients_config_filename ()	{return m_ambients_config_filename;}

	INGAME_EDITOR_VIRTUAL	void	load		(
								CInifile& ambients_config,
								CInifile& sound_channels_config,
								CInifile& effects_config,
								const shared_str& section
							);
	virtual SEffect*				get_rnd_effect		()	{return effects().empty()?0:effects()[Random.randI(effects().size())];}
	virtual u32					get_rnd_effect_time ()	{return Random.randI(m_effect_period.x, m_effect_period.y);}

	INGAME_EDITOR_VIRTUAL	SEffect*		create_effect			(CInifile& config, LPCSTR id);
	INGAME_EDITOR_VIRTUAL	SSndChannel*	create_sound_channel	(CInifile& config, LPCSTR id);
	INGAME_EDITOR_VIRTUAL					~CEnvAmbient			();
							void			destroy					();
	inline INGAME_EDITOR_VIRTUAL EffectVec&			effects			() { return m_effects; }
	inline INGAME_EDITOR_VIRTUAL SSndChannelVec&	get_snd_channels() { return m_sound_channels; }
};

class ENGINE_API	CEnvDescriptor:public virtual IEnvDescriptor
{
public:
	

	float m_fSunShaftsIntensity;
	float m_fWaterIntensity;
						CEnvDescriptor	(shared_str const& identifier);

	virtual void				load			(CEnvironment& environment, CInifile& config);
	virtual void				copy			(const IEnvDescriptor& src)
	{
		float tm0		= exec_time;
		float tm1		= exec_time_loaded; 
		*this			= *dynamic_cast<const CEnvDescriptor*>(&src);
		exec_time		= tm0;
		exec_time_loaded= tm1;
	}

	virtual void				on_device_create	();
	virtual void				on_device_destroy	();

	shared_str			m_identifier;
};

class ENGINE_API		CEnvDescriptorMixer : public CEnvDescriptor, public IEnvDescriptorMixer {
public:
	/*
	STextureList		sky_r_textures;
	STextureList		sky_r_textures_env;
	STextureList		clouds_r_textures;
	*/

public:
	CEnvDescriptorMixer(shared_str const& identifier);
	virtual void lerp(IEnvironment* parent, IEnvDescriptor& A, IEnvDescriptor& B, float f, IEnvModifier& M, float m_power);
	virtual	void clear();
	virtual void destroy();

};

class ENGINE_API	CEnvironment :public IEnvironment
{
	friend class dxEnvironmentRender;
	friend class XRayEnvironmentRender;
private:
	float					NormalizeTime(float tm);
	float					TimeDiff(float prev, float cur);
	float					TimeWeight(float val, float min_t, float max_t);
	void					SelectEnvs(EnvVec* envs, IEnvDescriptor*& e0, IEnvDescriptor*& e1, float tm);
	void					SelectEnv(EnvVec* envs, IEnvDescriptor*& e, float tm);

	void					calculate_dynamic_sun_dir();
public:
	CEnvDescriptor*			GetCurrentDescriptor(size_t i) { return dynamic_cast<CEnvDescriptor*>(Current[i]); }

	FactoryPtr<IEnvironmentRender>	m_pRender;



	// wind blast params
	float					wind_blast_strength;
	Fvector					wind_blast_direction;
	Fquaternion				wind_blast_start_time;
	Fquaternion				wind_blast_stop_time;
	float					wind_blast_strength_start_value;
	float					wind_blast_strength_stop_value;
	Fquaternion				wind_blast_current;
	// Environments
	BENCH_SEC_SCRAMBLEMEMBER2




		virtual void					SelectEnvs(float gt);

	virtual IEnvAmbient* AppendEnvAmb(const shared_str& sect);

	virtual void					Invalidate();
public:
	CEnvironment();

	virtual	~CEnvironment();

	virtual void	load();
	virtual void	unload();

	virtual void					mods_load();
	virtual void					mods_unload();

	virtual void					OnFrame();
	void					lerp(float& current_weight);

	virtual void					RenderSky();
	virtual void					RenderClouds();
	virtual void					RenderFlares();
	virtual void					RenderLast();

	virtual bool					SetWeatherFX(shared_str name);
	bool					StartWeatherFXFromTime(shared_str name, float time);
	virtual bool					IsWFXPlaying() { return bWFX; }
	void					StopWFX();

	virtual void					SetWeather(shared_str name, bool forced = false);
	virtual  shared_str				GetWeather() { return CurrentWeatherName; }
	void					ChangeGameTime(float game_time);
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
#else // #ifdef _EDITOR
#	ifdef INGAME_EDITOR
	float				GetGameTime() { return fGameTime; }
#	endif // #ifdef INGAME_EDITOR

	bool					m_paused;
#endif // #ifdef _EDITOR

	CInifile* m_ambients_config;
	CInifile* m_sound_channels_config;
	CInifile* m_effects_config;
	CInifile* m_suns_config;
	CInifile* m_thunderbolt_collections_config;
	CInifile* m_thunderbolts_config;

protected:
	INGAME_EDITOR_VIRTUAL	CEnvDescriptor* create_descriptor(shared_str const& identifier, CInifile* config);
	INGAME_EDITOR_VIRTUAL	void load_weathers();
	INGAME_EDITOR_VIRTUAL	void load_weather_effects();
	INGAME_EDITOR_VIRTUAL	void create_mixer();
	void destroy_mixer();

	void load_level_specific_ambients();

public:
	INGAME_EDITOR_VIRTUAL	SThunderboltDesc* thunderbolt_description(CInifile& config, shared_str const& section);
	INGAME_EDITOR_VIRTUAL	SThunderboltCollection* thunderbolt_collection(CInifile* pIni, CInifile* thunderbolts, LPCSTR section);
	INGAME_EDITOR_VIRTUAL	SThunderboltCollection* thunderbolt_collection(xr_vector<SThunderboltCollection*>& collection, shared_str const& id);
	INGAME_EDITOR_VIRTUAL	CLensFlareDescriptor* add_flare(xr_vector<CLensFlareDescriptor*>& collection, shared_str const& id);

public:
	float						p_var_alt;
	float						p_var_long;
	float						p_min_dist;
	float						p_tilt;
	float						p_second_prop;
	float						p_sky_color;
	float						p_sun_color;
	float						p_fog_color;
};

#undef INGAME_EDITOR_VIRTUAL

ENGINE_API extern Flags32	psEnvFlags;
ENGINE_API extern float		psVisDistance;

#endif //EnvironmentH