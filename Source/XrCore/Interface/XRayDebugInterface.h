#pragma once
class XRCORE_API XRayDebugInterface
{
public:
	virtual ~XRayDebugInterface();
	virtual void CriticalError(const char* Text) = 0;
};