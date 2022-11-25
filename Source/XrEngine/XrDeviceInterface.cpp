#include "stdafx.h"

ENGINE_API BOOL g_bRendering = FALSE;

ENGINE_API xr_list<LOADING_EVENT>	*		g_loading_events;

XrDeviceInterface::XrDeviceInterface()
{
	m_bNearer = false;
}

XrDeviceInterface::~XrDeviceInterface()
{

}
