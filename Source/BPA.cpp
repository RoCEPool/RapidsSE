/*============================================================================*/
/*  BPA.cpp - buffer pool analyzer                                            */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* dump given buffer                                                          */
/*----------------------------------------------------------------------------*/
void BPA::vBufferDump(PoolBuffer* pBuffer)
{
  eCN eClass = pBuffer->eClass();
  
  // dont dump FreeBuffer's
  if (CN_FREEBUFFER == eClass)
    return;
  
  pDUMPFUN* pDumpFun = gpDump[eClass];
  
  vTrace("%s%p %-12s: "_Stz,Stz::aTab(2),pBuffer,aCN[eClass]);

  (pDumpFun)(pBuffer->eClass(), pBuffer, Stz::aTab(1));
  
//  if (pBuffer->eClass() == CN_MEDIUMHEAP ||
//      pBuffer->eClass() == SN_SMALLTRANSIENT ) {
//    vHeapDump(reinterpret_cast<Heap*>(pBuffer));
//  }
}

/*----------------------------------------------------------------------------*/
/* dump all objects in given heap                                             */
/*----------------------------------------------------------------------------*/
void BPA::vEpochDump(s48K& sPortrait)
{
  i8 iSnapshotObjects[CN_ENUMS];
  
  //vTrace("\n"_Stz);
  //vTrace("%sWords created since last epoch:\n"_Stz,Stz::aTab(1));
  
  BPA::vWordList(gpAttachMap->m_iEpoch);
  
  BPA::vObjectSnapshot(&iSnapshotObjects[0]);
  
  sPortrait.vSprintCat("Objects created since last epoch:\n"_Stz);

  for (i8 i_Object = 0 ; i_Object < CN_ENUMS; i_Object++) {
    
    eCN eClass = (eCN)i_Object;
    
    if (iSnapshotObjects[eClass] == gpAttachMap->m_nObjectsInPool[eClass])
      continue;
    
    sPortrait.vSprintCat(
      "%s  %12s (eCN %3lld) %-6lld \n"_Stz,
      Stz::aTab(1),
      aCN[eClass],
      eClass,
      iSnapshotObjects[eClass] - gpAttachMap->m_nObjectsInPool[eClass]
      );
  }

}

/*----------------------------------------------------------------------------*/
/* dump all objects in given heap                                             */
/*----------------------------------------------------------------------------*/
void BPA::vEpochTake(void)
{
  // increment the epoch
  gpAttachMap->m_iEpoch++;
  BPA::vObjectSnapshot(gpAttachMap->m_nObjectsInPool);
}

/*----------------------------------------------------------------------------*/
/* dump all objects in given heap                                             */
/*----------------------------------------------------------------------------*/
void BPA::vHeapDump(Heap* pHeap)
{
  HeapBuffer* pHeapBuffer;
  
  vTrace("\n"_Stz);
  vTrace("%sObjects in %s Heap:\n"_Stz,
         Stz::aTab(1),
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
    for (FreeChunk* aChunk = pHeapBuffer->aFirstChunk();
         aChunk;
         aChunk = pHeapBuffer->aNextChunk(aChunk,pHeap->iObjectSize())) {
      
      eCN eClass = aChunk->eClass();
      
      if (CN_FREECHUNK == eClass)
        continue;
      
      if (CN_WORD == eClass) {
        ForthWord* pWord = _pWord(aChunk);
      
        // dump only the objects
        if (WT_OBJECT != pWord->eWordType())
          continue;
      }
      
      pDUMPFUN* pDumpFun = gpDump[eClass];
      
      vTrace("%s%p %-12s: "_Stz,Stz::aTab(2),pHeap,aCN[eClass]);

      (pDumpFun)(eClass, (Object*)aChunk, Stz::aTab(2));
    }
  }
  
  pHeap->vDropLock();
}

