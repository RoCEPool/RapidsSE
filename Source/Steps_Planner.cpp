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
  vAddPrimitive("planner.choosePath"_Str,     &vPlannerChoosePath,     false);
  vAddPrimitive("planner.delete"_Str,         &vPlannerDelete,         false);
  vAddPrimitive("planner.fetchTechnique@"_Str,&vPlannerFetchTechniqueFetch, false);
  vAddPrimitive("planner.genPaths"_Str,       &vPlannerGenPaths,       false);
  vAddPrimitive("planner.highkey@"_Str,       &vPlannerHighKeyFetch,   false);
  vAddPrimitive("planner.indexNm@"_Str,       &vPlannerIndexNmFetch,   false);
  vAddPrimitive("planner.key@"_Str,           &vPlannerKeyFetch,       false);
  vAddPrimitive("planner.lowkey@"_Str,        &vPlannerLowKeyFetch,    false);
  vAddPrimitive("planner.matchPaths"_Str,     &vPlannerMatchPaths,     false);
  vAddPrimitive("planner::new"_Str,           &vPlannerNew,            false);
  vAddPrimitive("planner.parseWhere"_Str,     &vPlannerParseWhere,     false);
  vAddPrimitive("planner.scorePaths"_Str,     &vPlannerScorePaths,     false);
  vAddPrimitive("planner.showPaths"_Str,      &vPlannerShowPaths,      false);
  vAddPrimitive("planner.showPredicates"_Str, &vPlannerShowPredicates, false);
  vAddPrimitive("planner.table@"_Str,         &vPlannerTableFetch,     false);
  vAddPrimitive("planner.table!"_Str,         &vPlannerTableStore,     false);
}

/*----------------------------------------------------------------------------*/
/* planner.choosePath -- choose an access path                                */
/*----------------------------------------------------------------------------*/
void Task::vPlannerChoosePath(MP_Task mpTask)
{
  ENTRY((Str*)__func__);                         //! needs to be tested on Linux
  
  Object* mpPtrPlanner;
  
  if ( ! mpTask->m_pDataStack->bPop(mpPtrPlanner) )
    THROW_INVALID_DSTACK("planner.delete"_Str);
  
  Planner* mpPlanner = _pPlanner(mpPtrPlanner);
  
  mpPlanner->vChoosePath();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner.delete -- delete an access path ( accessPath -- )                  */
/*----------------------------------------------------------------------------*/
void Task::vPlannerDelete(MP_Task mpTask)
{
ENTRY("vPlannerDelete"_Str);
  
  Object* pPtrPlanner;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrPlanner) )
    THROW_INVALID_DSTACK("planner.delete"_Str);
  
  Planner* pPlanner = _pPlanner(pPtrPlanner);

  delete pPlanner;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner.fetchTechnique@ -- fetch fetch technique ( -- )                    */
/*----------------------------------------------------------------------------*/
void Task::vPlannerFetchTechniqueFetch(MP_Task mpTask)
{
  ENTRY("vPlannerDelete"_Str);
  
  Object* pPtrPlanner;
  if ( ! mpTask->m_pDataStack->bPop(pPtrPlanner) )
    THROW_INVALID_DSTACK("planner.fetchTechnique@"_Str);
  Planner* mpPlanner = _pPlanner(pPtrPlanner);
  
  PredicateVec* mpPredicateVec = mpPlanner->pPredicateVec();
  Predicate* mpPredicate = mpPredicateVec->pGet(0);
  eFT eFetchTechnique = mpPredicate->eFetchTechnique();
  
  mpTask->m_pDataStack->vPushInt(eFetchTechnique);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner.genPaths -- generate all access paths for table                    */
/*----------------------------------------------------------------------------*/
void Task::vPlannerGenPaths(MP_Task mpTask)
{
  ENTRY("vPlannerGenPaths"_Str);
  
  Object* pPtrPlanner;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrPlanner) )
    THROW_INVALID_DSTACK("planner.delete"_Str);
  
  Planner* pPlanner = _pPlanner(pPtrPlanner);
  
  pPlanner->vGenPaths();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner.highKey@ -- fetch high key ( -- )                                  */
