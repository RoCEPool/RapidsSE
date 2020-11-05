/*============================================================================*/
/* Steps_Planner.cpp - planner step group                                     */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Planner(void)
{
  vAddStep("planner[].new"_Stz,            &Step::vPlannerNewN);
  
  vAddStep("planner[].chosenPath"_Stz,     &Step::vPlannerChosenPathN);
  vAddStep("planner[].choosePaths"_Stz,    &Step::vPlannerChoosePathsN);
  vAddStep("planner[].delete"_Stz,         &Step::vPlannerDeleteN);
  vAddStep("planner[].fetchTechnique@"_Stz,&Step::vPlannerFetchTechniqueFetchN);
  vAddStep("planner[]"_Stz,                &Step::vPlannerGetN);
  vAddStep("planner[].genPaths"_Stz,       &Step::vPlannerGenPathsN);
  vAddStep("planner[].upperkey@"_Stz,      &Step::vPlannerHighKeyFetchN);
  vAddStep("planner[].indexNm@"_Stz,       &Step::vPlannerIndexNmFetchN);
  vAddStep("planner[].isNull"_Stz,         &Step::vPlannerIsNullN);
  vAddStep("planner[].key@"_Stz,           &Step::vPlannerKeyFetchN);
  vAddStep("planner[].lowkey@"_Stz,        &Step::vPlannerLowKeyFetchN);
  vAddStep("planner[].set"_Stz,            &Step::vPlannerSetN);
  vAddStep("planner[].matchPaths"_Stz,     &Step::vPlannerMatchPathsN);
  vAddStep("planner[].parseWhere"_Stz,     &Step::vPlannerParseWhereN);
  vAddStep("planner[].phrases@"_Stz,       &Step::vPlannerPhrasesFetchN);
  vAddStep("planner[].scorePaths"_Stz,     &Step::vPlannerScorePathsN);
  vAddStep("planner[].showPaths"_Stz,      &Step::vPlannerShowPathsN);
  vAddStep("planner[].table@"_Stz,         &Step::vPlannerTableFetchN);
  vAddStep("planner[].table!"_Stz,         &Step::vPlannerTableStoreN);
  
  
  /*----------------------------------------------------------------------------*/
  /*  OLD STEPS -- to go                                                        */
  /*----------------------------------------------------------------------------*/
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
void Step::vPlannerChoosePathsN(void)
{
  //! refer:  https://en.cppreference.com/w/c/language/function_definition
  
  ENTRY();
  
  Planner* pPlanner = _pPlanner(*m_pLogicUnit->m_xpPlanner);
  
  for (i8 i_Phrase = 0; i_Phrase < pPlanner->nPhrases(); i_Phrase++)
    pPlanner->vChoosePath(i_Phrase);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner.chosenPath -- push chosen access path for given phrase             */
/*----------------------------------------------------------------------------*/
void Step::vPlannerChosenPathN(void)
{
  ENTRY();
  
  Planner* pPlanner = _pPlanner(*m_pLogicUnit->m_xpPlanner);
  
  i8 i_Phrase = m_pDataStack->iPopInt(__func__);
  AccessPath* pChosenPlan = pPlanner->pChosenPath(i_Phrase);
  
  m_pDataStack->vPushPtr(pChosenPlan);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner.delete -- delete the plan ( planner -- )                           */
/*----------------------------------------------------------------------------*/
void Step::vPlannerDeleteN(void)
{
  ENTRY();
  
  Planner* pPlanner = _pPlanner(*m_pLogicUnit->m_xpPlanner);
  
  delete pPlanner;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner.fetchTechnique@ -- fetch the fetch technique for given phrase( -- )*/
/*----------------------------------------------------------------------------*/
void Step::vPlannerFetchTechniqueFetchN(void)
{
  ENTRY();
  
  Planner* mpPlanner = _pPlanner(*m_pLogicUnit->m_xpPlanner);
  
  i8 i_Phrase = m_pDataStack->iPopInt(__func__);
  eAT eFetchTechnique = mpPlanner->pChosenPath(i_Phrase)->eAccess();
  
  m_pDataStack->vPushInt(eFetchTechnique);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner.genPaths -- generate all access paths for table                    */
/*----------------------------------------------------------------------------*/
void Step::vPlannerGenPathsN(void)
{
  ENTRY();
  
  Planner* pPlanner = _pPlanner(*m_pLogicUnit->m_xpPlanner);
  
  pPlanner->vGenPaths();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner[] -- get planner                                                   */
/*----------------------------------------------------------------------------*/
void Step::vPlannerGetN(void)
{
  ENTRY();
  
  Planner* pPlanner = _pPlanner(*m_pLogicUnit->m_xpPlanner);
  
  m_pDataStack->vPushPtr(pPlanner);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner.highKey@ -- fetch high key for given phrase ( -- )                 */
/*----------------------------------------------------------------------------*/
void Step::vPlannerHighKeyFetchN(void)
{
  ENTRY();
  
  Planner* mpPlanner = _pPlanner(*m_pLogicUnit->m_xpPlanner);
  
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
void Step::vPlannerIndexNmFetchN(void)
{
  ENTRY();
  
  Planner* mpPlanner = _pPlanner(*m_pLogicUnit->m_xpPlanner);
  
  i8 i_Phrase = m_pDataStack->iPopInt(__func__);
  AccessPath* mpChosenPath = mpPlanner->pChosenPath(i_Phrase);
  
  Index* pChosenIndex = mpChosenPath->pIndex();
  
  s32 sIndexNm = pChosenIndex->szIndexNm();
  m_pDataStack->vPushStr(sIndexNm);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner[].isNull -- is planner null ?                                      */
/*----------------------------------------------------------------------------*/
void Step::vPlannerIsNullN(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(m_pLogicUnit->m_xpPlanner.bIsNull() ? 1 : 0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner.key@ -- fetch key for given phrase ( -- )                          */
/*----------------------------------------------------------------------------*/
void Step::vPlannerKeyFetchN(void)
{
  ENTRY();
  
  Planner* mpPlanner = _pPlanner(*m_pLogicUnit->m_xpPlanner);
  
  i8 i_Phrase = m_pDataStack->iPopInt(__func__);
  AccessPath* mpChosenPath = mpPlanner->pChosenPath(i_Phrase);
  
  PredicateVec* mpLowerPredicates = mpChosenPath->pLowerPredicates();
  Predicate* mpLowerPredicate = mpLowerPredicates->pGet(0);
  s256 sEqlKey = mpLowerPredicate->pArg2()->sArgVal();
  
  m_pDataStack->vPushStr(sEqlKey);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner[].lowKey@ -- fetch low key for given phrase ( -- )                 */
/*----------------------------------------------------------------------------*/
void Step::vPlannerLowKeyFetchN(void)
{
  ENTRY();
  
  Planner* mpPlanner = _pPlanner(*m_pLogicUnit->m_xpPlanner);
  
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
void Step::vPlannerMatchPathsN(void)
{
  ENTRY();
  
  Planner* pPlanner = _pPlanner(*m_pLogicUnit->m_xpPlanner);
  
  pPlanner->vMatchPaths();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner::new -- new a planner ( -- planner )                               */
/*----------------------------------------------------------------------------*/
void Step::vPlannerNewN(void)
{
  ENTRY();
  
  m_pLogicUnit->m_xpPlanner = new Planner;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner.parseWhere -- parse a where clause                                 */
/*----------------------------------------------------------------------------*/
void Step::vPlannerParseWhereN(void)
{
  ENTRY();
  
  Planner* pPlanner = _pPlanner(*m_pLogicUnit->m_xpPlanner);
  
  s48K sWhere;
  m_pDataStack->vPopStr(sWhere,__func__);
  
  pPlanner->vParseWhere(sWhere);
  pPlanner->vParsePhrases();
  
  //  i8 nLastBuffers;
  //
  //  i8 nPhrases = pPlanner->nPhrases();
  //  if (nPhrases)
  //    nLastBuffers = pPlanner->nPhrases() * gxArgs.m_nTablePtns;
  //  else
  //    nLastBuffers = gxArgs.m_nTablePtns;
  //
  //  m_pTask->m_pSession->vLastBuffers(nLastBuffers);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner.phrases@ -- fetch number of phrases ( -- nPhrases )                */
/*----------------------------------------------------------------------------*/
void Step::vPlannerPhrasesFetchN(void)
{
  ENTRY();
  
  Planner* mpPlanner = _pPlanner(*m_pLogicUnit->m_xpPlanner);
  
  i8 nPhrases = mpPlanner->nPhrases();
  
  m_pDataStack->vPushInt(nPhrases);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner.scorePaths -- score the generated set of paths                     */
/*----------------------------------------------------------------------------*/
void Step::vPlannerScorePathsN(void)
{
  ENTRY();
  
  Planner* pPlanner = _pPlanner(*m_pLogicUnit->m_xpPlanner);
  
  pPlanner->vScorePaths();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner[].set -- set planner                                               */
/*----------------------------------------------------------------------------*/
void Step::vPlannerSetN(void)
{
  ENTRY();
  
  Planner* pPlanner = _pPlanner(*m_pLogicUnit->m_xpPlanner);
  ASSERT(! pPlanner || m_pLogicUnit->m_xpPlanner.bIsNull());
  m_pLogicUnit->m_xpPlanner = _pPlanner(pPlanner);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner.showPaths -- show generated paths                                  */
/*----------------------------------------------------------------------------*/
void Step::vPlannerShowPathsN(void)
{
  ENTRY();
  
  Planner* pPlanner = _pPlanner(*m_pLogicUnit->m_xpPlanner);
  
  pPlanner->vShowPaths();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner.table@ -- fetch table from access path                             */
/*----------------------------------------------------------------------------*/
void Step::vPlannerTableFetchN(void)
{
  ENTRY();
  
  Planner* pPlanner = _pPlanner(*m_pLogicUnit->m_xpPlanner);
  
  Table* pTable = pPlanner->pTable();
  
  m_pDataStack->vPushPtr(pTable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner.table! -- store table into access path                             */
/*----------------------------------------------------------------------------*/
void Step::vPlannerTableStoreN(void)
{
  ENTRY();
  
  Planner* pPlanner = _pPlanner(*m_pLogicUnit->m_xpPlanner);
  
  Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));
  
  pPlanner->vTable(pTable);
  
  EXIT();
}



/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* OLD STEPS -- to go                                                         */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* planner.choosePaths -- choose an access path for each phrase               */
/*----------------------------------------------------------------------------*/
void Step::vPlannerChoosePaths(void)
{
  //! refer:  https://en.cppreference.com/w/c/language/function_definition
  
  ENTRY();
  
  Planner* pPlanner = _pPlanner(m_pDataStack->pPopPtr(__func__));
  
  for (i8 i_Phrase = 0; i_Phrase < pPlanner->nPhrases(); i_Phrase++)
    pPlanner->vChoosePath(i_Phrase);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner.chosenPath -- push chosen access path for given phrase             */
/*----------------------------------------------------------------------------*/
void Step::vPlannerChosenPath(void)
{
  ENTRY();
  
  Planner* pPlanner = _pPlanner(m_pDataStack->pPopPtr(__func__));
  
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
  ENTRY();
  
  Planner* pPlanner = _pPlanner(m_pDataStack->pPopPtr(__func__));

  delete pPlanner;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner.fetchTechnique@ -- fetch the fetch technique for given phrase( -- )*/
/*----------------------------------------------------------------------------*/
void Step::vPlannerFetchTechniqueFetch(void)
{
  ENTRY();
  
  Planner* mpPlanner = _pPlanner(m_pDataStack->pPopPtr(__func__));
  
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
  ENTRY();
  
  Planner* pPlanner = _pPlanner(m_pDataStack->pPopPtr(__func__));
  
  pPlanner->vGenPaths();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner.highKey@ -- fetch high key for given phrase ( -- )                 */
/*----------------------------------------------------------------------------*/
void Step::vPlannerHighKeyFetch(void)
{
  ENTRY();
  
  Planner* mpPlanner = _pPlanner(m_pDataStack->pPopPtr(__func__));
  
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
  ENTRY();
  
  Planner* mpPlanner = _pPlanner(m_pDataStack->pPopPtr(__func__));
  
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
  ENTRY();
  
  Planner* mpPlanner = _pPlanner(m_pDataStack->pPopPtr(__func__));
  
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
  ENTRY();
  
  Planner* mpPlanner = _pPlanner(m_pDataStack->pPopPtr(__func__));
  
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
  ENTRY();
  
  Planner* pPlanner = _pPlanner(m_pDataStack->pPopPtr(__func__));
  
  pPlanner->vMatchPaths();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner::new -- new a planner ( -- planner )                               */
/*----------------------------------------------------------------------------*/
void Step::vPlannerNew(void)
{
  ENTRY();
  
  Planner* pPlanner = new Planner;
  
  m_pDataStack->vPushPtr(pPlanner);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner.parseWhere -- parse a where clause                                 */
/*----------------------------------------------------------------------------*/
void Step::vPlannerParseWhere(void)
{
  ENTRY();
  
  Planner* pPlanner = _pPlanner(m_pDataStack->pPopPtr(__func__));
  
  s48K sWhere;
  m_pDataStack->vPopStr(sWhere,__func__);
  
  pPlanner->vParseWhere(sWhere);
  pPlanner->vParsePhrases();
  
//  i8 nLastBuffers;
//  
//  i8 nPhrases = pPlanner->nPhrases();
//  if (nPhrases)
//    nLastBuffers = pPlanner->nPhrases() * gxArgs.m_nTablePtns;
//  else
//    nLastBuffers = gxArgs.m_nTablePtns;
//  
//  m_pTask->m_pSession->vLastBuffers(nLastBuffers);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner.phrases@ -- fetch number of phrases ( -- nPhrases )                */
/*----------------------------------------------------------------------------*/
void Step::vPlannerPhrasesFetch(void)
{
  ENTRY();
  
  Planner* mpPlanner = _pPlanner(m_pDataStack->pPopPtr(__func__));
  
  i8 nPhrases = mpPlanner->nPhrases();
  
  m_pDataStack->vPushInt(nPhrases);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner.scorePaths -- score the generated set of paths                     */
/*----------------------------------------------------------------------------*/
void Step::vPlannerScorePaths(void)
{
  ENTRY();
  
  Planner* pPlanner = _pPlanner(m_pDataStack->pPopPtr(__func__));
  
  pPlanner->vScorePaths();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner.showPaths -- show generated paths                                  */
/*----------------------------------------------------------------------------*/
void Step::vPlannerShowPaths(void)
{
  ENTRY();
  
  Planner* pPlanner = _pPlanner(m_pDataStack->pPopPtr(__func__));
  
  pPlanner->vShowPaths();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner.table@ -- fetch table from access path                             */
/*----------------------------------------------------------------------------*/
void Step::vPlannerTableFetch(void)
{
  ENTRY();
  
  Planner* pPlanner = _pPlanner(m_pDataStack->pPopPtr(__func__));
  
  Table* pTable = pPlanner->pTable();

  m_pDataStack->vPushPtr(pTable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner.table! -- store table into access path                             */
/*----------------------------------------------------------------------------*/
void Step::vPlannerTableStore(void)
{
  ENTRY();
  
  Planner* pPlanner = _pPlanner(m_pDataStack->pPopPtr(__func__));
  
  Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));
  
  pPlanner->vTable(pTable);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
