////////////////////////////////////////////////////////////////////////////
//	Module 		: xrServer_Objects_ALife.h
//	Created 	: 19.09.2002
//  Modified 	: 04.06.2003
//	Author		: Oles Shyshkovtsov, Alexander Maksimchuk, Victor Reutskiy and Dmitriy Iassenev
//	Description : Server objects for ALife simulator
////////////////////////////////////////////////////////////////////////////

#ifndef xrServer_Objects_ALifeH
#define xrServer_Objects_ALifeH

#include "pch_script.h"
#include "xrServer_Objects.h"
#include "../XrEngine/alife_space.h"
#include "../XrEngine/game_graph_space.h"
#ifdef XRGAME_EXPORTS
#include "alife_smart_terrain_task.h"
#endif //#ifdef XRGAME_EXPORTS

#pragma warning(push)
#pragma warning(disable:4005)

#ifdef XRGAME_EXPORTS
class 	CALifeSimulator;
#endif

class CSE_ALifeItemWeapon;
class CSE_ALifeDynamicObject;
class CSE_ALifeObject;
#ifdef XRGAME_EXPORTS
class CALifeSmartTerrainTask;
#endif //#ifdef XRGAME_EXPORTS
class CALifeMonsterAbstract;


struct  SFillPropData
{
	RTokenVec 						locations[4];
	RStringVec						level_ids;
	RTokenVec 						story_names;
	RTokenVec 						spawn_story_names;
	RStringVec						character_profiles;
	RStringVec						smart_covers;
	xr_map<shared_str, u32>			location_colors;
	u32								counter;
	SFillPropData();
	~SFillPropData();
	void							load();
	void							unload();
	void							inc();
	void							dec();
};

SERVER_ENTITY_DECLARE_BEGIN(CSE_ALifeSchedulable, IPureSchedulableObject)
CSE_ALifeItemWeapon* m_tpCurrentBestWeapon;
CSE_ALifeDynamicObject* m_tpBestDetector;
u64								m_schedule_counter;

CSE_ALifeSchedulable(LPCSTR caSection);
virtual							~CSE_ALifeSchedulable();
// we need this to prevent virtual inheritance :-(
virtual CSE_Abstract* base() = 0;
virtual const CSE_Abstract* base() const = 0;
virtual CSE_Abstract* init();
virtual CSE_ALifeSchedulable* cast_schedulable() { return this; };
virtual CSE_Abstract* cast_abstract() { return 0; };
// end of the virtual inheritance dependant code
virtual bool					need_update(CSE_ALifeDynamicObject* object);
virtual u32						ef_creature_type() const;
virtual u32						ef_anomaly_type() const;
virtual u32						ef_weapon_type() const;
virtual u32						ef_detector_type() const;
virtual bool					natural_weapon() const { return true; }
virtual bool					natural_detector() const { return true; }
#ifdef XRGAME_EXPORTS
virtual	CSE_ALifeItemWeapon* tpfGetBestWeapon(ALife::EHitType& tHitType, float& fHitPower) = 0;
virtual bool					bfPerformAttack() { return(true); };
virtual	void					vfUpdateWeaponAmmo() {};
virtual	void					vfProcessItems() {};
virtual	void					vfAttachItems(ALife::ETakeType		tTakeType = ALife::eTakeTypeAll) {};
virtual	ALife::EMeetActionType	tfGetActionType(CSE_ALifeSchedulable* tpALifeSchedulable, int			iGroupIndex, bool bMutualDetection) = 0;
virtual bool					bfActive() = 0;
virtual CSE_ALifeDynamicObject* tpfGetBestDetector() = 0;
#endif
};
add_to_type_list(CSE_ALifeSchedulable)
#define script_type_list save_type_list(CSE_ALifeSchedulable)

