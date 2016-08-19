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
/* ASSERT class no                                                            */
/*----------------------------------------------------------------------------*/
void BufferPoolAnalyzer::vAssertClass(eCN eClassNo) const
{
  _magic_();
  
  ASSERT(m_nClassNo == eClassNo);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/