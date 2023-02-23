/*============================================================================*/
/* Bench.cpp -- Bench                                                         */
/*============================================================================*/

#include "Bench.hpp"
#include "Board.hpp"
#include "Optimiser.hpp"
#include "CardIter.hpp"
#include "Smoosh.hpp"
#include "Smooth.hpp"
#include "Fold.hpp"
#include "Flattening.hpp"

#include "CarryForward.hpp"
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

  m_pFlattening = nullptr;
  
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

  delete m_pFlattening;
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
    //m_pOptimiser->pCardCtl()->vPrintSubCards(false, //CK_AGGELIST,
        //CK_KEY, //CK_PROJECTION,
        //CK_RPNCALC,
    //                                          0);
    m_pOptimiser->pCardCtl()->vSaveShorter();
    m_pOptimiser->pCardCtl()->vSaveXhand();

    vApplyTransformationMatrix();

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
/* apply the recursive transformation matrix                                  */
/*----------------------------------------------------------------------------*/
void Bench::vApplyTransformationMatrix(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  vSeries_PreparePipelines();
  vSeries_MakeRunnable();

  vSeries_Optimisations();
  vSeries_MakeWeavable();
}

/*----------------------------------------------------------------------------*/
/* apply the reducePipeline set of transformations to all subPipe boards      */
/*----------------------------------------------------------------------------*/
void Bench::vSeries_PreparePipelines(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  vSeries(BK_SUBQUERY, 0, __func__);

  m_pCI->vBoardIteration();
  while (m_pCI->bEachBoard()) {
    m_pCI->vUsingBoard(BK_SUBQUERY, m_pCI->G.i_Board);

    vGroup_InitialAdjustments();
    vGroup_CondenseInfo();
    vGroup_MetaAnalysis();
    vGroup_PreliminaryConversions();
  }

  vTransform_OuterQueryAnalysis();

  vEndSeries(BK_SUBQUERY);
}

