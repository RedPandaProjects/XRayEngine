////////////////////////////////////////////////////////////////////////////
//	Module 		: script_render_device_script.cpp
//	Created 	: 28.06.2004
//  Modified 	: 28.06.2004
//	Author		: Dmitriy Iassenev
//	Description : Script render device script export
////////////////////////////////////////////////////////////////////////////

#include "pch_script.h"
#include "script_render_device.h"

using namespace luabind;

bool is_device_paused(XrDeviceInterface* d)
{
	return !!Device->Paused();
}

void set_device_paused(XrDeviceInterface* d, bool b)
{
	Device->Pause(b, TRUE, FALSE,"set_device_paused_script");
}

extern ENGINE_API BOOL g_appLoaded;
bool is_app_ready()
{
	return !!g_appLoaded;
}

u32 time_global(const XrDeviceInterface *self)
{
	THROW		(self);
	return		(self->dwTimeGlobal);
}

#pragma optimize("s",on)
void CScriptRenderDevice::script_register(lua_State *L)
{
	module(L)
	[
		class_<XrDeviceInterface>("render_device")
			.def_readonly("width",					&XrDeviceInterface::dwWidth)
			.def_readonly("height",					&XrDeviceInterface::dwHeight)
			.def_readonly("time_delta",				&XrDeviceInterface::dwTimeDelta)
			.def_readonly("f_time_delta",			&XrDeviceInterface::fTimeDelta)
			.def_readonly("cam_pos",				&XrDeviceInterface::vCameraPosition)
			.def_readonly("cam_dir",				&XrDeviceInterface::vCameraDirection)
			.def_readonly("cam_top",				&XrDeviceInterface::vCameraTop)
			.def_readonly("cam_right",				&XrDeviceInterface::vCameraRight)
//			.def_readonly("view",					&XrDeviceInterface::mView)
//			.def_readonly("projection",				&XrDeviceInterface::mProject)
//			.def_readonly("full_transform",			&XrDeviceInterface::mFullTransform)
			.def_readonly("fov",					&XrDeviceInterface::fFOV)
			.def_readonly("aspect_ratio",			&XrDeviceInterface::fASPECT)
			.def("time_global",						&time_global)
			.def_readonly("precache_frame",			&XrDeviceInterface::dwPrecacheFrame)
			.def_readonly("frame",					&XrDeviceInterface::dwFrame)
			.def("is_paused",						&is_device_paused)
			.def("pause",							&set_device_paused),
			def("app_ready",						&is_app_ready)
	];
}
