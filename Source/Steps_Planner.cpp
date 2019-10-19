/*============================================================================*/
/* Steps_Planner.cpp - planner step group                                     */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Planner(void)
{
  vAddStep("planner::new"_Stz,           &Step::vPlannerNew);
  
  vAddStep("planner.choosePaths"_Stz,    &Step::vPlannerChoosePaths);
  vAddStep("planner.chosenPath"_Stz,     &Step::vPlannerChosenPath);
  vAddStep("planner.delete"_Stz,         &Step::vPlannerDelete);
  vAddStep("planner.fetchTechnique@"_Stz,&Step::vPlannerFetchTechniqueFetch);
  vAddStep("planner.genPaths"_Stz,       &Step::vPlannerGenPaths);
  vAddStep("planner.highkey@"_Stz,       &Step::vPlannerHighKeyFetch);
  vAddStep("planner.indexNm@"_Stz,       &Step::vPlannerIndexNmFetch);
  vAddStep("planner.key@"_Stz,           &Step::vPlannerKeyFetch);
  vAddStep("planner.lowkey@"_Stz,        &Step::vPlannerLowKeyFetch);
  vAddStep("planner.matchPaths"_Stz,     &Step::vPlannerMatchPaths);
  vAddStep("planner.phrases@"_Stz,       &Step::vPlannerPhrasesFetch);
  vAddStep("planner.parseWhere"_Stz,     &Step::vPlannerParseWhere);
  vAddStep("planner.scorePaths"_Stz,     &Step::vPlannerScorePaths);
  vAddStep("planner.showPaths"_Stz,      &Step::vPlannerShowPaths);
  vAddStep("planner.table@"_Stz,         &Step::vPlannerTableFetch);
  vAddStep("planner.table!"_Stz,         &Step::vPlannerTableStore);
}

/*----------------------------------------------------------------------------*/
/* planner.choosePaths -- choose an access path for each phrase               */
/*----------------------------------------------------------------------------*/
void Step::vPlannerChoosePaths(MP_Task mpTask, Forth* pForth)
{
  //! refer:  https://en.cppreference.com/w/c/language/function_definition
  
  ENTRY((Stz*)__func__);                         //! needs to be tested on Linux
  
  Object* mpPtrPlanner;
  
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(mpPtrPlanner) )
    vThrow(__FILE__,__LINE__,FK_DSTACK,__STEP__);
  Planner* mpPlanner = _pPlanner(mpPtrPlanner);
  
  for (i8 i_Phrase = 0; i_Phrase < mpPlanner->nPhrases(); i_Phrase++)
    mpPlanner->vChoosePath(i_Phrase);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner.chosenPath -- push chosen access path for given phrase             */
/*----------------------------------------------------------------------------*/
void Step::vPlannerChosenPath(MP_Task mpTask, Forth* pForth)
{
  ENTRY((Stz*)__func__);                         //! needs to be tested on Linux
  
  Object* mpPtrPlanner;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(mpPtrPlanner) )
    vThrow(__FILE__,__LINE__,FK_DSTACK,__STEP__);
  Planner* mpPlanner = _pPlanner(mpPtrPlanner);
  
  i8 i_Phrase;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(i_Phrase) )
    vThrow(__FILE__,__LINE__,FK_DSTACK,__STEP__);
  AccessPath* pChosenPlan = mpPlanner->pChosenPath(i_Phrase);
  
  mpTask->m_pForth->m_pDataStack->vPushPointer(pChosenPlan);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner.delete -- delete the plan ( planner -- )                           */
/*----------------------------------------------------------------------------*/
void Step::vPlannerDelete(MP_Task mpTask, Forth* pForth)
{
ENTRY("vPlannerDelete"_Stz);
  
  Object* pPtrPlanner;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrPlanner) )
    vThrow(__FILE__,__LINE__,FK_DSTACK,__STEP__);
  Planner* pPlanner = _pPlanner(pPtrPlanner);

  delete pPlanner;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner.fetchTechnique@ -- fetch the fetch technique for given phrase( -- )*/
/*----------------------------------------------------------------------------*/
void Step::vPlannerFetchTechniqueFetch(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vPlannerDelete"_Stz);
  
  Object* pPtrPlanner;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrPlanner) )
    vThrow(__FILE__,__LINE__,FK_DSTACK,__STEP__);
  Planner* mpPlanner = _pPlanner(pPtrPlanner);
  
  i8 i_Phrase;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(i_Phrase) )
    vThrow(__FILE__,__LINE__,FK_DSTACK,__STEP__);
  eAT eFetchTechnique = mpPlanner->pChosenPath(i_Phrase)->eAccess();
  
  mpTask->m_pForth->m_pDataStack->vPushI8(eFetchTechnique);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner.genPaths -- generate all access paths for table                    */
