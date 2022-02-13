// Weapon.h: interface for the CWeapon class.
#pragma once

#include "PhysicsShell.h"
#include "weaponammo.h"
#include "PHShellCreator.h"

#include "ShootingObject.h"
#include "hud_item_object.h"
#include "Actor_Flags.h"
#include "../XrRender/Public/KinematicsAnimated.h"
#include "game_cl_single.h"


// refs
class CEntity;
class ENGINE_API CMotionDef;
class CSE_ALifeItemWeapon;
class CSE_ALifeItemWeaponAmmo;
class CWeaponMagazined;
class CParticlesObject;
class CUIStaticItem;

class CWeapon : public CHudItemObject,
				public CShootingObject
{
private:
	typedef CHudItemObject inherited;

public:
							CWeapon				(LPCSTR name);
	virtual					~CWeapon			();

	// Generic
	virtual void			Load				(LPCSTR section);

	virtual BOOL			net_Spawn			(CSE_Abstract* DC);
	virtual void			net_Destroy			();
	virtual void			net_Export			(NET_Packet& P);
	virtual void			net_Import			(NET_Packet& P);
	
	virtual CWeapon			*cast_weapon			()					{return this;}
	virtual CWeaponMagazined*cast_weapon_magazined	()					{return 0;}


	//serialization
	virtual void			save				(NET_Packet &output_packet);
	virtual void			load				(IReader &input_packet);
	virtual BOOL			net_SaveRelevant	()								{return inherited::net_SaveRelevant();}

	virtual void			UpdateCL			();
	virtual void			shedule_Update		(u32 dt);

	virtual void			renderable_Render	();
	virtual void			OnDrawUI			();

	virtual void			OnH_B_Chield		();
	virtual void			OnH_A_Chield		();
	virtual void			OnH_B_Independent	(bool just_before_destroy);
	virtual void			OnH_A_Independent	();
	virtual void			OnEvent				(NET_Packet& P, u16 type);// {inherited::OnEvent(P,type);}

	virtual	void			Hit					(SHit* pHDS);

	virtual void			reinit				();
	virtual void			reload				(LPCSTR section);
	virtual void			create_physic_shell	();
	virtual void			activate_physic_shell();
	virtual void			setup_physic_shell	();

	virtual void			SwitchState			(u32 S);
	virtual bool			Activate			();

	virtual void			Hide				();
	virtual void			Show				();

	//������������� ���� ���� � �������� ����� ��� �������� �� OnH_B_Chield
	virtual void			OnActiveItem		();
	virtual void			OnHiddenItem		();

//////////////////////////////////////////////////////////////////////////
//  Network
//////////////////////////////////////////////////////////////////////////

public:
	virtual bool			can_kill			() const;
	virtual CInventoryItem	*can_kill			(CInventory *inventory) const;
	virtual const CInventoryItem *can_kill		(const xr_vector<const CGameObject*> &items) const;
	virtual bool			ready_to_kill		() const;
	virtual bool			NeedToDestroyObject	() const; 
	virtual ALife::_TIME_ID	TimePassedAfterIndependant() const;
protected:
	//����� �������� ������
	ALife::_TIME_ID			m_dwWeaponRemoveTime;
	ALife::_TIME_ID			m_dwWeaponIndependencyTime;

//////////////////////////////////////////////////////////////////////////
//  Animation 
//////////////////////////////////////////////////////////////////////////
public:

//	void					animGet				(MotionSVec& lst, LPCSTR prefix);
	void					signal_HideComplete	();

//////////////////////////////////////////////////////////////////////////
//  InventoryItem methods
//////////////////////////////////////////////////////////////////////////
public:
	virtual bool			Action(s32 cmd, u32 flags);

//////////////////////////////////////////////////////////////////////////
//  Weapon state
//////////////////////////////////////////////////////////////////////////
public:
	enum EWeaponStates {
		eIdle		= 0,
		eFire,
		eFire2,
		eReload,
		eShowing,
		eHiding,
		eHidden,
		eMisfire,
		eMagEmpty,
		eSwitch,
	};
	enum EWeaponSubStates{
		eSubstateReloadBegin		=0,
		eSubstateReloadInProcess,
		eSubstateReloadEnd,
	};

	virtual bool			IsHidden			()	const		{	return GetState() == eHidden;}						// Does weapon is in hidden state
	virtual bool			IsHiding			()	const		{	return GetState() == eHiding;}
	virtual bool			IsShowing			()	const		{	return GetState() == eShowing;}

	IC BOOL					IsValid				()	const		{	return iAmmoElapsed;						}
	// Does weapon need's update?
	BOOL					IsUpdating			();


	BOOL					IsMisfire			() const;
	BOOL					CheckForMisfire		();


	BOOL					AutoSpawnAmmo		() const		{ return m_bAutoSpawnAmmo; };
	bool					IsTriStateReload	() const		{ return m_bTriStateReload;}
	EWeaponSubStates		GetReloadState		() const		{ return (EWeaponSubStates)m_sub_state;}
protected:
	bool					m_bTriStateReload;
	u8						m_sub_state;
	// Weapon fires now
	bool					bWorking2;
	// a misfire happens, you'll need to rearm weapon
	bool					bMisfire;				

	BOOL					m_bAutoSpawnAmmo;
//////////////////////////////////////////////////////////////////////////
//  Weapon Addons
//////////////////////////////////////////////////////////////////////////
public:
	///////////////////////////////////////////
	// ������ � �������� � ������
	//////////////////////////////////////////


			bool IsGrenadeLauncherAttached	() const;
			bool IsScopeAttached			() const;
			bool IsSilencerAttached			() const;

	virtual bool GrenadeLauncherAttachable();
	virtual bool ScopeAttachable();
	virtual bool SilencerAttachable();
	virtual bool UseScopeTexture() {return true;};

	//���������� ��������� ��� �������� �������
			void UpdateAddonsVisibility();
			void UpdateHUDAddonsVisibility();
	//������������� ������� �������������� �������
	virtual void InitAddons();

	//��� ������������ ������ ��������� � ����������
	int	GetScopeX() {return m_iScopeX;}
	int	GetScopeY() {return m_iScopeY;}
	int	GetSilencerX() {return m_iSilencerX;}
	int	GetSilencerY() {return m_iSilencerY;}
	int	GetGrenadeLauncherX() {return m_iGrenadeLauncherX;}
	int	GetGrenadeLauncherY() {return m_iGrenadeLauncherY;}

	const shared_str& GetGrenadeLauncherName	()		{return m_sGrenadeLauncherName;}
	const shared_str& GetScopeName				()		{return m_sScopeName;}
	const shared_str& GetSilencerName			()		{return m_sSilencerName;}

	u8		GetAddonsState						()		const		{return m_flagsAddOnState;};
	void	SetAddonsState						(u8 st)	{m_flagsAddOnState=st;}//dont use!!! for buy menu only!!!
protected:
	//��������� ������������ �������
	u8 m_flagsAddOnState;

	//����������� ����������� ��������� �������
	ALife::EWeaponAddonStatus	m_eScopeStatus;
	ALife::EWeaponAddonStatus	m_eSilencerStatus;
	ALife::EWeaponAddonStatus	m_eGrenadeLauncherStatus;

	//�������� ������ ������������ �������
	shared_str		m_sScopeName;
	shared_str		m_sSilencerName;
	shared_str		m_sGrenadeLauncherName;

	//�������� ������ ��������� � ���������
	int	m_iScopeX, m_iScopeY;
	int	m_iSilencerX, m_iSilencerY;
	int	m_iGrenadeLauncherX, m_iGrenadeLauncherY;

///////////////////////////////////////////////////
//	��� ������ ����������� � ������������ �������
///////////////////////////////////////////////////
protected:
	//���������� ������ �����������
	bool			m_bZoomEnabled;
	//������� ������ �����������
	float			m_fZoomFactor;
	//����� �����������
	float			m_fZoomRotateTime;
	//�������� ��� ������������ �������, � ������ �����������
	CUIStaticItem*	m_UIScope;
	//����������� ���������� ������������
	float			m_fIronSightZoomFactor;
	//����������� ���������� �������
	float			m_fScopeZoomFactor;
	//����� ����� ����������� �������
	bool			m_bZoomMode;
	//�� 0 �� 1, ���������� ��������� ���������
	//�� ���������� HUD  
	float			m_fZoomRotationFactor;
	bool			m_bHideCrosshairInZoom;
public:

	IC bool					IsZoomEnabled		()	const	{return m_bZoomEnabled;}
	virtual	void			ZoomInc				(){};
	virtual	void			ZoomDec				(){};
	virtual void			OnZoomIn			();
	virtual void			OnZoomOut			();
			bool			IsZoomed			()	const	{return m_bZoomMode;};
	CUIStaticItem*			ZoomTexture			();	
			bool			ZoomHideCrosshair	()			{return m_bHideCrosshairInZoom || ZoomTexture();}

	IC float				GetZoomFactor		() const		{	return m_fZoomFactor;	}
	virtual	float			CurrentZoomFactor	();
	//����������, ��� ������ ��������� � ���������� �������� ��� ������������� ������������
			bool			IsRotatingToZoom	() const		{	return (m_fZoomRotationFactor<1.f);}

			void			LoadZoomOffset		(LPCSTR section, LPCSTR prefix);

	virtual float				Weight			();		

public:
    virtual EHandDependence		HandDependence		()	const		{	return eHandDependence;}
			bool				IsSingleHanded		()	const		{	return m_bIsSingleHanded; }

public:
	IC		LPCSTR			strap_bone0			() const {return m_strap_bone0;}
	IC		LPCSTR			strap_bone1			() const {return m_strap_bone1;}
	IC		void			strapped_mode		(bool value) {m_strapped_mode = value;}
	IC		bool			strapped_mode		() const {return m_strapped_mode;}

protected:
	LPCSTR					m_strap_bone0;
	LPCSTR					m_strap_bone1;
	Fmatrix					m_StrapOffset;
	bool					m_strapped_mode;
	bool					m_can_be_strapped;

	Fmatrix					m_Offset;
	// 0-������������ ��� ������� ���, 1-���� ����, 2-��� ����
	EHandDependence			eHandDependence;
	bool					m_bIsSingleHanded;

public:
	//����������� ���������
	Fvector					vLoadedFirePoint	;
	Fvector					vLoadedFirePoint2	;

private:
	//������� ��������� � ���������� ��� ���������
	struct					_firedeps
	{
		Fmatrix				m_FireParticlesXForm;	//����������� ��� ��������� ���� � ����
		Fvector				vLastFP, vLastFP2	;	//����
		Fvector				vLastFD				;	// direction
		Fvector				vLastSP				;	//�����	

		_firedeps()			{
			m_FireParticlesXForm.identity();
			vLastFP.set			(0,0,0);
			vLastFP2.set		(0,0,0);
			vLastFD.set			(0,0,0);
			vLastSP.set			(0,0,0);
		}
	}						m_firedeps			;
protected:
	virtual void			UpdateFireDependencies_internal	();
	virtual void			UpdatePosition			(const Fmatrix& transform);	//.
	virtual void			UpdateXForm				();
	virtual void			UpdateHudAdditonal		(Fmatrix&);
	IC		void			UpdateFireDependencies	()			{ if (dwFP_Frame==Device->dwFrame) return; UpdateFireDependencies_internal(); };

	virtual void			LoadFireParams		(LPCSTR section, LPCSTR prefix);
public:	
	IC		const Fvector&	get_LastFP				()			{ UpdateFireDependencies(); return m_firedeps.vLastFP;	}
	IC		const Fvector&	get_LastFP2				()			{ UpdateFireDependencies(); return m_firedeps.vLastFP2;	}
	IC		const Fvector&	get_LastFD				()			{ UpdateFireDependencies(); return m_firedeps.vLastFD;	}
	IC		const Fvector&	get_LastSP				()			{ UpdateFireDependencies(); return m_firedeps.vLastSP;	}

	virtual const Fvector&	get_CurrentFirePoint	()			{ return get_LastFP();				}
	virtual const Fvector&	get_CurrentFirePoint2	()			{ return get_LastFP2();				}
	virtual const Fmatrix&	get_ParticlesXFORM		()			{ UpdateFireDependencies(); return m_firedeps.m_FireParticlesXForm;	}
	virtual void			ForceUpdateFireParticles();

	//////////////////////////////////////////////////////////////////////////
	// Weapon fire
	//////////////////////////////////////////////////////////////////////////
protected:
	virtual void			SetDefaults			();

	//������������� ������ ����
			void			FireTrace			(const Fvector& P, const Fvector& D);
	virtual float			GetWeaponDeterioration	();

	virtual void			FireStart			() {CShootingObject::FireStart();}
	virtual void			FireEnd				();// {CShootingObject::FireEnd();}

	virtual void			Fire2Start			();
	virtual void			Fire2End			();
	virtual void			Reload				();
			void			StopShooting		();
    

	// ��������� ������������ ��������
	virtual void			OnShot				(){};
	virtual void			AddShotEffector		();
	virtual void			RemoveShotEffector	();
	virtual	void			ClearShotEffector	();

public:
	//������� ��������� (� ��������) ������ � ������ ������������� �������
	float					GetFireDispersion	(bool with_cartridge)			;
	float					GetFireDispersion	(float cartridge_k)				;
//	const Fvector&			GetRecoilDeltaAngle	();
	virtual	int				ShotsFired			() { return 0; }

	//�������� ������ � ���������� �� ��� ��������� �����������
	float					GetConditionDispersionFactor	() const;
	float					GetConditionMisfireProbability	() const;
	virtual	float			GetConditionToShow				() const;

public:
	//������ ��� �������� 
	float					camMaxAngle;
	float					camRelaxSpeed;
	float					camRelaxSpeed_AI;
	float					camDispersion;
	float					camDispersionInc;
	float					camDispertionFrac;
	float					camMaxAngleHorz;
	float					camStepAngleHorz;

protected:
	//������ ���������� ��������� ��� ������������ ����������� 
	//(�� ������� ��������� ���������� ���������)
	float					fireDispersionConditionFactor;
	//����������� ������ ��� ������������ �����������
	float					misfireProbability;
	float					misfireConditionK;
	//���������� ����������� ��� ��������
	float					conditionDecreasePerShot;

	//  [8/2/2005]
	float					m_fPDM_disp_base			;
	float					m_fPDM_disp_vel_factor		;
	float					m_fPDM_disp_accel_factor	;
	float					m_fPDM_disp_crouch			;
	float					m_fPDM_disp_crouch_no_acc	;
	//  [8/2/2005]

protected:
	//��� ������ ������
	Fvector					m_vRecoilDeltaAngle;

	//��� ���������, ���� ��� ����� ����������� ������� ������������� 
	//������
	float					m_fMinRadius;
	float					m_fMaxRadius;

//////////////////////////////////////////////////////////////////////////
// ��������
//////////////////////////////////////////////////////////////////////////

protected:	
	//��� ������� ������
			void			StartFlameParticles2();
			void			StopFlameParticles2	();
			void			UpdateFlameParticles2();
protected:
	shared_str					m_sFlameParticles2;
	//������ ��������� ��� �������� �� 2-�� ������
	CParticlesObject*		m_pFlameParticles2;

//////////////////////////////////////////////////////////////////////////
// Weapon and ammo
//////////////////////////////////////////////////////////////////////////
public:
	IC int					GetAmmoElapsed		()	const		{	return /*int(m_magazine.size())*/iAmmoElapsed;}
	IC int					GetAmmoMagSize		()	const		{	return iMagazineSize;						}
	int						GetAmmoCurrent		(bool use_item_to_spawn = false)  const;

	void					SetAmmoElapsed		(int ammo_count);

	virtual void			OnMagazineEmpty		();
			void			SpawnAmmo			(u32 boxCurr = 0xffffffff, 
													LPCSTR ammoSect = NULL, 
													u32 ParentID = 0xffffffff);

	//  [8/3/2005]
	virtual	float			Get_PDM_Base		()	const	{ return m_fPDM_disp_base			; };
	virtual	float			Get_PDM_Vel_F		()	const	{ return m_fPDM_disp_vel_factor		; };
	virtual	float			Get_PDM_Accel_F		()	const	{ return m_fPDM_disp_accel_factor	; };
	virtual	float			Get_PDM_Crouch		()	const	{ return m_fPDM_disp_crouch			; };
	virtual	float			Get_PDM_Crouch_NA	()	const	{ return m_fPDM_disp_crouch_no_acc	; };
	//  [8/3/2005]
protected:
	int						iAmmoElapsed;		// ammo in magazine, currently
	int						iMagazineSize;		// size (in bullets) of magazine

	//��� �������� � GetAmmoCurrent
	mutable int				iAmmoCurrent;
	mutable u32				m_dwAmmoCurrentCalcFrame;	//���� �� ������� ���������� ���-�� ��������
	//  [10/5/2005]
	bool					m_bAmmoWasSpawned;
	//  [10/5/2005]

	virtual bool			IsNecessaryItem	    (const shared_str& item_sect);

public:
	xr_vector<shared_str>	m_ammoTypes;

	CWeaponAmmo*			m_pAmmo;
	u32						m_ammoType;
	shared_str				m_ammoName;
	BOOL					m_bHasTracers;
	u8						m_u8TracerColorID;
	u32						m_set_next_ammoType_on_reload;
	// Multitype ammo support
	xr_vector<CCartridge>	m_magazine;
	CCartridge				m_DefaultCartridge;
	float					m_fCurrentCartirdgeDisp;

		bool				unlimited_ammo				();
	IC	bool				can_be_strapped				() const {return m_can_be_strapped;};

	LPCSTR					GetCurrentAmmo_ShortName	();

protected:
	u32						m_ef_main_weapon_type;
	u32						m_ef_weapon_type;

public:
	virtual u32				ef_main_weapon_type	() const;
	virtual u32				ef_weapon_type		() const;

protected:
	// This is because when scope is attached we can't ask scope for these params
	// therefore we should hold them by ourself :-((
	float					m_addon_holder_range_modifier;
	float					m_addon_holder_fov_modifier;
public:
	virtual	void			modify_holder_params		(float &range, float &fov) const;
	virtual bool			use_crosshair				()	const {return true;}
			bool			show_crosshair				();
			bool			show_indicators				();
	virtual BOOL			ParentMayHaveAimBullet		();
	virtual BOOL			ParentIsActor				();

private:
	float					m_hit_probability[egdCount];

public:
	const float				&hit_probability			() const;
};
