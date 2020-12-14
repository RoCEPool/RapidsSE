/*============================================================================*/
/* PoolHelper.cpp - buffer pool analyzer                                      */
/*============================================================================*/

#include "Utilities.h"

#include "ResourceTracker.hpp" //?
#include "File.hpp" //?

extern SizeHeap  gxaSizeHeap[];

/*----------------------------------------------------------------------------*/
/* dump given buffer                                                          */
/*----------------------------------------------------------------------------*/
void PoolHelper::vBufferDump(PoolBuffer* pBuffer)
{
  eCN eClass = pBuffer->eClass();
  
  // dont dump FreeBuffer's
  if (CN_FREEBUFFER == eClass)
    return;
  
  //pDUMPFUN* pDumpFun = gpDump[eClass];
  
  vTrace("%s%p %-12s: "_Stz,Stz::szTab(2),pBuffer,aCN[eClass]);

  //(pDumpFun)(pBuffer->eClass(), pBuffer, Stz::szTab(1));
  
//  if (pBuffer->eClass() == CN_MEDIUMHEAP ||
//      pBuffer->eClass() == SN_SMALLTRANSIENT ) {
//    vHeapDump(reinterpret_cast<Heap*>(pBuffer));
//  }
}


/*----------------------------------------------------------------------------*/
/* dump all objects in given heap                                             */
/*----------------------------------------------------------------------------*/
void PoolHelper::vHeapDump(Heap* pHeap)
{
  HeapBuffer* pHeapBuffer;
  
  vTrace("\n"_Stz);
  vTrace("%sObjects in %s Heap:\n"_Stz,
         Stz::szTab(1),
         
           pHeap->eClass() == CN_TINYTRANSIENT    ? "Tiny Transient"_Stz
         : pHeap->eClass() == CN_SMALLTRANSIENT   ? "Small Transient"_Stz
         : pHeap->eClass() == CN_MEDIUMTRANSIENT  ? "Medium Transient"_Stz
         : pHeap->eClass() == CN_LARGETRANSIENT   ? "Large Transient"_Stz
         : pHeap->eClass() == CN_TINYPERSISTENT   ? "Tiny Persistent"_Stz
         : pHeap->eClass() == CN_SMALLPERSISTENT  ? "Small Persistent"_Stz
         : pHeap->eClass() == CN_MEDIUMPERSISTENT ? "Medium Persistent"_Stz
         : pHeap->eClass() == CN_LARGEPERSISTENT  ? "Large Persistent"_Stz
         : "unknown"_Stz
         );
         
  pHeap->vGrabLock();
  
  for (pHeapBuffer = pHeap->pFirstBuffer();
       pHeapBuffer;
       pHeapBuffer = pHeapBuffer->pNextBuffer()) {
    for (FreeChunk* aChunk = pHeapBuffer->aFirstChunk();
         aChunk;
         aChunk = pHeapBuffer->aNextChunk(aChunk,pHeap->nObjectSize())) {
      
      eCN eClass = aChunk->eClass();
      
      if (CN_FREECHUNK == eClass)
        continue;
      
      if (CN_WORD == eClass) {
        Word* pWord = _pWord(aChunk);
      
        continue;
      }
      
      //pDUMPFUN* pDumpFun = gpDump[eClass];
      
      vTrace("%s%p %-12s: "_Stz,Stz::szTab(2),pHeap,aCN[eClass]);

      //(pDumpFun)(eClass, (Object*)aChunk, Stz::szTab(2));
    }
  }
  
  pHeap->vDropLock();
}

