/*============================================================================*/
/* Bench.cpp -- Bench                                                         */
/*============================================================================*/

#include "Bench.hpp"
#include "Board.hpp"
#include "Optimiser.hpp"
#include "CardIter.hpp"
#include "Card_AbsJoin.hpp"

#include "Error.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Bench,CN_BENCH)
__aNewFunction(Bench,CN_BENCH)
__vDeleteFunction(Bench,CN_BENCH)
__aFooterFunction(Bench)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Bench::Bench(Optimiser* pOptimiser)
: Object(CN_BENCH)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);
    
  m_pOptimiser = pOptimiser;

  m_cPrefix = ' ';

  m_bIsFlat = false;

  m_iOdnLo      = 1;
  m_iOdnHi      = C_UNKNOWN;
  m_cTableSyn   = 'a';

  m_pCI = new CardIter(this);
  
  _checkInitialised_(CN_BENCH);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Bench::~Bench(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);
  delete m_pCI;
  vDeleteAll();
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*-- series ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* transform matrix                                                           */
/*----------------------------------------------------------------------------*/
bool Bench::bApplyTransformationMatrix(eEC* peErrorCode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  bool bClean = true;

  //m_pCardCtl->vReset();
  //m_pCardCtl->vPrintSubCards(false,CK_AGGELIST,CK_KEY,CK_PROJECTION,0);
  //m_pCardCtl->vPrintSubCards(CK_AGGELIST, true);
  //m_pCardCtl->vPrintSubCards(CK_KEY, true);

  try {
    m_pOptimiser->pCardCtl()->vPrintSubCards(false, //CK_AGGELIST,
        //CK_KEY, //CK_PROJECTION,
        //CK_RPNCALC,
                                             CK_STREAM, 0);
    m_pOptimiser->pCardCtl()->vSaveShorter();
    m_pOptimiser->pCardCtl()->vSaveXhand();

    //vApplyTransformationMatrix_originalModel();
    vApplyTransformationMatrix_recursiveModel();

    bClean = true;
  }

  catch (Exception xException) {
    // set peErrorCode
    bClean = false;
  }

  catch (Error xError) {
    *peErrorCode = xError.eErrorCode();
    // todo: this should read xError.szErrorMsg()
    //Stz* szError = xError.aErrorMsg();

    bClean = false;
  }

  return bClean;
}

/*----------------------------------------------------------------------------*/
/* apply the original transformation matrix                                   */
/*----------------------------------------------------------------------------*/
void Bench::vApplyTransformationMatrix_originalModel(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  m_pCI->m_bRecursiveModel = false;

  vSeries_PreparePipelines_originalModel();
  vSeries_MakeRunnable_originalModel();
  vSeries_Optimisations();
  vSeries_MakeWeavable();
}

/*----------------------------------------------------------------------------*/
/* apply the recursive transformation matrix                                  */
/*----------------------------------------------------------------------------*/
void Bench::vApplyTransformationMatrix_recursiveModel(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  m_pCI->m_bRecursiveModel = true;

  vSeries_PreparePipelines_recursiveModel();
  vSeries_MakeRunnable_recursiveModel();

  vSeries_Optimisations();
  vSeries_MakeWeavable();
}


/*----------------------------------------------------------------------------*/
/* apply the reducePipeline set of transformations to all subPipe boards      */
/*----------------------------------------------------------------------------*/
void Bench::vSeries_PreparePipelines_originalModel(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  vSeries(BK_SUBPIPE, 0, __func__);

  vS10_T00_Begin();

  m_pCI->vBoardIteration();
  while (m_pCI->bEachBoard()) {
    m_pCI->vUsingBoard(BK_SUBPIPE, m_pCI->G.i_Board);

    vGroup_InitialAdjustments();
    vGroup_CondenseInfo_originalModel();
    vGroup_MetaAnalysis();
    vGroup_MainConversions();
  }

  vEndSeries(BK_SUBPIPE);
}

/*----------------------------------------------------------------------------*/
/* apply the reducePipeline set of transformations to all subPipe boards      */
/*----------------------------------------------------------------------------*/
void Bench::vSeries_PreparePipelines_recursiveModel(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  vSeries(BK_SUBPIPE, 0, __func__);

  vS10_T00_Begin();

  m_pCI->vBoardIteration();
  while (m_pCI->bEachBoard()) {
    m_pCI->vUsingBoard(BK_SUBPIPE, m_pCI->G.i_Board);

    vGroup_InitialAdjustments();
    vGroup_CondenseInfo_recursiveModel();
    vGroup_MetaAnalysis();
    vGroup_PreliminaryConversions_recursiveModel();
  }

  vTransform_OuterQueryAnalysis();

  vEndSeries(BK_SUBPIPE);
}

/*----------------------------------------------------------------------------*/
/* apply the reducePipeline set of transformations to all subPipe boards      */
/*----------------------------------------------------------------------------*/
void Bench::vSeries_MakeRunnable_originalModel(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  vSeries(BK_SUBPIPE, 0, __func__);

  m_pCI->vBoardIteration();
  while (m_pCI->bEachBoard()) {
    m_pCI->vUsingBoard(BK_SUBPIPE, m_pCI->G.i_Board);

    vGroup_PredicatePullup_originalModel();
    vGroup_IndexBounds_originalModel();
    vGroup_JoinOrdering_originalModel();
  }

  m_pCI->vUsingBoard(BK_SUBPIPE, 0);

  vGroup_FlattenSubquery_originalModel();

  vEndSeries(BK_SUBPIPE);
}

/*----------------------------------------------------------------------------*/
/* apply the reducePipeline set of transformations to all subPipe boards      */
/*----------------------------------------------------------------------------*/
void Bench::vSeries_MakeRunnable_recursiveModel(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  vSeries(BK_SUBPIPE, 0, __func__);

  m_pCI->G.pSubQuery = m_pCI->G.pBoard->pSubQuery();

  vSeries_MakeRunnable_recurse();

  vEndSeries(BK_SUBPIPE);
}

