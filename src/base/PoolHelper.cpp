/*============================================================================*/
/* PoolHelper.cpp - buffer pool analyzer                                      */
/*============================================================================*/

#include "BASE.h"

#include "TagConstants.h"

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
}

/*----------------------------------------------------------------------------*/
/* dump objects after given timepoint                                         */
/*----------------------------------------------------------------------------*/
void PoolHelper::vObjectDump(i8 iAfter, Heap* pHeap)
{
  _static_

  pHeap->vGrabWriteLock();
  
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

  pHeap->vDropWriteLock();

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

  pHeap->vGrabWriteLock();
  
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

  pHeap->vDropWriteLock();
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

  pHeap->vGrabWriteLock();
  
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

  pHeap->vDropWriteLock();
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

    pHeap->vGrabWriteLock();
    
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

    pHeap->vDropWriteLock();
  
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
  

  for (i8 i_PoolBuffer = gpBufferPool->iBufferFirst();
       i_PoolBuffer <= gpBufferPool->iBufferLast();
       i_PoolBuffer++) {
    
    PoolBuffer* pBuffer = FileMap::pToPoolBuffer(i_PoolBuffer);
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
  AM::pHeaps()->Persistent.m_pTiny->vAnalyse(iCountObjects);
  nObjInHeap = 0;

  vTrace("\n"_Stz);
  vTrace("  Persistent Tiny Heap %s Object Summary:\n"_Stz,sMagHeap.szStz());
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

  if(nObjInHeap != AM::pHeaps()->Persistent.m_pTiny->nCurrAllocated())
  {
    vTrace("  ******************************************************************\n"_Stz);
    vTrace("  WARNING: Number of objects: %lld does not match current allocs: %lld\n"_Stz,
           nObjInHeap, AM::pHeaps()->Persistent.m_pTiny->nCurrAllocated());
    vTrace("  ******************************************************************\n"_Stz);
  }
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  nBlockSize = C_SIZE_SMALL_OBJECT;
  MagLib::vByteMag(nBlockSize,sMagHeap);
  AM::pHeaps()->Persistent.m_pSmall->vAnalyse(iCountObjects);
  nObjInHeap = 0;

  vTrace("\n"_Stz);
  vTrace("  Persistent Small Heap %s Object Summary:\n"_Stz,sMagHeap.szStz());
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

  if(nObjInHeap != AM::pHeaps()->Persistent.m_pSmall->nCurrAllocated())
  {
    vTrace("  ******************************************************************\n"_Stz);
    vTrace("  WARNING: Number of objects: %lld does not match current allocs: %lld\n"_Stz,
           nObjInHeap, AM::pHeaps()->Persistent.m_pSmall->nCurrAllocated());
    vTrace("  ******************************************************************\n"_Stz);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  nBlockSize = C_SIZE_MEDIUM_OBJECT;
  MagLib::vByteMag(nBlockSize,sMagHeap);
  AM::pHeaps()->Persistent.m_pMedium->vAnalyse(iCountObjects);
  nObjInHeap = 0;

  vTrace("\n"_Stz);
  vTrace("  Persistent Medium Heap %s Object Summary:\n"_Stz,sMagHeap.szStz());
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
           aCN[eClass],nCntObj,nObjSize,sMagObj.szStz(),iPctUse,sFlag.szStz());
  }
  MagLib::vByteMag(nObjInHeap*nBlockSize,sMagBlocks);
  vTrace("  [%lld objects ; %s of blocks]\n"_Stz,nObjInHeap,sMagBlocks.szStz());

  if(nObjInHeap != AM::pHeaps()->Persistent.m_pMedium->nCurrAllocated())
  {
    vTrace("  ******************************************************************\n"_Stz);
    vTrace("  WARNING: Number of objects: %lld does not match current allocs: %lld\n"_Stz,
           nObjInHeap, AM::pHeaps()->Persistent.m_pMedium->nCurrAllocated());
    vTrace("  ******************************************************************\n"_Stz);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  nBlockSize = C_SIZE_LARGE_OBJECT;
  MagLib::vByteMag(nBlockSize,sMagHeap);
  AM::pHeaps()->Persistent.m_pLarge->vAnalyse(iCountObjects);
  nObjInHeap = 0;

  vTrace("\n"_Stz);
  vTrace("  Persistent Large Heap %s Object Summary:\n"_Stz,sMagHeap.szStz());
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
           aCN[eClass],nCntObj,nObjSize,sMagObj.szStz(),iPctUse,sFlag.szStz());
  }
  MagLib::vByteMag(nObjInHeap*nBlockSize,sMagBlocks);
  vTrace("  [%lld objects ; %s of blocks]\n"_Stz,nObjInHeap,sMagBlocks.szStz());

  if(nObjInHeap != AM::pHeaps()->Persistent.m_pLarge->nCurrAllocated())
  {
    vTrace("  ******************************************************************\n"_Stz);
    vTrace("  WARNING: Number of objects: %lld does not match current allocs: %lld\n"_Stz,
           nObjInHeap, AM::pHeaps()->Persistent.m_pLarge->nCurrAllocated());
    vTrace("  ******************************************************************\n"_Stz);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
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

  if(nObjInHeap != AM::pHeaps()->Transient.m_pTiny->nCurrAllocated())
  {
    vTrace("  ******************************************************************\n"_Stz);
    vTrace("  WARNING: Number of objects: %lld does not match current allocs: %lld\n"_Stz,
           nObjInHeap, AM::pHeaps()->Transient.m_pTiny->nCurrAllocated());
    vTrace("  ******************************************************************\n"_Stz);
  }

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

  if(nObjInHeap != AM::pHeaps()->Transient.m_pSmall->nCurrAllocated())
  {
    vTrace("  ******************************************************************\n"_Stz);
    vTrace("  WARNING: Number of objects: %lld does not match current allocs: %lld\n"_Stz,
           nObjInHeap, AM::pHeaps()->Transient.m_pSmall->nCurrAllocated());
    vTrace("  ******************************************************************\n"_Stz);
  }

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

  if(nObjInHeap != AM::pHeaps()->Transient.m_pMedium->nCurrAllocated())
  {
    vTrace("  ******************************************************************\n"_Stz);
    vTrace("  WARNING: Number of objects: %lld does not match current allocs: %lld\n"_Stz,
           nObjInHeap, AM::pHeaps()->Transient.m_pMedium->nCurrAllocated());
    vTrace("  ******************************************************************\n"_Stz);
  }

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

  if(nObjInHeap != AM::pHeaps()->Transient.m_pLarge->nCurrAllocated())
  {
    vTrace("  ******************************************************************\n"_Stz);
    vTrace("  WARNING: Number of objects: %lld does not match current allocs: %lld\n"_Stz,
           nObjInHeap, AM::pHeaps()->Transient.m_pLarge->nCurrAllocated());
    vTrace("  ******************************************************************\n"_Stz);
  }
}

