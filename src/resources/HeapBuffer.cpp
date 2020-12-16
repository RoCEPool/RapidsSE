/*============================================================================*/
/* HeapBuffer.cpp - segment of objects                                        */
/*============================================================================*/

#include "Resources.h"

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
HeapBuffer* Resources::_pHeapBuffer(void* aAddr)
{
  HeapBuffer* pHeapBuffer = reinterpret_cast<HeapBuffer*>(aAddr);
  
  RASSERT_TYPE(pHeapBuffer,CN_HEAPBUFFER);
  
  return pHeapBuffer;
}

/*----------------------------------------------------------------------------*/
/* get a new heap buffer from buffer pool                                     */
/*----------------------------------------------------------------------------*/
void* HeapBuffer::operator new(size_t)
{
  HeapBuffer* pHeapBuffer = (HeapBuffer*)gpBufferPool->pAllocHeapBuffer();
  
  return pPoolCheck<void*>(pHeapBuffer);
}

/*----------------------------------------------------------------------------*/
/* return a heap buffer to the buffer pool (no longer needed)                 */
/*----------------------------------------------------------------------------*/
void HeapBuffer::operator delete(void* pHeapBufferAddress)
{
  HeapBuffer* pHeapBuffer = static_cast<HeapBuffer*>(pHeapBufferAddress);
  
  gpBufferPool->vFree((PoolBuffer*)pHeapBuffer);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
HeapBuffer::HeapBuffer(void)
: PoolBuffer(CN_HEAPBUFFER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_HEAPBUFFER,_FOOTER_);
  
  ASSERT(CacheLine::bOnCacheLineBoundary(this));

  m_pNextBuffer = nullptr;
  m_pPrevBuffer = nullptr;
  
  _checkInitialised_(CN_HEAPBUFFER);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_HEAPBUFFER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
HeapBuffer::~HeapBuffer(void) noexcept(false)
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
  for (FreeChunk* aChunk = aFirstChunk();
       aChunk;
       aChunk = aNextChunk(aChunk,nBytes), iChunk++) {

    eCN eClass = aChunk->eClass();
    //vTrace("Analyse : %p %lld\n"_Stz,aChunk,eClass);
    
    if (eClass >= CN_ENUMS || eClass < 1) {
      vTrace("Ghost Object: %p %lld\n"_Stz,aChunk,(i8)eClass);
      continue;
    }
    
    if (CN_FREECHUNK == eClass)
      continue;

    xCounts[eClass]++;
  }
}

/*----------------------------------------------------------------------------*/
/* dump this heap buffer                                                      */
/*----------------------------------------------------------------------------*/
void HeapBuffer::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  HeapBuffer* pHeapBuffer = _pHeapBuffer(pObject);
  
  vTrace("nextBuffer %p prevBuffer %p\n"_Stz,
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
FreeChunk* HeapBuffer::aFirstChunk(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_HEAPBUFFER,_FOOTER_);
  
  FreeChunk* aFirstChunk = aAdd<FreeChunk*>(this,sizeof(HeapBuffer));
  
  ASSERT(CacheLine::bOnCacheLineBoundary(aFirstChunk));
  return aFirstChunk;
}

/*----------------------------------------------------------------------------*/
/* calculate address of next free chunk in this heap buffer                   */
/*----------------------------------------------------------------------------*/
FreeChunk* HeapBuffer::aNextChunk(FreeChunk* aChunk, i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_HEAPBUFFER,_FOOTER_);
  
  ASSERT(CacheLine::bMultipleCacheLines(nBytes));
  
  // calculate address of next buffer
  FreeChunk* aNextChunk = aAdd<FreeChunk*>(aChunk,nBytes);

  // check next buffer ends within this segment
  if (aChunkEnd(aNextChunk,nBytes) > aLastUsableByte())
    return nullptr;
  
  ASSERT(CacheLine::bOnCacheLineBoundary(aNextChunk));
  return aNextChunk;
}

/*----------------------------------------------------------------------------*/
/* analyse the objects in this heap buffer                                    */
/*----------------------------------------------------------------------------*/
void HeapBuffer::vVerify(i8 iChunkSize)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_HEAPBUFFER,_FOOTER_);
  
  i8 iChunk = 0;
  for (FreeChunk* aChunk = aFirstChunk();
       aChunk;
       aChunk = aNextChunk(aChunk,iChunkSize), iChunk++) {
    
    eCN eClass = aChunk->eClass();
    ASSERT(eClass < CN_ENUMS);
  }
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* calculate address of the last byte of given buffer                         */
/*----------------------------------------------------------------------------*/
Stz* HeapBuffer::aChunkEnd(FreeChunk* pChunk, i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_HEAPBUFFER,_FOOTER_);
  
  Stz* aChunkEnd = aAdd<Stz*>(pChunk,nBytes - 1);
  
  return aChunkEnd;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
