#pragma once

#include "pure.h"
#include "stats.h"
#include "../XrRender/Public/FactoryPtr.h"
#include "../XrRender/Public/RenderDeviceRender.h"

#ifdef INGAME_EDITOR
#	include "../XrWeatherEditor/Public/interfaces.hpp"
#endif // #ifdef INGAME_EDITOR

#define VIEWPORT_NEAR  0.2f

#define DEVICE_RESET_PRECACHE_FRAME_COUNT 10

typedef fastdelegate::FastDelegate0<bool>		LOADING_EVENT;
extern	ENGINE_API xr_list<LOADING_EVENT>		*g_loading_events;
class ENGINE_API XrDeviceInterface
{
public:
	XrDeviceInterface();
	virtual ~XrDeviceInterface();
public:
#ifdef INGAME_EDITOR
	virtual 		XrWeatherEditor::ide* WeatherEditor() const { return nullptr; }
#endif
#ifndef MASTER_GOLD
	virtual			bool				IsEditorMode() { return false; }
#else
	constexpr bool IsEditorMode() { return false; }
#endif
	virtual			bool				Paused()const = 0;
	virtual void Reset(bool precache = false) = 0;
	IC u32	 TimerAsync() { return dwTimeGlobal; }
	IC u32	 TimerAsync_MMT() { return dwTimeContinual; }

	Fmatrix									mInvFullTransform;
	CRegistrator	<pureFrame			>			seqFrameMT;
	CRegistrator	<pureDeviceReset	>			seqDeviceReset;
	xr_vector		<fastdelegate::FastDelegate0<> >	seqParallel;
	CStats* Statistic;
	float									fWidth_2, fHeight_2;
	IC void time_factor(const float& time_factor)
	{
		/*Timer.time_factor(time_factor);
		TimerGlobal.time_factor(time_factor);*/
	}

	IC	 float time_factor() const
	{
		return 1;
	}

	// Multi-threading
	xrCriticalSection	mt_csEnter;
	xrCriticalSection	mt_csLeave;
	volatile BOOL		mt_bMustExit;

	ICF		void			remove_from_seq_parallel(const fastdelegate::FastDelegate0<>& delegate)
	{
		xr_vector<fastdelegate::FastDelegate0<> >::iterator I = std::find(
			seqParallel.begin(),
			seqParallel.end(),
			delegate
		);
		if (I != seqParallel.end())
			seqParallel.erase(I);
	}
	virtual void	Pause(BOOL bOn, BOOL bTimer, BOOL bSound, LPCSTR reason) = 0;
	virtual void PreCache(u32 amount, bool b_draw_loadscreen, bool b_wait_user_input) = 0;
	virtual void Clear() = 0;
	BOOL									m_bNearer;
	IC void									SetNearer(BOOL enabled)
	{
	}
	virtual bool Begin() = 0;
	virtual void End() = 0;
public:
	bool									dwMaximized;
	u32										dwWidth;
	u32										dwHeight;
	u32										dwPrecacheFrame;
	BOOL									b_is_Ready;
	BOOL									b_is_Active;
public:

	// Engine flow-control
	u32										dwFrame;

	float									fTimeDelta;
	float									fTimeGlobal;
	float									fTimeContinual;
	u32										dwTimeDelta;
	u32										dwTimeGlobal;
	u32										dwTimeContinual;

	Fvector									vCameraPosition;
	Fvector									vCameraDirection;
	Fvector									vCameraTop;
	Fvector									vCameraRight;

	Fmatrix									mView;
	Fmatrix									mProject;
	Fmatrix									mFullTransform;

	// Copies of corresponding members. Used for synchronization.
	Fvector									vCameraPosition_saved;

	Fmatrix									mView_saved;
	Fmatrix									mProject_saved;
	Fmatrix									mFullTransform_saved;

	float									fFOV;
	float									fASPECT;
protected:

	u32										Timer_MM_Delta;
public:

	// Registrators
	CRegistrator	<pureRenderDebug	>			seqRenderDebug;
	CRegistrator	<pureRenderUI		>			seqRenderUI;
	CRegistrator	<pureAppActivate	>			seqAppActivate;
	CRegistrator	<pureAppDeactivate	>			seqAppDeactivate;
	CRegistrator	<pureAppStart		>			seqAppStart;
	CRegistrator	<pureAppEnd			>			seqAppEnd;
	CRegistrator	<pureFrame			>			seqFrame;
	CRegistrator	<pureScreenResolutionChanged>	seqResolutionChanged;
	CRegistrator	<pureDrawUI			>			seqDrawUI;

	HWND									m_hWnd;
	//	CStats*									Statistic;
public:
	virtual		CStatsPhysics* _BCL		StatPhysics() = 0;
	virtual				void	_BCL		AddSeqFrame(pureFrame* f, bool mt) = 0;
	virtual				void	_BCL		RemoveSeqFrame(pureFrame* f) = 0;
};