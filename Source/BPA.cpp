/*============================================================================*/
/*  BPA.cpp - buffer pool analyzer                                            */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;
extern Heap* gpMediumHeap;
extern Heap* gpSmallHeap;
extern Heap* gpJumboHeap;

extern Atomic gzDirOpenCount;
extern Atomic gzFileOpenCount;
extern Atomic gzShmOpenCount;
extern Atomic gzSocketOpenCount;
extern Atomic gzThreadOpenCount;

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
BPA::~BPA(void)
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
//      pBuffer->eClass() == CN_SMALLHEAP ) {
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
  
  Buffer* pMediumHeap = nullptr;
  Buffer* pSmallHeap  = nullptr;
  
  bn nBufferCount = gpBufferPool->m_nBuffers - 1;
  for (bn nBuffer = gpBufferPool->nBufferFirst();
       nBuffer <= nBufferCount;
       nBuffer++) {
    
    Buffer* pBuffer = gpBufferPool->pToBuffer(nBuffer);
    
    if (CN_MEDIUMHEAP == pBuffer->eClass())
      pMediumHeap = pBuffer;
    
    if (CN_SMALLHEAP == pBuffer->eClass())
      pSmallHeap = pBuffer;
    
    vBufferDump(pBuffer);
  }
  
  if (pMediumHeap)
    vHeapDump(reinterpret_cast<Heap*>(pMediumHeap));
  
  if (pSmallHeap)
    vHeapDump(reinterpret_cast<Heap*>(pSmallHeap));
  
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
  
  gpSmallHeap->vAnalyse(iCountObjects);
  
  vTrace("\n"_Stz);
  vTrace("  SmallHeap Object Summary:\n"_Stz);
  for (i8 i_Index = 0; i_Index < CN_ENUMS; i_Index++) {
    eCN eClass = (eCN)i_Index;
    
    if (iCountObjects[eClass] == 0)
      continue;
    
    vTrace("  %12s %-6lld \n"_Stz, aCN[eClass], iCountObjects[eClass]);
  }
  
  gpMediumHeap->vAnalyse(iCountObjects);
  
  vTrace("\n"_Stz);
  vTrace("  MediumHeap Object Summary:\n"_Stz);
  for (i8 i_Index = 0; i_Index < CN_ENUMS; i_Index++) {
    eCN eClass = (eCN)i_Index;
    
    if (iCountObjects[eClass] == 0)
      continue;
    
    vTrace("  %12s %-6lld \n"_Stz, aCN[eClass], iCountObjects[eClass]);
  }
  
  for (i8 i_Index = 0; i_Index < CN_ENUMS; i_Index++)
    iCountObjects[i_Index] = 0;
  
//  gpJumboHeap->vAnalyse(iCountObjects);
//  
//  vTrace("\n"_Stz);
//  vTrace("  JumboHeap Object s:\n"_Stz);
//  for (i8 i_Index = 0; index < CN_ENUMS; i_Index++) {
//    eCN eClass = (eCN)i_Index;
//    
//    if (iCountObjects[eClass] == 0)
//      continue;
//    
//    vTrace("  %12s %-6lld \n"_Stz, aCN[eClass], iCountObjects[eClass]);
//  }
  
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
  
  bn nBuffer = 0;
  bn nBufferCount = gpBufferPool->m_nBuffers - 1;
  
  for (bn nBuffer = gpBufferPool->nBufferFirst();
       nBuffer <= nBufferCount;
       nBuffer++) {
    
    Buffer* pBuffer = gpBufferPool->pToBuffer(nBuffer);
    pCountObjects[pBuffer->eClass()]++;
  }
  
  gpSmallHeap->vAnalyse(iCountObjects);
  for (i8 i_Index = 0; i_Index < CN_ENUMS; i_Index++) {
    eCN eClass = (eCN)i_Index;
    
    if (iCountObjects[eClass] == 0)
      continue;
    
    pCountObjects[eClass] += iCountObjects[eClass];
  }
  
  gpMediumHeap->vAnalyse(iCountObjects);
  for (i8 i_Index = 0; i_Index < CN_ENUMS; i_Index++) {
    eCN eClass = (eCN)i_Index;
    
    if (iCountObjects[eClass] == 0)
      continue;
    
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
/* report on shm region                                                       */
/*----------------------------------------------------------------------------*/
void BPA::vRegionSummary()
{
  _precondition_(_MAGIC_,CN_BPA,_FOOTER_);
  
  // get magnitudes of the various buffer pools
  s32 sMagSmall;  Magnitudes::vMagnitude(C_SIZE_SMALL_OBJECT, sMagSmall);
  s32 sMagMedium; Magnitudes::vMagnitude(C_SIZE_MEDIUM_OBJECT,sMagMedium);
  s32 sMagLarge;  Magnitudes::vMagnitude(C_SIZE_LARGE_OBJECT, sMagLarge);
  s32 sMagJumbo;  Magnitudes::vMagnitude(C_SIZE_JUMBO_OBJECT, sMagJumbo);
  
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
  
  vTrace("   SmallHeap (%4s) buffers %-6lld alloc %-6lld free %-6lld\n"_Stz,
         sMagSmall.aChar(),
         gpSmallHeap->iBufferCount(),
         gpSmallHeap->iAllocCount(),
         gpSmallHeap->iFreeCount());
  
  vTrace("  MediumHeap (%4s) buffers %-6lld alloc %-6lld free %-6lld\n"_Stz,
         sMagMedium.aChar(),
         gpMediumHeap->iBufferCount(),
         gpMediumHeap->iAllocCount(),
         gpMediumHeap->iFreeCount());
  
  vTrace("   JumboHeap (%4s) buffers %-6lld alloc %-6lld free %-6lld\n"_Stz,
         sMagJumbo.aChar(),
         gpJumboHeap->iBufferCount(),
         gpJumboHeap->iAllocCount(),
         gpJumboHeap->iFreeCount());
  
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
         *gzDirOpenCount,
         *gzFileOpenCount,
         *gzShmOpenCount,
         *gzSocketOpenCount,
         *gzThreadOpenCount);
  vTrace("****************************************************************\n"_Stz);
  
  _postcondition_(_MAGIC_,CN_BPA,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* list all words created since last epoch                                    */
/*----------------------------------------------------------------------------*/
void BPA::vWordList(i8 iEpoch)
{
  _precondition_(_MAGIC_,CN_BPA,_FOOTER_);
  
  Buffer* pMediumHeap = nullptr;
  Buffer* pSmallHeap  = nullptr;
  
  bn nBufferCount = gpBufferPool->m_nBuffers - 1;
  for (bn nBuffer = gpBufferPool->nBufferFirst();
       nBuffer <= nBufferCount;
       nBuffer++) {
    
    Buffer* pBuffer = gpBufferPool->pToBuffer(nBuffer);
    
    if (CN_MEDIUMHEAP == pBuffer->eClass())
    pMediumHeap = pBuffer;
    
    if (CN_SMALLHEAP == pBuffer->eClass())
    pSmallHeap = pBuffer;
  }
  
  if (pMediumHeap)
    vHeapDump(reinterpret_cast<Heap*>(pMediumHeap), iEpoch);
  
  if (pSmallHeap)
    vHeapDump(reinterpret_cast<Heap*>(pSmallHeap), iEpoch);
  
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