SERVER_ENTITY_DECLARE_BEGIN2(CSE_ALifeGraphPoint,ISE_ALifeGraphPoint, CSE_Abstract)
public:
	shared_str						m_caConnectionLevelName;
	shared_str						m_caConnectionPointName;

	CSE_ALifeGraphPoint(LPCSTR caSection);
	virtual							~CSE_ALifeGraphPoint();
	virtual bool					match_configuration() const { return false; }

	virtual  ISE_Abstract* CastAbstract() { return this; }
	virtual ISE_ALifeGraphPoint* CastALifeGraphPoint() { return this; }
#ifndef XRGAME_EXPORTS
	virtual void 			on_render(CDUInterface* du, ISE_AbstractLEOwner* owner, bool bSelected, const Fmatrix& parent, int priority, bool strictB2F);
#endif
	SERVER_ENTITY_DECLARE_END
		add_to_type_list(CSE_ALifeGraphPoint)
#define script_type_list save_type_list(CSE_ALifeGraphPoint)

SERVER_ENTITY_DECLARE_BEGIN3(CSE_ALifeObject, ISE_ALifeObject, CSE_Abstract, CRandom)
		

public:
	typedef CSE_Abstract inherited;

#ifdef XRGAME_EXPORTS
	CALifeSimulator* m_alife_simulator;
#endif

	CSE_ALifeObject(LPCSTR caSection);
	virtual ISE_Abstract* CastAbstract() { return this; }
	virtual							~CSE_ALifeObject();
	virtual bool					used_ai_locations() const;
	virtual bool					can_save() const;
	virtual bool					can_switch_online() const;
	virtual bool					can_switch_offline() const;
	virtual bool					interactive() const;
	virtual CSE_ALifeObject* cast_alife_object() { return this; }
	bool					move_offline() const;
	void					can_switch_online(bool value);
	void					can_switch_offline(bool value);
	void					use_ai_locations(bool value);
	void					interactive(bool value);
	void					move_offline(bool value);
	bool					visible_for_map() const;
	void					visible_for_map(bool value);
	virtual u32						ef_equipment_type() const;
	virtual u32						ef_main_weapon_type() const;
	virtual u32						ef_weapon_type() const;
	virtual u32						ef_detector_type() const;
#ifdef XRGAME_EXPORTS
	virtual void					spawn_supplies(LPCSTR);
	virtual void					spawn_supplies();
	CALifeSimulator& alife() const;
	virtual Fvector					draw_level_position() const;
	virtual	bool					keep_saved_data_anyway() const;
#endif
	SERVER_ENTITY_DECLARE_END
		add_to_type_list(CSE_ALifeObject)
#define script_type_list save_type_list(CSE_ALifeObject)

		SERVER_ENTITY_DECLARE_BEGIN0(CSE_ALifeGroupAbstract)
		ALife::OBJECT_VECTOR			m_tpMembers;
	bool							m_bCreateSpawnPositions;
	u16								m_wCount;
	ALife::_TIME_ID					m_tNextBirthTime;

	CSE_ALifeGroupAbstract(LPCSTR caSection);
	virtual							~CSE_ALifeGroupAbstract();
	virtual	CSE_Abstract* init();
	virtual CSE_Abstract* base() = 0;
	virtual const CSE_Abstract* base() const = 0;
	virtual CSE_ALifeGroupAbstract* cast_group_abstract() { return this; };
	virtual CSE_Abstract* cast_abstract() { return 0; };

#ifdef XRGAME_EXPORTS
	virtual	bool					synchronize_location();
	virtual	void					try_switch_online();
	virtual	void					try_switch_offline();
	virtual	void					switch_online();
	virtual	void					switch_offline();
	virtual	bool					redundant() const;
#endif
	SERVER_ENTITY_DECLARE_END
		add_to_type_list(CSE_ALifeGroupAbstract)
