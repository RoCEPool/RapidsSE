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
  vAddPrimitive("groupBy.applyBuffer"_Str, &vGroupByApplyBuffer, false);
  vAddPrimitive("groupBy.delete"_Str,      &vGroupByDelete,      false);
  vAddPrimitive("groupBy.end"_Str,         &vGroupByEnd,         false);
  vAddPrimitive("groupBy::new"_Str,        &vGroupByNew,         false);
  vAddPrimitive("groupBy.start"_Str,       &vGroupByStart,       false);
}

/*----------------------------------------------------------------------------*/
/* groupBy.applyBuffer -- apply buffer of lineitems to accum                  */
/*----------------------------------------------------------------------------*/
void Task::vGroupByApplyBuffer(MP_Task mpTask)
{
  ENTRY("vGroupByApplyBuffer"_Str);
  
  Object* pPtrGroupBy;
  if ( ! mpTask->m_pDataStack->bPop(pPtrGroupBy) )
    THROW_INVALID_DSTACK("groupBy.applyBuffer"_Str);
  GroupBy* pGroupBy = _pGroupBy(pPtrGroupBy);
  
  EXIT();
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
/* groupBy.end -- end group by processing                                     */
/*----------------------------------------------------------------------------*/
void Task::vGroupByEnd(MP_Task mpTask)
{
  ENTRY("vGroupByEnd"_Str);
  
  Object* pPtrGroupBy;
  if ( ! mpTask->m_pDataStack->bPop(pPtrGroupBy) )
    THROW_INVALID_DSTACK("groupBy.end"_Str);
  GroupBy* pGroupBy = _pGroupBy(pPtrGroupBy);
  
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

/*----------------------------------------------------------------------------*/
/* groupBy.start -- start group by processing                                 */
/*----------------------------------------------------------------------------*/
void Task::vGroupByStart(MP_Task mpTask)
{
  ENTRY("vGroupByStart"_Str);
  
  Object* pPtrGroupBy;
  if ( ! mpTask->m_pDataStack->bPop(pPtrGroupBy) )
    THROW_INVALID_DSTACK("groupBy.start"_Str);
  GroupBy* pGroupBy = _pGroupBy(pPtrGroupBy);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
