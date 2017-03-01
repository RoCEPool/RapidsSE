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
  RASSERT(iSize);
  
  Buffer* pBuffer = (Buffer*)Heap::pAllocate(CN_HEAPBUFFER);
  
  return pPoolCheck<void*>(pBuffer);
}

/*----------------------------------------------------------------------------*/
/* return a heap buffer to the buffer pool (no longer needed)                 */
/*----------------------------------------------------------------------------*/
void HeapBuffer::operator delete(void* pHeapBufferAddress)
{
  Buffer* pBuffer = static_cast<Buffer*>(pHeapBufferAddress);
  
  Heap::vFree(pBuffer);
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
  
  _checkInitialised_(CN_HEAPBUFFER);

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
void HeapBuffer::vDump(eCN eClass, Object* pObject, const char* aTab)
{
  ASSERT(eClass == CN_HEAPBUFFER);
  ASSERT(pObject->eClass() == CN_HEAPBUFFER);
  
  HeapBuffer* pHeapBuffer = reinterpret_cast<HeapBuffer*>(pObject);
  
  vTrace("nextBuffer %p prevBuffer %p\n",
         pHeapBuffer->m_pNextBuffer,
         pHeapBuffer->m_pPrevBuffer);
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
  
  FreeChunk* pFirstChunk = pAdd<FreeChunk*>(this,sizeof(HeapBuffer));
  
  return pFirstChunk;
}

/*----------------------------------------------------------------------------*/
/* calculate address of next free chunk in this heap buffer                   */
/*----------------------------------------------------------------------------*/
FreeChunk* HeapBuffer::pNextChunk(FreeChunk* pChunk, i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_HEAPBUFFER,_FOOTER_);
  
  // calculate address of next buffer
  FreeChunk* pNextChunk = pAdd<FreeChunk*>(pChunk,nBytes);

  // check next buffer ends within this segment
  if (pChunkEnd(pNextChunk,nBytes) > m_pLastByte)
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
  
  char* pChunkEnd = pAdd<char*>(pChunk,nBytes - 1);
  
  return pChunkEnd;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
