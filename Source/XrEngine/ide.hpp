////////////////////////////////////////////////////////////////////////////
//	Module 		: ide.hpp
//	Created 	: 11.12.2007
//  Modified 	: 29.12.2007
//	Author		: Dmitriy Iassenev
//	Description : editor ide function
////////////////////////////////////////////////////////////////////////////

#ifndef IDE_HPP_INCLUDED
#define IDE_HPP_INCLUDED

#ifdef INGAME_EDITOR

#include "../XrWeatherEditor/Public/ide.hpp"

namespace XrWeatherEditor {
	class ide;
} // namespace XrWeatherEditor

inline XrWeatherEditor::ide&	ide	()
{
	VERIFY	(Device.editor());
	return	(*Device.editor());
}

#endif // #ifdef INGAME_EDITOR

#endif // ifndef IDE_HPP_INCLUDED