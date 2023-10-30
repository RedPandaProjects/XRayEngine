#pragma once
class XRayLevelToBlueprint
{
public:
	virtual void			ActorTransferInfo	(shared_str Name,bool Value) = 0;
	virtual bool			ActorHasInfo		(shared_str Name) = 0;
	virtual class CObject*	GetActor			() =0;
	virtual void			SpawnObject			(shared_str SectionName,shared_str WayObjectName,int PointIndex = 0,float YawAngle = 0) =0;
	virtual float			GetGameTimeAsFloat	() = 0;
};