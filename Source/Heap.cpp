/*============================================================================*/
/* Heap.cpp -- Heap Allocator                                                 */
/*============================================================================*/

#include "StorageEngine.h"
#include "Pool.h"

extern AttachMap*  gpAttachMap;
extern BufferPool* gpBufferPool;

extern i8  giClassSize[];
extern eHT geWhichHeap[];

extern BufferPool* gpBufferPool;



/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* new                                                                        */
/*----------------------------------------------------------------------------*/
void* Heap::operator new(size_t)
{
  Buffer* pBuffer = gpBufferPool->pAllocOtherBuffer();
  
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
  m_okLock.vInitLock(LK_HEAP);
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
Heap::~Heap(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump this heap                                                             */
/*----------------------------------------------------------------------------*/
void Heap::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  RASSERT(eClass == CN_SMALLTRANSIENT  || eClass == CN_MEDIUMTRANSIENT ||
         eClass == CN_LARGETRANSIENT  || eClass == CN_JUMBOTRANSIENT  ||
         eClass == CN_SMALLPERSISTENT || eClass == CN_MEDIUMPERSISTENT ||
         eClass == CN_LARGEPERSISTENT || eClass == CN_JUMBOPERSISTENT);
  
  RASSERT(pObject->eClass() == CN_SMALLTRANSIENT   ||
         pObject->eClass() == CN_MEDIUMTRANSIENT  ||
         pObject->eClass() == CN_LARGETRANSIENT  ||
         pObject->eClass() == CN_JUMBOTRANSIENT   ||
         pObject->eClass() == CN_SMALLPERSISTENT  ||
         pObject->eClass() == CN_MEDIUMPERSISTENT ||
         pObject->eClass() == CN_LARGEPERSISTENT  ||
         pObject->eClass() == CN_JUMBOPERSISTENT);
  
  Heap* pHeap = reinterpret_cast<Heap*>(pObject);
  
  vTrace("BufferCount %lld FreeCount %lld\n"_Stz,
         pHeap->m_iBufferCount,
         pHeap->m_iFreeCount);
  
  i8 iCountObjects[CN_ENUMS];
  for (i8 i_Index = 0; i_Index < CN_ENUMS; i_Index++) {
    iCountObjects[i_Index] = 0;
  }
  
//  pHeap->vAnalyse(iCountObjects);
//  
//  vTrace("%s  Objects:\n"_Stz, aTab);
//  for (i8 i_Index = 0; i_Index < CN_ENUMS; i_Index++) {
//    eCN eClass = (eCN)i_Index;
//    
//    if (iCountObjects[eClass] == 0)
//      continue;
//    
//    vTrace("%s    %s %lld \n"_Stz, aTab, aCN[eClass], iCountObjects[eClass]);
//  }
//  vTrace("\n"_Stz);  
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add free chunk onto free chain                                             */
/*----------------------------------------------------------------------------*/
void Heap::vAddFree(FreeChunk* aFreeChunk)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);

  ASSERT(m_bFixedHeap);
  
  m_iBufferCount++;
  
  vPushFree(aFreeChunk,CN_NONE);
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
  
  m_okLock.vGrabWrite();

  // check for free chunks in the heap and extend if none
  if (! m_pFreeHead ) {
    if (! bExtendHeap()) {
      // error: we have run out of space
      vTrace("RUNNING OUT OF MEMORY!!!\n"_Stz);
      vWarn("Heap::pAllocObject: unable to extend heap\n"_Stz);
      m_okLock.vDropWrite();
      return nullptr;
    }
  }

  Object* pObject = reinterpret_cast<Object*>(pPopFree());
  
  m_iAllocCount++;

  m_okLock.vDropWrite();
  
#ifdef __DEBUG_CHECKS
  MemLib::vScribble(pObject,m_iObjectSize,eAllocClass);
#endif
  
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
  
  for (i8 i_Index = 0; i_Index < CN_ENUMS; i_Index++)
    iCounts[i_Index] = 0;
  
  m_okLock.vGrabWrite();
  
  HeapBuffer* pHeapBuffer = m_pFirstBuffer;
  while (pHeapBuffer) {
    pHeapBuffer->vAnalyse(m_iObjectSize,iCounts);
    pHeapBuffer = pHeapBuffer->pNextBuffer();
  }
  
  m_okLock.vDropWrite();
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
  
  ASSERT(m_okLock.bIsLocked());
  m_okLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* get the first buffer                                                       */
/*----------------------------------------------------------------------------*/
HeapBuffer* Heap::pFirstBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  ASSERT(m_okLock.bIsLocked());
  
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

  m_okLock.vGrabWrite();
  
  vPushFree(reinterpret_cast<FreeChunk*>(pObject),pObject->eClass());
  
  m_iAllocCount--;

  m_okLock.vDropWrite();
  
  //vVerify();
}

