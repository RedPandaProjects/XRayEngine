#pragma once
class ENGINE_API XrGameEditorInterface
{
public:
	XrGameEditorInterface();
	virtual ~XrGameEditorInterface();
	virtual	void Create()=0;
	virtual	void Destroy()=0;
};