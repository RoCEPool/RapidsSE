/*============================================================================*/
/* Steps_Entry.cpp - entry step group                                         */
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

extern NP_Task         gnpBootTask;
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;

// index: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Entry(void)
{
  vAddPrimitive("entry.matches"_Stz,      &vEntryMatches,     false);
  vAddPrimitive("entry.toDataRow"_Stz,    &vEntryToDataRow,   false);
  vAddPrimitive("entry.embeddedRow"_Stz,  &vEntryEmbeddedRow, false);
  vAddPrimitive("entry.same"_Stz,         &vEntrySame,        false);
}

/*----------------------------------------------------------------------------*/
/* entry.matches -- return true if entry matches key                          */
/*----------------------------------------------------------------------------*/
void Task::vEntryMatches(MP_Task mpTask)
{
  ENTRY("vEntryMatches"_Stz);
  
  Object* pPtrEntry = nullptr;
  Object* pPtrIndex = nullptr;
  ValWrk xValWrk;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrEntry) )
    THROW_INVALID_DSTACK("entry.matches"_Stz);
  
  Entry* pEntry = _pEntry(pPtrEntry);
  ASSERT(CN_ENTRY == pEntry->eClass());
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("entry.matches"_Stz);
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  if ( ! mpTask->m_pDataStack->bPop(xValWrk) )
    THROW_INVALID_DSTACK("entry.matches"_Stz);
  
  KeyWrk xKeyWrk;
  
  StrVal* pStrVal = _pStrVal(xValWrk.pNestedValueSelf());
  
  xKeyWrk.vAddDatum(pStrVal, pIndex->pIdxColVec()->pGet(0));
  
  //pEntry->vShow(0, pIndex->pIdxColVec());
  bool bIsEqual = false;
  bool bSame = pEntry->aKey()->bJuxtaCompare(JC_SAME, xKeyWrk.aKey(), pIndex->pIdxColVec(), bIsEqual);

  mpTask->m_pDataStack->vPushInt(bSame ? 1 : 0);
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* entry.toDataRow -- return row from entry                                   */
/*----------------------------------------------------------------------------*/
void Task::vEntryToDataRow(MP_Task mpTask)
{
  ENTRY("vEntryRow"_Stz);
  
  Object* pPtrEntry = nullptr;
  Object* pPtrIndex = nullptr;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrEntry) )
    THROW_INVALID_DSTACK("entry.toDataRow"_Stz);
  
  Entry* pEntry = _pEntry(pPtrEntry);
  ASSERT(CN_ENTRY == pEntry->eClass() || CN_ROW == pEntry->eClass());
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("entry.toDataRow"_Stz);
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the table from the index
  Table* pTable = pIndex->pTable();
  
  // get the hiddenindex from the table
  Index* pHiddenIndex = pTable->pHiddenIndex();
  
  // get the Id from the entry
  Id xId = pEntry->xId();
  
  // find the buffer from the buffer pool given the buffer no
  bn nBufferNo = xId.iBuffer();
  EntryBuffer* pEntryBuffer = reinterpret_cast<EntryBuffer*>
     (gpBufferPool->pToBuffer(nBufferNo));
  
  ASSERT(pEntryBuffer);
  ASSERT(pEntryBuffer->eClass() == CN_ENTRYBUFFER);
  ASSERT(pEntryBuffer->pIndex());
  
  // form the hidden index key using the row id from the entry
  KeyWrk xKeyWrk;
  ValWrk xValWrk;
  xValWrk = xId.iRow();
  
  // hiddenIndex should have only one column
  ASSERT(pHiddenIndex->pIdxColVec()->nItems() == 1);
  
  // get the 0th column of the hidden index
  Column* pColumn = pHiddenIndex->pIdxColVec()->pGet(0);
  ASSERT(pColumn->iColumnNo() == -1);
  
  // add the rowId to the key
  xKeyWrk.vAddDatum(xValWrk, pColumn);

  // position in the entry buffer at the row id key
  Entry* pRowEntry = pEntryBuffer->pPosition(xKeyWrk, JC_AFTER);
  
  // return the row entry
  mpTask->m_pDataStack->vPushObject(pRowEntry);
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* entry.embeddedRow -- return row from entry                                 */
/*----------------------------------------------------------------------------*/
void Task::vEntryEmbeddedRow(MP_Task mpTask)
{
  ENTRY("vEntryEmbeddedRow"_Stz);
  
  Object* pPtrEntry = nullptr;
  Object* pPtrIndex = nullptr;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrEntry) )
    THROW_INVALID_DSTACK("entry.embeddedRow"_Stz);
  
  Entry* pEntry = _pEntry(pPtrEntry);
  ASSERT(CN_ENTRY == pEntry->eClass() || CN_ROW == pEntry->eClass());
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("entry.embeddedRow"_Stz);
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass() || CN_INDEX == pIndex->eClass());
  
  s48K sRow;
  RowData::vRowAsString(sRow, (Stz*)pEntry->aRow(), pIndex->pTable()->pColumnVec());
  
  // return the row entry
  mpTask->m_pDataStack->vPushS48K(sRow);
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* entry.same -- return true if entry is same as given entry                  */
/*----------------------------------------------------------------------------*/
void Task::vEntrySame(MP_Task mpTask)
{
  ENTRY("vEntryMatches"_Stz);
  
  Object* pPtrEntry = nullptr;
  Object* pPtrOtherEntry = nullptr;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrEntry) )
    THROW_INVALID_DSTACK("entry.same"_Stz);
  
  Entry* pEntry = _pEntry(pPtrEntry);
  ASSERT(CN_ENTRY == pEntry->eClass());
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrOtherEntry) )
    THROW_INVALID_DSTACK("entry.same"_Stz);
  
  Entry* pOtherEntry = _pEntry(pPtrOtherEntry);
  ASSERT(CN_ENTRY == pOtherEntry->eClass());
  
  mpTask->m_pDataStack->vPushInt(pEntry == pOtherEntry ? 1 : 0);
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
