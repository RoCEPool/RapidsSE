/*============================================================================*/
/* Heap.cpp -- Heap Allocator                                                 */
/*============================================================================*/

#include "BASE.h"

#include "AM.hpp"
#include "LogicUnit.hpp"
#include "Job.hpp"
#include "SeDeputy.hpp"
#include "StepAccum.hpp"
#include "Op.hpp"

extern BufferPool* gpBufferPool;

extern SizeHeap  gxaSizeHeap[];

extern pthread_key_t gbTLS_FaultInjection;
extern pthread_key_t giTLS_FaultPoint;
extern pthread_key_t giTLS_CurrentPoint;


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* new                                                                        */
/*----------------------------------------------------------------------------*/
void* Heap::operator new(size_t)
{
  PoolBuffer* pBuffer = gpBufferPool->pAllocOtherBuffer();
  
  return pPoolCheck<void*>(pBuffer,__func__);
}

/*----------------------------------------------------------------------------*/
/* delete                                                                     */
/*----------------------------------------------------------------------------*/
void Heap::operator delete(void* pHeapAddress)
{
  PoolBuffer* pBuffer = static_cast<PoolBuffer*>(pHeapAddress);
  
  gpBufferPool->vFreeOtherBuffer(pBuffer);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Heap::Heap(eCN eClassNo, i8 iObjectSize, eHT eHeap)
: PoolBuffer(eClassNo)
, m_okLock(this)
{
  _precondition_(_ADDRESS_,_MAGIC_,eClassNo);
  
  m_eHeap          = eHeap;
  m_nObjectSize    = iObjectSize;
  m_nCurrAllocated = 0;
  m_nBufferCount   = 0;
  m_pFirstBuffer   = nullptr;
  m_pLastBuffer    = nullptr;
  m_nFreeChunks    = 0;
  m_pFreeHead      = nullptr;
  m_bFixedHeap     = false;
  
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
  UNUSED(aTab);
  
  RASSERT(
          eClass == CN_TINYTRANSIENT    ||
          eClass == CN_SMALLTRANSIENT   ||
          eClass == CN_MEDIUMTRANSIENT  ||
          eClass == CN_LARGETRANSIENT   ||
          
          eClass == CN_TINYPERSISTENT   ||
          eClass == CN_SMALLPERSISTENT  ||
          eClass == CN_MEDIUMPERSISTENT ||
          eClass == CN_LARGEPERSISTENT
          );
  
  RASSERT(
          pObject->eGetClassNo() == CN_TINYTRANSIENT    ||
          pObject->eGetClassNo() == CN_SMALLTRANSIENT   ||
          pObject->eGetClassNo() == CN_MEDIUMTRANSIENT  ||
          pObject->eGetClassNo() == CN_LARGETRANSIENT   ||
           
          pObject->eGetClassNo() == CN_TINYPERSISTENT  ||
          pObject->eGetClassNo() == CN_SMALLPERSISTENT  ||
          pObject->eGetClassNo() == CN_MEDIUMPERSISTENT ||
          pObject->eGetClassNo() == CN_LARGEPERSISTENT
          );
  
  Heap* pHeap = reinterpret_cast<Heap*>(pObject);
  
  vTrace("BufferCount %lld FreeCount %lld\n"_Stz,
         pHeap->m_nBufferCount,
         pHeap->m_nFreeChunks);
  
//  i8 iCountObjects[CN_ENUMS];
//  for (i8 i_Index = 0; i_Index < CN_ENUMS; i_Index++) {
//    iCountObjects[i_Index] = 0;
//  }
  
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
/* allocate an object from the heap                                           */
/*----------------------------------------------------------------------------*/
Object* Heap::pAllocObject(eCN eAllocClass)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  ASSERT(gxaSizeHeap[eAllocClass].m_nClassSize < m_nObjectSize);

  UNUSED(eAllocClass);

  AM::MEMORY::vAssertAllocate();

  Job* pJob = MY::pJob();
  Thread* pThread = MY::pThread();
  
//  if (pThread && pThread->iMallocWatch()) {
//    vTrace("allocating %s"_Stz,aCN[eAllocClass]);
//    vTrace("\n"_Stz);
//    i8 iDebug = 1;
//    UNUSED(iDebug);
//  }


//#ifdef WITH_OOM_FAULTINJECTION
//  bool bFaultInjection = static_cast<bool>(PsxThd::pTlsGet(gbTLS_FaultInjection));
//  if (bFaultInjection) {
//    i8 iFaultPoint = (i8)(PsxThd::pTlsGet(giTLS_FaultPoint));
//    i8 iCurrentPoint = (i8)(PsxThd::pTlsGet(giTLS_CurrentPoint));
//    vTrace("FI: FP:%lld CP:%lld\n"_Stz, iFaultPoint, iCurrentPoint);
//    if (iFaultPoint == iCurrentPoint)
//      THROW_LOWMEMORY();
//  }
//#endif

  m_okLock.vGrabWrite();
  
  // check for free chunks in the heap and extend if none
  if (! m_pFreeHead ) {

    // other buffers
    if (! bExtendHeap()) {
//      PoolHelper::vPoolAnalyze();
      // error: we have run out of space
      vTrace("NODE%lld: HEAP RUNNING OUT OF MEMORY!!!\n"_Stz, AM::NODE::iThisNodeId());
      //vTrace("************************************************\n"_Stz);
      //vTrace("LOW MEMORY encountered : %s %s %lld %s!!!\n"_Stz,
      //       __FILE__,__func__,(i8)__LINE__, aCN[eAllocClass]);
      //vThreadCallStackDump();
      //vTrace("************************************************\n"_Stz);
      m_okLock.vDropWrite();
      //PoolHelper::vPoolAnalyze();
      THROW_LOWMEMORY();
      return nullptr;
    }
  }

  Object* pObject = reinterpret_cast<Object*>(pPopFree());
  
  m_nCurrAllocated++;

#ifdef WITH_CHECK_INITIALISED
  MemLib::vScribble(pObject,m_nObjectSize,eAllocClass);
#endif
  
  m_okLock.vDropWrite();

  LogicUnit* pLogicUnit = MY::pOpLogicUnit();
  if (pLogicUnit) {

    if (pLogicUnit->m_xpStats.bIsNotNull()) {

      pLogicUnit->m_xpStats->m_nAllocs++;
      pLogicUnit->m_xpStats->m_nBytesAlloc += m_nObjectSize;

      i8 nMaxBytes = pLogicUnit->m_xpStats->m_nBytesAlloc - pLogicUnit->m_xpStats->m_nBytesFree;
      if (nMaxBytes > pLogicUnit->m_xpStats->m_nMaxBytes)
        pLogicUnit->m_xpStats->m_nMaxBytes = nMaxBytes;
    }
  }

  if (pJob)
  {
    pJob->vTrackAllocObject(eAllocClass);
  }
  else if(pThread && pThread->pObject0()) {
    Job* pJob = _pJob(MY::pThread()->pObject0());
    pJob->vTrackAllocObject(eAllocClass);
  }

  if(MY::pStepAccum())
  {
    MY::pStepAccum()->vHeapAllocCount(m_eHeap);
    MY::pStepAccum()->m_nSumAllocs++;
  }

  //bool bSetLogMemory = false;
  

  //vVerify();
  
  //eLastClass = eAllocClass;
  
  ASSERT(CacheLine::bOnCacheLineBoundary(pObject));
    
//  if ((Object*)0x2048091aa040 == pObject) {
//    vTrace("AllocObject:: %p %s:%-3lld (address)\n"_Stz,
//           pObject,aCN[eAllocClass],eAllocClass);
//    vDebugPoint();
//  }
//  else
//  if (CN_PIPELINE == eAllocClass) {
//    vTrace("AllocObject:: %p %s:%-3lld (class)\n"_Stz,
//           pObject,aCN[eAllocClass],eAllocClass);
//    vDebugPoint();
//  }
  
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
    pHeapBuffer->vAnalyse(m_nObjectSize,iCounts);
    pHeapBuffer = pHeapBuffer->pNextBuffer();
  }
  
  m_okLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* get number of buffers in the heap                                          */
/*----------------------------------------------------------------------------*/
i8 Heap::nBufferCount(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  return m_nBufferCount;
}

/*----------------------------------------------------------------------------*/
/* get number of allocated objects                                            */
/*----------------------------------------------------------------------------*/
i8 Heap::nCurrAllocated(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  return m_nCurrAllocated;
}

/*----------------------------------------------------------------------------*/
/* drop the lock                                                              */
/*----------------------------------------------------------------------------*/
void Heap::vDropWriteLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  ASSERT(m_okLock.bIsLocked());
  m_okLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* drop the lock                                                              */
/*----------------------------------------------------------------------------*/
void Heap::vDropReadLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);

  ASSERT(m_okLock.bIsLocked());
  m_okLock.vDropRead();
}

