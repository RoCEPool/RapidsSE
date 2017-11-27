/*============================================================================*/
/* Steps_EntryBuffer.cpp - entry buffer step group                            */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern BufferPool*     gpBufferPool;
extern AttachMap*      gpAttachMap;
extern NP_Task         gnpBootTask;
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern BitMap*         gpBitMap;

// indexbuffer: ////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_EntryBuffer(void)
{
  vAddPrimitive("entryBuffer::create"_Stz,     &vEntryBufferCreate,      false);
  vAddPrimitive("entryBuffer.delete"_Stz,      &vEntryBufferDelete,      false);
  vAddPrimitive("entryBuffer.dump"_Stz,        &vEntryBufferDump,        false);
  vAddPrimitive("entryBuffer.firstEntry"_Stz,  &vEntryBufferFirstEntry,  false);
  vAddPrimitive("entryBuffer.index@"_Stz,      &vEntryBufferIndexFetch,  false);
  vAddPrimitive("entryBuffer.index!"_Stz,      &vEntryBufferIndexStore,  false);
  vAddPrimitive("entryBuffer.insertRow"_Stz,   &vEntryBufferInsertRow,   false);
  vAddPrimitive("entryBuffer.isLast@"_Stz,     &vEntryBufferIsLastFetch, false);
  vAddPrimitive("entryBuffer.isLast!"_Stz,     &vEntryBufferIsLastStore, false);
  vAddPrimitive("entryBuffer.iterate"_Stz,     &vEntryBufferIterate,     false);
  vAddPrimitive("entryBuffer.nextEntry"_Stz,   &vEntryBufferNextEntry,   false);
  vAddPrimitive("entryBuffer.nextBuffer"_Stz,  &vEntryBufferNextBuffer,  false);
  vAddPrimitive("entryBuffer.partition@"_Stz,  &vEntryBufferPartitionFetch, false);
  vAddPrimitive("entryBuffer.partition!"_Stz,  &vEntryBufferPartitionStore, false);
  vAddPrimitive("entryBuffer::toEntryBuffer"_Stz,&vEntryBufferToEntryBuffer,false);
}

/*----------------------------------------------------------------------------*/
/* entryBuffer::create -- create a new index buffer                           */
/*----------------------------------------------------------------------------*/
void Task::vEntryBufferCreate(MP_Task mpTask)
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
  
  EntryBuffer* pEntryBuffer = new EntryBuffer(pIndex->pTable()->pHiddenIndex(), 0, nPartitionNo);
  
  mpTask->m_pDataStack->vPushObject(pEntryBuffer);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* entryBuffer.delete -- delete the index buffer                              */
/*----------------------------------------------------------------------------*/
void Task::vEntryBufferDelete(MP_Task mpTask)
{
  ENTRY("vEntryBufferDelete"_Stz);
  
  Object* pPtrIndexBuffer = nullptr;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndexBuffer) )
    THROW_INVALID_DSTACK("entryBuffer.delete"_Stz);
  
  EntryBuffer* pIndexBuffer = _pEntryBuffer(pPtrIndexBuffer);
  ASSERT(CN_ENTRYBUFFER == pIndexBuffer->eClass());
  
  delete pIndexBuffer;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* entryBuffer.dump -- dump the index buffer                                  */
/*----------------------------------------------------------------------------*/
void Task::vEntryBufferDump(MP_Task mpTask)
{
  ENTRY("vEntryBufferDump"_Stz);
  
  Object* pPtrIndexBuffer = nullptr;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndexBuffer) )
    THROW_INVALID_DSTACK("entryBuffer.dump"_Stz);
  
  EntryBuffer* pIndexBuffer = _pEntryBuffer(pPtrIndexBuffer);
  ASSERT(CN_ENTRYBUFFER == pIndexBuffer->eClass());
  
  pIndexBuffer->vShow();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* entryBuffer.firstEntry -- get first entry from index buffer                */
/*----------------------------------------------------------------------------*/
void Task::vEntryBufferFirstEntry(MP_Task mpTask)
{
  ENTRY("vEntryBufferFirstEntry"_Stz);
  
  Object* pPtrEntry = nullptr;
  Object* pPtrIndexBuffer;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndexBuffer) )
    THROW_INVALID_DSTACK("entryBuffer.firstEntry!"_Stz);
  
  EntryBuffer* pIndexBuffer = _pEntryBuffer(pPtrIndexBuffer);
  ASSERT(nullptr == pIndexBuffer || CN_ENTRYBUFFER == pIndexBuffer->eClass());
  
  if (pIndexBuffer)
    mpTask->m_pDataStack->vPushObject(pIndexBuffer->pNextEntry(pIndexBuffer->pHead()));
  else
    mpTask->m_pDataStack->vPushObject(nullptr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* entryBuffer.index@ -- get index from entry buffer                          */
/*----------------------------------------------------------------------------*/
void Task::vEntryBufferIndexFetch(MP_Task mpTask)
{
  ENTRY("vEntryBufferIndexFetch"_Stz);
  
  Object* pPtrIndex;
  Object* pPtrIndexBuffer;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndexBuffer) )
    THROW_INVALID_DSTACK("entryBuffer.index@"_Stz);
  
  EntryBuffer* pIndexBuffer = _pEntryBuffer(pPtrIndexBuffer);
  ASSERT(CN_ENTRYBUFFER == pIndexBuffer->eClass());
  
  mpTask->m_pDataStack->vPushObject(pIndexBuffer->pIndex());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* entryBuffer.index! -- set index in entry buffer                            */
