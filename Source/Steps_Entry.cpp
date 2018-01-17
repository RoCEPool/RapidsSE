/*============================================================================*/
/* Steps_Entry.cpp - entry step group                                         */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern BufferPool*     gpBufferPool;
extern AttachMap*      gpAttachMap;

// index: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Entry(void)
{
  vAddPrimitive("entry.matches"_Stz,      &vEntryMatches,     false);
  vAddPrimitive("entry.toDataRow"_Stz,    &vEntryToDataRow,   false);
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
  
  Formatters* pFormatters = &mpTask->m_pSEitf->m_xFormatters;
  xKeyWrk.vAddDatum(pStrVal, pIndex->pIdxColVec()->pGet(0),pFormatters);

  //pEntry->vShow(0, pIndex,true);
  bool bIsEqual = false;
  bool bSame = pEntry->aKey()->bJuxtaCompare(JC_SAME, xKeyWrk.aKey(),
                                             pIndex->pIdxColVec(), 1, bIsEqual);

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
  ASSERT(CN_ENTRY == pEntry->eClass());
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("entry.toDataRow"_Stz);
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the table from the index
  Table* pTable = pIndex->pTable();
  
  Entry* pRowEntry = pDataRow(pTable, pEntry);
  
  // return the row entry
  mpTask->m_pDataStack->vPushObject(pRowEntry);
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

/*----------------------------------------------------------------------------*/
/* PRIVATE                                                                    */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get the dataRow entry matching entry from the tables hidden index          */
/*----------------------------------------------------------------------------*/
Entry* Task::pDataRow(Table* pTable, Entry* pEntry)
{
  
  // get the hiddenindex from the table
  Index* pHiddenIndex = pTable->pHiddenIndex();

  // get the Id from the entry
  Id xId = pEntry->xId();

  // find the buffer from the buffer pool given the buffer no
  //bn nBufferNo = xId.iBuffer();
  //EntryBuffer* pEntryBuffer = reinterpret_cast<EntryBuffer*>
  //   (gpBufferPool->pToBuffer(nBufferNo));

  EntryBuffer* pEntryBuffer = pEntry->pEntryBuffer();

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
  xKeyWrk.vAddDatumInteger(xValWrk, pColumn);

  // position in the entry buffer at the row id key
  Entry* pRowEntry = pEntryBuffer->pPosition(xKeyWrk,1,JC_AFTER);
  
  return pRowEntry;
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
