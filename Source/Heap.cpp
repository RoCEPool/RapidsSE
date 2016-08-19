/*============================================================================*/
/* Heap.cpp - heap allocator                                                  */
/*============================================================================*/

#include "MemoryManagement.h"

extern BufferPool* gpBufferPool;

/*----------------------------------------------------------------------------*/
/* new                                                                        */
/*----------------------------------------------------------------------------*/
void* Heap::operator new(size_t)
{
  Buffer* pBuffer = gpBufferPool->pAllocBuffer();
  
  return pPoolCheck<void*>(pBuffer);
}

/*----------------------------------------------------------------------------*/
/* delete                                                                     */
/*----------------------------------------------------------------------------*/
void Heap::operator delete(void* pHeapAddress)
{
  Buffer* pBuffer = static_cast<Buffer*>(pHeapAddress);
  
  gpBufferPool->vFree(pBuffer);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Heap::Heap(eCN eClassNo, i8 iObjectSize)
: Buffer(eClassNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,eClassNo);
  
  m_iObjectSize  = iObjectSize;
  m_zLock.vCheckInit();
  m_iAllocCount  = 0;
  m_iBufferCount = 0;
  m_pFirstBuffer = nullptr;
  m_pLastBuffer  = nullptr;
  m_iFreeCount   = 0;
  m_pFreeHead    = nullptr;
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Heap::~Heap(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
}

/*----------------------------------------------------------------------------*/
/* allocate an object from the heap                                           */
/*----------------------------------------------------------------------------*/
Object* Heap::pAllocObject(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_zLock.vGrabWrite();

  // check for free chunks in the heap and extend if none
  if (! m_pFreeHead ) {
    if (! bExtendHeap()) {
      // we have run out of space!!
      vWarn("Heap::pAllocObject: unable to extend heap\n");
      m_zLock.vDropWrite();
      return nullptr;
    }
  }

  Object* pObject = static_cast<Object*>(pPopFree());
  
  m_iAllocCount++;

  m_zLock.vDropWrite();
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* get number of allocated objects                                            */
/*----------------------------------------------------------------------------*/
i8 Heap::iAllocCount(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_iAllocCount;
}

/*----------------------------------------------------------------------------*/
/* analyse the objects in the heap                                           */
/*----------------------------------------------------------------------------*/
void Heap::vAnalyse(i8 xCounts[])
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_zLock.vGrabWrite();
  
  HeapBuffer* pHeapBuffer = m_pFirstBuffer;
  while (pHeapBuffer) {
    pHeapBuffer->vAnalyse(m_iObjectSize,xCounts);
    pHeapBuffer = pHeapBuffer->pNextBuffer();
  }
  
  m_zLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* get number of buffers in the heap                                          */
/*----------------------------------------------------------------------------*/
i8 Heap::iBufferCount(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_iBufferCount;
}

/*----------------------------------------------------------------------------*/
/* dump this heap                                                             */
/*----------------------------------------------------------------------------*/
void Heap::vDump(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  vTrace("Heap %p: BufferCount %lld FreeCount %lld\n",
         this,
         m_iBufferCount,
         m_iFreeCount);
}

/*----------------------------------------------------------------------------*/
/* get number of free chunks                                                  */
/*----------------------------------------------------------------------------*/
i8 Heap::iFreeCount(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_iFreeCount;
}

/*----------------------------------------------------------------------------*/
/* free object back to the heap                                               */
/*----------------------------------------------------------------------------*/
void Heap::vFreeObject(Object* pObject)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  m_zLock.vGrabWrite();
  
  vPushFree(static_cast<FreeChunk*>(pObject));
  
  m_iAllocCount--;

  m_zLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* get size of objects                                                        */
/*----------------------------------------------------------------------------*/
i8 Heap::iObjectSize(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_iObjectSize;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add another buffer of free chunks to this heap                             */
/*----------------------------------------------------------------------------*/
bool Heap::bExtendHeap(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  // - firstly, get a new heap buffer (from buffer pool)
  
  HeapBuffer* pHeapBuffer = new HeapBuffer;
  if ( ! pHeapBuffer )
    return false;
  
  // - secondly, add the heap buffer into the buffer list
  
  if ( ! m_pFirstBuffer ) {
    // first buffer in list
    ASSERT( ! m_pLastBuffer ); // should also be null
    m_pFirstBuffer = pHeapBuffer;
    m_pLastBuffer  = pHeapBuffer;
  }
  else {
    // add segment to end of list
    pHeapBuffer->vPrevBuffer(m_pLastBuffer);
    m_pLastBuffer->vNextBuffer(pHeapBuffer);
    m_pLastBuffer= pHeapBuffer;
  }

  m_iBufferCount++;
  
  // - thirdly, add all new free chunks into free chain
  
  FreeChunk* pFreeChunk;
  for (pFreeChunk = pHeapBuffer->pFirstChunk();
       pFreeChunk;
       pFreeChunk = pHeapBuffer->pNextChunk(pFreeChunk,m_iObjectSize)) {
    pFreeChunk->vSetMagic();
    vPushFree(pFreeChunk);
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop chunk from free chain                                                  */
/*----------------------------------------------------------------------------*/
FreeChunk* Heap::pPopFree(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  // free chain must exist
  ASSERT(m_pFreeHead);
  
  FreeChunk* pFreeChunk = m_pFreeHead;
  m_pFreeHead = pFreeChunk->pNextFree();
  
  m_iFreeCount--;
  
  return pFreeChunk;
}

/*----------------------------------------------------------------------------*/
/* push free chunk onto free chain                                            */
/*----------------------------------------------------------------------------*/
void Heap::vPushFree(FreeChunk* pFreeChunk)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  pFreeChunk->vNextFree(m_pFreeHead);
  m_pFreeHead = pFreeChunk;
  
  m_iFreeCount++;
}

/*----------------------------------------------------------------------------*/
/* verify Medium pool                                                         */
/*----------------------------------------------------------------------------*/
void Heap::vVerify(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/