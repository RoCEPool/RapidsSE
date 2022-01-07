/*============================================================================*/
/* PoolHelper.cpp - buffer pool analyzer                                      */
/*============================================================================*/

#include "BASE.h"

extern SizeHeap     gxaSizeHeap[];
extern ClassCatalog gxClassCatalog;

/*----------------------------------------------------------------------------*/
/* dump objects after given timepoint                                         */
/*----------------------------------------------------------------------------*/
void PoolHelper::vObjectDump(i8 iAfter)
{
  _static_
  
  vObjectDump(iAfter,AM::pHeaps()->Transient.m_pTiny);
  vObjectDump(iAfter,AM::pHeaps()->Transient.m_pSmall);
  vObjectDump(iAfter,AM::pHeaps()->Transient.m_pMedium);
  vObjectDump(iAfter,AM::pHeaps()->Transient.m_pLarge);
  vObjectDump(iAfter,AM::pHeaps()->Transient.m_pData);
}

/*----------------------------------------------------------------------------*/
/* dump objects after given timepoint                                         */
/*----------------------------------------------------------------------------*/
void PoolHelper::vObjectDump(i8 iAfter, Heap* pHeap)
{
  _static_
  
  pHeap->vGrabLock();
  
  i8 iCount = 0;
  for (HeapBuffer* pHeapBuffer = pHeap->pFirstBuffer();
       pHeapBuffer;
       pHeapBuffer = pHeapBuffer->pNextBuffer()) {
    
    for (FreeChunk* aChunk = pHeapBuffer->aFirstChunk();
         aChunk;
         aChunk = pHeapBuffer->aNextChunk(aChunk,pHeap->nObjectSize())) {
      
      eCN eClass = aChunk->eGetClassNo();
      
      if (CN_FREECHUNK == eClass)
        continue;
                  
      Footer* aFooter =
        aAdd<Footer*>(aChunk,gxaSizeHeap[eClass].m_nClassSize-sizeof(Footer));

      i8 iWhenAllocated =
        aFooter->bValidFooter() ? aFooter->iWhenAllocated() : -1;

      if (iWhenAllocated <= iAfter)
        continue;
      
      vTrace("%s%p %-12s %16lld: "_Stz,
             Stz::szTab(2),aChunk,aCN[eClass],iWhenAllocated);
      if (! (++iCount % 4))
        vTrace("\n"_Stz);
    }
  }
  if (iCount % 4)
    vTrace("\n"_Stz);
  
  pHeap->vDropLock();

}

/*----------------------------------------------------------------------------*/
/* dump all objects in given heap                                             */
/*----------------------------------------------------------------------------*/
void PoolHelper::vHeapDump(Heap* pHeap)
{
  _static_

  HeapBuffer* pHeapBuffer;
  
  vTrace("\n"_Stz);
  vTrace("%sObjects in %s Heap:\n"_Stz,
         Stz::szTab(1),
         
           pHeap->eGetClassNo() == CN_TINYTRANSIENT    ? "Tiny Transient"_Stz
         : pHeap->eGetClassNo() == CN_SMALLTRANSIENT   ? "Small Transient"_Stz
         : pHeap->eGetClassNo() == CN_MEDIUMTRANSIENT  ? "Medium Transient"_Stz
         : pHeap->eGetClassNo() == CN_LARGETRANSIENT   ? "Large Transient"_Stz
         : pHeap->eGetClassNo() == CN_TINYPERSISTENT   ? "Tiny Persistent"_Stz
         : pHeap->eGetClassNo() == CN_SMALLPERSISTENT  ? "Small Persistent"_Stz
         : pHeap->eGetClassNo() == CN_MEDIUMPERSISTENT ? "Medium Persistent"_Stz
         : pHeap->eGetClassNo() == CN_LARGEPERSISTENT  ? "Large Persistent"_Stz
         : "unknown"_Stz
         );
         
  pHeap->vGrabLock();
  
  i8 iCount = 0;
  for (pHeapBuffer = pHeap->pFirstBuffer();
       pHeapBuffer;
       pHeapBuffer = pHeapBuffer->pNextBuffer()) {
    
    for (FreeChunk* aChunk = pHeapBuffer->aFirstChunk();
         aChunk;
         aChunk = pHeapBuffer->aNextChunk(aChunk,pHeap->nObjectSize())) {
      
      eCN eClass = aChunk->eGetClassNo();
      
      if (CN_FREECHUNK == eClass)
        continue;
      
      if (CN_WORD == eClass)
        continue;
            
      Footer* aFooter =
        aAdd<Footer*>(aChunk,gxaSizeHeap[eClass].m_nClassSize-sizeof(Footer));

      i8 iWhenAllocated =
        aFooter->bValidFooter() ? aFooter->iWhenAllocated() : -1;

      vTrace("%s%p %-12s %16lld: "_Stz,
             Stz::szTab(2),aChunk,aCN[eClass],iWhenAllocated);
      if (! (++iCount % 4))
        vTrace("\n"_Stz);
    }
  }
  
  vTrace("\n"_Stz);
  
  pHeap->vDropLock();
}