/*----------------------------------------------------------------------------*/
/* dump the heap                                                              */
/*----------------------------------------------------------------------------*/
void Heap::vDumpHeap(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);

  vGrabWriteLock();

  i8 nSizeObject = nObjectSize();
  
  for (HeapBuffer* pHeapBuffer = pFirstBuffer();
       pHeapBuffer;
       pHeapBuffer = pHeapBuffer->pNextBuffer()) {

    for (FreeChunk* aChunk = pHeapBuffer->aFirstChunk();
         aChunk;
         aChunk = pHeapBuffer->aNextChunk(aChunk,nSizeObject)) {
      
      eCN eChunkClass = aChunk->eGetClassNo();
      
      if (CN_FREECHUNK == eChunkClass) {
                
        vTrace("%p %s:%-3lld (was %s:%-3lld)\n"_Stz
               ,aChunk
               ,aCN[eChunkClass]
               ,eChunkClass
               ,aCN[aChunk->eWasClass()]
               ,aChunk->eWasClass()
               );

        continue;
      }
  
      Object* pObject = (Object*)aChunk;
      
      vTrace("%p %s:%-3lld\n"_Stz
             ,pObject
             ,aCN[pObject->eGetClassNo()]
             ,pObject->eGetClassNo()
             );
    }

  }

  vDropWriteLock();
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
i8 Heap::nFreeChunks(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  return m_nFreeChunks;
}

