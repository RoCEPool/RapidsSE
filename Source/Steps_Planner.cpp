/*============================================================================*/
/* Steps_Planner.cpp - planner step group                                     */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap*      gpAttachMap;
extern BufferPool*     gpBufferPool;
extern BPA*            gpBPA;
extern Dic*            gpMainDic;
extern Word*           gpWord;

extern Grid*           gpGrid;
extern StepArray*      gpStepArray;
extern ThreadList*     gpThreadList;
extern TaskList*       gpTaskList;

extern NP_Task         gnpBootTask;
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;
extern Arguments       gxArguments;

// accessPath: /////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Planner(void)
{
  vAddPrimitive("planner.choosePaths"_Stz,    &vPlannerChoosePaths,    false);
  vAddPrimitive("planner.chosenPath"_Stz,     &vPlannerChosenPath,     false);
  vAddPrimitive("planner.delete"_Stz,         &vPlannerDelete,         false);
  vAddPrimitive("planner.fetchTechnique@"_Stz,&vPlannerFetchTechniqueFetch, false);
  vAddPrimitive("planner.genPaths"_Stz,       &vPlannerGenPaths,       false);
  vAddPrimitive("planner.highkey@"_Stz,       &vPlannerHighKeyFetch,   false);
  vAddPrimitive("planner.indexNm@"_Stz,       &vPlannerIndexNmFetch,   false);
  vAddPrimitive("planner.key@"_Stz,           &vPlannerKeyFetch,       false);
  vAddPrimitive("planner.lowkey@"_Stz,        &vPlannerLowKeyFetch,    false);
  vAddPrimitive("planner.matchPaths"_Stz,     &vPlannerMatchPaths,     false);
  vAddPrimitive("planner.phrases@"_Stz,       &vPlannerPhrasesFetch,   false);
  vAddPrimitive("planner::new"_Stz,           &vPlannerNew,            false);
  vAddPrimitive("planner.parseWhere"_Stz,     &vPlannerParseWhere,     false);
  vAddPrimitive("planner.scorePaths"_Stz,     &vPlannerScorePaths,     false);
  vAddPrimitive("planner.showPaths"_Stz,      &vPlannerShowPaths,      false);
  vAddPrimitive("planner.table@"_Stz,         &vPlannerTableFetch,     false);
  vAddPrimitive("planner.table!"_Stz,         &vPlannerTableStore,     false);
}

