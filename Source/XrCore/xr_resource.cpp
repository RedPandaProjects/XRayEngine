#include "stdafx.h"

xr_resource::~xr_resource()
{

}

void xr_resource::Delete(xr_resource* Resource)
{
	xr_delete(Resource);
}
