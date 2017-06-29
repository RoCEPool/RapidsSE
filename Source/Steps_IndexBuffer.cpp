/*============================================================================*/
/* Steps_IndexBuffer.cpp - index buffer step group                            */
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
extern StepBoard*      gpStepBoard;
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;

// indexbuffer: ////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_IndexBuffer(void)
{
  vAddPrimitive("indexBuffer::create"_Str,     &vIndexBufferCreate,      false);
  vAddPrimitive("indexBuffer.firstEntry"_Str,  &vIndexBufferFirstEntry,  false);
  vAddPrimitive("indexBuffer.insertRow"_Str,   &vIndexBufferInsertRow,   false);
  vAddPrimitive("indexBuffer.isLast@"_Str,     &vIndexBufferIsLastFetch, false);
  vAddPrimitive("indexBuffer.isLast!"_Str,     &vIndexBufferIsLastStore, false);
  vAddPrimitive("indexBuffer.nextEntry"_Str,   &vIndexBufferNextEntry,   false);
  vAddPrimitive("indexBuffer.nextBuffer"_Str,  &vIndexBufferNextBuffer,  false);
}

/*----------------------------------------------------------------------------*/
/* indexBuffer::create -- create a new index buffer                            */
/*----------------------------------------------------------------------------*/
void Task::vIndexBufferCreate(MP_Task mpTask)
{
  ENTRY("vIndexBufferCreate"_Str);
  
  Object* pPtrIndex = nullptr;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("entry.row"_Str);
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  EntryBuffer* pEntryBuffer = new EntryBuffer(pIndex->pTable()->pHiddenIndex(), 0);
  
  mpTask->m_pDataStack->vPushObject(pEntryBuffer);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* indexBuffer.nextBuffer -- get next buffer of this index buffer             */
/*----------------------------------------------------------------------------*/
void Task::vIndexBufferNextBuffer(MP_Task mpTask)
{
  ENTRY("vIndexBufferNextBuffer"_Str);
  
  Object* pPtrEntry = nullptr;
  Object* pPtrIndexBuffer;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndexBuffer) )
    THROW_INVALID_DSTACK("indexBuffer.nextBuffer"_Str);
  
  EntryBuffer* pIndexBuffer = _pEntryBuffer(pPtrIndexBuffer);
  ASSERT(nullptr == pIndexBuffer || CN_INDEXBUFFER == pIndexBuffer->eClass());
  
  if (pIndexBuffer->pNextBuffer()) {
    bn nBuffer = pIndexBuffer->pNextBuffer()->nBufferNo();
  }
  
  mpTask->m_pDataStack->vPushObject(pIndexBuffer->pNextBuffer());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* indexBuffer.firstEntry -- get first entry from index buffer                */
/*----------------------------------------------------------------------------*/
void Task::vIndexBufferFirstEntry(MP_Task mpTask)
{
  ENTRY("vIndexBufferFirstEntry"_Str);
  
  Object* pPtrEntry = nullptr;
  Object* pPtrIndexBuffer;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndexBuffer) )
    THROW_INVALID_DSTACK("indexBuffer.firstEntry!"_Str);
  
  EntryBuffer* pIndexBuffer = _pEntryBuffer(pPtrIndexBuffer);
  ASSERT(nullptr == pIndexBuffer || CN_INDEXBUFFER == pIndexBuffer->eClass()
          || CN_ROWBUFFER == pIndexBuffer->eClass());
  
  mpTask->m_pDataStack->vPushObject(pIndexBuffer->pNextEntry(pIndexBuffer->pHead()));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* indexBuffer.insertRow -- insert a row entry into the index buffer          */
/*----------------------------------------------------------------------------*/
void Task::vIndexBufferInsertRow(MP_Task mpTask)
{
  ENTRY("vIndexBufferInsertRow"_Str);
  
  Object* pPtrEntry = nullptr;
  Object* pPtrIndexBuffer;
  ValWrk xRow;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndexBuffer) )
    THROW_INVALID_DSTACK("indexBuffer.insertEntry"_Str);
  
  EntryBuffer* pIndexBuffer = _pEntryBuffer(pPtrIndexBuffer);
  ASSERT(nullptr == pIndexBuffer || CN_INDEXBUFFER == pIndexBuffer->eClass());
  
  if ( ! mpTask->m_pDataStack->bPop(xRow) )
    THROW_INVALID_DSTACK("indexBuffer.insertEntry"_Str);
  
  // create working row
  StrVal* pStrVal = _pStrVal(xRow.pNestedValueSelf());
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
  
  xRowWrk.vData(pStrVal->aStr(), pStrVal->iLength());
  xRowWrk.vShow(pIndexBuffer->pIndex()->pIdxColVec());
  Entry* pRow;
  
  if (pIndexBuffer->bInsertRow(xRowWrk, &pRow))
    mpTask->m_pDataStack->vPushInt(1);
  else
    mpTask->m_pDataStack->vPushInt(0);
  
  pIndexBuffer->vShow();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* indexBuffer.isLast@ -- get if buffer is last                               */
/*----------------------------------------------------------------------------*/
void Task::vIndexBufferIsLastFetch(MP_Task mpTask)
{
  ENTRY("vIndexBufferIsLastFetch"_Str);
  
  Object* pPtrEntry = nullptr;
  Object* pPtrIndexBuffer;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndexBuffer) )
    THROW_INVALID_DSTACK("indexBuffer.isLast@"_Str);
  
  EntryBuffer* pIndexBuffer = _pEntryBuffer(pPtrIndexBuffer);
  ASSERT(nullptr == pIndexBuffer || CN_INDEXBUFFER == pIndexBuffer->eClass());
  
  mpTask->m_pDataStack->vPushInt(pIndexBuffer->bIsLast());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* indexBuffer.isLast! -- set buffer is last                                  */
/*----------------------------------------------------------------------------*/
void Task::vIndexBufferIsLastStore(MP_Task mpTask)
{
  ENTRY("vIndexBufferIsLastStore"_Str);
  
  Object* pPtrEntry = nullptr;
  Object* pPtrIndexBuffer;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndexBuffer) )
    THROW_INVALID_DSTACK("indexBuffer.isLast!"_Str);
  
  EntryBuffer* pIndexBuffer = _pEntryBuffer(pPtrIndexBuffer);
  ASSERT(nullptr == pIndexBuffer || CN_INDEXBUFFER == pIndexBuffer->eClass());
  
  pIndexBuffer->vIsLast(true);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* indexBuffer.nextEntry -- get next entry from index buffer                  */
/*----------------------------------------------------------------------------*/
void Task::vIndexBufferNextEntry(MP_Task mpTask)
{
  ENTRY("vIndexBufferNextEntry"_Str);
  
  Object* pPtrEntry = nullptr;
  Object* pPtrIndexBuffer;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndexBuffer) )
    THROW_INVALID_DSTACK("indexBuffer.nextEntry"_Str);
  
  EntryBuffer* pIndexBuffer = _pEntryBuffer(pPtrIndexBuffer);
  ASSERT(nullptr == pIndexBuffer || CN_INDEXBUFFER == pIndexBuffer->eClass() ||
         CN_ROWBUFFER == pIndexBuffer->eClass());
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrEntry) )
    THROW_INVALID_DSTACK("indexBuffer.nextEntry"_Str);
  
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
