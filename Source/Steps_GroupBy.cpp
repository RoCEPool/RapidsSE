/*============================================================================*/
/* Steps_GroupBy.cpp -- Group By step group                                   */
/*============================================================================*/

#include "StorageEngine.h"

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

// accum: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_GroupBy(void)
{
  vAddPrimitive("groupBy.delete"_Str, &vAccumDelete, false);
  vAddPrimitive("groupBy::new"_Str,   &vAccumNew,    false);
}

/*----------------------------------------------------------------------------*/
/* groupBy.delete -- delete group by                                          */
/*----------------------------------------------------------------------------*/
void Task::vGroupByDelete(MP_Task mpTask)
{
  ENTRY("vGroupByDelete"_Str);
  
  Object* pPtrGroupBy;
  if ( ! mpTask->m_pDataStack->bPop(pPtrGroupBy) )
    THROW_INVALID_DSTACK("groupBy.delete"_Str);
  GroupBy* pGroupBy = _pGroupBy(pPtrGroupBy);
  
  delete pGroupBy;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* groupBy::new -- group by new                                               */
/*----------------------------------------------------------------------------*/
void Task::vGroupByNew(MP_Task mpTask)
{
  ENTRY("vGroupByNew"_Str);
  
  GroupBy* pGroupBy = new GroupBy;
  
  mpTask->m_pDataStack->vPushObject(pGroupBy);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
