#include "pch.h"


void XRayRenderInterface::add_Visual(IRenderVisual* V)
{
	Rdsgraph_Genderal.add_leafs_Dynamic((XRayRenderVisual*)V);
}

void XRayRenderInterface::add_Geometry(IRenderVisual* V)
{
	Rdsgraph_Genderal.add_Static((XRayRenderVisual*)V);
}