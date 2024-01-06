#include "stdafx.h"
#include "RBMKUnrealProxy.h"

IRBMKUnrealAttachable::~IRBMKUnrealAttachable()
{
}

void* IRBMKUnrealAttachable::QueryInterface(ERBMKUnrealInterfaceType AttachableType)
{
	return nullptr;
}

void* IRBMKUnrealProxy::QueryInterface(ERBMKUnrealInterfaceType AttachableType)
{
	if(AttachableType == ERBMKUnrealInterfaceType::StalkerProxy)
	{
		return this;
	}
	return IRBMKUnrealAttachable::QueryInterface(AttachableType);
}

void* IRBMKUnrealPlayerCharacter::QueryInterface(ERBMKUnrealInterfaceType AttachableType)
{
	if(AttachableType == ERBMKUnrealInterfaceType::StalkerPlayerCharacter)
	{
		return this;
	}
	return IRBMKUnrealProxy::QueryInterface(AttachableType);
}