/*----------------------------------------------------------------------------*/
/* dump all objects in given heap                                             */
/*----------------------------------------------------------------------------*/
void PoolHelper::vHeapDump(Heap* pHeap, i8 iEpoch)
{
  _static_

  UNUSED(iEpoch);
  
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
      
      eCN eClass = aChunk->eGetClassNo();
      
      if (CN_FREECHUNK == eClass)
        continue;
      
      if (CN_WORD != eClass)
        continue;
      
      //Word* pWord = _pWord(aChunk);
      
      // dump only the objects
      continue;
      
      // dump only object created in last epoch
      //if (pWord->iEpoch() != iEpoch)
      //  continue;
      
      //pDUMPFUN* pDumpFun = gpDump[eClass];
      
      vTrace("%s%p %-12s: "_Stz,Stz::szTab(2),pHeap,aCN[eClass]);
      
      //(pDumpFun)(eClass, (Object*)aChunk, Stz::szTab(2));
    }
  }
  
  pHeap->vDropLock();
}

/*----------------------------------------------------------------------------*/
/* inspect objects after given timepoint                                      */
/*----------------------------------------------------------------------------*/
void PoolHelper::vInspectObjects(i8 iAfter)
{
  _static_
  
  Heaps* pHeaps = AM::pHeaps();

  eHT eHeap = HT_INVALID;
  
  for (Heap* pHeap = pHeaps->pEachHeap(eHeap);
       pHeap;
       pHeap = pHeaps->pEachHeap(eHeap)) {
  
    pHeap->vGrabLock();
    
    for (HeapBuffer* pHeapBuffer = pHeap->pFirstBuffer();
         pHeapBuffer;
         pHeapBuffer = pHeapBuffer->pNextBuffer()) {
      
      for (FreeChunk* aChunk = pHeapBuffer->aFirstChunk();
           aChunk;
           aChunk = pHeapBuffer->aNextChunk(aChunk,pHeap->nObjectSize())) {
        
        eCN eClass = aChunk->eGetClassNo();
        if (CN_FREECHUNK == eClass)
          continue; // next chunk
        
        Object* pObject = (Object*)aChunk;
        Footer* aFooter = pObject->aCalcFooterAddress();
        UNUSED(aFooter);
        
        i8 iWhenAllocated = pObject->iWhenAllocated();
        if (iWhenAllocated <= iAfter)
          continue; // next object
               
        bool b = gxClassCatalog.bCallClassFn(CF_INSPECT,pObject);
        UNUSED(b);
        
       } // each chunk/object in heapBuffer
      
    } // each heapBuffer in heap
    
    pHeap->vDropLock();
  
  } // each heap in heapMem
}


