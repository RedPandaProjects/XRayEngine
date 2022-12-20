#pragma once
class XRayUnrealProxyInterface
{
public:
	virtual void Attach(class IRenderVisual* Visual) = 0;
	virtual void Detach(class IRenderVisual* Visual) = 0;
	virtual class AStalkerProxy*CastToStalkerProxy() = 0;
};