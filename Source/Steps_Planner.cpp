/*============================================================================*/
/* Steps_Planner.cpp - planner step group                                     */
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

// accessPath: /////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Planner(void)
{
  vAddPrimitive("planner.delete"_Str,   &vPlannerDelete,      false);
  vAddPrimitive("planner.genPaths"_Str, &vPlannerGenPaths,    false);
  vAddPrimitive("planner::new"_Str,     &vPlannerNew,         false);
  vAddPrimitive("planner.table@"_Str,   &vPlannerTableFetch,  false);
  vAddPrimitive("planner.table!"_Str,   &vPlannerTableStore,  false);
}

/*----------------------------------------------------------------------------*/
/* planner.delete -- delete an access path ( accessPath -- )                  */
/*----------------------------------------------------------------------------*/
void Task::vPlannerDelete(NNP_Task nnpTask)
{
ENTRY("vPlannerDelete"_Str);
  
  Object* pPtrPlanner;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrPlanner) )
    THROW_INVALID_DSTACK("planner.delete"_Str);
  
  Planner* pPlanner = _pPlanner(pPtrPlanner);

  delete pPlanner;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner.genPaths -- generate all access paths for table                    */
/*----------------------------------------------------------------------------*/
void Task::vPlannerGenPaths(NNP_Task nnpTask)
{
  ENTRY("vPlannerGenPaths"_Str);
  
  Object* pPtrPlanner;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrPlanner) )
    THROW_INVALID_DSTACK("planner.delete"_Str);
  
  Planner* pPlanner = _pPlanner(pPtrPlanner);
  
  pPlanner->vGenPaths();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner::new -- new a planner ( -- planner )                               */
/*----------------------------------------------------------------------------*/
void Task::vPlannerNew(NNP_Task nnpTask)
{
ENTRY("vPlannerNew"_Str);
  
  Planner* pPlanner = new Planner;
  
  nnpTask->m_pDataStack->vPushObject(pPlanner);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* accessPath.table@ -- fetch table from access path                          */
/*----------------------------------------------------------------------------*/
void Task::vPlannerTableFetch(NNP_Task nnpTask)
{
  ENTRY("vPlannerTableFetch"_Str);
  
  Object* pPtrPlanner;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrPlanner) )
    THROW_INVALID_DSTACK("accessPath.table@"_Str);
  Planner* pPlanner = _pPlanner(pPtrPlanner);
  
  Table* pTable = pPlanner->pTable();

  nnpTask->m_pDataStack->vPushObject(pTable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* accessPath.table! -- store table into access path                          */
/*----------------------------------------------------------------------------*/
void Task::vPlannerTableStore(NNP_Task nnpTask)
{
  ENTRY("vPlannerTableStore"_Str);
  
  Object* pPtrPlanner;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrPlanner) )
    THROW_INVALID_DSTACK("accessPath.table!"_Str);
  Planner* pPlanner = _pPlanner(pPtrPlanner);
  
  Object* pPtrTable;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("accessPath.table!"_Str);
  Table* pTable = _pTable(pPtrTable);
  
  pPlanner->vTable(pTable);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
