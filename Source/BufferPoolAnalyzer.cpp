/*============================================================================*/
/*  BufferPoolAnalyzer.cpp - buffer pool analyzer                             */
/*============================================================================*/

#include "Tasking.h"

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
  
  vTrace("/**************************************/\n");
  vTrace("    BufferPoolAnalyzer\n");
  vTrace("/**************************************/\n");

  vTrace("PoolSize %lld BufferSize %lld \n"
         "BufferCount %lld InUse %lld FreeCount %lld\n",
         gpBufferPool->m_iPoolSize,
         gpBufferPool->m_iBufferSize,
         gpBufferPool->m_iBufferCount,
         gpBufferPool->m_iUseCount,
         gpBufferPool->m_iFreeCount
         );
  
  i8 iCountPoolObjects[CN_ENUMS];
  i8 iCountSlotObjects[CN_ENUMS];
  
  for (i8 index = 0; index < CN_ENUMS; index++) {
    iCountPoolObjects[index] = 0;
    iCountSlotObjects[index] = 0;
  }
  
  bn nBuffer = gpBufferPool->nBufferFirst();
  bn nBufferCount = gpBufferPool->m_iBufferCount - 1;
  for ( bn nBuffer = gpBufferPool->nBufferFirst(); nBuffer <= nBufferCount; nBuffer++) {
    Buffer* pBuffer = gpBufferPool->pToBuffer(nBuffer);

    iCountPoolObjects[pBuffer->eClass()]++;
        
//    if (pBuffer->eClass() == CN_CATALOGALLOCATOR ||
//        pBuffer->eClass() == CN_SCHEMAALLOCATOR) {
//     
//      SlotAllocator* pSlotAllocator = static_cast<SlotAllocator*>(pBuffer);
//      for (sn nSlot = 0_sn; nSlot < pSlotAllocator->nSlotLast(); nSlot++) {
//        
//        Slot* pSlot = pSlotAllocator->pToSlot(nSlot);
//        if (pSlot->bIsAllocated())
//          iCountSlotObjects[pSlot->eSlotClass()]++;
//      }
//
//    }
  }
  
  vTrace("\nBufferPool Objects:\n");
  for (i8 index = 0; index < CN_ENUMS; index++) {
    eCN eClass = (eCN)index;
    
    if (iCountPoolObjects[eClass] == 0)
      continue;
    
    vTrace("%18s: %d \n", pCN[eClass], iCountPoolObjects[eClass]);
  }
  
  vTrace("\nBufferPool Slot Objects:\n");
  for (i8 index = 0; index < CN_ENUMS; index++) {
    eCN eClass = (eCN)index;
    
    if (iCountSlotObjects[eClass] == 0)
      continue;
    
    vTrace("%18s: %d \n", pCN[eClass], iCountSlotObjects[eClass]);
  }
  vTrace("/**************************************/\n");

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