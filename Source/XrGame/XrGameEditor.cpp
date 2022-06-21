#include "StdAfx.h"
#ifndef SHIPPING
#include "XrGameEditor.h"
extern void clean_game_globals();
extern void init_game_globals();
XrGameEditor::XrGameEditor()
{
	GameEditor = this;
}

XrGameEditor::~XrGameEditor()
{
}

void XrGameEditor::Create()
{
	init_game_globals();
}

void XrGameEditor::Destroy()
{
	clean_game_globals();
}
XrGameEditor GameEditorImpl;
#endif