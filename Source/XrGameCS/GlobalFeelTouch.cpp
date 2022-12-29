#include "stdafx.h"
#include "GlobalFeelTouch.hpp"
#include <functional>

GlobalFeelTouch::GlobalFeelTouch()
{
}

GlobalFeelTouch::~GlobalFeelTouch()
{
}

void GlobalFeelTouch::feel_touch_update(Fvector& P, float R)
{
	//we ignore P and R arguments, we need just delete evaled denied objects...
	xr_vector<Feel::Touch::DenyTouch>::iterator new_end = 
	std::remove_if(feel_touch_disable.begin(), feel_touch_disable.end(), [](Feel::Touch::DenyTouch const& left) {if (left.Expire <= Device->dwTimeGlobal)	return true;	return false; });
	feel_touch_disable.erase(new_end, feel_touch_disable.end());
}

bool GlobalFeelTouch::is_object_denied(CObject const * O)
{
	/*Fvector temp_vector;
	feel_touch_update(temp_vector, 0.f);*/
	if (std::find_if(feel_touch_disable.begin(), feel_touch_disable.end(),
		[O](Feel::Touch::DenyTouch const& left) {return left.O == O; }) == feel_touch_disable.end())
	{
		return false;
	}
	return true;
}