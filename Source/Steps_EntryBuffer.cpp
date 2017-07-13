/*============================================================================*/
/* Steps_EntryBuffer.cpp - entry buffer step group                            */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap*      gpAttachMap;
extern BufferPool*     gpBufferPool;
extern Heap*           gpMediumHeap;
extern BPA*            gpBPA;
extern Dic*            gpMainDic;
extern Word*           gpWord;

extern Grid*           gpGrid;
extern StepArray*      gpStepArray;
extern ThreadList*     gpThreadList;
extern TaskList*       gpTaskList;
extern TxCollection*   gpTxCollection;

extern NP_Task         gnpRootTask;
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;
extern BitMap*         gpBitMap;

// indexbuffer: ////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_EntryBuffer(void)
{
  vAddPrimitive("entryBuffer::create"_Str,     &vEntryBufferCreate,      false);
  vAddPrimitive("entryBuffer.delete"_Str,      &vEntryBufferDelete,      false);
  vAddPrimitive("entryBuffer.firstEntry"_Str,  &vEntryBufferFirstEntry,  false);
  vAddPrimitive("entryBuffer.insertRow"_Str,   &vEntryBufferInsertRow,   false);
  vAddPrimitive("entryBuffer.isLast@"_Str,     &vEntryBufferIsLastFetch, false);
  vAddPrimitive("entryBuffer.isLast!"_Str,     &vEntryBufferIsLastStore, false);
  vAddPrimitive("entryBuffer.iterate"_Str,     &vEntryBufferIterate,     false);
  vAddPrimitive("entryBuffer.nextEntry"_Str,   &vEntryBufferNextEntry,   false);
  vAddPrimitive("entryBuffer.nextBuffer"_Str,  &vEntryBufferNextBuffer,  false);
}

/*----------------------------------------------------------------------------*/
/* entryBuffer::create -- create a new index buffer                           */
/*----------------------------------------------------------------------------*/
void Task::vEntryBufferCreate(MP_Task mpTask)
{
  ENTRY("vEntryBufferCreate"_Str);
  
  Object* pPtrIndex = nullptr;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("entryBuffer::create"_Str);
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  EntryBuffer* pEntryBuffer = new EntryBuffer(pIndex->pTable()->pHiddenIndex(), 0);
  
  mpTask->m_pDataStack->vPushObject(pEntryBuffer);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* entryBuffer.delete -- delete the index buffer                              */
/*----------------------------------------------------------------------------*/
void Task::vEntryBufferDelete(MP_Task mpTask)
{
  ENTRY("vEntryBufferDelete"_Str);
  
  Object* pPtrIndexBuffer = nullptr;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndexBuffer) )
    THROW_INVALID_DSTACK("entryBuffer.delete"_Str);
  
  EntryBuffer* pIndexBuffer = _pEntryBuffer(pPtrIndexBuffer);
  ASSERT(CN_ENTRYBUFFER == pIndexBuffer->eClass());
  
  delete pIndexBuffer;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* entryBuffer.nextBuffer -- get next buffer of this index buffer             */
/*----------------------------------------------------------------------------*/
void Task::vEntryBufferNextBuffer(MP_Task mpTask)
{
  ENTRY("vEntryBufferNextBuffer"_Str);
  
  Object* pPtrEntry = nullptr;
  Object* pPtrIndexBuffer;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndexBuffer) )
    THROW_INVALID_DSTACK("entryBuffer.nextBuffer"_Str);
  
  EntryBuffer* pIndexBuffer = _pEntryBuffer(pPtrIndexBuffer);
  ASSERT(nullptr == pIndexBuffer || CN_ENTRYBUFFER == pIndexBuffer->eClass());
  
  if (pIndexBuffer->pNextBuffer()) {
    bn nBuffer = pIndexBuffer->pNextBuffer()->nBufferNo();
  }
  
  mpTask->m_pDataStack->vPushObject(pIndexBuffer->pNextBuffer());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* entryBuffer.firstEntry -- get first entry from index buffer                */
