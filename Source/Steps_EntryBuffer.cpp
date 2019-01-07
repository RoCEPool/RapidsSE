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
  vAddStep("entryBuffer::create"_Stz,     &Step::vEntryBufferCreate);
  vAddStep("entryBuffer.delete"_Stz,      &Step::vEntryBufferDelete);
  vAddStep("entryBuffer.dump"_Stz,        &Step::vEntryBufferDump);
  vAddStep("entryBuffer.firstEntry"_Stz,  &Step::vEntryBufferFirstEntry);
  vAddStep("entryBuffer.index@"_Stz,      &Step::vEntryBufferIndexFetch);
  vAddStep("entryBuffer.index!"_Stz,      &Step::vEntryBufferIndexStore);
  vAddStep("entryBuffer.isLast@"_Stz,     &Step::vEntryBufferIsLastFetch);
  vAddStep("entryBuffer.isLast!"_Stz,     &Step::vEntryBufferIsLastStore);
  vAddStep("entryBuffer.iterate"_Stz,     &Step::vEntryBufferIterate);
  vAddStep("entryBuffer.nextEntry"_Stz,   &Step::vEntryBufferNextEntry);
  vAddStep("entryBuffer.nextBuffer"_Stz,  &Step::vEntryBufferNextBuffer);
  vAddStep("entryBuffer.partition@"_Stz,  &Step::vEntryBufferPartitionFetch);
  vAddStep("entryBuffer.partition!"_Stz,  &Step::vEntryBufferPartitionStore);
  vAddStep("entryBuffer::toEntryBuffer"_Stz,&Step::vEntryBufferToEntryBuffer);
}