/*----------------------------------------------------------------------------*/
void Task::vEntryBufferIndexStore(MP_Task mpTask)
{
  ENTRY("vEntryBufferIndexStore"_Stz);
  
  Object* pPtrIndex;
  Object* pPtrIndexBuffer;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndexBuffer) )
    THROW_INVALID_DSTACK("entryBuffer.isLast!"_Stz);
  
  EntryBuffer* pIndexBuffer = _pEntryBuffer(pPtrIndexBuffer);
  ASSERT(nullptr == pIndexBuffer || CN_ENTRYBUFFER == pIndexBuffer->eClass());
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("entryBuffer.index@"_Stz);
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  pIndexBuffer->vIndex(pIndex);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* entryBuffer.insertRow -- insert a row entry into the index buffer          */
/*----------------------------------------------------------------------------*/
void Task::vEntryBufferInsertRow(MP_Task mpTask)
{
  ENTRY("vEntryBufferInsertRow"_Stz);
  
  Object* pPtrOldEntry = nullptr;
  Object* pPtrIndexBuffer;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndexBuffer) )
    THROW_INVALID_DSTACK("entryBuffer.insertRow"_Stz);
  
  EntryBuffer* pIndexBuffer = _pEntryBuffer(pPtrIndexBuffer);
  ASSERT(nullptr == pIndexBuffer || CN_ENTRYBUFFER == pIndexBuffer->eClass());
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrOldEntry) )
    THROW_INVALID_DSTACK("entryBuffer.insertRow"_Stz);
  
  Entry* pOldEntry = _pEntry(pPtrOldEntry);
  ASSERT(CN_ENTRY == pOldEntry->eClass());
  
  Id xId = pOldEntry->xId();
  SEitf* pSEitf = mpTask->m_pSEitf;
  
  if (pSEitf && pSEitf->bRowSeen(xId)) {
    mpTask->m_pDataStack->vPushInt(1);
    return;
  }
  
  Entry* pRow;
  
  if (pIndexBuffer->bInsertRow(pOldEntry, &pRow)) {
    ASSERT( ! pRow->nOffsets() );
    mpTask->m_pDataStack->vPushInt(1);
  }
  else
    mpTask->m_pDataStack->vPushInt(0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* entryBuffer.isLast@ -- get if buffer is last                               */
/*----------------------------------------------------------------------------*/
void Task::vEntryBufferIsLastFetch(MP_Task mpTask)
{
  ENTRY("vEntryBufferIsLastFetch"_Stz);
  
  Object* pPtrEntry = nullptr;
  Object* pPtrIndexBuffer;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndexBuffer) )
    THROW_INVALID_DSTACK("entryBuffer.isLast@"_Stz);
  
  EntryBuffer* pIndexBuffer = _pEntryBuffer(pPtrIndexBuffer);
  ASSERT(nullptr == pIndexBuffer || CN_ENTRYBUFFER == pIndexBuffer->eClass());
  
  mpTask->m_pDataStack->vPushInt(pIndexBuffer->bIsLast());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* entryBuffer.isLast! -- set buffer is last                                  */
/*----------------------------------------------------------------------------*/
void Task::vEntryBufferIsLastStore(MP_Task mpTask)
{
  ENTRY("vEntryBufferIsLastStore"_Stz);
  
  Object* pPtrEntry = nullptr;
  Object* pPtrIndexBuffer;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndexBuffer) )
    THROW_INVALID_DSTACK("entryBuffer.isLast!"_Stz);
  
  EntryBuffer* pIndexBuffer = _pEntryBuffer(pPtrIndexBuffer);
  ASSERT(nullptr == pIndexBuffer || CN_ENTRYBUFFER == pIndexBuffer->eClass());
  
  pIndexBuffer->vIsLast(true);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* entryBuffer.iterate -- iterate entry sequence                              */
