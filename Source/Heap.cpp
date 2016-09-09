/*============================================================================*/
/* Heap.cpp -- Heap Allocator                                                 */
/*============================================================================*/

#include "Pool.h"

extern BufferPool* gpBufferPool;

extern i8 giClassSize[];

/*----------------------------------------------------------------------------*/
/* new                                                                        */
/*----------------------------------------------------------------------------*/
void* Heap::operator new(size_t iSize)
{
  ASSERT(iSize);
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
  
  Memory::vCheckInitialised(this,sizeof(Heap));

  _postcondition_(_ADDRESS_,_MAGIC_,eClassNo,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Heap::~Heap(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* allocate an object from the heap                                           */
/*----------------------------------------------------------------------------*/
Object* Heap::pAllocObject(eCN eAllocClass)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  ASSERT(giClassSize[eAllocClass] < m_iObjectSize);
  
  static eCN eLastClass = CN_ENUMS;
  
  vVerify();
  
  m_zLock.vGrabWrite();

  // check for free chunks in the heap and extend if none
  if (! m_pFreeHead ) {
    if (! bExtendHeap()) {
      // we have run out of space!!
      vWarn("Heap::pAllocObject: unable to extend heap\n");
      m_zLock.vDropWrite();
      return nullptr;
    }
    m_zLock.vDropWrite();
    vVerify();
    m_zLock.vGrabWrite();
  }

  Object* pObject = static_cast<Object*>(pPopFree());
  
  m_iAllocCount++;

  m_zLock.vDropWrite();
  
  Memory::vScribble(pObject,m_iObjectSize);
  
  //vVerify();
  
  eLastClass = eAllocClass;
    
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* get number of allocated objects                                            */
/*----------------------------------------------------------------------------*/
i8 Heap::iAllocCount(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  return m_iAllocCount;
}

/*----------------------------------------------------------------------------*/
/* analyse the objects in the heap                                            */
/*----------------------------------------------------------------------------*/
void Heap::vAnalyse(i8 xCounts[])
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
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
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  return m_iBufferCount;
}

/*----------------------------------------------------------------------------*/
/* grab the lock                                                              */
/*----------------------------------------------------------------------------*/
void Heap::vGrabLock()
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  ASSERT(!m_zLock.bIsLocked());
  m_zLock.vGrabWrite();
  
}

/*----------------------------------------------------------------------------*/
/* get the first buffer                                                       */
/*----------------------------------------------------------------------------*/
HeapBuffer* Heap::pFirstBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  ASSERT(m_zLock.bIsLocked());
  
  return m_pFirstBuffer;
}

/*----------------------------------------------------------------------------*/
/* drop the lock                                                              */
/*----------------------------------------------------------------------------*/
void Heap::vDropLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  ASSERT(m_zLock.bIsLocked());
  m_zLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* dump this heap                                                             */
/*----------------------------------------------------------------------------*/
void Heap::vDump(eCN eClass, Object* pObject, const char* pTab)
{
  ASSERT(eClass == CN_SMALLHEAP || eClass == CN_MEDIUMHEAP);
  ASSERT(pObject->eClass() == CN_SMALLHEAP || pObject->eClass() == CN_MEDIUMHEAP);
  
  Heap* pHeap = reinterpret_cast<Heap*>(pObject);
  
  vTrace("%s%s %p:\n", pTab, eClass == CN_SMALLHEAP ? "SmallHeap" : "MediumHeap", pHeap);
  vTrace("%s  BufferCount %lld\n", pTab, pHeap->m_iBufferCount);
  vTrace("%s  FreeCount %lld\n\n",  pTab, pHeap->m_iFreeCount);
  
  i8 iCountObjects[CN_ENUMS];
  for (i8 index = 0; index < CN_ENUMS; index++) {
    iCountObjects[index] = 0;
  }
  
  pHeap->vAnalyse(iCountObjects);
  
  vTrace("%s  Objects:\n", pTab);
  for (i8 index = 0; index < CN_ENUMS; index++) {
    eCN eClass = (eCN)index;
    
    if (iCountObjects[eClass] == 0)
      continue;
    
    vTrace("%s    %s %lld \n", pTab, pCN[eClass], iCountObjects[eClass]);
  }
  vTrace("\n");
  
}

/*----------------------------------------------------------------------------*/
/* get the next buffer                                                       */
/*----------------------------------------------------------------------------*/
HeapBuffer* Heap::pNextBuffer(HeapBuffer* pBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  ASSERT(m_zLock.bIsLocked());
  
  return pBuffer->pNextBuffer();
}

/*----------------------------------------------------------------------------*/
/* get number of free chunks                                                  */
/*----------------------------------------------------------------------------*/
i8 Heap::iFreeCount(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  return m_iFreeCount;
}

/*----------------------------------------------------------------------------*/
/* free object back to the heap                                               */
/*----------------------------------------------------------------------------*/
void Heap::vFreeObject(Object* pObject)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);

  m_zLock.vGrabWrite();
  
  vPushFree(static_cast<FreeChunk*>(pObject));
  
  m_iAllocCount--;

  m_zLock.vDropWrite();
  
  vVerify();
}

/*----------------------------------------------------------------------------*/
/* get size of objects                                                        */
/*----------------------------------------------------------------------------*/
i8 Heap::iObjectSize(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
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
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
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
    //pFreeChunk->vSetMagic();
    new(pFreeChunk) FreeChunk;
    vPushFree(pFreeChunk);
    //vTrace("bExtendHeap %p : %p\n",pHeapBuffer,pFreeChunk);
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop chunk from free chain                                                  */
/*----------------------------------------------------------------------------*/
FreeChunk* Heap::pPopFree(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
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
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  pFreeChunk->vClass(CN_FREECHUNK);
  pFreeChunk->vNextFree(m_pFreeHead);
  m_pFreeHead = pFreeChunk;
  
  m_iFreeCount++;
}

/*----------------------------------------------------------------------------*/
/* verify heap                                                                */
/*----------------------------------------------------------------------------*/
void Heap::vVerify(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  m_zLock.vGrabWrite();
  
  HeapBuffer* pHeapBuffer = m_pFirstBuffer;
  while (pHeapBuffer) {
    pHeapBuffer->vVerify(m_iObjectSize);
    pHeapBuffer = pHeapBuffer->pNextBuffer();
  }
  
  m_zLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Heap::pFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/