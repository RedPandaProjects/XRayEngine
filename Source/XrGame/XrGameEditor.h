#include "..\XrEngine\XrGameEditorInterface.h"
class XrGameEditor:public XrGameEditorInterface
{
public:
	XrGameEditor();
	virtual ~XrGameEditor();
	virtual	void Create();
	virtual	void Destroy();
};