/*----------------------------------------------------------------------------*/
void Task::vEntryBufferIterate(MP_Task mpTask)
{
  ENTRY("vEntryBufferIiterate"_Stz);
  
  Object* pPtrIndexBuffer;
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndexBuffer) )
    THROW_INVALID_DSTACK("entryBuffer.iterate"_Stz);
  EntryBuffer* pIndexBuffer = _pEntryBuffer(pPtrIndexBuffer);
  ASSERT(nullptr == pIndexBuffer || CN_ENTRYBUFFER == pIndexBuffer->eClass());
  
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
    pIndexEntry = pIndexBuffer->pNextEntry(pIndexEntry);
    if (! pIndexEntry) {
      pIndexBuffer = pIndexBuffer->pNextBuffer();
      if (! pIndexBuffer)
        break;
      pIndexEntry = pIndexBuffer->pNextEntry(pIndexBuffer->pHead());
    }
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
//  Table* pTable = pIndex->pTable();
//  Slots* pSlots = pTable->pSlots();
//  Partition* pPartition = pSlots->pPartition(0);
//  IndexData* pIndexData = pPartition->pHiddenIndexData();
//  
//  EntryBuffer* pRowBuffer = pIndexData->pFirstBuffer();
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
void Task::vEntryBufferNextBuffer(MP_Task mpTask)
{
  ENTRY("vEntryBufferNextBuffer"_Stz);
  
  Object* pPtrEntry = nullptr;
  Object* pPtrIndexBuffer;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndexBuffer) )
    THROW_INVALID_DSTACK("entryBuffer.nextBuffer"_Stz);
  
  EntryBuffer* pIndexBuffer = _pEntryBuffer(pPtrIndexBuffer);
  ASSERT(nullptr == pIndexBuffer || CN_ENTRYBUFFER == pIndexBuffer->eClass());
  
  if (pIndexBuffer->pNextBuffer()) {
    bn nBuffer = pIndexBuffer->pNextBuffer()->nBufferNo();
  }
  
  mpTask->m_pDataStack->vPushObject(pIndexBuffer->pNextBuffer());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* entryBuffer.nextEntry -- get next entry from index buffer                  */
/*----------------------------------------------------------------------------*/
void Task::vEntryBufferNextEntry(MP_Task mpTask)
{
  ENTRY("vEntryBufferNextEntry"_Stz);
  
  Object* pPtrEntry = nullptr;
  Object* pPtrIndexBuffer;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndexBuffer) )
    THROW_INVALID_DSTACK("entryBuffer.nextEntry"_Stz);
  
  EntryBuffer* pIndexBuffer = _pEntryBuffer(pPtrIndexBuffer);
  ASSERT(nullptr == pIndexBuffer || CN_ENTRYBUFFER == pIndexBuffer->eClass());
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrEntry) )
    THROW_INVALID_DSTACK("entryBuffer.nextEntry"_Stz);
  
  Entry* pEntry = _pEntry(pPtrEntry);
  
  Entry *pNextEntry;
  
  if ( pPtrEntry ) {
    ASSERT(CN_ENTRY == pEntry->eClass());
    pNextEntry = pIndexBuffer->pNextEntry(pEntry);
  } else {
    pNextEntry = pIndexBuffer->pNextEntry(pIndexBuffer->pHead());
  }
  
  mpTask->m_pDataStack->vPushObject(pNextEntry);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* entryBuffer.partition@ -- get partition no of this index buffer            */
/*----------------------------------------------------------------------------*/
void Task::vEntryBufferPartitionFetch(MP_Task mpTask)
{
  ENTRY("vEntryBufferPartitionFetch"_Stz);
  
  Object* pPtrEntry = nullptr;
  Object* pPtrIndexBuffer;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndexBuffer) )
    THROW_INVALID_DSTACK("entryBuffer.partition@"_Stz);
  
  EntryBuffer* pIndexBuffer = _pEntryBuffer(pPtrIndexBuffer);
  ASSERT(nullptr == pIndexBuffer || CN_ENTRYBUFFER == pIndexBuffer->eClass());
  
  mpTask->m_pDataStack->vPushInt(pIndexBuffer->nPartitionNo());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* entryBuffer.partition! -- set partition no of this index buffer            */
/*----------------------------------------------------------------------------*/
void Task::vEntryBufferPartitionStore(MP_Task mpTask)
{
  ENTRY("vEntryBufferPartitionStore"_Stz);
  
  Object* pPtrEntry = nullptr;
  Object* pPtrIndexBuffer;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndexBuffer) )
    THROW_INVALID_DSTACK("entryBuffer.partition!"_Stz);
  
  EntryBuffer* pIndexBuffer = _pEntryBuffer(pPtrIndexBuffer);
  ASSERT(nullptr == pIndexBuffer || CN_ENTRYBUFFER == pIndexBuffer->eClass());
  
  i8 iPartitionNo;
  if ( ! mpTask->m_pDataStack->bPop(iPartitionNo) )
    THROW_INVALID_DSTACK("entryBuffer.partition!"_Stz);
  
  pIndexBuffer->vPartitionNo(iPartitionNo);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* entryBuffer::toEntryBuffer -- convert string to entry buffer pointer       */
/*----------------------------------------------------------------------------*/
void Task::vEntryBufferToEntryBuffer(MP_Task mpTask)
{
  ENTRY("vEntryBufferToEntryBuffer"_Stz);
  
  s48K sString;
  
  if ( ! mpTask->m_pDataStack->bPop(sString) )
    THROW_INVALID_DSTACK("entryBuffer::toEntryBuffer"_Stz);
  
  EntryBuffer* pEntryBuffer = reinterpret_cast<EntryBuffer*>(sString.iToI8());
  RASSERT(CN_ENTRYBUFFER == pEntryBuffer->eClass());
  
  mpTask->m_pDataStack->vPushObject(pEntryBuffer);
  
  EXIT();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
