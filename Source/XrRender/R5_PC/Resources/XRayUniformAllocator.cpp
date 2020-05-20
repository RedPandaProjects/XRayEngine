#include "pch.h"
XRayUniformAllocator* GUniformAllocator = 0;
XRayUniformAllocator::XRayUniformAllocator()
{
	GlobalUniform = BearRenderInterface::CreateUniformBuffer(sizeof(XRayGlobalUniform), 1, true);
	for (size_t i = 0; i < UT_Count; i++)
		m_Counter[i] = 0;
	
	{
		Reserve(UT_AlphaRef, 256);
		void* gptr = Uniform[XRayUniformAllocator::UT_AlphaRef]->Lock();
		for (size_t i = 0; i < 256; i++)
		{
			R_ASSERT(Alloc(UT_AlphaRef) == i);
			u8* ptr = (u8*)gptr + CalcOffset(XRayUniformAllocator::UT_AlphaRef, i);
			float* in_scalar = (float*)ptr;
			*in_scalar = float(i) / 255.f;
		}
		Uniform[XRayUniformAllocator::UT_AlphaRef]->Unlock();
	}
}

XRayUniformAllocator::~XRayUniformAllocator()
{
}

void XRayUniformAllocator::Reserve(EUniformType Type, size_t count)
{
	if (count == 0)return;
	if (Uniform[Type].empty())
	{
		Uniform[Type] = BearRenderInterface::CreateUniformBuffer(GetUniformElementSize(Type), bear_recommended_size(count), true);
		m_Size[Type] = bear_recommended_size(count);
	}
	if (Uniform[Type]->GetCount() < count)
	{
		Uniform[Type] = BearRenderInterface::CreateUniformBuffer(GetUniformElementSize(Type), bear_recommended_size(count), true);
		m_Size[Type] = bear_recommended_size(count);
	}
	

}

size_t XRayUniformAllocator::Alloc(EUniformType Type)
{
	R_ASSERT(m_Counter[Type] < m_Size[Type]);
	return m_Counter[Type]++;
}