/*----------------------------------------------------------------------------*/
/* apply the xyz set of transformations to all subPipe boards                 */
/*----------------------------------------------------------------------------*/
void Bench::vSeries_MakeRunnable_recurse(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  // things before sub-queries

  SubQuery* pOuterQuery = m_pCI->G.pSubQuery;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_pCI->vSubQueryIteration();
  while (m_pCI->bEachSubQuery()) {

//    vTrace("Orientation: OuterQuery %lld; SubQuery %lld, Caller %lld\n"_Stz,
//           pOuterQuery->pBoard()->i_Board(),
//           m_pCI->G.pSubQuery->pBoard()->i_Board(),
//           m_pCI->G.pSubQuery->i_Caller()
//           );

    if (m_pCI->G.pSubQuery->i_Caller() != pOuterQuery->pBoard()->i_Board())
      continue;

    vTrace("Orientation: OuterQuery %lld; SubQuery %lld, Caller %lld\n"_Stz,
           pOuterQuery->pBoard()->i_Board(),
           m_pCI->G.pSubQuery->pBoard()->i_Board(),
           m_pCI->G.pSubQuery->i_Caller()
    );

    m_pCI->G_vPushBoard(m_pCI->G.pSubQuery->pBoard());
    vSeries_MakeRunnable_recurse();
    m_pCI->G_vPopIter();

    vGroup_FlattenSubquery_recursiveModel();
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // things after sub-queries
  vGroup_FinalConversions_recursiveModel();

  vGroup_PredicatePullup_recursiveModel();
  vGroup_IndexBounds_recursiveModel();
  vGroup_JoinOrdering_recursiveModel();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vSeries_Optimisations(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  vSeries(BK_SUBPIPE, 0, __func__);

  vS10_T00_Begin();

  i8 i_Board = 0;

  m_pCI->vUsingBoard(BK_SUBPIPE, i_Board);

  vGroup_ChooseEEops();

  vEndSeries(BK_SUBPIPE);
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vSeries_MakeWeavable(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  vSeries(BK_SUBPIPE, 0, __func__);

  vSA0_T00_Begin();

  vGroup_CullBounds();
  vGroup_Filters();
  vGroup_Staging();
  vGroup_Partitioning();
  vGroup_FinalAdjustments();

  vEndSeries(BK_SUBPIPE, 0);
}

/*----------------------------------------------------------------------------*/
/*-- group -------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vGroup_InitialAdjustments(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  vTransform_MoveOn();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vGroup_CondenseInfo_originalModel(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  vTransform_DeleteUnneeded();
  vTransform_ScanBecomesTable();
  vTransform_AddLevel();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vGroup_CondenseInfo_recursiveModel(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  vTransform_DeleteUnneeded();
  vTransform_ScanBecomesTable();
  vTransform_AddLevel();
  vTransform_NotOps();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vGroup_MetaAnalysis(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);
/*
  vTransform_TableAnalysis();
  vTransform_StreamAnalysis();
  vTransform_IndexAnalysis();
  vTransform_ProjectionAnalysis();
*/

  vTransform_TableAnalysis();
  vTransform_ProjectionAnalysis();
  vTransform_IndexAnalysis();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vGroup_MainConversions(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  vTransform_StmColToTblCol_originalModel();
  vTransform_RpnCalcToBoolCalc();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vGroup_PreliminaryConversions_recursiveModel(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  //vTransform_StmColToTblCol_originalModel();
  vTransform_RpnCalcToBoolCalc();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vGroup_FinalConversions_recursiveModel(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  vTransform_StmColToTblCol_recursiveModel();
  //vTransform_RpnCalcToBoolCalc();
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vGroup_PredicatePullup_originalModel(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);


  vTransform_MarkCmpPredWrtTable();
  vTransform_CopyMarkedCmpPred();
  vTransform_CullMarkedCmpPred();
  vTransform_DeleteEmptyWhere();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vGroup_PredicatePullup_recursiveModel(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  vTransform_MarkCmpPredWrtTable();
  vTransform_CopyMarkedCmpPred();
  vTransform_CullMarkedCmpPred();
  vTransform_DeleteEmptyWhere();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vGroup_IndexBounds_originalModel(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  vTransform_DeriveBoundsFromFilter_originalModel();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vGroup_IndexBounds_recursiveModel(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  vTransform_DeriveBoundsFromFilter_originalModel();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vGroup_JoinOrdering_originalModel(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  if (Card_Table::nTables(m_pCI) < 2)
    return;

  vTransform_JoinOrdering_originalModel();
  vTransform_PredicateReposition_originalModel();
  vTransform_DeriveBoundsFromFilter_originalModel();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vGroup_JoinOrdering_recursiveModel(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  if (Card_Table::nTables_reverse_foldBreak(m_pCI) < 2)
    return;

  vTransform_JoinOrdering_originalModel();
  vTransform_PredicateReposition_originalModel();
  vTransform_DeriveBoundsFromFilter_originalModel();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vGroup_FlattenSubquery_originalModel(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  vTransform_SubQueryAnalysisMainQuery();
  vTransform_SubQueryAnalysisSubQuery();

  vTransform_FlattenByDecisionTree_originalModel();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vGroup_FlattenSubquery_recursiveModel(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  vTransform_FlattenByDecisionTree_recursiveModel();
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vGroup_ChooseEEops(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  vTransform_MorphTableToEEop();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vGroup_CullBounds(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  vTransform_CullNonusedBounds();
  vTransform_CullUsedBoundsFromFilter();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vGroup_Filters(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  vTransform_IndividuateFilter();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vGroup_Staging(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  vTransform_ReinstateStages();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vGroup_Partitioning(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  vTransform_AddPartitioning();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vGroup_FinalAdjustments(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  vTransform_TblColToStmCol();
  vTransform_StmColToColNmOfKeyExists();
}


/*----------------------------------------------------------------------------*/
/*-- transform ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* move all <absJoin>/<on> to <where>                                         */
/*----------------------------------------------------------------------------*/
void Bench::vTransform_MoveOn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  m_pCI->vTransform(__func__, "move <on> in <absJoin>'s to <where>"_Stz);

  vAssert_Board_any();

  Card_Where::vMove_All_on_To_where(m_pCI);

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* Condense Info                                                              */
/*----------------------------------------------------------------------------*/
void Bench::vTransform_DeleteUnneeded(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  m_pCI->vTransform(__func__, "delete superfluous <absJoin>'s"_Stz);

  vAssert_Board_any();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Delete All AbsJoin
  Card_AbsJoin::vDeleteAllAbsJoin(m_pCI);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Delete All Combined
  m_pCI->G_vRestart();
  Card_Combined::vDeleteAllCombined(m_pCI);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Delete All Send
  m_pCI->G_vRestart();
  Card_Send::vDeleteAllSend(m_pCI);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Delete All Stage
  m_pCI->G_vRestart();
  Card_Stage::vDeleteAllStage(m_pCI);

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* Condense Info                                                              */
/*----------------------------------------------------------------------------*/
void Bench::vTransform_ScanBecomesTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  m_pCI->vTransform(__func__, "delete superfluous <absJoin>'s"_Stz);

  vAssert_Board_any();

  m_pCI->G_vRestart();
  while (m_pCI->G_bEachMain(CK_SCAN))
    Card_Scan::vScanBecomesTable(m_pCI);

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* Add Levels                                                                 */
/*----------------------------------------------------------------------------*/
void Bench::vTransform_AddLevel(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  m_pCI->vTransform(__func__, "delete superfluous <absJoin>'s"_Stz);

  vAssert_Board_any();

  m_pCI->G_vRestart();
  while (m_pCI->G_bEachMain())
    ((Card_OP*)m_pCI->G.p_MainCard)->vAddLevel();

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* Not Ops                                                                    */
/*----------------------------------------------------------------------------*/
void Bench::vTransform_NotOps(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  m_pCI->G_vRestart();
  while (m_pCI->G_bEachSubOfMain(CK_RPNCALC))
    m_pCI->G.p_rpnCalc->vConvert_sqyOp_not_To_notSqyOp();
}

/*----------------------------------------------------------------------------*/
/* Table Analysis                                                             */
/*----------------------------------------------------------------------------*/
void Bench::vTransform_TableAnalysis(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  m_pCI->vTransform(__func__, "add a <tableSyn> to all <table>'s"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Affix Table Syntax To All Table
  Card_Table::vAffixTableSyn(m_pCI, &m_cTableSyn);

//  while (m_pCI->G_bEachMainWithSub(CK_TABLE, CK_TABLENM))  {
//    m_pCI->G.p_table->vSet_Syn(&m_cTableSyn);
//  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Produce Table Partition Under All Table
  m_pCI->G_vRestart();
  Card_Table::vProduceTablePtns(m_pCI);

//  while (m_pCI->G_bEachMainWithSub(CK_TABLE, CK_TABLENM)) {
//
//    m_pCI->G.p_tablePtn = Card_TablePtn::pDerive(m_pCI->G.p_table);
//    m_pCI->G.p_table->vAppend(m_pCI->G.p_tablePtn);
//
//    Card_TableNm* p_tableNm = m_pCI->G.p_table->pSub_tableNm();
//    Table* pTable = AM::pTableFind(p_tableNm->sz());
//
//    m_pCI->G.p_size = Card_Size::pDerive(pTable);
//    m_pCI->G.p_table->vAppend(m_pCI->G.p_size);
//  }

  m_pCI->vEndTransform();
}
/*----------------------------------------------------------------------------*/
//void Bench::vS20b_T_Table_Analysis(void)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);
//
//  m_pCI->vTransform(__func__, "add a <tableSyn> to all <table>'s"_Stz);
//
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  // Affix Table Syntax To All Table
//
//  while (m_pCI->G_bEachMainWithSub(CK_TABLE, CK_TABLENM))  {
//    m_pCI->G.p_table->vSet_Syn(&m_cTableSyn);
//  }
//
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  // Produce Table Partition Under All Table
//
//  m_pCI->G_vRestart();
//
//  while (m_pCI->G_bEachMainWithSub(CK_TABLE, CK_TABLENM))
//    m_pCI->G.p_table->vProjectionAnalysis(this);
//
//
//  m_pCI->vEndTransform();
//}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vTransform_IndexAnalysis(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  m_pCI->vTransform(__func__, "move @c predicates to a filter under table"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Append Index List Under All Table
  Card_Table::vAppendIndexList(m_pCI);

//  while (m_pCI->G_bEachMain(CK_TABLE)) {
//    m_pCI->G.p_tableNm = m_pCI->G.p_table->pSub_tableNm();
//    Table* pTable = Card::pAccess_Table(m_pCI->G.p_tableNm->sz());
//
//    m_pCI->G.p_indexList = Card_IndexList::pDerive(pTable);
//
//    m_pCI->G_vAppendChild(CK_TABLE,CK_INDEXLIST);
//  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Affix TblColOdn To All IdxCol

  m_pCI->G_vRestart();
  Card_Table::vAffixTableColOrd(m_pCI);

//  while (m_pCI->G_bEachMainWith2Sub(CK_TABLE,CK_STREAM,CK_INDEXLIST)) {
//    CardIter xCI(m_pCI->G.p_indexList);
//    while ((m_pCI->G.p_index = _pCard_Index(xCI.pEachChild()))) {
//      m_pCI->G.p_idxColList = m_pCI->G.p_index->pSub_idxColList();
//      m_pCI->G.p_idxColList->vAffix_tblColOdn(m_pCI->G.p_stream);
//    }
//  }

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* Stream Analysis                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vTransform_StreamAnalysis(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  m_pCI->vTransform(__func__, "add a <stream> under every <table>"_Stz);

  vAssert_Board_any();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Produce Stream Under All Table

  vStreamAnalysisReset();

  while (m_pCI->G_bEachMainWithSub(CK_TABLE, CK_TABLENM)) {
    m_pCI->m_pTable = Card_Table::pAccess_Table(m_pCI->G.p_tableNm->sz());
    m_pCI->G.p_stream = Card_Stream::pDerive_Of_table(m_pCI->m_pTable);

    m_pCI->G.p_table->vAppend(m_pCI->G.p_stream);
  }

//  while (m_pCI->G_bEachMainWithSub(CK_TABLE, CK_TABLENM))
//    m_pCI->G.p_table->vProjectionAnalysis(this);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Produce Stream Under All Project

  m_pCI->G_vRestart();
  vStreamAnalysisReset();

  while (m_pCI->G_bEachMainWithSub(CK_PROJECT,CK_PROJECTION)) {
    m_pCI->G.p_stream = Card_Stream::pDerive_Of_projection(m_pCI->G.p_projection);

    m_pCI->G.p_project->vAppend(m_pCI->G.p_stream);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Produce Stream Under All Agg Reg

  m_pCI->G_vRestart();
  vStreamAnalysisReset();

  while (m_pCI->G_bEachMainWithSub(CK_AGGREG,CK_PROJECTION)) {
    m_pCI->G.p_stream = Card_Stream::pDerive_Of_projection(m_pCI->G.p_projection);

    m_pCI->G.p_aggreg->vAppend(m_pCI->G.p_stream);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Produce Stream Under All GbyAgg

  m_pCI->G_vRestart();
  vStreamAnalysisReset();

  while (m_pCI->G_bEachMainWithSub(CK_GBYAGG,CK_PROJECTION)) {
    m_pCI->G.p_stream = Card_Stream::pDerive_Of_projection(m_pCI->G.p_projection);

    m_pCI->G.p_gbyAgg->vAppend(m_pCI->G.p_stream);
  }


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Produce Stream Under All Sort

  m_pCI->G_vRestart();
  vStreamAnalysisReset();

  while (m_pCI->G_bEachMain(CK_SORT)) {

    if (m_pCI->G_bResolveSubCard(CK_STREAM,m_pCI->G.i_MainCard)) {
      m_pCI->G.p_stream = _pCard_Stream(m_pCI->G.p_stream->pDupCardAndSubCards());
      m_pCI->G.p_sort->vAppend(m_pCI->G.p_stream);
    }
  }




  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Derive Ordinal Range For All Table Stream

  m_pCI->G_vRestart();
  while (m_pCI->G_bEachMainWithSub(CK_TABLE,CK_STREAM)) {
    m_pCI->G.p_stream->vDerive_OrdinalRange_table(m_pCI->G.p_table->cValue(), &m_iOdnHi, &m_iOdnLo);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Derive Ordinal Range For All Project Stream

  m_pCI->G_vRestart();
  while (m_pCI->G_bEachMainWithSub(CK_PROJECT,CK_STREAM)) {
    m_pCI->G.p_stream->vDerive_OrdinalRange_project();
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Derive Ordinal Range For All Agg Reg Stream

  m_pCI->G_vRestart();
  while (m_pCI->G_bEachMainWithSub(CK_AGGREG,CK_STREAM)) {
    m_pCI->G.p_stream->vDerive_OrdinalRange_aggreg();
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Derive Ordinal Range For All GbyAgg Stream

  m_pCI->G_vRestart();
  while (m_pCI->G_bEachMainWithSub(CK_GBYAGG,CK_STREAM)) {
    m_pCI->G.p_stream->vDerive_OrdinalRange_aggreg();
  }


  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* Projection Analysis                                                        */
/*----------------------------------------------------------------------------*/
void Bench::vTransform_ProjectionAnalysis(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  m_pCI->vTransform(__func__, "((desc))"_Stz);

  vAssert_Board_any();

//  while (m_pCI->G_bEachMain(CK_TABLE))
//    m_pCI->G.p_table->vProjectionAnalysis(m_pCI);

  Card_Projection::vProjection_Analysis_v2(m_pCI);

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* convert all <stmColOdn> to <tblColOdn>                                     */
/*----------------------------------------------------------------------------*/
void Bench::vTransform_StmColToTblCol_originalModel(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  m_pCI->vTransform(__func__, "all $n become $c.n"_Stz);

  vAssert_Board_any();

  while (m_pCI->G_bEachMain())
    ((Card_OP *) m_pCI->G.p_MainCard)->vConvert_stmColOdn_To_tblColOdn_original(m_pCI);

  m_pCI->G.pDeck->vNoTermsStmColOdn();

  m_pCI->G.pBoard->vMayHaveStmColOdn(false);

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* convert all <stmColOdn> to <tblColOdn>                                     */
/*----------------------------------------------------------------------------*/
void Bench::vTransform_StmColToTblCol_recursiveModel(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  m_pCI->vTransform(__func__, "all $n become $c.n"_Stz);

  vAssert_Board_any();

  while (m_pCI->G_bEachMain())
    ((Card_OP *) m_pCI->G.p_MainCard)->vConvert_stmColOdn_To_tblColOdn_recursive(m_pCI);

  m_pCI->G.pDeck->vNoTermsStmColOdn();

  m_pCI->G.pBoard->vMayHaveStmColOdn(false);

  m_pCI->vEndTransform();
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vTransform_RpnCalcToBoolCalc(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  m_pCI->vTransform(__func__, "all $n become $c.n"_Stz);

  vAssert_Board_any();

  m_pCI->G_vRestart();
  while (m_pCI->G_bEachMainWithSub(CK_WHERE, CK_RPNCALC))
    ((Card_OP*)m_pCI->G.p_MainCard)->vConvert_rpnCalc_To_boolCalc(m_pCI);

  m_pCI->G_vRestart();
  while (m_pCI->G_bEachMainWithSub(CK_HAVING, CK_RPNCALC))
    ((Card_OP*)m_pCI->G.p_MainCard)->vConvert_rpnCalc_To_boolCalc(m_pCI);

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vTransform_MarkCmpPredWrtTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  m_pCI->vTransform(__func__, "put a table @c against where pred"_Stz);

  vAssert_Board_any();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // At Mark Where Pred Wrt Table

  m_pCI->vPrintBoard(__LINE__);
  Card_Where::vMarkWherePredWrtTable(m_pCI);

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vTransform_CopyMarkedCmpPred(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  m_pCI->vTransform(__func__, "put a table @c against where pred"_Stz);

  vAssert_Board_any();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Move At Marked Where Pred To Table Filter

  m_pCI->G_vRestart();
  Card_Where::vMoveWherePredToTableFilter(m_pCI);

  m_pCI->vPrintBoard();

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vTransform_CullMarkedCmpPred(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  m_pCI->vTransform(__func__, "put a table @c against where pred"_Stz);

  vAssert_Board_any();

  m_pCI->G_vRestart();
  Card_Where::vCullMarkedCmpPred(m_pCI);

  m_pCI->vPrintBoard();

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vTransform_DeleteEmptyWhere(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  m_pCI->vTransform(__func__, "put a table @c against where pred"_Stz);

  vAssert_Board_any();

  m_pCI->G_vRestart();
  Card_Where::vDeleteEmpyWhere(m_pCI);

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* copy <table>/<filter> up into <table>/<bounds>                             */
/*----------------------------------------------------------------------------*/
void Bench::vTransform_DeriveBoundsFromFilter_originalModel(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  m_pCI->vTransform(__func__, "Add <bounds> under each filtered <table>"_Stz);

  vAssert_Board_any();
  Card_Table::vDeriveBoundsFromFilter(m_pCI);

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vTransform_FlattenByDecisionTree_originalModel(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  if (1 ==  nBoards())
    return;

  m_pCI->vTransform(__func__, "((10))"_Stz);

  vAssert_Board_mqA();

  m_pCI->G.pSubQuery =  pGetBoard(C_SQ_BOARD)->pSubQuery();

  if (m_pCI->G.pSubQuery->bViaFrom())                /* Fsq_      Q7,8,9 */ vFold_10_F();
  else
  if (m_pCI->G.pSubQuery->bViaWhere()) {
    if (m_pCI->G.pSubQuery->bIsAggregation()) {
      if (m_pCI->G.pSubQuery->bIsCorrelated()) {
             if (m_pCI->G.pSubQuery->bByExists())    /* W_A_C_E          */ NEVER_REACH
        else if (m_pCI->G.pSubQuery->bByNotExists()) /* W_A_C_NE         */ NEVER_REACH
        else if (m_pCI->G.pSubQuery->bByIn())        /* W_A_C_I          */ NEVER_REACH
        else if (m_pCI->G.pSubQuery->bByNotIn())     /* W_A_C_NI         */ NEVER_REACH
        else                                         /* W_A_C_ro   Q2,17 */ vFold_20_W_A_C_ro();
      } else {
             if (m_pCI->G.pSubQuery->bByExists())    /* W_A_NC_E         */ NEVER_REACH
        else if (m_pCI->G.pSubQuery->bByNotExists()) /* W_A_NC_NE        */ NEVER_REACH
        else if (m_pCI->G.pSubQuery->bByIn())        /* W_A_NC_I     Q18 */ vFold_30_W_A_NC_I();
        else if (m_pCI->G.pSubQuery->bByNotIn())     /* W_A_NC_NI        */ NEVER_REACH
        else                                         /* W_A_NC_ro    Q15 */ vFold_40_W_A_NC_ro();
      }
    } else {
      if (m_pCI->G.pSubQuery->bIsCorrelated()) {
             if (m_pCI->G.pSubQuery->bByExists())    /* W_L_C_E       Q4 */ vFold_50_W_L_C_E();
        else if (m_pCI->G.pSubQuery->bByNotExists()) /* W_L_C_NE         */ NEVER_REACH
        else if (m_pCI->G.pSubQuery->bByIn())        /* W_L_C_I          */ NEVER_REACH
        else if (m_pCI->G.pSubQuery->bByNotIn())     /* W_L_C_NI         */ NEVER_REACH
        else                                         /* W_L_C_ro         */ NEVER_REACH
      } else {
             if (m_pCI->G.pSubQuery->bByExists())    /* W_L_NC_E         */ NEVER_REACH
        else if (m_pCI->G.pSubQuery->bByNotExists()) /* W_L_NC_NE        */ NEVER_REACH
        else if (m_pCI->G.pSubQuery->bByIn())        /* W_L_NC_I         */ NEVER_REACH
        else if (m_pCI->G.pSubQuery->bByNotIn())     /* W_L_NC_NI    Q16 */ vFold_60_W_L_NC_NI();
        else                                         /* W_L_NC_ro        */ NEVER_REACH
      }
    }
  }
  else
  if (m_pCI->G.pSubQuery->bViaHaving()) {
    if (m_pCI->G.pSubQuery->bIsAggregation()) {
      if (m_pCI->G.pSubQuery->bIsCorrelated()) {
             if (m_pCI->G.pSubQuery->bByExists())    /* H_A_C_E          */ NEVER_REACH
        else if (m_pCI->G.pSubQuery->bByNotExists()) /* H_A_C_NE         */ NEVER_REACH
        else if (m_pCI->G.pSubQuery->bByIn())        /* H_A_C_I          */ NEVER_REACH
        else if (m_pCI->G.pSubQuery->bByNotIn())     /* H_A_C_NI         */ NEVER_REACH
        else                                         /* H_A_C_ro         */ NEVER_REACH
      } else {
             if (m_pCI->G.pSubQuery->bByExists())    /* H_A_NC_E         */ NEVER_REACH
        else if (m_pCI->G.pSubQuery->bByNotExists()) /* H_A_NC_NE        */ NEVER_REACH
        else if (m_pCI->G.pSubQuery->bByIn())        /* H_A_NC_I         */ NEVER_REACH
        else if (m_pCI->G.pSubQuery->bByNotIn())     /* H_A_NC_NI        */ NEVER_REACH
        else                                         /* H_A_NC_ro    Q11 */ vFold_70_H_A_NC_ro();
      }
    } else {
      if (m_pCI->G.pSubQuery->bIsCorrelated()) {
             if (m_pCI->G.pSubQuery->bByExists())    /* H_L_C_E          */ NEVER_REACH
        else if (m_pCI->G.pSubQuery->bByNotExists()) /* H_L_C_NE         */ NEVER_REACH
        else if (m_pCI->G.pSubQuery->bByIn())        /* H_L_C_I          */ NEVER_REACH
        else if (m_pCI->G.pSubQuery->bByNotIn())     /* H_L_C_NI         */ NEVER_REACH
        else                                         /* H_L_C_ro         */ NEVER_REACH
      } else {
             if (m_pCI->G.pSubQuery->bByExists())    /* H_L_NC_E         */ NEVER_REACH
        else if (m_pCI->G.pSubQuery->bByNotExists()) /* H_L_NC_NE        */ NEVER_REACH
        else if (m_pCI->G.pSubQuery->bByIn())        /* H_L_NC_I         */ NEVER_REACH
        else if (m_pCI->G.pSubQuery->bByNotIn())     /* H_L_NC_NI        */ NEVER_REACH
        else                                         /* H_L_NC_ro        */ NEVER_REACH
      }
    }
  }
  else
    NEVER_REACH;

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vTransform_FlattenByDecisionTree_recursiveModel(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  m_pCI->vTransform(__func__, "Flatten by decision tree (recursive model)"_Stz);

  if (m_pCI->G.pSubQuery->bViaFrom())                /* Fsq_      Q7,8,9 */ vFold_10_F();
  else
  if (m_pCI->G.pSubQuery->bViaWhere()) {
    if (m_pCI->G.pSubQuery->bIsAggregation()) {
      if (m_pCI->G.pSubQuery->bIsCorrelated()) {
             if (m_pCI->G.pSubQuery->bByExists())    /* W_A_C_E          */ NEVER_REACH
        else if (m_pCI->G.pSubQuery->bByNotExists()) /* W_A_C_NE         */ NEVER_REACH
        else if (m_pCI->G.pSubQuery->bByIn())        /* W_A_C_I          */ NEVER_REACH
        else if (m_pCI->G.pSubQuery->bByNotIn())     /* W_A_C_NI         */ NEVER_REACH
        else                                         /* W_A_C_ro   Q2,17 */ vFold_20_W_A_C_ro();
      } else {
             if (m_pCI->G.pSubQuery->bByExists())    /* W_A_NC_E         */ NEVER_REACH
        else if (m_pCI->G.pSubQuery->bByNotExists()) /* W_A_NC_NE        */ NEVER_REACH
        else if (m_pCI->G.pSubQuery->bByIn())        /* W_A_NC_I     Q18 */ vFold_30_W_A_NC_I();
        else if (m_pCI->G.pSubQuery->bByNotIn())     /* W_A_NC_NI        */ NEVER_REACH
        else                                         /* W_A_NC_ro    Q15 */ vFold_40_W_A_NC_ro();
      }
    } else {
      if (m_pCI->G.pSubQuery->bIsCorrelated()) {
             if (m_pCI->G.pSubQuery->bByExists())    /* W_L_C_E       Q4 */ vFold_50_W_L_C_E();
        else if (m_pCI->G.pSubQuery->bByNotExists()) /* W_L_C_NE         */ NEVER_REACH
        else if (m_pCI->G.pSubQuery->bByIn())        /* W_L_C_I          */ NEVER_REACH
        else if (m_pCI->G.pSubQuery->bByNotIn())     /* W_L_C_NI         */ NEVER_REACH
        else                                         /* W_L_C_ro         */ NEVER_REACH
      } else {
             if (m_pCI->G.pSubQuery->bByExists())    /* W_L_NC_E         */ NEVER_REACH
        else if (m_pCI->G.pSubQuery->bByNotExists()) /* W_L_NC_NE        */ NEVER_REACH
        else if (m_pCI->G.pSubQuery->bByIn())        /* W_L_NC_I         */ NEVER_REACH
        else if (m_pCI->G.pSubQuery->bByNotIn())     /* W_L_NC_NI    Q16 */ vFold_60_W_L_NC_NI();
        else                                         /* W_L_NC_ro        */ NEVER_REACH
      }
    }
  }
  else
  if (m_pCI->G.pSubQuery->bViaHaving()) {
    if (m_pCI->G.pSubQuery->bIsAggregation()) {
      if (m_pCI->G.pSubQuery->bIsCorrelated()) {
             if (m_pCI->G.pSubQuery->bByExists())    /* H_A_C_E          */ NEVER_REACH
        else if (m_pCI->G.pSubQuery->bByNotExists()) /* H_A_C_NE         */ NEVER_REACH
        else if (m_pCI->G.pSubQuery->bByIn())        /* H_A_C_I          */ NEVER_REACH
        else if (m_pCI->G.pSubQuery->bByNotIn())     /* H_A_C_NI         */ NEVER_REACH
        else                                         /* H_A_C_ro         */ NEVER_REACH
      } else {
             if (m_pCI->G.pSubQuery->bByExists())    /* H_A_NC_E         */ NEVER_REACH
        else if (m_pCI->G.pSubQuery->bByNotExists()) /* H_A_NC_NE        */ NEVER_REACH
        else if (m_pCI->G.pSubQuery->bByIn())        /* H_A_NC_I         */ NEVER_REACH
        else if (m_pCI->G.pSubQuery->bByNotIn())     /* H_A_NC_NI        */ NEVER_REACH
        else                                         /* H_A_NC_ro    Q11 */ vFold_70_H_A_NC_ro();
      }
    } else {
      if (m_pCI->G.pSubQuery->bIsCorrelated()) {
             if (m_pCI->G.pSubQuery->bByExists())    /* H_L_C_E          */ NEVER_REACH
        else if (m_pCI->G.pSubQuery->bByNotExists()) /* H_L_C_NE         */ NEVER_REACH
        else if (m_pCI->G.pSubQuery->bByIn())        /* H_L_C_I          */ NEVER_REACH
        else if (m_pCI->G.pSubQuery->bByNotIn())     /* H_L_C_NI         */ NEVER_REACH
        else                                         /* H_L_C_ro         */ NEVER_REACH
      } else {
             if (m_pCI->G.pSubQuery->bByExists())    /* H_L_NC_E         */ NEVER_REACH
        else if (m_pCI->G.pSubQuery->bByNotExists()) /* H_L_NC_NE        */ NEVER_REACH
        else if (m_pCI->G.pSubQuery->bByIn())        /* H_L_NC_I         */ NEVER_REACH
        else if (m_pCI->G.pSubQuery->bByNotIn())     /* H_L_NC_NI        */ NEVER_REACH
        else                                         /* H_L_NC_ro        */ NEVER_REACH
      }
    }
  }
  else
    NEVER_REACH;

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* Analyse all sub-queries                                                    */
/*----------------------------------------------------------------------------*/
void Bench::vTransform_OuterQueryAnalysis(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  //m_pCI->vTransform(__func__, "Analyse all sub-queries"_Stz);

  m_pCI->vBoardIteration();
  while (m_pCI->bEachBoard()) {

    SubQuery* pOuterQuery = m_pCI->G.pBoard->pSubQuery();
    pOuterQuery->vDisposition(QD_LINEAR);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    m_pCI->vRestart();
    while (m_pCI->G_bEachMain()) {
      if (m_pCI->G.p_MainCard->bIsCard(CK_AGGREG) ||
          m_pCI->G.p_MainCard->bIsCard(CK_GBYAGG)) {
        pOuterQuery->vDisposition(QD_AGGREGATION);
        break;
      }
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // search for from calls to sub-queries

    m_pCI->vRestart();
    while (m_pCI->G_bEachMain(CK_CALL)) {

//      Stz* szBoardNm = m_pCI->G.p_call->szValue();
//      Board* pBoard = pGetBoard(szBoardNm);
//      SubQuery *pSubQuery = pBoard->pSubQuery();
//      pSubQuery->vCaller(m_pCI->G.i_Board);
//      pSubQuery->vClause(QC_FROM_CLAUSE);

      Board::vOuter_Query_Analysis_Of_from_call(m_pCI);
      pOuterQuery->vSubQueries(pOuterQuery->nSubQueries() + 1);
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // search for where calls to sub-queries

    m_pCI->vRestart();
    while (m_pCI->G_bEachMainWithSub(CK_WHERE, CK_BOOLCALC)) {
      Board::vOuter_Query_Analysis_Of_where_boolCalc(m_pCI);
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // search for having calls to sub-queries

    m_pCI->vRestart();
    while (m_pCI->G_bEachMainWithSub(CK_HAVING, CK_BOOLCALC)) {
      Board::vOuter_Query_Analysis_Of_having_boolCalc(m_pCI);
    }

    pOuterQuery->vDump();

  }

  //m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* Analyse main query for sub-queries                                         */
/*----------------------------------------------------------------------------*/
void Bench::vTransform_SubQueryAnalysisMainQuery(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  m_pCI->vTransform(__func__, "Analyse main query for sub-queries"_Stz);


  SubQuery *pMainQuery = pGetBoard((i8)0)->pSubQuery();
  pMainQuery->vDisposition(QD_LINEAR);

  m_pCI->G_vRestart();
  while (m_pCI->G_bEachMain(CK_AGGREG)) {
    //SubQuery* pMainQuery =  pGetBoard((i8)0)->pSubQuery();

    // the main-query has an aggreg
    pMainQuery->vDisposition(QD_AGGREGATION);
  }

  m_pCI->G_vRestart();
  while (m_pCI->G_bEachMain(CK_GBYAGG)) {
    //SubQuery* pMainQuery =  pGetBoard((i8)0)->pSubQuery();

    // the main-query has an gbyagg
    pMainQuery->vDisposition(QD_AGGREGATION);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  i8 nBoards = this->nBoards();
  if (nBoards > 1) {
    m_pCI->G_vRestart();
    while (m_pCI->G_bEachMain(CK_CALL)) {
      SubQuery* pSubQuery =  pGetBoard((i8)1)->pSubQuery();

      pSubQuery->vCaller(0);
      pSubQuery->vClause(QC_FROM_CLAUSE);

      //SubQuery* pMainQuery =  pGetBoard((i8)0)->pSubQuery();
      pMainQuery->vSubQueries(1);
    }
//    m_pCI->G_vRestart();
//    while (m_pCI->G_bEachMainWithSub(CK_WHERE, CK_RPNCALC)) {
//      Board::vS26_T10d_Main_Query_Analysis_Of_where_rpnCalc(m_pCI);
//    }
    m_pCI->G_vRestart();
    while (m_pCI->G_bEachMainWithSub(CK_HAVING, CK_BOOLCALC)) {
      Board::vMain_Query_Analysis_Of_having_boolCalc(m_pCI);
    }
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vTransform_SubQueryAnalysisSubQuery(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  m_pCI->vTransform(__func__, "Analyse sub-queries"_Stz);

  vAssert_Board_mqA();


  i8 nBoards = this->nBoards();
  if (1 == nBoards) {
    // there is no sub-query

    SubQuery* pMainQuery =  pGetBoard((i8)0)->pSubQuery();

    pMainQuery->vSubQueries(0);
  }
  else
  if (2 == nBoards) {

    if (m_pCI->G_bPushBoard(1)) {

      SubQuery* pSubQuery =  pGetBoard(1)->pSubQuery();
      pSubQuery->vDisposition(QD_LINEAR);

//      while (m_pCI->G_bEachMain()) {
//        m_pCI->G_vNewMember(CK_GENESIS,"sq"_Stz);
//        if ((m_pCI->G.p_level = m_pCI->G.p_MainCard->pSub_level()))
//          m_pCI->G.p_MainCard->vInsertAfter(m_pCI->G.p_genesis,m_pCI->G.p_level);
//        else
//          m_pCI->G.p_MainCard->vPush(m_pCI->G.p_genesis);
//      }

      m_pCI->G_vRestart();
      while (m_pCI->G_bEachMain(CK_PARAMLIST)) {
        // there is a subQuery and it has a paramList

        //SubQuery* pSubQuery =  pGetBoard(1)->pSubQuery();

        // TODO: hack the number of params... fix
        i8 nParams = 1; //m_pCI->G.p_paramList->nChildCards();

        pSubQuery->vCorrCols(nParams);
      }

      m_pCI->G_vRestart();
      while (m_pCI->G_bEachMain(CK_AGGREG)) {
        // there is a subQuery and it has an aggreg

        //SubQuery* pSubQuery =  pGetBoard(1)->pSubQuery();

        pSubQuery->vDisposition(QD_AGGREGATION);
      }

      m_pCI->G_vRestart();
      while (m_pCI->G_bEachMain(CK_GBYAGG)) {
        // there is a subQuery and it has an gbyAgg
        //SubQuery* pSubQuery =  pGetBoard(1)->pSubQuery();
        pSubQuery->vDisposition(QD_AGGREGATION);
      }

      m_pCI->G_vPopIter();
    }
  }
  else
    NEVER_REACH;

  m_pCI->vEndTransform();

}

/*----------------------------------------------------------------------------*/
/* convert all <table> into either <scan> , <indexJoin> or <keyExists>        */
/*----------------------------------------------------------------------------*/
void Bench::vTransform_MorphTableToEEop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  m_pCI->vTransform(__func__, "convert all <table> into either <scan> or <indexJoin>"_Stz);

  vAssert_Board_mqA();

  Board::vMorph_Table_To_EEop(m_pCI);

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* inspect filters and remove cmp predicates which are in bounds              */
/*----------------------------------------------------------------------------*/
void Bench::vTransform_CullNonusedBounds(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  m_pCI->vTransform(__func__, "((desc))"_Stz);

  vAssert_Board_mqA();

  m_pCI->vPrintBoard(__LINE__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //

  m_pCI->G_vRestart();

  while (m_pCI->G_bEachMain()) {

    if (!(m_pCI->G.p_MainCard->bIsCard(CK_INDEXJOIN) ||
          m_pCI->G.p_MainCard->bIsCard(CK_INDEXSCAN)))
      continue;
    if (!(m_pCI->G.p_indexList = m_pCI->G.p_MainCard->pSub_indexList()))
      continue;

    // have <bounds> & <filter> under <main>



    Card_Index::vCullNonusedBounds(m_pCI);

//    while ((m_pCI->G.p_index = _pCard_Index(xCI_indexes.pEachChild(CK_INDEX)))) {
//
//      bool bChosenIndex = m_pCI->G.p_index->Y_bHas(P_ChosenIndex);
//
//      m_pCI->G.p_bounds = m_pCI->G.p_index->pSub_bounds();
//      if (! m_pCI->G.p_bounds)
//        continue;
//
//      CardIter xCI_bounds(m_pCI->G.p_bounds);
//      while (xCI_bounds.bEachChild()) {
//        if (bChosenIndex && ! bFoundFirst && xCI_bounds.G.m_pChild->bIsCard(CK_EBOUND)) {
//          bFoundFirst = true;
//          continue;
//        }
//
//        xCI_bounds.G.m_pChild->vMark1(MARK_AT);
//      }
//
//      m_pCI->vPrintBoard(__LINE__);
//      m_pCI->G.p_bounds->vCullBound(MARK_AT);
//
//    }
  }


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


  m_pCI->vPrintBoard();

  m_pCI->vEndTransform();
}


/*----------------------------------------------------------------------------*/
/* inspect filters and remove cmp predicates which are in bounds              */
/*----------------------------------------------------------------------------*/
void Bench::vTransform_CullUsedBoundsFromFilter(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  m_pCI->vTransform(__func__, "((desc))"_Stz);

  vAssert_Board_mqA();

  m_pCI->vPrintBoard(__LINE__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // mark any cmpPred that are a bound in bounds

  m_pCI->G_vRestart();
  while (m_pCI->G_bEachMain()) {

    if (! (m_pCI->G.p_MainCard->bIsCard(CK_INDEXJOIN) ||
           m_pCI->G.p_MainCard->bIsCard(CK_INDEXSCAN)))
      continue;
    if (! (m_pCI->G.p_bounds = m_pCI->G.p_MainCard->pSub_bounds()))
      continue;
    if (! (m_pCI->G.p_filter = m_pCI->G.p_MainCard->pSub_filter()))
      continue;

    // have <bounds> & <filter> under <main>

    CardIter xCR(m_pCI->G.p_filter);
    while ((m_pCI->G.p_cmpPred = _pCard_CmpPred(xCR.pRecurseEach(CK_CMPPRED)))) {
      if (m_pCI->G.p_cmpPred->bSubsetOf_bounds(m_pCI->G.p_bounds))
        m_pCI->G.p_cmpPred->vMark1(MARK_AT);
    }
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cull marked cmpPred from filter

  m_pCI->G_vRestart();
  Card_Filter::vCullUsedBounds(m_pCI);

//  while (m_pCI->G_bEachMain()) {
//    if ((m_pCI->G.p_filter = m_pCI->G.p_MainCard->pSub_filter()))
//      m_pCI->G.p_filter->vCullPred(MARK_AT);
//  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // delete any filter if empty

  m_pCI->G_vRestart();
  Card_Filter::vDeleteEmptyFilters(m_pCI);

//  while (m_pCI->G_bEachMain()) {
//    if ((m_pCI->G.p_filter = m_pCI->G.p_MainCard->pSub_filter()))
//      m_pCI->G.p_MainCard->vDeleteFilterIfEmpty(m_pCI->G.p_filter);
//  }

  m_pCI->vPrintBoard();

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* individuate <scan|indexJoin>/<filter> as main <filter>                     */
/*----------------------------------------------------------------------------*/
void Bench::vTransform_IndividuateFilter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  m_pCI->vTransform(__func__, "individuate <scan|indexJoin>/<filter> as main <filter>"_Stz);

  vAssert_Board_mqA();

  m_pCI->G_vRestart();
  Card_Scan::vIndividuateFilter(m_pCI);

  m_pCI->G_vRestart();
  Card_IndexScan::vIndividuateFilter(m_pCI);

  m_pCI->G_vRestart();
  Card_IndexJoin::vIndividuateFilter(m_pCI);

  m_pCI->G_vRestart();
  Card_CrossJoin::vIndividuateFilter(m_pCI);

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vTransform_TblColToStmCol(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  m_pCI->vTransform(__func__, "vTransform_TblColToStmCol"_Stz);

  vAssert_Board_mqA();

  m_pCI->G.pBoard->vMayHaveStmColOdn(true);

  while (m_pCI->G_bEachMain())
    ((Card_OP *) m_pCI->G.p_MainCard)->vConvert_tblColOdn_To_stmColOdn(m_pCI);

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vTransform_StmColToColNmOfKeyExists(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  m_pCI->vTransform(__func__, "((20))))"_Stz);

  vAssert_Board_mqA();

  m_pCI->G.pBoard->vMayHaveStmColOdn(true);

  while (m_pCI->G_bEachMain()) {

    if (! (   CK_KEYEXISTS    == m_pCI->G.e_MainCard
           || CK_KEYNOTEXISTS == m_pCI->G.e_MainCard))
      continue;

    m_pCI->G.p_constraint = m_pCI->G.p_MainCard->pSub_constraint();

    m_pCI->G.p_constraint->vConvert_stmColOdn_To_stmColNm(m_pCI);
  }

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* reinstate stages                                                           */
/*----------------------------------------------------------------------------*/
void Bench::vTransform_ReinstateStages(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  m_pCI->vTransform(__func__, "reinstate stages"_Stz);

  vAssert_Board_mqA();

  //m_pCI->m_pCardCtl->vPrintSubCards(true,CK_PROJECTION,0);
  //m_pCI->m_pCardCtl->vPrintSubCards(true,CK_RPNCALC,0);

  m_pCI->vPrintBoard(__LINE__);

  Board::vReinstate_stages(m_pCI);

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* add partitioning                                                           */
/*----------------------------------------------------------------------------*/
void Bench::vTransform_AddPartitioning(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  m_pCI->vTransform(__func__, "add partitioning"_Stz);

  vAssert_Board_mqA();

  Board::vAdd_Partitioning(m_pCI);

  m_pCI->vEndTransform();
}


/*----------------------------------------------------------------------------*/
/*-- fold --------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vFold_10_F(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  // F
  // push a (from) sub-query before the main-query
  // as an op-sequence
  // (refer: Q7,8,9)

  /*
      select count(*)
        from (select *
                from orders, lineitem
               where L_ORDERKEY = O_ORDERKEY)
      ;
   */

  m_pCI->vTransform(__func__, "((20))"_Stz);

  vAssert_Board_mqA();
  m_pCI->vPrintBoard(__LINE__);

  // 1) find W_A_C_ro subquery call in mq:<where>/<boolCalc>
  vAssert_Board_mqA();
  RASSERT(m_pCI->G_bEachMain(CK_CALL));
  vTrace(">>>>>>>>> Found From CALl: %s\n"_Stz,m_pCI->G.p_call->szValue());
  m_pCI->vPrintBoard(__LINE__);


  m_pCI->G_vNewMember(CK_FOLD,"FROM"_Stz);
  m_pCI->G.pDeck->vPush(m_pCI->G.p_fold);

  m_pCI->vPrintBoard(__LINE__);


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get copy of sq and push before mq

  // x) push to sq
  if (! m_pCI->G_bPushBoard(m_pCI->G.p_call->szValue())) {
    vTrace("DID NOT FIND Board %s\n"_Stz,m_pCI->G.p_call->szValue());
    NEVER_REACH
  }
  vAssert_Board_sqB();

  m_pCI->m_pCard_copied = m_pCI->G.pDeck->pDupSubCards(0);

  m_pCI->vPrintBoard(__LINE__);

  // ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^

  // x) pop back to mq
  m_pCI->G_vPopIter();
  vAssert_Board_mqA();

  m_pCI->G.pDeck->vPush(m_pCI->m_pCard_copied);
  m_pCI->G.pDeck->vReplaceChildWithItsSubs(m_pCI->m_pCard_copied);
  DELETE_CARD(m_pCI->m_pCard_copied);

  m_pCI->G.pDeck->vRemoveChild(m_pCI->G.p_call);
  DELETE_CARD(m_pCI->G.p_call);

  m_pCI->G.pDeck->vRenumberRecursive();
  m_pCI->vPrintBoard(__LINE__);

  // 7) derive the fold (SQ result) expr
  m_pCI->G_vRestart();
  if (m_pCI->G_bEachMain(CK_FOLD)) {
    m_pCI->G.i_FoldCard = m_pCI->G.i_MainCard;
    Board::vDerive_foldProjection_F(m_pCI);
  }
  m_pCI->vPrintBoard(__LINE__);

//  Card_Stream::vStream_AdjustLoHi(m_pCI);
//  m_pCI->vPrintBoard(__LINE__);
//
//  Board::vRenumberStmOdn(m_pCI);
//  m_pCI->vPrintBoard(__LINE__);

  //Board::vRelabelExpressions(m_pCI);

  Card_Projection::vProjection_Analysis_v2(m_pCI);
  m_pCI->G.pDeck->vRenumberRecursive();
  m_pCI->vPrintBoard(__LINE__);

  vIsFlat(true);
  m_pCI->G.pDeck->vClearMarksRecursive();

  m_pCI->vPrintBoard(__LINE__);


  // discuss with Alka
  m_pCI->G_vRestart();
  if (m_pCI->G_bEachMain(CK_FOLD)) {
    if (m_pCI->G_bEachMain(CK_WHERE)) {
      Card_Level* p_level_where = m_pCI->G.p_where->pSub_level();
      if (m_pCI->G_bEachMainWithSub(CK_PROJECT, CK_LEVEL)) {
        p_level_where->Y_vSet(P_Level,m_pCI->G.p_level->Y_iGet(P_Level));
      }
    }
  }

  m_pCI->vEndTransform();
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vFold_20_W_A_C_ro(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  // W A C ro
  // push a (single-value aggregate correlated ro) sub-query before the main-query
  // as an aggregation-fan (projecting key-of-correlation and aggregated value)
  // further: the key-of-correlation needs to be added to group-by)
  // (refer: Q2,17)

  /* ARCHETYPE :
    select count(*)
      from Lineitem, Part, PartSupp
     where P_PARTKEY  = L_PARTKEY
       and PS_PARTKEY = P_PARTKEY
       and l_quantity < (select avg(l_quantity)
                           from lineitem
                          where L_PARTKEY = P_PARTKEY)
    ;
 */

  m_pCI->vTransform(__func__, "((20))"_Stz);

  // 1) find W_A_C_ro subquery call in mq:<where>/<boolCalc>
  vAssert_Board_mqA();
  RASSERT(m_pCI->G_bEachMainWithSub(CK_WHERE, CK_BOOLCALC));
  if (m_pCI->m_bRecursiveModel) {
    if (! Board::bFindAndMarkSubQryCall_recursiveModel(m_pCI)) {
      vTrace("DID NOT FIND W_A_C_ro SubQuery call\n"_Stz);
      NEVER_REACH
    }
  }
  else {
    if (! Board::bFindAndMarkSubQryCall_originalModel(m_pCI)) {
      vTrace("DID NOT FIND W_A_C_ro SubQuery call\n"_Stz);
      NEVER_REACH
    }
  }
  vTrace(">>>>>>>>> Found W_A_C_ro call:  %s %s %s\n"_Stz,m_pCI->m_sSqyArg.szStz(),m_pCI->m_sSqyRO.szStz(),m_pCI->m_sSqyNm.szStz());
  m_pCI->vPrintBoard(__LINE__);


  m_pCI->G_vNewMember(CK_FOLD,"W_A_C_ro"_Stz);
  m_pCI->G.pDeck->vPush(m_pCI->G.p_fold);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get copy of sq and push before mq

  // x) push to sq
  if (! m_pCI->G_bPushBoard(m_pCI->m_sSqyNm)) {
    vTrace("DID NOT FIND Board %s\n"_Stz,m_pCI->m_sSqyNm.szStz());
    NEVER_REACH
  }
  vAssert_Board_sqB();

  m_pCI->m_pCard_copied = m_pCI->G.pDeck->pDupSubCards(1);

  m_pCI->vPrintBoard(__LINE__);

  // ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^

  // x) pop back to mq
  m_pCI->G_vPopIter();
  vAssert_Board_mqA();

  m_pCI->G.pDeck->vPush(m_pCI->m_pCard_copied);
  m_pCI->G.pDeck->vReplaceChildWithItsSubs(m_pCI->m_pCard_copied);
  DELETE_CARD(m_pCI->m_pCard_copied);

  m_pCI->G.pDeck->vRenumberRecursive();

  m_pCI->vPrintBoard(__LINE__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // convert the parameterised aggreg-sq into a gbyAgg-sq gby correlation column
  // such that the result of the sq is a stream { __SQ1IGBKY , __SQ1RESULT }

  m_pCI->G_vRestart();

  // v                             //  ]filter
  // T A A A P F T T T W A A A     //  .]boolCalc
  // s                             //  ..]cmpPred ==
  // f                             //           $d.2  ((m_pCI->m_pTerm_corrPred))
  //                               //             %1
  RASSERT(m_pCI->G_bEachMainWith2Sub(CK_TABLE,CK_STREAM,CK_FILTER));
  Board::vIdentify_Correlation_Column_In_cmpPred_Of_sq_boolCalc(m_pCI);
  RASSERT(m_pCI->m_pTerm_corrPred); // eg $d.2 (L_PARTKEY)
  Card_StmCol::vFind_stmCol_Definition_Of_Correlation_Column(m_pCI);
  RASSERT(m_pCI->G.p_stmCol);       // eg L_PARTKEY


  // T A A A P F T T T W A A A     //
  //   v v v                       //
  // T G G G P F T T T W A A A     // push corrPred <expr> into <gbyElist> and <aggElist>

  RASSERT(m_pCI->G_bEachMain(CK_AGGREG));
  Board::vEnrich_sq_aggreg_ToBe_gbyAgg_Gby_Correlation_Column(m_pCI);

  m_pCI->vPrintBoard(__LINE__);

  // T A A A P F T T T W A A A     //
  //         v                     //
  // T G G G P F T T T W A A A     // push corrPred <expr> into <projElist>

  if (m_pCI->G_bIsNextMain(CK_PROJECT)) {
    RASSERT(m_pCI->G_bEachMain(CK_PROJECT));
    Board::vEnhance_sq_project_To_Include_Correlation_Column(m_pCI);
  }
  else {

  }
  RASSERT(m_pCI->G_bEachMain(CK_FOLD));
  Board::vDerive_foldElist_From_project_stream(m_pCI);

  m_pCI->G.pDeck->vRenumberRecursive();
  Card_Projection::vRelabelExpressions(m_pCI);
  m_pCI->vPrintBoard(__LINE__);

  // T G G G P F T T T W A A A      //
  //             s s s              // adjust stream ordinsla

  Card_Stream::vStream_AdjustLoHi(m_pCI);
  Board::vRenumberStmOdn(m_pCI);
  m_pCI->vPrintBoard(__LINE__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // convert the mq to join to the gby correlation column streamed from sq

  m_pCI->G_vRestart();

  // v                             //  ]filter
  // T A A A P F T T T W A A A     //  .]boolCalc
  // s                             //  ..]cmpPred ==
  // f                             //           $d.2  ((m_pCI->m_pTerm_corrPred))
  //                               //             %1

  RASSERT(m_pCI->G_bEachMainWith2Sub(CK_TABLE,CK_FILTER,CK_BOOLCALC))
  bool b = m_pCI->G.p_boolCalc->bYank_PRED(MARK_AT, m_pCI->G.p_PRED);
  RASSERT(b);
  RASSERT(m_pCI->G.p_PRED->Y_bHas(P_CorrPred));
  m_pCI->vPrintBoard(__LINE__);

  m_pCI->G.p_table->vRemove_filter_IfEmpty(m_pCI->G.p_filter);
  m_pCI->vPrintBoard(__LINE__);

  //                   v           //  ]filter
  // T A A A P F T T T W A A A     //  .]boolCalc
  // s                 f           //  ..]cmpPred ==
  // f                 bc          //           $d.2  ((m_pCI->m_pTerm_corrPred))
  //                               //             %1

  RASSERT(m_pCI->G_bEachMain(CK_FOLD));
  RASSERT(m_pCI->G_bEachMainWithSub(CK_WHERE, CK_BOOLCALC));
  m_pCI->G.p_boolCalc->vPush_Correlated_cmpPred_Onto_mq_boolCalc(m_pCI->G.p_PRED);
  m_pCI->vPrintBoard(__LINE__);

  //                   v           //  ]filter
  // T A A A P F T T T W A A A     //  .]boolCalc
  // s                 f           //  ..]roPred <
  // f                 bc          //           $a.5
  //                               //           $b.1
  //                               // __subquery0002

  Board::vResolve_SubqueryCall_In_mq_boolCalc_Into_cmpPred(m_pCI, m_pCI->G.p_boolCalc);
  m_pCI->vPrintBoard(__LINE__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // move the correlation predicate to table as a bounds

  // TODO: this should be in two steps

  m_pCI->G_vRestart();

  //                   v           //  ]filter
  // T A A A P F T T T W A A A     //  .]boolCalc
  // s                 f           //  ..]cmpPred ==
  // f                 bc          //           $d.2  ((m_pCI->m_pTerm_corrPred))
  //             b <-  cp          //             %1

  RASSERT(m_pCI->G_bEachMain(CK_FOLD));
  RASSERT(m_pCI->G_bEachMainWithSub(CK_WHERE, CK_BOOLCALC));
  Board::vMove_corrPred_To_mq_table_As_bounds(m_pCI, m_pCI->G.p_boolCalc);
  m_pCI->vPrintBoard(__LINE__);


//  m_pCI->G_vRestart();
//  if (m_pCI->G_bEachMain(CK_FOLD)) {
//    if (m_pCI->G_bEachMain(CK_WHERE)) {
//      // TODO:
//      // R_vUTE_Unproject_foldExpr_After_where();
//      R_vUTE_Bump_where_BaseOn_foldExpr();
//    }
//  }

  vIsFlat(true);
  m_pCI->G.pDeck->vClearMarksRecursive();
  m_pCI->vPrintBoard(__LINE__);
  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vFold_30_W_A_NC_I(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  // W A NC I
  // push a (single-value aggregate non-correlated in) sub-query before the main-query
  // as an aggregation-fan (projecting a single aggregated value)
  // (refer: Q18)

  /* ARCHETYPE :
      select count(*)
        from orders, lineitem
       where O_ORDERKEY = L_ORDERKEY
         and O_ORDERKEY in (select L_ORDERKEY
                              from lineitem
                             group by L_ORDERKEY
                            having sum(l_quantity) > 100)
      ;
   */

  m_pCI->vTransform(__func__, "((vFold_30_W_A_NC_I))"_Stz);

  vAssert_Board_mqA();


  // W A NC I
  //               //
  // T T W A A A   // find W_L_NC_I subquery call in mq:<where>/<boolCalc>
  //     f         //
  //     bc        //
  //    @ro        //

  vAssert_Board_mqA();
  if (m_pCI->G_bEachMainWithSub(CK_WHERE, CK_BOOLCALC)) {
    if (m_pCI->m_bRecursiveModel) {
      if (! Board::bFindAndMarkSubQryCall_recursiveModel(m_pCI)) {
        vTrace("DID NOT FIND W_A_C_ro SubQuery call\n"_Stz);
        NEVER_REACH
      }
    }
    else {
      if (! Board::bFindAndMarkSubQryCall_originalModel(m_pCI)) {
        vTrace("DID NOT FIND W_A_C_ro SubQuery call\n"_Stz);
        NEVER_REACH
      }
    }
  }
  vTrace("Found W_A_NC_I call to %s(%s)->%s\n"_Stz,m_pCI->m_sSqyNm.szStz(),m_pCI->m_sSqyArg.szStz(),m_pCI->m_sSqyRsl.szStz());

  m_pCI->G_vNewMember(CK_FOLD,"W_A_NC_I"_Stz);
  m_pCI->G.pDeck->vPush(m_pCI->G.p_fold);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // x) push to sq
  if (! m_pCI->G_bPushBoard(m_pCI->m_sSqyNm)) {
    vTrace("DID NOT FIND Board %s\n"_Stz,m_pCI->m_sSqyNm.szStz());
    NEVER_REACH
  }
  vAssert_Board_sqB();

  m_pCI->m_pCard_yanked = m_pCI->G.pDeck->pDupSubCards(0);

  m_pCI->vPrintBoard(__LINE__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // x) pop back to mq
  m_pCI->G_vPopIter();
  vAssert_Board_mqA();

  // W A NC I
  //       I-sqr-> $1            // Folded nc sq provides sqr into mq as $1
  //               ----mq-----   //
  // T G G G H P F T T W A A A   // push sq before Fold
  // ----sq-----                 //

  m_pCI->G.pDeck->vPush(m_pCI->m_pCard_yanked);
  m_pCI->G.pDeck->vReplaceChildWithItsSubs(m_pCI->m_pCard_yanked);
  DELETE_CARD(m_pCI->m_pCard_yanked);

  m_pCI->vPrintBoard(__LINE__);

  // W A NC I
  //       I-sqr-> $1            // Put an expr describing sqr into Fold
  //           v v               //
  // T G G G H P F T T W A A A   // derive foldElist from sq Project
  //           s el              //

//  RASSERT(m_pCI->G_bEachMainWithSub(CK_PROJECT,CK_STREAM));
//  RASSERT(m_pCI->G_bEachMain(CK_FOLD));
//  Board::vDerive_foldElist_From_project_stream(m_pCI);
  // 7) derive the fold (SQ result) expr
  m_pCI->G_vRestart();
  if (m_pCI->G_bEachMain(CK_FOLD)) {
    m_pCI->G.i_FoldCard = m_pCI->G.i_MainCard;
    Board::vDerive_foldProjection_F(m_pCI);
    //m_pCI->G.p_fold->Y_vSet(P_BumpRight,(i8)1);

  }

  m_pCI->vPrintBoard(__LINE__);

  // W A NC I
  //       I-sqr-> $1            // Injection of sqr into mq adds new $1
  //                     v v v   //
  // T G G G H P F T T W G G G   // bump stmOdn:$n in mq gbyElist by 1
  //                     elelel  // (if G is actually G -- might be A)

//  if (m_pCI->G_bEachMainWithSub(CK_GBYAGG,CK_KEY)) {
//    m_pCI->G.p_fold->Y_vSet(P_BumpRight,(i8)1);
//    i8 nBumpRight = m_pCI->G.p_fold->Y_iGet(P_BumpRight);
//    m_pCI->G.p_key->vBump_stmOdn_In_rpnCalc(m_pCI);
//
//    m_pCI->G.p_fold->Y_vCut(P_BumpRight);
//    m_pCI->G.p_fold->Y_vSet(P_BumpedRight,(nBumpRight));
//    m_pCI->vPrintBoard(__LINE__);
//  }

  Card_Projection::vProjection_Analysis_v2(m_pCI);
  m_pCI->G.pDeck->vRenumberRecursive();
  m_pCI->vPrintBoard(__LINE__);

  m_pCI->G_vRestart();
  if (m_pCI->G_bEachMain(CK_FOLD)) {
    m_pCI->G.p_fold->vBump_Rightwards(m_pCI, 1);
    m_pCI->vPrintBoard(__LINE__);
  }

  m_pCI->G_vRestart();
  while (m_pCI->G_bEachMain())
    ((Card_OP *) m_pCI->G.p_MainCard)->vConvert_stmColOdn_To_tblColOdn_recursive(m_pCI);
  m_pCI->vPrintBoard(__LINE__);


  // W A NC I
  //       I-sqr-> $1            // The mq IN transforms to arg == $1 (the corr pred)
  //                             //
  // T G G G H P F T T W G G G   // find table for sqy arg (will be an mq table)
  //               f             // derive cmpPred from sqy arg
  //               bc            // add cmpPred to table
  //               cp  @ro       //


  m_pCI->G_vRestart();
  if (m_pCI->G_bEachMain(CK_WHERE)) {

    m_pCI->G.p_bolPred = m_pCI->G.p_where->pSub_bolPred();
    m_pCI->G.p_bolPred->vMorph_To_cmpPred(m_pCI);
  }

  m_pCI->vPrintBoard(__LINE__);




/*

  char cTblSyn = Card::cTableColSyn(m_pCI->m_sSqyArg);
  m_pCI->G.p_table = m_pCI->G.pDeck->pFindTableCard(cTblSyn, m_pCI->G.p_MainCard);

  m_pCI->G_vNewMember(CK_CMPPRED,"=="_Stz);
  m_pCI->G_vNewMember(CK_TERM,m_pCI->m_sSqyArg.szStz());
  m_pCI->G.p_cmpPred->vAppend(m_pCI->G.p_term);
  //m_pCI->G_vNewMember(CK_TERM,m_pCI->m_sSqyRsl.szStz());
  m_pCI->G_vNewMember(CK_TERM,"$1"_Stz);
  m_pCI->G.p_cmpPred->vAppend(m_pCI->G.p_term);

  m_pCI->G.p_table->vAddPredToTableFilter(m_pCI,m_pCI->G.p_cmpPred);
  m_pCI->vPrintBoard(__LINE__);

  // W A NC I
  //       I-sqr-> $1            // The corr pred in mq is a new bounds
  //                             //
  // T G G G H P F T T W G G G   // derive bounds from filter
  //               b             //
  //               ^             //
  //               f             //
  //               bc            //
  //              @cp            //

  m_pCI->G.p_table->vBounds_DeriveFrom_filter(m_pCI->G.p_filter,false);
  m_pCI->vPrintBoard(__LINE__);

  // W A NC I
  //       I-sqr-> $1            // Moved corr pred can be culled
  //                             //
  // T G G G H P F T T W G G G   // cull @cp from filter
  //               b             //
  //               f             //
  //               bc            //

  m_pCI->G.p_boolCalc = m_pCI->G.p_filter->pSub_boolCalc();
  m_pCI->G.p_boolCalc->vCullPred(MARK_UP);
  m_pCI->vPrintBoard(__LINE__);

  // W A NC I
  //       I-sqr-> $1            // Culled corr pred may leave filter empty
  //                             //
  // T G G G H P F T T W G G G   // delete empty filter
  //               b             //

  Board::vS36_T30_Delete_Empty_filter(m_pCI);

  // W A NC I
  //       I-sqr-> $1            // The original ro pred can now be removed
  //                             //
  // T G G G H P F T T W A A A   // cull the ro pred
  //                   f         //
  //                   bc        //
  //                  @ro (cull) //

  m_pCI->G_vRestart();
  if (m_pCI->G_bEachMainWithSub(CK_WHERE,CK_BOOLCALC)) {
    m_pCI->G.p_boolCalc->vCullPred(MARK_AT);
  }

  m_pCI->vPrintBoard(__LINE__);

  // W A NC I
  //       I-sqr-> $1            // Removal of ro pred may leave Where empty
  //                             //
  // T G G G H P F T T W A A A   // delete potentially empty where
  //                   f         //
  //                   bc        //
  //                 (empty)     //

  m_pCI->G_vRestart();
  m_pCI->G_vDeleteWhenEmptySub(CK_WHERE,CK_BOOLCALC);

  m_pCI->vPrintBoard(__LINE__);


  // todo: should this be earlier? As soon as $1 is injected into mq??

  Card_Stream::vStream_AdjustLoHi(m_pCI);
  m_pCI->vPrintBoard(__LINE__);

  Board::vRenumberStmOdn(m_pCI);
  m_pCI->vPrintBoard(__LINE__);

  Card_Projection::vRelabelExpressions(m_pCI);
*/

  vIsFlat(true);
  m_pCI->G.pDeck->vClearMarksRecursive();

  m_pCI->vPrintBoard(__LINE__);

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vFold_40_W_A_NC_ro(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  // W A NC ro
  // push a (single-value aggregate non-correlated in) sub-query before the main-query
  // as an aggregation-fan (projecting a single aggregated value)
  // (refer: Q15)

  /*
   */

  m_pCI->vTransform(__func__, "((30))"_Stz);

  vAssert_Board_mqA();

  Card_Term* p_term_sqCall = nullptr;

  // 1) find W_L_C_E subquery call in mq:<where>/<boolCalc>
  vAssert_Board_mqA();
  if (m_pCI->G_bEachMainWithSub(CK_WHERE, CK_BOOLCALC)) {
    if (m_pCI->m_bRecursiveModel) {
      if (! Board::bFindAndMarkSubQryCall_recursiveModel(m_pCI)) {
        vTrace("DID NOT FIND W_A_C_ro SubQuery call\n"_Stz);
        NEVER_REACH
      }
      p_term_sqCall = m_pCI->m_pTerm_sqCall;
    }
    else {
      if (! Board::bFindAndMarkSubQryCall_originalModel(m_pCI)) {
        vTrace("DID NOT FIND W_A_C_ro SubQuery call\n"_Stz);
        NEVER_REACH
      }
    }
  }
  vTrace("Found W_A_NC_ro call to %s()->%s\n"_Stz,m_pCI->m_sSqyNm.szStz(),m_pCI->m_sSqyRsl.szStz());

  m_pCI->G_vNewMember(CK_FOLD,"W_A_NC_ro"_Stz);
  m_pCI->G.pDeck->vPush(m_pCI->G.p_fold);

  /*
  // find table for sqyarg
  char cTblSyn = Card::cTableColSyn(m_pCI->m_sSqyArg);
  m_pCI->G.p_table = m_pCI->G.pDeck->pFindTableCard(cTblSyn, m_pCI->G.p_MainCard);

  // TODO: derive cmpPred from marked tstPred and add to corresponding table

  m_pCI->G_vNewMember(CK_CMPPRED,"=="_Stz);
  m_pCI->G_vNewMember(CK_TERM,m_pCI->m_sSqyArg.szStz());
  m_pCI->G.p_cmpPred->vAppend(m_pCI->G.p_term);
  //m_pCI->G_vNewMember(CK_TERM,m_pCI->m_sSqyRsl.szStz());
  m_pCI->G_vNewMember(CK_TERM,"$1"_Stz);
  m_pCI->G.p_cmpPred->vAppend(m_pCI->G.p_term);

  R_vUTE_AddPredToTableFilter(m_pCI->G.p_table,m_pCI->G.p_cmpPred);
  m_pCI->vPrintBoard(__LINE__);

  m_pCI->G_vRestart();
  if (m_pCI->G_bEachMainWithSub(CK_WHERE,CK_BOOLCALC)) {
    m_pCI->G.p_boolCalc->vBoolCalc_CullPred(MARK_AT);
  }

  Board::vRelabelExpressions(m_pCI);

  m_pCI->vPrintBoard(__LINE__);

  // delete potentially empty where
  m_pCI->G_vRestart();
  m_pCI->G_vDeleteWhenEmptySub(CK_WHERE,CK_BOOLCALC);

  m_pCI->vPrintBoard(__LINE__);

*/

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // x) push to sq
  if (! m_pCI->G_bPushBoard(m_pCI->m_sSqyNm)) {
    vTrace("DID NOT FIND Board %s\n"_Stz,m_pCI->m_sSqyNm.szStz());
    NEVER_REACH
  }
  vAssert_Board_sqB();

  m_pCI->m_pCard_yanked = m_pCI->G.pDeck->pDupSubCards(0);

  m_pCI->vPrintBoard(__LINE__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // x) pop back to mq
  m_pCI->G_vPopIter();
  vAssert_Board_mqA();

  m_pCI->G.pDeck->vPush(m_pCI->m_pCard_yanked);
  m_pCI->G.pDeck->vReplaceChildWithItsSubs(m_pCI->m_pCard_yanked);
  DELETE_CARD(m_pCI->m_pCard_yanked);

  m_pCI->vPrintBoard(__LINE__);

  // 7) derive the fold (SQ result) expr
  m_pCI->G_vRestart();
  if (m_pCI->G_bEachMain(CK_FOLD)) {
    m_pCI->G.i_FoldCard = m_pCI->G.i_MainCard;
    Board::vDerive_foldExpr_H_A_NC_ro(m_pCI);
  }
  m_pCI->vPrintBoard(__LINE__);


  if (m_pCI->G_bEachMain(CK_WHERE)) {
    m_pCI->G.p_where->vBump_Rightwards(1);
  }
  m_pCI->vPrintBoard(__LINE__);

  if (m_pCI->G_bEachMain(CK_GBYAGG)) {
    m_pCI->G.p_gbyAgg->vBump_Rightwards(1);
  }
  m_pCI->vPrintBoard(__LINE__);




  // 8) inject SQ result into MQ <gbyAgg>
//  Board::vInject_sqResult_Into_gbyAgg(m_pCI);
//  m_pCI->G.pDeck->vRenumberRecursive();
//  m_pCI->vPrintBoard(__LINE__);


  m_pCI->G_vRestart();
  Card_Projection::vProjection_Analysis_v2(m_pCI);
  m_pCI->vPrintBoard(__LINE__);


  m_pCI->G_vRestart();
  if (m_pCI->G_bEachMain(CK_WHERE)) {

    m_pCI->G.p_roPred = m_pCI->G.p_where->pSub_roPred();
    m_pCI->G.p_roPred->vMorph_To_bolPred(m_pCI);
  }

  m_pCI->vPrintBoard(__LINE__);


  p_term_sqCall->vReplace_sqCall_With_sqResult("$1"_Stz); // todo: sq result no
  m_pCI->vPrintBoard(__LINE__);
  vDebugPoint();


  vIsFlat(true);
  m_pCI->G.pDeck->vClearMarksRecursive();

  m_pCI->vPrintBoard(__LINE__);

  m_pCI->vEndTransform();









  /*

  while (! m_pCI->G_bIsNextMain(CK_FOLD)) {

    if (m_pCI->G_bIsNextMain(CK_TABLE)) {
      RASSERT(m_pCI->G_bEachMain());
      continue;
    }
    else
    if (m_pCI->G_bIsNextMain(CK_PROJECT)) {
      RASSERT(m_pCI->G_bEachMainWithSub(CK_PROJECT, CK_STREAM));
      break;
    }
    else
    if (m_pCI->G_bIsNextMain(CK_AGGREG)) {
      RASSERT(m_pCI->G_bEachMainWithSub(CK_AGGREG, CK_PROJECTION));
      m_pCI->G.p_stream = Card_Stream::pDerive_Of_projection((Card_Projection *) (m_pCI->G.p_projection));
      m_pCI->G.p_aggreg->vAppend(m_pCI->G.p_stream);
      RASSERT(m_pCI->G_bEachMainWithSub(CK_AGGREG, CK_PROJECTION));
      m_pCI->G.p_stream = Card_Stream::pDerive_Of_projection((Card_Projection *) (m_pCI->G.p_projection));
      m_pCI->G.p_aggreg->vAppend(m_pCI->G.p_stream);
      RASSERT(m_pCI->G_bEachMainWithSub(CK_AGGREG, CK_PROJECTION));
      m_pCI->G.p_stream = Card_Stream::pDerive_Of_projection((Card_Projection *) (m_pCI->G.p_projection));
      m_pCI->G.p_aggreg->vAppend(m_pCI->G.p_stream);
      break;
    }
    else {

    }
  }

  RASSERT(m_pCI->G_bEachMain(CK_FOLD));
  Board::vDerive_foldElist_From_project_stream(m_pCI);
  m_pCI->vPrintBoard(__LINE__);

  if (m_pCI->G_bEachMainWithSub(CK_GBYAGG,CK_KEY)) {
    i8 nBumpRight = m_pCI->G.p_fold->Y_iGet(P_BumpRight);
    m_pCI->G.p_key->vBump_stmOdn_In_rpnCalc(m_pCI);

    m_pCI->G.p_fold->Y_vCut(P_BumpRight);
    m_pCI->G.p_fold->Y_vSet(P_BumpedRight,(nBumpRight));
  }

  // find table for sqyarg
  char cTblSyn = Card::cTableColSyn(m_pCI->m_sSqyArg);
  m_pCI->G.p_table = m_pCI->G.pDeck->pFindTableCard(cTblSyn, m_pCI->G.p_MainCard);

  // TODO: derive cmpPred from marked tstPred and add to corresponding table

  m_pCI->G_vNewMember(CK_CMPPRED,"=="_Stz);
  m_pCI->G_vNewMember(CK_TERM,m_pCI->m_sSqyArg.szStz());
  m_pCI->G.p_cmpPred->vAppend(m_pCI->G.p_term);
  //m_pCI->G_vNewMember(CK_TERM,m_pCI->m_sSqyRsl.szStz());
  m_pCI->G_vNewMember(CK_TERM,"$1"_Stz);
  m_pCI->G.p_cmpPred->vAppend(m_pCI->G.p_term);

  m_pCI->G.p_table->vAddPredToTableFilter(m_pCI, m_pCI->G.p_cmpPred);
  m_pCI->vPrintBoard(__LINE__);


  // filter into bounds

  m_pCI->G.p_table->vBounds_DeriveFrom_filter(m_pCI->G.p_filter,false);
  m_pCI->vPrintBoard(__LINE__);

  m_pCI->G.p_boolCalc = m_pCI->G.p_filter->pSub_boolCalc();
  m_pCI->G.p_boolCalc->vCullPred(MARK_UP);
  m_pCI->vPrintBoard(__LINE__);

  Board::vS36_T30_Delete_Empty_filter(m_pCI);

  m_pCI->G_vRestart();
  if (m_pCI->G_bEachMainWithSub(CK_WHERE,CK_BOOLCALC)) {
    m_pCI->G.p_boolCalc->vCullPred(MARK_AT);
  }


  m_pCI->vPrintBoard(__LINE__);

  // delete potentially empty where
  m_pCI->G_vRestart();
  m_pCI->G_vDeleteWhenEmptySub(CK_WHERE,CK_BOOLCALC);

  m_pCI->vPrintBoard(__LINE__);


  Card_Stream::vStream_AdjustLoHi(m_pCI);
  m_pCI->vPrintBoard(__LINE__);

  Board::vRenumberStmOdn(m_pCI);
  m_pCI->vPrintBoard(__LINE__);

  Card_Projection::vRelabelExpressions(m_pCI);

  vIsFlat(true);
  m_pCI->G.pDeck->vClearMarksRecursive();

  m_pCI->vPrintBoard(__LINE__);


  m_pCI->vEndTransform();
  */

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vFold_50_W_L_C_E(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  // W L C E
  // push a (linear correlated exists) sub-query before the main-query
  // as a keyExists.  join ordering will be required to make it runnable
  // (refer: Q4)

  // assumptions
  // A1) sq is a single <table>
  // A2) will have index and bound [BVO: vSeries_42_Bound_Analysis]

  /* ARCHETYPE :
      select count(*)
        from orders
       where exists (select *
                       from lineitem
                      where L_ORDERKEY = O_ORDERKEY
                        and l_commitdate < l_receiptdate)
      ;
   */

  m_pCI->vTransform(__func__, "((20))"_Stz);
  vAssert_Board_mqA();

  // W L C E
  //             //
  //             //
  // T W A A A   // find W_L_C_E subquery call in mq:<where>/<boolCalc>
  //   f         //
  //   bc        //
  //  @ro exists //

  // 1) find W_L_C_E subquery call in mq:<where>/<boolCalc>
  if (m_pCI->G_bEachMainWithSub(CK_WHERE, CK_BOOLCALC)) {
    if (m_pCI->m_bRecursiveModel) {
      if (! Board::bFindAndMarkSubQryCall_recursiveModel(m_pCI)) {
        vTrace("DID NOT FIND W_A_C_ro SubQuery call\n"_Stz);
        NEVER_REACH
      }
    }
    else {
      if (! Board::bFindAndMarkSubQryCall_originalModel(m_pCI)) {
        vTrace("DID NOT FIND W_A_C_ro SubQuery call\n"_Stz);
        NEVER_REACH
      }
    }
  }
  vTrace("Found W_L_C_E call to %s(%s)\n"_Stz,m_pCI->m_sSqyNm.szStz(),m_pCI->m_sSqyArg.szStz());
  m_pCI->vPrintBoard(__LINE__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // 2) push to sq
  if (! m_pCI->G_bPushBoard(m_pCI->m_sSqyNm)) {
    vTrace("DID NOT FIND Board %s\n"_Stz,m_pCI->m_sSqyNm.szStz());
    NEVER_REACH
  }
  vAssert_Board_sqB();

  m_pCI->G_vRestart();
  if (m_pCI->G_bEachMainWithSub(CK_TABLE,CK_FILTER)) {
    m_pCI->G_vMorphCard(m_pCI->G.p_filter,CK_CONSTRAINT);
  }
  m_pCI->vPrintBoard(__LINE__);

  m_pCI->G_vRestart();
  if (m_pCI->G_bEachMainWith2Sub(CK_TABLE,CK_INDEX,CK_CONSTRAINT)) {
    m_pCI->G.p_table->vRemoveChild(m_pCI->G.p_constraint);
    m_pCI->G.p_index->vAppend(m_pCI->G.p_constraint);
  }
  m_pCI->vPrintBoard(__LINE__);

  // 3) get copy of sq:<table>/* into cc:<table>
  m_pCI->G_vRestart();
  if (m_pCI->G_bEachMain(CK_TABLE)) {
    m_pCI->G_vCopyCard(m_pCI->G.p_table);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // 4) pop back to mq
  m_pCI->G_vPopIter();
  vAssert_Board_mqA();

  m_pCI->G_vNewMember(CK_FOLD,"W_L_C_E"_Stz);
  m_pCI->G_vInsertBefore(m_pCI->G.p_fold,m_pCI->G.p_where);


  // W L C E
  //                 //
  //       E-sqr->   // Insert sq table before fold
  // --    ----mq--- //
  // T T F W A A A   // put cc:<table>/* before mq:<where>
  //   --sq          //

  m_pCI->G_vInsertBefore(m_pCI->m_pCard_copied,m_pCI->G.p_fold);
  m_pCI->G_vAppendNewChild(m_pCI->m_pCard_copied, CK_KEYEXISTS);
  m_pCI->vPrintBoard(__LINE__);

  // W L C E
  //                 // Replace %1 parameter with calling argument
  //                 //
  // T T F W A A A   // %1 -> $t.n
  //   b             //
  //   %1->$t.c      //
  //   ke            //

  Card_Term::bReplaceValueOfSub(m_pCI, m_pCI->m_pCard_copied,CK_TERM,"%1"_Stz,m_pCI->m_sSqyArg);
  m_pCI->G_vCopyCard(nullptr);
  m_pCI->vPrintBoard(__LINE__);

  // W L C E
  //                 // Cull @ro exists from <where>/<filter>
  //                 //
  // T T F W A A A   //
  //       f         //
  //       bc        //
  //      @ro exists //

  m_pCI->G_vRestart();
  if (m_pCI->G_bEachMainWithSub(CK_WHERE,CK_BOOLCALC)) {
    m_pCI->G.p_boolCalc->vCullPred(MARK_AT);
  }
  m_pCI->vPrintBoard(__LINE__);

  // W L C E
  //                 // Delete potentially empty where
  //                 //
  // T T F A A A     //
  //                 //

  m_pCI->G_vRestart();
  m_pCI->G_vDeleteWhenEmptySub(CK_WHERE,CK_BOOLCALC);
  m_pCI->vPrintBoard(__LINE__);

  vIsFlat(true);
  m_pCI->G.pDeck->vClearMarksRecursive();

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vFold_60_W_L_NC_NI(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  // W L NC NI
  // push a (linear non-correlated not in) sub-query before the main-query
  // as a keyExists
  // (refer: Q16)

  /* ARCHETYPE :
      select count(*)
        from partsupp, part
       where PS_PARTKEY = P_PARTKEY
         and PS_SUPPKEY not in (select S_SUPPKEY
                                  from supplier
                                 where s_comment like '%Customer%Complaints%')
      ;
 */

  m_pCI->vTransform(__func__, "((vFold_60_W_L_NC_NI))"_Stz);

  // 1) find W_L_NC_NI subquery call in mq:<where>/<boolCalc>
  //vAssert_Board_mqA();

  // W L NC NI
  //               //
  //               //
  // T T W A A A   // find W_L_NC_NI subquery call in mq:<where>/<boolCalc>
  //     f         //
  //     bc        //
  //    @ro notIn  //
  //        __sq2  // sub-query
  //           #1  // result
  //         $a.2  // argument


  if (m_pCI->G_bEachMainWithSub(CK_WHERE, CK_BOOLCALC)) {
    if (m_pCI->m_bRecursiveModel) {
      if (! Board::bFindAndMarkSubQryCall_recursiveModel(m_pCI)) {
        vTrace("DID NOT FIND W_A_C_ro SubQuery call\n"_Stz);
        NEVER_REACH
      }
    }
    else {
      if (! Board::bFindAndMarkSubQryCall_originalModel(m_pCI)) {
        vTrace("DID NOT FIND W_A_C_ro SubQuery call\n"_Stz);
        NEVER_REACH
      }
    }
  }
  vTrace(">>>>>>>>> Found W_L_NC_NI call to %s(%s)->%s\n"_Stz,m_pCI->m_sSqyNm.szStz(),m_pCI->m_sSqyArg.szStz(),m_pCI->m_sSqyRsl.szStz());
  m_pCI->vPrintBoard(__LINE__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // 2) push to sq
  if (! m_pCI->G_bPushBoard(m_pCI->m_sSqyNm)) {
    m_pCI->vTracer("DID NOT FIND Board %s\n"_Stz,m_pCI->m_sSqyNm.szStz());
    NEVER_REACH
  }
  vAssert_Board_sqB();

  // x) pull <where> into <table>/<filter>



  m_pCI->vPrintBoard(__LINE__);

  m_pCI->G_vRestart();
  if (m_pCI->G_bEachMain(CK_TABLE)) {
    if (m_pCI->G_bEachMain(CK_WHERE)) {
      Card_Where::vMoveWhereToTableAsConstraint(m_pCI);
    }
  }
  m_pCI->vPrintBoard(__LINE__);

  // 3) get copy of sq:<table>/* into cc:<table>
  m_pCI->G_vRestart();
  if (m_pCI->G_bEachMain(CK_TABLE)) {
    m_pCI->G_vCopyCard(m_pCI->G.p_table);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // 4) pop back to mq
  m_pCI->G_vPopIter();
  vAssert_Board_mqA();

  // 5) fold
  //vR_S40_T50a_Fold_W_L_C_E_Before_Mq_As_keyExists();
  // put cc:<table>/* before mq:<where>

  m_pCI->G_vNewMember(CK_FOLD,"W_L_NC_NI"_Stz);
  m_pCI->G_vInsertBefore(m_pCI->G.p_fold,m_pCI->G.p_where);

  // W L NC NI
  //                   //
  // ------  --mq----  //
  // T T T F W A A A   // find W_L_NC_NI subquery call in mq:<where>/<boolCalc>
  //     --sq          //
  //     kne           //

  m_pCI->G.p_table = _pCard_Table(m_pCI->m_pCard_copied);

  m_pCI->G_vInsertBefore(m_pCI->m_pCard_copied,m_pCI->G.p_fold);
  m_pCI->G_vAppendNewChild(m_pCI->m_pCard_copied, CK_KEYNOTEXISTS);

  Card_Projection::vProjection_Analysis_v2(m_pCI);
  m_pCI->G.pDeck->vRenumberRecursive();
//  Card_Stream::vStream_AdjustLoHi(m_pCI);
//  Board::vRenumberStmOdn(m_pCI);
  m_pCI->vPrintBoard(__LINE__);

  Stz* szSqyArg = m_pCI->m_sSqyArg;

  i8 i_Table = m_pCI->G.pDeck->iFindChild(m_pCI->G.p_table);
  bool b = m_pCI->G_bResolveSubCard(CK_EXPR,szSqyArg,CK_TBLCOLODN,i_Table);
  RASSERT(b);

  Stz* szTblColOdn = m_pCI->m_pCard_resolved->pSub_tblColOdn()->szValue();
  m_pCI->G.p_bounds = Card_Bounds::pBounds_Create_Eq_ForGivenCol(szTblColOdn);
  //m_pCI->m_pCard_copied->vAppend(m_pCI->G.p_bounds);

  m_pCI->G.p_indexList = m_pCI->G.p_table->pSub_indexList();
  m_pCI->G.p_index = m_pCI->G.p_table->pSub_index();
  m_pCI->G.p_index->vAppend(m_pCI->G.p_bounds);


  m_pCI->vPrintBoard(__LINE__);

  // W L NC NI
  //                   //
  // T T T F W A A A   //
  //     b             //
  //     kne           //

  m_pCI->G_vRestart();
  RASSERT(m_pCI->G_bEachMainWith2Sub(CK_TABLE,CK_KEYNOTEXISTS,CK_CONSTRAINT));
  m_pCI->G.p_table->vRemoveChild(m_pCI->G.p_constraint);
  m_pCI->G.p_indexList = m_pCI->G.p_table->pSub_indexList();
  m_pCI->G.p_index = m_pCI->G.p_table->pSub_index();
  m_pCI->G.p_index->vAppend(m_pCI->G.p_constraint);

  m_pCI->vPrintBoard(__LINE__);

  m_pCI->G_vCopyCard(nullptr);


  // W L NC NI
  //                 // Cull @ro notIn
  //                 //
  // T T T W A A A   //
  //       f         //
  //       bc        //
  //      @ro notIn  //
  //          __sq2  //
  //             #1  //
  //           $a.2  //

  m_pCI->G_vRestart();
  if (m_pCI->G_bEachMainWithSub(CK_WHERE,CK_BOOLCALC)) {
    m_pCI->G.p_boolCalc->vCullPred(MARK_AT);
  }

  m_pCI->vPrintBoard(__LINE__);


  // W L NC NI
  //               // Delete potentially empty where
  //               //
  // T T T A A A   //
  //               //
  //               //

  // delete potentially empty where
  m_pCI->G_vRestart();
  m_pCI->G_vDeleteWhenEmptySub(CK_WHERE,CK_BOOLCALC);

  m_pCI->vPrintBoard(__LINE__);

  vIsFlat(true);
  m_pCI->G.pDeck->vClearMarksRecursive();

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* Folding a non-correlated aggregate subquery (via having clause)            */
/*----------------------------------------------------------------------------*/
void Bench::vFold_70_H_A_NC_ro(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  // H A NC ro (refer: Q11)
  // 1) the subquery produces a single result, such as avg(col)
  // 2) that SQ result needs to be in every MQ row so the Having clause
  //    can use it to filter rows, so:
  //   a) cross join SQ result with every row of first table of MQ
  //   b) inject SQ result into key & projection of MQ GbyAgg
  //   c) correct the Having op to cater for SQ result
  //   d) Un-project the SQ result from final rows

  /* ARCHETYPE _r_a_b_HAVING:
      select l_orderkey, sum(l_quantity)
        from lineitem
       group by l_orderkey
      having sum(l_quantity) > (select avg(l_quantity)
                                  from lineitem);
  */

  m_pCI->vTransform(__func__, "((vFold_70_H_A_NC_ro))"_Stz);

  vAssert_Board_mqA();

  // 1) find H_A_NC_ro subquery call in mq:<having>/<boolCalc>/<roPred>
  if (m_pCI->G_bEachMainWithSub(CK_HAVING, CK_BOOLCALC)) {
    if (m_pCI->m_bRecursiveModel) {
      if (! Board::bFindAndMarkSubQryCall_recursiveModel(m_pCI)) {
        vTrace("DID NOT FIND W_A_C_ro SubQuery call\n"_Stz);
        NEVER_REACH
      }
    }
    else {
      if (! Board::bFindAndMarkSubQryCall_originalModel(m_pCI)) {
        vTrace("DID NOT FIND W_A_C_ro SubQuery call\n"_Stz);
        NEVER_REACH
      }
    }
  }
  vTrace(">>>>>>>>> Found H_A_NC_ro call:  %s %s %s\n"_Stz,m_pCI->m_sSqyArg.szStz(),m_pCI->m_sSqyRO.szStz(),m_pCI->m_sSqyNm.szStz());
  m_pCI->vPrintBoard(__LINE__);

  // 2) push to sq
  if (! m_pCI->G_bPushBoard(m_pCI->m_sSqyNm)) {
    m_pCI->vTracer("DID NOT FIND Board %s\n"_Stz,m_pCI->m_sSqyNm.szStz());
    NEVER_REACH
  }
  vAssert_Board_sqB();

  // 3) get copy of sq
  m_pCI->m_pCard_yanked = m_pCI->G.pDeck->pDupSubCards(0);

  // 4) pop back to mq
  m_pCI->G_vPopIter();
  vAssert_Board_mqA();

  // 5) add <fold> (before MQ)
  m_pCI->G_vNewMember(CK_FOLD,"H_A_NC_ro"_Stz);
  m_pCI->G.pDeck->vPush(m_pCI->G.p_fold);

  // 6) copy up sq (add before fold)
  m_pCI->G.pDeck->vPush(m_pCI->m_pCard_yanked);
  m_pCI->G.pDeck->vReplaceChildWithItsSubs(m_pCI->m_pCard_yanked);
  DELETE_CARD(m_pCI->m_pCard_yanked);

  m_pCI->vPrintBoard(__LINE__);

  // 7) derive the fold (SQ result) expr
  m_pCI->G_vRestart();
  if (m_pCI->G_bEachMain(CK_FOLD)) {
    m_pCI->G.i_FoldCard = m_pCI->G.i_MainCard;
    Board::vDerive_foldExpr_H_A_NC_ro(m_pCI);
  }
  m_pCI->vPrintBoard(__LINE__);

  if (m_pCI->G_bEachMain(CK_WHERE)) {
    m_pCI->G.p_where->vBump_Rightwards(1);
  }

  // 8) inject SQ result into MQ <gbyAgg>
  Board::vInject_sqResult_Into_gbyAgg(m_pCI);
  m_pCI->G.pDeck->vRenumberRecursive();
  m_pCI->vPrintBoard(__LINE__);

  m_pCI->G_vRestart();
  if (m_pCI->G_bEachMain(CK_HAVING)) {

    // 9) correct MQ <having> after inject
    Board::vCorrect_having_After_Inject(m_pCI);
    m_pCI->vPrintBoard(__LINE__);

    // 10) un-project injected SQ result
    Board::vUnproject_injected_sqResult(m_pCI);
    Card_Projection::vProjection_Analysis_v2(m_pCI);
  }

  m_pCI->vPrintBoard(__LINE__);

  vIsFlat(true);
  m_pCI->G.pDeck->vClearMarksRecursive();

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* Folding a non-correlated aggregate subquery (via having clause)            */
/*----------------------------------------------------------------------------*/
void Bench::vFold_70_H_A_NC_ro_smoothing(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  // H A NC ro (refer: Q11)
  // 1) the subquery produces a single result, such as avg(col)
  // 2) that SQ result needs to be in every MQ row so the Having clause
  //    can use it to filter rows, so:
  //   a) cross join SQ result with every row of first table of MQ
  //   b) inject SQ result into key & projection of MQ GbyAgg
  //   c) correct the Having op to cater for SQ result
  //   d) Un-project the SQ result from final rows

  /* ARCHETYPE _r_a_b_HAVING:
      select l_orderkey, sum(l_quantity)
        from lineitem
       group by l_orderkey
      having sum(l_quantity) > (select avg(l_quantity)
                                  from lineitem);
  */

  m_pCI->vTransform(__func__, "((vFold_70_H_A_NC_ro))"_Stz);

  vAssert_Board_mqA();

  // 1) find H_A_NC_ro subquery call in mq:<having>/<boolCalc>/<roPred>
  if (m_pCI->G_bEachMainWithSub(CK_HAVING, CK_BOOLCALC)) {
    if (m_pCI->m_bRecursiveModel) {
      if (! Board::bFindAndMarkSubQryCall_recursiveModel(m_pCI)) {
        vTrace("DID NOT FIND W_A_C_ro SubQuery call\n"_Stz);
        NEVER_REACH
      }
    }
    else {
      if (! Board::bFindAndMarkSubQryCall_originalModel(m_pCI)) {
        vTrace("DID NOT FIND W_A_C_ro SubQuery call\n"_Stz);
        NEVER_REACH
      }
    }
  }
  vTrace(">>>>>>>>> Found H_A_NC_ro call:  %s %s %s\n"_Stz,m_pCI->m_sSqyArg.szStz(),m_pCI->m_sSqyRO.szStz(),m_pCI->m_sSqyNm.szStz());
  m_pCI->vPrintBoard(__LINE__);

  // 2) push to sq
  if (! m_pCI->G_bPushBoard(m_pCI->m_sSqyNm)) {
    m_pCI->vTracer("DID NOT FIND Board %s\n"_Stz,m_pCI->m_sSqyNm.szStz());
    NEVER_REACH
  }
  vAssert_Board_sqB();

  // 3) get copy of sq
  m_pCI->m_pCard_yanked = m_pCI->G.pDeck->pDupSubCards(0);

  // 4) pop back to mq
  m_pCI->G_vPopIter();
  vAssert_Board_mqA();

  // 5) add <fold> (before MQ)
  m_pCI->G_vNewMember(CK_FOLD,"H_A_NC_ro"_Stz);
  m_pCI->G.pDeck->vPush(m_pCI->G.p_fold);

  // 6) copy up sq (add before fold)
  m_pCI->G.pDeck->vPush(m_pCI->m_pCard_yanked);
  m_pCI->G.pDeck->vReplaceChildWithItsSubs(m_pCI->m_pCard_yanked);
  DELETE_CARD(m_pCI->m_pCard_yanked);

  m_pCI->vPrintBoard(__LINE__);

  // 7) derive the fold (SQ result) expr
  m_pCI->G_vRestart();
  if (m_pCI->G_bEachMain(CK_FOLD)) {
    m_pCI->G.i_FoldCard = m_pCI->G.i_MainCard;
    Board::vDerive_foldExpr_H_A_NC_ro(m_pCI);
  }
  m_pCI->vPrintBoard(__LINE__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // Table* [Where] GbyAgg_PNO [Project (avg)] Having

  m_pCI->G.p_projection = nullptr;


  // Table*
  while (m_pCI->G_bIsNextMain(CK_TABLE) && m_pCI->G_bEachMain(CK_TABLE)) {
    m_pCI->G.p_table->vReProjection(m_pCI);
  }

  // [Where]
  if (m_pCI->G_bIsNextMain(CK_WHERE) && m_pCI->G_bEachMain(CK_WHERE)) {
    m_pCI->G.p_where->vBump_Rightwards(1);
  }

  // GbyAgg_partition
  if (m_pCI->G_bIsNextMain(CK_GBYAGG) && m_pCI->G_bEachMain(CK_GBYAGG)) {
  }

  // GbyAgg_node
  if (m_pCI->G_bIsNextMain(CK_GBYAGG) && m_pCI->G_bEachMain(CK_GBYAGG)) {
  }

  // GbyAgg_origin
  if (m_pCI->G_bIsNextMain(CK_GBYAGG) && m_pCI->G_bEachMain(CK_GBYAGG)) {
  }

  // [project (avg)]
  if (m_pCI->G_bIsNextMain(CK_PROJECT) && m_pCI->G_bEachMain(CK_PROJECT)) {
  }

  // Having
  if (m_pCI->G_bIsNextMain(CK_GBYAGG) && m_pCI->G_bEachMain(CK_GBYAGG)) {
  }







  if (m_pCI->G_bEachMain(CK_WHERE)) {
    m_pCI->G.p_where->vBump_Rightwards(1);
  }

  // 8) inject SQ result into MQ <gbyAgg>
  Board::vInject_sqResult_Into_gbyAgg(m_pCI);
  m_pCI->G.pDeck->vRenumberRecursive();
  m_pCI->vPrintBoard(__LINE__);

  m_pCI->G_vRestart();
  if (m_pCI->G_bEachMain(CK_HAVING)) {

    // 9) correct MQ <having> after inject
    Board::vCorrect_having_After_Inject(m_pCI);
    m_pCI->vPrintBoard(__LINE__);

    // 10) un-project injected SQ result
    Board::vUnproject_injected_sqResult(m_pCI);
    Card_Projection::vProjection_Analysis_v2(m_pCI);
  }
  m_pCI->vPrintBoard(__LINE__);



  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


  vIsFlat(true);
  m_pCI->G.pDeck->vClearMarksRecursive();

  m_pCI->vEndTransform();
}



/*----------------------------------------------------------------------------*/
/*-- Optimise  ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vOptimiseLoop(void) {

  bool bOpts;
  i8 iOpts = 0;

  //vTrace("%s\n"_Stz,  pOptimiser()->szWeave());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Discover and perform the optimisation transformations

  do {

    bOpts = false;

    bOpts |= bOptimise_CutSort();
    bOpts |= bOptimise_DupFilter();
    bOpts |= bOptimise_ElyFilters();
    bOpts |= bOptimise_ElyProjections();
    bOpts |= bOptimise_MinGroupby();
    bOpts |= bOptimise_OtmCalculations();
    bOpts |= bOptimise_PpnProjections();
    // todo: disuss with Alka
//    bOpts |= bOptimise_RplAggregByOrdAgg();
    bOpts |= bOptimise_RplAggregBySortOrdAgg();
    bOpts |= bOptimise_RplMaxByTop();
    bOpts |= bOptimise_RplScanByIndexScan();
    bOpts |= bOptimise_RplSortDescLimitByTop();
    bOpts |= bOptimise_RplSortLimitOnOriginByAllLevels();
    bOpts |= bOptimise_SeqJoinOrder();

  } while (bOpts);

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Bench::bOptimise_CutSort(void) {

  //
  // Query1, Query4, Query7, Query8, Query12
  //

  // Notes:
  // sbyElist matches the gbyElist of an earlier (on all partitions) stage
  //   including the direction

  m_pCI->G_vRestart();
  if (m_pCI->G_bEachMain(CK_SORT)) {
    Card_Order* p_order1 = m_pCI->G.p_sort->pSub_stream()->pSub_order();
    if (! p_order1)
      return false;

    if (m_pCI->G_bResolveSubCard(CK_STREAM,m_pCI->G.i_MainCard)) {
      Card_Order *p_order2 = m_pCI->G.p_stream->pSub_order();
      if (! p_order2)
        return false;

      // are the two order's the same?
      if (p_order1->bValuesSameRecursive(p_order2)) {

        m_pCI->G_vRemoveDeleteMain(CK_SORT);
        return true;
      }

    }
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Bench::bOptimise_DupFilter(void) {

  // Query7, Query19

  // Notes:
  // If the query has filters with an OR we need to pick all the values for a column
  // from all the OR filters and copy them close to the table scan/indesxScan/indexJoin
  // of the concerned table.

  // Also Note: These are filters that are not being moved but extracts being
  // copied to the tables scan/indesxScan/indexJoin.

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Bench::bOptimise_ElyFilters(void) {

  // Query2, Query3, Query4, Query5, Query7, Query8,
  //   Query9, Query10, Query11, Query12, Query16, Query17 Query19

  // Notes:
  // get columns from cmpPred's and find the tables they come from in previous
  // scan/indexScan/indexJoin cards and move them after the scan/indexJoin/indexJoin
  // of the tables

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Bench::bOptimise_ElyProjections(void) {

  // Query2, Query3, Query4, Query5, Query7, Query8,
  //   Query9, Query10, Query11, Query12, Query14,
  //      Query16, Query17, Query18, Query19

  // Notes:
  // get all $ columns from all operators -
  // aggreg, filter, Hash, IndexJoin, keyExists, keyNotExists, OrdAgg, project, Sort, Top
  // (leaving them as is unless its a project being modified)
  // and find the tables they come from in previous scan/indexScan/indexJoin
  // cards and add a project (or add to the project if already there)
  // after the scan/indexJoin/indexJoin of the tables
  // (ensuring no duplicate projections)

  // idea being to get all pprojections realted to a table closest to
  // its scan/indexScan/indexJoin

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Bench::bOptimise_MinGroupby(void) {

  //
  // Query3, Query10, Query18
  //

  // Notes:
  // * need to get the index if the groupby colum is coming from an indexscan
  // * and check if index is a unique index on that column
  // * if a match is found , discard all other columns off the gbyElist
  //   except the match (unique one) and need to rejoin with the same table
  //   to get the columns if they were being projected


  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Bench::bOptimise_OtmCalculations(void) {

  // Query1, Query4, Query5, Query6, Query10, Query12, Query14, Query15,
  //   Query19, Query20

  // Notes:
  // To figure out if their are any literal calculations that can be done upfront.
  // instead of executing the calculation for every row that is being processed.
  // So need to decipher any arithmetic OP (+-*/) with 2 literals.
  // eg 0.06 0.01 -
  // date '1993-01-01' - interval '1' year

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Bench::bOptimise_PpnProjections(void) {

  // Query2, Query10

  // Notes:
  // If a query has an aggregate and either :
  //   the group by columns don't include the columns in the select list
  //   or due to an optimisation OT_MIN_GROUPBY the columns are reduced from
  //     the group by list
  // then we need to rejoin with the particular table/tables and get the required
  // selected columns

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Bench::bOptimise_RplAggregByOrdAgg(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  // Query13, Query17 : do we change the scan to indexScan ????
  // Query18

  // Notes:
  // When we have an aggreg grouped on a column on which their is an index available,
  // we can change the scan to an indexScan (hence data will come in ascending order
  // of the group by key)
  // and then we can change the aggreg to an ordAgm_pCI->G.
  //
  // Also Note: when the ordAgg replaces the aggreg at an all partitions level,
  //  we can delete the aggregates on all nodes and on origin.
  //  This further aggregation is not required.

  m_pCI->G.pDeck->vRenumberRecursive();

  m_pCI->vPrintBoard(__LINE__);

  m_pCI->G_vRestart();
  if (m_pCI->G_bEachMain(CK_GBYAGG)) {
    Card_Order* p_order1 = m_pCI->G.p_gbyAgg->pSub_stream()->pSub_order();

    if (m_pCI->G_bResolveSubCard(CK_STREAM,m_pCI->G.i_MainCard)) {
      Card_Order *p_order2 = m_pCI->G.p_stream->pSub_order();
      if (! p_order2)
        return false;

      p_order1->vDump(0,"ORDER1"_Stz);
      p_order2->vDump(0,"ORDER2"_Stz);

      // are the two order's the same?
      if (p_order1->bValuesSameRecursive(p_order2)) {
        m_pCI->G.p_gbyAgg->vCard(CK_ORDAGG);

        m_pCI->vPrintBoard(__LINE__);

        RASSERT(m_pCI->G_bEachMain(CK_GBYAGG));
        m_pCI->G_vRemoveDeleteMain(CK_GBYAGG);

        m_pCI->vPrintBoard(__LINE__);

        RASSERT(m_pCI->G_bEachMain(CK_GBYAGG));
        m_pCI->G_vRemoveDeleteMain(CK_GBYAGG);

        return true;
      }
    }
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Bench::bOptimise_RplAggregBySortOrdAgg(void) {

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Bench::bOptimise_RplMaxByTop(void) {

  // Query15

  // Notes:
  // If a projEList has only one item and it is a max,
  //  then the projectOP can be replace by a TOP of the max column
  //  with 1 as the parameter to TOP
  // e.g
  //   project " type DECIMAL ) $1 max "
  // can be replace by :
  //   top "TOP type DECIMAL $2" 1

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Bench::bOptimise_RplScanByIndexScan(void) {

  // Query6, Query10, Query14 and Query15

  // Notes:
  //
  // If there is a Scan followed by a filter and the filter is on a single column
  // with a lower/upper/lower+upper/equal bound then we can replace
  // the scan and part / all of the filter with an indexScan

  // Points to note:
  // ** Some queries (eg Query1) we don't change the scan to an indexscan
  //    because the selection range of the filter covers almost 95% of the rows.
  //    In such a case a scan is better.
  //
  // ** This optimisation can be applied only after OT_ELY_FILTERS.
  //    We won't see it till the Filters are moved close to the scans.

//  m_pCI->G_vRestart();
//  if (m_pCI->G_bEachMainWithSub(CK_SCAN,CK_BOUNDS)) {
//    if (m_pCI->G.p_bounds->pSub_lBound() || m_pCI->G.p_bounds->pSub_uBound()) {
//      m_pCI->G.p_scan->vCard(CK_INDEXSCAN);
//
//      m_pCI->vPrintBoard(__LINE__);
//
//      vS90_T30_Derive_order();
//
//      m_pCI->vPrintBoard(__LINE__);
//
//      return true;
//    }
//  }

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Bench::bOptimise_RplSortDescLimitByTop(void) {

  // Query10

  // Notes:
  // sbyElist expressions has a single column and its direction is descending
  // and it is followed by a limit we can replace the sort+limit by TOP.
  // TOP will have the EList from sbyEList and the number (top limit) from limit.

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Bench::bOptimise_RplSortLimitOnOriginByAllLevels(void) {

  // Query3

  // Notes:
  // If an aggreg is replaced by an OrdAgg, we know that we don't need the
  // ordAgg at all nodes and origin. It can be only at the partition level.
  // You don't need to aggregate all nodes etc.
  // Because we know that the ordered data being aggregated at all partitions
  // is complete we can replace a sort on origin that follows the aggreg
  // by a sort and limit on all partitions/ on all nodes and on origin.
  //
  // This ways reduced that data to be sorted at each level and reducing the rows
  // moving around as we have applied the limit as well

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Bench::bOptimise_SeqJoinOrder(void) {

  return false;
}









/*----------------------------------------------------------------------------*/
/*-- other -------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/






/*----------------------------------------------------------------------------*/
/* add a new board                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vAddBoard(Board* pBoard)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  //RASSERT(! pGetBoard(pBoard->eBoardKind()));

  vPush(pBoard);
}

/*----------------------------------------------------------------------------*/
/* does given board exist?                                                    */
/*----------------------------------------------------------------------------*/
bool Bench::bBoardExists(eBK eBoard, i8 i_SubBoard)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  if (pGetBoard(eBoard,i_SubBoard))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* get number of board                                                        */
/*----------------------------------------------------------------------------*/
i8 Bench::nBoards(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  i8 nBoards = nItems();

  return nBoards;
}

/*----------------------------------------------------------------------------*/
/* get card registers                                                         */
/*----------------------------------------------------------------------------*/
CardCtl* Bench::pCardCtl(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  return m_pOptimiser->pCardCtl();
}

/*----------------------------------------------------------------------------*/
/* clear marks                                                                */
/*----------------------------------------------------------------------------*/
void Bench::vClearMarks(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  i8 nBoards = this->nBoards();
  for (i8 i_Board = 0; i_Board < nBoards; i_Board++) {
    Board *pBoard = this->pGet(i_Board);

    pBoard->vClearMarks();
  }
}



/*----------------------------------------------------------------------------*/
/* get board of given kind                                                    */
/*----------------------------------------------------------------------------*/
Board* Bench::pGetBoard(eBK eBoard, i8 i_Board)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  i8 nBoards = this->nBoards();
  for (i8 i_each = 0; i_each < nBoards; i_each++) {
    Board *pBoard = this->pGet(i_each);

    if (eBoard == pBoard->eBoardKind() && (C_UNKNOWN == i_Board ||
                                       i_Board == pBoard->i_Board()))
      return pBoard;
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get board                                                                  */
/*----------------------------------------------------------------------------*/
Board* Bench::pGetBoard(i8 i_Board)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  Board *pBoard = this->pGet(i_Board);

  return pBoard;
}

/*----------------------------------------------------------------------------*/
/* get board of given name                                                    */
/*----------------------------------------------------------------------------*/
Board* Bench::pGetBoard(Stz* szBoardNm)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  i8 nBoards = this->nBoards();
  for (i8 i_Board = 0; i_Board < nBoards; i_Board++) {
    Board *pBoard = this->pGet(i_Board);

    if (pBoard->szBoardNm()->bSame(szBoardNm))
      return pBoard;
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* is the bench flat?                                                         */
/*----------------------------------------------------------------------------*/
bool Bench::bIsFlat(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  return m_bIsFlat;
}

/*----------------------------------------------------------------------------*/
/* set is the bench flat?                                                     */
/*----------------------------------------------------------------------------*/
void Bench::vIsFlat(bool bIsFlat)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  m_bIsFlat = bIsFlat;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Optimiser* Bench::pOptimiser(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  return m_pOptimiser;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
char Bench::cPrefix(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  return m_cPrefix;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vPrefix(char cPrefix)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  m_cPrefix = cPrefix;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vEndSeries(eBK eBoard, i8 i_Board)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  RASSERT(0 == m_pCI->iTransformDepth());

  //m_pTransforms->vXform(C_NA);
  //m_pCardCtl->vUseXhand();

  m_pCI->vPrintBench(eBoard, i_Board);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vSeries(eBK eBoard, i8 i_Board, const char* azHand)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  m_pCI->vUsingBoard(eBoard, i_Board);
  m_pCI->vXhand(azHand);

  vClearMarks();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                   vSeries_10_Make_Runnable */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vS10_T00_Begin(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  vST00_Begin();

  vPrefix('r');
  m_pCI->m_pCardCtl->vUseExpanded();
  m_pCI->vPrintBench(BK_SUBPIPE);
  m_pCI->m_pCardCtl->vUseExpanded();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                               vSeries_26_SubQuery_Analysis */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vS26_T10_SubQuery_Analysis_Of_Main_Query(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  m_pCI->vTransform(__func__, "((10))"_Stz);

  vAssert_Board_mqA();

  while (m_pCI->G_bEachMain()) {
    m_pCI->G_vNewMember(CK_GENESIS,"mq"_Stz);
    if ((m_pCI->G.p_level = m_pCI->G.p_MainCard->pSub_level()))
      m_pCI->G.p_MainCard->vInsertAfter(m_pCI->G.p_genesis,m_pCI->G.p_level);
    else
      m_pCI->G.p_MainCard->vPush(m_pCI->G.p_genesis);
  }

  m_pCI->G_vRestart();
  while (m_pCI->G_bEachMain(CK_AGGREG)) {
    // the main-query has an aggreg

    SubQuery* pMainQuery =  pGetBoard((i8)0)->pSubQuery();

    pMainQuery->vDisposition(QD_AGGREGATION);
  }

  m_pCI->G_vRestart();
  while (m_pCI->G_bEachMain(CK_GBYAGG)) {
    SubQuery* pMainQuery =  pGetBoard((i8)0)->pSubQuery();

    pMainQuery->vDisposition(QD_AGGREGATION);
  }

  i8 nBoards = this->nBoards();
  if (nBoards > 1) {
    m_pCI->G_vRestart();
    while (m_pCI->G_bEachMain(CK_CALL)) {
      SubQuery* pSubQuery =  pGetBoard((i8)1)->pSubQuery();

      pSubQuery->vCaller(0);

      pSubQuery->vClause(QC_FROM_CLAUSE);

      SubQuery* pMainQuery =  pGetBoard((i8)0)->pSubQuery();
      pMainQuery->vSubQueries(1);
    }
    m_pCI->G_vRestart();
    while (m_pCI->G_bEachMainWithSub(CK_WHERE, CK_RPNCALC)) {
      Board::vS26_T10d_Main_Query_Analysis_Of_where_rpnCalc(m_pCI);
    }
    m_pCI->G_vRestart();
    while (m_pCI->G_bEachMainWithSub(CK_HAVING, CK_RPNCALC)) {
      Board::vS26_T10e_Main_Query_Analysis_Of_having_rpnCalc(m_pCI);
    }
  }

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vS26_T20_SubQuery_Analysis_Of_Sub_Query(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  m_pCI->vTransform(__func__, "((20))"_Stz);

  vAssert_Board_mqA();


  i8 nBoards = this->nBoards();
  if (1 == nBoards) {
    // there is no sub-query

    SubQuery* pMainQuery =  pGetBoard((i8)0)->pSubQuery();

    pMainQuery->vSubQueries(0);
  }
  else
  if (2 == nBoards) {

    if (m_pCI->G_bPushBoard(1)) {

      SubQuery* pSubQuery =  pGetBoard(1)->pSubQuery();
      pSubQuery->vDisposition(QD_LINEAR);

      while (m_pCI->G_bEachMain()) {
        m_pCI->G_vNewMember(CK_GENESIS,"sq"_Stz);
        if ((m_pCI->G.p_level = m_pCI->G.p_MainCard->pSub_level()))
          m_pCI->G.p_MainCard->vInsertAfter(m_pCI->G.p_genesis,m_pCI->G.p_level);
        else
          m_pCI->G.p_MainCard->vPush(m_pCI->G.p_genesis);
      }

      m_pCI->G_vRestart();
      while (m_pCI->G_bEachMain(CK_PARAMLIST)) {
        // there is a subQuery and it has a paramList

        SubQuery* pSubQuery =  pGetBoard(1)->pSubQuery();

        // TODO: hack the number of params... fix
        i8 nParams = 1; //m_pCI->G.p_paramList->nChildCards();

        pSubQuery->vCorrCols(nParams);
      }

      m_pCI->G_vRestart();
      while (m_pCI->G_bEachMain(CK_AGGREG)) {
        // there is a subQuery and it has an aggreg

        SubQuery* pSubQuery =  pGetBoard(1)->pSubQuery();

        pSubQuery->vDisposition(QD_AGGREGATION);
      }

      m_pCI->G_vRestart();
      while (m_pCI->G_bEachMain(CK_GBYAGG)) {
        // there is a subQuery and it has an gbyAgg
        SubQuery* pSubQuery =  pGetBoard(1)->pSubQuery();
        pSubQuery->vDisposition(QD_AGGREGATION);
      }

      m_pCI->G_vPopIter();
    }
  }
  else
    NEVER_REACH;

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                     vSeries_32_To_BoolCalc */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/* SubQuery Analysis                                                          */
/*----------------------------------------------------------------------------*/
void Bench::vS32_T_SubQuery_Analysis(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  m_pCI->vTransform(__func__, "rpcCalc of where becomes boolCalc "_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Convert SqyOp Not To NotSqyOp

  while (m_pCI->G_bEachMainWithSub(CK_WHERE, CK_RPNCALC)) {
    m_pCI->G.p_rpnCalc->vConvert_sqyOp_not_To_notSqyOp();
  }

  m_pCI->m_pCardCtl->vPrintSubCards(true,CK_RPNCALC,0);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Convert RpnCalc To Bool Calc

  m_pCI->G_vRestart();
  while (m_pCI->G_bEachMainWithSub(CK_WHERE, CK_RPNCALC)) {
    m_pCI->G.p_boolCalc = m_pCI->G.p_rpnCalc->pToBoolCalc();

    m_pCI->G.p_where->vReplaceSubCard(m_pCI->G.p_rpnCalc,m_pCI->G.p_boolCalc);

    m_pCI->G_vDeleteMember(m_pCI->G.p_rpnCalc);
  }

  m_pCI->G_vRestart();
  while (m_pCI->G_bEachMainWithSub(CK_HAVING, CK_RPNCALC)) {
    m_pCI->G.p_boolCalc = m_pCI->G.p_rpnCalc->pToBoolCalc();

    m_pCI->G.p_having->vReplaceSubCard(m_pCI->G.p_rpnCalc,m_pCI->G.p_boolCalc);

    m_pCI->G_vDeleteMember(m_pCI->G.p_rpnCalc);
  }

  m_pCI->m_pCardCtl->vPrintSubCards(true,CK_RPNCALC,0);

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                   vSeries_50_Join_Ordering */
/*============================================================================*/


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vTransform_JoinOrdering_originalModel(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  m_pCI->vTransform(__func__, "((replace with an explanation))"_Stz);

  Card_Deck* pPipeline = m_pCI->G.pDeck;

  Board* pBoard = new Board(aBK[BK_PARTITION],
                            BK_PARTITION,
                            C_UNKNOWN,
                            BT_ORIGINAL,
                            m_pCI->m_pBench);
  m_pCI->G_vPushBoard(pBoard);

  // 1) Extract tableList from pipeline
  m_pCI->G.p_tableList = Card_TableList::pExtract(pPipeline);
  m_pCI->G.pDeck->vAppend(m_pCI->G.p_tableList);
  m_pCI->G.pDeck->vRenumberRecursive();
  m_pCI->vPrintBoard(__LINE__);

  // 2) Extract joinPredList from pipeline
  m_pCI->G.p_joinPredList = Card_JoinPredList::pExtract(pPipeline);
  m_pCI->G.pDeck->vAppend(m_pCI->G.p_joinPredList);
  m_pCI->G.pDeck->vRenumberRecursive();
  m_pCI->vPrintBoard(__LINE__);

  // 3) Compute all possible join permutations
  Card_JoinPermAll* p_joinPermAll =
      Card_JoinPermAll::pAllPermutedJoins(m_pCI->G.p_tableList, m_pCI->G.p_joinPredList);

  m_pCI->G.pDeck->vAppend(new Card_Section("All Permutations"_Stz));
  CardIter xCI(p_joinPermAll);
  while (xCI.G_bEachMain())
    m_pCI->G.pDeck->vAppend(xCI.G.p_joinPermSeq->pDupCardAndSubCards());
  m_pCI->G.pDeck->vRenumberRecursive();
  m_pCI->vPrintBoard(__LINE__);

  // 4) Remove invalid permutations
  p_joinPermAll->vRemoveInvalid();
  m_pCI->G.pDeck->vAppend(new Card_Section("Valid Permutations"_Stz));

  xCI.vRestart();
  while (xCI.G_bEachMain())
    m_pCI->G.pDeck->vAppend(xCI.G.p_joinPermSeq->pDupCardAndSubCards());
  m_pCI->G.pDeck->vRenumberRecursive();
  m_pCI->vPrintBoard(__LINE__);

  // 5) Calculate join load
  p_joinPermAll->vCalculateLoad(m_pCI->G.p_tableList);
  m_pCI->G.pDeck->vAppend(new Card_Section("Join Loads"_Stz));
  xCI.vRestart();
  while (xCI.G_bEachMain())
    m_pCI->G.pDeck->vAppend(xCI.G.p_joinPermSeq->pDupCardAndSubCards());
  m_pCI->G.pDeck->vRenumberRecursive();
  m_pCI->vPrintBoard(__LINE__);

  // get the join permutations strings
  xCI.vRestart();
  i8 iJoinPermutations = 0;
  while (xCI.G_bEachMain(CK_JOINPERMSEQ)) {
    Card_JoinOrdering* p_joinOrdering = Card_JoinOrdering::pDerive(xCI.G.p_joinPermSeq);
    s256 sJoinOrder;
    p_joinOrdering->vJoinOrderString(sJoinOrder,m_pCI->G.p_tableList);
    //vTrace("JoinOrder %lld :%s\n"_Stz, iJoinPermutations, sJoinOrder.szStz());
    m_pOptimiser->vJoinPermutation(iJoinPermutations, sJoinOrder);
    iJoinPermutations++;
    DELETE_CARD(p_joinOrdering);
  }
  m_pOptimiser->vJoinPermutations(iJoinPermutations);


  // 6) Pick final join ordering
  Card_JoinOrdering* p_joinOrdering =
      Card_JoinOrdering::pPick(p_joinPermAll);
  m_pCI->G.pDeck->vAppend(new Card_Section("Picked Ordering"_Stz));
  m_pCI->G.pDeck->vAppend(p_joinOrdering->pDupCardAndSubCards());
  m_pCI->G.pDeck->vRenumberRecursive();
  m_pCI->vPrintBoard(__LINE__);

  // get the final join ordering string
  s256 sJoinOrder;
  p_joinOrdering->vJoinOrderString(sJoinOrder,m_pCI->G.p_tableList);
  //vTrace("JoinOrder: %s\n"_Stz,sJoinOrder.szStz());
  m_pOptimiser->vJoinOrder(sJoinOrder);

  DELETE_CARD(p_joinPermAll);

  pBoard = m_pCI->G.pBoard;
  m_pCI->G_vPopIter();
  vRemove(pBoard);
  delete pBoard;

  p_joinOrdering->vReorder(m_pCI->G.pDeck);
  m_pCI->vPrintBoard(__LINE__);

  DELETE_CARD(p_joinOrdering);

  Card_Projection::vProjection_Analysis_v2(m_pCI);

  m_pCI->vPrintBoard(__LINE__);

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* Predicate Pullup                                                           */
/*----------------------------------------------------------------------------*/
void Bench::vTransform_PredicateReposition_originalModel(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  m_pCI->vTransform(__func__, "put a table @c against where pred"_Stz);

  vAssert_Board_any();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // At Mark Where Pred Wrt Table

  m_pCI->vPrintBoard(__LINE__);

  while (m_pCI->G_bEachMainWithSub(CK_TABLE, CK_BOOLCALC)) {
    m_pCI->G.p_boolCalc->vMarkWrt_table(m_pCI->G.pDeck);
  }

  m_pCI->vPrintBoard(__LINE__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Move At Marked Where Pred To Table Filter

  m_pCI->G_vRestart();

  while (m_pCI->G_bEachMainWithSub(CK_TABLE, CK_BOOLCALC)) {
    m_pCI->G.p_boolCalc->vCopyMarkedPredToTable(m_pCI->G.pDeck);
  }

  m_pCI->vPrintBoard();

  m_pCI->G_vRestart();

  while (m_pCI->G_bEachMainWithSub(CK_TABLE, CK_BOOLCALC)) {
    m_pCI->G.p_boolCalc->vCullPred(MARK_AT);
  }

  m_pCI->vPrintBoard();


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Delete Empty Where

  m_pCI->G_vRestart();

  while (m_pCI->G_bEachMainWithSub(CK_TABLE, CK_FILTER)) {
    m_pCI->G.p_table->vRemove_filter_IfEmpty(m_pCI->G.p_filter);
  }

  m_pCI->vPrintBoard();

  m_pCI->vEndTransform();
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                    vSeries_75_EE_operators */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vS75_T20_Morph_keyExists_filter_To_constraint(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  m_pCI->vTransform(__func__, "convert all <table> into either <scan> or <indexJoin>"_Stz);

  vAssert_Board_mqA();

  while (m_pCI->G_bEachMainWithSub(CK_KEYEXISTS,CK_FILTER)) {
    m_pCI->G.p_filter->vCard(CK_CONSTRAINT);
  }

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vS75_T40_Convert_stmColOdn_Of_constraint_To_stmColNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  m_pCI->vTransform(__func__, "convert all <table> into either <scan> or <indexJoin>"_Stz);

  vAssert_Board_mqA();

  while (m_pCI->G_bEachMainWithSub(CK_KEYEXISTS,CK_CONSTRAINT)) {
    Board::vS75_T40_Convert_stmColOdn_Of_constraint_To_stmColNm(m_pCI);
  }

  m_pCI->vEndTransform();
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                      vSeries_80_Index_Scan */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Bench::bS80_T00_Test(bool bEnabled)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  if (! bEnabled)
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vS80_T00_Skip(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vS80_T00_Begin(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  m_pCI->m_pCardCtl->vUseShorter();

  vST00_Begin();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vS80_T30_Morph_scan_To_indexScan(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  m_pCI->vTransform(__func__, "((replace with an explanation))"_Stz);

  vAssert_Board_mqA();

  m_pCI->G_vRestart();
  if (m_pCI->G_bEachMainWith2Sub(CK_SCAN,CK_INDEXLIST,CK_BOUNDS)) {
    if (m_pCI->G.p_bounds->pSub_lBound() || m_pCI->G.p_bounds->pSub_uBound()) {
      m_pCI->G.p_scan->vCard(CK_INDEXSCAN);

      m_pCI->G.p_indexList->vChoose_index_For_indexScan();
    }
  }

  m_pCI->vPrintBoard();

  m_pCI->vEndTransform();
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                 vSeries_90_Stream_Ordering */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Bench::bS90_T00_Test(bool bEnabled)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  if (! bEnabled)
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vS90_T00_Skip(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vS90_T00_Begin(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  //m_pCI->m_pCardCtl->vUseShorter();

  m_pCI->m_pCardCtl->vPrintSubCards(true,
                                    CK_KEY,
                                    CK_INDEXLIST,
                                    CK_PROJECTION,
                                    CK_RPNCALC,
                                    CK_STREAM,
                                    0);
  m_pCI->m_pCardCtl->vPrintSubCards(false,
                                    CK_STMCOLLIST,
                                    0);

  m_pCI->m_pCardCtl->vPrintBelow(CK_PROJECT,true);

  vST00_Begin();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vS90_T30_Derive_order(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  m_pCI->vTransform(__func__, "remove <sort> if <sbyElist> is the same as the <gbyElist>"_Stz);

  vAssert_Board_mqA();

  Board::vDerive_order(m_pCI);

  m_pCI->vEndTransform();
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                               vSeries_A0_Make_Weavable */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vSA0_T00_Begin(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  m_pCI->m_pCardCtl->vUseShorter();

  vST00_Begin();
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vAssert_Board_any(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  // nothing to do?
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vAssert_Board_mqA(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  RASSERT(0 == m_pCI->G.pBoard->i_Board());
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vAssert_Board_sqB(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  RASSERT(1 == m_pCI->G.pBoard->i_Board());
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vST00_Begin(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  if (! m_pCI->m_pCardCtl->bBoardPrint())
    return;

  char* azSeparator = (char*)
      "<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>"
      "<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>"
      "<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>";

  vTrace("\n%s\n%186s\n%s\n"_Stz,
         azSeparator,
         m_pCI->szXhand(),
         azSeparator);

  m_pCI->vXdesc(""_Stz);
  m_pCI->vXform(""_Stz);
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vStreamAnalysisReset(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  m_iOdnLo      = 1;
  m_iOdnHi      = C_UNKNOWN;
  //m_cTableSyn   = 'a';
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
