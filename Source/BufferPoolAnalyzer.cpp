/*============================================================================*/
/*  BufferPoolAnalyzer.cpp - buffer pool analyzer                             */
/*============================================================================*/

#include "StorageEngine.h"

extern Heap* gpMediumHeap;
extern Heap* gpSmallHeap;
extern Arguments gxArguments;

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
BufferPoolAnalyzer::BufferPoolAnalyzer(void)
{
  _magic_();
  m_nClassNo = CN_BUFFERPOOLANALYZER;
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
BufferPoolAnalyzer::~BufferPoolAnalyzer(void)
{
  ASSERT(m_nClassNo == CN_BUFFERPOOLANALYZER);
}

/*----------------------------------------------------------------------------*/
/* analyze the buffer pool                                                    */
/*----------------------------------------------------------------------------*/
void BufferPoolAnalyzer::vAnalyze(void)
{
  ASSERT(m_nClassNo == CN_BUFFERPOOLANALYZER);
  
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
    pBuffer->vDump();
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
}

/*----------------------------------------------------------------------------*/
/* dump the buffer pool objects                                               */
/*----------------------------------------------------------------------------*/
void BufferPoolAnalyzer::vDump(void)
{
  ASSERT(m_nClassNo == CN_BUFFERPOOLANALYZER);
  
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
}

/*----------------------------------------------------------------------------*/
/* ASSERT class no                                                            */
/*----------------------------------------------------------------------------*/
void BufferPoolAnalyzer::vAssertClass(eCN eClassNo) const
{
  _magic_();
  
  ASSERT(m_nClassNo == eClassNo);
}

/*----------------------------------------------------------------------------*/
/* dump the buffer                                                            */
/*----------------------------------------------------------------------------*/
void BufferPoolAnalyzer::vDump(Buffer* pBuffer)
{
  _magic_();
  
  switch (pBuffer->eClass()) {
  
    case CN_ATTACHMAP:
      ((AttachMap *)(pBuffer))->vDump(pSingleTab()); break;
    case CN_BUFFER:
      break;
    case CN_FILE:
      ((File *)(pBuffer))->vDump(pSingleTab()); break;
    case CN_DICTIONARY:
      ((Dictionary *)(pBuffer))->vDump(pSingleTab()); break;
    case CN_SMALLHEAP:
    case CN_MEDIUMHEAP:
      ((Heap *)(pBuffer))->vDump(pSingleTab());
      vDumpHeap((Heap *)pBuffer);
      break;
    case CN_HEAPBUFFER:
      
      ((HeapBuffer *)(pBuffer))->vDump(pSingleTab()); break;
    case CN_TXARRAY:
      ((TxArray *)(pBuffer))->vDump(pSingleTab()); break;
    break;
    
  default:
    vTrace("%sNo vDump() method in class %s\n\n", pSingleTab(), pCN[pBuffer->eClass()]);
  }
   
}

/*----------------------------------------------------------------------------*/
/* dump this heap buffer                                                      */
/*----------------------------------------------------------------------------*/
void BufferPoolAnalyzer::vDumpHeap(Heap* pBuffer)
{
  _magic_();
  
  HeapBuffer* pHeapBuffer;
  s16K pSpace;
  pSpace = "  ";
  
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
      switch (eClass) {
        case CN_NODE:
          ((Node *)(pChunk))->vDump(pDoubleTab()); break;
        case CN_TASK:
          ((Task *)(pChunk))->vDump(pDoubleTab()); break;
        case CN_TASKLIST: ((TaskList *)(pChunk))->vDump(pDoubleTab()); break;
        case CN_WORD: break;
        default:
          vTrace("\n%sNo vDump() method in class %s\n\n", pDoubleTab(),pCN[eClass]);
          break;
      }
    }
  }
  pBuffer->vDropLock();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/