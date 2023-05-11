#pragma once

namespace PS {
	class CPEDef;
	DEFINE_VECTOR(CPEDef*,PEDVec,PEDIt);

	class CPGDef;
	DEFINE_VECTOR(CPGDef*,PGDVec,PGDIt);
} // namespace PS

class ECORE_API CPSLibrary
{
	PS::PEDVec			m_PEDs;
    PS::PGDVec			m_PGDs;

    xr_string			m_CurrentParticles;
public:
	void 	 	FindByName		(LPCSTR new_name, bool& res);
public:
    bool 				Load			(LPCSTR nm);
    bool				Save			(LPCSTR nm);
    
	bool				Load2			();
	bool				Save2			();
public:
						CPSLibrary		(){;}
    		 			~CPSLibrary		(){;}

    void				OnCreate		();
    void				OnDestroy		();

    PS::CPEDef*			FindPED			(LPCSTR name);
    PS::PEDIt			FindPEDIt		(LPCSTR name);
    PS::CPGDef*			FindPGD			(LPCSTR name);
    PS::PGDIt			FindPGDIt		(LPCSTR name);

    // get object properties methods
    IC PS::PEDIt		FirstPED		()	{return m_PEDs.begin();}
    IC PS::PEDIt		LastPED			()	{return m_PEDs.end();}
    IC PS::PGDIt		FirstPGD		()	{return m_PGDs.begin();}
    IC PS::PGDIt		LastPGD			()	{return m_PGDs.end();}

    PS::CPEDef*			AppendPED		(PS::CPEDef* src=0);
    PS::CPGDef*			AppendPGD		(PS::CPGDef* src=0);
    void				Remove			(LPCSTR name);
    void				RenamePED		(PS::CPEDef* src, LPCSTR new_name);
    void				RenamePGD		(PS::CPGDef* src, LPCSTR new_name);

    void				Reload			();
    bool				Save			();

	virtual	PS::CPGDef const* const*	particles_group_begin	() const;
	virtual	PS::CPGDef const* const*	particles_group_end		() const;
	virtual	void						particles_group_next	(PS::CPGDef const* const*& iterator) const;
	virtual	shared_str const&			particles_group_id		(PS::CPGDef const& particles_group) const;
};

#define PS_LIB_SIGN 			"PS_LIB"

#define PS_VERSION				0x0001
//----------------------------------------------------
#define PS_CHUNK_VERSION		0x0001
#define PS_CHUNK_FIRSTGEN		0x0002
#define PS_CHUNK_SECONDGEN		0x0003
#define PS_CHUNK_THIRDGEN		0x0004