/*----------------------------------------------------------------------------*/
/* dump all objects in given heap                                             */
/*----------------------------------------------------------------------------*/
void BPA::vHeapDump(Heap* pHeap, i8 iEpoch)
{
  HeapBuffer* pHeapBuffer;
  
  vTrace("\n"_Stz);
  vTrace("%sWords in %s Heap:\n"_Stz,
         Stz::aTab(2),
         pHeap->iObjectSize() == C_SIZE_SMALL_OBJECT ? "Small"_Stz : "Medium"_Stz);
  
  pHeap->vGrabLock();
  
  for (pHeapBuffer = pHeap->pFirstBuffer();
       pHeapBuffer;
       pHeapBuffer = pHeapBuffer->pNextBuffer()) {
    for (FreeChunk* aChunk = pHeapBuffer->aFirstChunk();
         aChunk;
         aChunk = pHeapBuffer->aNextChunk(aChunk,pHeap->iObjectSize())) {
      
      eCN eClass = aChunk->eClass();
      
      if (CN_FREECHUNK == eClass)
        continue;
      
      if (CN_WORD != eClass)
        continue;
      
      ForthWord* pWord = _pWord(aChunk);
      
      // dump only the objects
      if (WT_OBJECT != pWord->eWordType())
        continue;
      
      // dump only object created in last epoch
      if (pWord->iEpoch() != iEpoch)
        continue;
      
      pDUMPFUN* pDumpFun = gpDump[eClass];
      
      vTrace("%s%p %-12s: "_Stz,Stz::aTab(2),pHeap,aCN[eClass]);
      
      (pDumpFun)(eClass, (Object*)aChunk, Stz::aTab(2));
    }
  }
  
  pHeap->vDropLock();
}

