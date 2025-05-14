#pragma once

#include "../XrEngine/Effector.h"

// приседание после падения
class CEffectorFall : public CEffectorCam
{
	float	fPower;
	float	fPhase;
public:
	virtual	BOOL	ProcessCam(SCamEffectorInfo& info);

	CEffectorFall(float power, float life_time=1);
	virtual ~CEffectorFall();
};
