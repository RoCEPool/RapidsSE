/*============================================================================*/
/* HeapBuffer.cpp - segment of objects                                        */
/*============================================================================*/

#include "MemoryManagement.h"

extern BufferPool*  gpBufferPool;

/*----------------------------------------------------------------------------*/
/* get a new heap buffer from buffer pool                                     */
/*----------------------------------------------------------------------------*/
void* HeapBuffer::operator new(size_t)
{
  Buffer* pBuffer = gpBufferPool->pAllocBuffer();
  
  return pPoolCheck<void*>(pBuffer);
}

/*----------------------------------------------------------------------------*/
/* return a heap buffer to the buffer pool (no longer needed)                 */
/*----------------------------------------------------------------------------*/
void HeapBuffer::operator delete(void* pHeapBufferAddress)
{
  Buffer* pBuffer = static_cast<Buffer*>(pHeapBufferAddress);
  
  gpBufferPool->vFree(pBuffer);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
HeapBuffer::HeapBuffer(void)
: Buffer(CN_HEAPBUFFER)
{
  _poolAddress_(this);
  _magic_();
  _class_(CN_HEAPBUFFER);
  
  m_pNextBuffer = nullptr;
  m_pPrevBuffer = nullptr;
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
HeapBuffer::~HeapBuffer(void)
{
  _poolAddress_(this);
  _magic_();
  _class_(CN_HEAPBUFFER);
}

/*----------------------------------------------------------------------------*/
/* analyse the objects in this heap buffer                                    */
/*----------------------------------------------------------------------------*/
void HeapBuffer::vAnalyse(i8 nBytes, i8 xCounts[])
{
  _precondition_(_ADDRESS_,_MAGIC_);
 
  for (FreeChunk* pChunk = pFirstChunk();
       pChunk;
       pChunk = pNextChunk(pChunk,nBytes)) {

    eCN eClass = pChunk->eClass();
    if (CN_NONE == eClass)
      continue;
    
    xCounts[eClass]++;
  }
}

/*----------------------------------------------------------------------------*/
/* dump this heap buffer                                                      */
/*----------------------------------------------------------------------------*/
void HeapBuffer::vDump(void)
{
  _poolAddress_(this);
  _magic_();
  _class_(CN_HEAPBUFFER);
  
  vTrace("HeapBuffer: %p NextBuffer %p PrevBuffer %p\n",
         this,
         m_pNextBuffer,
         m_pPrevBuffer);
}

/*----------------------------------------------------------------------------*/
/* get next heap buffer in chain                                              */
/*----------------------------------------------------------------------------*/
HeapBuffer* HeapBuffer::pNextBuffer(void)
{
  _poolAddress_(this);
  _magic_();
  _class_(CN_HEAPBUFFER);
  
  return m_pNextBuffer;
}

/*----------------------------------------------------------------------------*/
/* set next heap buffer in chain                                              */
/*----------------------------------------------------------------------------*/
void HeapBuffer::vNextBuffer(HeapBuffer* pNextBuffer)
{
  _poolAddress_(this);
  _magic_();
  _class_(CN_HEAPBUFFER);
  
  m_pNextBuffer = pNextBuffer;
}

/*----------------------------------------------------------------------------*/
/* get prev heap buffer in chain                                              */
/*----------------------------------------------------------------------------*/
HeapBuffer* HeapBuffer::pPrevBuffer(void)
{
  _poolAddress_(this);
  _magic_();
  _class_(CN_HEAPBUFFER);
  
  return m_pPrevBuffer;
}

/*----------------------------------------------------------------------------*/
/* set prev heap buffer in chain                                              */
/*----------------------------------------------------------------------------*/
void HeapBuffer::vPrevBuffer(HeapBuffer* pPrevBuffer)
{
  _poolAddress_(this);
  _magic_();
  _class_(CN_HEAPBUFFER);
  
  m_pPrevBuffer = pPrevBuffer;
}

/*----------------------------------------------------------------------------*/
/* calculate address of first chunk in this heap buffer                       */
/*----------------------------------------------------------------------------*/
FreeChunk* HeapBuffer::pFirstChunk(void)
{
  _poolAddress_(this);
  _magic_();
  _class_(CN_HEAPBUFFER);
  
  FreeChunk* pFirstChunk = static_cast<FreeChunk*>(pvAdd(this,sizeof(HeapBuffer)));
  
  return pFirstChunk;
}

/*----------------------------------------------------------------------------*/
/* calculate address of next free chunk in this heap buffer                   */
/*----------------------------------------------------------------------------*/
FreeChunk* HeapBuffer::pNextChunk(FreeChunk* pChunk, i8 nBytes)
{
  _poolAddress_(this);
  _magic_();
  _class_(CN_HEAPBUFFER);
  
  // calculate address of next buffer
  FreeChunk* pNextChunk = static_cast<FreeChunk*>(pvAdd(pChunk,nBytes));

  // check next buffer ends within this segment
  if (pChunkEnd(pNextChunk,nBytes) > pLastByte())
    return nullptr;
  
  return pNextChunk;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* calculate address of the last byte of given Medium buffer                  */
/*----------------------------------------------------------------------------*/
char* HeapBuffer::pChunkEnd(FreeChunk* pChunk, i8 nBytes)
{
  _poolAddress_(this);
  _magic_();
  _class_(CN_HEAPBUFFER);
  
  char* pChunkEnd = pcAdd(pChunk,nBytes - 1);
  
  return pChunkEnd;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/