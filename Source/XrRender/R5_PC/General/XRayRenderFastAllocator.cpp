#include "pch.h"
#define DEBUG_FASTMEMORY
#ifdef DEBUG_FASTMEMORY
static size_t LCounter = 0;
#endif
XRayRenderFastAllocator* GRenderFastAllocator = 0;

XRayRenderFastAllocator::XRayRenderFastAllocator()
{
	m_flush = false;
}

XRayRenderFastAllocator::~XRayRenderFastAllocator()
{
    while (m_firstBuffer)
    {
        Buffer* buffer = m_firstBuffer;
        m_firstBuffer = buffer->next;
        xr_free(buffer);
#ifdef DEBUG_FASTMEMORY
        LCounter--;
#endif
    }
    while (m_firstFreeBuffer) 
    {
        Buffer* buffer = m_firstFreeBuffer;
        m_firstFreeBuffer = buffer->next;
        xr_free(buffer);
#ifdef DEBUG_FASTMEMORY
        LCounter--;
#endif
    }
#ifdef DEBUG_FASTMEMORY
    R_ASSERT(LCounter == 0);
#endif
}

void* XRayRenderFastAllocator::Alloc()
{
    if (m_firstFreeBlock) {
        Block* block = m_firstFreeBlock;
        m_firstFreeBlock = block->next;
        return block;
    }

    if (m_bufferedBlocks >= count_block) {
        m_firstBuffer = AllocaBuffer();
        m_bufferedBlocks = 0;
    }

    return m_firstBuffer->getBlock(m_bufferedBlocks++);
}



void XRayRenderFastAllocator::FlushStart()
{
	m_flush = true;
}

void XRayRenderFastAllocator::FlushEnd()
{
    m_firstFreeBlock = 0;
    if (m_firstFreeBuffer == nullptr)
    {

        m_firstFreeBuffer = m_firstBuffer;
    }
    else
    {
        Buffer* buffer = m_firstFreeBuffer;
        while (buffer->next)
        {
            buffer = buffer->next;
        }
        buffer->next = m_firstBuffer;
    }
    m_firstBuffer = 0;
	m_flush = false;
    m_bufferedBlocks = count_block;
}

void XRayRenderFastAllocator::Free(void* data)
{
	if (m_flush)return;
    Block* block = reinterpret_cast<Block*>(data);
    block->next = m_firstFreeBlock;
    m_firstFreeBlock = block;
}

XRayRenderFastAllocator::Buffer* XRayRenderFastAllocator::AllocaBuffer()
{
    if (m_firstFreeBuffer)
    {
        Buffer* out = m_firstFreeBuffer;
        m_firstFreeBuffer = m_firstFreeBuffer->next;
        out->next = m_firstBuffer;
        return out;
    }
#ifdef DEBUG_FASTMEMORY
    LCounter++;
#endif
    return xr_new<Buffer>(m_firstBuffer);
}