/*----------------------------------------------------------------------------*/
/* summary report on objects in the pool and heaps                            */
/*----------------------------------------------------------------------------*/
void PoolHelper::vObjectSummary(RowBuffer* pRowBuffer, StreamDef* pStreamDef,
																RowBuffer* pRowBufferWarn, StreamDef* pWarnDef)
{
  _static_

  s32 sFlag;
  s32 sMagObj;
  s32 sMagHeap;
  s32 sMagBlocks;
  i8  nObjInHeap;

  i8 iCountObjects[CN_ENUMS];

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  i8 nBlockSize =0;

  nBlockSize = C_SIZE_TINY_OBJECT;
  MagLib::vByteMag(nBlockSize,sMagHeap);
  AM::pHeaps()->Persistent.m_pTiny->vAnalyse(iCountObjects);
  nObjInHeap = 0;

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

    Row* pRow = pRowBuffer->pAllotRow(pStreamDef,
                                      C_VARGS_BEGIN,
                                      "persistent"_Stz,
                                      "tiny"_Stz,
                                      aCN[eClass],
                                      nCntObj,
                                      nObjSize,
                                      nSumObjSize,
                                      sMagObj.szStz(),
                                      iPctUse,
                                      false,    // tiny heap is always the smallest
                                      C_VARGS_END);
    ASSERT(pRow);
    UNUSED(pRow);
  }

  if(nObjInHeap != AM::pHeaps()->Persistent.m_pTiny->nCurrAllocated())
  {
    s2K sReason;
    sReason.vSprint("In persistent tiny heap, number of objects (%lld) does not match current allocs (%lld)"_Stz,
                    nObjInHeap,
                    AM::pHeaps()->Persistent.m_pTiny->nCurrAllocated());

		pRowBufferWarn->pAllotRow(pWarnDef,
                            C_VARGS_BEGIN,
                            sReason.szStz(),
                            nullptr,
                            nullptr,
                            C_VARGS_END);
  }
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  nBlockSize = C_SIZE_SMALL_OBJECT;
  MagLib::vByteMag(nBlockSize,sMagHeap);
  AM::pHeaps()->Persistent.m_pSmall->vAnalyse(iCountObjects);
  nObjInHeap = 0;

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

    bool bChgHeap = (nObjSize < C_SIZE_TINY_OBJECT);

    Row* pRow = pRowBuffer->pAllotRow(pStreamDef,
                                      C_VARGS_BEGIN,
                                      "persistent"_Stz,
                                      "small"_Stz,
                                      aCN[eClass],
                                      nCntObj,
                                      nObjSize,
                                      nSumObjSize,
                                      sMagObj.szStz(),
                                      iPctUse,
                                      bChgHeap,
                                      C_VARGS_END);
    ASSERT(pRow);
    UNUSED(pRow);
  }

  if(nObjInHeap != AM::pHeaps()->Persistent.m_pSmall->nCurrAllocated())
  {
    s2K sReason;
    sReason.vSprint("In persistent small heap, number of objects (%lld) does not match current allocs (%lld)"_Stz,
                    nObjInHeap,
                    AM::pHeaps()->Persistent.m_pSmall->nCurrAllocated());

		pRowBufferWarn->pAllotRow(pWarnDef,
                            C_VARGS_BEGIN,
                            sReason.szStz(),
                            nullptr,
                            nullptr,
                            C_VARGS_END);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  nBlockSize = C_SIZE_MEDIUM_OBJECT;
  MagLib::vByteMag(nBlockSize,sMagHeap);
  AM::pHeaps()->Persistent.m_pMedium->vAnalyse(iCountObjects);
  nObjInHeap = 0;

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

    bool bChgHeap = (nObjSize < C_SIZE_TINY_OBJECT);

    Row* pRow = pRowBuffer->pAllotRow(pStreamDef,
                                      C_VARGS_BEGIN,
                                      "persistent"_Stz,
                                      "medium"_Stz,
                                      aCN[eClass],
                                      nCntObj,
                                      nObjSize,
                                      nSumObjSize,
                                      sMagObj.szStz(),
                                      iPctUse,
                                      bChgHeap,
                                      C_VARGS_END);
    ASSERT(pRow);
    UNUSED(pRow);
  }

  if(nObjInHeap != AM::pHeaps()->Persistent.m_pMedium->nCurrAllocated())
  {
    s2K sReason;
    sReason.vSprint("In persistent medium heap, number of objects (%lld) does not match current allocs (%lld)"_Stz,
                    nObjInHeap,
                    AM::pHeaps()->Persistent.m_pMedium->nCurrAllocated());

		pRowBufferWarn->pAllotRow(pWarnDef,
                            C_VARGS_BEGIN,
                            sReason.szStz(),
                            nullptr,
                            nullptr,
                            C_VARGS_END);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  nBlockSize = C_SIZE_LARGE_OBJECT;
  MagLib::vByteMag(nBlockSize,sMagHeap);
  AM::pHeaps()->Persistent.m_pLarge->vAnalyse(iCountObjects);
  nObjInHeap = 0;

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

    bool bChgHeap = (nObjSize < C_SIZE_TINY_OBJECT);

    Row* pRow = pRowBuffer->pAllotRow(pStreamDef,
                                      C_VARGS_BEGIN,
                                      "persistent"_Stz,
                                      "large"_Stz,
                                      aCN[eClass],
                                      nCntObj,
                                      nObjSize,
                                      nSumObjSize,
                                      sMagObj.szStz(),
                                      iPctUse,
                                      bChgHeap,
                                      C_VARGS_END);
    ASSERT(pRow);
    UNUSED(pRow);
  }

  if(nObjInHeap != AM::pHeaps()->Persistent.m_pLarge->nCurrAllocated())
  {
    s2K sReason;
    sReason.vSprint("In persistent large heap, number of objects (%lld) does not match current allocs (%lld)"_Stz,
                    nObjInHeap,
                    AM::pHeaps()->Persistent.m_pLarge->nCurrAllocated());

		pRowBufferWarn->pAllotRow(pWarnDef,
                            C_VARGS_BEGIN,
                            sReason.szStz(),
                            nullptr,
                            nullptr,
                            C_VARGS_END);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  nBlockSize = C_SIZE_TINY_OBJECT;
  MagLib::vByteMag(nBlockSize,sMagHeap);
  AM::pHeaps()->Transient.m_pTiny->vAnalyse(iCountObjects);
  nObjInHeap = 0;

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

    Row* pRow = pRowBuffer->pAllotRow(pStreamDef,
                                      C_VARGS_BEGIN,
                                      "transient"_Stz,
                                      "tiny"_Stz,
                                      aCN[eClass],
                                      nCntObj,
                                      nObjSize,
                                      nSumObjSize,
                                      sMagObj.szStz(),
                                      iPctUse,
                                      false,      // tiny heap is the smallest
                                      C_VARGS_END);
    ASSERT(pRow);
    UNUSED(pRow);
  }

  if(nObjInHeap != AM::pHeaps()->Transient.m_pTiny->nCurrAllocated())
  {
    s2K sReason;
    sReason.vSprint("In transient tiny heap, number of objects (%lld) does not match current allocs (%lld)"_Stz,
                    nObjInHeap,
                    AM::pHeaps()->Transient.m_pTiny->nCurrAllocated());

		pRowBufferWarn->pAllotRow(pWarnDef,
                            C_VARGS_BEGIN,
                            sReason.szStz(),
                            nullptr,
                            nullptr,
                            C_VARGS_END);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  nBlockSize = C_SIZE_SMALL_OBJECT;
  MagLib::vByteMag(nBlockSize,sMagHeap);
  AM::pHeaps()->Transient.m_pSmall->vAnalyse(iCountObjects);
  nObjInHeap = 0;

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

    bool bChgHeap = (nObjSize < C_SIZE_TINY_OBJECT);

    Row* pRow = pRowBuffer->pAllotRow(pStreamDef,
                                      C_VARGS_BEGIN,
                                      "transient"_Stz,
                                      "small"_Stz,
                                      aCN[eClass],
                                      nCntObj,
                                      nObjSize,
                                      nSumObjSize,
                                      sMagObj.szStz(),
                                      iPctUse,
                                      bChgHeap,
                                      C_VARGS_END);
    ASSERT(pRow);
    UNUSED(pRow);
  }

  if(nObjInHeap != AM::pHeaps()->Transient.m_pSmall->nCurrAllocated())
  {
    s2K sReason;
    sReason.vSprint("In transient small heap, number of objects (%lld) does not match current allocs (%lld)"_Stz,
                    nObjInHeap,
                    AM::pHeaps()->Transient.m_pSmall->nCurrAllocated());

		pRowBufferWarn->pAllotRow(pWarnDef,
                            C_VARGS_BEGIN,
                            sReason.szStz(),
                            nullptr,
                            nullptr,
                            C_VARGS_END);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  nBlockSize = C_SIZE_MEDIUM_OBJECT;
  MagLib::vByteMag(nBlockSize,sMagHeap);
  AM::pHeaps()->Transient.m_pMedium->vAnalyse(iCountObjects);
  nObjInHeap = 0;

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

    bool bChgHeap = (nObjSize < C_SIZE_TINY_OBJECT);

    Row* pRow = pRowBuffer->pAllotRow(pStreamDef,
                                      C_VARGS_BEGIN,
                                      "transient"_Stz,
                                      "medium"_Stz,
                                      aCN[eClass],
                                      nCntObj,
                                      nObjSize,
                                      nSumObjSize,
                                      sMagObj.szStz(),
                                      iPctUse,
                                      bChgHeap,
                                      C_VARGS_END);
    ASSERT(pRow);
    UNUSED(pRow);
  }

  if(nObjInHeap != AM::pHeaps()->Transient.m_pMedium->nCurrAllocated())
  {
    s2K sReason;
    sReason.vSprint("In transient medium heap, number of objects (%lld) does not match current allocs (%lld)"_Stz,
                    nObjInHeap,
                    AM::pHeaps()->Transient.m_pMedium->nCurrAllocated());

		pRowBufferWarn->pAllotRow(pWarnDef,
                            C_VARGS_BEGIN,
                            sReason.szStz(),
                            nullptr,
                            nullptr,
                            C_VARGS_END);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  nBlockSize = C_SIZE_LARGE_OBJECT;
  MagLib::vByteMag(nBlockSize,sMagHeap);
  AM::pHeaps()->Transient.m_pLarge->vAnalyse(iCountObjects);
  nObjInHeap = 0;

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

    bool bChgHeap = (nObjSize < C_SIZE_TINY_OBJECT);

    Row* pRow = pRowBuffer->pAllotRow(pStreamDef,
                                      C_VARGS_BEGIN,
                                      "transient"_Stz,
                                      "large"_Stz,
                                      aCN[eClass],
                                      nCntObj,
                                      nObjSize,
                                      nSumObjSize,
                                      sMagObj.szStz(),
                                      iPctUse,
                                      bChgHeap,
                                      C_VARGS_END);
    ASSERT(pRow);
    UNUSED(pRow);
  }

  if(nObjInHeap != AM::pHeaps()->Transient.m_pLarge->nCurrAllocated())
  {
    s2K sReason;
    sReason.vSprint("In transient large heap, number of objects (%lld) does not match current allocs (%lld)"_Stz,
                    nObjInHeap,
                    AM::pHeaps()->Transient.m_pLarge->nCurrAllocated());

		pRowBufferWarn->pAllotRow(pWarnDef,
                            C_VARGS_BEGIN,
                            sReason.szStz(),
                            nullptr,
                            nullptr,
                            C_VARGS_END);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // BufferPool Object Summary

  for (i8 i_Index = 0; i_Index < CN_ENUMS; i_Index++)
    iCountObjects[i_Index] = 0;


  for (i8 i_PoolBuffer = gpBufferPool->iBufferFirst();
       i_PoolBuffer <= gpBufferPool->iBufferLast();
       i_PoolBuffer++) {

    PoolBuffer* pBuffer = FileMap::pToPoolBuffer(i_PoolBuffer);
    iCountObjects[pBuffer->eGetClassNo()]++;
  }

  nBlockSize = C_SIZE_POOL_BUFFER;

  for (i8 i_Index = 0; i_Index < CN_ENUMS; i_Index++) {
    eCN eClass = (eCN)i_Index;

    if (iCountObjects[eClass] == 0)
      continue;

    i8 nObjSize    = gxaSizeHeap[eClass].m_nClassSize;
    i8 nSumObjSize = iCountObjects[eClass] * nObjSize;
    i8 iPctUse     = (nObjSize * 100) / nBlockSize;

    MagLib::vByteMag(nSumObjSize,sMagObj);

    Row* pRow = pRowBuffer->pAllotRow(pStreamDef,
                                      C_VARGS_BEGIN,
                                      "bufferpool"_Stz,
                                      0,
                                      aCN[eClass],
                                      iCountObjects[eClass],
                                      nObjSize,
                                      nSumObjSize,
                                      sMagObj.szStz(),
                                      iPctUse,
                                      false,
                                      C_VARGS_END);
    ASSERT(pRow);
    UNUSED(pRow);
  }
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
  
  for (i8 i_PoolBuffer = gpBufferPool->iBufferFirst();
       i_PoolBuffer <= gpBufferPool->iBufferLast();
       i_PoolBuffer++) {
    
    PoolBuffer* pBuffer = FileMap::pToPoolBuffer(i_PoolBuffer);
    
    eCN eClass = pBuffer->eGetClassNo();
        
    pPoolSnapshot->m_nObjects[eClass]++;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  AM::pHeaps()->Persistent.m_pTiny->vAnalyse(iCountObjects);

  for (i8 i_Index = 0; i_Index < CN_ENUMS; i_Index++) {
    eCN eClass = (eCN)i_Index;

    if (iCountObjects[eClass] == 0)
      continue;

    pPoolSnapshot->m_nObjects[eClass] += iCountObjects[eClass];
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  AM::pHeaps()->Persistent.m_pSmall->vAnalyse(iCountObjects);

  for (i8 i_Index = 0; i_Index < CN_ENUMS; i_Index++) {
    eCN eClass = (eCN)i_Index;

    if (iCountObjects[eClass] == 0)
      continue;

    pPoolSnapshot->m_nObjects[eClass] += iCountObjects[eClass];
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  AM::pHeaps()->Persistent.m_pMedium->vAnalyse(iCountObjects);

  for (i8 i_Index = 0; i_Index < CN_ENUMS; i_Index++) {
    eCN eClass = (eCN)i_Index;

    if (iCountObjects[eClass] == 0)
      continue;

    pPoolSnapshot->m_nObjects[eClass] += iCountObjects[eClass];
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  AM::pHeaps()->Persistent.m_pLarge->vAnalyse(iCountObjects);

  for (i8 i_Index = 0; i_Index < CN_ENUMS; i_Index++) {
    eCN eClass = (eCN)i_Index;

    if (iCountObjects[eClass] == 0)
      continue;

    pPoolSnapshot->m_nObjects[eClass] += iCountObjects[eClass];
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
}

/*----------------------------------------------------------------------------*/
/* validate all objects                                                       */
/*----------------------------------------------------------------------------*/
void PoolHelper::vObjectValidate(void)
{
  _static_

  AM::pHeaps()->Persistent.m_pTiny->vValidateContents();
  AM::pHeaps()->Persistent.m_pSmall->vValidateContents();
  AM::pHeaps()->Persistent.m_pMedium->vValidateContents();
  AM::pHeaps()->Persistent.m_pLarge->vValidateContents();
  AM::pHeaps()->Transient.m_pTiny->vValidateContents();
  AM::pHeaps()->Transient.m_pSmall->vValidateContents();
  AM::pHeaps()->Transient.m_pMedium->vValidateContents();
  AM::pHeaps()->Transient.m_pLarge->vValidateContents();
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
  
  i8 iPersistentTiny =
     AM::pHeaps()->Persistent.m_pTiny->nFreeChunks() * C_SIZE_TINY_OBJECT;

  i8 iPersistentSmall =
     AM::pHeaps()->Persistent.m_pSmall->nFreeChunks() * C_SIZE_SMALL_OBJECT;

  i8 iPersistentMedium =
     AM::pHeaps()->Persistent.m_pMedium->nFreeChunks() * C_SIZE_MEDIUM_OBJECT;

  i8 iPersistentLarge =
     AM::pHeaps()->Persistent.m_pLarge->nFreeChunks() * C_SIZE_LARGE_OBJECT;

  
  i8 iTotalFree = iTransientTiny
                    + iTransientSmall
                    + iTransientMedium
                    + iTransientLarge
         + iPersistentTiny
         + iPersistentSmall + iPersistentMedium + iPersistentLarge
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
  
  vTrace("  Persistent TinyHeap   (%6s) buffers %-6lld alloc %-6lld free %-6lld\n"_Stz,
         sMagTiny.azChar(),
         AM::pHeaps()->Persistent.m_pTiny->nBufferCount(),
         AM::pHeaps()->Persistent.m_pTiny->nCurrAllocated(),
         AM::pHeaps()->Persistent.m_pTiny->nFreeChunks());

  vTrace("  Persistent SmallHeap  (%6s) buffers %-6lld alloc %-6lld free %-6lld\n"_Stz,
         sMagSmall.azChar(),
         AM::pHeaps()->Persistent.m_pSmall->nBufferCount(),
         AM::pHeaps()->Persistent.m_pSmall->nCurrAllocated(),
         AM::pHeaps()->Persistent.m_pSmall->nFreeChunks());

  vTrace("  Persistent MediumHeap (%6s) buffers %-6lld alloc %-6lld free %-6lld\n"_Stz,
         sMagMedium.azChar(),
         AM::pHeaps()->Persistent.m_pMedium->nBufferCount(),
         AM::pHeaps()->Persistent.m_pMedium->nCurrAllocated(),
         AM::pHeaps()->Persistent.m_pMedium->nFreeChunks());

  vTrace("  Persistent LargeHeap  (%6s) buffers %-6lld alloc %-6lld free %-6lld\n"_Stz,
         sMagLarge.azChar(),
         AM::pHeaps()->Persistent.m_pLarge->nBufferCount(),
         AM::pHeaps()->Persistent.m_pLarge->nCurrAllocated(),
         AM::pHeaps()->Persistent.m_pLarge->nFreeChunks());
}

/*----------------------------------------------------------------------------*/
/* report on shm region                                                       */
/*----------------------------------------------------------------------------*/
void PoolHelper::vRegionSummary(RowBuffer* pRowBuffer, StreamDef* pStreamDef,
																RowBuffer* pRowBufferWarn, StreamDef* pWarnDef)
{
  _static_
  UNUSED(pRowBufferWarn);
  UNUSED(pWarnDef);

  Heap* pHeap;
  i8 iChunkSize;
  i8 iSumBufsUsed;
  i8 iSumChunksUsed;
  i8 iWastageKb;
  i8 iWastagePct;

  pHeap = AM::pHeaps()->Transient.m_pTiny;
  iChunkSize = C_SIZE_TINY_OBJECT;
  iSumBufsUsed = pHeap->nBufferCount() * C_SIZE_POOL_BUFFER / C_1KB;
  iSumChunksUsed = pHeap->nCurrAllocated() * iChunkSize / C_1KB;
  iWastageKb = (iSumChunksUsed == 0 ? 0 : (pHeap->nBufferCount() - (iSumChunksUsed * C_1KB / C_SIZE_POOL_BUFFER) - 1) * C_SIZE_POOL_BUFFER / C_1KB);
  iWastagePct = (iSumBufsUsed  == 0 ? 0 : 100 * iWastageKb / iSumBufsUsed);
  // Wastage is the size of the extra buffers that were needed when the chunks could have been packed
  // into a smaller number of buffers.

	pRowBuffer->pAllotRow(pStreamDef,
                        C_VARGS_BEGIN,
                        "transient"_Stz,
                        "tiny"_Stz,
                        iChunkSize / C_1KB,
                        pHeap->nBufferCount(),
                        iSumBufsUsed,
                        pHeap->nCurrAllocated(),
                        pHeap->nFreeChunks(),
                        iSumChunksUsed,
                        iWastageKb,
                        iWastagePct,
                        C_VARGS_END);


  pHeap = AM::pHeaps()->Transient.m_pSmall;
  iChunkSize = C_SIZE_SMALL_OBJECT;
  iSumBufsUsed = pHeap->nBufferCount() * C_SIZE_POOL_BUFFER / C_1KB;
  iSumChunksUsed = pHeap->nCurrAllocated() * iChunkSize / C_1KB;
  iWastageKb = (iSumChunksUsed == 0 ? 0 : (pHeap->nBufferCount() - (iSumChunksUsed * C_1KB / C_SIZE_POOL_BUFFER) - 1) * C_SIZE_POOL_BUFFER / C_1KB);
  iWastagePct = (iSumBufsUsed  == 0 ? 0 : 100 * iWastageKb / iSumBufsUsed);

	pRowBuffer->pAllotRow(pStreamDef,
                        C_VARGS_BEGIN,
                        "transient"_Stz,
                        "small"_Stz,
                        iChunkSize / C_1KB,
                        pHeap->nBufferCount(),
                        iSumBufsUsed,
                        pHeap->nCurrAllocated(),
                        pHeap->nFreeChunks(),
                        iSumChunksUsed,
                        iWastageKb,
                        iWastagePct,
                        C_VARGS_END);


  pHeap = AM::pHeaps()->Transient.m_pMedium;
  iChunkSize = C_SIZE_MEDIUM_OBJECT;
  iSumBufsUsed = pHeap->nBufferCount() * C_SIZE_POOL_BUFFER / C_1KB;
  iSumChunksUsed = pHeap->nCurrAllocated() * iChunkSize / C_1KB;
  iWastageKb = (iSumChunksUsed == 0 ? 0 : (pHeap->nBufferCount() - (iSumChunksUsed * C_1KB / C_SIZE_POOL_BUFFER) - 1) * C_SIZE_POOL_BUFFER / C_1KB);
  iWastagePct = (iSumBufsUsed  == 0 ? 0 : 100 * iWastageKb / iSumBufsUsed);

	pRowBuffer->pAllotRow(pStreamDef,
                        C_VARGS_BEGIN,
                        "transient"_Stz,
                        "medium"_Stz,
                        iChunkSize / C_1KB,
                        pHeap->nBufferCount(),
                        iSumBufsUsed,
                        pHeap->nCurrAllocated(),
                        pHeap->nFreeChunks(),
                        iSumChunksUsed,
                        iWastageKb,
                        iWastagePct,
                        C_VARGS_END);


  pHeap = AM::pHeaps()->Transient.m_pLarge;
  iChunkSize = C_SIZE_LARGE_OBJECT;
  iSumBufsUsed = pHeap->nBufferCount() * C_SIZE_POOL_BUFFER / C_1KB;
  iSumChunksUsed = pHeap->nCurrAllocated() * iChunkSize / C_1KB;
  iWastageKb = (iSumChunksUsed == 0 ? 0 : (pHeap->nBufferCount() - (iSumChunksUsed * C_1KB / C_SIZE_POOL_BUFFER) - 1) * C_SIZE_POOL_BUFFER / C_1KB);
  iWastagePct = (iSumBufsUsed  == 0 ? 0 : 100 * iWastageKb / iSumBufsUsed);

	pRowBuffer->pAllotRow(pStreamDef,
                        C_VARGS_BEGIN,
                        "transient"_Stz,
                        "large"_Stz,
                        iChunkSize / C_1KB,
                        pHeap->nBufferCount(),
                        iSumBufsUsed,
                        pHeap->nCurrAllocated(),
                        pHeap->nFreeChunks(),
                        iSumChunksUsed,
                        iWastageKb,
                        iWastagePct,
                        C_VARGS_END);


  pHeap = AM::pHeaps()->Persistent.m_pTiny;
  iChunkSize = C_SIZE_TINY_OBJECT;
  iSumBufsUsed = pHeap->nBufferCount() * C_SIZE_POOL_BUFFER / C_1KB;
  iSumChunksUsed = pHeap->nCurrAllocated() * iChunkSize / C_1KB;
  iWastageKb = (iSumChunksUsed == 0 ? 0 : (pHeap->nBufferCount() - (iSumChunksUsed * C_1KB / C_SIZE_POOL_BUFFER) - 1) * C_SIZE_POOL_BUFFER / C_1KB);
  iWastagePct = (iSumBufsUsed  == 0 ? 0 : 100 * iWastageKb / iSumBufsUsed);

	pRowBuffer->pAllotRow(pStreamDef,
                        C_VARGS_BEGIN,
                        "persistent"_Stz,
                        "tiny"_Stz,
                        iChunkSize / C_1KB,
                        pHeap->nBufferCount(),
                        iSumBufsUsed,
                        pHeap->nCurrAllocated(),
                        pHeap->nFreeChunks(),
                        iSumChunksUsed,
                        iWastageKb,
                        iWastagePct,
                        C_VARGS_END);


  pHeap = AM::pHeaps()->Persistent.m_pSmall;
  iChunkSize = C_SIZE_SMALL_OBJECT;
  iSumBufsUsed = pHeap->nBufferCount() * C_SIZE_POOL_BUFFER / C_1KB;
  iSumChunksUsed = pHeap->nCurrAllocated() * iChunkSize / C_1KB;
  iWastageKb = (iSumChunksUsed == 0 ? 0 : (pHeap->nBufferCount() - (iSumChunksUsed * C_1KB / C_SIZE_POOL_BUFFER) - 1) * C_SIZE_POOL_BUFFER / C_1KB);
  iWastagePct = (iSumBufsUsed  == 0 ? 0 : 100 * iWastageKb / iSumBufsUsed);

	pRowBuffer->pAllotRow(pStreamDef,
                        C_VARGS_BEGIN,
                        "persistent"_Stz,
                        "small"_Stz,
                        iChunkSize / C_1KB,
                        pHeap->nBufferCount(),
                        iSumBufsUsed,
                        pHeap->nCurrAllocated(),
                        pHeap->nFreeChunks(),
                        iSumChunksUsed,
                        iWastageKb,
                        iWastagePct,
                        C_VARGS_END);


  pHeap = AM::pHeaps()->Persistent.m_pMedium;
  iChunkSize = C_SIZE_MEDIUM_OBJECT;
  iSumBufsUsed = pHeap->nBufferCount() * C_SIZE_POOL_BUFFER / C_1KB;
  iSumChunksUsed = pHeap->nCurrAllocated() * iChunkSize / C_1KB;
  iWastageKb = (iSumChunksUsed == 0 ? 0 : (pHeap->nBufferCount() - (iSumChunksUsed * C_1KB / C_SIZE_POOL_BUFFER) - 1) * C_SIZE_POOL_BUFFER / C_1KB);
  iWastagePct = (iSumBufsUsed  == 0 ? 0 : 100 * iWastageKb / iSumBufsUsed);

	pRowBuffer->pAllotRow(pStreamDef,
                        C_VARGS_BEGIN,
                        "persistent"_Stz,
                        "medium"_Stz,
                        iChunkSize / C_1KB,
                        pHeap->nBufferCount(),
                        iSumBufsUsed,
                        pHeap->nCurrAllocated(),
                        pHeap->nFreeChunks(),
                        iSumChunksUsed,
                        iWastageKb,
                        iWastagePct,
                        C_VARGS_END);


  pHeap = AM::pHeaps()->Persistent.m_pLarge;
  iChunkSize = C_SIZE_LARGE_OBJECT;
  iSumBufsUsed = pHeap->nBufferCount() * C_SIZE_POOL_BUFFER / C_1KB;
  iSumChunksUsed = pHeap->nCurrAllocated() * iChunkSize / C_1KB;
  iWastageKb = (iSumChunksUsed == 0 ? 0 : (pHeap->nBufferCount() - (iSumChunksUsed * C_1KB / C_SIZE_POOL_BUFFER) - 1) * C_SIZE_POOL_BUFFER / C_1KB);
  iWastagePct = (iSumBufsUsed  == 0 ? 0 : 100 * iWastageKb / iSumBufsUsed);

	pRowBuffer->pAllotRow(pStreamDef,
                        C_VARGS_BEGIN,
                        "persistent"_Stz,
                        "large"_Stz,
                        iChunkSize / C_1KB,
                        pHeap->nBufferCount(),
                        iSumBufsUsed,
                        pHeap->nCurrAllocated(),
                        pHeap->nFreeChunks(),
                        iSumChunksUsed,
                        iWastageKb,
                        iWastagePct,
                        C_VARGS_END);
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
    
    
    eHT eHeap = gxaSizeHeap[eClass].m_eWhichHeap;
    i8  iSize = gxaSizeHeap[eClass].m_nClassSize;
    
    if (AM::AM::TRACE::bVerbose(C_VB_BOOT))
      vTrace("[%lld] class %18s heap %18s size %lldB\n"_Stz,
             i_Class,aCN[eClass],aHT[eHeap],iSize);
    

    // sanity check the class names seem roughly in order
    switch (eClass) {
      case CN_NONE:  RASSERT(aCN[CN_NONE]->bSame("NONE"_Stz));  break;
      case CN__A__:  RASSERT(aCN[CN__A__]->bSame("_A__"_Stz));  break;
      case CN__B__:  RASSERT(aCN[CN__B__]->bSame("_B__"_Stz));  break;
      case CN__C__:  RASSERT(aCN[CN__C__]->bSame("_C__"_Stz));  break;
      case CN__D__:  RASSERT(aCN[CN__D__]->bSame("_D__"_Stz));  break;
      case CN__E__:  RASSERT(aCN[CN__E__]->bSame("_E__"_Stz));  break;
      case CN__F__:  RASSERT(aCN[CN__F__]->bSame("_F__"_Stz));  break;
      case CN__G__:  RASSERT(aCN[CN__G__]->bSame("_G__"_Stz));  break;
      case CN__H__:  RASSERT(aCN[CN__H__]->bSame("_H__"_Stz));  break;
      case CN__I__:  RASSERT(aCN[CN__I__]->bSame("_I__"_Stz));  break;
      case CN__J__:  RASSERT(aCN[CN__J__]->bSame("_J__"_Stz));  break;
      case CN__K__:  RASSERT(aCN[CN__K__]->bSame("_K__"_Stz));  break;
      case CN__L__:  RASSERT(aCN[CN__L__]->bSame("_L__"_Stz));  break;
      case CN__M__:  RASSERT(aCN[CN__M__]->bSame("_M__"_Stz));  break;
      case CN__N__:  RASSERT(aCN[CN__N__]->bSame("_N__"_Stz));  break;
      case CN__O__:  RASSERT(aCN[CN__O__]->bSame("_O__"_Stz));  break;
      case CN__P__:  RASSERT(aCN[CN__P__]->bSame("_P__"_Stz));  break;
      case CN__Q__:  RASSERT(aCN[CN__Q__]->bSame("_Q__"_Stz));  break;
      case CN__R__:  RASSERT(aCN[CN__R__]->bSame("_R__"_Stz));  break;
      case CN__S__:  RASSERT(aCN[CN__S__]->bSame("_S__"_Stz));  break;
      case CN__T__:  RASSERT(aCN[CN__T__]->bSame("_T__"_Stz));  break;
      case CN__U__:  RASSERT(aCN[CN__U__]->bSame("_U__"_Stz));  break;
      case CN__V__:  RASSERT(aCN[CN__V__]->bSame("_V__"_Stz));  break;
      case CN__W__:  RASSERT(aCN[CN__W__]->bSame("_W__"_Stz));  break;
      case CN__X__:  RASSERT(aCN[CN__X__]->bSame("_X__"_Stz));  break;
      case CN__Y__:  RASSERT(aCN[CN__Y__]->bSame("_Y__"_Stz));  break;
      case CN__Z__:  RASSERT(aCN[CN__Z__]->bSame("_Z__"_Stz));  break;
      case CN_ENUMS: RASSERT(aCN[CN_ENUMS]->bSame("ENUMS"_Stz)); break;
      default: break;
    }
    
    RASSERT(eClass == gxaSizeHeap[eClass].m_eClassType);
    
    switch(eHeap) {
      case HT_INVALID:
        break;
        
      // transient -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
        
      case HT_T_TINY:
        if (iSize >= C_SIZE_TINY_OBJECT) {
          vTrace("Heaps::vValidateEnums: transient tiny class %lld %s is too big\n"_Stz,
                 eClass,aCN[eClass]);
          vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
                 "Heaps::vValidateEnums: transient tiny class %lld %s is too big\n"_Stz,
                 eClass,aCN[eClass]);
        }
        break;
        
      case HT_T_SMALL:
        if (iSize >= C_SIZE_SMALL_OBJECT) {
          vTrace("Heaps::vValidateEnums: transient small class %lld %s is too big\n"_Stz,
                  eClass,aCN[eClass]);
          vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
                 "Heaps::vValidateEnums: transient small class %lld %s is too big\n"_Stz,
                 eClass,aCN[eClass]);
        }
        break;
        
      case HT_T_MEDIUM:
        if (iSize >= C_SIZE_MEDIUM_OBJECT) {
          vTrace("Heaps::vValidateEnums: transient medium class %lld %s is too big\n"_Stz,
                 eClass,aCN[eClass]);
          vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
                 "Heaps::vValidateEnums: transient medium class %lld %s is too big\n"_Stz,
                 eClass,aCN[eClass]);
        }
        break;
        
      case HT_T_LARGE:
        if (iSize >= C_SIZE_LARGE_OBJECT) {
          vTrace("Heaps::vValidateEnums: transient large class %lld %s is too big\n"_Stz,
                 eClass,aCN[eClass]);
          vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
                 "Heaps::vValidateEnums: transient large class %lld %s is too big\n"_Stz,
                 eClass,aCN[eClass]);
        }
        break;
               // persistent -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

      case HT_P_TINY:
        if (iSize >= C_SIZE_SMALL_OBJECT) {
          vTrace("Heaps::vValidateEnums: persistent tiny class %lld %s is too big\n"_Stz,
                 eClass,aCN[eClass]);
          vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
                 "Heaps::vValidateEnums: persistent tiny class %lld %s is too big\n"_Stz,
                 eClass,aCN[eClass]);
        }
        break;
        
      case HT_P_SMALL:
        if (iSize >= C_SIZE_SMALL_OBJECT) {
          vTrace("Heaps::vValidateEnums: persistent small class %lld %s is too big\n"_Stz,
                 eClass,aCN[eClass]);
          vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
                 "Heaps::vValidateEnums: persistent small class %lld %s is too big\n"_Stz,
                 eClass,aCN[eClass]);
        }
        break;
        
      case HT_P_MEDIUM:
        if (iSize >= C_SIZE_MEDIUM_OBJECT) {
          vTrace("Heaps::vValidateEnums: persistent medium class %lld %s is too big\n"_Stz,
                 eClass,aCN[eClass]);
          vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
                 "Heaps::vValidateEnums: persistent medium class %lld %s is too big\n"_Stz,
                 eClass,aCN[eClass]);
        }
        break;
        
      case HT_P_LARGE:
        if (iSize >= C_SIZE_LARGE_OBJECT) {
          vTrace("Heaps::vValidateEnums: persistent large class %lld %s is too big\n"_Stz,
                 eClass,aCN[eClass]);
          vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
                 "Heaps::vValidateEnums: persistent large class %lld %s is too big\n"_Stz,
                 eClass,aCN[eClass]);
        }
        break;
        // virtual -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

      case HT_V_TINY:
        if (iSize >= C_SIZE_SMALL_OBJECT) {
          vTrace("Heaps::vValidateEnums: virtual tiny class %lld %s is too big\n"_Stz,
                 eClass,aCN[eClass]);
          vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
                 "Heaps::vValidateEnums: virtual tiny class %lld %s is too big\n"_Stz,
                 eClass,aCN[eClass]);
        }
        break;

      case HT_V_SMALL:
        if (iSize >= C_SIZE_SMALL_OBJECT) {
          vTrace("Heaps::vValidateEnums: virtual small class %lld %s is too big\n"_Stz,
                 eClass,aCN[eClass]);
          vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
                 "Heaps::vValidateEnums: virtual small class %lld %s is too big\n"_Stz,
                 eClass,aCN[eClass]);
        }
        break;

      case HT_V_MEDIUM:
        if (iSize >= C_SIZE_MEDIUM_OBJECT) {
          vTrace("Heaps::vValidateEnums: virtual medium class %lld %s is too big\n"_Stz,
                 eClass,aCN[eClass]);
          vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
                 "Heaps::vValidateEnums: virtual medium class %lld %s is too big\n"_Stz,
                 eClass,aCN[eClass]);
        }
        break;

      case HT_V_LARGE:
        if (iSize >= C_SIZE_LARGE_OBJECT) {
          vTrace("Heaps::vValidateEnums: virtual large class %lld %s is too big\n"_Stz,
                 eClass,aCN[eClass]);
          vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
                 "Heaps::vValidateEnums: virtual large class %lld %s is too big\n"_Stz,
                 eClass,aCN[eClass]);
        }
        break;
                
      default:
        vTrace("Heaps::vValidateEnums: Invalid which heap: eHeap %lld\n"_Stz,
               eHeap);

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
  
  for (i8 i_PoolBuffer = gpBufferPool->iBufferFirst();
       i_PoolBuffer <= gpBufferPool->iBufferLast();
       i_PoolBuffer++) {
    
    PoolBuffer* pBuffer = FileMap::pToPoolBuffer(i_PoolBuffer);
    
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
