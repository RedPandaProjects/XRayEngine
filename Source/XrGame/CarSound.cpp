#include "stdafx.h"
#ifdef DEBUG
#include "PHDebug.h"
#endif
#include "../xrEngine/alife_space.h"
#include "hit.h"
#include "PHDestroyable.h"
#include "car.h"
#include "../XrEngine/Render/Kinematics.h"
//#include "PHWorld.h"
//extern CPHWorld*	ph_world;
#include "../xrphysics/IPHWorld.h"
CCar::SCarSound::SCarSound(CCar* car)
{
	volume                 =1.f;
	pcar=car;
	relative_pos.set(0.f,0.5f,-1.f);
}

CCar::SCarSound::~SCarSound()
{
	
}
void CCar::SCarSound::Init()
{
	CInifile* ini=CastToIKinematics(pcar->Visual())->LL_UserData();
	if (ini->section_exist("car_sound") && ini->line_exist("car_sound","snd_volume"))
	{
		volume  			= ini->r_float("car_sound","snd_volume");

		snd_engine.Create		(ini->r_string("car_sound","snd_name"),SOUND_TYPE_FROM_SOURCE);//
		snd_engine_start.Create	(READ_IF_EXISTS(ini,r_string,"car_sound","engine_start","car\\test_car_start"),SOUND_TYPE_FROM_SOURCE);
		snd_engine_stop.Create	(READ_IF_EXISTS(ini,r_string,"car_sound","engine_stop","car\\test_car_stop"),SOUND_TYPE_FROM_SOURCE);
		float fengine_start_delay=READ_IF_EXISTS(ini,r_float,"car_sound","engine_sound_start_dellay",0.25f);
		engine_start_delay=iFloor((snd_engine_start.IsValid() ? iFloor(snd_engine_start.GetDuration()) : 1.f)*fengine_start_delay);
		if(ini->line_exist("car_sound","relative_pos"))
		{
			relative_pos.set(ini->r_fvector3("car_sound","relative_pos"));
		}
		if(ini->line_exist("car_sound","transmission_switch"))
		{
			snd_transmission.Create(ini->r_string("car_sound","transmission_switch"),SOUND_TYPE_FROM_SOURCE);
		}
	
	
	} else {
		Msg					("! Car doesn't contain sound params");
	}
	eCarSound=sndOff;
}
void CCar::SCarSound::SetSoundPosition(FRBMKSoundSourceRef &snd)
{
	VERIFY(!physics_world()->Processing());
	if (snd.IsPlaying())
	{
		Fvector pos;
		pcar->XFORM().transform_tiny(pos,relative_pos);
		snd.SetPosition		(pos);
	}
}
void CCar::SCarSound::UpdateStarting()
{	
	VERIFY(!physics_world()->Processing());
	SetSoundPosition(snd_engine_start);

	if(snd_engine.IsPlaying())
	{
			UpdateDrive();
	} else
	{
		
		if(time_state_start+engine_start_delay<Device->dwTimeGlobal)
		{
			snd_engine.Play(pcar,true);
			UpdateDrive();
		}
	}

	if(!snd_engine_start.IsPlaying())Drive();
}
void CCar::SCarSound::UpdateStoping()
{
	VERIFY(!physics_world()->Processing());
	SetSoundPosition(snd_engine_stop);
	if(!snd_engine_stop.IsPlaying())SwitchOff();
}
void CCar::SCarSound::UpdateStalling()
{
	SetSoundPosition(snd_engine_stop);
	if(!snd_engine_stop.IsPlaying())SwitchOff();
}
void CCar::SCarSound::UpdateDrive()
{
VERIFY(!physics_world()->Processing());
float		scale							= 0.5f+0.5f*pcar->m_current_rpm/pcar->m_torque_rpm; clamp(scale,0.5f,1.25f);
			snd_engine.SetFrequency		(scale);
			SetSoundPosition(snd_engine);
}
void CCar::SCarSound::SwitchState(ESoundState new_state)
{
	eCarSound=new_state;
	time_state_start=Device->dwTimeGlobal;
}
void CCar::SCarSound::Update()
{
	VERIFY(!physics_world()->Processing());
	if(eCarSound==sndOff) return;
	
	switch (eCarSound)
	{
	case sndStarting	:UpdateStarting	()	;	break;
	case sndDrive		:UpdateDrive	()	;	break;
	case sndStalling	:UpdateStalling	()	;	break;
	case sndStoping		:UpdateStalling	()	;	break;
	}
	

}

void CCar::SCarSound::SwitchOn()
{
	pcar->processing_activate();
}
void CCar::SCarSound::Destroy()
{
	SwitchOff();
	snd_engine.Reset	();
	snd_transmission.Reset();
	snd_engine_stop.Reset();
	snd_engine_start.Reset();
}

void CCar::SCarSound::SwitchOff()
{
	eCarSound=sndOff;
	pcar->processing_deactivate();
}

void CCar::SCarSound::Start()
{
	VERIFY(!physics_world()->Processing());
	if(eCarSound==sndOff) SwitchOn();
	SwitchState(sndStarting);
	snd_engine_start.Play(pcar);
	SetSoundPosition(snd_engine_start);
}

void CCar::SCarSound::Stall()
{
	VERIFY(!physics_world()->Processing());
	if(eCarSound==sndOff)return;
	SwitchState(sndStalling);
	snd_engine.StopWithFade();
	snd_engine_stop.Play(pcar);
	SetSoundPosition(snd_engine_stop);
}

void CCar::SCarSound::Stop()
{
	VERIFY(!physics_world()->Processing());
	if(eCarSound==sndOff)return;
	SwitchState(sndStoping);
	snd_engine.StopWithFade();
	snd_engine_stop.Play(pcar);
	SetSoundPosition(snd_engine_stop);
}

void CCar::SCarSound::Drive()
{
	VERIFY(!physics_world()->Processing());
	if(eCarSound==sndOff) SwitchOn();
	SwitchState(sndDrive);
	if(!snd_engine.IsPlaying())snd_engine.Play(pcar,true);
	SetSoundPosition(snd_engine);
}
void CCar::SCarSound::TransmissionSwitch()
{
	VERIFY(!physics_world()->Processing());
	if(snd_transmission.IsValid()&&eCarSound!=sndOff)
	{
		snd_transmission.Play(pcar);
		SetSoundPosition(snd_transmission);
	}
}

