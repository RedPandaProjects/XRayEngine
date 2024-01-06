////////////////////////////////////////////////////////////////////////////
//	Module 		: script_sound_script.cpp
//	Created 	: 06.02.2004
//  Modified 	: 06.02.2004
//	Author		: Dmitriy Iassenev
//	Description : XRay Script sound class script export
////////////////////////////////////////////////////////////////////////////

#include "pch_script.h"
#include "script_sound.h"
#include "script_game_object.h"

using namespace luabind;


void CScriptSound::script_register(lua_State *L)
{
	module(L)
	[			
		class_<CScriptSound>("sound_object")
			.enum_("sound_play_type")
			[
				value("looped",					1),
				value("s2d",					2),
				value("s3d",					0)
			]
			//.property("frequency",				&CScriptSound::GetFrequency,	&CScriptSound::SetFrequency)
			.property("min_distance",			&CScriptSound::GetMinDistance,	&CScriptSound::SetMinDistance)
			.property("max_distance",			&CScriptSound::GetMaxDistance,	&CScriptSound::SetMaxDistance)
			.property("volume",					&CScriptSound::GetVolume,		&CScriptSound::SetVolume)
			.def(									constructor<LPCSTR>())
			.def(									constructor<LPCSTR,ESoundTypes>())
			.def("get_position",				&CScriptSound::GetPosition)
			.def("set_position",				&CScriptSound::SetPosition)
			.def("play",						(void (CScriptSound::*)(CScriptGameObject*))(&CScriptSound::Play))
			.def("play",						(void (CScriptSound::*)(CScriptGameObject*,float))(&CScriptSound::Play))
			.def("play",						(void (CScriptSound::*)(CScriptGameObject*,float,int))(&CScriptSound::Play))
			.def("play_at_pos",					(void (CScriptSound::*)(CScriptGameObject*,const Fvector &))(&CScriptSound::PlayAtPos))
			.def("play_at_pos",					(void (CScriptSound::*)(CScriptGameObject*,const Fvector &,float))(&CScriptSound::PlayAtPos))
			.def("play_at_pos",					(void (CScriptSound::*)(CScriptGameObject*,const Fvector &,float, int))(&CScriptSound::PlayAtPos))
			.def("play_no_feedback",			&CScriptSound::PlayNoFeedback)
			.def("stop",						&CScriptSound::Stop)
			.def("stop_deffered",				&CScriptSound::StopDeffered)
			.def("playing",						&CScriptSound::IsPlaying)
			.def("length",						&CScriptSound::Length)
	];
}