/*----------------------------------------------------------------------------*/
/* dump all objects in given heap                                             */
/*----------------------------------------------------------------------------*/
void PoolHelper::vHeapDump(Heap* pHeap, i8 iEpoch)
{
  HeapBuffer* pHeapBuffer;
  
  vTrace("\n"_Stz);
  vTrace("%sWords in %s Heap:\n"_Stz,
         Stz::szTab(2),
         pHeap->nObjectSize() == C_SIZE_SMALL_OBJECT ? "Small"_Stz : "Medium"_Stz);
  
  pHeap->vGrabLock();
  
  for (pHeapBuffer = pHeap->pFirstBuffer();
       pHeapBuffer;
       pHeapBuffer = pHeapBuffer->pNextBuffer()) {
    for (FreeChunk* aChunk = pHeapBuffer->aFirstChunk();
         aChunk;
         aChunk = pHeapBuffer->aNextChunk(aChunk,pHeap->nObjectSize())) {
      
      eCN eClass = aChunk->eClass();
      
      if (CN_FREECHUNK == eClass)
        continue;
      
      if (CN_WORD != eClass)
        continue;
      
      Word* pWord = _pWord(aChunk);
      
      // dump only the objects
      continue;
      
      // dump only object created in last epoch
      if (pWord->iEpoch() != iEpoch)
        continue;
      
      //pDUMPFUN* pDumpFun = gpDump[eClass];
      
      vTrace("%s%p %-12s: "_Stz,Stz::szTab(2),pHeap,aCN[eClass]);
      
      //(pDumpFun)(eClass, (Object*)aChunk, Stz::szTab(2));
    }
  }
  
  pHeap->vDropLock();
}

/*----------------------------------------------------------------------------*/
/* list all objects                                                           */
/*----------------------------------------------------------------------------*/
void PoolHelper::vObjectList(void)
{
  vTrace("\n"_Stz);
  vTrace("%sObjects in PoolBuffer Pool:\n"_Stz,Stz::szTab(1));
  
  PoolBuffer* pTinyTransient   = nullptr;
  PoolBuffer* pSmallTransient  = nullptr;
  PoolBuffer* pMediumTransient = nullptr;
  PoolBuffer* pLargeTransient  = nullptr;
  
  PoolBuffer* pTinyPersistent   = nullptr;
  PoolBuffer* pSmallPersistent  = nullptr;
  PoolBuffer* pMediumPersistent = nullptr;
  PoolBuffer* pLargePersistent  = nullptr;
  
  for (i8 i_Buffer = gpBufferPool->iBufferFirst();
       i_Buffer <= gpBufferPool->iBufferLast();
       i_Buffer++) {
    
    PoolBuffer* pBuffer = gpBufferPool->pToPoolBuffer(i_Buffer);
    
    if (CN_TINYTRANSIENT == pBuffer->eClass())
      pTinyTransient = pBuffer;
    
    if (CN_SMALLTRANSIENT == pBuffer->eClass())
      pSmallTransient = pBuffer;
    
    if (CN_MEDIUMTRANSIENT == pBuffer->eClass())
      pMediumTransient = pBuffer;
    
    if (CN_LARGETRANSIENT == pBuffer->eClass())
      pLargeTransient = pBuffer;
    
    
    if (CN_TINYPERSISTENT == pBuffer->eClass())
      pTinyPersistent = pBuffer;
    
    if (CN_SMALLPERSISTENT == pBuffer->eClass())
      pSmallPersistent = pBuffer;
    
    if (CN_MEDIUMPERSISTENT  == pBuffer->eClass())
      pMediumPersistent = pBuffer;
    
    if (CN_LARGEPERSISTENT  == pBuffer->eClass())
      pLargePersistent = pBuffer;
        
    vBufferDump(pBuffer);
  }
  
  if (pTinyTransient)
    vHeapDump(reinterpret_cast<Heap*>(pTinyTransient));
  
  if (pSmallTransient)
    vHeapDump(reinterpret_cast<Heap*>(pSmallTransient));
  
  if (pMediumTransient)
    vHeapDump(reinterpret_cast<Heap*>(pMediumTransient));
  
  if (pLargeTransient)
    vHeapDump(reinterpret_cast<Heap*>(pLargeTransient));
  
  
  
  if (pTinyPersistent)
    vHeapDump(reinterpret_cast<Heap*>(pTinyPersistent));
  
  if (pSmallPersistent)
    vHeapDump(reinterpret_cast<Heap*>(pSmallPersistent));
  
  if (pMediumTransient)
    vHeapDump(reinterpret_cast<Heap*>(pMediumPersistent));
  
  if (pLargePersistent)
    vHeapDump(reinterpret_cast<Heap*>(pLargePersistent));
  
}

