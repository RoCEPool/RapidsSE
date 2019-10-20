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
void Step::vEntBufferDump(void)
{
  ENTRY("vEntBufferDump"_Stz);
  
  Object* pPtrIndexBuffer = nullptr;
  
  if ( ! m_pDataStack->bPopObject(pPtrIndexBuffer) )
    THROW_DSTACK();
  
  EntBuffer* pIndexBuffer = _pEntBuffer(pPtrIndexBuffer);
  
  pIndexBuffer->vShow();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* entryBuffer.index@ -- get index from entry buffer                          */
/*----------------------------------------------------------------------------*/
void Step::vEntBufferIndexFetch(void)
{
  ENTRY("vEntBufferIndexFetch"_Stz);
  
  Object* pPtrIndex;
  Object* pPtrIndexBuffer;
  
  if ( ! m_pDataStack->bPopObject(pPtrIndexBuffer) )
    THROW_DSTACK();
  
  EntBuffer* pIndexBuffer = _pEntBuffer(pPtrIndexBuffer);
  
  m_pDataStack->vPushPointer(pIndexBuffer->pIndex());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* entryBuffer.index! -- set index in entry buffer                            */
/*----------------------------------------------------------------------------*/
void Step::vEntBufferIndexStore(void)
{
  ENTRY("vEntBufferIndexStore"_Stz);
  
  Object* pPtrIndex;
  Object* pPtrIndexBuffer;
  
  if ( ! m_pDataStack->bPopObject(pPtrIndexBuffer) )
    THROW_DSTACK();
  
  EntBuffer* pIndexBuffer = _pEntBuffer(pPtrIndexBuffer);
  
  if ( ! m_pDataStack->bPopObject(pPtrIndex) )
    THROW_DSTACK();
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  pIndexBuffer->vIndex(pIndex);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* entryBuffer.nextBuffer -- get next buffer of this index buffer             */
/*----------------------------------------------------------------------------*/
void Step::vEntBufferNextBuffer(void)
{
  ENTRY("vEntBufferNextBuffer"_Stz);
  
  Object* pPtrEntry = nullptr;
  Object* pPtrIndexBuffer;
  
  if ( ! m_pDataStack->bPopObject(pPtrIndexBuffer) )
    THROW_DSTACK();
  
  EntBuffer* pIndexBuffer = _pEntBuffer(pPtrIndexBuffer);
  
  if (pIndexBuffer->pNextBuffer()) {
    bn nBuffer = pIndexBuffer->pNextBuffer()->nPoolBufferNo();
  }
  
  m_pDataStack->vPushPointer(pIndexBuffer->pNextBuffer());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* entryBuffer.partition@ -- get partition no of this index buffer            */
/*----------------------------------------------------------------------------*/
void Step::vEntBufferPartitionFetch(void)
{
  ENTRY("vEntBufferPartitionFetch"_Stz);
  
  Object* pPtrEntry = nullptr;
  Object* pPtrIndexBuffer;
  
  if ( ! m_pDataStack->bPopObject(pPtrIndexBuffer) )
    THROW_DSTACK();
  
  EntBuffer* pIndexBuffer = _pEntBuffer(pPtrIndexBuffer);
  
  m_pDataStack->vPushI8(pIndexBuffer->iPartitionNo());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* entryBuffer.partition! -- set partition no of this index buffer            */
/*----------------------------------------------------------------------------*/
void Step::vEntBufferPartitionStore(void)
{
  ENTRY("vEntBufferPartitionStore"_Stz);
  
  Object* pPtrEntry = nullptr;
  Object* pPtrIndexBuffer;
  
  if ( ! m_pDataStack->bPopObject(pPtrIndexBuffer) )
    THROW_DSTACK();
  
  EntBuffer* pIndexBuffer = _pEntBuffer(pPtrIndexBuffer);
  
  i8 iPartitionNo;
  if ( ! m_pDataStack->bPopI8(iPartitionNo) )
    THROW_DSTACK();
  
  pIndexBuffer->vPartitionNo(iPartitionNo);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* entryBuffer::toEntBuffer -- convert string to entry buffer pointer       */
/*----------------------------------------------------------------------------*/
void Step::vEntBufferToEntBuffer(void)
{
  ENTRY("vEntBufferToEntBuffer"_Stz);
  
  s48K sString;
  
  if ( ! m_pDataStack->bPopS48K(sString) )
    THROW_DSTACK();
  
  EntBuffer* pEntBuffer = _pEntBuffer((void*)sString.iToI8());
  
  m_pDataStack->vPushPointer(pEntBuffer);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
