/*============================================================================*/
/*  BPA.cpp - buffer pool analyzer                                            */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

extern ResourceTracker gxResourceTracker;

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
BPA::BPA(void)
: Object(CN_BPA)
{
  _magic_();
  
  _checkInitialised_(CN_BPA);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
BPA::~BPA(void) noexcept(false)
{
  _magic_();
  
}

/*----------------------------------------------------------------------------*/
/* dump the buffer                                                            */
/*----------------------------------------------------------------------------*/
void BPA::vBufferDump(Buffer* pBuffer)
{
  _precondition_(_MAGIC_,CN_BPA,_FOOTER_);
 
  eCN eClass = pBuffer->eClass();
  
  // dont dump FreeBuffer's
  if (CN_FREEBUFFER == eClass)
    return;
  
  pDUMPFUN* pDumpFun = gpDump[eClass];
  
  vTrace("%s%p %-12s: "_Stz,aDoubleTab(),pBuffer,aCN[eClass]);

  (pDumpFun)(pBuffer->eClass(), pBuffer, aSingleTab());
  
//  if (pBuffer->eClass() == CN_MEDIUMHEAP ||
//      pBuffer->eClass() == SN_SMALLTRANSIENT ) {
//    vHeapDump(reinterpret_cast<Heap*>(pBuffer));
//  }
}

/*----------------------------------------------------------------------------*/
/* dump all objects in given heap                                             */
/*----------------------------------------------------------------------------*/
void BPA::vHeapDump(Heap* pHeap)
{
  _precondition_(_MAGIC_,CN_BPA,_FOOTER_);
  
  HeapBuffer* pHeapBuffer;
  
  vTrace("\n"_Stz);
  vTrace("%sObjects in %s Heap:\n"_Stz,
         aSingleTab(),
         pHeap->eClass() == CN_SMALLTRANSIENT ? "Small Transient"_Stz :
         pHeap->eClass() == CN_MEDIUMTRANSIENT ? "Medium Transient"_Stz :
         pHeap->eClass() == CN_LARGETRANSIENT ? "Large Transient"_Stz :
         pHeap->eClass() == CN_JUMBOTRANSIENT ? "Jumbo Transient"_Stz :
         pHeap->eClass() == CN_SMALLPERSISTENT ? "Small Persistent"_Stz :
         pHeap->eClass() == CN_MEDIUMPERSISTENT ? "Medium Persistent"_Stz :
         pHeap->eClass() == CN_LARGEPERSISTENT ? "Large Persistent"_Stz :
         pHeap->eClass() == CN_JUMBOPERSISTENT ? "Jumbo Persistent"_Stz : "unknown"_Stz);
         
  pHeap->vGrabLock();
  
  for (pHeapBuffer = pHeap->pFirstBuffer();
       pHeapBuffer;
       pHeapBuffer = pHeapBuffer->pNextBuffer()) {
    for (FreeChunk* pChunk = pHeapBuffer->pFirstChunk();
         pChunk;
         pChunk = pHeapBuffer->pNextChunk(pChunk,pHeap->iObjectSize())) {
      
      eCN eClass = pChunk->eClass();
      
      if (CN_FREECHUNK == eClass)
        continue;
      
      if (CN_WORD == eClass) {
        Word* pWord = _pWord(pChunk);
      
        // dump only the objects
        if (WT_OBJECT != pWord->eWordType())
          continue;
      }
      
      pDUMPFUN* pDumpFun = gpDump[eClass];
      
      vTrace("%s%p %-12s: "_Stz,aDoubleTab(),pHeap,aCN[eClass]);

      (pDumpFun)(eClass, pChunk, aDoubleTab());
    }
  }
  
  pHeap->vDropLock();
}

/*----------------------------------------------------------------------------*/
/* dump all objects in given heap                                             */
/*----------------------------------------------------------------------------*/
void BPA::vHeapDump(Heap* pHeap, i8 iEpoch)
{
  _precondition_(_MAGIC_,CN_BPA,_FOOTER_);
  
  HeapBuffer* pHeapBuffer;
  
  vTrace("\n"_Stz);
  vTrace("%sWords in %s Heap:\n"_Stz,
         aDoubleTab(),
         pHeap->iObjectSize() == C_SIZE_SMALL_OBJECT ? "Small"_Stz : "Medium"_Stz);
  
  pHeap->vGrabLock();
  
  for (pHeapBuffer = pHeap->pFirstBuffer();
       pHeapBuffer;
       pHeapBuffer = pHeapBuffer->pNextBuffer()) {
    for (FreeChunk* pChunk = pHeapBuffer->pFirstChunk();
         pChunk;
         pChunk = pHeapBuffer->pNextChunk(pChunk,pHeap->iObjectSize())) {
      
      eCN eClass = pChunk->eClass();
      
      if (CN_FREECHUNK == eClass)
        continue;
      
      if (CN_WORD != eClass)
        continue;
      
      Word* pWord = _pWord(pChunk);
      
      // dump only the objects
      if (WT_OBJECT != pWord->eWordType())
        continue;
      
      // dump only object created in last epoch
      if (pWord->iEpoch() != iEpoch)
        continue;
      
      pDUMPFUN* pDumpFun = gpDump[eClass];
      
      vTrace("%s%p %-12s: "_Stz,aDoubleTab(),pHeap,aCN[eClass]);
      
      (pDumpFun)(eClass, pChunk, aDoubleTab());
    }
  }
  
  pHeap->vDropLock();
}

/*----------------------------------------------------------------------------*/
/* list all objects                                                           */
/*----------------------------------------------------------------------------*/
void BPA::vObjectList(void)
{
  _precondition_(_MAGIC_,CN_BPA,_FOOTER_);
  
  vTrace("\n"_Stz);
  vTrace("%sObjects in Buffer Pool:\n"_Stz,aSingleTab());
  
  Buffer* pSmallTransient  = nullptr;
  Buffer* pMediumTransient = nullptr;
  Buffer* pLargeTransient  = nullptr;
  Buffer* pJumboTransient  = nullptr;
  
  Buffer* pSmallPersistent  = nullptr;
  Buffer* pMediumPersistent = nullptr;
  Buffer* pLargePersistent  = nullptr;
  Buffer* pJumboPersistent  = nullptr;
  
  bn nBufferCount = gpBufferPool->m_nBuffers - 1;
  for (bn nBuffer = gpBufferPool->nBufferFirst();
       nBuffer <= nBufferCount;
       nBuffer++) {
    
    Buffer* pBuffer = gpBufferPool->pToBuffer(nBuffer);
    
    if (CN_SMALLTRANSIENT == pBuffer->eClass())
      pSmallTransient = pBuffer;
    
    if (CN_MEDIUMTRANSIENT == pBuffer->eClass())
      pMediumTransient = pBuffer;
    
    if (CN_LARGETRANSIENT == pBuffer->eClass())
      pLargeTransient = pBuffer;
    
    if (CN_JUMBOTRANSIENT == pBuffer->eClass())
      pJumboTransient = pBuffer;
    
    if (CN_SMALLPERSISTENT == pBuffer->eClass())
      pSmallPersistent = pBuffer;
    
    if (CN_MEDIUMPERSISTENT  == pBuffer->eClass())
      pMediumPersistent = pBuffer;
    
    if (CN_LARGEPERSISTENT  == pBuffer->eClass())
      pLargePersistent = pBuffer;
    
    if (CN_JUMBOPERSISTENT  == pBuffer->eClass())
      pJumboPersistent = pBuffer;
    
    vBufferDump(pBuffer);
  }
  
  if (pSmallTransient)
    vHeapDump(reinterpret_cast<Heap*>(pSmallTransient));
  
  if (pMediumTransient)
    vHeapDump(reinterpret_cast<Heap*>(pMediumTransient));
  
  if (pLargeTransient)
    vHeapDump(reinterpret_cast<Heap*>(pLargeTransient));
  
  if (pJumboTransient)
    vHeapDump(reinterpret_cast<Heap*>(pJumboTransient));
  
  if (pSmallPersistent)
    vHeapDump(reinterpret_cast<Heap*>(pSmallPersistent));
  
  if (pMediumTransient)
    vHeapDump(reinterpret_cast<Heap*>(pMediumPersistent));
  
  if (pLargePersistent)
    vHeapDump(reinterpret_cast<Heap*>(pLargePersistent));
  
  if (pJumboPersistent)
    vHeapDump(reinterpret_cast<Heap*>(pJumboPersistent));
  
  _postcondition_(_MAGIC_,CN_BPA,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* summary report on objects in the pool and heaps                            */
/*----------------------------------------------------------------------------*/
void BPA::vObjectSummary(void)
{
  _precondition_(_MAGIC_,CN_BPA,_FOOTER_);
  
  i8 iCountObjects[CN_ENUMS];
  for (i8 i_Index = 0; i_Index < CN_ENUMS; i_Index++)
    iCountObjects[i_Index] = 0;
  
  bn nBuffer = 0;
  bn nBufferCount = gpBufferPool->m_nBuffers - 1;
  
  for (bn nBuffer = gpBufferPool->nBufferFirst();
       nBuffer <= nBufferCount;
       nBuffer++) {
    
    Buffer* pBuffer = gpBufferPool->pToBuffer(nBuffer);
    iCountObjects[pBuffer->eClass()]++;
  }
  
  vTrace("\n"_Stz);
  vTrace("  BufferPool Object Summary:\n"_Stz);
  for (i8 i_Index = 0; i_Index < CN_ENUMS; i_Index++) {
    eCN eClass = (eCN)i_Index;
    
    if (iCountObjects[eClass] == 0)
      continue;
    
    vTrace("  %12s %-6lld \n"_Stz, aCN[eClass], iCountObjects[eClass]);
  }
  
  gpAttachMap->m_xHeapMem.Transient.m_pSmall->vAnalyse(iCountObjects);
  
  vTrace("\n"_Stz);
  vTrace("  Transient SmallHeap Object Summary:\n"_Stz);
  for (i8 i_Index = 0; i_Index < CN_ENUMS; i_Index++) {
    eCN eClass = (eCN)i_Index;
    
    if (iCountObjects[eClass] == 0)
      continue;
    
    vTrace("  %12s %-6lld \n"_Stz, aCN[eClass], iCountObjects[eClass]);
  }
  
  gpAttachMap->m_xHeapMem.Transient.m_pMedium->vAnalyse(iCountObjects);
  
  vTrace("\n"_Stz);
  vTrace("  Transient MediumHeap Object Summary:\n"_Stz);
  for (i8 i_Index = 0; i_Index < CN_ENUMS; i_Index++) {
    eCN eClass = (eCN)i_Index;
    
    if (iCountObjects[eClass] == 0)
      continue;
    
    vTrace("  %12s %-6lld \n"_Stz, aCN[eClass], iCountObjects[eClass]);
  }
  
  gpAttachMap->m_xHeapMem.Transient.m_pLarge->vAnalyse(iCountObjects);
  
  vTrace("\n"_Stz);
  vTrace("  Transient LargeHeap Object Summary:\n"_Stz);
  for (i8 i_Index = 0; i_Index < CN_ENUMS; i_Index++) {
    eCN eClass = (eCN)i_Index;
    
    if (iCountObjects[eClass] == 0)
      continue;
    
    vTrace("  %12s %-6lld \n"_Stz, aCN[eClass], iCountObjects[eClass]);
  }
  
  
//  for (i8 i_Index = 0; i_Index < CN_ENUMS; i_Index++)
//    iCountObjects[i_Index] = 0;
//
//  gpAttachMap->m_xHeapMem.Transient.m_pJumbo->vAnalyse(iCountObjects);
//
//  vTrace("\n"_Stz);
//  vTrace("  Transient JumboHeap Object s:\n"_Stz);
//  for (i8 i_Index = 0; i_Index < CN_ENUMS; i_Index++) {
//    eCN eClass = (eCN)i_Index;
//
//    if (iCountObjects[eClass] == 0)
//      continue;
//
//    vTrace("  %12s %-6lld \n"_Stz, aCN[eClass], iCountObjects[eClass]);
//  }
  
  gpAttachMap->m_xHeapMem.Persistent.m_pSmall->vAnalyse(iCountObjects);
  
  vTrace("\n"_Stz);
  vTrace("  Persistent SmallHeap Object Summary:\n"_Stz);
  for (i8 i_Index = 0; i_Index < CN_ENUMS; i_Index++) {
    eCN eClass = (eCN)i_Index;
    
    if (iCountObjects[eClass] == 0)
      continue;
    
    vTrace("  %12s %-6lld \n"_Stz, aCN[eClass], iCountObjects[eClass]);
  }
  
  gpAttachMap->m_xHeapMem.Persistent.m_pMedium->vAnalyse(iCountObjects);
  
  vTrace("\n"_Stz);
  vTrace("  Persistent MediumHeap Object Summary:\n"_Stz);
  for (i8 i_Index = 0; i_Index < CN_ENUMS; i_Index++) {
    eCN eClass = (eCN)i_Index;
    
    if (iCountObjects[eClass] == 0)
      continue;
    
    vTrace("  %12s %-6lld \n"_Stz, aCN[eClass], iCountObjects[eClass]);
  }
  
  gpAttachMap->m_xHeapMem.Persistent.m_pLarge->vAnalyse(iCountObjects);
  
  vTrace("\n"_Stz);
  vTrace("  Persistent LargeHeap Object Summary:\n"_Stz);
  for (i8 i_Index = 0; i_Index < CN_ENUMS; i_Index++) {
    eCN eClass = (eCN)i_Index;
    
    if (iCountObjects[eClass] == 0)
      continue;
    
    vTrace("  %12s %-6lld \n"_Stz, aCN[eClass], iCountObjects[eClass]);
  }
  
  _postcondition_(_MAGIC_,CN_BPA,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* summary report on objects in the pool                                      */
/*----------------------------------------------------------------------------*/
void BPA::vObjectSnapshot(i8* pCountObjects)
{
  _precondition_(_MAGIC_,CN_BPA,_FOOTER_);
  
  i8 iCountObjects[CN_ENUMS];
  
  for (i8 i_Index = 0; i_Index < CN_ENUMS; i_Index++)
    pCountObjects[i_Index] = 0;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  bn nBuffer = 0;
  bn nBufferCount = gpBufferPool->m_nBuffers - 1;
  
  for (bn nBuffer = gpBufferPool->nBufferFirst();
       nBuffer <= nBufferCount;
       nBuffer++) {
    
    Buffer* pBuffer = gpBufferPool->pToBuffer(nBuffer);
    
    eCN eClass = pBuffer->eClass();
    
    if (CN_QUEUES == eClass)
      int iDebug = 1;
    
    pCountObjects[eClass]++;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  gpAttachMap->m_xHeapMem.Transient.m_pSmall->vAnalyse(iCountObjects);
  
  for (i8 i_Index = 0; i_Index < CN_ENUMS; i_Index++) {
    eCN eClass = (eCN)i_Index;
    
    if (iCountObjects[eClass] == 0)
      continue;
    
    if (CN_QUEUES == eClass)
      int iDebug = 1;
    
    pCountObjects[eClass] += iCountObjects[eClass];
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  gpAttachMap->m_xHeapMem.Transient.m_pMedium->vAnalyse(iCountObjects);
  
  for (i8 i_Index = 0; i_Index < CN_ENUMS; i_Index++) {
    eCN eClass = (eCN)i_Index;
    
    if (iCountObjects[eClass] == 0)
      continue;
    
    if (CN_QUEUES == eClass)
      int iDebug = 1;

    pCountObjects[eClass] += iCountObjects[eClass];
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  gpAttachMap->m_xHeapMem.Transient.m_pLarge->vAnalyse(iCountObjects);
  
  for (i8 i_Index = 0; i_Index < CN_ENUMS; i_Index++) {
    eCN eClass = (eCN)i_Index;
    
    if (iCountObjects[eClass] == 0)
      continue;
    
    if (CN_QUEUES == eClass)
      int iDebug = 1;

    pCountObjects[eClass] += iCountObjects[eClass];
  }
  
  _postcondition_(_MAGIC_,CN_BPA,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* analyze the buffer pool                                                    */
/*----------------------------------------------------------------------------*/
void BPA::vPoolAnalyze(void)
{
  _precondition_(_MAGIC_,CN_BPA,_FOOTER_);
  
  // increment the epoch
  gpAttachMap->m_iEpoch++;
  
  vTrace("\n"_Stz);
  vTrace("****************************************************************\n"_Stz);
  vTrace("* Buffer Pool Analysis                                         *\n"_Stz);
  vTrace("****************************************************************\n"_Stz);
  
  vRegionSummary();
  vObjectSummary();
  vResourceSummary();
  
  vTrace("\n"_Stz);
  
  _postcondition_(_MAGIC_,CN_BPA,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* dump the buffer pool objects                                               */
/*----------------------------------------------------------------------------*/
void BPA::vPoolDump(void)
{
  _precondition_(_MAGIC_,CN_BPA,_FOOTER_);
  
  // increment the epoch
  gpAttachMap->m_iEpoch++;
  
  vTrace("\n"_Stz);
  vTrace("****************************************************************\n"_Stz);
  vTrace("* Buffer Pool Dump                                             *\n"_Stz);
  vTrace("****************************************************************\n"_Stz);
  
  vRegionSummary();
  vObjectSummary();
  vObjectList();
  vResourceSummary();
  
  vTrace("\n"_Stz);
  
  _postcondition_(_MAGIC_,CN_BPA,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* get free space in the buffer pool                                          */
/*----------------------------------------------------------------------------*/
i8 BPA::iPoolFree(void)
{
  i8 iTransientSmall =
     gpAttachMap->m_xHeapMem.Transient.m_pSmall->iFreeCount() * C_SIZE_SMALL_OBJECT;
  
  i8 iTransientMedium =
     gpAttachMap->m_xHeapMem.Transient.m_pMedium->iFreeCount() * C_SIZE_MEDIUM_OBJECT;
  
  i8 iTransientLarge =
     gpAttachMap->m_xHeapMem.Transient.m_pLarge->iFreeCount() * C_SIZE_LARGE_OBJECT;
  
  
  i8 iPersistentSmall =
     gpAttachMap->m_xHeapMem.Persistent.m_pSmall->iFreeCount() * C_SIZE_SMALL_OBJECT;
  
  i8 iPersistentMedium =
     gpAttachMap->m_xHeapMem.Persistent.m_pMedium->iFreeCount() * C_SIZE_MEDIUM_OBJECT;
  
  i8 iPersistentLarge =
     gpAttachMap->m_xHeapMem.Persistent.m_pLarge->iFreeCount() * C_SIZE_LARGE_OBJECT;

  
  i8 iTotalFree =
       iTransientSmall  + iTransientMedium  + iTransientLarge +
       iPersistentSmall + iPersistentMedium + iPersistentLarge;
  
  return iTotalFree;
}

/*----------------------------------------------------------------------------*/
/* read all the buffer pool buffers from file                                 */
/*----------------------------------------------------------------------------*/
void BPA::vReadBuffers(Stz* sFilePath)
{
  _precondition_(_MAGIC_,CN_BPA,_FOOTER_);
  
  File* pFile = new File();
  pFile->bOpen(sFilePath, C_OPEN_READ);
  
  Buffer* pBuffer = new Buffer(CN_HEAPBUFFER);
  
  while (true) {
    i8 iBytes = pFile->iReadBytes(pBuffer, C_SIZE_POOL_BUFFER);
    if (iBytes != C_SIZE_POOL_BUFFER)
      break;
    
    vBufferDump(pBuffer);
  }
  
  _postcondition_(_MAGIC_,CN_BPA,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* report on shm region                                                       */
/*----------------------------------------------------------------------------*/
void BPA::vRegionSummary()
{
  _precondition_(_MAGIC_,CN_BPA,_FOOTER_);
  
  // get magnitudes of the various buffer pools
  s32 sMagSmall;  Magnitudes::vByteMag(C_SIZE_SMALL_OBJECT, sMagSmall);
  s32 sMagMedium; Magnitudes::vByteMag(C_SIZE_MEDIUM_OBJECT,sMagMedium);
  s32 sMagLarge;  Magnitudes::vByteMag(C_SIZE_LARGE_OBJECT, sMagLarge);
  s32 sMagJumbo;  Magnitudes::vByteMag(C_SIZE_JUMBO_OBJECT, sMagJumbo);
  
  vTrace("\n"_Stz);
  vTrace("  Region %s %p size %lldB epoch %lld\n"_Stz,
         gpAttachMap->m_sRegionName.aChar(),
         gpBufferPool,
         gpAttachMap->m_iRegionSize,
         gpAttachMap->m_iEpoch);
  
  vTrace("\n"_Stz);
  
  vTrace("  BufferPool (%4s) buffers %-6lld alloc %-6lld free %-6lld \n"_Stz,
         sMagLarge.aChar(),
         gpBufferPool->m_nBuffers,
         gpBufferPool->m_iAllocCount,
         gpBufferPool->m_iFreeCount
         );
  
  vTrace("   Transient SmallHeap (%4s) buffers %-6lld alloc %-6lld free %-6lld\n"_Stz,
         sMagSmall.aChar(),
         gpAttachMap->m_xHeapMem.Transient.m_pSmall->iBufferCount(),
         gpAttachMap->m_xHeapMem.Transient.m_pSmall->iAllocCount(),
         gpAttachMap->m_xHeapMem.Transient.m_pSmall->iFreeCount());
  
  vTrace("  Transient MediumHeap (%4s) buffers %-6lld alloc %-6lld free %-6lld\n"_Stz,
         sMagMedium.aChar(),
         gpAttachMap->m_xHeapMem.Transient.m_pMedium->iBufferCount(),
         gpAttachMap->m_xHeapMem.Transient.m_pMedium->iAllocCount(),
         gpAttachMap->m_xHeapMem.Transient.m_pMedium->iFreeCount());
  
  vTrace("   Transient LargeHeap (%4s) buffers %-6lld alloc %-6lld free %-6lld\n"_Stz,
         sMagLarge.aChar(),
         gpAttachMap->m_xHeapMem.Transient.m_pLarge->iBufferCount(),
         gpAttachMap->m_xHeapMem.Transient.m_pLarge->iAllocCount(),
         gpAttachMap->m_xHeapMem.Transient.m_pLarge->iFreeCount());
  
  vTrace("   Transient JumboHeap (%4s) buffers %-6lld alloc %-6lld free %-6lld\n"_Stz,
         sMagJumbo.aChar(),
         gpAttachMap->m_xHeapMem.Transient.m_pJumbo->iBufferCount(),
         gpAttachMap->m_xHeapMem.Transient.m_pJumbo->iAllocCount(),
         gpAttachMap->m_xHeapMem.Transient.m_pJumbo->iFreeCount());
  
  vTrace("   Persistent SmallHeap (%4s) buffers %-6lld alloc %-6lld free %-6lld\n"_Stz,
         sMagSmall.aChar(),
         gpAttachMap->m_xHeapMem.Persistent.m_pSmall->iBufferCount(),
         gpAttachMap->m_xHeapMem.Persistent.m_pSmall->iAllocCount(),
         gpAttachMap->m_xHeapMem.Persistent.m_pSmall->iFreeCount());
  
  vTrace("  Persistent MediumHeap (%4s) buffers %-6lld alloc %-6lld free %-6lld\n"_Stz,
         sMagMedium.aChar(),
         gpAttachMap->m_xHeapMem.Persistent.m_pMedium->iBufferCount(),
         gpAttachMap->m_xHeapMem.Persistent.m_pMedium->iAllocCount(),
         gpAttachMap->m_xHeapMem.Persistent.m_pMedium->iFreeCount());
  
  vTrace("   Persistent LargeHeap (%4s) buffers %-6lld alloc %-6lld free %-6lld\n"_Stz,
         sMagJumbo.aChar(),
         gpAttachMap->m_xHeapMem.Persistent.m_pLarge->iBufferCount(),
         gpAttachMap->m_xHeapMem.Persistent.m_pLarge->iAllocCount(),
         gpAttachMap->m_xHeapMem.Persistent.m_pLarge->iFreeCount());
  
  vTrace("   Persistent JumboHeap (%4s) buffers %-6lld alloc %-6lld free %-6lld\n"_Stz,
         sMagJumbo.aChar(),
         gpAttachMap->m_xHeapMem.Persistent.m_pJumbo->iBufferCount(),
         gpAttachMap->m_xHeapMem.Persistent.m_pJumbo->iAllocCount(),
         gpAttachMap->m_xHeapMem.Persistent.m_pJumbo->iFreeCount());
  
  _postcondition_(_MAGIC_,CN_BPA,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* summarize resource usage                                                   */
/*----------------------------------------------------------------------------*/
void BPA::vResourceSummary(void)
{
  _precondition_(_MAGIC_,CN_BPA,_FOOTER_);
  
  vTrace("\n"_Stz);
  vTrace("****************************************************************\n"_Stz);
  vTrace(" Resources: dir %lld file %lld shm %lld socket %lld thread %lld\n"_Stz,
         gxResourceTracker.nDirOpen(),
         gxResourceTracker.nFileOpen(),
         gxResourceTracker.nShmOpen(),
         gxResourceTracker.nSocketOpen(),
         gxResourceTracker.nThreadOpen());
  vTrace("****************************************************************\n"_Stz);
  
  _postcondition_(_MAGIC_,CN_BPA,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* list all words created since last epoch                                    */
/*----------------------------------------------------------------------------*/
void BPA::vWordList(i8 iEpoch)
{
  _precondition_(_MAGIC_,CN_BPA,_FOOTER_);
  
  Buffer* pMediumTransient = nullptr;
  Buffer* pSmallTransient  = nullptr;
  
  bn nBufferCount = gpBufferPool->m_nBuffers - 1;
  for (bn nBuffer = gpBufferPool->nBufferFirst();
       nBuffer <= nBufferCount;
       nBuffer++) {
    
    Buffer* pBuffer = gpBufferPool->pToBuffer(nBuffer);
    
    if (CN_MEDIUMTRANSIENT == pBuffer->eClass())
    pMediumTransient = pBuffer;
    
    if (CN_SMALLTRANSIENT == pBuffer->eClass())
    pSmallTransient = pBuffer;
  }
  
  if (pMediumTransient)
    vHeapDump(reinterpret_cast<Heap*>(pMediumTransient), iEpoch);
  
  if (pSmallTransient)
    vHeapDump(reinterpret_cast<Heap*>(pSmallTransient), iEpoch);
  
  _postcondition_(_MAGIC_,CN_BPA,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* write all the buffer pool buffers to file                                  */
/*----------------------------------------------------------------------------*/
void BPA::vWriteBuffers(Stz* sFilePath)
{
  _precondition_(_MAGIC_,CN_BPA,_FOOTER_);
  
  File* pFile = new File();
  pFile->bOpen(sFilePath, C_OPEN_WRITE);
  
  i8 iStart  = Synch::iTicks();
  bn nBuffer;
  
  bn nBufferCount = gpBufferPool->m_nBuffers - 1;
  for (nBuffer = gpBufferPool->nBufferFirst();
       nBuffer <= nBufferCount;
       nBuffer++) {
    
    Buffer* pBuffer = gpBufferPool->pToBuffer(nBuffer);
    bool bResult = pFile->bWriteBytes((void*)pBuffer,C_SIZE_POOL_BUFFER);
    ASSERT(bResult);
    
  }
  ASSERT(*nBufferCount + 1 == *nBuffer);
  
  i8 iUpdate = Synch::iTicks();
  
  i8 iTicks = (iUpdate - iStart);
  i8 iTicksPerSecond = Synch::iTickFrequency();
  
  i8 iMilli = iTicks * C_1K / iTicksPerSecond;
  
  vTrace("Writing %lld buffers took %lld ms\n"_Stz, *nBufferCount, iMilli);
         
  _postcondition_(_MAGIC_,CN_BPA,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* write given heap to file                                                   */
/*----------------------------------------------------------------------------*/
void BPA::vWriteHeap(Heap* pHeap, File* pFile)
{
  _precondition_(_MAGIC_,CN_BPA,_FOOTER_);
  
  Timer xTimer;
  xTimer.vStart();
  
  pHeap->vGrabLock();
  
  bool bResult = pFile->bWriteBytes(pHeap,C_SIZE_POOL_BUFFER);
  ASSERT(bResult);
  
  for (HeapBuffer* pHeapBuffer = pHeap->pFirstBuffer();
       pHeapBuffer;
       pHeapBuffer = pHeap->pNextBuffer(pHeapBuffer)) {
    
    bResult = pFile->bWriteBytes(pHeapBuffer,C_SIZE_POOL_BUFFER);
    ASSERT(bResult);
  }
  
  pHeap->vDropLock();
  
  xTimer.vUpdate();
  vTrace("duration %lldms\n"_Stz,xTimer.iMilliseconds());
}

/*----------------------------------------------------------------------------*/
/* write all the heaps to given file                                          */
/*----------------------------------------------------------------------------*/
void BPA::vWritePersistentHeapMem(Stz* sFilePath)
{
  _precondition_(_MAGIC_,CN_BPA,_FOOTER_);
  
  File* pFile = new File();
  pFile->bOpen(sFilePath, C_OPEN_WRITE);
  
  vTrace("Writing persistent small heap\n"_Stz);
  vWriteHeap(gpAttachMap->m_xHeapMem.Persistent.m_pSmall,pFile);
  
  vTrace("Writing persistent medium heap\n"_Stz);
  vWriteHeap(gpAttachMap->m_xHeapMem.Persistent.m_pMedium,pFile);
  
  vTrace("Writing persistent large heap\n"_Stz);
  vWriteHeap(gpAttachMap->m_xHeapMem.Persistent.m_pLarge,pFile);

  pFile->vClose();
  delete pFile;
  
  _postcondition_(_MAGIC_,CN_BPA,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* BPA::aFooter(void)
{
  _precondition_(_MAGIC_,CN_BPA);
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
