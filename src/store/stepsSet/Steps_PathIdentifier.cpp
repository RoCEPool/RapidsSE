/*============================================================================*/
/* Steps_PathIdentifier.cpp - Path Identifier step group                      */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "SkipRoots.hpp"
#include "EntBuffer.hpp"
#include "IndexTree.hpp"
#include "CrvBuffer.hpp"
#include "TablePtn.hpp"
#include "Table.hpp"
#include "Node_I.hpp"
#include "Arg.hpp"
#include "RelOp.hpp"
#include "Predicate.hpp"
#include "PredicateVec.hpp"
#include "AccessPath.hpp"
#include "PathIdentifier.hpp"
#include "Job.hpp"
#include "LogicUnit.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_PathIdentifier(void)
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
/* PathIdentifier[].choosePaths -- choose an access path for each phrase      */
/*----------------------------------------------------------------------------*/
void Step::vPathIdentifierChoosePaths(void)
{
  //! refer:  https://en.cppreference.com/w/c/language/function_definition
  
  ENTRY();
  
  PathIdentifier* pPathIdentifier = _pPathIdentifier(*m_pLogicUnit->m_xpPathIdentifier);
  
  for (i8 i_Phrase = 0; i_Phrase < pPathIdentifier->nPhrases(); i_Phrase++)
    pPathIdentifier->vChoosePath(i_Phrase);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PathIdentifier[].chosenPath -- push chosen access path for given phrase    */
/*----------------------------------------------------------------------------*/
void Step::vPathIdentifierChosenPath(void)
{
  ENTRY();
  
  PathIdentifier* pPathIdentifier = _pPathIdentifier(*m_pLogicUnit->m_xpPathIdentifier);
  
  i8 i_Phrase = m_pDataStack->iPopInt(__func__);
  AccessPath* pChosenPlan = pPathIdentifier->pChosenPath(i_Phrase);
  
  m_pDataStack->vPushPtr(pChosenPlan);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PathIdentifier[].delete -- delete the plan                                 */
/*----------------------------------------------------------------------------*/
void Step::vPathIdentifierDelete(void)
{
  ENTRY();
  
  PathIdentifier* pPathIdentifier = _pPathIdentifier(*m_pLogicUnit->m_xpPathIdentifier);
  
  delete pPathIdentifier;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PathIdentifier[].fetchTechnique@ -- fetch the fetch technique for given phrase( -- )*/
/*----------------------------------------------------------------------------*/
void Step::vPathIdentifierFetchTechniqueFetch(void)
{
  ENTRY();
  
  PathIdentifier* pPathIdentifier = _pPathIdentifier(*m_pLogicUnit->m_xpPathIdentifier);
  
  i8 i_Phrase = m_pDataStack->iPopInt(__func__);
  eAT eFetchTechnique = pPathIdentifier->pChosenPath(i_Phrase)->eAccess();
  
  m_pDataStack->vPushInt(eFetchTechnique);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PathIdentifier[].genPaths -- generate all access paths for table           */
/*----------------------------------------------------------------------------*/
void Step::vPathIdentifierGenPaths(void)
{
  ENTRY();
  
  PathIdentifier* pPathIdentifier = _pPathIdentifier(*m_pLogicUnit->m_xpPathIdentifier);
  
  pPathIdentifier->vGenPaths();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PathIdentifier[] -- get path idenitifier                                   */
/*----------------------------------------------------------------------------*/
void Step::vPathIdentifierGet(void)
{
  ENTRY();
  
  PathIdentifier* pPathIdentifier = _pPathIdentifier(*m_pLogicUnit->m_xpPathIdentifier);
  
  m_pDataStack->vPushPtr(pPathIdentifier);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PathIdentifier[].highKey@ -- fetch high key for given phrase ( -- )        */
/*----------------------------------------------------------------------------*/
void Step::vPathIdentifierHighKeyFetch(void)
{
  ENTRY();
  
  PathIdentifier* pPathIdentifier = _pPathIdentifier(*m_pLogicUnit->m_xpPathIdentifier);
  
  i8 i_Phrase = m_pDataStack->iPopInt(__func__);
  AccessPath* mpChosenPath = pPathIdentifier->pChosenPath(i_Phrase);
  
  PredicateVec* mpUpperPredicates = mpChosenPath->pUpperPredicates();
  Predicate* mpUpperPredicate = mpUpperPredicates->pGet(0);
  s256 sHighKey = mpUpperPredicate->pArg2()->sArgVal();
  
  m_pDataStack->vPushStr(sHighKey);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PathIdentifier[].indexNm@ -- fetch index name for given phrase ( -- )      */
/*----------------------------------------------------------------------------*/
void Step::vPathIdentifierIndexNmFetch(void)
{
  ENTRY();
  
  PathIdentifier* pPathIdentifier = _pPathIdentifier(*m_pLogicUnit->m_xpPathIdentifier);
  
  i8 i_Phrase = m_pDataStack->iPopInt(__func__);
  AccessPath* mpChosenPath = pPathIdentifier->pChosenPath(i_Phrase);
  
  Index* pChosenIndex = mpChosenPath->pIndex();
  
  s32 sIndexNm = pChosenIndex->szIndexNm();
  m_pDataStack->vPushStr(sIndexNm);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PathIdentifier[].isNull -- is pathIdentifier null ?                        */
/*----------------------------------------------------------------------------*/
void Step::vPathIdentifierIsNull(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(m_pLogicUnit->m_xpPathIdentifier.bIsNull() ? 1 : 0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PathIdentifier[].key@ -- fetch key for given phrase ( -- )                 */
/*----------------------------------------------------------------------------*/
void Step::vPathIdentifierKeyFetch(void)
{
  ENTRY();
  
  PathIdentifier* pPathIdentifier = _pPathIdentifier(*m_pLogicUnit->m_xpPathIdentifier);
  
  i8 i_Phrase = m_pDataStack->iPopInt(__func__);
  AccessPath* mpChosenPath = pPathIdentifier->pChosenPath(i_Phrase);
  
  PredicateVec* mpLowerPredicates = mpChosenPath->pLowerPredicates();
  Predicate* mpLowerPredicate = mpLowerPredicates->pGet(0);
  s256 sEqlKey = mpLowerPredicate->pArg2()->sArgVal();
  
  m_pDataStack->vPushStr(sEqlKey);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PathIdentifier[].lowKey@ -- fetch low key for given phrase ( -- )          */
/*----------------------------------------------------------------------------*/
void Step::vPathIdentifierLowKeyFetch(void)
{
  ENTRY();
  
  PathIdentifier* pPathIdentifier = _pPathIdentifier(*m_pLogicUnit->m_xpPathIdentifier);
  
  i8 i_Phrase = m_pDataStack->iPopInt(__func__);
  AccessPath* mpChosenPath = pPathIdentifier->pChosenPath(i_Phrase);
  
  PredicateVec* mpLowerPredicates = mpChosenPath->pLowerPredicates();
  Predicate* mpLowerPredicate = mpLowerPredicates->pGet(0);
  s256 sLowKey = mpLowerPredicate->pArg2()->sArgVal();
  
  m_pDataStack->vPushStr(sLowKey);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PathIdentifier[].matchPaths -- attempt to match predicates with paths      */
/*----------------------------------------------------------------------------*/
void Step::vPathIdentifierMatchPaths(void)
{
  ENTRY();
  
  PathIdentifier* pPathIdentifier = _pPathIdentifier(*m_pLogicUnit->m_xpPathIdentifier);
  
  pPathIdentifier->vMatchPaths();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PathIdentifier[].new -- new a pathIdentifier                               */
/*----------------------------------------------------------------------------*/
void Step::vPathIdentifierNew(void)
{
  ENTRY();
  
  m_pLogicUnit->m_xpPathIdentifier = new PathIdentifier;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PathIdentifier[].parseWhere -- parse a where clause                        */
/*----------------------------------------------------------------------------*/
void Step::vPathIdentifierParseWhere(void)
{
  ENTRY();
  
  PathIdentifier* pPathIdentifier = _pPathIdentifier(*m_pLogicUnit->m_xpPathIdentifier);
  
  s48K sWhere;
  m_pDataStack->vPopStr(sWhere,__func__);
  
  pPathIdentifier->vParseWhere(sWhere);
  pPathIdentifier->vParsePhrases();
  
  //  i8 nLastBuffers;
  //
  //  i8 nPhrases = pPathIdentifier->nPhrases();
  //  if (nPhrases)
  //    nLastBuffers = pPathIdentifier->nPhrases() * gxCmdLineArgs.m_xSysArgs.m_nPtnsPerNode;
  //  else
  //    nLastBuffers = gxCmdLineArgs.m_xSysArgs.m_nPtnsPerNode;
  //
  //  m_pTask->m_pSession->vLastBuffers(nLastBuffers);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PathIdentifier[].phrases@ -- fetch number of phrases ( -- nPhrases )       */
/*----------------------------------------------------------------------------*/
void Step::vPathIdentifierPhrasesFetch(void)
{
  ENTRY();
  
  PathIdentifier* pPathIdentifier = _pPathIdentifier(*m_pLogicUnit->m_xpPathIdentifier);
  
  i8 nPhrases = pPathIdentifier->nPhrases();
  
  m_pDataStack->vPushInt(nPhrases);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PathIdentifier[].scorePaths -- score the generated set of paths            */
/*----------------------------------------------------------------------------*/
void Step::vPathIdentifierScorePaths(void)
{
  ENTRY();
  
  PathIdentifier* pPathIdentifier = _pPathIdentifier(*m_pLogicUnit->m_xpPathIdentifier);
  
  pPathIdentifier->vScorePaths();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PathIdentifier[].set -- set pathIdentifier                                 */
/*----------------------------------------------------------------------------*/
void Step::vPathIdentifierSet(void)
{
  ENTRY();

  PathIdentifier* pPathIdentifier = _pPathIdentifier(m_pDataStack->pPopPtr(__func__));
  ASSERT(! pPathIdentifier || m_p_TaskLogicUnit->m_xpPathIdentifier.bIsNull());
  m_pLogicUnit->m_xpPathIdentifier = _pPathIdentifier(pPathIdentifier);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PathIdentifier[].showPaths -- show generated paths                         */
/*----------------------------------------------------------------------------*/
void Step::vPathIdentifierShowPaths(void)
{
  ENTRY();
  
  PathIdentifier* pPathIdentifier = _pPathIdentifier(*m_pLogicUnit->m_xpPathIdentifier);
  
  pPathIdentifier->vShowPaths();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PathIdentifier[].table@ -- fetch table from access path                    */
/*----------------------------------------------------------------------------*/
void Step::vPathIdentifierTableFetch(void)
{
  ENTRY();
  
  PathIdentifier* pPathIdentifier = _pPathIdentifier(*m_pLogicUnit->m_xpPathIdentifier);
  
  Table* pTable = pPathIdentifier->pTable();
  
  m_pDataStack->vPushPtr(pTable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PathIdentifier[].table! -- store table into access path                    */
/*----------------------------------------------------------------------------*/
void Step::vPathIdentifierTableStore(void)
{
  ENTRY();
  
  PathIdentifier* pPathIdentifier = _pPathIdentifier(*m_pLogicUnit->m_xpPathIdentifier);
  
  Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));
  
  pPathIdentifier->vTable(pTable);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
