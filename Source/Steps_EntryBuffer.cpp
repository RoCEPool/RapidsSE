/*============================================================================*/
/* Steps_EntBuffer.cpp - entry buffer step group                              */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_EntBuffer(void)
{
  vAddStep("entryBuffer.dump"_Stz,        &Step::vEntBufferDump);
  vAddStep("entryBuffer.index@"_Stz,      &Step::vEntBufferIndexFetch);
  vAddStep("entryBuffer.nextBuffer"_Stz,  &Step::vEntBufferNextBuffer);
  vAddStep("entryBuffer.partition@"_Stz,  &Step::vEntBufferPartitionFetch);
  vAddStep("entryBuffer.partition!"_Stz,  &Step::vEntBufferPartitionStore);
  vAddStep("entryBuffer::toEntBuffer"_Stz,&Step::vEntBufferToEntBuffer);
}

/*----------------------------------------------------------------------------*/
/* entryBuffer.dump -- dump the index buffer                                  */
/*----------------------------------------------------------------------------*/
void Step::vEntBufferDump(void)
{
  ENTRY();
  
  EntBuffer* pIndexBuffer = _pEntBuffer(m_pDataStack->pPopPtr(__func__));
  
  pIndexBuffer->vShow();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* entryBuffer.index@ -- get index from entry buffer                          */
/*----------------------------------------------------------------------------*/
void Step::vEntBufferIndexFetch(void)
{
  ENTRY();
  
  Object* pPtrIndex;
  EntBuffer* pIndexBuffer = _pEntBuffer(m_pDataStack->pPopPtr(__func__));
  
  m_pDataStack->vPushPtr(pIndexBuffer->pIndex());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* entryBuffer.nextBuffer -- get next buffer of this index buffer             */
/*----------------------------------------------------------------------------*/
void Step::vEntBufferNextBuffer(void)
{
  ENTRY();
  
  Object* pPtrEntry = nullptr;
  EntBuffer* pIndexBuffer = _pEntBuffer(m_pDataStack->pPopPtr(__func__));
  
  if (pIndexBuffer->pNextBuffer()) {
    i8 nBuffer = pIndexBuffer->pNextBuffer()->iBufferNo();
  }
  
  m_pDataStack->vPushPtr(pIndexBuffer->pNextBuffer());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* entryBuffer.partition@ -- get partition no of this index buffer            */
/*----------------------------------------------------------------------------*/
void Step::vEntBufferPartitionFetch(void)
{
  ENTRY();
  
  Object* pPtrEntry = nullptr;
  EntBuffer* pIndexBuffer = _pEntBuffer(m_pDataStack->pPopPtr(__func__));
  
  m_pDataStack->vPushInt(pIndexBuffer->iPartitionNo());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* entryBuffer.partition! -- set partition no of this index buffer            */
/*----------------------------------------------------------------------------*/
void Step::vEntBufferPartitionStore(void)
{
  ENTRY();
  
  Object* pPtrEntry = nullptr;
  EntBuffer* pIndexBuffer = _pEntBuffer(m_pDataStack->pPopPtr(__func__));
  
  i8 iPartitionNo = m_pDataStack->iPopInt(__func__);
  
  pIndexBuffer->vPartitionNo(iPartitionNo);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* entryBuffer::toEntBuffer -- convert string to entry buffer pointer       */
/*----------------------------------------------------------------------------*/
void Step::vEntBufferToEntBuffer(void)
{
  ENTRY();
  
  s48K sString;
  
  m_pDataStack->vPopStr(sString,__func__);
  
  EntBuffer* pEntBuffer = _pEntBuffer((void*)sString.iToI8());
  
  m_pDataStack->vPushPtr(pEntBuffer);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