/*----------------------------------------------------------------------------*/
/* planner.choosePaths -- choose an access path for each phrase               */
/*----------------------------------------------------------------------------*/
void Task::vPlannerChoosePaths(MP_Task mpTask)
{
  ENTRY((Stz*)__func__);                         //! needs to be tested on Linux
  
  Object* mpPtrPlanner;
  
  if ( ! mpTask->m_pDataStack->bPop(mpPtrPlanner) )
    THROW_INVALID_DSTACK((Stz*)__func__);
  
  Planner* mpPlanner = _pPlanner(mpPtrPlanner);
  
  for (i8 i_Phrase = 0; i_Phrase < mpPlanner->nPhrases(); i_Phrase++)
    mpPlanner->vChoosePath(i_Phrase);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner.chosenPath -- push chosen access path for given phrase             */
/*----------------------------------------------------------------------------*/
void Task::vPlannerChosenPath(MP_Task mpTask)
{
  ENTRY((Stz*)__func__);                         //! needs to be tested on Linux
  
  Object* mpPtrPlanner;
  if ( ! mpTask->m_pDataStack->bPop(mpPtrPlanner) )
    THROW_INVALID_DSTACK("planner.chosenPath"_Stz);
  Planner* mpPlanner = _pPlanner(mpPtrPlanner);
  
  i8 i_Phrase;
  if ( ! mpTask->m_pDataStack->bPop(i_Phrase) )
    THROW_INVALID_DSTACK("planner.chosenPath"_Stz);
  AccessPath* pChosenPlan = mpPlanner->pChosenPath(i_Phrase);
  
  mpTask->m_pDataStack->vPushObject(pChosenPlan);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner.delete -- delete the plan ( planner -- )                           */
/*----------------------------------------------------------------------------*/
void Task::vPlannerDelete(MP_Task mpTask)
{
ENTRY("vPlannerDelete"_Stz);
  
  Object* pPtrPlanner;
  if ( ! mpTask->m_pDataStack->bPop(pPtrPlanner) )
    THROW_INVALID_DSTACK("planner.delete"_Stz);
  Planner* pPlanner = _pPlanner(pPtrPlanner);

  delete pPlanner;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner.fetchTechnique@ -- fetch the fetch technique for given phrase( -- )*/
/*----------------------------------------------------------------------------*/
void Task::vPlannerFetchTechniqueFetch(MP_Task mpTask)
{
  ENTRY("vPlannerDelete"_Stz);
  
  Object* pPtrPlanner;
  if ( ! mpTask->m_pDataStack->bPop(pPtrPlanner) )
    THROW_INVALID_DSTACK("planner.fetchTechnique@"_Stz);
  Planner* mpPlanner = _pPlanner(pPtrPlanner);
  
  i8 i_Phrase;
  if ( ! mpTask->m_pDataStack->bPop(i_Phrase) )
    THROW_INVALID_DSTACK("planner.fetchTechnique@"_Stz);
  eAT eFetchTechnique = mpPlanner->pChosenPath(i_Phrase)->eAccess();
  
  mpTask->m_pDataStack->vPushInt(eFetchTechnique);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner.genPaths -- generate all access paths for table                    */
/*----------------------------------------------------------------------------*/
void Task::vPlannerGenPaths(MP_Task mpTask)
{
  ENTRY("vPlannerGenPaths"_Stz);
  
  Object* pPtrPlanner;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrPlanner) )
    THROW_INVALID_DSTACK("planner.delete"_Stz);
  
  Planner* pPlanner = _pPlanner(pPtrPlanner);
  
  pPlanner->vGenPaths();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner.highKey@ -- fetch high key for given phrase ( -- )                 */
/*----------------------------------------------------------------------------*/
void Task::vPlannerHighKeyFetch(MP_Task mpTask)
{
  ENTRY("vPlannerHighKeyFetch"_Stz);
  
  Object* pPtrPlanner;
  if ( ! mpTask->m_pDataStack->bPop(pPtrPlanner) )
    THROW_INVALID_DSTACK("planner.highKey@"_Stz);
  Planner* mpPlanner = _pPlanner(pPtrPlanner);
  
  i8 i_Phrase;
  if ( ! mpTask->m_pDataStack->bPop(i_Phrase) )
    THROW_INVALID_DSTACK("planner.highKey@"_Stz);
  AccessPath* mpChosenPath = mpPlanner->pChosenPath(i_Phrase);
  
  PredicateVec* mpUpperPredicates = mpChosenPath->pUpperPredicates();
  Predicate* mpUpperPredicate = mpUpperPredicates->pGet(0);
  s256 sHighKey = mpUpperPredicate->pArg2()->sArgVal();
  
  mpTask->m_pDataStack->vPushS256(sHighKey);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner.indexNm@ -- fetch index name for given phrase ( -- )               */
/*----------------------------------------------------------------------------*/
void Task::vPlannerIndexNmFetch(MP_Task mpTask)
{
  ENTRY("vPlannerIndexFetch"_Stz);
  
  Object* pPtrPlanner;
  if ( ! mpTask->m_pDataStack->bPop(pPtrPlanner) )
    THROW_INVALID_DSTACK("planner.indexNm@"_Stz);
  Planner* mpPlanner = _pPlanner(pPtrPlanner);
  
  i8 i_Phrase;
  if ( ! mpTask->m_pDataStack->bPop(i_Phrase) )
    THROW_INVALID_DSTACK("planner.indexNm@"_Stz);
  AccessPath* mpChosenPath = mpPlanner->pChosenPath(i_Phrase);
  
  Index* pChosenIndex = mpChosenPath->pIndex();
  
  s32 sIndexNm = pChosenIndex->aIndexName();
  mpTask->m_pDataStack->vPushS32(sIndexNm);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner.key@ -- fetch key for given phrase ( -- )                          */
/*----------------------------------------------------------------------------*/
void Task::vPlannerKeyFetch(MP_Task mpTask)
{
  ENTRY("vPlannerKeyFetch"_Stz);
  
  Object* pPtrPlanner;
  if ( ! mpTask->m_pDataStack->bPop(pPtrPlanner) )
    THROW_INVALID_DSTACK("planner.key@"_Stz);
  Planner* mpPlanner = _pPlanner(pPtrPlanner);
  
  i8 i_Phrase;
  if ( ! mpTask->m_pDataStack->bPop(i_Phrase) )
    THROW_INVALID_DSTACK("planner.key@"_Stz);
  AccessPath* mpChosenPath = mpPlanner->pChosenPath(i_Phrase);
  
  PredicateVec* mpLowerPredicates = mpChosenPath->pLowerPredicates();
  Predicate* mpLowerPredicate = mpLowerPredicates->pGet(0);
  s256 sEqlKey = mpLowerPredicate->pArg2()->sArgVal();
  
  mpTask->m_pDataStack->vPushS256(sEqlKey);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner.lowKey@ -- fetch low key for given phrase ( -- )                   */
/*----------------------------------------------------------------------------*/
void Task::vPlannerLowKeyFetch(MP_Task mpTask)
{
  ENTRY("vPlannerLowKeyFetch"_Stz);
  
  Object* pPtrPlanner;
  if ( ! mpTask->m_pDataStack->bPop(pPtrPlanner) )
    THROW_INVALID_DSTACK("planner.lowKey@"_Stz);
  Planner* mpPlanner = _pPlanner(pPtrPlanner);
  
  i8 i_Phrase;
  if ( ! mpTask->m_pDataStack->bPop(i_Phrase) )
    THROW_INVALID_DSTACK("planner.lowKey@"_Stz);
  AccessPath* mpChosenPath = mpPlanner->pChosenPath(i_Phrase);
  
  PredicateVec* mpLowerPredicates = mpChosenPath->pLowerPredicates();
  Predicate* mpLowerPredicate = mpLowerPredicates->pGet(0);
  s256 sLowKey = mpLowerPredicate->pArg2()->sArgVal();
  
  mpTask->m_pDataStack->vPushS256(sLowKey);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner.matchPaths -- attempt to match predicates with paths               */
/*----------------------------------------------------------------------------*/
void Task::vPlannerMatchPaths(MP_Task mpTask)
{
  ENTRY("vPlannerMatchnPaths"_Stz);
  
  Object* pPtrPlanner;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrPlanner) )
    THROW_INVALID_DSTACK("planner.delete"_Stz);
  
  Planner* pPlanner = _pPlanner(pPtrPlanner);
  
  pPlanner->vMatchPaths();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner::new -- new a planner ( -- planner )                               */
/*----------------------------------------------------------------------------*/
void Task::vPlannerNew(MP_Task mpTask)
{
ENTRY("vPlannerNew"_Stz);
  
  Planner* pPlanner = new Planner;
  
  mpTask->m_pDataStack->vPushObject(pPlanner);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner.parseWhere -- parse a where clause                                 */
/*----------------------------------------------------------------------------*/
void Task::vPlannerParseWhere(MP_Task mpTask)
{
  ENTRY("vPlannerParseWhere"_Stz);
  
  Object* pPtrPlanner;
  if ( ! mpTask->m_pDataStack->bPop(pPtrPlanner) )
    THROW_INVALID_DSTACK("planner.parseWhere"_Stz);
  Planner* pPlanner = _pPlanner(pPtrPlanner);
  
  s48K sWhere;
  if ( ! mpTask->m_pDataStack->bPop(sWhere) )
    THROW_INVALID_DSTACK("planner.parseWhere"_Stz);
  
  pPlanner->vParseWhere(sWhere);
  pPlanner->vParsePhrases();
  
//  i8 nLastBuffers;
//  
//  i8 nPhrases = pPlanner->nPhrases();
//  if (nPhrases)
//    nLastBuffers = pPlanner->nPhrases() * gxArguments.m_nSlots;
//  else
//    nLastBuffers = gxArguments.m_nSlots;
//  
//  mpTask->m_pSEitf->vLastBuffers(nLastBuffers);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner.phrases@ -- fetch number of phrases ( -- nPhrases )                */
/*----------------------------------------------------------------------------*/
void Task::vPlannerPhrasesFetch(MP_Task mpTask)
{
  ENTRY("vPlannerPhrasesFetch"_Stz);
  
  Object* pPtrPlanner;
  if ( ! mpTask->m_pDataStack->bPop(pPtrPlanner) )
    THROW_INVALID_DSTACK("planner.phrases@"_Stz);
  Planner* mpPlanner = _pPlanner(pPtrPlanner);
  
  i8 nPhrases = mpPlanner->nPhrases();
  
  mpTask->m_pDataStack->vPushInt(nPhrases);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner.scorePaths -- score the generated set of paths                     */
/*----------------------------------------------------------------------------*/
void Task::vPlannerScorePaths(MP_Task mpTask)
{
  ENTRY("vPlannerScorePaths"_Stz);
  
  Object* pPtrPlanner;
  if ( ! mpTask->m_pDataStack->bPop(pPtrPlanner) )
    THROW_INVALID_DSTACK("planner.scorePaths"_Stz);
  Planner* pPlanner = _pPlanner(pPtrPlanner);
  
  pPlanner->vScorePaths();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner.showPaths -- show generated paths                                  */
/*----------------------------------------------------------------------------*/
void Task::vPlannerShowPaths(MP_Task mpTask)
{
  ENTRY("vPlannerShowPaths"_Stz);
  
  Object* pPtrPlanner;
  if ( ! mpTask->m_pDataStack->bPop(pPtrPlanner) )
    THROW_INVALID_DSTACK("planner.showPaths"_Stz);
  Planner* pPlanner = _pPlanner(pPtrPlanner);
  
  pPlanner->vShowPaths();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner.table@ -- fetch table from access path                             */
/*----------------------------------------------------------------------------*/
void Task::vPlannerTableFetch(MP_Task mpTask)
{
  ENTRY("vPlannerTableFetch"_Stz);
  
  Object* pPtrPlanner;
  if ( ! mpTask->m_pDataStack->bPop(pPtrPlanner) )
    THROW_INVALID_DSTACK("planner.table@"_Stz);
  Planner* pPlanner = _pPlanner(pPtrPlanner);
  
  Table* pTable = pPlanner->pTable();

  mpTask->m_pDataStack->vPushObject(pTable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner.table! -- store table into access path                             */
/*----------------------------------------------------------------------------*/
void Task::vPlannerTableStore(MP_Task mpTask)
{
  ENTRY("vPlannerTableStore"_Stz);
  
  Object* pPtrPlanner;
  if ( ! mpTask->m_pDataStack->bPop(pPtrPlanner) )
    THROW_INVALID_DSTACK("planner.table!"_Stz);
  Planner* pPlanner = _pPlanner(pPtrPlanner);
  
  Object* pPtrTable;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("planner.table!"_Stz);
  Table* pTable = _pTable(pPtrTable);
  
  pPlanner->vTable(pTable);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
