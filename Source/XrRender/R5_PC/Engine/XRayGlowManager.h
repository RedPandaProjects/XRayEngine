// GlowManager.h: interface for the CGlowManager class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "../../xrCDB/ispatial.h"
#include "../../xrCDB/xr_collide_defs.h"

class XRayGlow				: public IRender_Glow, public ISpatial
{
public:
	struct {
		u32				bActive	:	1;
	}					flags		;
	float				fade		;
	XRayShader			shader		;
	u32					dwFrame		;

	Fvector				position	;
	Fvector				direction	;
	float				radius		;
	Fcolor				color		;

	// Ray-testing cache
	BOOL				bTestResult	;
	collide::ray_cache	RayCache	;
	u32					qid_pass	;
	u32					qid_total	;
public:
	XRayGlow();
	virtual	~XRayGlow();

	virtual void					set_active			(bool);
	virtual bool					get_active			();
	virtual void					set_position		(const Fvector& P);
	virtual void					set_direction		(const Fvector& P);
	virtual void					set_radius			(float			R);
	virtual void					set_texture			(LPCSTR			name);
	virtual void					set_color			(const Fcolor&	C);
	virtual void					set_color			(float r, float g, float b);
	virtual void					spatial_move		();
};

#define MAX_GlowsPerFrame	64

class XRayGlowManager 
{
	xr_vector<ref_glow>		Glows			;
	xr_vector<ref_glow>		Selected		;
	xr_vector<XRayGlow*>		m_RenderGlows;
	/*xr_vector<ref_glow>		SelectedToTest_2;	// 2-frames behind
	xr_vector<ref_glow>		SelectedToTest_1;	// 1-frames behind
	xr_vector<ref_glow>		SelectedToTest_0;	// 0-frames behind*/

	BOOL					b_hardware		;
	u32						dwTestID		;

	BearFactoryPointer<BearRHI::BearRHIVertexBuffer> m_VertexBuffer;
public:
	void	Add				(ref_glow	g)	;

	void	Load			(IReader	*fs);
	void	Unload			();

	void	RenderSelected	();
	void	Render			();

	XRayGlowManager();
	~XRayGlowManager();
};