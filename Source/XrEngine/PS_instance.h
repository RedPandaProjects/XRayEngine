#ifndef _CPS_Instance_H_
#define _CPS_Instance_H_

#include "../xrcdb/ispatial.h"
#include "isheduled.h"
#include "irenderable.h"

class ENGINE_API CPS_Instance	:
	public ISpatial,
	public ISheduled,
	public IRenderable
{
	friend class			IGame_Persistent;


private:
	bool					m_destroy_on_game_load;

protected:
	BOOL					m_bAutoRemove		;
	BOOL					m_bDead				;

protected:
	
	virtual void			PSI_internal_delete	();

public:
	virtual					~CPS_Instance();
							CPS_Instance		(bool destroy_on_game_load);

	IC		const bool		&destroy_on_game_load() const				{	return m_destroy_on_game_load;	}
	virtual void			PSI_destroy			();
	virtual BOOL			PSI_alive			()						= 0;
	IC BOOL					PSI_IsAutomatic		()						{	return m_bAutoRemove;				}

	virtual void			Play				(bool bHudMode)	= 0;
	virtual BOOL			Locked				()				{ return FALSE; }

	virtual	shared_str		shedule_Name		() const		{ return shared_str("particle_instance"); };

	virtual void			shedule_Update		(u32 dt);
	virtual	IRenderable*	dcast_Renderable	()				{ return this;	}
};

#endif