/*----------------------------------------------------------------------------*/
void Step::vPlannerGenPaths(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vPlannerGenPaths"_Stz);
  
  Object* pPtrPlanner;
  
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrPlanner) )
    vThrow(__FILE__,__LINE__,FK_DSTACK,__STEP__);
  
  Planner* pPlanner = _pPlanner(pPtrPlanner);
  
  pPlanner->vGenPaths();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner.highKey@ -- fetch high key for given phrase ( -- )                 */
/*----------------------------------------------------------------------------*/
void Step::vPlannerHighKeyFetch(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vPlannerHighKeyFetch"_Stz);
  
  Object* pPtrPlanner;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrPlanner) )
    vThrow(__FILE__,__LINE__,FK_DSTACK,__STEP__);
  Planner* mpPlanner = _pPlanner(pPtrPlanner);
  
  i8 i_Phrase;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(i_Phrase) )
    vThrow(__FILE__,__LINE__,FK_DSTACK,__STEP__);
  AccessPath* mpChosenPath = mpPlanner->pChosenPath(i_Phrase);
  
  PredicateVec* mpUpperPredicates = mpChosenPath->pUpperPredicates();
  Predicate* mpUpperPredicate = mpUpperPredicates->pGet(0);
  s256 sHighKey = mpUpperPredicate->pArg2()->sArgVal();
  
  mpTask->m_pForth->m_pDataStack->vPushS256(sHighKey);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner.indexNm@ -- fetch index name for given phrase ( -- )               */
/*----------------------------------------------------------------------------*/
void Step::vPlannerIndexNmFetch(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vPlannerIndexFetch"_Stz);
  
  Object* pPtrPlanner;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrPlanner) )
    vThrow(__FILE__,__LINE__,FK_DSTACK,__STEP__);
  Planner* mpPlanner = _pPlanner(pPtrPlanner);
  
  i8 i_Phrase;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(i_Phrase) )
    vThrow(__FILE__,__LINE__,FK_DSTACK,__STEP__);
  AccessPath* mpChosenPath = mpPlanner->pChosenPath(i_Phrase);
  
  Index* pChosenIndex = mpChosenPath->pIndex();
  
  s32 sIndexNm = pChosenIndex->aIndexName();
  mpTask->m_pForth->m_pDataStack->vPushS32(sIndexNm);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner.key@ -- fetch key for given phrase ( -- )                          */
/*----------------------------------------------------------------------------*/
void Step::vPlannerKeyFetch(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vPlannerKeyFetch"_Stz);
  
  Object* pPtrPlanner;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrPlanner) )
    vThrow(__FILE__,__LINE__,FK_DSTACK,__STEP__);
  Planner* mpPlanner = _pPlanner(pPtrPlanner);
  
  i8 i_Phrase;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(i_Phrase) )
    vThrow(__FILE__,__LINE__,FK_DSTACK,__STEP__);
  AccessPath* mpChosenPath = mpPlanner->pChosenPath(i_Phrase);
  
  PredicateVec* mpLowerPredicates = mpChosenPath->pLowerPredicates();
  Predicate* mpLowerPredicate = mpLowerPredicates->pGet(0);
  s256 sEqlKey = mpLowerPredicate->pArg2()->sArgVal();
  
  mpTask->m_pForth->m_pDataStack->vPushS256(sEqlKey);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner.lowKey@ -- fetch low key for given phrase ( -- )                   */
/*----------------------------------------------------------------------------*/
void Step::vPlannerLowKeyFetch(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vPlannerLowKeyFetch"_Stz);
  
  Object* pPtrPlanner;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrPlanner) )
    vThrow(__FILE__,__LINE__,FK_DSTACK,__STEP__);
  Planner* mpPlanner = _pPlanner(pPtrPlanner);
  
  i8 i_Phrase;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(i_Phrase) )
    vThrow(__FILE__,__LINE__,FK_DSTACK,__STEP__);
  AccessPath* mpChosenPath = mpPlanner->pChosenPath(i_Phrase);
  
  PredicateVec* mpLowerPredicates = mpChosenPath->pLowerPredicates();
  Predicate* mpLowerPredicate = mpLowerPredicates->pGet(0);
  s256 sLowKey = mpLowerPredicate->pArg2()->sArgVal();
  
  mpTask->m_pForth->m_pDataStack->vPushS256(sLowKey);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner.matchPaths -- attempt to match predicates with paths               */