/*----------------------------------------------------------------------------*/
/* list all objects                                                           */
/*----------------------------------------------------------------------------*/
void BPA::vObjectList(void)
{
  vTrace("\n"_Stz);
  vTrace("%sObjects in PoolBuffer Pool:\n"_Stz,Stz::aTab(1));
  
  PoolBuffer* pSmallTransient  = nullptr;
  PoolBuffer* pMediumTransient = nullptr;
  PoolBuffer* pLargeTransient  = nullptr;
  PoolBuffer* pJumboTransient  = nullptr;
  
  PoolBuffer* pSmallPersistent  = nullptr;
  PoolBuffer* pMediumPersistent = nullptr;
  PoolBuffer* pLargePersistent  = nullptr;
  PoolBuffer* pJumboPersistent  = nullptr;
  
  bn nBufferCount = gpBufferPool->m_nBuffers - 1;
  for (bn nBuffer = gpBufferPool->nBufferFirst();
       nBuffer <= nBufferCount;
       nBuffer++) {
    
    PoolBuffer* pBuffer = gpBufferPool->pToPoolBuffer(nBuffer);
    
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
}

/*----------------------------------------------------------------------------*/
/* summary report on objects in the pool and heaps                            */
/*----------------------------------------------------------------------------*/
void BPA::vObjectSummary(void)
{
  i8 iCountObjects[CN_ENUMS];
  for (i8 i_Index = 0; i_Index < CN_ENUMS; i_Index++)
    iCountObjects[i_Index] = 0;
  
  bn nBuffer = 0;
  bn nBufferCount = gpBufferPool->m_nBuffers - 1;
  
  for (bn nBuffer = gpBufferPool->nBufferFirst();
       nBuffer <= nBufferCount;
       nBuffer++) {
    
    PoolBuffer* pBuffer = gpBufferPool->pToPoolBuffer(nBuffer);
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
}

/*----------------------------------------------------------------------------*/
/* summary report on objects in the pool                                      */
/*----------------------------------------------------------------------------*/
void BPA::vObjectSnapshot(i8* pCountObjects)
{
  i8 iCountObjects[CN_ENUMS];
  
  for (i8 i_Index = 0; i_Index < CN_ENUMS; i_Index++)
    pCountObjects[i_Index] = 0;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  bn nBuffer = 0;
  bn nBufferCount = gpBufferPool->m_nBuffers - 1;
  
  for (bn nBuffer = gpBufferPool->nBufferFirst();
       nBuffer <= nBufferCount;
       nBuffer++) {
    
    PoolBuffer* pBuffer = gpBufferPool->pToPoolBuffer(nBuffer);
    
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
}

/*----------------------------------------------------------------------------*/
/* analyze the buffer pool                                                    */
/*----------------------------------------------------------------------------*/
void BPA::vPoolAnalyze(void)
{
  // increment the epoch
  gpAttachMap->m_iEpoch++;
  
  vTrace("\n"_Stz);
  vTrace("****************************************************************\n"_Stz);
  vTrace("* PoolBuffer Pool Analysis                                         *\n"_Stz);
  vTrace("****************************************************************\n"_Stz);
  
  vRegionSummary();
  vObjectSummary();
  vResourceSummary();
  
  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* dump the buffer pool objects                                               */
/*----------------------------------------------------------------------------*/
void BPA::vPoolDump(void)
{
  // increment the epoch
  gpAttachMap->m_iEpoch++;
  
  vTrace("\n"_Stz);
  vTrace("****************************************************************\n"_Stz);
  vTrace("* PoolBuffer Pool Dump                                             *\n"_Stz);
  vTrace("****************************************************************\n"_Stz);
  
  vRegionSummary();
  vObjectSummary();
  vObjectList();
  vResourceSummary();
  
  vTrace("\n"_Stz);
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
  File* pFile = new File();
  pFile->bOpenRead(sFilePath);
  
  PoolBuffer* pBuffer = new PoolBuffer(CN_HEAPBUFFER);
  
  while (true) {
    i8 nBytes = pFile->nReadBytes(pBuffer, C_SIZE_POOL_BUFFER);
    if (nBytes != C_SIZE_POOL_BUFFER)
      break;
    
    vBufferDump(pBuffer);
  }
}

/*----------------------------------------------------------------------------*/
/* report on shm region                                                       */
/*----------------------------------------------------------------------------*/
void BPA::vRegionSummary()
{
  // get magnitudes of the various buffer pools
  s32 sMagSmall;  Magnitudes::vByteMag(C_SIZE_SMALL_OBJECT, sMagSmall);
  s32 sMagMedium; Magnitudes::vByteMag(C_SIZE_MEDIUM_OBJECT,sMagMedium);
  s32 sMagLarge;  Magnitudes::vByteMag(C_SIZE_LARGE_OBJECT, sMagLarge);
  s32 sMagJumbo;  Magnitudes::vByteMag(C_SIZE_JUMBO_OBJECT, sMagJumbo);
  
  vTrace("\n"_Stz);
  vTrace("  Region %s %p size %lldB epoch %lld\n"_Stz,
         gpAttachMap->Arg.m_sRegionName.aChar(),
         gpBufferPool,
         gpAttachMap->Arg.m_iRegionSize,
         gpAttachMap->m_iEpoch);
  
  vTrace("\n"_Stz);
  
  vTrace("  BufferPool (%4s) buffers %-6lld alloc %-6lld free %-6lld \n"_Stz,
         sMagLarge.aChar(),
         gpBufferPool->m_nBuffers,
         gpBufferPool->m_nAllocCount,
         gpBufferPool->m_nFreeCount
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
}

/*----------------------------------------------------------------------------*/
/* summarize resource usage                                                   */
/*----------------------------------------------------------------------------*/
void BPA::vResourceSummary(void)
{
  vTrace("\n"_Stz);
  vTrace("****************************************************************\n"_Stz);
  vTrace(" Resources: dir %lld file %lld shm %lld socket %lld thread %lld\n"_Stz,
         gpAttachMap->m_oResourceTracker.nDirOpen(),
         gpAttachMap->m_oResourceTracker.nFileOpen(),
         gpAttachMap->m_oResourceTracker.nShmOpen(),
         gpAttachMap->m_oResourceTracker.nSocketOpen(),
         gpAttachMap->m_oResourceTracker.nThreadOpen());
  vTrace("****************************************************************\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* list all words created since last epoch                                    */
/*----------------------------------------------------------------------------*/
void BPA::vWordList(i8 iEpoch)
{
  PoolBuffer* pMediumTransient = nullptr;
  PoolBuffer* pSmallTransient  = nullptr;
  
  bn nBufferCount = gpBufferPool->m_nBuffers - 1;
  for (bn nBuffer = gpBufferPool->nBufferFirst();
       nBuffer <= nBufferCount;
       nBuffer++) {
    
    PoolBuffer* pBuffer = gpBufferPool->pToPoolBuffer(nBuffer);
    
    if (CN_MEDIUMTRANSIENT == pBuffer->eClass())
    pMediumTransient = pBuffer;
    
    if (CN_SMALLTRANSIENT == pBuffer->eClass())
    pSmallTransient = pBuffer;
  }
  
  if (pMediumTransient)
    vHeapDump(reinterpret_cast<Heap*>(pMediumTransient), iEpoch);
  
  if (pSmallTransient)
    vHeapDump(reinterpret_cast<Heap*>(pSmallTransient), iEpoch);
}

/*----------------------------------------------------------------------------*/
/* write all the buffer pool buffers to file                                  */
/*----------------------------------------------------------------------------*/
void BPA::vWriteBuffers(Stz* sFilePath)
{
  File* pFile = new File();
  pFile->bOpenWrite(sFilePath);
  
  i8 iStart  = Tsc::iTicks();
  bn nBuffer;
  
  bn nBufferCount = gpBufferPool->m_nBuffers - 1;
  for (nBuffer = gpBufferPool->nBufferFirst();
       nBuffer <= nBufferCount;
       nBuffer++) {
    
    PoolBuffer* pBuffer = gpBufferPool->pToPoolBuffer(nBuffer);
    bool bResult = pFile->bWriteBytes((void*)pBuffer,C_SIZE_POOL_BUFFER);
    ASSERT(bResult);
    
  }
  ASSERT(*nBufferCount + 1 == *nBuffer);
  
  i8 iUpdate = Tsc::iTicks();
  
  i8 iTicks = (iUpdate - iStart);
  i8 iTicksPerSecond = Tsc::iTicksPerSecond();
  
  i8 iMilli = iTicks * C_1K / iTicksPerSecond;
  
  vTrace("Writing %lld buffers took %lld ms\n"_Stz, *nBufferCount, iMilli);
}

/*----------------------------------------------------------------------------*/
/* write given heap to file                                                   */
/*----------------------------------------------------------------------------*/
void BPA::vWriteHeap(Heap* pHeap, File* pFile)
{
  Timer oTimer;
  oTimer.vStart();
  
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
  
  oTimer.vUpdate();
  vTrace("duration %lldms\n"_Stz,oTimer.iMilliseconds());
}

/*----------------------------------------------------------------------------*/
/* write all the heaps to given file                                          */
/*----------------------------------------------------------------------------*/
void BPA::vWritePersistentHeapMem(Stz* sFilePath)
{
  File* pFile = new File();
  pFile->bOpenWrite(sFilePath);
  
  vTrace("Writing persistent small heap\n"_Stz);
  vWriteHeap(gpAttachMap->m_xHeapMem.Persistent.m_pSmall,pFile);
  
  vTrace("Writing persistent medium heap\n"_Stz);
  vWriteHeap(gpAttachMap->m_xHeapMem.Persistent.m_pMedium,pFile);
  
  vTrace("Writing persistent large heap\n"_Stz);
  vWriteHeap(gpAttachMap->m_xHeapMem.Persistent.m_pLarge,pFile);

  pFile->vClose();
  delete pFile;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
