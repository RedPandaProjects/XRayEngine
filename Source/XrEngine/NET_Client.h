#pragma once
#include <queue>

class ENGINE_API IPureClient
{
protected:
	CTimer* device_timer;

	std::queue<NET_Packet*> mDataQueue;
	
public:
	IPureClient				(CTimer* tm);
	virtual ~IPureClient	();
	
	virtual void			OnMessage				(void* data, u32 size);
	
	// time management
	IC u32					timeServer				()	{ return device_timer->GetElapsed_ms(); }
	IC u32					timeServer_Async		()	{ return device_timer->GetElapsed_ms(); }
};

