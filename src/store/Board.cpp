/*============================================================================*/
/* Board.cpp -- Board                                                         */
/*============================================================================*/

#include "Board.hpp"

#include "PtnDef.hpp"
#include "Cue.hpp"

#include "Optimiser.hpp"
#include "OptimiserFun.hpp"
#include "OptimiserCatalog.hpp"

#include "RelOp.hpp"
#include "Error.hpp"
#include "CardIter.hpp"

extern OptimiserCatalog gxOptimiserCatalog;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Board,CN_BOARD)
__aNewFunction(Board,CN_BOARD)
__vDeleteFunction(Board,CN_BOARD)
__aFooterFunction(Board)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Board::Board(Stz* szBoardNm, eBK eBoard, i8 i_Board, Bench* pBench)
: Object(CN_BOARD)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  m_i_Board    = i_Board;
  m_sBoardNm   = szBoardNm;
  m_eBoard = eBoard;

  m_pBench     = pBench;

  m_pDeck = new Card_Deck;

  m_pSubQuery = new SubQuery(this);

  m_pState = new TagSet;

  m_pMark = new Mark;


  _checkInitialised_(CN_BOARD);

  if (BK_JOINORDERING == eBoard)
    m_pMark->vChrSeq("JO"_Stz);
  else
    m_pMark->vSprint("Y%lld"_Stz,i_Board);

  pBench->vAddBoard(this);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Board::~Board(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  delete m_pDeck;

  delete m_pSubQuery;

  delete m_pState;

  delete m_pMark;
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vDerive_foldExpr_H_A_NC_ro(CardIter* pCI)
{
  _static_

  pCI->G.p_projection = nullptr;
  for (i8 i_Main = pCI->Sqy.m_i_FoldCard - 1; i_Main >= 0; i_Main--) {
    Card *pMain = pCI->G.pDeck->pGetChild(i_Main);
    if ((pCI->G.p_projection = pMain->pSub_projection()))
      break;
  }
  if (! pCI->G.p_projection)
    return;


  pCI->G_vAppendNewChild(pCI->G.p_fold, CK_EXPR);

  pCI->G_vAppendNewChild(pCI->G.p_expr, CK_AS, Symbol(SF_AS_NM, "_SQ1RESULT_"_Stz));
  pCI->G_vAppendNewChild(pCI->G.p_expr, CK_RPNCALC);
  pCI->G_vAppendNewChild(pCI->G.p_rpnCalc, CK_TERM, Symbol(SF_COL_NO, "$1"_Stz));

  Card_Expr* p_expr_found =
      pCI->Prior.m_pProjection->pGetExpr(0);

  pCI->G.p_term->pSymbol()->Y_vColType(p_expr_found->szP_Type());

  pCI->G.p_rpnCalc->pSymbol()->Y_vColType(p_expr_found->szP_Type());
  pCI->G.p_expr->pSymbol()->Y_vColType(p_expr_found->szP_Type());

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vDerive_foldProjection_F(CardIter* pCI)
{
  _static_

  pCI->G.p_projection = nullptr;
  for (i8 i_Main = pCI->Sqy.m_i_FoldCard - 1; i_Main >= 0; i_Main--) {
    Card *pMain = pCI->G.pDeck->pGetChild(i_Main);
    if ((pCI->G.p_projection = pMain->pSub_projection()))
      break;
  }
  if (! pCI->G.p_projection)
    return;

  pCI->G.p_fold->vAppend(pCI->G.p_projection->pDupCardRecursive());
}

/*----------------------------------------------------------------------------*/
/* find and mark possible call to subquery referenced in pCI->G.p_boolCalc    */
/*----------------------------------------------------------------------------*/
bool Board::bFindAndMarkSubQryCall(CardIter* pCI)
{
  _static_

  bool bFoundCall = false;

  Card_PRED* p_pred_ROOT = static_cast<Card_PRED*>(pCI->G.p_boolCalc->pGetChild(0));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambdaFn = [pCI,&bFoundCall](Card_PRED* p_pred, bool bUpwards)
  {
    if (bUpwards)
      return C_CONTINUE_RECURSION;

    //  --  --  --  --  --  --  --  --  --  --  --  -- -- --

    if (! (pCI->Sqy.m_pTerm_sqCall = p_pred->pGetTerm_boardNm(pCI)))
      return C_CONTINUE_RECURSION;

    pCI->Sqy.m_sSqyNm  = pCI->Sqy.m_pTerm_sqCall->szSymbol(SF_NONE);
    p_pred->vMark1(MARK_AT);
    bFoundCall = true;

    if (p_pred->pSymbol()->bSame("exists"_Stz) ||
        p_pred->pSymbol()->bSame("notExists"_Stz)) {

      RASSERT(p_pred->bIsSubQueryOp());

      pCI->Sqy.m_sSqyArg = p_pred->pGetTerm(0)->szSymbol(SF_NONE);
      pCI->Sqy.m_sSqyNm  = p_pred->pGetTerm(1)->szSymbol(SF_NONE);
      return C_TERMINATE_RECURSION;
    }

    if (p_pred->pSymbol()->bSame("in"_Stz) || p_pred->pSymbol()->bSame("notIn"_Stz)) {
      RASSERT(p_pred->bIsSubQueryOp());
      pCI->Sqy.m_sSqyArg = p_pred->pGetTerm(2)->szSymbol(SF_NONE);
      return C_TERMINATE_RECURSION;
    }

    RASSERT(p_pred->bIsRelnOp());

    return C_TERMINATE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  bool bIgnore = p_pred_ROOT->bPredRecursive(bLambdaFn);
  UNUSED(bIgnore);

  if (bFoundCall)
    return true;
  return false;
}

/*----------------------------------------------------------------------------*/
/* convert <table> into either <scan>, <indexJoin> or <keyExists>             */
/*----------------------------------------------------------------------------*/
void Board::vMorphAll_table_To_EEop(CardIter* pCI)
{
  _static_

  pCI->vPrintBoard(__LINE__);

  pCI->vBeginEach();
  while (pCI->G_bEachMainWithSub(CK_FROM,CK_TABLE)) {

    pCI->G.p_table->vMorph_table_To_EEop(pCI);
  }

  pCI->vPrintBoard(__LINE__);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vValidate(CardIter* pCI)
{
  _static_

  //pCI->vUsingBoard((i8)0);

  if (pCI->G.pBoard->eBoard() == BK_SUBQUERY)
    vValidate_SubQuery(pCI);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vValidate_SubQuery(CardIter *pCI)
{
  _static_

  if (!pCI->m_pBench->bMadeSimpler())
    return;

  pCI->vSave();
  pCI->vNullMembers();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // main cards

  RASSERT(pCI->m_pBench);

  Card_Deck* pDeck = pCI->G.pDeck;


  pDeck->vValidate(pCI);
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pCI->vRestore();
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Bench* Board::pBench(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BOARD, _FOOTER_);

  return m_pBench;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
i8 Board::i_Board(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BOARD, _FOOTER_);

  return m_i_Board;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vBoardNo(i8 i_Board)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BOARD, _FOOTER_);

  m_i_Board = i_Board;
}

/*----------------------------------------------------------------------------*/
/* get board kind                                                             */
/*----------------------------------------------------------------------------*/
eBK Board::eBoard(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  return m_eBoard;
}

/*----------------------------------------------------------------------------*/
/* get board name                                                             */
/*----------------------------------------------------------------------------*/
Stz* Board::szBoardNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  return m_sBoardNm;
}

/*----------------------------------------------------------------------------*/
/* set board name                                                             */
/*----------------------------------------------------------------------------*/
void Board::vBoardNm(Stz* szBoardNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  m_sBoardNm = szBoardNm;
}

/*----------------------------------------------------------------------------*/
/* get card registers                                                         */
/*----------------------------------------------------------------------------*/
CardCtl* Board::pCardCtl(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  return m_pBench->pCardCtl();
}

/*----------------------------------------------------------------------------*/
/* clear all marks                                                           */
/*----------------------------------------------------------------------------*/
void Board::vClearCardMarks(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  m_pDeck->vMarksClearRecursive();
}

/*----------------------------------------------------------------------------*/
/* get card subSet                                                            */
/*----------------------------------------------------------------------------*/
Card_Deck* Board::pDeck(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  return m_pDeck;
}

/*----------------------------------------------------------------------------*/
/* get mark                                                                   */
/*----------------------------------------------------------------------------*/
Mark* Board::pMark(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  return m_pMark;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vPrintBoard(Stz* szDescription)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  Stz* szQueryNm = m_pBench->pOptimiser()->szQueryNm();

  Ribbon* pRibbon = new Ribbon(m_pBench->pOptimiser()->pCardCtl(),this);

  vPinTitle(pRibbon, m_pBench->iXform(), szDescription);

  vPinCards(pRibbon);

  pRibbon->vDrawH('-',true);
  pRibbon->vDrawH('=');

  vPinMargin(pRibbon);

  i8 iBenchNo = m_pBench->iBenchNo();
  i8 iPrintNo = m_pBench->iPrint_inc();
  pRibbon->vPrintRibbon(szQueryNm, iBenchNo,iPrintNo);

  delete pRibbon;
}

/*----------------------------------------------------------------------------*/
/* get sub-query                                                              */
/*----------------------------------------------------------------------------*/
void Board::vRenumberCards(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  m_pDeck->vRenumberRoot(m_i_Board);
}

/*----------------------------------------------------------------------------*/
/* get card subSet                                                            */
/*----------------------------------------------------------------------------*/
TagSet* Board::pState(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  return m_pState;
}


/*----------------------------------------------------------------------------*/
/* get sub-query                                                              */
/*----------------------------------------------------------------------------*/
SubQuery* Board::pSubQuery(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  return m_pSubQuery;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* pin left margin                                                            */
/*----------------------------------------------------------------------------*/
void Board::vPinMargin(Ribbon* pRibbon)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  s32 sLeftMargin;

  sLeftMargin.vSprint("%s "_Stz,
                      m_pMark->szChrSeq());

  pRibbon->vInsertCharsV(0,0,sLeftMargin);
}
/*----------------------------------------------------------------------------*/
//void Board::vPinMargin(Ribbon* pRibbon)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
//
////  pRibbon->vDrawH('-',true);
////  pRibbon->vDrawH('=');
//
//  //pRibbon->vPutTextRight(szXhand(),1);
//  //pRibbon->vPutTextRight(szXfunc(),2);
//
//  s32 sLeftMargin;
//
//  char cPrefix = m_pBench->cPrefix();
//
//  if (' ' == cPrefix)
//    sLeftMargin.vSprint("%lld "_Stz, m_i_Board);
//  else
//  if ('m' == cPrefix || 'w' == cPrefix)
//    sLeftMargin.vSprint("%c "_Stz, cPrefix);
//  else
//  if (m_pBench->bIsFlat() && 0 == m_i_Board)
//    sLeftMargin.vSprint("fq "_Stz);
//  else
//    sLeftMargin.vSprint("%c%lld "_Stz, cPrefix,m_i_Board);
//
//  pRibbon->vInsertCharsV(0,0,sLeftMargin);
//  //pRibbon->vDrawV(0,0,"x"_Stz);
//}

/*----------------------------------------------------------------------------*/
/* pin board onto given ribbon                                                */
/*----------------------------------------------------------------------------*/
void Board::vPinCards(Ribbon* pRibbon)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  CardCtl* pCardCtl = m_pBench->pOptimiser()->pCardCtl();

  if (! m_pDeck->nItems())
    return;

  pRibbon->vLaneFirst();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Ribbon* pSubRibbon = new Ribbon(pCardCtl,this);

  Kid xBI(m_pDeck);

  i8 nCount = 0;

  while (xBI.bEach()) {

    if (! nCount++)
      pSubRibbon->vLaneFirst();
    else
    if (pSubRibbon->i_Char() && pCardCtl->bHeatNext(xBI.pChild()->eCard()))
      pSubRibbon->vHeatNext();
    else
      pSubRibbon->vLaneNext();


    bool bPrune = false;
    eAL eLevel = AL_NONE;
    Card_Level* p_level = xBI.pChild()->pSub_level();
    if (p_level)
      eLevel = (eAL) p_level->pSymbol()->Y_iGet(P_Level);
    xBI.pChild()->vPrintCardRecursive(pSubRibbon,
                                          0,
                                          m_pDeck,
                                          bPrune,
                                          eLevel,
                                          false,
                                          m_pBench->iXform(),
                                          m_pBench->szXfunc());
  }

  pRibbon->vPinSubRibbon(pRibbon->i_Heat(), 0, pSubRibbon);

  delete pSubRibbon;
}

/*----------------------------------------------------------------------------*/
/* pin board onto given ribbon                                                */
/*----------------------------------------------------------------------------*/
void Board::vPinTitle(Ribbon* pRibbon,
                      i8 iTransformNo, Stz* szDescription)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
  UNUSED(iTransformNo);
  UNUSED(szDescription);

  if (! m_pDeck->nItems())
    return;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // line 1
  pRibbon->vSprintOther("Transform: %02lld Board %lld %s %p Query %s %s"_Stz,
                        m_pBench->iXform(),
                        m_i_Board,
                        m_sBoardNm.szStz(),
                        this,
                        m_pBench->pOptimiser()->szQueryNm(),
                        AM::JOB::bHasTag(C_OPTIMISE_PASSTHRU) ?
                        "/optimise_passthru" : ""
  );

  // line 2
  pRibbon->vSprintOther("%s %s"_Stz,
                        aXF[m_pBench->eXform()],
                        m_pBench->szXdesc());

  // line 3
  if (0 == m_i_Board)
    pRibbon->vSprintOther("%s %s"_Stz,
                          "MainQuery"_Stz,
                          aQD[m_pSubQuery->eDisposition()]);
  else
    pRibbon->vSprintOther("%s(%lld params) %s caller: board[%lld] %s %s %s"_Stz,
                          "SubQuery"_Stz,
                          m_pSubQuery->nCorrCols(),
                          aQD[m_pSubQuery->eDisposition()],

                          m_pSubQuery->pBoard()->i_Board(),
                          aQC[m_pSubQuery->eClause()],
                          aQO[m_pSubQuery->eCallOp()],
                          aRO[m_pSubQuery->eRelnOp()]
    );

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pRibbon->vTitleLines();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
