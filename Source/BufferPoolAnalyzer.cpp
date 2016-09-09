/*============================================================================*/
/*  BufferPoolAnalyzer.cpp - buffer pool analyzer                             */
/*============================================================================*/

#include "StorageEngine.h"

extern Heap* gpMediumHeap;
extern Heap* gpSmallHeap;
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
BufferPoolAnalyzer::BufferPoolAnalyzer(void)
: Header(CN_BUFFERPOOLANALYZER)
{
  _magic_();
  
  Memory::vCheckInitialised(this,sizeof(BufferPoolAnalyzer));
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
BufferPoolAnalyzer::~BufferPoolAnalyzer(void)
{
  _magic_();
  
}

/*----------------------------------------------------------------------------*/
/* analyze the buffer pool                                                    */
/*----------------------------------------------------------------------------*/
void BufferPoolAnalyzer::vAnalyze(void)
{
  _precondition_(_MAGIC_,CN_BUFFERPOOLANALYZER,_FOOTER_);
  
  i8 iCountObjects[CN_ENUMS];
  for (i8 index = 0; index < CN_ENUMS; index++)
    iCountObjects[index] = 0;
  
  bn nBuffer = 0;
  bn nBufferCount = gpBufferPool->m_iBufferCount - 1;
  
  for (bn nBuffer = gpBufferPool->nBufferFirst();
       nBuffer <= nBufferCount;
       nBuffer++) {
    
    Buffer* pBuffer = gpBufferPool->pToBuffer(nBuffer);
    iCountObjects[pBuffer->eClass()]++;
  }
  
  vTrace("**************************************************************\n");
  vTrace("* Buffer Pool Analysis                                       *\n");
  vTrace("**************************************************************\n");
  
  vTrace("  Region %s %p\n",gxArguments.m_sRegionName.a(),gpBufferPool);
  vTrace("  RegionSize %lld BufferSize %lld \n",
         gpBufferPool->m_iRegionSize,
         gpBufferPool->m_iBufferSize);
  
  vTrace("\n");
  
  vTrace("  BufferPool buffers %-6lld alloc %-6lld free %-6lld \n",
         gpBufferPool->m_iBufferCount,
         gpBufferPool->m_iAllocCount,
         gpBufferPool->m_iFreeCount
         );
  
  vTrace("   SmallHeap buffers %-6lld alloc %-6lld free %-6lld\n",
         gpSmallHeap->iBufferCount(),
         gpSmallHeap->iAllocCount(),
         gpSmallHeap->iFreeCount());
  
  vTrace("  MediumHeap buffers %-6lld alloc %-6lld free %-6lld\n",
         gpMediumHeap->iBufferCount(),
         gpMediumHeap->iAllocCount(),
         gpMediumHeap->iFreeCount());
  
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
  
  vTrace("**************************************************************\n");
  vTrace(" dir %lld file %lld shm %lld socket %lld thread %lld\n",
         *gzDirOpenCount,
         *gzFileOpenCount,
         *gzShmOpenCount,
         *gzSocketOpenCount,
         *gzThreadOpenCount
         );
  vTrace("**************************************************************\n");
  
  _postcondition_(_MAGIC_,CN_BUFFERPOOLANALYZER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* dump the buffer pool objects                                               */
/*----------------------------------------------------------------------------*/
void BufferPoolAnalyzer::vDump(void)
{
  _precondition_(_MAGIC_,CN_BUFFERPOOLANALYZER,_FOOTER_);
  
  i8 iCountObjects[CN_ENUMS];
  for (i8 index = 0; index < CN_ENUMS; index++)
    iCountObjects[index] = 0;
  
  bn nBuffer = 0;
  bn nBufferCount = gpBufferPool->m_iBufferCount - 1;
  
  for (bn nBuffer = gpBufferPool->nBufferFirst();
       nBuffer <= nBufferCount;
       nBuffer++) {
    
    Buffer* pBuffer = gpBufferPool->pToBuffer(nBuffer);
    
    iCountObjects[pBuffer->eClass()]++;
  }
  
  vTrace("  Region %s %p\n",gxArguments.m_sRegionName.a(),gpBufferPool);
  vTrace("  RegionSize %lld BufferSize %lld \n",
         gpBufferPool->m_iRegionSize,
         gpBufferPool->m_iBufferSize);
  
  vTrace("\n");
  
  vTrace("  BufferPool buffers %-6lld alloc %-6lld free %-6lld \n",
         gpBufferPool->m_iBufferCount,
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
  
  for (bn nBuffer = gpBufferPool->nBufferFirst();
       nBuffer <= nBufferCount;
       nBuffer++) {
    
    Buffer* pBuffer = gpBufferPool->pToBuffer(nBuffer);
  
    vDump(pBuffer);
    iCountObjects[pBuffer->eClass()]++;
  }
  vTrace("**************************************************************\n");
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_BUFFERPOOLANALYZER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* ASSERT class no                                                            */
/*----------------------------------------------------------------------------*/
void BufferPoolAnalyzer::vAssertClass(eCN eClassNo) const
{
}

/*----------------------------------------------------------------------------*/
/* dump the buffer                                                            */
/*----------------------------------------------------------------------------*/
void BufferPoolAnalyzer::vDump(Buffer* pBuffer)
{
  _precondition_(_MAGIC_,CN_BUFFERPOOLANALYZER,_FOOTER_);
  
  pDUMPFUN* pDumpFun =gpDump[pBuffer->eClass()];
  (pDumpFun)(pBuffer->eClass(), pBuffer, pSingleTab());
  
  if (pBuffer->eClass() == CN_MEDIUMHEAP ||
      pBuffer->eClass() == CN_SMALLHEAP ) {
    vDumpHeap(reinterpret_cast<Heap*>(pBuffer));
  }
}

/*----------------------------------------------------------------------------*/
/* dump this heap buffer                                                      */
/*----------------------------------------------------------------------------*/
void BufferPoolAnalyzer::vDumpHeap(Heap* pBuffer)
{
  _precondition_(_MAGIC_,CN_BUFFERPOOLANALYZER,_FOOTER_);
  
  HeapBuffer* pHeapBuffer;
  
  pBuffer->vGrabLock();
  
  for (pHeapBuffer = pBuffer->pFirstBuffer();
       pHeapBuffer;
       pHeapBuffer = pHeapBuffer->pNextBuffer()) {
    for (FreeChunk* pChunk = pHeapBuffer->pFirstChunk();
         pChunk;
         pChunk = pHeapBuffer->pNextChunk(pChunk,pBuffer->iObjectSize())) {
      
      eCN eClass = pChunk->eClass();
      
      if (CN_FREECHUNK == eClass)
        continue;
      
      pDUMPFUN* pDumpFun =gpDump[eClass];
      (pDumpFun)(eClass, pChunk, pDoubleTab());
    }
  }
  pBuffer->vDropLock();
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* BufferPoolAnalyzer::pFooter(void)
{
  _precondition_(_MAGIC_,CN_BUFFERPOOLANALYZER);
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/