/*----------------------------------------------------------------------------*/
/* summary report on objects in the pool and heaps                            */
/*----------------------------------------------------------------------------*/
void PoolHelper::vObjectSummary(void)
{
  _static_

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
    iCountObjects[pBuffer->eGetClassNo()]++;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  vTrace("\n"_Stz);
  vTrace("  BufferPool Object Summary:\n"_Stz);
  for (i8 i_Index = 0; i_Index < CN_ENUMS; i_Index++) {
    eCN eClass = (eCN)i_Index;
    
    if (iCountObjects[eClass] == 0)
      continue;
    
    vTrace("  %20s %-6lld \n"_Stz, aCN[eClass], iCountObjects[eClass]);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  i8 nBlockSize =0;
  
  nBlockSize = C_SIZE_TINY_OBJECT;
  MagLib::vByteMag(nBlockSize,sMagHeap);
  AM::pHeaps()->Transient.m_pTiny->vAnalyse(iCountObjects);
  nObjInHeap = 0;
  
  vTrace("\n"_Stz);
  vTrace("  Transient Tiny Heap %s Object Summary:\n"_Stz,sMagHeap.szStz());
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

    vTrace("  %20s %-6lld (size %7lldB sum %6s use %2lld%%) %s\n"_Stz,
           aCN[eClass],nCntObj,nObjSize,sMagObj.szStz(),iPctUse,sFlag.szStz());
  }
  MagLib::vByteMag(nObjInHeap*nBlockSize,sMagBlocks);
  vTrace("  [%lld objects ; %s of blocks]\n"_Stz,nObjInHeap,sMagBlocks.szStz());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  nBlockSize = C_SIZE_SMALL_OBJECT;
  MagLib::vByteMag(nBlockSize,sMagHeap);
  AM::pHeaps()->Transient.m_pSmall->vAnalyse(iCountObjects);
  nObjInHeap = 0;
  
  vTrace("\n"_Stz);
  vTrace("  Transient Small Heap %s Object Summary:\n"_Stz,sMagHeap.szStz());
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

    vTrace("  %20s %-6lld (size %7lldB sum %6s use %2lld%%) %s\n"_Stz,
           aCN[eClass],nCntObj,nObjSize,sMagObj.szStz(),iPctUse,sFlag.szStz());
  }
  MagLib::vByteMag(nObjInHeap*nBlockSize,sMagBlocks);
  vTrace("  [%lld objects ; %s of blocks]\n"_Stz,nObjInHeap,sMagBlocks.szStz());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  nBlockSize = C_SIZE_MEDIUM_OBJECT;
  MagLib::vByteMag(nBlockSize,sMagHeap);
  AM::pHeaps()->Transient.m_pMedium->vAnalyse(iCountObjects);
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
    
    vTrace("  %20s %-6lld (size %7lldB sum %6s use %2lld%%) %s\n"_Stz,
           aCN[eClass],nCntObj,nObjSize,sMagObj.szStz(),iPctUse);
  }
  MagLib::vByteMag(nObjInHeap*nBlockSize,sMagBlocks);
  vTrace("  [%lld objects ; %s of blocks]\n"_Stz,nObjInHeap,sMagBlocks.szStz());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  nBlockSize = C_SIZE_LARGE_OBJECT;
  MagLib::vByteMag(nBlockSize,sMagHeap);
  AM::pHeaps()->Transient.m_pLarge->vAnalyse(iCountObjects);
  nObjInHeap = 0;

  vTrace("\n"_Stz);
  vTrace("  Transient Large Heap %s Object Summary:\n"_Stz,sMagHeap.szStz());
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
    

    vTrace("  %20s %-6lld (size %7lldB sum %6s use %2lld%%) %s\n"_Stz,
           aCN[eClass],nCntObj,nObjSize,sMagObj.szStz(),iPctUse);
  }
  MagLib::vByteMag(nObjInHeap*nBlockSize,sMagBlocks);
  vTrace("  [%lld objects ; %s of blocks]\n"_Stz,nObjInHeap,sMagBlocks.szStz());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  nBlockSize = C_SIZE_DATA_OBJECT;
  MagLib::vByteMag(nBlockSize,sMagHeap);
  AM::pHeaps()->Transient.m_pData->vAnalyse(iCountObjects);
  nObjInHeap = 0;

  vTrace("\n"_Stz);
  vTrace("  Transient Data Heap %s Object Summary:\n"_Stz,sMagHeap.szStz());
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

    sFlag = ""_Stz;
    
    vTrace("  %20s %-6lld (size %7lldB sum %6s use %2lld%%) %s\n"_Stz,
           aCN[eClass],nCntObj,nObjSize,sMagObj.szStz(),iPctUse);
  }
  MagLib::vByteMag(nObjInHeap*nBlockSize,sMagBlocks);
  vTrace("  [%lld objects ; %s of blocks]\n"_Stz,nObjInHeap,sMagBlocks.szStz());
}

