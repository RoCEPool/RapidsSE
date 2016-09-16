/*============================================================================*/
/* HeapBuffer.cpp - segment of objects                                        */
/*============================================================================*/

#include "Pool.h"

extern BufferPool*  gpBufferPool;

/*----------------------------------------------------------------------------*/
/* get a new heap buffer from buffer pool                                     */
/*----------------------------------------------------------------------------*/
void* HeapBuffer::operator new(size_t iSize)
{
  ASSERT(iSize);
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_HEAPBUFFER,_FOOTER_);
  
  m_pNextBuffer = nullptr;
  m_pPrevBuffer = nullptr;
  
  _checkInitialised_(sizeof(HeapBuffer));

  _postcondition_(_ADDRESS_,_MAGIC_,CN_HEAPBUFFER,_FOOTER_);
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_HEAPBUFFER,_FOOTER_);
 
  i8 iChunk = 0;
  for (FreeChunk* pChunk = pFirstChunk();
       pChunk;
       pChunk = pNextChunk(pChunk,nBytes), iChunk++) {

    eCN eClass = pChunk->eClass();
    //vTrace("Analyse %p : %p %lld\n",this,pChunk,eClass);
    
    if (CN_FREECHUNK == eClass)
      continue;
    
    xCounts[eClass]++;
  }
}

/*----------------------------------------------------------------------------*/
/* dump this heap buffer                                                      */
/*----------------------------------------------------------------------------*/
void HeapBuffer::vDump(eCN eClass, Object* pObject, const char* pTab)
{
  ASSERT(eClass == CN_HEAPBUFFER);
  ASSERT(pObject->eClass() == CN_HEAPBUFFER);
  
  HeapBuffer* pHeapBuffer = reinterpret_cast<HeapBuffer*>(pObject);
  
  vTrace("%sHeapBuffer: %p\n", pTab, pHeapBuffer);
  vTrace("%s  NextBuffer %p\n", pTab, pHeapBuffer->m_pNextBuffer);
  vTrace("%s  PrevBuffer %p\n\n", pTab, pHeapBuffer->m_pPrevBuffer);
}

/*----------------------------------------------------------------------------*/
/* get next heap buffer in chain                                              */
/*----------------------------------------------------------------------------*/
HeapBuffer* HeapBuffer::pNextBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_HEAPBUFFER,_FOOTER_);
  
  return m_pNextBuffer;
}

/*----------------------------------------------------------------------------*/
/* set next heap buffer in chain                                              */
/*----------------------------------------------------------------------------*/
void HeapBuffer::vNextBuffer(HeapBuffer* pNextBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_HEAPBUFFER,_FOOTER_);
  
  m_pNextBuffer = pNextBuffer;
}

/*----------------------------------------------------------------------------*/
/* get prev heap buffer in chain                                              */
/*----------------------------------------------------------------------------*/
HeapBuffer* HeapBuffer::pPrevBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_HEAPBUFFER,_FOOTER_);
  
  return m_pPrevBuffer;
}

/*----------------------------------------------------------------------------*/
/* set prev heap buffer in chain                                              */
/*----------------------------------------------------------------------------*/
void HeapBuffer::vPrevBuffer(HeapBuffer* pPrevBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_HEAPBUFFER,_FOOTER_);
  
  m_pPrevBuffer = pPrevBuffer;
}

/*----------------------------------------------------------------------------*/
/* calculate address of first chunk in this heap buffer                       */
/*----------------------------------------------------------------------------*/
FreeChunk* HeapBuffer::pFirstChunk(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_HEAPBUFFER,_FOOTER_);
  
  FreeChunk* pFirstChunk = static_cast<FreeChunk*>(pvAdd(this,sizeof(HeapBuffer)));
  
  return pFirstChunk;
}

/*----------------------------------------------------------------------------*/
/* calculate address of next free chunk in this heap buffer                   */
/*----------------------------------------------------------------------------*/
FreeChunk* HeapBuffer::pNextChunk(FreeChunk* pChunk, i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_HEAPBUFFER,_FOOTER_);
  
  // calculate address of next buffer
  FreeChunk* pNextChunk = static_cast<FreeChunk*>(pvAdd(pChunk,nBytes));

  // check next buffer ends within this segment
  if (pChunkEnd(pNextChunk,nBytes) > pLastUsableByte())
    return nullptr;
  
  return pNextChunk;
}

/*----------------------------------------------------------------------------*/
/* analyse the objects in this heap buffer                                    */
/*----------------------------------------------------------------------------*/
void HeapBuffer::vVerify(i8 iChunkSize)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_HEAPBUFFER,_FOOTER_);
  
  i8 iChunk = 0;
  for (FreeChunk* pChunk = pFirstChunk();
       pChunk;
       pChunk = pNextChunk(pChunk,iChunkSize), iChunk++) {
    
    eCN eClass = pChunk->eClass();
    ASSERT(eClass < CN_ENUMS);
  }
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* calculate address of the last byte of given buffer                         */
/*----------------------------------------------------------------------------*/
char* HeapBuffer::pChunkEnd(FreeChunk* pChunk, i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_HEAPBUFFER,_FOOTER_);
  
  char* pChunkEnd = pcAdd(pChunk,nBytes - 1);
  
  return pChunkEnd;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/