/*============================================================================*/
/*  BPA.cpp - buffer pool analyzer                                            */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;
extern Heap* gpMediumHeap;
extern Heap* gpSmallHeap;
extern Heap* gpJumboHeap;
extern Arguments gxArguments;

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
: Header(CN_BPA)
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
  
  vTrace("%s%p %-12s: ",pDoubleTab(),pBuffer,pCN[eClass]);

  (pDumpFun)(pBuffer->eClass(), pBuffer, pSingleTab());
  
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
  
  vTrace("\n");
  vTrace("%sObjects in %s Heap:\n",
         pSingleTab(),
         pHeap->iObjectSize() == C_SIZE_SMALL_OBJECT ? "Small" : "Medium");
  
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
      
      vTrace("%s%p %-12s: ",pDoubleTab(),pHeap,pCN[eClass]);

      (pDumpFun)(eClass, pChunk, pDoubleTab());
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
  
  vTrace("\n");
  vTrace("%sObjects in Buffer Pool:\n",pSingleTab());
  
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
  for (i8 index = 0; index < CN_ENUMS; index++)
    iCountObjects[index] = 0;
  
  bn nBuffer = 0;
  bn nBufferCount = gpBufferPool->m_nBuffers - 1;
  
  for (bn nBuffer = gpBufferPool->nBufferFirst();
       nBuffer <= nBufferCount;
       nBuffer++) {
    
    Buffer* pBuffer = gpBufferPool->pToBuffer(nBuffer);
    iCountObjects[pBuffer->eClass()]++;
  }
  
  vTrace("\n");
  vTrace("  BufferPool Object Summary:\n");
  for (i8 index = 0; index < CN_ENUMS; index++) {
    eCN eClass = (eCN)index;
    
    if (iCountObjects[eClass] == 0)
      continue;
    
    vTrace("  %12s %-6lld \n", pCN[eClass], iCountObjects[eClass]);
  }
  
  gpSmallHeap->vAnalyse(iCountObjects);
  
  vTrace("\n");
  vTrace("  SmallHeap Object Summary:\n");
  for (i8 index = 0; index < CN_ENUMS; index++) {
    eCN eClass = (eCN)index;
    
    if (iCountObjects[eClass] == 0)
      continue;
    
    vTrace("  %12s %-6lld \n", pCN[eClass], iCountObjects[eClass]);
  }
  
  gpMediumHeap->vAnalyse(iCountObjects);
  
  vTrace("\n");
  vTrace("  MediumHeap Object Summary:\n");
  for (i8 index = 0; index < CN_ENUMS; index++) {
    eCN eClass = (eCN)index;
    
    if (iCountObjects[eClass] == 0)
      continue;
    
    vTrace("  %12s %-6lld \n", pCN[eClass], iCountObjects[eClass]);
  }
  
  for (i8 index = 0; index < CN_ENUMS; index++)
    iCountObjects[index] = 0;
  
//  gpJumboHeap->vAnalyse(iCountObjects);
//  
//  vTrace("\n");
//  vTrace("  JumboHeap Object s:\n");
//  for (i8 index = 0; index < CN_ENUMS; index++) {
//    eCN eClass = (eCN)index;
//    
//    if (iCountObjects[eClass] == 0)
//      continue;
//    
//    vTrace("  %12s %-6lld \n", pCN[eClass], iCountObjects[eClass]);
//  }
  
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
  
  vTrace("\n");
  vTrace("****************************************************************\n");
  vTrace("* Buffer Pool Analysis                                         *\n");
  vTrace("****************************************************************\n");
  
  vRegionSummary();
  vObjectSummary();
  vResourceSummary();
  
  vTrace("\n");
  
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
  
  vTrace("\n");
  vTrace("****************************************************************\n");
  vTrace("* Buffer Pool Dump                                             *\n");
  vTrace("****************************************************************\n");
  
  vRegionSummary();
  vObjectSummary();
  vObjectList();
  vResourceSummary();
  
  vTrace("\n");
  
  _postcondition_(_MAGIC_,CN_BPA,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* report on shm region                                                       */
/*----------------------------------------------------------------------------*/
void BPA::vRegionSummary(void)
{
  _precondition_(_MAGIC_,CN_BPA,_FOOTER_);
  
  // get magnitudes of the various buffer pools
  s32 sMagSmall;  Magnitudes::vMagnitude(C_SIZE_SMALL_OBJECT, sMagSmall.a());
  s32 sMagMedium; Magnitudes::vMagnitude(C_SIZE_MEDIUM_OBJECT,sMagMedium.a());
  s32 sMagLarge;  Magnitudes::vMagnitude(C_SIZE_LARGE_OBJECT, sMagLarge.a());
  s32 sMagJumbo;  Magnitudes::vMagnitude(C_SIZE_JUMBO_OBJECT, sMagJumbo.a());
  
  vTrace("\n");
  vTrace("  Region %s %p size %lldB epoch %lld\n",
         gxArguments.m_sRegionName.a(),
         gpBufferPool,
         gxArguments.m_iRegionSize,
         gpAttachMap->m_iEpoch);
  
  vTrace("\n");
  
  vTrace("  BufferPool (%4s) buffers %-6lld alloc %-6lld free %-6lld \n",
         sMagLarge.a(),
         gpBufferPool->m_nBuffers,
         gpBufferPool->m_iAllocCount,
         gpBufferPool->m_iFreeCount
         );
  
  vTrace("   SmallHeap (%4s) buffers %-6lld alloc %-6lld free %-6lld\n",
         sMagSmall.a(),
         gpSmallHeap->iBufferCount(),
         gpSmallHeap->iAllocCount(),
         gpSmallHeap->iFreeCount());
  
  vTrace("  MediumHeap (%4s) buffers %-6lld alloc %-6lld free %-6lld\n",
         sMagMedium.a(),
         gpMediumHeap->iBufferCount(),
         gpMediumHeap->iAllocCount(),
         gpMediumHeap->iFreeCount());
  
  vTrace("   JumboHeap (%4s) buffers %-6lld alloc %-6lld free %-6lld\n",
         sMagJumbo.a(),
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
  
  vTrace("\n");
  vTrace("****************************************************************\n");
  vTrace(" Resources: dir %lld file %lld shm %lld socket %lld thread %lld\n",
         *gzDirOpenCount,
         *gzFileOpenCount,
         *gzShmOpenCount,
         *gzSocketOpenCount,
         *gzThreadOpenCount);
  vTrace("****************************************************************\n");
  
  _postcondition_(_MAGIC_,CN_BPA,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* BPA::pFooter(void)
{
  _precondition_(_MAGIC_,CN_BPA);
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
