/*============================================================================*/
/* Steps_PathIdentifier.cpp - Path Identifier step group                      */
/*============================================================================*/

#include "BASE.h"
#include "STORE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "StepTiming.hpp"

#include "Node_I.hpp"
#include "Arg.hpp"
#include "RelOp.hpp"
#include "Predicate.hpp"
#include "PredicateVec.hpp"
#include "AccessPath.hpp"
#include "PathIdentifier.hpp"
#include "Response.hpp"
#include "Result.hpp"
#include "Job.hpp"
#include "LogicUnit.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Planner(void)
{
  vAddStep("PathIdentifier[].new"_Stz,            &Step::vPathIdentifierNew);
  
  vAddStep("PathIdentifier[].chosenPath"_Stz,     &Step::vPathIdentifierChosenPath);
  vAddStep("PathIdentifier[].choosePaths"_Stz,    &Step::vPathIdentifierChoosePaths);
  vAddStep("PathIdentifier[].delete"_Stz,         &Step::vPathIdentifierDelete);
  vAddStep("PathIdentifier[].fetchTech@"_Stz,&Step::vPathIdentifierFetchTechniqueFetch);
  vAddStep("PathIdentifier[]"_Stz,                &Step::vPathIdentifierGet);
  vAddStep("PathIdentifier[].genPaths"_Stz,       &Step::vPathIdentifierGenPaths);
  vAddStep("PathIdentifier[].upperkey@"_Stz,      &Step::vPathIdentifierHighKeyFetch);
  vAddStep("PathIdentifier[].indexNm@"_Stz,       &Step::vPathIdentifierIndexNmFetch);
  vAddStep("PathIdentifier[].isNull"_Stz,         &Step::vPathIdentifierIsNull);
  vAddStep("PathIdentifier[].key@"_Stz,           &Step::vPathIdentifierKeyFetch);
  vAddStep("PathIdentifier[].lowkey@"_Stz,        &Step::vPathIdentifierLowKeyFetch);
  vAddStep("PathIdentifier[].set"_Stz,            &Step::vPathIdentifierSet);
  vAddStep("PathIdentifier[].matchPaths"_Stz,     &Step::vPathIdentifierMatchPaths);
  vAddStep("PathIdentifier[].parseWhere"_Stz,     &Step::vPathIdentifierParseWhere);
  vAddStep("PathIdentifier[].phrases@"_Stz,       &Step::vPathIdentifierPhrasesFetch);
  vAddStep("PathIdentifier[].scorePaths"_Stz,     &Step::vPathIdentifierScorePaths);
  vAddStep("PathIdentifier[].showPaths"_Stz,      &Step::vPathIdentifierShowPaths);
  vAddStep("PathIdentifier[].table@"_Stz,         &Step::vPathIdentifierTableFetch);
  vAddStep("PathIdentifier[].table!"_Stz,         &Step::vPathIdentifierTableStore);
}