/*----------------------------------------------------------------------------*/
void Task::vEntryBufferFirstEntry(MP_Task mpTask)
{
  ENTRY("vEntryBufferFirstEntry"_Str);
  
  Object* pPtrEntry = nullptr;
  Object* pPtrIndexBuffer;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndexBuffer) )
    THROW_INVALID_DSTACK("entryBuffer.firstEntry!"_Str);
  
  EntryBuffer* pIndexBuffer = _pEntryBuffer(pPtrIndexBuffer);
  ASSERT(nullptr == pIndexBuffer || CN_ENTRYBUFFER == pIndexBuffer->eClass());
  
  mpTask->m_pDataStack->vPushObject(pIndexBuffer->pNextEntry(pIndexBuffer->pHead()));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* entryBuffer.insertRow -- insert a row entry into the index buffer          */
/*----------------------------------------------------------------------------*/
//#define ENTWRK_APPEND
#ifdef ENTWRK_APPEND
void Task::vEntryBufferInsertRow(MP_Task mpTask)
{
  ENTRY("vEntryBufferInsertRow"_Str);
 
  Object* pPtrEntry = nullptr;
  Object* pPtrIndexBuffer;
  s256 sRow;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndexBuffer) )
    THROW_INVALID_DSTACK("entryBuffer.insertRow"_Str);
  
  EntryBuffer* pIndexBuffer = _pEntryBuffer(pPtrIndexBuffer);
  ASSERT(nullptr == pIndexBuffer || CN_ENTRYBUFFER == pIndexBuffer->eClass());
  
  if ( ! mpTask->m_pDataStack->bPop(sRow) )
    THROW_INVALID_DSTACK("entryBuffer.insertRow"_Str);

  // create working row
  ValWrk xValWrk;
  
  //----------------------------------------------------------------------------
  // form the rowId
  bn nBufferNo = pIndexBuffer->nBufferNo();
  i8 nRowNo = pIndexBuffer->nEntries();
  Id rowId(*nBufferNo, nRowNo);
  
  //----------------------------------------------------------------------------
  // form the key
  xValWrk = nRowNo;
  
  KeyWrk xKeyWrk;
  Column* pColumn = pIndexBuffer->pIndex()->pIdxColVec()->pGet(0);
  ASSERT(pColumn->iColumnNo() == -1);
  xKeyWrk.vAddDatum(xValWrk, pColumn);
  
  //----------------------------------------------------------------------------
  // add the id and key to EntWrk
  EntWrk xRowWrk(rowId, xKeyWrk);
  
  xRowWrk.vData(sRow.aStr(), sRow.iLength());
  //xRowWrk.vShow(pIndexBuffer->pIndex()->pIdxColVec());
  
  Entry* pRow;
  
  if (pIndexBuffer->bInsertRow(xRowWrk, &pRow))
    mpTask->m_pDataStack->vPushInt(1);
  else
    mpTask->m_pDataStack->vPushInt(0);
  
  ASSERT( ! pRow->nOffsets() );
  
  EXIT();
}
#else
void Task::vEntryBufferInsertRow(MP_Task mpTask)
{
  ENTRY("vEntryBufferInsertRow"_Str);
  
  Object* pPtrEntry = nullptr;
  Object* pPtrIndexBuffer;
  s256 sRow;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndexBuffer) )
    THROW_INVALID_DSTACK("entryBuffer.insertRow"_Str);
  
  EntryBuffer* pIndexBuffer = _pEntryBuffer(pPtrIndexBuffer);
  ASSERT(nullptr == pIndexBuffer || CN_ENTRYBUFFER == pIndexBuffer->eClass());
  
  if ( ! mpTask->m_pDataStack->bPop(sRow) )
    THROW_INVALID_DSTACK("entryBuffer.insertRow"_Str);

   Entry* pRow;
   
   if (pIndexBuffer->bInsertRow(sRow, &pRow))
   mpTask->m_pDataStack->vPushInt(1);
   else
   mpTask->m_pDataStack->vPushInt(0);
   
   //pRow->vShow(0, pIndexBuffer->pIndex());
  
  ASSERT( ! pRow->nOffsets() );
  
  EXIT();
}