/*----------------------------------------------------------------------------*/
/* summary report on objects in the pool                                      */
/*----------------------------------------------------------------------------*/
HACK(PoolHelper,"vObjectSnapshot() is ignoring tiny heap : Ray 28-Dec-2020)");

void PoolHelper::vObjectSnapshot(ObjectSnapshot* pPoolSnapshot)
{
  i8 iCountObjects[CN_ENUMS];
  
  for (i8 i_Index = 0; i_Index < CN_ENUMS; i_Index++)
    pPoolSnapshot->m_nObjects[i_Index] = 0;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  for (i8 i_Buffer = gpBufferPool->iBufferFirst();
       i_Buffer <= gpBufferPool->iBufferLast();
       i_Buffer++) {
    
    PoolBuffer* pBuffer = gpBufferPool->pToPoolBuffer(i_Buffer);
    
    eCN eClass = pBuffer->eGetClassNo();
        
    pPoolSnapshot->m_nObjects[eClass]++;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  AM::pHeaps()->Transient.m_pTiny->vAnalyse(iCountObjects);

  for (i8 i_Index = 0; i_Index < CN_ENUMS; i_Index++) {
    eCN eClass = (eCN)i_Index;

    if (iCountObjects[eClass] == 0)
      continue;

    pPoolSnapshot->m_nObjects[eClass] += iCountObjects[eClass];
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  AM::pHeaps()->Transient.m_pSmall->vAnalyse(iCountObjects);
  
  for (i8 i_Index = 0; i_Index < CN_ENUMS; i_Index++) {
    eCN eClass = (eCN)i_Index;
    
    if (iCountObjects[eClass] == 0)
      continue;
        
    pPoolSnapshot->m_nObjects[eClass] += iCountObjects[eClass];
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  AM::pHeaps()->Transient.m_pMedium->vAnalyse(iCountObjects);
  
  for (i8 i_Index = 0; i_Index < CN_ENUMS; i_Index++) {
    eCN eClass = (eCN)i_Index;
    
    if (iCountObjects[eClass] == 0)
      continue;
    
    pPoolSnapshot->m_nObjects[eClass] += iCountObjects[eClass];
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  AM::pHeaps()->Transient.m_pLarge->vAnalyse(iCountObjects);
  
  for (i8 i_Index = 0; i_Index < CN_ENUMS; i_Index++) {
    eCN eClass = (eCN)i_Index;
    
    if (iCountObjects[eClass] == 0)
      continue;
    
    pPoolSnapshot->m_nObjects[eClass] += iCountObjects[eClass];
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  AM::pHeaps()->Transient.m_pData->vAnalyse(iCountObjects);
  
  for (i8 i_Index = 0; i_Index < CN_ENUMS; i_Index++) {
    eCN eClass = (eCN)i_Index;
    
    if (iCountObjects[eClass] == 0)
      continue;
    
    pPoolSnapshot->m_nObjects[eClass] += iCountObjects[eClass];
  }
}

/*----------------------------------------------------------------------------*/
/* validate all objects                                                       */
/*----------------------------------------------------------------------------*/
void PoolHelper::vObjectValidate(void)
{
  _static_
  
  AM::pHeaps()->Transient.m_pTiny->vValidateContents();
  AM::pHeaps()->Transient.m_pSmall->vValidateContents();
  AM::pHeaps()->Transient.m_pMedium->vValidateContents();
  AM::pHeaps()->Transient.m_pLarge->vValidateContents();
  AM::pHeaps()->Transient.m_pData->vValidateContents();
}

/*----------------------------------------------------------------------------*/
/* analyze the buffer pool                                                    */
/*----------------------------------------------------------------------------*/
void PoolHelper::vPoolAnalyze(void)
{
  _static_

  // increment the epoch
  AM::iNextEpoch();
  
  vTrace("\n"_Stz);
  vTrace("****************************************************************\n"_Stz);
  vTrace("* Buffer Pool Analysis                                         *\n"_Stz);
  vTrace("****************************************************************\n"_Stz);
  
  vRegionSummary();
  vObjectSummary();
  //vResourceSummary();
  
  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* dump the buffer pool objects                                               */
/*----------------------------------------------------------------------------*/
void PoolHelper::vPoolDump(void)
{
  _static_

  // increment the epoch
  AM::iNextEpoch();
  
  vTrace("\n"_Stz);
  vTrace("****************************************************************\n"_Stz);
  vTrace("* Buffer Pool Dump                                             *\n"_Stz);
  vTrace("****************************************************************\n"_Stz);
  
  vRegionSummary();
  vObjectSummary();
  //vResourceSummary();
  
  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* get free space in the buffer pool                                          */
/*----------------------------------------------------------------------------*/
i8 PoolHelper::iPoolFree(void)
{
  _static_

  i8 iTransientTiny =
     AM::pHeaps()->Transient.m_pTiny->nFreeChunks()   * C_SIZE_TINY_OBJECT;
  
  i8 iTransientSmall =
     AM::pHeaps()->Transient.m_pSmall->nFreeChunks()  * C_SIZE_SMALL_OBJECT;
  
  i8 iTransientMedium =
     AM::pHeaps()->Transient.m_pMedium->nFreeChunks() * C_SIZE_MEDIUM_OBJECT;
  
  i8 iTransientLarge =
     AM::pHeaps()->Transient.m_pLarge->nFreeChunks()  * C_SIZE_LARGE_OBJECT;

  i8 iTransientData =
     AM::pHeaps()->Transient.m_pData->nFreeChunks()   * C_SIZE_DATA_OBJECT;

  
//  i8 iPersistentTiny =
//     AM::pHeaps()->Persistent.m_pTiny->nFreeChunks() * C_SIZE_TINY_OBJECT;
//
//  i8 iPersistentSmall =
//     AM::pHeaps()->Persistent.m_pSmall->nFreeChunks() * C_SIZE_SMALL_OBJECT;
//
//  i8 iPersistentMedium =
//     AM::pHeaps()->Persistent.m_pMedium->nFreeChunks() * C_SIZE_MEDIUM_OBJECT;
//
//  i8 iPersistentLarge =
//     AM::pHeaps()->Persistent.m_pLarge->nFreeChunks() * C_SIZE_LARGE_OBJECT;

  
  i8 iTotalFree = iTransientTiny
                    + iTransientSmall
                    + iTransientMedium
                    + iTransientLarge
                    + iTransientData
//         + iPersistentTiny
//         + iPersistentSmall + iPersistentMedium + iPersistentLarge
  ;
  
  return iTotalFree;
}

/*----------------------------------------------------------------------------*/
/* report on shm region                                                       */
/*----------------------------------------------------------------------------*/
void PoolHelper::vRegionSummary()
{
  _static_

  // get MagLib of the various buffer pools
  s32 sMagTiny;   MagLib::vByteMag(C_SIZE_TINY_OBJECT,  sMagTiny);
  s32 sMagSmall;  MagLib::vByteMag(C_SIZE_SMALL_OBJECT, sMagSmall);
  s32 sMagMedium; MagLib::vByteMag(C_SIZE_MEDIUM_OBJECT,sMagMedium);
  s32 sMagLarge;  MagLib::vByteMag(C_SIZE_LARGE_OBJECT, sMagLarge);
  
  vTrace("\n"_Stz);
  vTrace("  Region %s %p size %lldB epoch %lld\n"_Stz,
         AM::szRegionName(),
         gpBufferPool,
         AM::iRegionSize(),
         AM::iEpoch());
  
  vTrace("\n"_Stz);
  vTrace("  BufferPool buffer %6s buffers %lld alloc %lld free %lld \n"_Stz,
         sMagLarge.azChar(),
         gpBufferPool->m_nBuffers,
         gpBufferPool->m_nAllocCount,
         gpBufferPool->m_nFreeCount
         );
  
  vTrace("\n"_Stz);
  vTrace("   Transient TinyHeap   (%6s) buffers %-6lld alloc %-6lld free %-6lld\n"_Stz,
         sMagTiny.azChar(),
         AM::pHeaps()->Transient.m_pTiny->nBufferCount(),
         AM::pHeaps()->Transient.m_pTiny->nCurrAllocated(),
         AM::pHeaps()->Transient.m_pTiny->nFreeChunks());
  
  vTrace("   Transient SmallHeap  (%6s) buffers %-6lld alloc %-6lld free %-6lld\n"_Stz,
         sMagSmall.azChar(),
         AM::pHeaps()->Transient.m_pSmall->nBufferCount(),
         AM::pHeaps()->Transient.m_pSmall->nCurrAllocated(),
         AM::pHeaps()->Transient.m_pSmall->nFreeChunks());
  
  vTrace("   Transient MediumHeap (%6s) buffers %-6lld alloc %-6lld free %-6lld\n"_Stz,
         sMagMedium.azChar(),
         AM::pHeaps()->Transient.m_pMedium->nBufferCount(),
         AM::pHeaps()->Transient.m_pMedium->nCurrAllocated(),
         AM::pHeaps()->Transient.m_pMedium->nFreeChunks());
  
  vTrace("   Transient LargeHeap  (%6s) buffers %-6lld alloc %-6lld free %-6lld\n"_Stz,
         sMagLarge.azChar(),
         AM::pHeaps()->Transient.m_pLarge->nBufferCount(),
         AM::pHeaps()->Transient.m_pLarge->nCurrAllocated(),
         AM::pHeaps()->Transient.m_pLarge->nFreeChunks());
  
  vTrace("   Transient DataHeap  (%6s) buffers %-6lld alloc %-6lld free %-6lld\n"_Stz,
         sMagLarge.azChar(),
         AM::pHeaps()->Transient.m_pData->nBufferCount(),
         AM::pHeaps()->Transient.m_pData->nCurrAllocated(),
         AM::pHeaps()->Transient.m_pData->nFreeChunks());
  
//  vTrace((char*)"  Persistent TinyHeap   (%6s) buffers %-6lld alloc %-6lld free %-6lld\n"_Stz,
//         sMagTiny.azChar(),
//         AM::pHeaps()->Persistent.m_pTiny->nBufferCount(),
//         AM::pHeaps()->Persistent.m_pTiny->nCurrAllocated(),
//         AM::pHeaps()->Persistent.m_pTiny->nFreeChunks());
//
//  vTrace((char*)"  Persistent SmallHeap  (%6s) buffers %-6lld alloc %-6lld free %-6lld\n"_Stz,
//         sMagSmall.azChar(),
//         AM::pHeaps()->Persistent.m_pSmall->nBufferCount(),
//         AM::pHeaps()->Persistent.m_pSmall->nCurrAllocated(),
//         AM::pHeaps()->Persistent.m_pSmall->nFreeChunks());
//
//  vTrace((char*)"  Persistent MediumHeap (%6s) buffers %-6lld alloc %-6lld free %-6lld\n"_Stz,
//         sMagMedium.azChar(),
//         AM::pHeaps()->Persistent.m_pMedium->nBufferCount(),
//         AM::pHeaps()->Persistent.m_pMedium->nCurrAllocated(),
//         AM::pHeaps()->Persistent.m_pMedium->nFreeChunks());
//
//  vTrace((char*)"  Persistent LargeHeap  (%6s) buffers %-6lld alloc %-6lld free %-6lld\n"_Stz,
//         sMagLarge.azChar(),
//         AM::pHeaps()->Persistent.m_pLarge->nBufferCount(),
//         AM::pHeaps()->Persistent.m_pLarge->nCurrAllocated(),
//         AM::pHeaps()->Persistent.m_pLarge->nFreeChunks());
}

/*----------------------------------------------------------------------------*/
/* validate enum tables                                                       */
/*----------------------------------------------------------------------------*/
void PoolHelper::vValidateEnums(void)
{
  // note: using i8 for i_Class instead of eCN because you cannot
  //       increment (++) an enum
  
  for (i8 i_Class = CN_NONE; i_Class < CN_ENUMS; i_Class++) {
    
    eCN eClass = (eCN)i_Class;
    
    //vTrace("%s\n"_Stz,aCN[eClass]);
    
    eHT eHeap = gxaSizeHeap[eClass].m_eWhichHeap;
    i8  iSize = gxaSizeHeap[eClass].m_nClassSize;
    
    switch(eHeap) {
      case HT_INVALID:
        break;
        
      // transient -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
        
      case HT_T_TINY:
        if (iSize >= C_SIZE_TINY_OBJECT)
          vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
                 "Heaps::vValidateEnums: transient tiny class %lld %s is too big\n"_Stz,
                 eClass,aCN[eClass]);
        break;
        
      case HT_T_SMALL:
        if (iSize >= C_SIZE_SMALL_OBJECT)
          vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
                 "Heaps::vValidateEnums: transient small class %lld %s is too big\n"_Stz,
                 eClass,aCN[eClass]);
        break;
        
      case HT_T_MEDIUM:
        if (iSize >= C_SIZE_MEDIUM_OBJECT)
          vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
                 "Heaps::vValidateEnums: transient medium class %lld %s is too big\n"_Stz,
                 eClass,aCN[eClass]);
        break;
        
      case HT_T_LARGE:
        if (iSize >= C_SIZE_LARGE_OBJECT)
          vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
                 "Heaps::vValidateEnums: transient large class %lld %s is too big\n"_Stz,
                 eClass,aCN[eClass]);
        break;
         
      case HT_T_DATA:
        if (iSize >= C_SIZE_DATA_OBJECT)
          vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
                 "Heaps::vValidateEnums: transient data class %lld %s is too big\n"_Stz,
                 eClass,aCN[eClass]);
        break;
               // persistent -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

      case HT_P_TINY:
        if (iSize >= C_SIZE_SMALL_OBJECT)
          vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
                 "Heaps::vValidateEnums: persistent tiny class %lld %s is too big\n"_Stz,
                 eClass,aCN[eClass]);
        break;
        
      case HT_P_SMALL:
        if (iSize >= C_SIZE_SMALL_OBJECT)
          vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
                 "Heaps::vValidateEnums: persistent small class %lld %s is too big\n"_Stz,
                 eClass,aCN[eClass]);
        break;
        
      case HT_P_MEDIUM:
        if (iSize >= C_SIZE_MEDIUM_OBJECT)
          vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
                 "Heaps::vValidateEnums: persistent medium class %lld %s is too big\n"_Stz,
                 eClass,aCN[eClass]);
        break;
        
      case HT_P_LARGE:
        if (iSize >= C_SIZE_LARGE_OBJECT)
          vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
                 "Heaps::vValidateEnums: persistent large class %lld %s is too big\n"_Stz,
                 eClass,aCN[eClass]);
        break;
                
      default:
        vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
               "Heaps::vValidateEnums: Invalid which heap: eHeap %lld\n"_Stz,
               eHeap);
    }
  }
}


/*----------------------------------------------------------------------------*/
/* list all words created since last epoch                                    */
/*----------------------------------------------------------------------------*/
void PoolHelper::vWordList(i8 iEpoch)
{
  _static_

  PoolBuffer* pMediumTransient = nullptr;
  PoolBuffer* pSmallTransient  = nullptr;
  
  for (i8 i_Buffer = gpBufferPool->iBufferFirst();
       i_Buffer <= gpBufferPool->iBufferLast();
       i_Buffer++) {
    
    PoolBuffer* pBuffer = gpBufferPool->pToPoolBuffer(i_Buffer);
    
    if (CN_MEDIUMTRANSIENT == pBuffer->eGetClassNo())
      pMediumTransient = pBuffer;
    
    if (CN_SMALLTRANSIENT == pBuffer->eGetClassNo())
      pSmallTransient = pBuffer;
  }
  
  if (pMediumTransient)
    vHeapDump(reinterpret_cast<Heap*>(pMediumTransient), iEpoch);
  
  if (pSmallTransient)
    vHeapDump(reinterpret_cast<Heap*>(pSmallTransient), iEpoch);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