/*----------------------------------------------------------------------------*/
/* entryBuffer::create -- create a new index buffer                           */
/*----------------------------------------------------------------------------*/
void Step::vEntryBufferCreate(MP_Task mpTask)
{
  ENTRY("vEntryBufferCreate"_Stz);
  
  Object* pPtrIndex = nullptr;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("entryBuffer::create"_Stz);
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  i8 nPartitionNo;
  if ( ! mpTask->m_pDataStack->bPop(nPartitionNo) )
    THROW_INVALID_DSTACK("entryBuffer::create"_Stz);
  
  EntryBuffer* pEntryBuffer = new EntryBuffer;
  pEntryBuffer->vIndex(pIndex->pTable()->pHiddenIndex());
  pEntryBuffer->vPartitionNo(nPartitionNo);
  
  mpTask->m_pDataStack->vPushObject(pEntryBuffer);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* entryBuffer.delete -- delete the index buffer                              */
/*----------------------------------------------------------------------------*/
void Step::vEntryBufferDelete(MP_Task mpTask)
{
  ENTRY("vEntryBufferDelete"_Stz);
  
  Object* pPtrIndexBuffer = nullptr;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndexBuffer) )
    THROW_INVALID_DSTACK("entryBuffer.delete"_Stz);
  
  EntryBuffer* pIndexBuffer = _pEntryBuffer(pPtrIndexBuffer);
  
  delete pIndexBuffer;
  
  EXIT();
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
/* entryBuffer.firstEntry -- get first entry from index buffer                */
/*----------------------------------------------------------------------------*/
void Step::vEntryBufferFirstEntry(MP_Task mpTask)
{
  ENTRY("vEntryBufferFirstEntry"_Stz);
  
  Object* pPtrEntry = nullptr;
  Object* pPtrIndexBuffer;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndexBuffer) )
    THROW_INVALID_DSTACK("entryBuffer.firstEntry!"_Stz);
  
  EntryBuffer* pIndexBuffer = _pEntryBuffer(pPtrIndexBuffer);
  
  if (pIndexBuffer)
    mpTask->m_pDataStack->vPushObject(pIndexBuffer->pFirstEntry());
  else
    mpTask->m_pDataStack->vPushObject(nullptr);
  
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
  
  mpTask->m_pDataStack->vPushObject(pIndexBuffer->pIndex());
  
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
  
  mpTask->m_pDataStack->vPushInt(pIndexBuffer->bIsLast());
  
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
/* entryBuffer.iterate -- iterate entry sequence                              */
/*----------------------------------------------------------------------------*/
void Step::vEntryBufferIterate(MP_Task mpTask)
{
  ENTRY("vEntryBufferIiterate"_Stz);
  
  Object* pPtrIndexBuffer;
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndexBuffer) )
    THROW_INVALID_DSTACK("entryBuffer.iterate"_Stz);
  EntryBuffer* pIndexBuffer = _pEntryBuffer(pPtrIndexBuffer);
  
  Object* pPtrEntry = nullptr;
  if ( ! mpTask->m_pDataStack->bPop(pPtrEntry) )
    THROW_INVALID_DSTACK("entryBuffer.iterate"_Stz);
  Entry* pUpperIndexEntry = _pEntry(pPtrEntry);
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrEntry) )
    THROW_INVALID_DSTACK("entryBuffer.iterate"_Stz);
  Entry* pLowerIndexEntry = _pEntry(pPtrEntry);
  
  Object* pPtrGroupBy = nullptr;
  if ( ! mpTask->m_pDataStack->bPop(pPtrGroupBy) )
    THROW_INVALID_DSTACK("entryBuffer.iterate"_Stz);
  GroupBy* pGroupByOp = _pGroupBy(pPtrGroupBy);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // iterate all entries from lower to upper
  
  if (! pIndexBuffer)
    return;
  
  Index* pIndex = pIndexBuffer->pIndex();
  
  i8 nRows = 0;
  Entry* pIndexEntry = pLowerIndexEntry;
  while (pIndexBuffer && pIndexEntry) {
   
    // apply
    //Entry* pLineItemEntry = GroupBy::pToLineItemEntry(pIndex,pIndexEntry);
    //pGroupByOp->vApplyEntry(pLineItemEntry);
    pGroupByOp->vApplyEntry(pIndexEntry);
    
    // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
    // set bit on
    
//    Id xId = pIndexEntry->xId();
//    
//    bn nBufferNo = xId.iBuffer();
//    i8 iRow = xId.iRow();
//    
//    EntryBuffer* pDataBuffer = _pEntryBuffer(gpBufferPool->pToBuffer(nBufferNo));
//    ASSERT(pDataBuffer);
//    
//    BitArray* pBitArray = pDataBuffer->pBitArray();
//    ASSERT(pBitArray);
//    pBitArray->vSetOn(iRow);

    //gpAttachMap->m_pBitMap->vSetOn(*nBufferNo,iRow);
    
    // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -

    nRows++;
    
    // test for end
    //if (pIndexEntry == pUpperIndexEntry)
    //  break;
    
    // move to next entry
    pIndexEntry = pIndexBuffer->pNextEntry(pIndexEntry, 0);
    if (! pIndexEntry) {
      pIndexBuffer = pIndexBuffer->pNextBuffer();
      if (! pIndexBuffer)
        break;
      pIndexEntry = pIndexBuffer->pFirstEntry();
    }
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
//  Table* pTable = pIndex->pTable();
//  Slots* pSlots = pTable->pSlots();
//  Partition* pPartition = pSlots->pPartition(0);
//  IndexTree* pIndexTree = pPartition->pHiddenIndexTree();
//  
//  EntryBuffer* pRowBuffer = pIndexTree->pFirstL0Buffer();
//  
//  while ( (pRowBuffer = pRowBuffer->pNextBuffer()) ) {
//    BitArray* pBitArray = pRowBuffer->pBitArray();
//    //vTrace("BitArray: %lld bits on\n"_Stz,pBitArray->nBitsOn());
//    Entry* pRowEntry = pRowBuffer->pFirstEntry();
//    while ( (pRowEntry = pRowBuffer->pNextEntry(pRowEntry)) ) {
//      pGroupByOp->vApplyEntry(pRowEntry);
//    }
//  }
  
  vTrace("Rows processed: %lld\n"_Stz,nRows);
  
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
    bn nBuffer = pIndexBuffer->pNextBuffer()->nBufferNo();
  }
  
  mpTask->m_pDataStack->vPushObject(pIndexBuffer->pNextBuffer());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* entryBuffer.nextEntry -- get next entry from index buffer                  */
/*----------------------------------------------------------------------------*/
void Step::vEntryBufferNextEntry(MP_Task mpTask)
{
  ENTRY("vEntryBufferNextEntry"_Stz);
  
  Object* pPtrEntry = nullptr;
  Object* pPtrIndexBuffer;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndexBuffer) )
    THROW_INVALID_DSTACK("entryBuffer.nextEntry"_Stz);
  
  EntryBuffer* pIndexBuffer = _pEntryBuffer(pPtrIndexBuffer);
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrEntry) )
    THROW_INVALID_DSTACK("entryBuffer.nextEntry"_Stz);
  
  Entry* pEntry = _pEntry(pPtrEntry);
  
  Entry *pNextEntry;
  
  if ( pPtrEntry ) {
    ASSERT(CN_ENTRY == pEntry->eClass());
    pNextEntry = pIndexBuffer->pNextEntry(pEntry, 0);
  } else {
    pNextEntry = pIndexBuffer->pFirstEntry();
  }
  
  mpTask->m_pDataStack->vPushObject(pNextEntry);
  
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
  
  mpTask->m_pDataStack->vPushInt(pIndexBuffer->nPartitionNo());
  
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
  
  mpTask->m_pDataStack->vPushObject(pEntryBuffer);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