/*----------------------------------------------------------------------------*/
void Task::vPlannerHighKeyFetch(MP_Task mpTask)
{
  ENTRY("vPlannerHighKeyFetch"_Str);
  
  Object* pPtrPlanner;
  if ( ! mpTask->m_pDataStack->bPop(pPtrPlanner) )
    THROW_INVALID_DSTACK("planner.highKey@"_Str);
  Planner* mpPlanner = _pPlanner(pPtrPlanner);
  
  PredicateVec* mpPredicateVec = mpPlanner->pPredicateVec();
  Predicate* mpPredicate = mpPredicateVec->pGet(0);
  Arg* pArg2 = mpPredicate->pArg2();
  s256 sHighKey = pArg2->sArgVal();
  
  mpTask->m_pDataStack->vPushS256(sHighKey);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner.indexNm@ -- fetch index name ( -- )                                */
/*----------------------------------------------------------------------------*/
void Task::vPlannerIndexNmFetch(MP_Task mpTask)
{
  ENTRY("vPlannerIndexFetch"_Str);
  
  Object* pPtrPlanner;
  if ( ! mpTask->m_pDataStack->bPop(pPtrPlanner) )
    THROW_INVALID_DSTACK("planner.indexNm@"_Str);
  Planner* mpPlanner = _pPlanner(pPtrPlanner);
  
  Index* pIndex = mpPlanner->pChosenIndex();
  
  s32 sIndexNm = pIndex->aIndexName();
  mpTask->m_pDataStack->vPushS32(sIndexNm);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner.key@ -- fetch key ( -- )                                           */
/*----------------------------------------------------------------------------*/
void Task::vPlannerKeyFetch(MP_Task mpTask)
{
  ENTRY("vPlannerKeyFetch"_Str);
  
  Object* pPtrPlanner;
  if ( ! mpTask->m_pDataStack->bPop(pPtrPlanner) )
    THROW_INVALID_DSTACK("planner.key@"_Str);
  Planner* mpPlanner = _pPlanner(pPtrPlanner);
  
  PredicateVec* mpPredicateVec = mpPlanner->pPredicateVec();
  Predicate* mpPredicate = mpPredicateVec->pGet(0);
  Arg* pArg1 = mpPredicate->pArg1();
  s256 sKey = pArg1->sArgVal();
  
  mpTask->m_pDataStack->vPushS256(sKey);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner.lowKey@ -- fetch low key ( -- )                                    */
/*----------------------------------------------------------------------------*/
void Task::vPlannerLowKeyFetch(MP_Task mpTask)
{
  ENTRY("vPlannerLowKeyFetch"_Str);
  
  Object* pPtrPlanner;
  if ( ! mpTask->m_pDataStack->bPop(pPtrPlanner) )
    THROW_INVALID_DSTACK("planner.lowKey@"_Str);
  Planner* mpPlanner = _pPlanner(pPtrPlanner);
  
  PredicateVec* mpPredicateVec = mpPlanner->pPredicateVec();
  Predicate* mpPredicate = mpPredicateVec->pGet(0);
  Arg* pArg2 = mpPredicate->pArg2();
  s256 sLow = pArg2->sArgVal();
  
  mpTask->m_pDataStack->vPushS256(sLow);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner.matchPaths -- attempt to match predicates with paths               */
/*----------------------------------------------------------------------------*/
void Task::vPlannerMatchPaths(MP_Task mpTask)
{
  ENTRY("vPlannerMatchnPaths"_Str);
  
  Object* pPtrPlanner;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrPlanner) )
    THROW_INVALID_DSTACK("planner.delete"_Str);
  
  Planner* pPlanner = _pPlanner(pPtrPlanner);
  
  pPlanner->vMatchPaths();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner::new -- new a planner ( -- planner )                               */
/*----------------------------------------------------------------------------*/
void Task::vPlannerNew(MP_Task mpTask)
{
ENTRY("vPlannerNew"_Str);
  
  Planner* pPlanner = new Planner;
  
  mpTask->m_pDataStack->vPushObject(pPlanner);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* accessPath.parseWhere -- parse a where clause                              */
/*----------------------------------------------------------------------------*/
void Task::vPlannerParseWhere(MP_Task mpTask)
{
  ENTRY("vPlannerParseWhere"_Str);
  
  Object* pPtrPlanner;
  if ( ! mpTask->m_pDataStack->bPop(pPtrPlanner) )
    THROW_INVALID_DSTACK("accessPath.parseWhere"_Str);
  Planner* pPlanner = _pPlanner(pPtrPlanner);
  
  s48K sWhere;
  if ( ! mpTask->m_pDataStack->bPop(sWhere) )
    THROW_INVALID_DSTACK("accessPath.parseWhere"_Str);
  
  pPlanner->vWhere(sWhere);
  
  pPlanner->vParseWhere();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* accessPath.scorePaths -- score the generated set of paths                  */
/*----------------------------------------------------------------------------*/
void Task::vPlannerScorePaths(MP_Task mpTask)
{
  ENTRY("vPlannerScorePaths"_Str);
  
  Object* pPtrPlanner;
  if ( ! mpTask->m_pDataStack->bPop(pPtrPlanner) )
    THROW_INVALID_DSTACK("accessPath.scorePaths"_Str);
  Planner* pPlanner = _pPlanner(pPtrPlanner);
  
  pPlanner->vScorePaths();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* accessPath.showPaths -- show generated paths                               */
/*----------------------------------------------------------------------------*/
void Task::vPlannerShowPaths(MP_Task mpTask)
{
  ENTRY("vPlannerShowPaths"_Str);
  
  Object* pPtrPlanner;
  if ( ! mpTask->m_pDataStack->bPop(pPtrPlanner) )
    THROW_INVALID_DSTACK("accessPath.showPaths"_Str);
  Planner* pPlanner = _pPlanner(pPtrPlanner);
  
  pPlanner->vShowPaths();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* accessPath.showPredicates -- show predicates                               */
/*----------------------------------------------------------------------------*/
void Task::vPlannerShowPredicates(MP_Task mpTask)
{
  ENTRY("vPlannerShowPredicates"_Str);
  
  Object* pPtrPlanner;
  if ( ! mpTask->m_pDataStack->bPop(pPtrPlanner) )
    THROW_INVALID_DSTACK("accessPath.showPredicates"_Str);
  Planner* pPlanner = _pPlanner(pPtrPlanner);
  
  pPlanner->vShowPredicates();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* accessPath.table@ -- fetch table from access path                          */
/*----------------------------------------------------------------------------*/
void Task::vPlannerTableFetch(MP_Task mpTask)
{
  ENTRY("vPlannerTableFetch"_Str);
  
  Object* pPtrPlanner;
  if ( ! mpTask->m_pDataStack->bPop(pPtrPlanner) )
    THROW_INVALID_DSTACK("accessPath.table@"_Str);
  Planner* pPlanner = _pPlanner(pPtrPlanner);
  
  Table* pTable = pPlanner->pTable();

  mpTask->m_pDataStack->vPushObject(pTable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* accessPath.table! -- store table into access path                          */
/*----------------------------------------------------------------------------*/
void Task::vPlannerTableStore(MP_Task mpTask)
{
  ENTRY("vPlannerTableStore"_Str);
  
  Object* pPtrPlanner;
  if ( ! mpTask->m_pDataStack->bPop(pPtrPlanner) )
    THROW_INVALID_DSTACK("accessPath.table!"_Str);
  Planner* pPlanner = _pPlanner(pPtrPlanner);
  
  Object* pPtrTable;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("accessPath.table!"_Str);
  Table* pTable = _pTable(pPtrTable);
  
  pPlanner->vTable(pTable);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
