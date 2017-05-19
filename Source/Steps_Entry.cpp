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
  ValWsp xValWsp;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrEntry) )
    THROW_INVALID_DSTACK("entry.matches"_Str);
  
  Entry* pEntry = _aEntry(pPtrEntry);
  ASSERT(CN_ENTRY == pEntry->eClass());
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("entry.matches"_Str);
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  if ( ! nnpTask->m_pDataStack->bPop(xValWsp) )
    THROW_INVALID_DSTACK("entry.matches"_Str);
  
  KeyWsp xKeyWsp;
  
  StrVal* pStrVal = _pStrVal(xValWsp.pNestedValueSelf());
  
  xKeyWsp.vAddElement(pStrVal, pIndex->pIdxColVec()->pGet(0));
  
  //pEntry->vShow(0, pIndex->pIdxColVec());
  bool bSame = pEntry->aKey()->bJuxtaCompare(PN_SAME, xKeyWsp.aKey(), pIndex->pIdxColVec());

  nnpTask->m_pDataStack->vPushInt(bSame ? 1 : 0);
}

/*----------------------------------------------------------------------------*/
/* entry.row -- return row from entry                                         */
/*----------------------------------------------------------------------------*/
void Task::vEntryRow(NNP_Task nnpTask)
{
  ENTRY("vEntryRow"_Str);
  
  Object* pPtrEntry = nullptr;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrEntry) )
    THROW_INVALID_DSTACK("index.row"_Str);
  
  Entry* pEntry = _aEntry(pPtrEntry);
  ASSERT(CN_ENTRY == pEntry->eClass());
  
  Id xId = pEntry->xId();
  
  Row* pRow = xId.pRow();
  
  nnpTask->m_pDataStack->vPushObject(pRow);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