#define script_type_list save_type_list(CSE_ALifeGroupAbstract)

		template<class __A> class CSE_ALifeGroupTemplate : public __A, public CSE_ALifeGroupAbstract {
		typedef __A					inherited1;
		typedef CSE_ALifeGroupAbstract inherited2;
		public:
			CSE_ALifeGroupTemplate(LPCSTR caSection) : __A(pSettings->line_exist(caSection, "monster_section") ? pSettings->r_string(caSection, "monster_section") : caSection), CSE_ALifeGroupAbstract(caSection)
			{
			};

			virtual							~CSE_ALifeGroupTemplate()
			{
			};

			virtual void STATE_Read(NET_Packet& tNetPacket, u16 size)
			{
				inherited1::STATE_Read(tNetPacket, size);
				inherited2::STATE_Read(tNetPacket, size);
			};

			virtual void STATE_Write(NET_Packet& tNetPacket)
			{
				inherited1::STATE_Write(tNetPacket);
				inherited2::STATE_Write(tNetPacket);
			};

			virtual void UPDATE_Read(NET_Packet& tNetPacket)
			{
				inherited1::UPDATE_Read(tNetPacket);
				inherited2::UPDATE_Read(tNetPacket);
			};

			virtual void UPDATE_Write(NET_Packet& tNetPacket)
			{
				inherited1::UPDATE_Write(tNetPacket);
				inherited2::UPDATE_Write(tNetPacket);
			};

			virtual CSE_Abstract* init()
			{
				inherited1::init();
				inherited2::init();
				return						(base());
			}

			virtual CSE_Abstract* base()
			{
				return						(inherited1::base());
			}

			virtual const CSE_Abstract* base() const
			{
				return						(inherited1::base());
			}

#ifndef XRGAME_EXPORTS
			virtual void		FillProps(LPCSTR pref, PropItemVec& items)
			{
				inherited1::FillProps(pref, items);
				inherited2::FillProps(pref, items);
			};
#endif // #ifndef XRGAME_EXPORTS

			virtual CSE_Abstract* cast_abstract()
			{
				return						(this);
			}

			virtual CSE_ALifeGroupAbstract* cast_group_abstract()
			{
				return						(this);
			}

#ifdef XRGAME_EXPORTS
			virtual	void					switch_online()
			{
				inherited2::switch_online();
			}

			virtual	void					switch_offline()
			{
				inherited2::switch_offline();
			}

			virtual	bool					synchronize_location()
			{
				return						(inherited2::synchronize_location());
			}

			virtual	void					try_switch_online()
			{
				inherited2::try_switch_online();
			}

			virtual	void					try_switch_offline()
			{
				inherited2::try_switch_offline();
			}

			virtual	bool					redundant() const
			{
				return							(inherited2::redundant());
			}
#endif
	};

	SERVER_ENTITY_DECLARE_BEGIN2(CSE_ALifeDynamicObject, CSE_ALifeObject, ISE_ALifeDynamicObject)
		ALife::_TIME_ID					m_tTimeID;
	u64								m_switch_counter;

	CSE_ALifeDynamicObject(LPCSTR caSection);
	virtual							~CSE_ALifeDynamicObject();
	virtual ISE_ALifeObject * CastALifeObject() {return this;}
	virtual ISE_Abstract    * CastAbstract() { return this; }
	virtual ISE_ALifeDynamicObject* CastALifeDynamicObject() { return this; }
#ifdef XRGAME_EXPORTS
	virtual void					on_spawn();
	virtual void					on_before_register();
	virtual void					on_register();
	virtual void					on_unregister();
	virtual	bool					synchronize_location();
	virtual	void					try_switch_online();
	virtual	void					try_switch_offline();
	virtual	void					switch_online();
	virtual	void					switch_offline();
	virtual	void					add_online(const bool& update_registries);
	virtual	void					add_offline(const xr_vector<ALife::_OBJECT_ID>& saved_children, const bool& update_registries);
	virtual	bool					redundant() const;
	void					attach(CSE_ALifeInventoryItem* tpALifeInventoryItem, bool		bALifeRequest, bool bAddChildren = true);
	void					detach(CSE_ALifeInventoryItem* tpALifeInventoryItem, ALife::OBJECT_IT* I = 0, bool bALifeRequest = true, bool bRemoveChildren = true);
	virtual void					clear_client_data();
	virtual void					on_failed_switch_online();