/*----------------------------------------------------------------------------*/
void Step::vPlannerMatchPaths(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vPlannerMatchnPaths"_Stz);
  
  Object* pPtrPlanner;
  
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrPlanner) )
    vThrow(__FILE__,__LINE__,FK_DSTACK,__STEP__);
  
  Planner* pPlanner = _pPlanner(pPtrPlanner);
  
  pPlanner->vMatchPaths();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner::new -- new a planner ( -- planner )                               */
/*----------------------------------------------------------------------------*/
void Step::vPlannerNew(MP_Task mpTask, Forth* pForth)
{
ENTRY("vPlannerNew"_Stz);
  
  Planner* pPlanner = new Planner;
  
  mpTask->m_pForth->m_pDataStack->vPushPointer(pPlanner);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner.parseWhere -- parse a where clause                                 */
/*----------------------------------------------------------------------------*/
void Step::vPlannerParseWhere(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vPlannerParseWhere"_Stz);
  
  Object* pPtrPlanner;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrPlanner) )
    vThrow(__FILE__,__LINE__,FK_DSTACK,__STEP__);
  Planner* pPlanner = _pPlanner(pPtrPlanner);
  
  s48K sWhere;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(sWhere) )
    vThrow(__FILE__,__LINE__,FK_DSTACK,__STEP__);
  
  pPlanner->vParseWhere(sWhere);
  pPlanner->vParsePhrases();
  
//  i8 nLastBuffers;
//  
//  i8 nPhrases = pPlanner->nPhrases();
//  if (nPhrases)
//    nLastBuffers = pPlanner->nPhrases() * gxArguments.m_nSlotsPerNode;
//  else
//    nLastBuffers = gxArguments.m_nSlotsPerNode;
//  
//  mpTask->m_pSession->vLastBuffers(nLastBuffers);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner.phrases@ -- fetch number of phrases ( -- nPhrases )                */
/*----------------------------------------------------------------------------*/
void Step::vPlannerPhrasesFetch(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vPlannerPhrasesFetch"_Stz);
  
  Object* pPtrPlanner;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrPlanner) )
    vThrow(__FILE__,__LINE__,FK_DSTACK,__STEP__);
  Planner* mpPlanner = _pPlanner(pPtrPlanner);
  
  i8 nPhrases = mpPlanner->nPhrases();
  
  mpTask->m_pForth->m_pDataStack->vPushI8(nPhrases);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner.scorePaths -- score the generated set of paths                     */
/*----------------------------------------------------------------------------*/
void Step::vPlannerScorePaths(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vPlannerScorePaths"_Stz);
  
  Object* pPtrPlanner;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrPlanner) )
    vThrow(__FILE__,__LINE__,FK_DSTACK,__STEP__);
  Planner* pPlanner = _pPlanner(pPtrPlanner);
  
  pPlanner->vScorePaths();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner.showPaths -- show generated paths                                  */
/*----------------------------------------------------------------------------*/
void Step::vPlannerShowPaths(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vPlannerShowPaths"_Stz);
  
  Object* pPtrPlanner;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrPlanner) )
    vThrow(__FILE__,__LINE__,FK_DSTACK,__STEP__);
  Planner* pPlanner = _pPlanner(pPtrPlanner);
  
  pPlanner->vShowPaths();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner.table@ -- fetch table from access path                             */
/*----------------------------------------------------------------------------*/
void Step::vPlannerTableFetch(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vPlannerTableFetch"_Stz);
  
  Object* pPtrPlanner;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrPlanner) )
    vThrow(__FILE__,__LINE__,FK_DSTACK,__STEP__);
  Planner* pPlanner = _pPlanner(pPtrPlanner);
  
  Table* pTable = pPlanner->pTable();

  mpTask->m_pForth->m_pDataStack->vPushPointer(pTable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner.table! -- store table into access path                             */
/*----------------------------------------------------------------------------*/
void Step::vPlannerTableStore(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vPlannerTableStore"_Stz);
  
  Object* pPtrPlanner;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrPlanner) )
    vThrow(__FILE__,__LINE__,FK_DSTACK,__STEP__);
  Planner* pPlanner = _pPlanner(pPtrPlanner);
  
  Object* pPtrTable;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrTable) )
    vThrow(__FILE__,__LINE__,FK_DSTACK,__STEP__);
  Table* pTable = _pTable(pPtrTable);
  
  pPlanner->vTable(pTable);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
