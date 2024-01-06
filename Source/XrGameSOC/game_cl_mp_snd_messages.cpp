#include "stdafx.h"
#include "game_cl_mp.h"
#include "game_cl_mp_snd_messages.h"
#include "Level.h"

void	game_cl_mp::LoadSndMessage			(LPCSTR caSection, LPCSTR caLine, u32 ID)
{
	if (!pSettings->section_exist(caSection)) return;
	if (!pSettings->line_exist(caSection, caLine)) return;

	string4096			Line;
	std::strcpy(Line, pSettings->r_string(caSection, caLine));
	u32 count	= _GetItemCount(Line);
	if (count < 2) return;
	string4096 Name, Prior;
	_GetItem(Line, 0, Name);
	_GetItem(Line, 1, Prior);
	m_pSndMessages.push_back(SND_Message());
	m_pSndMessages.back().Load(ID, atol(Prior), Name);
}

void	game_cl_mp::PlaySndMessage			(u32 ID)
{
	SNDMESSAGES_it it = std::find(m_pSndMessages.begin(), m_pSndMessages.end(), ID);
	if (it == m_pSndMessages.end() || !(*it == ID)) 
	{
		R_ASSERT2(0, "No such sound!!!");
		return;
	};
	SND_Message& SndMsg = *it;
	
//	if (Device->dwTimeGlobal<pSndMgs->pSound._handle()->length_ms() + pSndMgs->LastStarted) return;
	if (SndMsg.pSound.IsPlaying()) return;

	u32 MaxDelay = 0;
	for (u32 i=0; i<m_pSndMessagesInPlay.size(); i++)
	{
		SND_Message* pSndMsgIP = m_pSndMessagesInPlay[i];
		if (!pSndMsgIP->pSound.IsPlaying())		 continue;
		if (pSndMsgIP->priority>SndMsg.priority) return;
		if (pSndMsgIP->priority<SndMsg.priority) 
		{
			pSndMsgIP->pSound.Stop();
			continue;
		}
		if (pSndMsgIP->priority == SndMsg.priority&&pSndMsgIP->pSound)
		{
			if (Device->dwTimeContinual>pSndMsgIP->LastStarted + static_cast<u32>(pSndMsgIP->pSound.GetDuration() * 1000.f)) continue;

			u32 Delay = pSndMsgIP->LastStarted + static_cast<u32>(pSndMsgIP->pSound.GetDuration() * 1000.f) - Device->dwTimeContinual;
			if (Delay > MaxDelay)
			{
				MaxDelay = Delay;
			};
		}
	}
#ifdef DEBUG
	if (MaxDelay>0) 
	{
		Msg("- SndMsgDelay - %d", MaxDelay);
	};
#endif
	
	SndMsg.pSound.Play(nullptr, false,float(MaxDelay)/1000.0f);
	SndMsg.LastStarted = Device->dwTimeContinual+MaxDelay;
	m_pSndMessagesInPlay.push_back(&SndMsg);
}

void	game_cl_mp::UpdateSndMessages	()
{
	for (u32 i=0; i<m_pSndMessagesInPlay.size();)
	{
		SND_Message* pSndMsg = m_pSndMessagesInPlay[i];
		if (pSndMsg->pSound.IsPlaying())
		{
			m_pSndMessagesInPlay.erase(m_pSndMessagesInPlay.begin() + i);			
			continue;
		}
		i++;
	}
}