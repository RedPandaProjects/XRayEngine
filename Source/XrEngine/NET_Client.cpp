#include "stdafx.h"
#include "net_client.h"

IPureClient::IPureClient(CTimer* timer)
{
	device_timer			= timer;
}

IPureClient::~IPureClient()
{
	while (!mDataQueue.empty())
	{
		xr_delete(mDataQueue.front());
		mDataQueue.pop();
	}
}

void IPureClient::OnMessage(void* data, u32 size)
{
	NET_Packet* P = new NET_Packet;

	P->construct(data, size);	
	P->timeReceive = timeServer_Async();
	
	mDataQueue.push(P);
}