#endif
	virtual CSE_ALifeDynamicObject* cast_alife_dynamic_object() { return this; }
	SERVER_ENTITY_DECLARE_END
		add_to_type_list(CSE_ALifeDynamicObject)
#define script_type_list save_type_list(CSE_ALifeDynamicObject)

		SERVER_ENTITY_DECLARE_BEGIN2(CSE_ALifeDynamicObjectVisual, CSE_ALifeDynamicObject, CSE_Visual)
		CSE_ALifeDynamicObjectVisual(LPCSTR caSection);
	virtual							~CSE_ALifeDynamicObjectVisual();
	virtual ISE_Visual* visual();
	SERVER_ENTITY_DECLARE_END
		add_to_type_list(CSE_ALifeDynamicObjectVisual)
#define script_type_list save_type_list(CSE_ALifeDynamicObjectVisual)

		SERVER_ENTITY_DECLARE_BEGIN2(CSE_ALifePHSkeletonObject, CSE_ALifeDynamicObjectVisual, CSE_PHSkeleton)
		CSE_ALifePHSkeletonObject(LPCSTR caSection);
	virtual							~CSE_ALifePHSkeletonObject();
	virtual bool					can_save() const;
	virtual bool					used_ai_locations() const;
	virtual	void					load(NET_Packet& tNetPacket);
	virtual CSE_Abstract* cast_abstract() { return this; }
public:
	SERVER_ENTITY_DECLARE_END
		add_to_type_list(CSE_ALifePHSkeletonObject)
#define script_type_list save_type_list(CSE_ALifePHSkeletonObject)

		SERVER_ENTITY_DECLARE_BEGIN3(CSE_ALifeSpaceRestrictor, CSE_ALifeDynamicObject, ISE_ALifeSpaceRestrictor, CSE_Shape)

	CSE_ALifeSpaceRestrictor(LPCSTR caSection);
	virtual							~CSE_ALifeSpaceRestrictor();
	virtual ISE_Shape* shape();
	virtual bool					can_switch_offline() const;
	virtual bool					used_ai_locations() const;
	virtual  ISE_ALifeObject* CastALifeObject() { return this; };
	virtual  ISE_Abstract* CastAbstract() { return this; };
	virtual ISE_ALifeSpaceRestrictor* CastALifeSpaceRestricto() { return this; };
	SERVER_ENTITY_DECLARE_END
		add_to_type_list(CSE_ALifeSpaceRestrictor)
#define script_type_list save_type_list(CSE_ALifeSpaceRestrictor)

SERVER_ENTITY_DECLARE_BEGIN2(CSE_ALifeLevelChanger, CSE_ALifeSpaceRestrictor, ISE_ALifeLevelChanger)

	CSE_ALifeLevelChanger(LPCSTR caSection);
	virtual							~CSE_ALifeLevelChanger();
	virtual  ISE_ALifeObject* CastALifeObject() { return this; };
	virtual  ISE_Abstract* CastAbstract() { return this; };
	virtual ISE_ALifeLevelChanger* CastALifeLevelChanger() { return this; }
	SERVER_ENTITY_DECLARE_END
		add_to_type_list(CSE_ALifeLevelChanger)