/*----------------------------------------------------------------------------*/
/* free object back to the heap                                               */
/*----------------------------------------------------------------------------*/
void Heap::vFreeObject(Object* pObject)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);

  eCN eClassNo = pObject->eGetClassNo();

  Job* pJob = MY::pJob();
  if(pJob && pJob->bObjectTracker())
  {
    pJob->vTrackFreeObject(eClassNo);
  }

  if(MY::pStepAccum())
  {
    MY::pStepAccum()->vHeapFreeCount(m_eHeap);
    MY::pStepAccum()->m_nSumFrees++;
  }

  //Thread* pThread = MY::pThread();
  
  //if (pThread && pThread->iMallocWatch()) {
  //  i8 iDebug = 1;
  //  UNUSED(iDebug);
  //}


//  if ((Object*)0x2048091aa040 == pObject) {
//    vTrace("FreeObject:: %p %s:%-3lld (address)\n"_Stz,
//           pObject,aCN[eClassNo],eClassNo);
//    vDebugPoint();
//  }
//  else
//  if (CN_PIPELINE == eClassNo) {
//    vTrace("FreeObject:: %p %s:%-3lld (class)\n"_Stz,
//           pObject,aCN[eClassNo],eClassNo);
//    vDebugPoint();
//  }

  
//  if (eEC eError = pObject->eValidObject()) {
//    
//    if (EC_OBJECT_FOOTER_INVALID == eError) {
//      vTrace("Object %p %s is not valid : %s\n"_Stz,
//             pObject,aCN[pObject->eGetClassNo()],aEC[eError]);
//    }
//    else {
//      vTrace("Object %p is not valid : %s\n"_Stz,
//             pObject,aEC[eError]);
//    }
//  }
//  else {
//    //vTrace("Object %p valid\n"_Stz,pObject);
//  }

  //if (CN_LARGETRANSIENT == eClassNo) {
    
  if (HT_T_LARGE == gxaSizeHeap[eClassNo].m_eWhichHeap ||
      HT_P_LARGE == gxaSizeHeap[eClassNo].m_eWhichHeap) {
    
    RASSERT(CN_LARGETRANSIENT == this->eGetClassNo() ||
            CN_LARGEPERSISTENT == this->eGetClassNo());
    
    //vTrace("FREE CN_LARGETRANSIENT\n"_Stz);
    
    if (m_nFreeChunks > 10) {
      //vTrace("FREE INTO POOL\n"_Stz);
      
      PoolBuffer* pPoolBuffer = PoolBuffer::pWithinPoolBuffer(pObject);
      
      HeapBuffer* pHeapBuffer = _pHeapBuffer(pPoolBuffer);
      vRemoveBuffer(pHeapBuffer);
      
      gpBufferPool->vFreeHeapBuffer(pPoolBuffer);
      m_okLock.vGrabWrite();
      m_nCurrAllocated--;
      m_okLock.vDropWrite();

      LogicUnit* pLogicUnit = MY::pOpLogicUnit();
      if (pLogicUnit) {
        pLogicUnit->m_xpStats->m_nFrees++;
        pLogicUnit->m_xpStats->m_nBytesFree += m_nObjectSize;
      }

      return;
    }
  }
  
  m_okLock.vGrabWrite();
  
  FreeChunk* aFreeChunk = reinterpret_cast<FreeChunk*>(pObject);
  vPushFree(aFreeChunk,eClassNo);
  
  m_nCurrAllocated--;

  m_okLock.vDropWrite();

  LogicUnit* pLogicUnit = MY::pOpLogicUnit();
  if (pLogicUnit) {
    pLogicUnit->m_xpStats->m_nFrees++;
    pLogicUnit->m_xpStats->m_nBytesFree += m_nObjectSize;
  }

  //vVerify();
}

