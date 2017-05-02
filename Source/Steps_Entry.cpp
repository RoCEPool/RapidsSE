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
  vAddPrimitive("entry.row"_Str,   &vEntryRow,   false);
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
