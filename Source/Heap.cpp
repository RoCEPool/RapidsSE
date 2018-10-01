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

bool gbAddrAllocated = false;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

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
  m_zLock.vInitLock(LK_HEAP);
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
/* allocate an object from a heap                                             */
/*----------------------------------------------------------------------------*/
Object* Heap::pAllocate(eCN eAllocClass, bool bPersistent)
{
  eHT eHeap = geWhichHeap[eAllocClass];
  
  Object* pObject;
  
  if (bPersistent) {
    switch(eHeap) {
      case HT_TRANSIENT_SMALL:  eHeap = HT_PERSISTENT_SMALL;  break;
      case HT_TRANSIENT_MEDIUM: eHeap = HT_PERSISTENT_MEDIUM; break;
      case HT_TRANSIENT_LARGE:  eHeap = HT_PERSISTENT_LARGE;  break;
      case HT_TRANSIENT_JUMBO:  eHeap = HT_PERSISTENT_JUMBO;  break;
      default: break;
    }
  }
  
  switch(eHeap) {
    case HT_INVALID:
      vFatal("Heap::pAllocate: Invalid heap allocation: class %lld %s\n"_Stz,
             eAllocClass,aCN[eAllocClass]);
    
    case HT_PERSISTENT_SMALL:
      pObject = gpAttachMap->m_xHeaps.Persistent.m_pSmall->pAllocObject(eAllocClass);
      return pObject;
      
    case HT_PERSISTENT_MEDIUM:
      pObject = gpAttachMap->m_xHeaps.Persistent.m_pMedium->pAllocObject(eAllocClass);
      return pObject;
      
    case HT_PERSISTENT_LARGE:
      pObject = gpAttachMap->m_xHeaps.Persistent.m_pLarge->pAllocObject(eAllocClass);
      return pObject;
      
    case HT_PERSISTENT_JUMBO:
      pObject = gpAttachMap->m_xHeaps.Persistent.m_pJumbo->pAllocObject(eAllocClass);
      return pObject;
      
    case HT_TRANSIENT_SMALL:
      pObject = gpAttachMap->m_xHeaps.Transient.m_pSmall->pAllocObject(eAllocClass);
      return pObject;
      
    case HT_TRANSIENT_MEDIUM:
      pObject = gpAttachMap->m_xHeaps.Transient.m_pMedium->pAllocObject(eAllocClass);
      return pObject;

    case HT_TRANSIENT_LARGE:
      pObject = gpAttachMap->m_xHeaps.Transient.m_pLarge->pAllocObject(eAllocClass);
      return pObject;
      
    case HT_TRANSIENT_JUMBO:
      pObject = gpAttachMap->m_xHeaps.Transient.m_pJumbo->pAllocObject(eAllocClass);
      return pObject;
      
    default:
      vFatal("Heap::pAllocate: Invalid heap allocation: eHeap %lld\n"_Stz,
             eHeap);
  }
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* validate tables                                                            */
/*----------------------------------------------------------------------------*/
void Heap::vValidateTables(void)
{
  // note: using i8 for iClassNo instead of eCN because you cannot
  //       increment (++) an enum
  
  for (i8 iClassNo = CN_NONE; iClassNo < CN_ENUMS; iClassNo++) {
    
    eCN eClassNo = (eCN)iClassNo;
    
    eHT eHeap = geWhichHeap[eClassNo];
    i8  iSize = giClassSize[eHeap];
    
    switch(eHeap) {
      case HT_INVALID:
        break;
        
      case HT_TRANSIENT_SMALL:
        if (iSize >= C_SIZE_SMALL_OBJECT)
          vFatal("Heap::vValidateTables: transient small class %lld %s is too big\n"_Stz,
                 eClassNo,aCN[eClassNo]);
        //if (!iSize)
        //  vWarn("Heap::vValidateTables: small class %lld %lls has 0 size\n"_Stz,
        //         eClassNo,aCN[eClassNo]);
        break;
        
      case HT_TRANSIENT_MEDIUM:
        if (iSize >= C_SIZE_MEDIUM_OBJECT)
          vFatal("Heap::vValidateTables: transient medium class %lld %s is too big\n"_Stz,
                 eClassNo,aCN[eClassNo]);
        //if (!iSize)
        // vWarn("Heap::vValidateTables: medium class %lld %lls has 0 size\n"_Stz,
        //        eClassNo,aCN[eClassNo]);
        break;
        
      case HT_TRANSIENT_LARGE:
        if (iSize >= C_SIZE_LARGE_OBJECT)
          vFatal("Heap::vValidateTables: transient large class %lld %s is too big\n"_Stz,
                 eClassNo,aCN[eClassNo]);
        //if (!iSize)
        // vWarn("Heap::vValidateTables: large class %lld %lls has 0 size\n"_Stz,
        //        eClassNo,aCN[eClassNo]);
        break;
        
      case HT_TRANSIENT_JUMBO:
        if (iSize >= C_SIZE_JUMBO_OBJECT)
          vFatal("Heap::vValidateTables: transient jumbo class %lld %s is too big\n"_Stz,
                 eClassNo,aCN[eClassNo]);
        //if (!iSize)
        //  vWarn("Heap::vValidateTables: jumbo class %lld %lls has 0 size\n"_Stz,
        //        eClassNo,aCN[eClassNo]);
        break;
        
      case HT_PERSISTENT_SMALL:
        if (iSize >= C_SIZE_SMALL_OBJECT)
          vFatal("Heap::vValidateTables: persistent small class %lld %s is too big\n"_Stz,
                 eClassNo,aCN[eClassNo]);
        //if (!iSize)
        //  vWarn("Heap::vValidateTables: small class %lld %lls has 0 size\n"_Stz,
        //         eClassNo,aCN[eClassNo]);
        break;
        
      case HT_PERSISTENT_MEDIUM:
        if (iSize >= C_SIZE_MEDIUM_OBJECT)
          vFatal("Heap::vValidateTables: persistent medium class %lld %s is too big\n"_Stz,
                 eClassNo,aCN[eClassNo]);
        //if (!iSize)
        // vWarn("Heap::vValidateTables: medium class %lld %lls has 0 size\n"_Stz,
        //        eClassNo,aCN[eClassNo]);
        break;
        
      case HT_PERSISTENT_LARGE:
        if (iSize >= C_SIZE_LARGE_OBJECT)
          vFatal("Heap::vValidateTables: persistent large class %lld %s is too big\n"_Stz,
                 eClassNo,aCN[eClassNo]);
        //if (!iSize)
        // vWarn("Heap::vValidateTables: large class %lld %lls has 0 size\n"_Stz,
        //        eClassNo,aCN[eClassNo]);
        break;
        
      case HT_PERSISTENT_JUMBO:
        if (iSize >= C_SIZE_JUMBO_OBJECT)
          vFatal("Heap::vValidateTables: persistent jumbo class %lld %s is too big\n"_Stz,
                 eClassNo,aCN[eClassNo]);
        //if (!iSize)
        //  vWarn("Heap::vValidateTables: jumbo class %lld %lls has 0 size\n"_Stz,
        //        eClassNo,aCN[eClassNo]);
        break;
        
      default:
        vFatal("Heap::vValidateTables: Invalid which heap: eHeap %lld\n"_Stz,
               eHeap);
    }
  }
}

/*----------------------------------------------------------------------------*/
/* search free list                                                           */
/*----------------------------------------------------------------------------*/
void Heap::vSearchFree(void)
{
  if (gbAddrAllocated) {
    for (FreeChunk* pTestChunk = gpAttachMap->m_xHeaps.Transient.m_pMedium->m_pFreeHead;
         pTestChunk;
         pTestChunk = pTestChunk->pNextFree())
      if ((void*)pTestChunk >= (void*)0x204828fc0038 &&
          (void*)pTestChunk < (void*)0x204828fcfd30)
        int iDebug = true;
  }
  
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* free an object back to heap                                                */
/*----------------------------------------------------------------------------*/
void Heap::vFree(Object* pObject, bool bPersistent)
{
  eCN eAllocClass = pObject->eClass();
  
  if (gpBufferPool->bDontFree()) {
    
    if (CN_ENTRYBUFFER != eAllocClass) {
      pObject->vSetClass(CN_FREEDNOT);
      return;
    }
    
    if ( ! gpBufferPool->bFreeRB() ) {
      pObject->vSetClass(CN_FREEDNOT);
      return;
    }
  }
  
  // should not be freeing objects that are part of a word body
  ASSERT(!pObject->bWordBody());
  
  eHT eHeap = geWhichHeap[eAllocClass];
  
  if (bPersistent) {
    switch(eHeap) {
      case HT_TRANSIENT_SMALL:  eHeap = HT_PERSISTENT_SMALL;  break;
      case HT_TRANSIENT_MEDIUM: eHeap = HT_PERSISTENT_MEDIUM; break;
      case HT_TRANSIENT_LARGE:  eHeap = HT_PERSISTENT_LARGE;  break;
      case HT_TRANSIENT_JUMBO:  eHeap = HT_PERSISTENT_JUMBO;  break;
      default: break;
    }
  }
  
  switch(eHeap) {
    case HT_INVALID:
      MemLib::vWordDump(pObject,256,0);
      vFatal("Heap::vFree: Invalid heap free: class %lld %s\n"_Stz,
             eAllocClass,aCN[eAllocClass]);
      
    case HT_TRANSIENT_SMALL:
      gpAttachMap->m_xHeaps.Transient.m_pSmall->vFreeObject(pObject);
      return;
      
    case HT_TRANSIENT_MEDIUM:
      gpAttachMap->m_xHeaps.Transient.m_pMedium->vFreeObject(pObject);
      return;
      
    case HT_TRANSIENT_LARGE:
      gpAttachMap->m_xHeaps.Transient.m_pLarge->vFreeObject(pObject);
      return;
      
    case HT_TRANSIENT_JUMBO:
      gpAttachMap->m_xHeaps.Transient.m_pJumbo->vFreeObject(pObject);
      return;
      
    case HT_PERSISTENT_SMALL:
      gpAttachMap->m_xHeaps.Transient.m_pSmall->vFreeObject(pObject);
      return;
      
    case HT_PERSISTENT_MEDIUM:
      gpAttachMap->m_xHeaps.Transient.m_pMedium->vFreeObject(pObject);
      return;
      
    case HT_PERSISTENT_LARGE:
      gpAttachMap->m_xHeaps.Transient.m_pLarge->vFreeObject(pObject);
      return;
      
    case HT_PERSISTENT_JUMBO:
      gpAttachMap->m_xHeaps.Transient.m_pJumbo->vFreeObject(pObject);
      return;
      
    default:
      vFatal("Heap::vFree: Invalid heap free: eHeap %lld\n"_Stz,
             eHeap);
      
  }
}

/*----------------------------------------------------------------------------*/
/* free a non-object buffer                                                   */
/*----------------------------------------------------------------------------*/
void Heap::vFree_NonObject(Buffer* pBuffer)
{
  pBuffer->vSetMagic();

  gpAttachMap->m_xHeaps.Transient.m_pLarge->vFreeObject(pBuffer);
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
      vTrace("RUNNING OUT OF MEMORY!!!\n"_Stz);
      vWarn("Heap::pAllocObject: unable to extend heap\n"_Stz);
      m_zLock.vDropWrite();
      return nullptr;
    }
  }

  Object* pObject = static_cast<Object*>(pPopFree());
  
  m_iAllocCount++;

  m_zLock.vDropWrite();
  
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
    //vTrace("bExtendHeap %p : %p\n"_Stz,pHeapBuffer,pFreeChunk);
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
  
  ASSERT(CN_FREECHUNK == pFreeChunk->eClass());
  
  m_iFreeCount--;
  
  if (m_iObjectSize <= C_1MB) {
    vValidFree(pFreeChunk);
    if (m_pFreeHead)
      vValidFree(m_pFreeHead);
  }
    
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
  
  if (m_iObjectSize <= C_1MB && m_pFreeHead)
    vValidFree(m_pFreeHead);
  
  pFreeChunk->vSetClass(CN_FREECHUNK);
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
Footer* Heap::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
