#include "..\XrEngine\XrGameEditorInterface.h"
#ifndef SHIPPING
class XrGameEditor:public XrGameEditorInterface
{
public:
	XrGameEditor();
	virtual ~XrGameEditor();
	virtual	void Create();
	virtual	void Destroy();
};
#endif