/*----------------------------------------------------------------------------*/
/* grab the lock                                                              */
/*----------------------------------------------------------------------------*/
void Heap::vGrabLock()
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  ASSERT(!m_okLock.bIsLocked());
  m_okLock.vGrabWrite();
  
}

/*----------------------------------------------------------------------------*/
/* monitor portrait                                                           */
/*----------------------------------------------------------------------------*/
void Heap::vMonitorPortrait(s48K& sPortrait)
{
  sPortrait.vSprint("Heap: size %lld buffers %lld allocated %lld free %lld\n"_Stz,
                    m_iObjectSize,m_iBufferCount,m_iAllocCount,m_iFreeCount);
}

/*----------------------------------------------------------------------------*/
/* get the next buffer                                                        */
/*----------------------------------------------------------------------------*/
HeapBuffer* Heap::pNextBuffer(HeapBuffer* pBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  ASSERT(m_okLock.bIsLocked());
  
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
  FreeChunk* aFreeChunk;
  for (aFreeChunk = pHeapBuffer->aFirstChunk();
       aFreeChunk;
       aFreeChunk = pHeapBuffer->aNextChunk(aFreeChunk,m_iObjectSize)) {
    //aFreeChunk->vSetMagic();
    new(aFreeChunk) FreeChunk;
    vPushFree(aFreeChunk,CN_NONE);
    //vTrace("bExtendHeap %p : %p\n"_Stz,pHeapBuffer,aFreeChunk);
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
  
  FreeChunk* aFreeChunk = m_pFreeHead;
  m_pFreeHead = aFreeChunk->pNextFree();
  
  ASSERT(CN_FREECHUNK == aFreeChunk->eClass());
  
  m_iFreeCount--;
  
  if (m_iObjectSize <= C_1MB) {
    vValidFree(aFreeChunk);
    if (m_pFreeHead)
      vValidFree(m_pFreeHead);
  }
    
  return aFreeChunk;
}

/*----------------------------------------------------------------------------*/
/* push free chunk onto free chain                                            */
/*----------------------------------------------------------------------------*/
void Heap::vPushFree(FreeChunk* aFreeChunk, eCN eWasClass)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  if (m_iObjectSize <= C_1MB)
    vValidFree(aFreeChunk);
  
  if (m_iObjectSize <= C_1MB && m_pFreeHead)
    vValidFree(m_pFreeHead);
  
  aFreeChunk->vSetClass(CN_FREECHUNK);
  aFreeChunk->vNextFree(m_pFreeHead);
  
#ifdef __DEBUG_CHECKS
  i8 nBytes = m_iObjectSize - sizeof(FreeChunk);
  void* aAddress = aAdd<void*>(aFreeChunk,sizeof(FreeChunk));
  MemLib::vScribble(aAddress,nBytes,CN_FREECHUNK);
#endif
  
  m_pFreeHead = aFreeChunk;
  
  m_iFreeCount++;
}

/*----------------------------------------------------------------------------*/
/* push free chunk onto free chain                                            */
/*----------------------------------------------------------------------------*/
void Heap::vValidFree(FreeChunk* aFreeChunk)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);

  HeapBuffer* pHeapBuffer = (HeapBuffer*)((u8)aFreeChunk & (u8)~0xFFFFF);
  
  ASSERT(pHeapBuffer->eClass() == CN_HEAPBUFFER);
  
  u8 uSize = (u8)aFreeChunk & (u8)0xFF;

  u8 uSizeHeapBuffer = sizeof(HeapBuffer);
  
  ASSERT(uSize == uSizeHeapBuffer);
}

/*----------------------------------------------------------------------------*/
/* verify heap                                                                */
/*----------------------------------------------------------------------------*/
void Heap::vVerify(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  m_okLock.vGrabWrite();
  
  HeapBuffer* pHeapBuffer = m_pFirstBuffer;
  while (pHeapBuffer) {
    pHeapBuffer->vVerify(m_iObjectSize);
    pHeapBuffer = pHeapBuffer->pNextBuffer();
  }
  
  m_okLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Heap::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