#define script_type_list save_type_list(CSE_ALifeLevelChanger)

		SERVER_ENTITY_DECLARE_BEGIN2(CSE_ALifeSmartZone, CSE_ALifeSpaceRestrictor, CSE_ALifeSchedulable)
		CSE_ALifeSmartZone(LPCSTR caSection);
	virtual							~CSE_ALifeSmartZone();
	virtual CSE_Abstract* base();
	virtual const CSE_Abstract* base() const;
	virtual CSE_Abstract* init();
	virtual CSE_Abstract* cast_abstract() { return this; };
	virtual CSE_ALifeSchedulable* cast_schedulable() { return this; };
	virtual void					update();
	virtual float					detect_probability();
	virtual void					smart_touch(CSE_ALifeMonsterAbstract* monster);
	virtual bool					used_ai_locations() const { return true; };
	virtual CSE_ALifeSmartZone* cast_smart_zone() { return this; };
#ifdef XRGAME_EXPORTS
	virtual bool					bfActive();
	virtual	CSE_ALifeItemWeapon* tpfGetBestWeapon(ALife::EHitType& tHitType, float& fHitPower);
	virtual CSE_ALifeDynamicObject* tpfGetBestDetector();
	virtual	ALife::EMeetActionType	tfGetActionType(CSE_ALifeSchedulable* tpALifeSchedulable, int			iGroupIndex, bool bMutualDetection);
	// additional functionality
	virtual bool					enabled(CSE_ALifeMonsterAbstract* object) const { return false; };
	virtual float					suitable(CSE_ALifeMonsterAbstract* object) const { return 0.f; };
	virtual void					register_npc(CSE_ALifeMonsterAbstract* object) {};
	virtual void					unregister_npc(CSE_ALifeMonsterAbstract* object) {};
	virtual	CALifeSmartTerrainTask* task(CSE_ALifeMonsterAbstract* object) { return 0; };
#endif
	SERVER_ENTITY_DECLARE_END
		add_to_type_list(CSE_ALifeSmartZone)
#define script_type_list save_type_list(CSE_ALifeSmartZone)

		SERVER_ENTITY_DECLARE_BEGIN2(CSE_ALifeObjectPhysic, CSE_ALifeDynamicObjectVisual, CSE_PHSkeleton)
		u32 							type;
	f32 							mass;
	shared_str 						fixed_bones;
	CSE_ALifeObjectPhysic(LPCSTR caSection);
	virtual 						~CSE_ALifeObjectPhysic();
	virtual bool					used_ai_locations() const;
	virtual bool					can_save() const;
	virtual	void					load(NET_Packet& tNetPacket);
	virtual CSE_Abstract* cast_abstract() { return this; }
	//	virtual	void					load					(IReader& r){inherited::load(r);}
	//	using inherited::load(IReader&);
private:
	u32			m_freeze_time;
	static const	u32			m_freeze_delta_time;
#ifdef DEBUG	//only for testing interpolation
	u32			m_last_update_time;
	static const	u32			m_update_delta_time;
#endif
	static const	u32			random_limit;
	CRandom		m_relevent_random;

public:
	enum {
		inventory_item_state_enabled = u8(1) << 0,
		inventory_item_angular_null = u8(1) << 1,
		inventory_item_linear_null = u8(1) << 2//,
		//animated						= u8(1) << 3
	};
	union mask_num_items {
		struct {
			u8	num_items : 5;
			u8	mask : 3;
		};
		u8		common;
	};
	/////////// network ///////////////
	u8								m_u8NumItems;
	bool							prev_freezed;
	bool							freezed;
	SPHNetState						State;

	virtual BOOL					Net_Relevant();

	SERVER_ENTITY_DECLARE_END
		add_to_type_list(CSE_ALifeObjectPhysic)
