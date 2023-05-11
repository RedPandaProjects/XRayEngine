#pragma once
namespace PS
{
	class CParticleEffect;

	class ECORE_API CPGDef
	{
	public:
		shared_str			m_Name;
		Flags32				m_Flags;
		float				m_fTimeLimit;
		struct SEffect{
			enum{
				flDefferedStop		= (1<<0),
				flOnPlayChild		= (1<<1),
				flEnabled			= (1<<2),
                flOnPlayChildRewind	= (1<<4),
                flOnBirthChild		= (1<<5),
				flOnDeadChild		= (1<<6),
			};
			Flags32			m_Flags;
			shared_str		m_EffectName;  
			shared_str		m_OnPlayChildName;
			shared_str		m_OnBirthChildName;
			shared_str		m_OnDeadChildName;
			float			m_Time0;
			float			m_Time1;
							SEffect				(){m_Flags.zero();/*set(flEnabled)*/m_Time0=0;m_Time1=0;}
#if 0
            BOOL			Equal				(const SEffect&);
#endif
		};
		DEFINE_VECTOR(SEffect*,EffectVec,EffectIt);
		EffectVec			m_Effects;
#if 0
// change Equal if variables changed 
		void   	OnEffectsEditClick	(ButtonValue* sender, bool& bDataModified, bool& bSafe);
		void   	OnEffectTypeChange	(PropValue* sender);
		void   	OnEffectEditClick	(ButtonValue* sender, bool& bDataModified, bool& bSafe);
		void   	OnControlClick	(ButtonValue* sender, bool& bDataModified, bool& bSafe);
		void   	OnParamsChange	(PropValue* sender);
		void				FillProp	   	(LPCSTR pref, ::PropItemVec& items, ::ListItem* owner);
		BOOL				Equal			(const CPGDef* pe);
		bool				Validate 			(bool bMsg);
#endif
	public:
							CPGDef		  	();
							~CPGDef		  	();
		void				SetName		  	(LPCSTR name);

		void 				Save		  	(IWriter& F);
		BOOL 				Load		 	(IReader& F);

		void 				Save2		  	(CInifile& ini);
		BOOL 				Load2		 	(CInifile& ini);

#if 0
        void				Clone			(CPGDef* source);
#endif
	};

}
//----------------------------------------------------
#define PGD_VERSION				0x0003
#define PGD_CHUNK_VERSION		0x0001
#define PGD_CHUNK_NAME			0x0002
#define PGD_CHUNK_FLAGS			0x0003
#define PGD_CHUNK_EFFECTS		0x0004 // obsolete
#define PGD_CHUNK_TIME_LIMIT	0x0005
#define PGD_CHUNK_EFFECTS2		0x0007
