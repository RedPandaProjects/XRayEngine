#pragma once
class XRayUnrealProxyInterface
{
public:
	virtual void							AttachAsRoot					(class IRenderVisual* Visual) = 0;
	virtual void							Detach							(class IRenderVisual* Visual) = 0;
	virtual void							Attach							(class IRenderVisual* Visual,const char*BoneName) = 0;
	virtual class AStalkerProxy*			CastToStalkerProxy				() = 0;
	virtual class AStalkerPlayerCharacter*	CastToStalkerPlayerCharacter	() = 0;
	virtual void							Lock							(class CObject*) = 0;
	virtual void							Unlock							(class CObject*) = 0;
};