/*----------------------------------------------------------------------------*/
/* apply the reducePipeline set of transformations to all subPipe boards      */
/*----------------------------------------------------------------------------*/
void Bench::vSeries_MakeRunnable(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  vSeries(BK_SUBQUERY, 0, __func__);

  m_pCI->G.pSubQuery = m_pCI->G.pBoard->pSubQuery();

  m_pFlattening = new Flattening(m_pCI);

  vSeries_Flatten_recurse();

  delete m_pFlattening;
  m_pFlattening = nullptr;

  vEndSeries(BK_SUBQUERY);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vSeries_Optimisations(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  vSeries(BK_SUBQUERY, 0, __func__);

  i8 i_Board = 0;

  m_pCI->vUsingBoard(BK_SUBQUERY, i_Board);

  vGroup_ChooseEEops();

  vEndSeries(BK_SUBQUERY);
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vSeries_MakeWeavable(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  vSeries(BK_SUBQUERY, 0, __func__);

  vGroup_CullBounds();
  vGroup_Filters();
  vGroup_Staging();
  vGroup_Partitioning();
  vGroup_FinalAdjustments();

  vEndSeries(BK_SUBQUERY, 0);
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
void Bench::vGroup_CondenseInfo(void)
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

  vTransform_TableAnalysis();
  vTransform_ProjectionAnalysis();
  vTransform_IndexAnalysis();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vGroup_PreliminaryConversions(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  //vTransform_StmColToTblCol_originalModel();
  vTransform_RpnCalcToBoolCalc();
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

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Affix TblColOdn To All IdxCol

  m_pCI->G_vRestart();
  Card_Table::vAffixTableColOrd(m_pCI);

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* Projection Analysis                                                        */
/*----------------------------------------------------------------------------*/
void Bench::vTransform_ProjectionAnalysis(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  m_pCI->vTransform(__func__, "((desc))"_Stz);

  Card_Projection::vProjection_Analysis_v2(m_pCI);

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vTransform_RpnCalcToBoolCalc(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  m_pCI->vTransform(__func__, "all $n become $c.n"_Stz);

  m_pCI->G_vRestart();
  while (m_pCI->G_bEachMainWithSub(CK_WHERE, CK_RPNCALC))
    ((Card_OP*)m_pCI->G.p_MainCard)->vConvert_rpnCalc_To_boolCalc(m_pCI);

  m_pCI->G_vRestart();
  while (m_pCI->G_bEachMainWithSub(CK_HAVING, CK_RPNCALC))
    ((Card_OP*)m_pCI->G.p_MainCard)->vConvert_rpnCalc_To_boolCalc(m_pCI);

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

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // search for parameters

    m_pCI->G_vRestart();
    i8 nParams = 0;
    while (m_pCI->G_bEachMain(CK_CORRELATION))
       nParams++;
    pOuterQuery->vCorrCols(nParams);

  }

  m_pCI->vBoardIteration();
  while (m_pCI->bEachBoard()) {
    SubQuery* pOuterQuery = m_pCI->G.pBoard->pSubQuery();
    pOuterQuery->vDump();
  }

    //m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* convert all <table> into either <scan> , <indexJoin> or <keyExists>        */
/*----------------------------------------------------------------------------*/
void Bench::vTransform_MorphTableToEEop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  m_pCI->vTransform(__func__, "convert all <table> into either <scan> or <indexJoin>"_Stz);

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

  m_pCI->vPrintBoard(__LINE__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // mark any cmpPred that are a bound in bounds

  m_pCI->G_vRestart();
  while (m_pCI->G_bEachMain()) {

    if (! (m_pCI->G.p_MainCard->bIsCard(CK_INDEXJOIN) ||
           m_pCI->G.p_MainCard->bIsCard(CK_INDEXSCAN) ||
           m_pCI->G.p_MainCard->bIsCard(CK_KEYEXISTS) ||
           m_pCI->G.p_MainCard->bIsCard(CK_KEYNOTEXISTS)
           ))
      continue;
    if (! (m_pCI->G.p_bounds = m_pCI->G.p_MainCard->pSub_bounds()))
      continue;
    if (! (m_pCI->G.p_constraint = m_pCI->G.p_MainCard->pSub_constraint()))
      continue;

    // have <bounds> & <filter> under <main>

    CardIter xCR(m_pCI->G.p_constraint);
    while ((m_pCI->G.p_cmpPred = _pCard_CmpPred(xCR.pRecurseEach(CK_CMPPRED)))) {
      if (m_pCI->G.p_cmpPred->bSubsetOf_bounds(m_pCI->G.p_bounds))
        m_pCI->G.p_cmpPred->vMark1(MARK_AT);
    }
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cull marked cmpPred from filter

  m_pCI->G_vRestart();
  Card_Constraint::vCullUsedBounds(m_pCI);

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

  m_pCI->G_vRestart();
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

  Board::vAdd_Partitioning(m_pCI);

  m_pCI->vEndTransform();
}


/*----------------------------------------------------------------------------*/
/*-- Optimisations  ----------------------------------------------------------*/
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

  NEVER_REACH


  /*
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
*/

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

  NEVER_REACH

/*
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
*/

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

  if (i_Board >= nBoards())
    return nullptr;

  Board *pBoard = this->pGet(i_Board);
  ASSERT(pBoard->i_Board() == i_Board);
  return pBoard;

//  i8 nBoards = this->nBoards();
//  for (i8 i_each = 0; i_each < nBoards; i_each++) {
//    Board *pBoard = this->pGet(i_each);
//
//    if (eBoard == pBoard->eBoardKind() &&
//        i_Board == pBoard->i_Board())
//      return pBoard;
//  }
//
//  return nullptr;
}
/*----------------------------------------------------------------------------*/
//Board* Bench::pGetBoard(eBK eBoard, i8 i_Board)
//{
//  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);
//
//  i8 nBoards = this->nBoards();
//  for (i8 i_each = 0; i_each < nBoards; i_each++) {
//    Board *pBoard = this->pGet(i_each);
//
//    if (eBoard == pBoard->eBoardKind() && (C_UNKNOWN == i_Board ||
//                                           i_Board == pBoard->i_Board()))
//      return pBoard;
//  }
//
//  return nullptr;
//}


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
/*                                                      vSeries_80_Index_Scan */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vS80_T30_Morph_scan_To_indexScan(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  m_pCI->vTransform(__func__, "((replace with an explanation))"_Stz);

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
void Bench::vS90_T30_Derive_order(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  m_pCI->vTransform(__func__, "remove <sort> if <sbyElist> is the same as the <gbyElist>"_Stz);

  
  Board::vDerive_order(m_pCI);

  m_pCI->vEndTransform();
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

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


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
