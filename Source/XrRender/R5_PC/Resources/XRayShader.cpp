#include "pch.h"
XRayShaderElement::~XRayShaderElement()
{
	for (size_t i = 0; i < 16; i++)
		GResourcesManager->Free(Textures[i]);
}

void XRayShaderElement::Clear()
{
	for (size_t i = 0; i < 16; i++)
		GResourcesManager->Free(Textures[i]);
	for (size_t i = 0; i < 16; i++)
		Textures[i] = 0;
		Pipeline.clear();
	DescriptorHeap.clear();
	RootSignature.clear();
}

void XRayShaderElement::Copy(const XRayShaderElement& right)
{
	Clear();
	memcpy(Textures, right.Textures, 16*sizeof(XRayTexture*));
	for (size_t i = 0; i < 16; i++)if (Textures[i])Textures[i]->Counter++;
	Pipeline.copy(right.Pipeline);
	RootSignature.copy(right.RootSignature);
	TypeTransformation = right.TypeTransformation;
	UseGlobalUniform = right.UseGlobalUniform;
	UseLocalUniform = right.UseLocalUniform;
	LocalUniform = right.LocalUniform;
	DetailScalar = right.DetailScalar;
	AlphaRef = right.AlphaRef;
	Sort = right.Sort;
	StencilRef = right.StencilRef;
	for (size_t i = 0; i < 16; i++)SamplerStates[i] = right.SamplerStates[i];
}

void XRayShaderElement::Swap(XRayShaderElement& right)
{
	for (size_t i = 0; i < 16; i++)
	std::swap(Textures[i], right.Textures[i]);
	Pipeline.swap(right.Pipeline);
	for (size_t i = 0; i < 16; i++)std::swap(SamplerStates[i] , right.SamplerStates[i]);
	RootSignature.swap(right.RootSignature);
	DescriptorHeap.swap(right.DescriptorHeap);
	std::swap(UseGlobalUniform, right.UseGlobalUniform);
	std::swap(UseLocalUniform, right.UseLocalUniform);
	std::swap(LocalUniform, right.LocalUniform);
	std::swap(DetailScalar, right.DetailScalar);
	std::swap(TypeTransformation, right.TypeTransformation);
	std::swap(AlphaRef, right.AlphaRef);
	std::swap(Sort, right.Sort);
	std::swap(StencilRef ,right.StencilRef);
}

void XRayShaderElement::CreateDescriptorHeap()
{
	if (!DescriptorHeap.empty())return;
	BearDescriptorHeapDescription DescriptorHeapDescription;
	DescriptorHeapDescription.RootSignature = RootSignature;
	DescriptorHeap = BearRenderInterface::CreateDescriptorHeap(DescriptorHeapDescription);
	for (size_t i = 0; i < 16; i++)

	{
		switch (SamplerStates[i])
		{
		case SSS_Default:
			DescriptorHeap->SetSampler(i, GResourcesManager->SamplerDefault);
			break;
		case	SSS_NoFilter:
			DescriptorHeap->SetSampler(i, GResourcesManager->SamplerNoFilter);
			break;
		case		SSS_Point:

			DescriptorHeap->SetSampler(i, GResourcesManager->SamplerPoint);
			break;
		case		SSS_RTLinear:

			DescriptorHeap->SetSampler(i, GResourcesManager->SamplerRTLinear);
			break;
		case		SSS_Linear:

			DescriptorHeap->SetSampler(i, GResourcesManager->SamplerLinear);
			break;
		default:
			break;
		}
	}
}

bool XRayShaderElement::Set(XRayObjectRender& item, u32 FVF)
{
	if (Pipeline.empty())return 0;
	auto i = Pipeline->Container.find(ToVertexDeclaration(FVF));
	if (i == Pipeline->Container.end())return false;
	if (i->second.empty())return false;

	item.Pipeline = i->second;

	item.DescriptorHeap = DescriptorHeap;
	item.UseGlobalUniform = UseGlobalUniform;
	item.UseLocalUniform = UseLocalUniform;
	if(item.UseLocalUniform)
		item.LocalUniform = LocalUniform;
	item.DetailScalar = DetailScalar;
	item.AlphaRef = AlphaRef;
	item.StencilRef = StencilRef;
	item.Sort = Sort;
	return true;
}