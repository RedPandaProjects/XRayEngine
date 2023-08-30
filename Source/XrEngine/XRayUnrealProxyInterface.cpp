#include "stdafx.h"
#include "XRayUnrealProxyInterface.h"

XRayUnrealAttachableInterface::~XRayUnrealAttachableInterface()
{
}

void* XRayUnrealAttachableInterface::QueryInterface(EXRayUnrealInterfaceType AttachableType)
{
	return nullptr;
}

void* XRayUnrealProxyInterface::QueryInterface(EXRayUnrealInterfaceType AttachableType)
{
	if(AttachableType == EXRayUnrealInterfaceType::StalkerProxy)
	{
		return this;
	}
	return XRayUnrealAttachableInterface::QueryInterface(AttachableType);
}

void* XRayUnrealPlayerCharacterInterface::QueryInterface(EXRayUnrealInterfaceType AttachableType)
{
	if(AttachableType == EXRayUnrealInterfaceType::StalkerPlayerCharacter)
	{
		return this;
	}
	return XRayUnrealProxyInterface::QueryInterface(AttachableType);
}
