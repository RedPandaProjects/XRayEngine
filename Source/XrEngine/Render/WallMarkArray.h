#pragma once

#include "FactoryPtr.h"
#include "UIShader.h"

typedef	FactoryPtr<IUIShader>	wm_shader;

class IWallMarkArray
{
public:
	virtual				~IWallMarkArray	() {}
	virtual void		Copy			(IWallMarkArray & InWallMarkArray) = 0;
	virtual void		AppendMark		(const char* Texture) = 0;
	virtual void		Clear			() = 0;
	virtual bool		IsEmpty			() = 0;
	virtual shared_str	GenerateWallmark() = 0;
};