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

extern NP_Task         gnpRootTask;
extern StepBoard*      gpStepBoard;
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;

// index: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Entry(void)
{
  vAddPrimitive("entry.matches"_Str, &vEntryMatches, false);
  vAddPrimitive("entry.row"_Str,     &vEntryRow,     false);
}

/*----------------------------------------------------------------------------*/
/* entry.matches -- return true if entry matches key                          */
/*----------------------------------------------------------------------------*/
void Task::vEntryMatches(NNP_Task nnpTask)
{
  ENTRY("vEntryMatches"_Str);
  
  Object* pPtrEntry = nullptr;
  Object* pPtrIndex = nullptr;
  ValWrk xValWrk;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrEntry) )
    THROW_INVALID_DSTACK("entry.matches"_Str);
  
  Entry* pEntry = _pEntry(pPtrEntry);
  ASSERT(CN_ENTRY == pEntry->eClass());
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("entry.matches"_Str);
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  if ( ! nnpTask->m_pDataStack->bPop(xValWrk) )
    THROW_INVALID_DSTACK("entry.matches"_Str);
  
  KeyWrk xKeyWrk;
  
  StrVal* pStrVal = _pStrVal(xValWrk.pNestedValueSelf());
  
  xKeyWrk.vAddDatum(pStrVal, pIndex->pIdxColVec()->pGet(0));
  
  //pEntry->vShow(0, pIndex->pIdxColVec());
  bool bIsEqual = false;
  bool bSame = pEntry->aKey()->bJuxtaCompare(JC_SAME, xKeyWrk.aKey(), pIndex->pIdxColVec(), bIsEqual);

  nnpTask->m_pDataStack->vPushInt(bSame ? 1 : 0);
}

/*----------------------------------------------------------------------------*/
/* entry.row -- return row from entry                                         */
/*----------------------------------------------------------------------------*/
#ifdef HIDDEN_INDEX
void Task::vEntryRow(NNP_Task nnpTask)
{
  ENTRY("vEntryRow"_Str);
  
  Object* pPtrEntry = nullptr;
  Object* pPtrIndex = nullptr;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrEntry) )
    THROW_INVALID_DSTACK("entry.row"_Str);
  
  Entry* pEntry = _pEntry(pPtrEntry);
  ASSERT(CN_ENTRY == pEntry->eClass());
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("entry.row"_Str);
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  // get the table from the index
  Table* pTable = pIndex->pTable();
  
  // get the hiddenindex from the table
  Index* pHiddenIndex = pTable->pHiddenIndex();
  
  // get the Id from the entry
  Id xId = pEntry->xId();
  
  // find the buffer from the buffer pool given the buffer no
  bn nBufferNo = xId.iBuffer();
  EntryBuffer* pEntryBuffer = reinterpret_cast<EntryBuffer*>(gpBufferPool->pToBuffer(nBufferNo));
  
  // form the hidden index key using the row id from the entry
  KeyWrk xKeyWrk;
  ValWrk xValWrk;
  xValWrk = xId.iRow();
  xKeyWrk.vAddDatum(xValWrk, pHiddenIndex->pIdxColVec()->pGet(0));

  // position in the entry buffer at the row id key
  Entry* pRowEntry = pEntryBuffer->pPosition(xKeyWrk, JC_AFTER);
  
  // return the row entry
  nnpTask->m_pDataStack->vPushObject(pRowEntry);
}

#else
void Task::vEntryRow(NNP_Task nnpTask)
{
  ENTRY("vEntryRow"_Str);
  
  Object* pPtrEntry = nullptr;
  Object* pPtrIndex = nullptr;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrEntry) )
    THROW_INVALID_DSTACK("entry.row"_Str);
  
  Entry* pEntry = _pEntry(pPtrEntry);
  ASSERT(CN_ENTRY == pEntry->eClass());
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("entry.row"_Str);
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  Id xId = pEntry->xId();
  
  Entry* pRow = xId.pRow();
  
  nnpTask->m_pDataStack->vPushObject(pRow);
}
#endif
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
