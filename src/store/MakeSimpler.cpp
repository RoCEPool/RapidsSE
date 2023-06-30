/*============================================================================*/
/* MakeSimpler.cpp -- Make Simpler                                            */
/*============================================================================*/

#include "MakeSimpler.hpp"

#include "Board.hpp"
#include "CardIter.hpp"
#include "Fold.hpp"
#include "Smoosh.hpp"
#include "Smooth.hpp"
#include "Optimiser.hpp"
#include "OuterQuery.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, MakeSimpler, CN_MAKESIMPLER)
__aNewFunction(MakeSimpler, CN_MAKESIMPLER)
__vDeleteFunction(MakeSimpler, CN_MAKESIMPLER)
__aFooterFunction(MakeSimpler)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
MakeSimpler::MakeSimpler(CardIter* pCI)
: Object(CN_MAKESIMPLER)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MAKESIMPLER, _FOOTER_);

  m_pCI = pCI;
  m_cTableSyn = 'a';

  _checkInitialised_(CN_MAKESIMPLER);

  _postcondition_(_ADDRESS_, _MAGIC_, CN_MAKESIMPLER, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
MakeSimpler::~MakeSimpler(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MAKESIMPLER, _FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* apply the reducePipeline set of transformations to all subPipe boards      */
/*----------------------------------------------------------------------------*/
void MakeSimpler::vSimplify(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MAKESIMPLER, _FOOTER_);

  m_pCI->vBoardIteration();
  while (m_pCI->bEachBoard()) {
    m_pCI->vUsingBoard(m_pCI->G.i_Board);
    vShortenQueryNames();
  }

  m_pCI->vBoardIteration();
  while (m_pCI->bEachBoard()) {
    m_pCI->vUsingBoard(m_pCI->G.i_Board);

    //vGroup_InitialAdjustments();
    vMoveOn();

    //vGroup_CondenseInfo();
    vDeleteUnneeded();
    vScanBecomesTable();
    vAddLevel();
    vNotOps();

    //vGroup_MetaAnalysis();
    vTableAnalysis(&m_cTableSyn);
    vProjectionAnalysis();
    vIndexAnalysis();

    //vGroup_PreliminaryConversions();
    vRpnCalcToBoolCalc();
    vAsReview();

    vDebugPoint();
  }

  OuterQuery* pOuterQuery = new OuterQuery;
  pOuterQuery->vQueryAnalysis(m_pCI);
  delete pOuterQuery;
}

/*----------------------------------------------------------------------------*/
/* apply the reducePipeline set of transformations to all subPipe boards      */
/*----------------------------------------------------------------------------*/
void MakeSimpler::vSimplify_passThru(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MAKESIMPLER, _FOOTER_);

  m_pCI->vBoardIteration();
  while (m_pCI->bEachBoard()) {
    m_pCI->vUsingBoard(m_pCI->G.i_Board);
    vShortenQueryNames();
  }

  m_pCI->vBoardIteration();
  while (m_pCI->bEachBoard()) {
    m_pCI->vUsingBoard(m_pCI->G.i_Board);

    vTableAnalysis_passThru(&m_cTableSyn);
    vIndexAnalysis_passThru();

    vProjectionAnalysis_passThru();

    m_pCI->vTransform(__func__, "all $n become $c.n"_Stz);

    while (m_pCI->G_bEachMain())
      ((Card_OP *) m_pCI->G.p_MainCard)->vConvert_stmColOdn_To_tblColOdn(m_pCI);

    m_pCI->vEndTransform();

    vDebugPoint();
  }

}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Add Levels                                                                 */
/*----------------------------------------------------------------------------*/
void MakeSimpler::vAddLevel(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MAKESIMPLER, _FOOTER_);

  m_pCI->vTransform(__func__, "delete superfluous <absJoin>'s"_Stz);

  m_pCI->G_vRestart();
  while (m_pCI->G_bEachMain())
    ((Card_OP*)m_pCI->G.p_MainCard)->vAddLevel();

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* As review                                                                  */
/*----------------------------------------------------------------------------*/
void MakeSimpler::vAsReview(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MAKESIMPLER, _FOOTER_);

  m_pCI->vTransform(__func__, "as review"_Stz);

  m_pCI->G_vRestart();

  RASSERT(nullptr == m_pCI->StreamOrder.m_pProjection_running);
  RASSERT(nullptr == m_pCI->StreamOrder.m_pKey_running);

  while (m_pCI->G_bEachMain()) {

    switch (m_pCI->G.e_MainCard) {

      case CK_GBYAGG:
        m_pCI->G.p_gbyAgg->vAs_Review(m_pCI);
        break;

      case CK_INDEXJOIN:  break;
      case CK_INDEXSCAN:  break;
      case CK_PROJECT:    break;
      case CK_SORT:       break;

      case CK_TABLE:
        m_pCI->StreamOrder.m_pProjection_running = m_pCI->G.p_table->pSub_projection();
        m_pCI->StreamOrder.m_pKey_running = nullptr;
        break;

      default:
        break;
    }

    m_pCI->vPrintBoard(__LINE__);
  }

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* Condense Info                                                              */
/*----------------------------------------------------------------------------*/
void MakeSimpler::vDeleteUnneeded(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MAKESIMPLER, _FOOTER_);

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
/*                                                                            */
/*----------------------------------------------------------------------------*/
void MakeSimpler::vIndexAnalysis(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MAKESIMPLER, _FOOTER_);

  m_pCI->vTransform(__func__, "index analysis"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Append Index List Under All Table
  Card_Table::vAppendIndexList(m_pCI);

  m_pCI->vPrintBoard();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Affix TblColOdn To All IdxCol

  m_pCI->G_vRestart();
  Card_Table::vAffixTableColOrd(m_pCI);

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void MakeSimpler::vIndexAnalysis_passThru(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MAKESIMPLER, _FOOTER_);

  m_pCI->vTransform(__func__, "index analysis"_Stz);

  while (m_pCI->G_bEachMain(CK_INDEXSCAN)) {
    m_pCI->G.p_indexScan->vUpdate_projection(m_pCI);

    m_pCI->G.p_index = m_pCI->G.p_indexScan->pSub_index();
    m_pCI->G.p_index->pSymbol()->Y_vSet(P_ChosenIndex);
  }

  m_pCI->vRestart();
  while (m_pCI->G_bEachMain(CK_INDEXJOIN)) {
    m_pCI->G.p_indexJoin->vUpdate_projection(m_pCI);

    m_pCI->G.p_index = m_pCI->G.p_indexJoin->pSub_index();
    m_pCI->G.p_index->pSymbol()->Y_vSet(P_ChosenIndex);

    m_pCI->G.p_bound = m_pCI->G.p_index->pSub_bound();
    m_pCI->G.p_bound->pSymbol()->Y_vSet(P_ChosenBound);
  }

  m_pCI->vRestart();
  while (m_pCI->G_bEachMain(CK_KEYEXISTS)) {
    m_pCI->G.p_keyExists->vUpdate_projection(m_pCI);

    m_pCI->G.p_index = m_pCI->G.p_keyExists->pSub_index();
    m_pCI->G.p_index->pSymbol()->Y_vSet(P_ChosenIndex);
  }

  m_pCI->vRestart();
  while (m_pCI->G_bEachMain(CK_KEYNOTEXISTS)) {
    m_pCI->G.p_keyNotExists->vUpdate_projection(m_pCI);

    m_pCI->G.p_index = m_pCI->G.p_keyNotExists->pSub_index();
    m_pCI->G.p_index->pSymbol()->Y_vSet(P_ChosenIndex);
  }

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* move all <absJoin>/<on> to <where>                                         */
/*----------------------------------------------------------------------------*/
void MakeSimpler::vMoveOn(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MAKESIMPLER, _FOOTER_);

  m_pCI->vTransform(__func__, "move <on> in <absJoin>'s to <where>"_Stz);

  Card_Where::vMove_All_on_To_where(m_pCI);

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* Not Ops                                                                    */
/*----------------------------------------------------------------------------*/
void MakeSimpler::vNotOps(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MAKESIMPLER, _FOOTER_);

  m_pCI->G_vRestart();
  while (m_pCI->G_bEachSubOfMain(CK_RPNCALC))
    m_pCI->G.p_rpnCalc->vConvert_sqyOp_not_To_notSqyOp();
}

