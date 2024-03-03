#include "StdAfx.h"

#include "MMSound.h"

#include "ai_sounds.h"
#include "xrUIXmlParser.h"

CMMSound::CMMSound()
{}

CMMSound::~CMMSound()
{
	all_Stop();
}

void CMMSound::Init(CUIXml& xml_doc, LPCSTR path){
	string256 _path;	
	m_bRandom = xml_doc.ReadAttribInt(path, 0, "random")? true : false;

	int nodes_num	= xml_doc.GetNodesNum(path, 0, "menu_music");

	XML_NODE* tab_node = xml_doc.NavigateToNode(path,0);
	xml_doc.SetLocalRoot(tab_node);	
	for (int i = 0; i < nodes_num; ++i)
		m_play_list.push_back(xml_doc.Read("menu_music", i, ""));		
	xml_doc.SetLocalRoot(xml_doc.GetRoot());

    strconcat(sizeof(_path),_path, path,":whell_sound");
	if (check_file(xml_doc.Read(_path, 0, "")))
        m_whell.Create(xml_doc.Read(_path, 0, "") ,SOUND_TYPE_FROM_SOURCE);

	strconcat(sizeof(_path),_path, path,":whell_click");
	if (check_file(xml_doc.Read(_path, 0, "")))
        m_whell_click.Create(xml_doc.Read(_path, 0, ""),SOUND_TYPE_FROM_SOURCE );
}

bool CMMSound::check_file(LPCSTR fname){
	string_path		_path;
	strconcat		(sizeof(_path),_path, fname, ".ogg");
	return FS.exist("$game_sounds$", _path) ? true : false;		
}

void CMMSound::whell_Play()
{

	if (m_whell.IsValid() && !m_whell.IsPlaying())
		m_whell.Play(NULL, true);

}

void CMMSound::whell_Stop(){
	if (m_whell.IsPlaying())
		m_whell.Stop();
}

void CMMSound::whell_Click()
{

   	if (m_whell_click.IsValid())
		m_whell_click.Play(NULL);

}

void CMMSound::whell_UpdateMoving(float frequency){
	m_whell.SetFrequency(frequency);
}

void CMMSound::music_Play()
{
	if (m_play_list.empty())
		return;

	int i = Random.randI(m_play_list.size());

	string_path		_path;
	strconcat		(sizeof(_path),_path, m_play_list[i].c_str(), ".ogg");
	VERIFY			(FS.exist("$game_sounds$", _path ));	

	m_music_stereo.Create(_path,SOUND_TYPE_FROM_SOURCE);
    m_music_stereo.Play(NULL);
}

void CMMSound::music_Update()
{
	if (Device->Paused()) return;

	if ( 0==m_music_stereo.IsPlaying() )
		music_Play();
}

void CMMSound::music_Stop()
{
	m_music_stereo.Stop();
}

void CMMSound::all_Stop(){
	music_Stop();
	m_whell.Stop();
	m_whell_click.Stop();
}