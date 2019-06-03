/*============================================================================*/
/* Steps_EntryBuffer.cpp - entry buffer step group                            */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_EntryBuffer(void)
{
  vAddStep("entryBuffer.dump"_Stz,        &Step::vEntryBufferDump);
  vAddStep("entryBuffer.index@"_Stz,      &Step::vEntryBufferIndexFetch);
  vAddStep("entryBuffer.index!"_Stz,      &Step::vEntryBufferIndexStore);
  vAddStep("entryBuffer.isLast@"_Stz,     &Step::vEntryBufferIsLastFetch);
  vAddStep("entryBuffer.isLast!"_Stz,     &Step::vEntryBufferIsLastStore);
  vAddStep("entryBuffer.nextBuffer"_Stz,  &Step::vEntryBufferNextBuffer);
  vAddStep("entryBuffer.partition@"_Stz,  &Step::vEntryBufferPartitionFetch);
  vAddStep("entryBuffer.partition!"_Stz,  &Step::vEntryBufferPartitionStore);
  vAddStep("entryBuffer::toEntryBuffer"_Stz,&Step::vEntryBufferToEntryBuffer);
}

/*----------------------------------------------------------------------------*/
/* entryBuffer.dump -- dump the index buffer                                  */
/*----------------------------------------------------------------------------*/
void Step::vEntryBufferDump(MP_Task mpTask)
{
  ENTRY("vEntryBufferDump"_Stz);
  
  Object* pPtrIndexBuffer = nullptr;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndexBuffer) )
    THROW_INVALID_DSTACK("entryBuffer.dump"_Stz);
  
  EntryBuffer* pIndexBuffer = _pEntryBuffer(pPtrIndexBuffer);
  
  pIndexBuffer->vShow();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* entryBuffer.index@ -- get index from entry buffer                          */
/*----------------------------------------------------------------------------*/
void Step::vEntryBufferIndexFetch(MP_Task mpTask)
{
  ENTRY("vEntryBufferIndexFetch"_Stz);
  
  Object* pPtrIndex;
  Object* pPtrIndexBuffer;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndexBuffer) )
    THROW_INVALID_DSTACK("entryBuffer.index@"_Stz);
  
  EntryBuffer* pIndexBuffer = _pEntryBuffer(pPtrIndexBuffer);
  
  mpTask->m_pDataStack->vPushPointer(pIndexBuffer->pIndex());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* entryBuffer.index! -- set index in entry buffer                            */
/*----------------------------------------------------------------------------*/
void Step::vEntryBufferIndexStore(MP_Task mpTask)
{
  ENTRY("vEntryBufferIndexStore"_Stz);
  
  Object* pPtrIndex;
  Object* pPtrIndexBuffer;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndexBuffer) )
    THROW_INVALID_DSTACK("entryBuffer.isLast!"_Stz);
  
  EntryBuffer* pIndexBuffer = _pEntryBuffer(pPtrIndexBuffer);
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("entryBuffer.index@"_Stz);
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  pIndexBuffer->vIndex(pIndex);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* entryBuffer.isLast@ -- get if buffer is last                               */
/*----------------------------------------------------------------------------*/
void Step::vEntryBufferIsLastFetch(MP_Task mpTask)
{
  ENTRY("vEntryBufferIsLastFetch"_Stz);
  
  Object* pPtrEntry = nullptr;
  Object* pPtrIndexBuffer;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndexBuffer) )
    THROW_INVALID_DSTACK("entryBuffer.isLast@"_Stz);
  
  EntryBuffer* pIndexBuffer = _pEntryBuffer(pPtrIndexBuffer);
  
  mpTask->m_pDataStack->vPushI8(pIndexBuffer->bIsLast());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* entryBuffer.isLast! -- set buffer is last                                  */
/*----------------------------------------------------------------------------*/
void Step::vEntryBufferIsLastStore(MP_Task mpTask)
{
  ENTRY("vEntryBufferIsLastStore"_Stz);
  
  Object* pPtrEntry = nullptr;
  Object* pPtrIndexBuffer;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndexBuffer) )
    THROW_INVALID_DSTACK("entryBuffer.isLast!"_Stz);
  
  EntryBuffer* pIndexBuffer = _pEntryBuffer(pPtrIndexBuffer);
  
  pIndexBuffer->vIsLast(true);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* entryBuffer.nextBuffer -- get next buffer of this index buffer             */
/*----------------------------------------------------------------------------*/
void Step::vEntryBufferNextBuffer(MP_Task mpTask)
{
  ENTRY("vEntryBufferNextBuffer"_Stz);
  
  Object* pPtrEntry = nullptr;
  Object* pPtrIndexBuffer;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndexBuffer) )
    THROW_INVALID_DSTACK("entryBuffer.nextBuffer"_Stz);
  
  EntryBuffer* pIndexBuffer = _pEntryBuffer(pPtrIndexBuffer);
  
  if (pIndexBuffer->pNextBuffer()) {
    bn nBuffer = pIndexBuffer->pNextBuffer()->nPoolBufferNo();
  }
  
  mpTask->m_pDataStack->vPushPointer(pIndexBuffer->pNextBuffer());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* entryBuffer.partition@ -- get partition no of this index buffer            */
/*----------------------------------------------------------------------------*/
void Step::vEntryBufferPartitionFetch(MP_Task mpTask)
{
  ENTRY("vEntryBufferPartitionFetch"_Stz);
  
  Object* pPtrEntry = nullptr;
  Object* pPtrIndexBuffer;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndexBuffer) )
    THROW_INVALID_DSTACK("entryBuffer.partition@"_Stz);
  
  EntryBuffer* pIndexBuffer = _pEntryBuffer(pPtrIndexBuffer);
  
  mpTask->m_pDataStack->vPushI8(pIndexBuffer->iPartitionNo());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* entryBuffer.partition! -- set partition no of this index buffer            */
/*----------------------------------------------------------------------------*/
void Step::vEntryBufferPartitionStore(MP_Task mpTask)
{
  ENTRY("vEntryBufferPartitionStore"_Stz);
  
  Object* pPtrEntry = nullptr;
  Object* pPtrIndexBuffer;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndexBuffer) )
    THROW_INVALID_DSTACK("entryBuffer.partition!"_Stz);
  
  EntryBuffer* pIndexBuffer = _pEntryBuffer(pPtrIndexBuffer);
  
  i8 iPartitionNo;
  if ( ! mpTask->m_pDataStack->bPop(iPartitionNo) )
    THROW_INVALID_DSTACK("entryBuffer.partition!"_Stz);
  
  pIndexBuffer->vPartitionNo(iPartitionNo);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* entryBuffer::toEntryBuffer -- convert string to entry buffer pointer       */
/*----------------------------------------------------------------------------*/
void Step::vEntryBufferToEntryBuffer(MP_Task mpTask)
{
  ENTRY("vEntryBufferToEntryBuffer"_Stz);
  
  s48K sString;
  
  if ( ! mpTask->m_pDataStack->bPop(sString) )
    THROW_INVALID_DSTACK("entryBuffer::toEntryBuffer"_Stz);
  
  EntryBuffer* pEntryBuffer = _pEntryBuffer((void*)sString.iToI8());
  
  mpTask->m_pDataStack->vPushPointer(pEntryBuffer);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
