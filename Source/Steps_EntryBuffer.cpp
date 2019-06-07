/*============================================================================*/
/* Steps_EntBuffer.cpp - entry buffer step group                            */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_EntBuffer(void)
{
  vAddStep("entryBuffer.dump"_Stz,        &Step::vEntBufferDump);
  vAddStep("entryBuffer.index@"_Stz,      &Step::vEntBufferIndexFetch);
  vAddStep("entryBuffer.index!"_Stz,      &Step::vEntBufferIndexStore);
  vAddStep("entryBuffer.nextBuffer"_Stz,  &Step::vEntBufferNextBuffer);
  vAddStep("entryBuffer.partition@"_Stz,  &Step::vEntBufferPartitionFetch);
  vAddStep("entryBuffer.partition!"_Stz,  &Step::vEntBufferPartitionStore);
  vAddStep("entryBuffer::toEntBuffer"_Stz,&Step::vEntBufferToEntBuffer);
}

/*----------------------------------------------------------------------------*/
/* entryBuffer.dump -- dump the index buffer                                  */
/*----------------------------------------------------------------------------*/
void Step::vEntBufferDump(MP_Task mpTask)
{
  ENTRY("vEntBufferDump"_Stz);
  
  Object* pPtrIndexBuffer = nullptr;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndexBuffer) )
    THROW_INVALID_DSTACK("entryBuffer.dump"_Stz);
  
  EntBuffer* pIndexBuffer = _pEntBuffer(pPtrIndexBuffer);
  
  pIndexBuffer->vShow();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* entryBuffer.index@ -- get index from entry buffer                          */
/*----------------------------------------------------------------------------*/
void Step::vEntBufferIndexFetch(MP_Task mpTask)
{
  ENTRY("vEntBufferIndexFetch"_Stz);
  
  Object* pPtrIndex;
  Object* pPtrIndexBuffer;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndexBuffer) )
    THROW_INVALID_DSTACK("entryBuffer.index@"_Stz);
  
  EntBuffer* pIndexBuffer = _pEntBuffer(pPtrIndexBuffer);
  
  mpTask->m_pDataStack->vPushPointer(pIndexBuffer->pIndex());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* entryBuffer.index! -- set index in entry buffer                            */
/*----------------------------------------------------------------------------*/
void Step::vEntBufferIndexStore(MP_Task mpTask)
{
  ENTRY("vEntBufferIndexStore"_Stz);
  
  Object* pPtrIndex;
  Object* pPtrIndexBuffer;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndexBuffer) )
    THROW_INVALID_DSTACK("entryBuffer.isLast!"_Stz);
  
  EntBuffer* pIndexBuffer = _pEntBuffer(pPtrIndexBuffer);
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("entryBuffer.index@"_Stz);
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  pIndexBuffer->vIndex(pIndex);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* entryBuffer.nextBuffer -- get next buffer of this index buffer             */
/*----------------------------------------------------------------------------*/
void Step::vEntBufferNextBuffer(MP_Task mpTask)
{
  ENTRY("vEntBufferNextBuffer"_Stz);
  
  Object* pPtrEntry = nullptr;
  Object* pPtrIndexBuffer;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndexBuffer) )
    THROW_INVALID_DSTACK("entryBuffer.nextBuffer"_Stz);
  
  EntBuffer* pIndexBuffer = _pEntBuffer(pPtrIndexBuffer);
  
  if (pIndexBuffer->pNextBuffer()) {
    bn nBuffer = pIndexBuffer->pNextBuffer()->nPoolBufferNo();
  }
  
  mpTask->m_pDataStack->vPushPointer(pIndexBuffer->pNextBuffer());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* entryBuffer.partition@ -- get partition no of this index buffer            */
/*----------------------------------------------------------------------------*/
void Step::vEntBufferPartitionFetch(MP_Task mpTask)
{
  ENTRY("vEntBufferPartitionFetch"_Stz);
  
  Object* pPtrEntry = nullptr;
  Object* pPtrIndexBuffer;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndexBuffer) )
    THROW_INVALID_DSTACK("entryBuffer.partition@"_Stz);
  
  EntBuffer* pIndexBuffer = _pEntBuffer(pPtrIndexBuffer);
  
  mpTask->m_pDataStack->vPushI8(pIndexBuffer->iPartitionNo());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* entryBuffer.partition! -- set partition no of this index buffer            */
/*----------------------------------------------------------------------------*/
void Step::vEntBufferPartitionStore(MP_Task mpTask)
{
  ENTRY("vEntBufferPartitionStore"_Stz);
  
  Object* pPtrEntry = nullptr;
  Object* pPtrIndexBuffer;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndexBuffer) )
    THROW_INVALID_DSTACK("entryBuffer.partition!"_Stz);
  
  EntBuffer* pIndexBuffer = _pEntBuffer(pPtrIndexBuffer);
  
  i8 iPartitionNo;
  if ( ! mpTask->m_pDataStack->bPop(iPartitionNo) )
    THROW_INVALID_DSTACK("entryBuffer.partition!"_Stz);
  
  pIndexBuffer->vPartitionNo(iPartitionNo);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* entryBuffer::toEntBuffer -- convert string to entry buffer pointer       */
/*----------------------------------------------------------------------------*/
void Step::vEntBufferToEntBuffer(MP_Task mpTask)
{
  ENTRY("vEntBufferToEntBuffer"_Stz);
  
  s48K sString;
  
  if ( ! mpTask->m_pDataStack->bPop(sString) )
    THROW_INVALID_DSTACK("entryBuffer::toEntBuffer"_Stz);
  
  EntBuffer* pEntBuffer = _pEntBuffer((void*)sString.iToI8());
  
  mpTask->m_pDataStack->vPushPointer(pEntBuffer);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
