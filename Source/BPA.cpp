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
  
  vTrace("%s%p %-12s: ",pSingleTab(),pBuffer,pCN[eClass]);

  (pDumpFun)(pBuffer->eClass(), pBuffer, pSingleTab());
  
//  if (pBuffer->eClass() == CN_MEDIUMHEAP ||
//      pBuffer->eClass() == CN_SMALLHEAP ) {
//    vHeapDump(reinterpret_cast<Heap*>(pBuffer));
//  }
}

/*----------------------------------------------------------------------------*/
/* dump this heap buffer                                                      */
/*----------------------------------------------------------------------------*/
void BPA::vHeapDump(Heap* pHeap)
{
  _precondition_(_MAGIC_,CN_BPA,_FOOTER_);
  
  HeapBuffer* pHeapBuffer;
  
  vTrace("\n");
  vTrace("%s%s Heap:\n",
         pTab(),
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
      
      vTrace("%s%p %-16s: ",pDoubleTab(),pHeap,pCN[eClass]);

      (pDumpFun)(eClass, pChunk, pDoubleTab());
    }
  }
  
  pHeap->vDropLock();
}

/*----------------------------------------------------------------------------*/
/* analyze the buffer pool                                                    */
/*----------------------------------------------------------------------------*/
void BPA::vPoolAnalyze(void)
{
  _precondition_(_MAGIC_,CN_BPA,_FOOTER_);
  
  // get magnitudes of the various buffer pools
  s32 sMagSmall;  Magnitudes::vMagnitude(C_SIZE_SMALL_OBJECT, sMagSmall.a());
  s32 sMagMedium; Magnitudes::vMagnitude(C_SIZE_MEDIUM_OBJECT,sMagMedium.a());
  s32 sMagLarge;  Magnitudes::vMagnitude(C_SIZE_LARGE_OBJECT, sMagLarge.a());
  s32 sMagJumbo;  Magnitudes::vMagnitude(C_SIZE_JUMBO_OBJECT, sMagJumbo.a());
  
  // increment the epoch
  gpAttachMap->m_iEpoch++;
  
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
  vTrace("****************************************************************\n");
  vTrace("* Buffer Pool Analysis                                         *\n");
  vTrace("****************************************************************\n");
  vTrace("\n");
  
  vTrace("  Region %s %p regionSize %lld\n",
         gxArguments.m_sRegionName.a(),
         gpBufferPool,
         gxArguments.m_iRegionSize);
  
  vTrace("\n");
  
  vTrace("Epoch: %lld\n", gpAttachMap->m_iEpoch);
  
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
  
  vTrace("\n");
  vTrace("  BufferPool Objects:\n");
  for (i8 index = 0; index < CN_ENUMS; index++) {
    eCN eClass = (eCN)index;
    
    if (iCountObjects[eClass] == 0)
      continue;
    
    vTrace("  %16s %-6lld \n", pCN[eClass], iCountObjects[eClass]);
  }
  
  for (i8 index = 0; index < CN_ENUMS; index++)
    iCountObjects[index] = 0;
  
  gpSmallHeap->vAnalyse(iCountObjects);
  
  vTrace("\n");
  vTrace("  SmallHeap Objects:\n");
  for (i8 index = 0; index < CN_ENUMS; index++) {
    eCN eClass = (eCN)index;
    
    if (iCountObjects[eClass] == 0)
      continue;
    
    vTrace("  %16s %-6lld \n", pCN[eClass], iCountObjects[eClass]);
  }
  
  for (i8 index = 0; index < CN_ENUMS; index++)
    iCountObjects[index] = 0;
  
  gpMediumHeap->vAnalyse(iCountObjects);
  
  vTrace("\n");
  vTrace("  MediumHeap Objects:\n");
  for (i8 index = 0; index < CN_ENUMS; index++) {
    eCN eClass = (eCN)index;
    
    if (iCountObjects[eClass] == 0)
      continue;
    
    vTrace("  %16s %-6lld \n", pCN[eClass], iCountObjects[eClass]);
  }
  
  for (i8 index = 0; index < CN_ENUMS; index++)
    iCountObjects[index] = 0;
  
  gpJumboHeap->vAnalyse(iCountObjects);
  
  vTrace("\n");
  vTrace("  JumboHeap Objects:\n");
  for (i8 index = 0; index < CN_ENUMS; index++) {
    eCN eClass = (eCN)index;
    
    if (iCountObjects[eClass] == 0)
      continue;
    
    vTrace("  %16s %-6lld \n", pCN[eClass], iCountObjects[eClass]);
  }
  
  vTrace("\n");
  vTrace("****************************************************************\n");
  vTrace(" dir %lld file %lld shm %lld socket %lld thread %lld\n",
         *gzDirOpenCount,
         *gzFileOpenCount,
         *gzShmOpenCount,
         *gzSocketOpenCount,
         *gzThreadOpenCount);
  vTrace("****************************************************************\n");
  vTrace("\n");
  
  _postcondition_(_MAGIC_,CN_BPA,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* dump the buffer pool objects                                               */
/*----------------------------------------------------------------------------*/
void BPA::vPoolDump(void)
{
  _precondition_(_MAGIC_,CN_BPA,_FOOTER_);
  
  vTrace("\n");
  vTrace("****************************************************************\n");
  vTrace("* Buffer Pool Dump                                             *\n");
  vTrace("****************************************************************\n");
  vTrace("\n");
  
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
  
  vTrace("  Region %s %p regionSize %lld \n",
         gxArguments.m_sRegionName.a(),
         gpBufferPool,
         gxArguments.m_iRegionSize);
  
  vTrace("\n");
  
  vTrace("  BufferPool buffers %-6lld alloc %-6lld free %-6lld \n",
         gpBufferPool->m_nBuffers,
         gpBufferPool->m_iAllocCount,
         gpBufferPool->m_iFreeCount
         );
  
  vTrace("\n");
  vTrace("  BufferPool Objects:\n");
  for (i8 index = 0; index < CN_ENUMS; index++) {
    eCN eClass = (eCN)index;
    
    if (iCountObjects[eClass] == 0)
      continue;
    
    vTrace("%s %s %-6lld \n", pSingleTab(), pCN[eClass], iCountObjects[eClass]);
  }
  vTrace("\n");
  
  Buffer* pMediumHeap = nullptr;
  Buffer* pSmallHeap  = nullptr;
  
  for (bn nBuffer = gpBufferPool->nBufferFirst();
       nBuffer <= nBufferCount;
       nBuffer++) {

    Buffer* pBuffer = gpBufferPool->pToBuffer(nBuffer);
    
    if (CN_MEDIUMHEAP == pBuffer->eClass())
      pMediumHeap = pBuffer;

    if (CN_SMALLHEAP == pBuffer->eClass())
      pSmallHeap = pBuffer;
    
    vBufferDump(pBuffer);
    iCountObjects[pBuffer->eClass()]++;
  }
  
  if (pMediumHeap)
    vHeapDump(reinterpret_cast<Heap*>(pMediumHeap));

  if (pSmallHeap)
    vHeapDump(reinterpret_cast<Heap*>(pSmallHeap));


  vTrace("\n");
  vTrace("****************************************************************\n");
  vTrace(" dir %lld file %lld shm %lld socket %lld thread %lld\n",
         *gzDirOpenCount,
         *gzFileOpenCount,
         *gzShmOpenCount,
         *gzSocketOpenCount,
         *gzThreadOpenCount);
  vTrace("****************************************************************\n");
  vTrace("\n");
  
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