#endif
/*----------------------------------------------------------------------------*/
/* entryBuffer.isLast@ -- get if buffer is last                               */
/*----------------------------------------------------------------------------*/
void Task::vEntryBufferIsLastFetch(MP_Task mpTask)
{
  ENTRY("vEntryBufferIsLastFetch"_Str);
  
  Object* pPtrEntry = nullptr;
  Object* pPtrIndexBuffer;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndexBuffer) )
    THROW_INVALID_DSTACK("entryBuffer.isLast@"_Str);
  
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
  ENTRY("vEntryBufferIsLastStore"_Str);
  
  Object* pPtrEntry = nullptr;
  Object* pPtrIndexBuffer;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndexBuffer) )
    THROW_INVALID_DSTACK("entryBuffer.isLast!"_Str);
  
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
  ENTRY("vEntryBufferIiterate"_Str);
  
  Object* pPtrIndexBuffer;
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndexBuffer) )
    THROW_INVALID_DSTACK("entryBuffer.iterate"_Str);
  EntryBuffer* pIndexBuffer = _pEntryBuffer(pPtrIndexBuffer);
  ASSERT(nullptr == pIndexBuffer || CN_ENTRYBUFFER == pIndexBuffer->eClass());
  
  Object* pPtrEntry = nullptr;
  if ( ! mpTask->m_pDataStack->bPop(pPtrEntry) )
    THROW_INVALID_DSTACK("entryBuffer.iterate"_Str);
  Entry* pUpperIndexEntry = _pEntry(pPtrEntry);
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrEntry) )
    THROW_INVALID_DSTACK("entryBuffer.iterate"_Str);
  Entry* pLowerIndexEntry = _pEntry(pPtrEntry);
  
  Object* pPtrGroupBy = nullptr;
  if ( ! mpTask->m_pDataStack->bPop(pPtrGroupBy) )
    THROW_INVALID_DSTACK("entryBuffer.iterate"_Str);
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
    // Entry* pLineItemEntry = GroupBy::pToLineItemEntry(pIndex,pIndexEntry);
    //pGroupByOp->vApplyEntry(pLineItemEntry);
    
    // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
    // set bit on
    
    Id xId = pIndexEntry->xId();
    
    bn nBufferNo = xId.iBuffer();
    i8 iRow = xId.iRow();
    
    EntryBuffer* pDataBuffer = _pEntryBuffer(gpBufferPool->pToBuffer(nBufferNo));
    ASSERT(pDataBuffer);
    
    BitArray* pBitArray = pDataBuffer->pBitArray();
    ASSERT(pBitArray);
    pBitArray->vSetOn(iRow);

    //gpBitMap->vSetOn(*nBufferNo,iRow);
    
    // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -

    nRows++;
    
    // test for end
    if (pIndexEntry == pUpperIndexEntry)
      break;
    
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
  
  Table* pTable = pIndex->pTable();
  Slots* pSlots = pTable->pSlots();
  Partition* pPartition = pSlots->pPartition(0);
  IndexData* pIndexData = pPartition->pHiddenIndexData();
  
  EntryBuffer* pRowBuffer = pIndexData->pFirstBuffer();
  
  while ( (pRowBuffer = pRowBuffer->pNextBuffer()) ) {
    BitArray* pBitArray = pRowBuffer->pBitArray();
    //vTrace("BitArray: %lld bits on\n"_Str,pBitArray->nBitsOn());
    Entry* pRowEntry = pRowBuffer->pFirstEntry();
    while ( (pRowEntry = pRowBuffer->pNextEntry(pRowEntry)) ) {
      pGroupByOp->vApplyEntry(pRowEntry);
    }
  }
    
  vTrace("Rows processed: %lld\n"_Str,nRows);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* entryBuffer.nextEntry -- get next entry from index buffer                  */
/*----------------------------------------------------------------------------*/
void Task::vEntryBufferNextEntry(MP_Task mpTask)
{
  ENTRY("vEntryBufferNextEntry"_Str);
  
  Object* pPtrEntry = nullptr;
  Object* pPtrIndexBuffer;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndexBuffer) )
    THROW_INVALID_DSTACK("entryBuffer.nextEntry"_Str);
  
  EntryBuffer* pIndexBuffer = _pEntryBuffer(pPtrIndexBuffer);
  ASSERT(nullptr == pIndexBuffer || CN_ENTRYBUFFER == pIndexBuffer->eClass());
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrEntry) )
    THROW_INVALID_DSTACK("entryBuffer.nextEntry"_Str);
  
  Entry* pEntry = _pEntry(pPtrEntry);
  
  Entry *pNextEntry;
  
  if ( pPtrEntry ) {
    ASSERT(CN_ENTRY == pEntry->eClass() || CN_ROW == pEntry->eClass());
    pNextEntry = pIndexBuffer->pNextEntry(pEntry);
  } else {
    pNextEntry = pIndexBuffer->pNextEntry(pIndexBuffer->pHead());
  }
  
  mpTask->m_pDataStack->vPushObject(pNextEntry);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