/*----------------------------------------------------------------------------*/
/* Projection Analysis                                                        */
/*----------------------------------------------------------------------------*/
void MakeSimpler::vProjectionAnalysis(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MAKESIMPLER, _FOOTER_);

  m_pCI->vTransform(__func__, "vProjectionAnalysis"_Stz);

  m_pCI->G.pDeck->vProjectionAnalysis(m_pCI);

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* Projection Analysis                                                        */
/*----------------------------------------------------------------------------*/
void MakeSimpler::vProjectionAnalysis_passThru(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MAKESIMPLER, _FOOTER_);

  m_pCI->vTransform(__func__, "vProjectionAnalysis_passThru"_Stz);

  m_pCI->G.pDeck->vProjectionAnalysis_passThru(m_pCI);

  m_pCI->vEndTransform();
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void MakeSimpler::vRpnCalcToBoolCalc(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MAKESIMPLER, _FOOTER_);

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
/* Condense Info                                                              */
/*----------------------------------------------------------------------------*/
void MakeSimpler::vScanBecomesTable(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MAKESIMPLER, _FOOTER_);

  m_pCI->vTransform(__func__, "delete superfluous <absJoin>'s"_Stz);

  m_pCI->G_vRestart();
  while (m_pCI->G_bEachMain(CK_SCAN))
    Card_Scan::vScanBecomesTable(m_pCI);

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void MakeSimpler::vShortenQueryNames(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MAKESIMPLER, _FOOTER_);

  m_pCI->vTransform(__func__, "vShortenQueryNames"_Stz);

  s32 sBoardNm = m_pCI->G.pBoard->szBoardNm();
  if (Symbol::b_IsSubQueryName(sBoardNm)) {
    i8 iSqNo = StrLib::iGleanInt(sBoardNm.azChar());
    Symbol xSymbol_sqNo;
    xSymbol_sqNo.vSetSubQueryOrdinal(iSqNo);
    m_pCI->G.pBoard->vBoardNm(xSymbol_sqNo);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  CardIter* pCI = m_pCI;

  auto bLambdaFn = [pCI](Card* p_card) {
    if (!Symbol::b_IsSubQueryName(p_card->szSymbol()))
      return C_CONTINUE_RECURSION;

    i8 iSqNo = StrLib::iGleanInt(p_card->szSymbol()->azChar());

    Symbol xSymbol_sqNo;
    xSymbol_sqNo.vSetSubQueryOrdinal(iSqNo);
    p_card->vSymbol(xSymbol_sqNo);

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  bool b = m_pCI->G.pDeck->bApplyRecursive(CK_NONE,bLambdaFn);


  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* Table Analysis                                                             */
/*----------------------------------------------------------------------------*/
void MakeSimpler::vTableAnalysis(char* acTableSyn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MAKESIMPLER, _FOOTER_);

  m_pCI->vTransform(__func__, "add a <tableSyn> to all <table>'s"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Affix Table Syntax To All Table
  Card_Table::vAffixTableSyn(m_pCI, acTableSyn);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Produce Table Partition Under All Table
  m_pCI->G_vRestart();
  Card_Table::vProduceTablePtns(m_pCI);

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* Table Analysis                                                             */
/*----------------------------------------------------------------------------*/
void MakeSimpler::vTableAnalysis_passThru(char* acTableSyn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MAKESIMPLER, _FOOTER_);

  m_pCI->vTransform(__func__, "add a <tableSyn> to all <table>'s"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Affix Table Syntax To All Table
  Card_Table::vAffixTableSyn_passThru(m_pCI, acTableSyn);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Produce Table Partition Under All Table
//  m_pCI->G_vRestart();
//  Card_Table::vProduceTablePtns(m_pCI);

  m_pCI->vEndTransform();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