/*----------------------------------------------------------------------------*/
/* grab the lock                                                              */
/*----------------------------------------------------------------------------*/
void Heap::vGrabWriteLock()
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  //ASSERT(!m_okLock.bIsLocked());
  m_okLock.vGrabWrite();
  
}

/*----------------------------------------------------------------------------*/
/* grab the lock                                                              */
/*----------------------------------------------------------------------------*/
void Heap::vGrabReadLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);

  //ASSERT(!m_okLock.bIsLocked());
  m_okLock.vGrabRead();

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
i8 Heap::nObjectSize(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  return m_nObjectSize;
}

/*----------------------------------------------------------------------------*/
/* form report string                                                         */
/*----------------------------------------------------------------------------*/
void Heap::vSummaryReport(String* pReportString)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  s32 sMagSum;   MagLib::vByteMag(nTotalHeapSize(),sMagSum);
  s32 sMagSize;  MagLib::vByteMag(nObjectSize(),sMagSize);

  pReportString->vSprint(
                                  
    "%16s (%6s)"
    " buffers %-6lld alloc %-6lld free %-6lld (sum=%s)"_Stz,
                                  
    aCN[eGetClassNo()],
    sMagSize.azChar(),
                                  
    nBufferCount(),
    nCurrAllocated(),
    nFreeChunks(),
    sMagSum.szStz()
    );
}

/*----------------------------------------------------------------------------*/
/* get computed size of heap                                                  */
/*----------------------------------------------------------------------------*/
i8 Heap::nTotalHeapSize(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  i8 iSum =   nCurrAllocated() * nObjectSize()
            +
              nFreeChunks() * nObjectSize();
  
  return iSum;
}