/*----------------------------------------------------------------------------*/
/* summary report on objects in the pool and heaps                            */
/*----------------------------------------------------------------------------*/
void PoolHelper::vObjectSummary(void)
{
  s32 sFlag;
  s32 sMagObj;
  s32 sMagHeap;
  s32 sMagBlocks;
  i8  nObjInHeap;
  
  i8 iCountObjects[CN_ENUMS];
  for (i8 i_Index = 0; i_Index < CN_ENUMS; i_Index++)
    iCountObjects[i_Index] = 0;
  

  for (i8 i_Buffer = gpBufferPool->iBufferFirst();
       i_Buffer <= gpBufferPool->iBufferLast();
       i_Buffer++) {
    
    PoolBuffer* pBuffer = gpBufferPool->pToPoolBuffer(i_Buffer);
    iCountObjects[pBuffer->eClass()]++;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  printf("\n");
  printf("  BufferPool Object Summary:\n");
  for (i8 i_Index = 0; i_Index < CN_ENUMS; i_Index++) {
    eCN eClass = (eCN)i_Index;
    
    if (iCountObjects[eClass] == 0)
      continue;
    
    printf((char*)"  %20s %-6lld \n"_Stz, aCN[eClass], iCountObjects[eClass]);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  i8 nBlockSize =0;
  
  nBlockSize = C_SIZE_TINY_OBJECT;
  MagLib::vByteMag(nBlockSize,sMagHeap);
  AM::pHeapMem()->Transient.m_pTiny->vAnalyse(iCountObjects);
  nObjInHeap = 0;
  
  printf("\n");
  printf((char*)"  Transient Tiny Heap %s Object Summary:\n"_Stz,sMagHeap.szStz());
  for (i8 i_Index = 0; i_Index < CN_ENUMS; i_Index++) {
    eCN eClass = (eCN)i_Index;
    
    i8 nCntObj = iCountObjects[eClass];
    if (! nCntObj)
      continue;
    
    nObjInHeap    += nCntObj;
    i8 nObjSize    = gxaSizeHeap[eClass].m_nClassSize;
    i8 nSumObjSize = nCntObj * nObjSize;
    i8 iPctUse     = (nObjSize * 100) / nBlockSize;
    
    MagLib::vByteMag(nSumObjSize,sMagObj);
    
    if (nObjSize < C_SIZE_TINY_OBJECT / 2)
      sFlag = " *half again!"_Stz;
    else
      sFlag = ""_Stz;

    printf((char*)"  %20s %-6lld (size %7lldB sum %6s use %2lld%%) %s\n"_Stz,
           aCN[eClass],nCntObj,nObjSize,sMagObj.szStz(),iPctUse,sFlag.szStz());
  }
  MagLib::vByteMag(nObjInHeap*nBlockSize,sMagBlocks);
  printf((char*)"  [%lld objects ; %s of blocks]\n"_Stz,nObjInHeap,sMagBlocks.szStz());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  nBlockSize = C_SIZE_SMALL_OBJECT;
  MagLib::vByteMag(nBlockSize,sMagHeap);
  AM::pHeapMem()->Transient.m_pSmall->vAnalyse(iCountObjects);
  nObjInHeap = 0;
  
  printf("\n");
  printf((char*)"  Transient Small Heap %s Object Summary:\n"_Stz,sMagHeap.szStz());
  for (i8 i_Index = 0; i_Index < CN_ENUMS; i_Index++) {
    eCN eClass = (eCN)i_Index;
    
    i8 nCntObj = iCountObjects[eClass];
    if (! nCntObj)
      continue;
    
    nObjInHeap    += nCntObj;
    i8 nObjSize    = gxaSizeHeap[eClass].m_nClassSize;
    i8 nSumObjSize = nCntObj * nObjSize;
    i8 iPctUse     = (nObjSize * 100) / nBlockSize;
    
    MagLib::vByteMag(nSumObjSize,sMagObj);
    
    if (nObjSize < C_SIZE_TINY_OBJECT)
      sFlag = " *into tiny"_Stz;
    else
      sFlag = ""_Stz;

    printf((char*)"  %20s %-6lld (size %7lldB sum %6s use %2lld%%) %s\n"_Stz,
           aCN[eClass],nCntObj,nObjSize,sMagObj.szStz(),iPctUse,sFlag.szStz());
  }
  MagLib::vByteMag(nObjInHeap*nBlockSize,sMagBlocks);
  printf((char*)"  [%lld objects ; %s of blocks]\n"_Stz,nObjInHeap,sMagBlocks.szStz());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  nBlockSize = C_SIZE_MEDIUM_OBJECT;
  MagLib::vByteMag(nBlockSize,sMagHeap);
  AM::pHeapMem()->Transient.m_pMedium->vAnalyse(iCountObjects);
  nObjInHeap = 0;

  vTrace("\n"_Stz);
  vTrace("  Transient Medium Heap %s Object Summary:\n"_Stz,sMagHeap.szStz());
  for (i8 i_Index = 0; i_Index < CN_ENUMS; i_Index++) {
    eCN eClass = (eCN)i_Index;
    
    i8 nCntObj = iCountObjects[eClass];
    if (! nCntObj)
      continue;
    
    nObjInHeap    += nCntObj;
    i8 nObjSize    = gxaSizeHeap[eClass].m_nClassSize;
    i8 nSumObjSize = nCntObj * nObjSize;
    i8 iPctUse     = (nObjSize * 100) / nBlockSize;
    
    MagLib::vByteMag(nSumObjSize,sMagObj);

    if (nObjSize < C_SIZE_SMALL_OBJECT)
      sFlag = " *into small"_Stz;
    else
      sFlag = ""_Stz;
    
    printf((char*)"  %20s %-6lld (size %7lldB sum %6s use %2lld%%) %s\n"_Stz,
           aCN[eClass],nCntObj,nObjSize,sMagObj.szStz(),iPctUse);
  }
  MagLib::vByteMag(nObjInHeap*nBlockSize,sMagBlocks);
  printf((char*)"  [%lld objects ; %s of blocks]\n"_Stz,nObjInHeap,sMagBlocks.szStz());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  nBlockSize = C_SIZE_LARGE_OBJECT;
  MagLib::vByteMag(nBlockSize,sMagHeap);
  AM::pHeapMem()->Transient.m_pLarge->vAnalyse(iCountObjects);
  nObjInHeap = 0;

  printf("\n");
  printf((char*)"  Transient Large Heap %s Object Summary:\n"_Stz,sMagHeap.szStz());
  for (i8 i_Index = 0; i_Index < CN_ENUMS; i_Index++) {
    eCN eClass = (eCN)i_Index;
    
    i8 nCntObj = iCountObjects[eClass];
    if (! nCntObj)
      continue;
    
    nObjInHeap    += nCntObj;
    i8 nObjSize    = gxaSizeHeap[eClass].m_nClassSize;
    i8 nSumObjSize = nCntObj * nObjSize;
    i8 iPctUse     = (nObjSize * 100) / nBlockSize;
    
    MagLib::vByteMag(nSumObjSize,sMagObj);

    if (nObjSize < C_SIZE_MEDIUM_OBJECT)
      sFlag = " *into medium"_Stz;
    else
      sFlag = ""_Stz;
    

    printf((char*)"  %20s %-6lld (size %7lldB sum %6s use %2lld%%) %s\n"_Stz,
           aCN[eClass],nCntObj,nObjSize,sMagObj.szStz(),iPctUse);
  }
  MagLib::vByteMag(nObjInHeap*nBlockSize,sMagBlocks);
  printf((char*)"  [%lld objects ; %s of blocks]\n"_Stz,nObjInHeap,sMagBlocks.szStz());
}

/*----------------------------------------------------------------------------*/
/* summary report on objects in the pool                                      */
/*----------------------------------------------------------------------------*/
void PoolHelper::vObjectSnapshot(ObjectsInPool* pCountObjects)
{
  i8 iCountObjects[CN_ENUMS];
  
  for (i8 i_Index = 0; i_Index < CN_ENUMS; i_Index++)
    pCountObjects->m_nObjects[i_Index] = 0;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  for (i8 i_Buffer = gpBufferPool->iBufferFirst();
       i_Buffer <= gpBufferPool->iBufferLast();
       i_Buffer++) {
    
    PoolBuffer* pBuffer = gpBufferPool->pToPoolBuffer(i_Buffer);
    
    eCN eClass = pBuffer->eClass();
    
    if (CN_QUEUES == eClass)
      int iDebug = 1;
    
    pCountObjects->m_nObjects[eClass]++;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  AM::pHeapMem()->Transient.m_pSmall->vAnalyse(iCountObjects);
  
  for (i8 i_Index = 0; i_Index < CN_ENUMS; i_Index++) {
    eCN eClass = (eCN)i_Index;
    
    if (iCountObjects[eClass] == 0)
      continue;
    
    if (CN_QUEUES == eClass)
      int iDebug = 1;
    
    pCountObjects->m_nObjects[eClass] += iCountObjects[eClass];
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  AM::pHeapMem()->Transient.m_pMedium->vAnalyse(iCountObjects);
  
  for (i8 i_Index = 0; i_Index < CN_ENUMS; i_Index++) {
    eCN eClass = (eCN)i_Index;
    
    if (iCountObjects[eClass] == 0)
      continue;
    
    if (CN_QUEUES == eClass)
      int iDebug = 1;

    pCountObjects->m_nObjects[eClass] += iCountObjects[eClass];
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  AM::pHeapMem()->Transient.m_pLarge->vAnalyse(iCountObjects);
  
  for (i8 i_Index = 0; i_Index < CN_ENUMS; i_Index++) {
    eCN eClass = (eCN)i_Index;
    
    if (iCountObjects[eClass] == 0)
      continue;
    
    if (CN_QUEUES == eClass)
      int iDebug = 1;

    pCountObjects->m_nObjects[eClass] += iCountObjects[eClass];
  }
}

/*----------------------------------------------------------------------------*/
/* analyze the buffer pool                                                    */
/*----------------------------------------------------------------------------*/
void PoolHelper::vPoolAnalyze(void)
{
  // increment the epoch
  AM::iNextEpoch();
  
  printf("\n");
  printf("****************************************************************\n");
  printf("* Buffer Pool Analysis                                         *\n");
  printf("****************************************************************\n");
  
  vRegionSummary();
  vObjectSummary();
  vResourceSummary();
  
  printf("\n");
}

/*----------------------------------------------------------------------------*/
/* dump the buffer pool objects                                               */
/*----------------------------------------------------------------------------*/
void PoolHelper::vPoolDump(void)
{
  // increment the epoch
  AM::iNextEpoch();
  
  vTrace("\n"_Stz);
  vTrace("****************************************************************\n"_Stz);
  vTrace("* Buffer Pool Dump                                             *\n"_Stz);
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
i8 PoolHelper::iPoolFree(void)
{
  i8 iTransientTiny =
     AM::pHeapMem()->Transient.m_pTiny->nFreeChunks() * C_SIZE_TINY_OBJECT;
  
  i8 iTransientSmall =
     AM::pHeapMem()->Transient.m_pSmall->nFreeChunks() * C_SIZE_SMALL_OBJECT;
  
  i8 iTransientMedium =
     AM::pHeapMem()->Transient.m_pMedium->nFreeChunks() * C_SIZE_MEDIUM_OBJECT;
  
  i8 iTransientLarge =
     AM::pHeapMem()->Transient.m_pLarge->nFreeChunks() * C_SIZE_LARGE_OBJECT;
  
  
  i8 iPersistentTiny =
     AM::pHeapMem()->Persistent.m_pTiny->nFreeChunks() * C_SIZE_TINY_OBJECT;
  
  i8 iPersistentSmall =
     AM::pHeapMem()->Persistent.m_pSmall->nFreeChunks() * C_SIZE_SMALL_OBJECT;
  
  i8 iPersistentMedium =
     AM::pHeapMem()->Persistent.m_pMedium->nFreeChunks() * C_SIZE_MEDIUM_OBJECT;
  
  i8 iPersistentLarge =
     AM::pHeapMem()->Persistent.m_pLarge->nFreeChunks() * C_SIZE_LARGE_OBJECT;

  
  i8 iTotalFree =
       iTransientTiny
         + iTransientSmall  + iTransientMedium  + iTransientLarge +
       iPersistentTiny
         + iPersistentSmall + iPersistentMedium + iPersistentLarge;
  
  return iTotalFree;
}

/*----------------------------------------------------------------------------*/
/* read all the buffer pool buffers from file                                 */
/*----------------------------------------------------------------------------*/
void PoolHelper::vReadBuffers(Stz* sFilePath)
{
  File* pFile = new File(nullptr);
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
void PoolHelper::vRegionSummary()
{
  // get MagLib of the various buffer pools
  s32 sMagTiny;   MagLib::vByteMag(C_SIZE_TINY_OBJECT,  sMagTiny);
  s32 sMagSmall;  MagLib::vByteMag(C_SIZE_SMALL_OBJECT, sMagSmall);
  s32 sMagMedium; MagLib::vByteMag(C_SIZE_MEDIUM_OBJECT,sMagMedium);
  s32 sMagLarge;  MagLib::vByteMag(C_SIZE_LARGE_OBJECT, sMagLarge);
  
  printf("\n");
  printf((char*)"  Region %s %p size %lldB epoch %lld\n"_Stz,
         AM::szRegionName(),
         gpBufferPool,
         AM::iRegionSize(),
         AM::iEpoch());
  
  printf("\n");
  printf((char*)"  BufferPool buffer %6s buffers %lld alloc %lld free %lld \n"_Stz,
         sMagLarge.azChar(),
         gpBufferPool->m_nBuffers,
         gpBufferPool->m_nAllocCount,
         gpBufferPool->m_nFreeCount
         );
  
  printf("\n");
  printf((char*)"   Transient TinyHeap   (%6s) buffers %-6lld alloc %-6lld free %-6lld\n"_Stz,
         sMagTiny.azChar(),
         AM::pHeapMem()->Transient.m_pTiny->nBufferCount(),
         AM::pHeapMem()->Transient.m_pTiny->nCurrAllocated(),
         AM::pHeapMem()->Transient.m_pTiny->nFreeChunks());
  
  printf((char*)"   Transient SmallHeap  (%6s) buffers %-6lld alloc %-6lld free %-6lld\n"_Stz,
         sMagSmall.azChar(),
         AM::pHeapMem()->Transient.m_pSmall->nBufferCount(),
         AM::pHeapMem()->Transient.m_pSmall->nCurrAllocated(),
         AM::pHeapMem()->Transient.m_pSmall->nFreeChunks());
  
  printf((char*)"   Transient MediumHeap (%6s) buffers %-6lld alloc %-6lld free %-6lld\n"_Stz,
         sMagMedium.azChar(),
         AM::pHeapMem()->Transient.m_pMedium->nBufferCount(),
         AM::pHeapMem()->Transient.m_pMedium->nCurrAllocated(),
         AM::pHeapMem()->Transient.m_pMedium->nFreeChunks());
  
  printf((char*)"   Transient LargeHeap  (%6s) buffers %-6lld alloc %-6lld free %-6lld\n"_Stz,
         sMagLarge.azChar(),
         AM::pHeapMem()->Transient.m_pLarge->nBufferCount(),
         AM::pHeapMem()->Transient.m_pLarge->nCurrAllocated(),
         AM::pHeapMem()->Transient.m_pLarge->nFreeChunks());
  
  printf((char*)"  Persistent TinyHeap   (%6s) buffers %-6lld alloc %-6lld free %-6lld\n"_Stz,
         sMagTiny.azChar(),
         AM::pHeapMem()->Persistent.m_pTiny->nBufferCount(),
         AM::pHeapMem()->Persistent.m_pTiny->nCurrAllocated(),
         AM::pHeapMem()->Persistent.m_pTiny->nFreeChunks());
  
  printf((char*)"  Persistent SmallHeap  (%6s) buffers %-6lld alloc %-6lld free %-6lld\n"_Stz,
         sMagSmall.azChar(),
         AM::pHeapMem()->Persistent.m_pSmall->nBufferCount(),
         AM::pHeapMem()->Persistent.m_pSmall->nCurrAllocated(),
         AM::pHeapMem()->Persistent.m_pSmall->nFreeChunks());
  
  printf((char*)"  Persistent MediumHeap (%6s) buffers %-6lld alloc %-6lld free %-6lld\n"_Stz,
         sMagMedium.azChar(),
         AM::pHeapMem()->Persistent.m_pMedium->nBufferCount(),
         AM::pHeapMem()->Persistent.m_pMedium->nCurrAllocated(),
         AM::pHeapMem()->Persistent.m_pMedium->nFreeChunks());
  
  printf((char*)"  Persistent LargeHeap  (%6s) buffers %-6lld alloc %-6lld free %-6lld\n"_Stz,
         sMagLarge.azChar(),
         AM::pHeapMem()->Persistent.m_pLarge->nBufferCount(),
         AM::pHeapMem()->Persistent.m_pLarge->nCurrAllocated(),
         AM::pHeapMem()->Persistent.m_pLarge->nFreeChunks());
}

/*----------------------------------------------------------------------------*/
/* summarize resource usage                                                   */
/*----------------------------------------------------------------------------*/
void PoolHelper::vResourceSummary(void)
{
  printf("\n");
  printf("****************************************************************\n");
  printf((char*)"  Resources: dir %lld file %lld shm %lld socket %lld thread %lld\n"_Stz,
         AM::pResourceTracker()->nDirOpen(),
         AM::pResourceTracker()->nFileOpen(),
         AM::pResourceTracker()->nShmOpen(),
         AM::pResourceTracker()->nSocketOpen(),
         AM::pResourceTracker()->nThreadOpen());
  printf("****************************************************************\n");
}

/*----------------------------------------------------------------------------*/
/* list all words created since last epoch                                    */
/*----------------------------------------------------------------------------*/
void PoolHelper::vWordList(i8 iEpoch)
{
  PoolBuffer* pMediumTransient = nullptr;
  PoolBuffer* pSmallTransient  = nullptr;
  
  for (i8 i_Buffer = gpBufferPool->iBufferFirst();
       i_Buffer <= gpBufferPool->iBufferLast();
       i_Buffer++) {
    
    PoolBuffer* pBuffer = gpBufferPool->pToPoolBuffer(i_Buffer);
    
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
void PoolHelper::vWriteBuffers(Stz* sFilePath)
{
  File* pFile = new File(nullptr);
  pFile->bOpenWrite(sFilePath);
  
  i8 iStart  = TickingDevice::iGetTick();
  
  for (i8 i_Buffer = gpBufferPool->iBufferFirst();
       i_Buffer <= gpBufferPool->iBufferLast();
       i_Buffer++) {
    
    PoolBuffer* pBuffer = gpBufferPool->pToPoolBuffer(i_Buffer);
    bool bResult = pFile->bWriteBytes((void*)pBuffer,C_SIZE_POOL_BUFFER);
    ASSERT(bResult);
    
  }
  i8 iUpdate = TickingDevice::iGetTick();
  
  i8 iTicks = (iUpdate - iStart);
  i8 iTicksPerSec = TickingDevice::iTicksPerSec();
  
  i8 iMilli = iTicks * C_1K / iTicksPerSec;
  
  i8 nBuffers =gpBufferPool->iBufferLast() - 1;
  vTrace("Writing %lld buffers took %lld ms\n"_Stz, nBuffers, iMilli);
}

/*----------------------------------------------------------------------------*/
/* write given heap to file                                                   */
/*----------------------------------------------------------------------------*/
void PoolHelper::vWriteHeap(Heap* pHeap, File* pFile)
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
void PoolHelper::vWritePersistentHeapMem(Stz* sFilePath)
{
  File* pFile = new File(nullptr);
  pFile->bOpenWrite(sFilePath);
  
  vTrace("Writing persistent tiny heap\n"_Stz);
  vWriteHeap(AM::pHeapMem()->Persistent.m_pTiny,pFile);
  
  vTrace("Writing persistent small heap\n"_Stz);
  vWriteHeap(AM::pHeapMem()->Persistent.m_pSmall,pFile);
  
  vTrace("Writing persistent medium heap\n"_Stz);
  vWriteHeap(AM::pHeapMem()->Persistent.m_pMedium,pFile);
  
  vTrace("Writing persistent large heap\n"_Stz);
  vWriteHeap(AM::pHeapMem()->Persistent.m_pLarge,pFile);

  pFile->vClose();
  delete pFile;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