#define script_type_list save_type_list(CSE_ALifeObjectPhysic)

		SERVER_ENTITY_DECLARE_BEGIN2(CSE_ALifeObjectHangingLamp, CSE_ALifeDynamicObjectVisual, CSE_PHSkeleton)

		void  					OnChangeFlag(PropValue* sender);
	enum {
		flPhysic = (1 << 0),
		flCastShadow = (1 << 1),
		flR1 = (1 << 2),
		flR2 = (1 << 3),
		flTypeSpot = (1 << 4),
		flPointAmbient = (1 << 5),
		flVolumetric = (1 << 6),
	};

	Flags16							flags;
	// light color    
	u32								color;
	float							brightness;
	shared_str						color_animator;
	// light texture    
	shared_str						light_texture;
	// range
	float							range;
	float							m_virtual_size;
	// bones&motions
	shared_str						light_ambient_bone;
	shared_str						light_main_bone;
	shared_str						fixed_bones;
	// spot
	float							spot_cone_angle;
	// ambient    
	float							m_ambient_radius;
	float							m_ambient_power;
	shared_str						m_ambient_texture;
	//	volumetric
	float							m_volumetric_quality;
	float							m_volumetric_intensity;
	float							m_volumetric_distance;
	// glow    
	shared_str						glow_texture;
	float							glow_radius;
	// game
	float							m_health;

	CSE_ALifeObjectHangingLamp(LPCSTR caSection);
	virtual							~CSE_ALifeObjectHangingLamp();
	virtual	void					load(NET_Packet& tNetPacket);
	virtual bool					used_ai_locations() const;
	virtual bool					match_configuration() const;
	virtual bool			validate();
#ifndef XRGAME_EXPORTS
	virtual void 			on_render(CDUInterface* du, ISE_AbstractLEOwner* owner, bool bSelected, const Fmatrix& parent, int priority, bool strictB2F);
#endif // #ifndef XRGAME_EXPORTS
	virtual CSE_Abstract* cast_abstract() { return this; }
	SERVER_ENTITY_DECLARE_END
		add_to_type_list(CSE_ALifeObjectHangingLamp)
#define script_type_list save_type_list(CSE_ALifeObjectHangingLamp)

		SERVER_ENTITY_DECLARE_BEGIN(CSE_ALifeObjectProjector, CSE_ALifeDynamicObjectVisual)
		CSE_ALifeObjectProjector(LPCSTR caSection);
	virtual							~CSE_ALifeObjectProjector();
	virtual bool					used_ai_locations() const;
	SERVER_ENTITY_DECLARE_END
		add_to_type_list(CSE_ALifeObjectProjector)
#define script_type_list save_type_list(CSE_ALifeObjectProjector)

		SERVER_ENTITY_DECLARE_BEGIN3(CSE_ALifeHelicopter, CSE_ALifeDynamicObjectVisual, CSE_Motion, CSE_PHSkeleton)
		shared_str							engine_sound;
	CSE_ALifeHelicopter(LPCSTR caSection);
	virtual							~CSE_ALifeHelicopter();
	virtual	void					load(NET_Packet& tNetPacket);
	virtual bool					can_save() const;
	virtual bool					used_ai_locations() const;
	virtual ISE_Motion* motion();
	virtual CSE_Abstract* cast_abstract() { return this; }

	SERVER_ENTITY_DECLARE_END
		add_to_type_list(CSE_ALifeHelicopter)
#define script_type_list save_type_list(CSE_ALifeHelicopter)

		SERVER_ENTITY_DECLARE_BEGIN2(CSE_ALifeCar, CSE_ALifeDynamicObjectVisual, CSE_PHSkeleton)
		struct SDoorState
	{
		void read(NET_Packet& P);
		void write(NET_Packet& P);
		u8 open_state;
		float health;
	};
	struct SWheelState
	{
		void read(NET_Packet& P);
		void write(NET_Packet& P);
		float health;
	};
	xr_vector<SDoorState>			door_states;
	xr_vector<SWheelState>			wheel_states;
	float							health;
	CSE_ALifeCar(LPCSTR caSection);
	virtual							~CSE_ALifeCar();
	virtual bool					used_ai_locations() const;
	virtual	void					load(NET_Packet& tNetPacket);
	virtual bool					can_save() const;
	virtual CSE_Abstract* cast_abstract() { return this; }
