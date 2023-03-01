/*============================================================================*/
/* PrepareSubQueries.cpp -- PrepareSubQueries                                           */
/*============================================================================*/

#include "PrepareSubQueries.hpp"

#include "Board.hpp"
#include "CardIter.hpp"
#include "Fold.hpp"
#include "Smoosh.hpp"
#include "Smooth.hpp"
#include "Optimiser.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, PrepareSubQueries, CN_PREPARESUBQUERIES)
__aNewFunction(PrepareSubQueries, CN_PREPARESUBQUERIES)
__vDeleteFunction(PrepareSubQueries, CN_PREPARESUBQUERIES)
__aFooterFunction(PrepareSubQueries)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
PrepareSubQueries::PrepareSubQueries(CardIter* pCI)
: Object(CN_PREPARESUBQUERIES)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PREPARESUBQUERIES, _FOOTER_);

  m_pCI = pCI;
  m_cTableSyn = 'a';

  _checkInitialised_(CN_PREPARESUBQUERIES);

  _postcondition_(_ADDRESS_, _MAGIC_, CN_PREPARESUBQUERIES, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
PrepareSubQueries::~PrepareSubQueries(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PREPARESUBQUERIES, _FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* apply the reducePipeline set of transformations to all subPipe boards      */
/*----------------------------------------------------------------------------*/
void PrepareSubQueries::vSeries_PrepareSubQueries(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PREPARESUBQUERIES, _FOOTER_);

  //vSeries(BK_SUBQUERY, 0, __func__);

  m_pCI->vBoardIteration();
  while (m_pCI->bEachBoard()) {
    m_pCI->vUsingBoard(BK_SUBQUERY, m_pCI->G.i_Board);

    //vGroup_InitialAdjustments();
    vTransform_MoveOn();
    vTransform_ShortenQueryNames();

    //vGroup_CondenseInfo();
    vTransform_DeleteUnneeded();
    vTransform_ScanBecomesTable();
    vTransform_AddLevel();
    vTransform_NotOps();

    //vGroup_MetaAnalysis();
    vTransform_TableAnalysis(&m_cTableSyn);
    vTransform_ProjectionAnalysis();
    vTransform_IndexAnalysis();

    //vGroup_PreliminaryConversions();
    vTransform_RpnCalcToBoolCalc();
  }

  vTransform_OuterQueryAnalysis();

  //vEndSeries(BK_SUBQUERY);
}

/*----------------------------------------------------------------------------*/
/* Analyse all sub-queries                                                    */
/*----------------------------------------------------------------------------*/
void PrepareSubQueries::vTransform_OuterQueryAnalysis(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PREPARESUBQUERIES, _FOOTER_);

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
/* move all <absJoin>/<on> to <where>                                         */
/*----------------------------------------------------------------------------*/
void PrepareSubQueries::vTransform_MoveOn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PREPARESUBQUERIES,_FOOTER_);

  m_pCI->vTransform(__func__, "move <on> in <absJoin>'s to <where>"_Stz);

  Card_Where::vMove_All_on_To_where(m_pCI);

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void PrepareSubQueries::vTransform_ShortenQueryNames(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PREPARESUBQUERIES,_FOOTER_);

  m_pCI->vTransform(__func__, "move <on> in <absJoin>'s to <where>"_Stz);


  s32 sBoardNm = m_pCI->G.pBoard->szBoardNm();
  i8 iSqNo = StrLib::iGleanInt(sBoardNm.azChar());
  sBoardNm.vSprint("__sq%02lld"_Stz,iSqNo);
  m_pCI->G.pBoard->vBoardNm(sBoardNm);


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  CardIter* pCI = m_pCI;

  auto bLambdaFn = [pCI](Card* p_card) {
    if (! Card::bIsSubQueryName(p_card->szValue()))
      return C_CONTINUE_RECURSION;

    i8 iSqNo = StrLib::iGleanInt(p_card->szValue()->azChar());

    p_card->vValueSprint("__sq%02lld"_Stz, iSqNo);

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  bool b = m_pCI->G.pDeck->bApplyRecursive(CK_NONE,bLambdaFn);


  m_pCI->vEndTransform();
}


/*----------------------------------------------------------------------------*/
/* Condense Info                                                              */
/*----------------------------------------------------------------------------*/
void PrepareSubQueries::vTransform_DeleteUnneeded(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PREPARESUBQUERIES,_FOOTER_);

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
void PrepareSubQueries::vTransform_ScanBecomesTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PREPARESUBQUERIES,_FOOTER_);

  m_pCI->vTransform(__func__, "delete superfluous <absJoin>'s"_Stz);

  m_pCI->G_vRestart();
  while (m_pCI->G_bEachMain(CK_SCAN))
    Card_Scan::vScanBecomesTable(m_pCI);

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* Add Levels                                                                 */
/*----------------------------------------------------------------------------*/
void PrepareSubQueries::vTransform_AddLevel(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PREPARESUBQUERIES,_FOOTER_);

  m_pCI->vTransform(__func__, "delete superfluous <absJoin>'s"_Stz);

  m_pCI->G_vRestart();
  while (m_pCI->G_bEachMain())
    ((Card_OP*)m_pCI->G.p_MainCard)->vAddLevel();

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* Not Ops                                                                    */
/*----------------------------------------------------------------------------*/
void PrepareSubQueries::vTransform_NotOps(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PREPARESUBQUERIES, _FOOTER_);

  m_pCI->G_vRestart();
  while (m_pCI->G_bEachSubOfMain(CK_RPNCALC))
    m_pCI->G.p_rpnCalc->vConvert_sqyOp_not_To_notSqyOp();
}

/*----------------------------------------------------------------------------*/
/* Table Analysis                                                             */
/*----------------------------------------------------------------------------*/
void PrepareSubQueries::vTransform_TableAnalysis(char* acTableSyn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PREPARESUBQUERIES,_FOOTER_);

  m_pCI->vTransform(__func__, "add a <tableSyn> to all <table>'s"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Affix Table Syntax To All Table
  Card_Table::vAffixTableSyn(m_pCI, acTableSyn);

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
//void PrepareSubQueries::vS20b_T_Table_Analysis(void)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_PREPARESUBQUERIES,_FOOTER_);
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
void PrepareSubQueries::vTransform_IndexAnalysis(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PREPARESUBQUERIES,_FOOTER_);

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
void PrepareSubQueries::vTransform_ProjectionAnalysis(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PREPARESUBQUERIES,_FOOTER_);

  m_pCI->vTransform(__func__, "((desc))"_Stz);

  Card_Projection::vProjection_Analysis_v2(m_pCI);

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void PrepareSubQueries::vTransform_RpnCalcToBoolCalc(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PREPARESUBQUERIES,_FOOTER_);

  m_pCI->vTransform(__func__, "all $n become $c.n"_Stz);

  m_pCI->G_vRestart();
  while (m_pCI->G_bEachMainWithSub(CK_WHERE, CK_RPNCALC))
    ((Card_OP*)m_pCI->G.p_MainCard)->vConvert_rpnCalc_To_boolCalc(m_pCI);

  m_pCI->G_vRestart();
  while (m_pCI->G_bEachMainWithSub(CK_HAVING, CK_RPNCALC))
    ((Card_OP*)m_pCI->G.p_MainCard)->vConvert_rpnCalc_To_boolCalc(m_pCI);

  m_pCI->vEndTransform();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
