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
  vAddStep("entryBuffer.index!"_Stz,      &Step::vEntBufferIndexStore);
  vAddStep("entryBuffer.nextBuffer"_Stz,  &Step::vEntBufferNextBuffer);
  vAddStep("entryBuffer.partition@"_Stz,  &Step::vEntBufferPartitionFetch);
  vAddStep("entryBuffer.partition!"_Stz,  &Step::vEntBufferPartitionStore);
  vAddStep("entryBuffer::toEntBuffer"_Stz,&Step::vEntBufferToEntBuffer);
}

/*----------------------------------------------------------------------------*/
/* entryBuffer.dump -- dump the index buffer                                  */
/*----------------------------------------------------------------------------*/
void Step::vEntBufferDump(Forth* pForth)
{
  ENTRY("vEntBufferDump"_Stz);
  
  Object* pPtrIndexBuffer = nullptr;
  
  if ( ! pForth->m_pDataStack->bPop(pPtrIndexBuffer) )
    THROW_DSTACK();
  
  EntBuffer* pIndexBuffer = _pEntBuffer(pPtrIndexBuffer);
  
  pIndexBuffer->vShow();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* entryBuffer.index@ -- get index from entry buffer                          */
/*----------------------------------------------------------------------------*/
void Step::vEntBufferIndexFetch(Forth* pForth)
{
  ENTRY("vEntBufferIndexFetch"_Stz);
  
  Object* pPtrIndex;
  Object* pPtrIndexBuffer;
  
  if ( ! pForth->m_pDataStack->bPop(pPtrIndexBuffer) )
    THROW_DSTACK();
  
  EntBuffer* pIndexBuffer = _pEntBuffer(pPtrIndexBuffer);
  
  pForth->m_pDataStack->vPushPointer(pIndexBuffer->pIndex());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* entryBuffer.index! -- set index in entry buffer                            */
/*----------------------------------------------------------------------------*/
void Step::vEntBufferIndexStore(Forth* pForth)
{
  ENTRY("vEntBufferIndexStore"_Stz);
  
  Object* pPtrIndex;
  Object* pPtrIndexBuffer;
  
  if ( ! pForth->m_pDataStack->bPop(pPtrIndexBuffer) )
    THROW_DSTACK();
  
  EntBuffer* pIndexBuffer = _pEntBuffer(pPtrIndexBuffer);
  
  if ( ! pForth->m_pDataStack->bPop(pPtrIndex) )
    THROW_DSTACK();
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  pIndexBuffer->vIndex(pIndex);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* entryBuffer.nextBuffer -- get next buffer of this index buffer             */
/*----------------------------------------------------------------------------*/
void Step::vEntBufferNextBuffer(Forth* pForth)
{
  ENTRY("vEntBufferNextBuffer"_Stz);
  
  Object* pPtrEntry = nullptr;
  Object* pPtrIndexBuffer;
  
  if ( ! pForth->m_pDataStack->bPop(pPtrIndexBuffer) )
    THROW_DSTACK();
  
  EntBuffer* pIndexBuffer = _pEntBuffer(pPtrIndexBuffer);
  
  if (pIndexBuffer->pNextBuffer()) {
    bn nBuffer = pIndexBuffer->pNextBuffer()->nPoolBufferNo();
  }
  
  pForth->m_pDataStack->vPushPointer(pIndexBuffer->pNextBuffer());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* entryBuffer.partition@ -- get partition no of this index buffer            */
/*----------------------------------------------------------------------------*/
void Step::vEntBufferPartitionFetch(Forth* pForth)
{
  ENTRY("vEntBufferPartitionFetch"_Stz);
  
  Object* pPtrEntry = nullptr;
  Object* pPtrIndexBuffer;
  
  if ( ! pForth->m_pDataStack->bPop(pPtrIndexBuffer) )
    THROW_DSTACK();
  
  EntBuffer* pIndexBuffer = _pEntBuffer(pPtrIndexBuffer);
  
  pForth->m_pDataStack->vPushI8(pIndexBuffer->iPartitionNo());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* entryBuffer.partition! -- set partition no of this index buffer            */
/*----------------------------------------------------------------------------*/
void Step::vEntBufferPartitionStore(Forth* pForth)
{
  ENTRY("vEntBufferPartitionStore"_Stz);
  
  Object* pPtrEntry = nullptr;
  Object* pPtrIndexBuffer;
  
  if ( ! pForth->m_pDataStack->bPop(pPtrIndexBuffer) )
    THROW_DSTACK();
  
  EntBuffer* pIndexBuffer = _pEntBuffer(pPtrIndexBuffer);
  
  i8 iPartitionNo;
  if ( ! pForth->m_pDataStack->bPop(iPartitionNo) )
    THROW_DSTACK();
  
  pIndexBuffer->vPartitionNo(iPartitionNo);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* entryBuffer::toEntBuffer -- convert string to entry buffer pointer       */
/*----------------------------------------------------------------------------*/
void Step::vEntBufferToEntBuffer(Forth* pForth)
{
  ENTRY("vEntBufferToEntBuffer"_Stz);
  
  s48K sString;
  
  if ( ! pForth->m_pDataStack->bPop(sString) )
    THROW_DSTACK();
  
  EntBuffer* pEntBuffer = _pEntBuffer((void*)sString.iToI8());
  
  pForth->m_pDataStack->vPushPointer(pEntBuffer);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
