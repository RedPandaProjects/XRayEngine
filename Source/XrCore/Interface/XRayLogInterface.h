#pragma once
class XRCORE_API XRayLogInterface
{
public:
	virtual ~XRayLogInterface();
	virtual void Log(const char*Text) = 0;
};