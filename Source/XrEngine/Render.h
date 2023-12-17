#ifndef _RENDER_H_
#define _RENDER_H_

#include "../xrCDB/frustum.h"
#include "vis_common.h"
//#include "IRenderDetailModel.h"

#include "../xrAPI/xrAPI.h"
#include "../XrEngine/Render/FactoryPtr.h"
class IUIShader;
typedef FactoryPtr<IUIShader> wm_shader;
//#include "WallMarkArray.h"

// refs
class ENGINE_API	IRenderable;
//class ENGINE_API	IRenderVisual;

//class ENGINE_API	IBlender;
//class ENGINE_API	CSkeletonWallmark;
//class ENGINE_API	CKinematics;
struct ENGINE_API	FSlideWindowItem;

//	Igor
class IRenderVisual;
class IKinematics;
class CGameFont;
//class IRenderDetailModel;

const	float		fLightSmoothFactor = 4.f;
//////////////////////////////////////////////////////////////////////////
// definition (Dynamic Light)
class	ENGINE_API	IRender_Light	: public xr_resource									{
public:
	enum LT
	{
		DIRECT		= 0,
		OMNIPART	= 3,
		POINT		= 1,
		SPOT		= 2,
		REFLECTED	= 4,
	};
public:
	virtual void					set_type			(LT type)							= 0;
	virtual void					set_active			(bool)								= 0;
	virtual bool					get_active			()									= 0;
	virtual void					set_shadow			(bool)								= 0;
	virtual void					set_position		(const Fvector& P)					= 0;
	virtual void					set_rotation		(const Fvector& D, const Fvector& R)= 0;
	virtual void					set_cone			(float angle)						= 0;
	virtual void					set_range			(float R)							= 0;
	virtual void					set_material		(LPCSTR name)						= 0;
	virtual void					set_color			(const Fcolor& C)					= 0;
	virtual void					set_color			(float r, float g, float b)			= 0;
	virtual void					set_intensity		(float Inintensity)					= 0;
	virtual void					set_volumetric_intensity		(float Inintensity)					= 0;

	virtual void					set_hud_mode		(bool b)							= 0;
	virtual bool					get_hud_mode		()									= 0;
	virtual ~IRender_Light()		;
};


//////////////////////////////////////////////////////////////////////////
// definition (Dynamic Glow)
class	ENGINE_API		IRender_Glow	: public xr_resource									{
public:
	virtual void					set_active			(bool)								= 0;
	virtual bool					get_active			()									= 0;
	virtual void					set_position		(const Fvector& P)					= 0;
	virtual void					set_direction		(const Fvector& P)					= 0;
	virtual void					set_radius			(float R)							= 0;
	virtual void					set_texture			(LPCSTR name)						= 0;
	virtual void					set_color			(const Fcolor& C)					= 0;
	virtual void					set_color			(float r, float g, float b)			= 0;
	virtual ~IRender_Glow()			;
};

//////////////////////////////////////////////////////////////////////////
// definition (Per-object render-specific data)
class	ENGINE_API	IRender_ObjectSpecific		{
public:
	enum mode
	{
		TRACE_LIGHTS	= (1<<0),
		TRACE_SUN		= (1<<1),
		TRACE_HEMI		= (1<<2),
		TRACE_ALL		= (TRACE_LIGHTS|TRACE_SUN|TRACE_HEMI),
	};
public:
	virtual	void						force_mode			(u32 mode)							= 0;
	virtual float						get_luminocity		()									= 0;
	virtual float						get_luminocity_hemi	()									= 0;
	virtual float*						get_luminocity_hemi_cube		()									= 0;

	virtual ~IRender_ObjectSpecific()	{};
};

//////////////////////////////////////////////////////////////////////////
// definition (Portal)
class	ENGINE_API	IRender_Portal				{
public:
	virtual ~IRender_Portal()			{};
};

//////////////////////////////////////////////////////////////////////////
// definition (Sector)
class	ENGINE_API	IRender_Sector				{
public:
	virtual ~IRender_Sector()			{};
};

//////////////////////////////////////////////////////////////////////////
// definition (Target)
class	ENGINE_API	IRender_Target				{
public:
	virtual	void					set_blur			(float	f)							= 0;
	virtual	void					set_gray			(float	f)							= 0;
	virtual void					set_duality_h		(float	f)							= 0;
	virtual void					set_duality_v		(float	f)							= 0;
	virtual void					set_noise			(float	f)							= 0;
	virtual void					set_noise_scale		(float	f)							= 0;
	virtual void					set_noise_fps		(float	f)							= 0;
	virtual void					set_color_base		(u32	f)							= 0;
	virtual void					set_color_gray		(u32	f)							= 0;
	//virtual void					set_color_add		(u32	f)							= 0;
	virtual void					set_color_add		(const Fvector	&f)					= 0;
	virtual u32						get_width			()									= 0;
	virtual u32						get_height			()									= 0;
	virtual void					set_cm_imfluence	(float	f)							= 0;
	virtual void					set_cm_interpolate	(float	f)							= 0;
	virtual void					set_cm_textures		(const shared_str &tex0, const shared_str &tex1)= 0;
	virtual ~IRender_Target()		{};
};

//////////////////////////////////////////////////////////////////////////
// definition (Renderer)
class	ENGINE_API	IRender_interface
{
public:

	virtual void					SpawnStaticDecal		(const shared_str& TextureName,const Fvector& Point, const Fvector& Normal, float Size)=0;
	virtual void					SpawnSkeletalDecal		(const IRenderVisual* InObject,const shared_str& TextureName, const Fvector& StartPoint, const Fvector& EndPoint, float Size)=0;

	virtual IRender_Light*			light_create			()											= 0;
	virtual void					light_destroy			(IRender_Light* p_)							{ };
	virtual IRender_Glow*			glow_create				()											= 0;
	virtual void					glow_destroy			(IRender_Glow* p_)							{ };

	// Models
	virtual IRenderVisual*			model_CreateParticles	(LPCSTR name)								= 0;
	virtual IRenderVisual*			model_Create			(LPCSTR name, IReader*	data=0)				= 0;
	virtual void					model_Delete			(IRenderVisual* &	V, BOOL bDiscard=FALSE)	= 0;
	virtual IRender_Target*			getTarget				()											= 0;
	
	virtual IRender_ObjectSpecific*	ros_create				(IRenderable* parent)						= 0;
	virtual void					ros_destroy				(IRender_ObjectSpecific*&)					= 0;
	virtual ~IRender_interface();
};
struct ENGINE_API		resptrcode_glow : public resptr_base<IRender_Glow>
{
	void				_dec()  override { if (0 == p_) return;	p_->dwReference--; if (0 == p_->dwReference) Render->glow_destroy(p_); }
	void				destroy() { _set(NULL); }
};
typedef	resptr_core<IRender_Glow, resptrcode_glow >		ref_glow;
struct ENGINE_API		resptrcode_light : public resptr_base<IRender_Light>
{
	void				_dec()  override { if (0 == p_) return;	p_->dwReference--; if (0 == p_->dwReference) Render->light_destroy(p_); }
	void				destroy() { _set(NULL); }
};
typedef	resptr_core<IRender_Light, resptrcode_light >	ref_light;
//extern ENGINE_API	IRender_interface*	Render;

#endif