protected:
	virtual void					data_load(NET_Packet& tNetPacket);
	virtual void					data_save(NET_Packet& tNetPacket);
	SERVER_ENTITY_DECLARE_END
		add_to_type_list(CSE_ALifeCar)
#define script_type_list save_type_list(CSE_ALifeCar)

		SERVER_ENTITY_DECLARE_BEGIN(CSE_ALifeObjectBreakable, CSE_ALifeDynamicObjectVisual)
		float							m_health;
	CSE_ALifeObjectBreakable(LPCSTR caSection);
	virtual							~CSE_ALifeObjectBreakable();
	virtual bool					used_ai_locations() const;
	virtual bool					can_switch_offline() const;
	SERVER_ENTITY_DECLARE_END
		add_to_type_list(CSE_ALifeObjectBreakable)
#define script_type_list save_type_list(CSE_ALifeObjectBreakable)

		SERVER_ENTITY_DECLARE_BEGIN2(CSE_ALifeObjectClimable, CSE_Shape, CSE_ALifeDynamicObject)
		CSE_ALifeObjectClimable(LPCSTR caSection);
	shared_str						material;
	virtual							~CSE_ALifeObjectClimable();
	virtual bool					used_ai_locations() const;
	virtual bool					can_switch_offline() const;
	virtual ISE_Shape* shape();

#ifndef XRGAME_EXPORTS
	virtual	void			set_additional_info(void* info);
#endif

	SERVER_ENTITY_DECLARE_END

		add_to_type_list(CSE_ALifeObjectClimable)
#define script_type_list save_type_list(CSE_ALifeObjectClimable)

		SERVER_ENTITY_DECLARE_BEGIN(CSE_ALifeMountedWeapon, CSE_ALifeDynamicObjectVisual)
		CSE_ALifeMountedWeapon(LPCSTR caSection);
	virtual							~CSE_ALifeMountedWeapon();
	SERVER_ENTITY_DECLARE_END
		add_to_type_list(CSE_ALifeMountedWeapon)
#define script_type_list save_type_list(CSE_ALifeMountedWeapon)

		SERVER_ENTITY_DECLARE_BEGIN(CSE_ALifeStationaryMgun, CSE_ALifeDynamicObjectVisual)
		bool							m_bWorking;
	Fvector							m_destEnemyDir;

	CSE_ALifeStationaryMgun(LPCSTR caSection);
	virtual							~CSE_ALifeStationaryMgun();

	SERVER_ENTITY_DECLARE_END
		//add_to_type_list(CSE_ALifeStationaryMgun)
		//#define script_type_list save_type_list(CSE_ALifeStationaryMgun)

		SERVER_ENTITY_DECLARE_BEGIN(CSE_ALifeTeamBaseZone, CSE_ALifeSpaceRestrictor)
		CSE_ALifeTeamBaseZone(LPCSTR caSection);
	virtual							~CSE_ALifeTeamBaseZone();

	u8								m_team;
	SERVER_ENTITY_DECLARE_END
		add_to_type_list(CSE_ALifeTeamBaseZone)
#define script_type_list save_type_list(CSE_ALifeTeamBaseZone)

		SERVER_ENTITY_DECLARE_BEGIN(CSE_ALifeInventoryBox, CSE_ALifeDynamicObjectVisual)
		bool				m_can_take;
	bool				m_closed;
	shared_str			m_tip_text;

	CSE_ALifeInventoryBox(LPCSTR caSection);
	virtual				~CSE_ALifeInventoryBox();
#ifdef XRGAME_EXPORTS
	virtual void		add_offline(const xr_vector<ALife::_OBJECT_ID>& saved_children, const bool& update_registries);
	virtual void		add_online(const bool& update_registries);
#endif
	SERVER_ENTITY_DECLARE_END
		add_to_type_list(CSE_ALifeInventoryBox)
#define script_type_list save_type_list(CSE_ALifeInventoryBox)

#pragma warning(pop)

#endif