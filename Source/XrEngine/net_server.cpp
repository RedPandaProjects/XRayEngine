#include "stdafx.h"
#include "net_server.h"
#include <functional>

#pragma warning(push)
#pragma warning(disable : 4995)
#include <malloc.h>
#pragma warning(pop)

ENGINE_API ClientID BroadcastCID(0xffffffff);

IClient::IClient(CTimer* timer) : server(NULL) {
    dwTime_LastUpdate = 0;
    flags.bLocal = FALSE;
    flags.bConnected = FALSE;
    flags.bReconnect = FALSE;
    flags.bVerified = TRUE;
}

IClient::~IClient() {}

// {0218FA8B-515B-4bf2-9A5F-2F079D1759F3}
static const GUID NET_GUID = {
    0x218fa8b, 0x515b, 0x4bf2, { 0x9a, 0x5f, 0x2f, 0x7, 0x9d, 0x17, 0x59, 0xf3 }
};
// {8D3F9E5E-A3BD-475b-9E49-B0E77139143C}
static const GUID CLSID_NETWORKSIMULATOR_DP8SP_TCPIP = {
    0x8d3f9e5e, 0xa3bd, 0x475b, { 0x9e, 0x49, 0xb0, 0xe7, 0x71, 0x39, 0x14, 0x3c }
};

static HRESULT WINAPI Handler(PVOID pvUserContext, DWORD dwMessageType, PVOID pMessage) {
    IPureServer* C = (IPureServer*)pvUserContext;
    return C->net_Handler(dwMessageType, pMessage);
}

//------------------------------------------------------------------------------

void IClient::_SendTo_LL(const void* data, u32 size, u32 flags, u32 timeout) {
    R_ASSERT(server);
    server->IPureServer::SendTo_LL(ID, const_cast<void*>(data), size, flags, timeout);
}

//------------------------------------------------------------------------------
IClient* IPureServer::ID_to_client(ClientID ID, bool ScanAll) {
    if (0 == ID.value())
        return NULL;
    IClient* ret_client = GetClientByID(ID);
    if (ret_client || !ScanAll)
        return ret_client;

    return NULL;
}

void IPureServer::_Recieve(const void* data, u32 data_size, u32 param) {
    if (data_size >= NET_PacketSizeLimit) {
        Msg("! too large packet size[%d] received, DoS attack?", data_size);
        return;
    }

    NET_Packet packet;
    packet.construct(data, data_size);

    ClientID id(param);

    csMessage.lock();

    //---------------------------------------
    u32 result = OnMessage(packet, id);
    csMessage.unlock();

    if (result)
        SendBroadcast(id, packet, result);
}

//==============================================================================

IPureServer::IPureServer(CTimer* timer) {
    device_timer = timer;
    stats.clear();
    stats.dwSendTime = device_timer->GetElapsed_ms();
    SV_Client = NULL;

#ifdef DEBUG
    sender_functor_invoked = false;
#endif
}

IPureServer::~IPureServer() {
    SV_Client = NULL;
}

IPureServer::EConnect IPureServer::Connect(LPCSTR options, GameDescriptionData& game_descr) 
{
    connect_options = options;

    return ErrNoError;
}

void IPureServer::Disconnect()
{
}

HRESULT IPureServer::net_Handler(u32 dwMessageType, PVOID pMessage) 
{
    return S_OK;
}

void IPureServer::Flush_Clients_Buffers() 
{
}

void IPureServer::SendTo_Buf(ClientID id, void* data, u32 size, u32 dwFlags, u32 dwTimeout) 
{
}

void IPureServer::SendTo_LL(ClientID ID /*DPNID ID*/, void* data, u32 size, u32 dwFlags, u32 dwTimeout) 
{
}

void IPureServer::SendTo(ClientID ID /*DPNID ID*/, NET_Packet& P, u32 dwFlags, u32 dwTimeout)
{
    SendTo_LL(ID, P.B.data, P.B.count, dwFlags, dwTimeout);
}

void IPureServer::SendBroadcast_LL(ClientID exclude, void* data, u32 size, u32 dwFlags)
{
}

void IPureServer::SendBroadcast(ClientID exclude, NET_Packet& P, u32 dwFlags) 
{
    // Perform broadcasting
    SendBroadcast_LL(exclude, P.B.data, P.B.count, dwFlags);
}

u32 IPureServer::OnMessage(NET_Packet& P,
    ClientID sender) // Non-Zero means broadcasting with "flags" as returned
{
    return 0;
}

void IPureServer::OnCL_Connected(IClient* CL) 
{
    Msg("* Player 0x%08x connected.\n", CL->ID.value());
}
void IPureServer::OnCL_Disconnected(IClient* CL) 
{
    Msg("* Player 0x%08x disconnected.\n", CL->ID.value());
}

BOOL IPureServer::HasBandwidth(IClient* C) 
{
    u32 dwTime = device_timer->GetElapsed_ms();
    u32 dwInterval = 0;

    UpdateClientStatistic(C);
    C->dwTime_LastUpdate = dwTime;
    dwInterval = 1000;
    return TRUE;
}

void IPureServer::UpdateClientStatistic(IClient* C) 
{
}

void IPureServer::ClearStatistic() 
{
};

bool IPureServer::DisconnectClient(IClient* C, LPCSTR Reason)
{
    if (!C)
        return false;

    return true;
}