/*----------------------------------------------------------------------------*/
/* validate all the objects in this heap                                      */
/*----------------------------------------------------------------------------*/
void Heap::vValidateContents(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);

  eEC eError;

  vGrabWriteLock();

  i8 nSizeObject = nObjectSize();
  
  for (HeapBuffer* pHeapBuffer = pFirstBuffer();
       pHeapBuffer;
       pHeapBuffer = pHeapBuffer->pNextBuffer()) {

    for (FreeChunk* aChunk = pHeapBuffer->aFirstChunk();
         aChunk;
         aChunk = pHeapBuffer->aNextChunk(aChunk,nSizeObject)) {
      
      eCN eClass = aChunk->eGetClassNo();
      
      if (CN_FREECHUNK == eClass)
        continue;
                
      Object* pObject = (Object*)aChunk;
      
      if ((eError = pObject->eValidObject())) {
        
        if (EC_OBJECT_FOOTER_INVALID == eError) {
          vTrace("Object %p %s is not valid : %s\n"_Stz,
                 pObject,aCN[pObject->eGetClassNo()],aEC[eError]);
        }
        else {
          vTrace("Object %p is not valid : %s\n"_Stz,
                 pObject,aEC[eError]);
        }
      }
      else {
        //vTrace("Object %p valid\n"_Stz,pObject);
      }
    }

  }

  vDropWriteLock();
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

  // check if any free buffers in the pool
  if ( ! (HeapBuffer*)gpBufferPool->nFreeCount())
    return false;

  // get a free buffer from buffer pool
  HeapBuffer* pHeapBuffer = new HeapBuffer;
  if ( ! pHeapBuffer )
    return false;
  
  // add that free buffer into this buffer list
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
  m_nBufferCount++;
  
  // now add all new free chunks into free chain
  FreeChunk* aFreeChunk;
  for (aFreeChunk = pHeapBuffer->aFirstChunk();
       aFreeChunk;
       aFreeChunk = pHeapBuffer->aNextChunk(aFreeChunk,m_nObjectSize)) {
    //aFreeChunk->vMagic();
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
  
  ASSERT(CN_FREECHUNK == aFreeChunk->eGetClassNo());
  
  m_nFreeChunks--;
  
  if (m_nObjectSize <= C_1MB) {
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
  UNUSED(eWasClass);

  if (m_nObjectSize <= C_1MB)
    vValidFree(aFreeChunk);
  
  if (m_nObjectSize <= C_1MB && m_pFreeHead)
    vValidFree(m_pFreeHead);
  
  aFreeChunk->vSetClassNo(CN_FREECHUNK);
  aFreeChunk->vNextFree(m_pFreeHead);
  aFreeChunk->vWasClass(eWasClass);

#ifdef WITH_CHECK_INITIALISED
  i8 nBytes = m_nObjectSize - sizeof(FreeChunk);
  void* aAddress = aAdd<void*>(aFreeChunk,sizeof(FreeChunk));
  MemLib::vScribble(aAddress,nBytes,CN_FREECHUNK);
#endif
  
  m_pFreeHead = aFreeChunk;
  
  m_nFreeChunks++;
}

/*----------------------------------------------------------------------------*/
/* push free chunk onto free chain                                            */
/*----------------------------------------------------------------------------*/
void Heap::vRemoveBuffer(HeapBuffer* pHeapBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);

  vGrabWriteLock();
  
  if (pHeapBuffer == m_pFirstBuffer)
    m_pFirstBuffer = pHeapBuffer->pNextBuffer();
  
  if (pHeapBuffer == m_pLastBuffer)
    m_pLastBuffer = pHeapBuffer->pPrevBuffer();
  
  if (pHeapBuffer->pPrevBuffer())
    pHeapBuffer->pPrevBuffer()->vNextBuffer(pHeapBuffer->pNextBuffer());

  if (pHeapBuffer->pNextBuffer())
    pHeapBuffer->pNextBuffer()->vPrevBuffer(pHeapBuffer->pPrevBuffer());

  // we have one less buffer
  m_nBufferCount--;

  vDropWriteLock();
}

/*----------------------------------------------------------------------------*/
/* push free chunk onto free chain                                            */
/*----------------------------------------------------------------------------*/
void Heap::vValidFree(FreeChunk* aFreeChunk)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);

  HeapBuffer* pHeapBuffer = (HeapBuffer*)((u8)aFreeChunk & ~C_MASK_LARGE_OBJECT);
  UNUSED(pHeapBuffer);

  ASSERT(pHeapBuffer->eGetClassNo() == CN_HEAPBUFFER);
  
  u8 uSize = (u8)aFreeChunk & (u8)0xFF;
  UNUSED(uSize);

  u8 uSizeHeapBuffer = sizeof(HeapBuffer);
  UNUSED(uSizeHeapBuffer);

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
    pHeapBuffer->vVerify(m_nObjectSize);
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
