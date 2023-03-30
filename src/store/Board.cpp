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
Board::Board(Stz* szBoardNm, eBK eBoardKind, i8 i_Board, Bench* pBench)
: Object(CN_BOARD)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  m_i_Board    = i_Board;
  m_sBoardNm   = szBoardNm;
  m_eBoardKind = eBoardKind;

  m_pBench     = pBench;

  m_pCard_Deck = new Card_Deck;

  m_iXform = C_UNKNOWN;

  m_pSubQuery = new SubQuery(this);

  _checkInitialised_(CN_BOARD);

  pBench->vAddBoard(this);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Board::~Board(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  //m_pCard_Deck->vRemoveAll();

  delete m_pCard_Deck;

  delete m_pSubQuery;
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
    if (pCI->G.p_projection = pMain->pSub_projection())
      break;
  }
  if (! pCI->G.p_projection)
    return;


  pCI->G.p_type = pCI->G.p_projection->pSub_type();

  pCI->G_vAppendNewChild(pCI->G.p_fold, CK_EXPR);

  if (pCI->G.p_type) {
    pCI->G.p_expr->vAppend(pCI->G.p_type->pDupCardAndSubCards());
  }
  else {
    pCI->G_vAppendNewChild(pCI->G.p_expr, CK_TYPE, "((type missing))"_Stz);
  }
  pCI->G_vAppendNewChild(pCI->G.p_expr, CK_AS, "__SQ1RESULT"_Stz);
  pCI->G_vAppendNewChild(pCI->G.p_expr, CK_RPNCALC);
  pCI->G_vAppendNewChild(pCI->G.p_rpnCalc,CK_TERM,"$1"_Stz);

  Card_Expr* p_expr_found =
      pCI->Analysis.m_pProjection_prior->pGetExpr(0);

  pCI->G.p_term->Y_vSet(P_Type,szCT_char[Card::eTo_eCT(p_expr_found->pSub_type()->szValue())]);

  pCI->G.p_rpnCalc->Y_vSet(P_Type,szCT_char[Card::eTo_eCT(pCI->G.p_type->szValue())]);

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
    if (pCI->G.p_projection = pMain->pSub_projection())
      break;
  }
  if (! pCI->G.p_projection)
    return;

  pCI->G.p_fold->vAppend(pCI->G.p_projection->pDupCardAndSubCards());
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

    pCI->Sqy.m_sSqyNm  = pCI->Sqy.m_pTerm_sqCall->szValue();
    p_pred->vMark1(MARK_AT);
    bFoundCall = true;

    if (p_pred->bIsValue("exists"_Stz) || p_pred->bIsValue("notExists"_Stz)) {

      RASSERT(p_pred->bIsSubQueryOp());

      pCI->Sqy.m_sSqyArg =  p_pred->pGetTerm(0)->szValue();
      pCI->Sqy.m_sSqyNm  = p_pred->pGetTerm(1)->szValue();
      return C_TERMINATE_RECURSION;
    }

    if (p_pred->bIsValue("in"_Stz) || p_pred->bIsValue("notIn"_Stz)) {
      RASSERT(p_pred->bIsSubQueryOp());
      pCI->Sqy.m_sSqyArg = p_pred->pGetTerm(2)->szValue();
      return C_TERMINATE_RECURSION;
    }

    RASSERT(p_pred->bIsRelnOp());

    return C_TERMINATE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  bool bIgnore = p_pred_ROOT->bPredRecursive(bLambdaFn);

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

  pCI->vRestart();
  while (pCI->G_bEachMain(CK_TABLE)) {

    pCI->G.p_bounds = pCI->G.p_table->pSub_bounds();
    pCI->G.p_index = pCI->G.p_table->pSub_index();
    pCI->G.p_indexJoin = pCI->G.p_table->pSub_indexJoin();
    pCI->G.p_indexList = pCI->G.p_table->pSub_indexList();
    pCI->G.p_keyExists = pCI->G.p_table->pSub_keyExists();
    pCI->G.p_keyNotExists = pCI->G.p_table->pSub_keyNotExists();

    vTrace("MainCard %p : [%lld] %s %s\n"_Stz,
           pCI->G.p_table, pCI->G.p_table->iCardNo(), aCK[pCI->G.p_table->eCard()], pCI->G.p_table->szValue());

    RASSERT(!pCI->G.p_indexJoin);

    pCI->G.p_table->vMorph_table_To_EEop(pCI);
  }

  pCI->vPrintBoard(__LINE__);

  pCI->G_vRestart();
  while (pCI->G_bEachMain(CK_SCAN)) {
    i8 i_Child;
    pCI->G.p_note = _pCard_Note(pCI->G.p_scan->pFindChild(CK_NOTE,i_Child));

    if (i_Child == pCI->G.p_scan->nChildCards() - 1) {
      pCI->G.p_scan->vRemoveChild(pCI->G.p_note);
      DELETE_CARD(pCI->G.p_note);
    }
  }
  pCI->vPrintBoard(__LINE__);

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vIndividuate_indexJoin_filter_b(CardIter* pCI)
{
  _static_

  // todo: xt

  pCI->G.p_indexJoin->vRemoveChild(pCI->G.p_filter);
  pCI->G.pDeck->vInsertAfter(pCI->G.p_filter,
                             pCI->G.p_indexJoin);

  if (! pCI->G.p_filter->pSub_level()) {
    pCI->G.p_filter->vPush(pCI->G.p_indexJoin->pSub_level()->pDupCardAndSubCards());
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vIndividuate_crossJoin_filter(CardIter* pCI)
{
  _static_

  // todo: xt

  pCI->G.p_crossJoin->vRemoveChild(pCI->G.p_filter);
  pCI->G.pDeck->vInsertAfter(pCI->G.p_filter,
                             pCI->G.p_crossJoin);

  if (! pCI->G.p_filter->pSub_level()) {
    pCI->G.p_filter->vPush(pCI->G.p_crossJoin->pSub_level()->pDupCardAndSubCards());
  }
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get board kind                                                             */
/*----------------------------------------------------------------------------*/
eBK Board::eBoardKind(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  return m_eBoardKind;
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
void Board::vClearMarks(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  m_pCard_Deck->vClearMarksRecursive();
}

/*----------------------------------------------------------------------------*/
/* get card subSet                                                            */
/*----------------------------------------------------------------------------*/
Card_Deck* Board::pDeck(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  return m_pCard_Deck;
}


/*----------------------------------------------------------------------------*/
/* pin board onto given ribbon                                                */
/*----------------------------------------------------------------------------*/
void Board::vPinBanners(Ribbon* pRibbon)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  pRibbon->vDrawH('-',true);

  pRibbon->vDrawH('=');

  //pRibbon->vPutTextRight(szXhand(),1);
  //pRibbon->vPutTextRight(szXform(),2);

  s32 sLeftMargin;

  char cPrefix = m_pBench->cPrefix();

  if (' ' == cPrefix)
    sLeftMargin.vSprint("%lld "_Stz, m_i_Board);
  else
  if ('m' == cPrefix || 'w' == cPrefix)
    sLeftMargin.vSprint("%c "_Stz, cPrefix);
  else
  if (m_pBench->bIsFlat() && 0 == m_i_Board)
    sLeftMargin.vSprint("fq "_Stz);
  else
    sLeftMargin.vSprint("%c%lld "_Stz, cPrefix,m_i_Board);

  pRibbon->vInsertCharsV(0,0,sLeftMargin);
  //pRibbon->vDrawV(0,0,"x"_Stz);
}

/*----------------------------------------------------------------------------*/
/* pin board onto given ribbon                                                */
/*----------------------------------------------------------------------------*/
void Board::vPinCards(Ribbon* pRibbon)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  CardCtl* pCardCtl = m_pBench->pOptimiser()->pCardCtl();

  if (! m_pCard_Deck->nItems())
    return;

  pRibbon->vLaneFirst();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Ribbon* pSubRibbon = new Ribbon(pCardCtl,this);

  CardIter xBI(m_pCard_Deck);

  i8 nCount = 0;

  while (xBI.G_bEachMain()) {

    // skip non-printable horizontal lines
    if (pCardCtl->bHeatNext(xBI.G.e_MainCard) &&
        xBI.G.p_MainCard->Y_bHas(pCardCtl->szHorizExclude(xBI.G.e_MainCard))) {

      while (Card* pNextMain = xBI.pNextChild()) {
        eCK eNextCard = pNextMain->eCard();
        if (pCardCtl->bHeatNext(eNextCard))
          break;
        xBI.vForward();
      }

      continue;
    }

    if (! nCount++)
      pSubRibbon->vLaneFirst();
    else
    if (pSubRibbon->i_Char() && pCardCtl->bHeatNext(xBI.G.e_MainCard))
      pSubRibbon->vHeatNext();
    else
      pSubRibbon->vLaneNext();

    /*
    if (pCard->bIsCard(CK_INDEXJOIN) ||
        pCard->bIsCard(CK_SCAN)      ||
        pCard->bIsCard(CK_TABLE)) {
      eLevel = AL_PARTITION;
    }
    else
    if (   pCard->bIsCard(CK_AGGREG)
        || pCard->bIsCard(CK_PROJECT)) {
      Card_Level* p_level = pCard->pSub_level();
      if (p_level)
        eLevel = (eAL)p_level->Y_iGet(P_Level);
    }
    else
    if (pCard->bIsCard(CK_STAGE)) {
      Card_Topology* p_topology = pCard->pSub_topology();
      eYG eStageTopo = (eYG)p_topology->Y_iGet(P_StageTopo);
      switch (eStageTopo) {
        case YG_ALL_PARTITIONS : eLevel = AL_PARTITION; break;
        case YG_ALL_NODES      : eLevel = AL_NODE;      break;
        case YG_ORIGIN_NODE    : eLevel = AL_ORIGIN;    break;
        default                : eLevel = AL_NONE;      break;
      }
    }
*/

    bool bPrune = false;
    eAL eLevel = AL_NONE;
    Card_Level* p_level = xBI.G.p_MainCard->pSub_level();
    if (p_level)
      eLevel = (eAL) p_level->Y_iGet(P_Level);
    xBI.G.p_MainCard->vPrintCardRecursive(pSubRibbon, 0, m_pCard_Deck, bPrune, eLevel, false);
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

  if (! m_pCard_Deck->nItems())
    return;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // line 1
  pRibbon->vSprint("Transform: %02lld Board %lld %s Query %s"_Stz,
                   m_iXform,
                   m_i_Board,
                   m_sBoardNm.szStz(),
                   m_pBench->pOptimiser()->szQueryNm()
                   );

  // line 2
  pRibbon->vSprint("%s"_Stz,szXdesc());

  // line 3
  if (0 == m_i_Board)
    pRibbon->vSprint("%s %s"_Stz,
                     "MainQuery"_Stz,
                     aQD[m_pSubQuery->eDisposition()]);
  else
    pRibbon->vSprint("%s(%lld params) %s caller: board[%lld] %s %s %s"_Stz,
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

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vPrintBoard(Stz* szDescription)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  Ribbon* pRibbon = new Ribbon(m_pBench->pOptimiser()->pCardCtl(),this);

  vPinTitle(pRibbon, m_iXform, szDescription);

  vPinCards(pRibbon);

  vPinBanners(pRibbon);

  pRibbon->vPrintRibbon();

  delete pRibbon;
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
/* get sub-query                                                              */
/*----------------------------------------------------------------------------*/
SubQuery* Board::pSubQuery(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  return m_pSubQuery;
}

/*----------------------------------------------------------------------------*/
/* get description of transformation                                          */
/*----------------------------------------------------------------------------*/
Stz* Board::szXdesc(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  return m_sXdesc;
}

/*----------------------------------------------------------------------------*/
/* set description of transformation                                          */
/*----------------------------------------------------------------------------*/
void Board::vXdesc(Stz* szDescription)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  m_sXdesc = szDescription;
}

/*----------------------------------------------------------------------------*/
/* get transformation                                                         */
/*----------------------------------------------------------------------------*/
i8 Board::iXform(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  return m_iXform;
}

/*----------------------------------------------------------------------------*/
/* get transformation                                                         */
/*----------------------------------------------------------------------------*/
void Board::vXform(i8 iXform)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  m_iXform = iXform;
}

/*----------------------------------------------------------------------------*/
/* get transformation                                                         */
/*----------------------------------------------------------------------------*/
Stz* Board::szXform(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  return m_sXform;
}

/*----------------------------------------------------------------------------*/
/* get transformation                                                         */
/*----------------------------------------------------------------------------*/
void Board::vXform(Stz* szTransformation)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  m_sXform = szTransformation;
}

/*----------------------------------------------------------------------------*/
/* get transformation phase                                                   */
/*----------------------------------------------------------------------------*/
Stz* Board::szXhand(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  return m_sXhand;
}

/*----------------------------------------------------------------------------*/
/* set transformation phase                                                   */
/*----------------------------------------------------------------------------*/
void Board::vXhand(const char* asXhand)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  m_sXhand = asXhand;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
