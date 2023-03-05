#pragma once
class XRayUnrealProxyInterface 
{
public:
	virtual void										AttachAsRoot								(class IRenderVisual* Visual) = 0;
	virtual void										Attach										(class IRenderVisual* Visual,const char*BoneName) = 0;
	virtual class AStalkerProxy*						CastToStalkerProxy							() = 0;
	virtual class AStalkerPlayerCharacter*				CastToStalkerPlayerCharacter				() = 0;
	virtual class XRayUnrealPlayerCharacterInterface*	CastToXRayUnrealPlayerCharacterInterface	() {return nullptr;}
	virtual void										Lock										(class CObject*) = 0;
	virtual void										Unlock										(class CObject*) = 0;
};

class XRayUnrealPlayerCharacterInterface:public XRayUnrealProxyInterface
{
public:
	class XRayUnrealPlayerCharacterInterface*			CastToXRayUnrealPlayerCharacterInterface	() {return this;}
	virtual void										AttachToCamera								(class IRenderVisual* Visual) = 0;
};