/*----------------------------------------------------------------------------*/
/* planner.choosePaths -- choose an access path for each phrase               */
/*----------------------------------------------------------------------------*/
void Step::vPathIdentifierChoosePaths(void)
{
  //! refer:  https://en.cppreference.com/w/c/language/function_definition
  
  ENTRY();
  
  PathIdentifier* pPlanner = _pPlanner(*( m_pJob->pLogicUnit_job() )->m_xpPlanner);
  
  for (i8 i_Phrase = 0; i_Phrase < pPlanner->nPhrases(); i_Phrase++)
    pPlanner->vChoosePath(i_Phrase);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner.chosenPath -- push chosen access path for given phrase             */
/*----------------------------------------------------------------------------*/
void Step::vPathIdentifierChosenPath(void)
{
  ENTRY();
  
  PathIdentifier* pPlanner = _pPlanner(*( m_pJob->pLogicUnit_job() )->m_xpPlanner);
  
  i8 i_Phrase = m_pDataStack->iPopInt(__func__);
  AccessPath* pChosenPlan = pPlanner->pChosenPath(i_Phrase);
  
  m_pDataStack->vPushPtr(pChosenPlan);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner.delete -- delete the plan ( planner -- )                           */
/*----------------------------------------------------------------------------*/
void Step::vPathIdentifierDelete(void)
{
  ENTRY();
  
  PathIdentifier* pPlanner = _pPlanner(*( m_pJob->pLogicUnit_job() )->m_xpPlanner);
  
  delete pPlanner;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner.fetchTechnique@ -- fetch the fetch technique for given phrase( -- )*/
/*----------------------------------------------------------------------------*/
void Step::vPathIdentifierFetchTechniqueFetch(void)
{
  ENTRY();
  
  PathIdentifier* mpPlanner = _pPlanner(*( m_pJob->pLogicUnit_job() )->m_xpPlanner);
  
  i8 i_Phrase = m_pDataStack->iPopInt(__func__);
  eAT eFetchTechnique = mpPlanner->pChosenPath(i_Phrase)->eAccess();
  
  m_pDataStack->vPushInt(eFetchTechnique);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner.genPaths -- generate all access paths for table                    */
/*----------------------------------------------------------------------------*/
void Step::vPathIdentifierGenPaths(void)
{
  ENTRY();
  
  PathIdentifier* pPlanner = _pPlanner(*( m_pJob->pLogicUnit_job() )->m_xpPlanner);
  
  pPlanner->vGenPaths();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PathIdentifier[] -- get planner                                                   */
/*----------------------------------------------------------------------------*/
void Step::vPathIdentifierGet(void)
{
  ENTRY();
  
  PathIdentifier* pPlanner = _pPlanner(*( m_pJob->pLogicUnit_job() )->m_xpPlanner);
  
  m_pDataStack->vPushPtr(pPlanner);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner.highKey@ -- fetch high key for given phrase ( -- )                 */
/*----------------------------------------------------------------------------*/
void Step::vPathIdentifierHighKeyFetch(void)
{
  ENTRY();
  
  PathIdentifier* mpPlanner = _pPlanner(*( m_pJob->pLogicUnit_job() )->m_xpPlanner);
  
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
void Step::vPathIdentifierIndexNmFetch(void)
{
  ENTRY();
  
  PathIdentifier* mpPlanner = _pPlanner(*( m_pJob->pLogicUnit_job() )->m_xpPlanner);
  
  i8 i_Phrase = m_pDataStack->iPopInt(__func__);
  AccessPath* mpChosenPath = mpPlanner->pChosenPath(i_Phrase);
  
  Index* pChosenIndex = mpChosenPath->pIndex();
  
  s32 sIndexNm = pChosenIndex->szIndexNm();
  m_pDataStack->vPushStr(sIndexNm);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PathIdentifier[].isNull -- is planner null ?                                      */
/*----------------------------------------------------------------------------*/
void Step::vPathIdentifierIsNull(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(( m_pJob->pLogicUnit_job() )->m_xpPlanner.bIsNull() ? 1 : 0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner.key@ -- fetch key for given phrase ( -- )                          */
/*----------------------------------------------------------------------------*/
void Step::vPathIdentifierKeyFetch(void)
{
  ENTRY();
  
  PathIdentifier* mpPlanner = _pPlanner(*( m_pJob->pLogicUnit_job() )->m_xpPlanner);
  
  i8 i_Phrase = m_pDataStack->iPopInt(__func__);
  AccessPath* mpChosenPath = mpPlanner->pChosenPath(i_Phrase);
  
  PredicateVec* mpLowerPredicates = mpChosenPath->pLowerPredicates();
  Predicate* mpLowerPredicate = mpLowerPredicates->pGet(0);
  s256 sEqlKey = mpLowerPredicate->pArg2()->sArgVal();
  
  m_pDataStack->vPushStr(sEqlKey);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PathIdentifier[].lowKey@ -- fetch low key for given phrase ( -- )                 */
/*----------------------------------------------------------------------------*/
void Step::vPathIdentifierLowKeyFetch(void)
{
  ENTRY();
  
  PathIdentifier* mpPlanner = _pPlanner(*( m_pJob->pLogicUnit_job() )->m_xpPlanner);
  
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
void Step::vPathIdentifierMatchPaths(void)
{
  ENTRY();
  
  PathIdentifier* pPlanner = _pPlanner(*( m_pJob->pLogicUnit_job() )->m_xpPlanner);
  
  pPlanner->vMatchPaths();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner::new -- new a planner ( -- planner )                               */
/*----------------------------------------------------------------------------*/
void Step::vPathIdentifierNew(void)
{
  ENTRY();
  
  ( m_pJob->pLogicUnit_job() )->m_xpPlanner = new PathIdentifier;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner.parseWhere -- parse a where clause                                 */
/*----------------------------------------------------------------------------*/
void Step::vPathIdentifierParseWhere(void)
{
  ENTRY();
  
  PathIdentifier* pPlanner = _pPlanner(*( m_pJob->pLogicUnit_job() )->m_xpPlanner);
  
  s48K sWhere;
  m_pDataStack->vPopStr(sWhere,__func__);
  
  pPlanner->vParseWhere(sWhere);
  pPlanner->vParsePhrases();
  
  //  i8 nLastBuffers;
  //
  //  i8 nPhrases = pPlanner->nPhrases();
  //  if (nPhrases)
  //    nLastBuffers = pPlanner->nPhrases() * gxArgs.m_nPtnsPerTable;
  //  else
  //    nLastBuffers = gxArgs.m_nPtnsPerTable;
  //
  //  m_pTask->m_pSession->vLastBuffers(nLastBuffers);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner.phrases@ -- fetch number of phrases ( -- nPhrases )                */
/*----------------------------------------------------------------------------*/
void Step::vPathIdentifierPhrasesFetch(void)
{
  ENTRY();
  
  PathIdentifier* mpPlanner = _pPlanner(*( m_pJob->pLogicUnit_job() )->m_xpPlanner);
  
  i8 nPhrases = mpPlanner->nPhrases();
  
  m_pDataStack->vPushInt(nPhrases);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner.scorePaths -- score the generated set of paths                     */
/*----------------------------------------------------------------------------*/
void Step::vPathIdentifierScorePaths(void)
{
  ENTRY();
  
  PathIdentifier* pPlanner = _pPlanner(*( m_pJob->pLogicUnit_job() )->m_xpPlanner);
  
  pPlanner->vScorePaths();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PathIdentifier[].set -- set planner                                               */
/*----------------------------------------------------------------------------*/
void Step::vPathIdentifierSet(void)
{
  ENTRY();
  
  PathIdentifier* pPlanner = _pPlanner(*( m_pJob->pLogicUnit_job() )->m_xpPlanner);
  ASSERT(! pPlanner || ( m_pJob->pLogicUnit_job() )->m_xpPlanner.bIsNull());
  ( m_pJob->pLogicUnit_job() )->m_xpPlanner = _pPlanner(pPlanner);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner.showPaths -- show generated paths                                  */
/*----------------------------------------------------------------------------*/
void Step::vPathIdentifierShowPaths(void)
{
  ENTRY();
  
  PathIdentifier* pPlanner = _pPlanner(*( m_pJob->pLogicUnit_job() )->m_xpPlanner);
  
  pPlanner->vShowPaths();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner.table@ -- fetch table from access path                             */
/*----------------------------------------------------------------------------*/
void Step::vPathIdentifierTableFetch(void)
{
  ENTRY();
  
  PathIdentifier* pPlanner = _pPlanner(*( m_pJob->pLogicUnit_job() )->m_xpPlanner);
  
  Table* pTable = pPlanner->pTable();
  
  m_pDataStack->vPushPtr(pTable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planner.table! -- store table into access path                             */
/*----------------------------------------------------------------------------*/
void Step::vPathIdentifierTableStore(void)
{
  ENTRY();
  
  PathIdentifier* pPlanner = _pPlanner(*( m_pJob->pLogicUnit_job() )->m_xpPlanner);
  
  Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));
  
  pPlanner->vTable(pTable);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
