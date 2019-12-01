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
void Step::vPlannerChoosePaths(void)
{
  //! refer:  https://en.cppreference.com/w/c/language/function_definition
  
  ENTRY(); // ENTRY((Stz*)__func__);                         //! needs to be tested on Linux
  
  Object* pPtrPlanner = m_pDataStack->pPopPtr(__func__); Planner* pPlanner = _pPlanner(pPtrPlanner);
  
  for (i8 i_Phrase = 0; i_Phrase < pPlanner->nPhrases(); i_Phrase++)
    pPlanner->vChoosePath(i_Phrase);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner.chosenPath -- push chosen access path for given phrase             */
/*----------------------------------------------------------------------------*/
void Step::vPlannerChosenPath(void)
{
  ENTRY(); // ENTRY((Stz*)__func__);                         //! needs to be tested on Linux
  
  Object* pPtrPlanner = m_pDataStack->pPopPtr(__func__); Planner* pPlanner = _pPlanner(pPtrPlanner);
  
  i8 i_Phrase = m_pDataStack->iPopInt(__func__);
  AccessPath* pChosenPlan = pPlanner->pChosenPath(i_Phrase);
  
  m_pDataStack->vPushPtr(pChosenPlan);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner.delete -- delete the plan ( planner -- )                           */
/*----------------------------------------------------------------------------*/
void Step::vPlannerDelete(void)
{
  ENTRY(); // ENTRY("vPlannerDelete"_Stz);
  
  Object* pPtrPlanner = m_pDataStack->pPopPtr(__func__); Planner* pPlanner = _pPlanner(pPtrPlanner);

  delete pPlanner;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner.fetchTechnique@ -- fetch the fetch technique for given phrase( -- )*/
/*----------------------------------------------------------------------------*/
void Step::vPlannerFetchTechniqueFetch(void)
{
  ENTRY(); // ENTRY("vPlannerDelete"_Stz);
  
  Object* pPtrPlanner = m_pDataStack->pPopPtr(__func__); Planner* mpPlanner = _pPlanner(pPtrPlanner);
  
  i8 i_Phrase = m_pDataStack->iPopInt(__func__);
  eAT eFetchTechnique = mpPlanner->pChosenPath(i_Phrase)->eAccess();
  
  m_pDataStack->vPushInt(eFetchTechnique);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner.genPaths -- generate all access paths for table                    */
/*----------------------------------------------------------------------------*/
void Step::vPlannerGenPaths(void)
{
  ENTRY(); // ENTRY("vPlannerGenPaths"_Stz);
  
  Object* pPtrPlanner = m_pDataStack->pPopPtr(__func__); Planner* pPlanner = _pPlanner(pPtrPlanner);
  
  pPlanner->vGenPaths();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner.highKey@ -- fetch high key for given phrase ( -- )                 */
/*----------------------------------------------------------------------------*/
void Step::vPlannerHighKeyFetch(void)
{
  ENTRY(); // ENTRY("vPlannerHighKeyFetch"_Stz);
  
  Object* pPtrPlanner = m_pDataStack->pPopPtr(__func__); Planner* mpPlanner = _pPlanner(pPtrPlanner);
  
  i8 i_Phrase = m_pDataStack->iPopInt(__func__);
  AccessPath* mpChosenPath = mpPlanner->pChosenPath(i_Phrase);
  
  PredicateVec* mpUpperPredicates = mpChosenPath->pUpperPredicates();
  Predicate* mpUpperPredicate = mpUpperPredicates->pGet(0);
  s256 sHighKey = mpUpperPredicate->pArg2()->sArgVal();
  
  m_pDataStack->vPushStr(sHighKey);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner.indexNm@ -- fetch index name for given phrase ( -- )               */
/*----------------------------------------------------------------------------*/
void Step::vPlannerIndexNmFetch(void)
{
  ENTRY(); // ENTRY("vPlannerIndexFetch"_Stz);
  
  Object* pPtrPlanner = m_pDataStack->pPopPtr(__func__); Planner* mpPlanner = _pPlanner(pPtrPlanner);
  
  i8 i_Phrase = m_pDataStack->iPopInt(__func__);
  AccessPath* mpChosenPath = mpPlanner->pChosenPath(i_Phrase);
  
  Index* pChosenIndex = mpChosenPath->pIndex();
  
  s32 sIndexNm = pChosenIndex->szIndexNm();
  m_pDataStack->vPushStr(sIndexNm);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner.key@ -- fetch key for given phrase ( -- )                          */
/*----------------------------------------------------------------------------*/
void Step::vPlannerKeyFetch(void)
{
  ENTRY(); // ENTRY("vPlannerKeyFetch"_Stz);
  
  Object* pPtrPlanner = m_pDataStack->pPopPtr(__func__); Planner* mpPlanner = _pPlanner(pPtrPlanner);
  
  i8 i_Phrase = m_pDataStack->iPopInt(__func__);
  AccessPath* mpChosenPath = mpPlanner->pChosenPath(i_Phrase);
  
  PredicateVec* mpLowerPredicates = mpChosenPath->pLowerPredicates();
  Predicate* mpLowerPredicate = mpLowerPredicates->pGet(0);
  s256 sEqlKey = mpLowerPredicate->pArg2()->sArgVal();
  
  m_pDataStack->vPushStr(sEqlKey);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner.lowKey@ -- fetch low key for given phrase ( -- )                   */
/*----------------------------------------------------------------------------*/
void Step::vPlannerLowKeyFetch(void)
{
  ENTRY(); // ENTRY("vPlannerLowKeyFetch"_Stz);
  
  Object* pPtrPlanner = m_pDataStack->pPopPtr(__func__); Planner* mpPlanner = _pPlanner(pPtrPlanner);
  
  i8 i_Phrase = m_pDataStack->iPopInt(__func__);
  AccessPath* mpChosenPath = mpPlanner->pChosenPath(i_Phrase);
  
  PredicateVec* mpLowerPredicates = mpChosenPath->pLowerPredicates();
  Predicate* mpLowerPredicate = mpLowerPredicates->pGet(0);
  s256 sLowKey = mpLowerPredicate->pArg2()->sArgVal();
  
  m_pDataStack->vPushStr(sLowKey);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner.matchPaths -- attempt to match predicates with paths               */
/*----------------------------------------------------------------------------*/
void Step::vPlannerMatchPaths(void)
{
  ENTRY(); // ENTRY("vPlannerMatchnPaths"_Stz);
  
  Object* pPtrPlanner = m_pDataStack->pPopPtr(__func__); Planner* pPlanner = _pPlanner(pPtrPlanner);
  
  pPlanner->vMatchPaths();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner::new -- new a planner ( -- planner )                               */
/*----------------------------------------------------------------------------*/
void Step::vPlannerNew(void)
{
  ENTRY(); // ENTRY("vPlannerNew"_Stz);
  
  Planner* pPlanner = new Planner;
  
  m_pDataStack->vPushPtr(pPlanner);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner.parseWhere -- parse a where clause                                 */
/*----------------------------------------------------------------------------*/
void Step::vPlannerParseWhere(void)
{
  ENTRY(); // ENTRY("vPlannerParseWhere"_Stz);
  
  Object* pPtrPlanner = m_pDataStack->pPopPtr(__func__); Planner* pPlanner = _pPlanner(pPtrPlanner);
  
  s48K sWhere;
  m_pDataStack->vPopStr(sWhere,__func__);
  
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
//  m_pTask->m_pSession->vLastBuffers(nLastBuffers);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner.phrases@ -- fetch number of phrases ( -- nPhrases )                */
/*----------------------------------------------------------------------------*/
void Step::vPlannerPhrasesFetch(void)
{
  ENTRY(); // ENTRY("vPlannerPhrasesFetch"_Stz);
  
  Object* pPtrPlanner = m_pDataStack->pPopPtr(__func__); Planner* mpPlanner = _pPlanner(pPtrPlanner);
  
  i8 nPhrases = mpPlanner->nPhrases();
  
  m_pDataStack->vPushInt(nPhrases);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner.scorePaths -- score the generated set of paths                     */
/*----------------------------------------------------------------------------*/
void Step::vPlannerScorePaths(void)
{
  ENTRY(); // ENTRY("vPlannerScorePaths"_Stz);
  
  Object* pPtrPlanner = m_pDataStack->pPopPtr(__func__); Planner* pPlanner = _pPlanner(pPtrPlanner);
  
  pPlanner->vScorePaths();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner.showPaths -- show generated paths                                  */
/*----------------------------------------------------------------------------*/
void Step::vPlannerShowPaths(void)
{
  ENTRY(); // ENTRY("vPlannerShowPaths"_Stz);
  
  Object* pPtrPlanner = m_pDataStack->pPopPtr(__func__); Planner* pPlanner = _pPlanner(pPtrPlanner);
  
  pPlanner->vShowPaths();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner.table@ -- fetch table from access path                             */
/*----------------------------------------------------------------------------*/
void Step::vPlannerTableFetch(void)
{
  ENTRY(); // ENTRY("vPlannerTableFetch"_Stz);
  
  Object* pPtrPlanner = m_pDataStack->pPopPtr(__func__); Planner* pPlanner = _pPlanner(pPtrPlanner);
  
  Table* pTable = pPlanner->pTable();

  m_pDataStack->vPushPtr(pTable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner.table! -- store table into access path                             */
/*----------------------------------------------------------------------------*/
void Step::vPlannerTableStore(void)
{
  ENTRY(); // ENTRY("vPlannerTableStore"_Stz);
  
  Object* pPtrPlanner = m_pDataStack->pPopPtr(__func__); Planner* pPlanner = _pPlanner(pPtrPlanner);
  
  Object* pPtrTable = m_pDataStack->pPopPtr(__func__); Table* pTable = _pTable(pPtrTable);
  
  pPlanner->vTable(pTable);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
