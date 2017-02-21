/*============================================================================*/
/* Heap.cpp -- Heap Allocator                                                 */
/*============================================================================*/

#include "Pool.h"

extern BufferPool* gpBufferPool;

extern i8  giClassSize[];
extern eHT geWhichHeap[];

extern BufferPool* gpBufferPool;
extern Heap*       gpSmallHeap;
extern Heap*       gpMediumHeap;
extern Heap*       gpJumboHeap;

/*----------------------------------------------------------------------------*/
/* new                                                                        */
/*----------------------------------------------------------------------------*/
void* Heap::operator new(size_t iSize)
{
  RASSERT(iSize);
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
  m_zLock.vInit("Heap::m_zLock");
  m_iAllocCount  = 0;
  m_iBufferCount = 0;
  m_pFirstBuffer = nullptr;
  m_pLastBuffer  = nullptr;
  m_iFreeCount   = 0;
  m_pFreeHead    = nullptr;
  m_bFixedHeap   = false;
  
  _checkInitialisedSize_(sizeof(Heap));

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
/* dump this heap                                                             */
/*----------------------------------------------------------------------------*/
void Heap::vDump(eCN eClass, Object* pObject, const char* pTab)
{
  ASSERT(eClass == CN_SMALLHEAP || eClass == CN_MEDIUMHEAP);
  ASSERT(pObject->eClass() == CN_SMALLHEAP || pObject->eClass() == CN_MEDIUMHEAP);
  
  Heap* pHeap = reinterpret_cast<Heap*>(pObject);
  
  vTrace("BufferCount %lld FreeCount %lld\n",
         pHeap->m_iBufferCount,
         pHeap->m_iFreeCount);
  
  i8 iCountObjects[CN_ENUMS];
  for (i8 index = 0; index < CN_ENUMS; index++) {
    iCountObjects[index] = 0;
  }
  
//  pHeap->vAnalyse(iCountObjects);
//  
//  vTrace("%s  Objects:\n", pTab);
//  for (i8 index = 0; index < CN_ENUMS; index++) {
//    eCN eClass = (eCN)index;
//    
//    if (iCountObjects[eClass] == 0)
//      continue;
//    
//    vTrace("%s    %s %lld \n", pTab, pCN[eClass], iCountObjects[eClass]);
//  }
//  vTrace("\n");  
}

/*----------------------------------------------------------------------------*/
/* allocate an object from a heap                                             */
/*----------------------------------------------------------------------------*/
Object* Heap::pAllocate(eCN eAllocClass)
{
  eHT eHeap = geWhichHeap[eAllocClass];
  
  Object* pObject;
  
  switch(eHeap) {
    case HT_INVALID:
      vFatal("Heap::pAllocate: Invalid heap allocation: class %lld %lls\n",
             eAllocClass,pCN[eAllocClass]);
    
    case HT_SMALL:
      pObject = gpSmallHeap->pAllocObject(eAllocClass);
      return pObject;
      
    case HT_MEDIUM:
      pObject = gpMediumHeap->pAllocObject(eAllocClass);
      return pObject;

    case HT_LARGE:
      pObject = gpBufferPool->pAllocBuffer();
      return pObject;
      
    case HT_JUMBO:
      pObject = gpJumboHeap->pAllocObject(eAllocClass);
      return pObject;
      
    default:
      vFatal("Heap::pAllocate: Invalid heap allocation: eHeap %lld\n",
             eHeap);
      
  }
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* free an object back to heap                                                */
/*----------------------------------------------------------------------------*/
void Heap::vFree(Object* pObject)
{
  eCN eAllocClass = pObject->eClass();
  
  // should not be freeing objects that are part of a word body
  ASSERT(!pObject->bWordBody());
  
  eHT eHeap = geWhichHeap[eAllocClass];
  
  switch(eHeap) {
    case HT_INVALID:
      vFatal("Heap::vFree: Invalid heap free: class %lld %lls\n",
             eAllocClass,pCN[eAllocClass]);
      
    case HT_SMALL:
      gpSmallHeap->vFreeObject(pObject);
      return;
      
    case HT_MEDIUM:
      gpMediumHeap->vFreeObject(pObject);
      return;
      
    case HT_LARGE:
      gpBufferPool->vFree((Buffer*)pObject);
      return;
      
    case HT_JUMBO:
      gpJumboHeap->vFreeObject(pObject);
      return;
      
    default:
      vFatal("Heap::vFree: Invalid heap free: eHeap %lld\n",
             eHeap);
      
  }
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add free chunk onto free chain                                             */
/*----------------------------------------------------------------------------*/
void Heap::vAddFree(FreeChunk* pFreeChunk)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);

  ASSERT(m_bFixedHeap);
  
  m_iBufferCount++;
  
  vPushFree(pFreeChunk);
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
/* allocate an object from the heap                                           */
/*----------------------------------------------------------------------------*/
Object* Heap::pAllocObject(eCN eAllocClass)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  ASSERT(giClassSize[eAllocClass] < m_iObjectSize);
  
  //static eCN eLastClass = CN_ENUMS;
  
  //vVerify();
  
  m_zLock.vGrabWrite();

  // check for free chunks in the heap and extend if none
  if (! m_pFreeHead ) {
    if (! bExtendHeap()) {
      // error: we have run out of space
      vWarn("Heap::pAllocObject: unable to extend heap\n");
      m_zLock.vDropWrite();
      return nullptr;
    }
  }

  Object* pObject = static_cast<Object*>(pPopFree());
  
  m_iAllocCount++;

  m_zLock.vDropWrite();
  
  Memory::vScribble(pObject,m_iObjectSize);
  
  //vVerify();
  
  //eLastClass = eAllocClass;
    
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* allocate an object from the heap                                           */
/*----------------------------------------------------------------------------*/
Object* Heap::pAllocObject(eCN eAllocClass, i8 iMaxSize)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  ASSERT(iMaxSize < m_iObjectSize);
  
  Object* pObject = pAllocObject(eAllocClass);
  
  return pObject;
}
  
/*----------------------------------------------------------------------------*/
/* analyse the objects in the heap                                            */
/*----------------------------------------------------------------------------*/
void Heap::vAnalyse(i8 iCounts[])
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  for (i8 index = 0; index < CN_ENUMS; index++)
    iCounts[index] = 0;
  
  m_zLock.vGrabWrite();
  
  HeapBuffer* pHeapBuffer = m_pFirstBuffer;
  while (pHeapBuffer) {
    pHeapBuffer->vAnalyse(m_iObjectSize,iCounts);
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
/* drop the lock                                                              */
/*----------------------------------------------------------------------------*/
void Heap::vDropLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  ASSERT(m_zLock.bIsLocked());
  m_zLock.vDropWrite();
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
/* set whether the heap is fixed                                              */
/*----------------------------------------------------------------------------*/
void Heap::vFixedHeap(bool bFixedHeap)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  m_bFixedHeap = bFixedHeap;
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
  
  //vVerify();
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
/* get the next buffer                                                       */
/*----------------------------------------------------------------------------*/
HeapBuffer* Heap::pNextBuffer(HeapBuffer* pBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  ASSERT(m_zLock.bIsLocked());
  
  return pBuffer->pNextBuffer();
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
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add another buffer of free chunks to this heap                             */
/*----------------------------------------------------------------------------*/
bool Heap::bExtendHeap(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  // check that we are free to extend this heap
  if ( m_bFixedHeap )
    return false;
  
  // get a free buffer from buffer pool
  HeapBuffer* pHeapBuffer = new HeapBuffer;
  if ( ! pHeapBuffer )
    return false;
  
  // add that freee buffer into this buffer list
  if ( ! m_pFirstBuffer ) {
    // set as first buffer in list
    ASSERT( ! m_pLastBuffer ); // should also be null
    m_pFirstBuffer = pHeapBuffer;
    m_pLastBuffer  = pHeapBuffer;
  }
  else {
    // add to end of list
    pHeapBuffer->vPrevBuffer(m_pLastBuffer);
    m_pLastBuffer->vNextBuffer(pHeapBuffer);
    m_pLastBuffer= pHeapBuffer;
  }

  // we have one more buffer
  m_iBufferCount++;
  
  // now add all new free chunks into free chain
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
  
  if (m_iObjectSize <= C_1MB)
    vValidFree(pFreeChunk);
  
  pFreeChunk->vClass(CN_FREECHUNK);
  pFreeChunk->vNextFree(m_pFreeHead);
  m_pFreeHead = pFreeChunk;
  
  m_iFreeCount++;
}

/*----------------------------------------------------------------------------*/
/* push free chunk onto free chain                                            */
/*----------------------------------------------------------------------------*/
void Heap::vValidFree(FreeChunk* pFreeChunk)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);

  HeapBuffer* pHeapBuffer = (HeapBuffer*)((u8)pFreeChunk & (u8)~0xFFFFF);
  
  ASSERT(pHeapBuffer->eClass() == CN_HEAPBUFFER);
  
  u8 uSize = (u8)pFreeChunk & (u8)0xFF;

  u8 uSizeHeapBuffer = sizeof(HeapBuffer);
  
  ASSERT(uSize == uSizeHeapBuffer);
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
