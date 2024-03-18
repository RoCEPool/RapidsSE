/*============================================================================*/
/* CardIter.cpp --                                                            */
/*============================================================================*/

#include "CardIter.hpp"
#include "Cue.hpp"
#include "OptimiserFun.hpp"
#include "OptimiserCatalog.hpp"

#include "eOT.h"

extern OptimiserCatalog gxOptimiserCatalog;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,CardIter,CN_CARDITER)
__aNewFunction(CardIter,CN_CARDITER)
__vDeleteFunction(CardIter,CN_CARDITER)
__aFooterFunction(CardIter)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
CardIter::CardIter(Bench* pBench)
    : Object(CN_CARDITER)
{
  _precondition_(_MAGIC_,_FOOTER_);

  m_pBench   = pBench;
  m_pCardCtl = nullptr;//m_pBench->pCardCtl();
  m_xSymbol_tableSyn.vInit_Tbl_Syn('a');

  m_i_RecursionDepth = 0;

  G.xKid.vParent(nullptr);

  G.m_bDontDescend = false;

  Flat.m_pSqy_inner     = nullptr;
  Flat.m_p_conj_iqyCall = nullptr;
  Flat.m_p_term_sqyCall = nullptr;
  Flat.m_p_expr_crlKey  = nullptr;

  Flat.m_p_conj_crlKey  = nullptr;
  Flat.m_p_term_crlKey  = nullptr;

  Flat.m_pCorrelation   = nullptr;

  Flat.m_pCopy_inner  = nullptr;

  Flat.m_i_fold         = C_UNKNOWN;
  Flat.m_pExpr_fold     = nullptr;
  Flat.m_pCopy_inner  = nullptr;

//  G.pBoard    = nullptr;
//  G.p_deck     = nullptr;

  m_nAncestors = 0;
  MemLib::vZero(m_apAncestry,sizeof(m_apAncestry),CN_CARDITER);

  vResetMembers();
  vClearRegisters();

  _checkInitialised_(CN_CARDITER);

  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
CardIter::~CardIter(void) noexcept(false)
{
  _precondition_(_MAGIC_,_FOOTER_);

  while (nItems())
    vRestoreMembers();

  RASSERT(0 == nItems());

}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* advance                                                                    */
/*----------------------------------------------------------------------------*/
Card* CardIter::pAdvanceChild(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  G.xKid.bEach();

  return G.xKid.pChild();
}

/*----------------------------------------------------------------------------*/
/* visit all sub-cards of given eToSubs and apply lambda function             */
/*----------------------------------------------------------------------------*/
bool CardIter::bLambdaWalk(Card* pCard, eCK eToSubs,
                               std::function<bool(Card* pCard)> bfLambdaFn)
{
  _precondition_( _MAGIC_, _FOOTER_);

  if (CK_NONE == eToSubs || eToSubs == pCard->eCard()) {
    bool bContinue = bfLambdaFn(pCard);
    if (! bContinue)
      return C_TERMINATE_RECURSION;
  }

  vAncestorPush(pCard);
  Kid xKid(pCard);
  while (xKid.bEach()) {
    xKid.pChild()->vItemise(xKid.i_Child());
    bool bContinue = bLambdaWalk(xKid.pChild(),eToSubs, bfLambdaFn);
    if (! bContinue) {
      vAncestorPop();
      return C_TERMINATE_RECURSION;
    }
    else
      vDebugPoint();
  }

  vAncestorPop();
  return C_CONTINUE_RECURSION;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vAppendChild(eCK eParent, Card* pChild)
{
  _precondition_(_MAGIC_,_FOOTER_);

  Card* pParent = pMember(eParent);

  pParent->vAppendChild(pChild);

  vMember(pChild->eCard(), pChild);

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vAppendNewChild(eCK eParent, eCK eChild)
{
  _precondition_( _MAGIC_, _FOOTER_);

  Card* pParent = pMember(eParent);
  Card* pChild = new Card(eChild,C_UNKNOWN);

  pParent->vAppendChild(pChild);
  vMember(eChild, pChild);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vAppendNewChild(Card* pParent, eCK eChild)
{
  _precondition_(_MAGIC_, _FOOTER_);

  Card* pChild = new Card(eChild,C_UNKNOWN);

  pParent->vAppendChild(pChild);
  vMember(eChild, pChild);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vAppendNewChild(Card* pParent, eCK eChild, Symbol* pSymbol)
{
  _precondition_(_MAGIC_,_FOOTER_);

  Card* pChild = new Card(eChild,C_UNKNOWN);

  pChild->pSymbol()->vApplyCastFacetsPpy(pSymbol);

  pParent->vAppendChild(pChild);
  vMember(eChild, pChild);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vAppendNewChild(Card* pParent, eCK eChild, Symbol xSymbol)
{
  _precondition_(_MAGIC_,_FOOTER_);

  Card* pChild = new Card(eChild,C_UNKNOWN);

  pChild->pSymbol()->vApplyCastFacets(&xSymbol);

  pParent->vAppendChild(pChild);
  vMember(eChild, pChild);
}

/*----------------------------------------------------------------------------*/
/* test ancestry                                                              */
/*----------------------------------------------------------------------------*/
bool CardIter::bAncestor(eCK eCard)
{
  _precondition_(_MAGIC_, _FOOTER_);

  if (pAncestor(eCard))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* get ancestor                                                               */
/*----------------------------------------------------------------------------*/
Card* CardIter::pAncestor(i8 i_Ancestor)
{
  _precondition_(_MAGIC_,_FOOTER_);

  if (i_Ancestor < 0 || i_Ancestor > m_nAncestors)
    return nullptr;

  return m_apAncestry[i_Ancestor];
}

/*----------------------------------------------------------------------------*/
/* find ancestor                                                             */
/*----------------------------------------------------------------------------*/
Card* CardIter::pAncestor(eCK eCard, i8* ai_Ancestor)
{
  _precondition_(_MAGIC_, _FOOTER_);

  for (i8 i_Ancestor = m_nAncestors-1; i_Ancestor >= 0; i_Ancestor--) {
    Card *pCard_ancestor = m_apAncestry[i_Ancestor];

    if (pCard_ancestor->bIsCard(eCard)) {
      if (ai_Ancestor)
        *ai_Ancestor = i_Ancestor;
      return pCard_ancestor;
    }
  }

  if (ai_Ancestor)
    *ai_Ancestor = C_UNKNOWN;

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* dump ancestry                                                             */
/*----------------------------------------------------------------------------*/
void CardIter::vAncestorDump(Card* pCard)
{
  _precondition_(_MAGIC_,_FOOTER_);

  vTrace("^^ Ancestor Dump\n"_Stz);

  for (i8 i_Ancestor = 0; i_Ancestor < m_nAncestors; i_Ancestor++) {
    Card* pCard_ancestor = m_apAncestry[i_Ancestor];
    vTrace("  [%lld] %p %s /%lld\n"_Stz,
           i_Ancestor,
           pCard_ancestor,
           aCK[pCard_ancestor->eCard()],
           pCard_ancestor->iItemise());
  }

  vTrace("$$ Ancestor Dump"_Stz);
  if (pCard)
    vTrace(" %p %s /%lld\n"_Stz,
           pCard,
           aCK[pCard->eCard()],
           pCard->iItemise());
  else
    vTrace("\n"_Stz);

}

/*----------------------------------------------------------------------------*/
/* get ancestor parent                                                        */
/*----------------------------------------------------------------------------*/
Card* CardIter::pAncestorParent(void)
{
  _precondition_(_MAGIC_,_FOOTER_);

  if (! m_nAncestors)
    return nullptr;

  return m_apAncestry[m_nAncestors-1];
}

/*----------------------------------------------------------------------------*/
/* pop ancestor                                                               */
/*----------------------------------------------------------------------------*/
void CardIter::vAncestorPop(void)
{
  _precondition_(_MAGIC_,_FOOTER_);

  if (m_nAncestors <= 0)
    return;

  m_nAncestors--;
}

/*----------------------------------------------------------------------------*/
/* push ancestor                                                              */
/*----------------------------------------------------------------------------*/
void CardIter::vAncestorPush(Card* pCard)
{
  _precondition_(_MAGIC_,_FOOTER_);

   RASSERT(m_nAncestors < C_CARD_ANCESTRY);

  m_apAncestry[m_nAncestors++] = pCard;
}

/*----------------------------------------------------------------------------*/
/* pop ancestor                                                               */
/*----------------------------------------------------------------------------*/
i8 CardIter::nAncestors(void)
{
  _precondition_(_MAGIC_,_FOOTER_);

  return m_nAncestors;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Expr* CardIter::pBackSearch_expr_For_col(Symbol* pSym_term_arg, i8 i_Before)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(pSym_term_arg->bIs_Col());

  Card_Expr* p_expr_col = nullptr;

  for (i8 i_Main = i_Before - 1; i_Main >= 0; i_Main--) {
    Card *pMain = this->G.p_deck->pGetChild(i_Main);
    Card_Emit* p_emit = pMain->pSub_emit();

    if (!p_emit)
      continue;

    i8 nChildren_emit = p_emit->nChildCards();
    for (i8 i_Child = 0; i_Child < nChildren_emit; i_Child++) {
      Card *pChild_prj = p_emit->pGetChild(i_Child);
      if (!pChild_prj->bIsCard(CK_EXPR))
        continue;
      Card_Expr* p_expr_prj = _pCard_Expr(pChild_prj);
      Card_RpnCalc* p_rpnCalc = p_expr_prj->pSub_rpnCalc(C_NOT_NULL);
      if (! p_rpnCalc)
        continue;
      if (1 != p_rpnCalc->nChildCards())
        continue;

      Symbol* pSym_rpnCalc_term0 = p_expr_prj->pGet_term(0)->pSymbol();
      RASSERT(pSym_rpnCalc_term0->bIs_Col());

      if (pSym_rpnCalc_term0->bMatches(pSym_term_arg)) {
        p_expr_col = p_expr_prj;
        break;
      }
    }

    if (p_expr_col)
      break;
  }

  return p_expr_col;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Expr* CardIter::pBackSearch_expr_For_colNm(Symbol* pSym_term, i8 i_Before)
{
  _precondition_( _MAGIC_, _FOOTER_);

  if (C_UNKNOWN == i_Before)
    i_Before = this->i_Child();

  Card_Expr* p_expr_col = nullptr;

  for (i8 i_Main = i_Before - 1; i_Main >= 0; i_Main--) {
    Card *pMain = this->G.p_deck->pGetChild(i_Main);

    if (Card_Emit* p_emit = pMain->pSub_emit()) {
      i8 nChildren_prj = p_emit->nChildCards();
      for (i8 i_Child = 0; i_Child < nChildren_prj; i_Child++) {
        Card *pChild_prj = p_emit->pGetChild(i_Child);
        if (pChild_prj->bIsCard(CK_EXPR)) {
          Card_Expr* p_expr_prj = _pCard_Expr(pChild_prj);
          if (p_expr_prj->pSymbol()->bMatches(pSym_term)) {
            p_expr_col = p_expr_prj;
            break;
          }
        }
      }

    }
  }

  return p_expr_col;
}

/*----------------------------------------------------------------------------*/
/* setup board iteration                                                      */
/*----------------------------------------------------------------------------*/
void CardIter::vBeginBoard(void)
{
  _precondition_(_MAGIC_,_FOOTER_);

  G.i_Board = C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vBeginEach(void)
{
  _precondition_(_MAGIC_, _FOOTER_);

  G.xKid.vBegin();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vBeginEach_reverse(void)
{
  _precondition_(_MAGIC_, _FOOTER_);

  G.xKid.vBegin_reverse();
}

/*----------------------------------------------------------------------------*/
/* get board                                                                  */
/*----------------------------------------------------------------------------*/
Board* CardIter::pBoard(void)
{
  _precondition_(_MAGIC_,_FOOTER_);

  return G.pBoard;
}

/*----------------------------------------------------------------------------*/
/* burst OP                                                                   */
/*----------------------------------------------------------------------------*/
void CardIter::vBurst(Card* pCard)
{
  _precondition_(_MAGIC_,_FOOTER_);

  for (Kid xCard(pCard); xCard.bEach(); /**/)
    vMember(xCard.pChild()->eCard(),xCard.pChild());
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
i8 CardIter::i_Child(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  return G.xKid.i_Child();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardIter::pChild(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  return G.xKid.pChild();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vChild(Card* pChild)
{
  _precondition_(_MAGIC_, _FOOTER_);

  RASSERT(G.xKid.bWithinRange());

  G.xKid.vReposition(G.xKid.pParent()->iFindChild(pChild));
}

/*----------------------------------------------------------------------------*/
/* clear the general M registers                                              */
/*----------------------------------------------------------------------------*/
void CardIter::vClearRegisters(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  MemLib::vZero(G.md_Coord,sizeof(G.md_Coord),CN_CARDITER);

  G.p_Op         = nullptr;
  G.e_Op         = CK_NONE;
  G.i_Op         = C_UNKNOWN;
  G.p_Sub        = nullptr;
  G.p_Sub2       = nullptr;
  G.p_Sub3       = nullptr;

  //Flat.m_p_term_crlKey  = nullptr;

  Flux.m_pArriving  = nullptr;
  Dic.m_p_expr    = nullptr;
  Dic.m_p_key    = nullptr;

  Next.m_pProject   = nullptr;
  Next.m_pEmitted   = nullptr;

  Early.m_pOP_root  = nullptr;
  Early.m_pOP_leaf  = nullptr;
  Early.m_pNeeded_later = nullptr;

  StreamOrder.m_pStreamKey  = nullptr;
  StreamOrder.m_pEmitted = nullptr;

  X.m_pExprList_a = nullptr;
  X.m_pExprList_i = nullptr;
  X.m_pExprList_k = nullptr;

  Flat.m_i_fold = C_UNKNOWN;

}

/*----------------------------------------------------------------------------*/
/* clear the general M registers if they match given pointer                  */
/*----------------------------------------------------------------------------*/
void CardIter::vClearRegistersIf(Card* pCard)
{
  _precondition_(_MAGIC_, _FOOTER_);

  if (pCard == G.p_Op) {
    G.p_Op = nullptr;
    G.e_Op = CK_NONE;
  }
  if (pCard == G.p_Sub) {
    G.p_Sub = nullptr;
  }
  if (pCard == G.p_Sub2) {
    G.p_Sub2 = nullptr;
  }
  if (pCard == G.p_Sub3) {
    G.p_Sub3 = nullptr;
  }
}

/*----------------------------------------------------------------------------*/
/* restart the M iterator                                                     */
/*----------------------------------------------------------------------------*/
void CardIter::vClearRestart(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  this->vBeginEach();

  vResetMembers();
  vClearRegisters();
}

/*----------------------------------------------------------------------------*/
/* cue user                                                                   */
/*----------------------------------------------------------------------------*/
eOA CardIter::eCueUser(String* psPrompt)
{
  _precondition_(_MAGIC_, _FOOTER_);

  TokenVec* pTknVec = nullptr;

  if (m_pCardCtl->bCueDisabled())
    return OA_CONTINUE_TRANSFORMING;

  psPrompt->vConcat(" >> "_Stz);

  m_pCardCtl->vCued(true);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  L_Input:

  s4K sInput;
  Cue::vGetInput(psPrompt->szStz(), sInput);

  if (0 == sInput.nLength())
    return OA_CONTINUE_TRANSFORMING;

  if (sInput.nLength() > 1)
    vDebugPoint();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  RASSERT(! pTknVec);
  pTknVec = AM::TOKENISE::pTokeniseSen(sInput);
  i8 nTokens = pTknVec->nTokens();

  if (0 == nTokens)
    return OA_CONTINUE_TRANSFORMING;

  OptimiserFun* pOptFun = nullptr;
  if (1 == nTokens)
    pOptFun = gxOptimiserCatalog.pFindFun(pTknVec->pGet(0)->szChars(),
                                          nullptr);
  else
    pOptFun = gxOptimiserCatalog.pFindFun(pTknVec->pGet(0)->szChars(),
                                          pTknVec->pGet(1)->szChars());
  if (! pOptFun) {
    vTrace("Unknown command\n"_Stz);
    delete pTknVec;
    pTknVec = nullptr;
    goto L_Input;
  }

  eOA eOptAct = OA_CONTINUE_TRANSFORMING;
  bool bExecuted = pOptFun->m_fpOptFun(m_pBench->pOptimiser(),pTknVec,eOptAct);
  UNUSED(bExecuted);

  delete pTknVec;
  pTknVec = nullptr;

  if (OA_INPUT_AGAIN == eOptAct)
    goto L_Input;

  if (OA_DISABLE_CUEING == eOptAct)
    m_pCardCtl->vCueDisabled(true);

  return eOptAct;
}

/*----------------------------------------------------------------------------*/
/* delete given member; clear-if registers; update M member to null           */
/*----------------------------------------------------------------------------*/
void CardIter::vDeleteMember(Card* pMember)
{
  _precondition_(_MAGIC_, _FOOTER_);

  eCK eMember = pMember->eCard();

  vClearRegistersIf(pMember);
  vMember(eMember, nullptr);

  DELETE_CARD(pMember);
}

/*----------------------------------------------------------------------------*/
/* delete main when given sub is empty                                        */
/*----------------------------------------------------------------------------*/
void CardIter::vDeleteWhenEmptySub(eCK eOp, eCK eSub)
{
  _precondition_(_MAGIC_, _FOOTER_);

  if (!bEachOp(eOp, eSub))
    return;

  vMember(G.e_Op, G.p_Op);

  if (G.p_Sub->nChildCards())
    return;

  vRemoveDeleteMain(eOp);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vDontDescend(bool bDontDescend)
{
  _precondition_(_MAGIC_, _FOOTER_);

  G.m_bDontDescend = bDontDescend;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool CardIter::bEachBoard(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  if (pEachBoard()) {
    Board* pBoard = m_pBench->pGetBoard(G.i_Board);
    vUseBoard(pBoard);
    return true;
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Board* CardIter::pEachBoard(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  i8 i_Board;

  G.pBoard = pEachBoard(i_Board);

  return G.pBoard;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Board* CardIter::pEachBoard(i8& ri_Board)
{
  _precondition_( _MAGIC_, _FOOTER_);

  if (C_UNKNOWN == G.i_Board)
    G.i_Board = 0;
  else
    G.i_Board++;

  if (G.i_Board >= m_pBench->nBoards()) {
    G.pBoard = nullptr;
    ri_Board  = C_UNKNOWN;
    G.i_Board = 0;

    return nullptr;
  }

  G.pBoard = m_pBench->pGetBoard(G.i_Board);
  ri_Board = G.i_Board;

  vUseCard(G.pBoard->pDeck());

  return G.pBoard;
}

/*----------------------------------------------------------------------------*/
/* conditionally step main register to next main and update M member          */
/*----------------------------------------------------------------------------*/
bool CardIter::bEachOp(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(G.xKid.bBegun());
  RASSERT(! G.xKid.bEnded());

  while (G.xKid.bEach()) {
    if (G.xKid.bParentIs(CK_DECK) && ! G.xKid.pChild()->bIsMain())
      continue;

    G.p_Op = G.xKid.pChild();
    vFocusOnOp();
    vMember(G.p_Op->eCard(), G.p_Op);
    return true;
  }

  G.p_Op = nullptr;
  G.e_Op = CK_NONE;
  G.xKid.vEnded();
  return false;
}

/*----------------------------------------------------------------------------*/
/* conditionally step main register to next main and update M member          */
/*----------------------------------------------------------------------------*/
bool CardIter::bEachOp(eCK eOp)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(G.xKid.bBegun());
  RASSERT(! G.xKid.bEnded());

  RASSERT(Card_OP::bIsMain(eOp));

  G.p_Op = nullptr;

  while (G.xKid.bEach()) {

    if (G.xKid.pChild()->eCard() != eOp)
      continue;

    G.p_Op = G.xKid.pChild();
    vFocusOnOp();
    vMember(G.p_Op->eCard(), G.p_Op);
    return true;
  }

  G.p_Op = nullptr;
  G.xKid.vEnded();
  return false;
}

/*----------------------------------------------------------------------------*/
/* conditionally step main register to next main and update M member          */
/*----------------------------------------------------------------------------*/
bool CardIter::bEachOp_foldBreak(eCK eOp)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(G.xKid.bBegun());
  RASSERT(! G.xKid.bEnded());

  RASSERT(Card_OP::bIsMain(eOp));

  G.p_Op = nullptr;

  while (G.xKid.bEach()) {

    if (G.xKid.pChild()->bIsCard(CK_c_FOLD))
      return false;

    if (G.xKid.pChild()->eCard() != eOp)
      continue;

    G.p_Op = G.xKid.pChild();
    vFocusOnOp();
    vMember(G.p_Op->eCard(), G.p_Op);
    return true;
  }

  G.p_Op = nullptr;
  G.xKid.vEnded();
  return false;

}

/*----------------------------------------------------------------------------*/
/* conditionally step main register to next main which has sub and update M   */
/*----------------------------------------------------------------------------*/
bool CardIter::bEachOp(eCK eOp, eCK eSub)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(! Card_OP::bIsMain(eSub) || CK_o_FILTER == eSub);

  while (bEachOp(eOp)) {
    if (! (G.p_Sub = G.p_Op->pFindSubCard(eSub)))
      continue;

    vMember(G.p_Sub->eCard(), G.p_Sub);
    return true;
  }

  RASSERT(G.xKid.bEnded());

  G.p_Sub = nullptr;
  vMember(CK_NONE, nullptr);

  return false;
}

/*----------------------------------------------------------------------------*/
/* conditionally step main register to next main which has sub and update M   */
/*----------------------------------------------------------------------------*/
bool CardIter::bEachOp(eCK eOp, eCK eSub, Stz* szSubFacet)
{
  _precondition_( _MAGIC_, _FOOTER_);

  while (bEachOp(eOp, eSub)) {
    if (G.p_Sub->pSymbol()->bHasFacet(szSubFacet)) {
      RASSERT(G.p_Sub->pSymbol()->bSingleFaceted());
      return true;
    }
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* conditionally step main register to next main which has sub and update M   */
/*----------------------------------------------------------------------------*/
bool CardIter::bEachOp(eCK eOp, eCK eSub1, eCK eSub2)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(! Card_OP::bIsMain(eSub2) || CK_o_FILTER == eSub2);

  while (bEachOp(eOp, eSub1)) {
    if (! (G.p_Sub2 = G.p_Op->pFindSubCard(eSub2)))
      continue;

    vMember(G.p_Sub2->eCard(), G.p_Sub2);
    return true;
  }

  RASSERT(G.xKid.bEnded());

  G.p_Sub2 = nullptr;
  vMember(eSub2, nullptr);

  return false;
}

/*----------------------------------------------------------------------------*/
/* conditionally step main register to next main which has sub and update M   */
/*----------------------------------------------------------------------------*/
bool CardIter::bEachOp(eCK eOp, eCK eSub1, eCK eSub2, eCK eSub3)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(! Card_OP::bIsMain(eSub3));

  while (bEachOp(eOp, eSub1, eSub2)) {
    if (! (G.p_Sub3 = G.p_Op->pFindSubCard(eSub3)))
      continue;

    vMember(G.p_Sub3->eCard(), G.p_Sub3);
    return true;
  }

  RASSERT(G.xKid.bEnded());

  G.p_Sub3 = nullptr;
  vMember(eSub3, nullptr);

  return false;

}

/*----------------------------------------------------------------------------*/
/* conditionally step main register to next main and update M member          */
/*----------------------------------------------------------------------------*/
bool CardIter::bEachOp_reverse(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(!bForwards());

  RASSERT(G.xKid.bBegun());
  RASSERT(! G.xKid.bEnded());

  G.p_Op = nullptr;

  while (G.xKid.bEach_reverse()) {
    if (G.xKid.bParentIs(CK_DECK) && ! G.xKid.pChild()->bIsMain())
      continue;

    G.p_Op = G.xKid.pChild();
    vFocusOnOp();
    vMember(G.p_Op->eCard(), G.p_Op);
    return true;
  }

  G.p_Op = nullptr;
  G.xKid.vEnded();
  return false;
}

/*----------------------------------------------------------------------------*/
/* conditionally step main register to next main and update M member          */
/*----------------------------------------------------------------------------*/
bool CardIter::bEachOp_reverse(eCK eOp)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(!bForwards());

  RASSERT(G.xKid.bBegun());
  RASSERT(! G.xKid.bEnded());

  RASSERT(Card_OP::bIsMain(eOp));

  G.p_Op = nullptr;

  while (G.xKid.bEach_reverse()) {
    if (G.xKid.pChild()->eCard() != eOp)
      continue;

    G.p_Op = G.xKid.pChild();
    vFocusOnOp();
    vMember(G.p_Op->eCard(), G.p_Op);
    return true;
  }

  G.p_Op = nullptr;
  G.xKid.vEnded();
  return false;
}

/*----------------------------------------------------------------------------*/
/* conditionally step main register to next main and update M member          */
/*----------------------------------------------------------------------------*/
bool CardIter::bEachOp_reverse_foldBreak(eCK eOp)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(!bForwards());

  RASSERT(G.xKid.bBegun());
  RASSERT(! G.xKid.bEnded());

  RASSERT(Card_OP::bIsMain(eOp));

  G.p_Op = nullptr;

  while (G.xKid.bEach_reverse()) {

    if (G.xKid.pChild()->bIsCard(CK_c_FOLD))
      return false;

    if (G.xKid.pChild()->eCard() != eOp)
      continue;

    G.p_Op = G.xKid.pChild();
    vFocusOnOp();
    vMember(G.p_Op->eCard(), G.p_Op);
    return true;
  }

  G.p_Op = nullptr;
  G.xKid.vEnded();
  return false;
}

/*----------------------------------------------------------------------------*/
/* conditionally step main register to next main which has sub and update G   */
/*----------------------------------------------------------------------------*/
bool CardIter::bEachOp_reverse(eCK eOp, eCK eSub)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(! Card_OP::bIsMain(eSub));

  while (bEachOp_reverse(eOp)) {
    if (! (G.p_Sub = G.p_Op->pFindSubCard(eSub)))
      continue;

    vFocusOnOp();

    vMember(G.e_Op, G.p_Op);
    vMember(G.p_Sub->eCard(), G.p_Sub);
    return true;
  }

  G.p_Op  = nullptr;
  G.e_Op  = CK_NONE;
  vMember(eOp, nullptr);
  G.p_Sub = nullptr;
  vMember(eSub, nullptr);
  return false;
}

/*----------------------------------------------------------------------------*/
/* conditionally step main register to next main which has sub and update G   */
/*----------------------------------------------------------------------------*/
bool CardIter::bEachOp_reverse_foldBreak(eCK eOp, eCK eSub)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(! Card_OP::bIsMain(eSub) || CK_o_FILTER == eSub);

  while (bEachOp_reverse_foldBreak(eOp)) {
    if (! (G.p_Sub = G.p_Op->pFindSubCard(eSub)))
      continue;

    vFocusOnOp();

    vMember(G.e_Op, G.p_Op);
    vMember(G.p_Sub->eCard(), G.p_Sub);
    return true;
  }

  // did not find card
  G.p_Op  = nullptr;
  G.e_Op  = CK_NONE;
  vMember(eOp, nullptr);
  G.p_Sub = nullptr;
  vMember(eSub, nullptr);
  return false;
}

/*----------------------------------------------------------------------------*/
/* step main register to next main which has sub and update M member          */
/*----------------------------------------------------------------------------*/
bool CardIter::bEachOp_bySub(eCK eSub)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(! Card_OP::bIsMain(eSub));

  while (bEachOp()) {
    if (! (G.p_Sub = G.p_Op->pFindSubCard(eSub)))
      continue;

    vFocusOnOp();

    vMember(G.e_Op, G.p_Op);
    vMember(G.p_Sub->eCard(), G.p_Sub);
    return true;
  }

  G.p_Op  = nullptr;
  G.e_Op  = CK_NONE;
  G.p_Sub = nullptr;
  vMember(eSub, nullptr);
  return false;
}

/*----------------------------------------------------------------------------*/
/* step main register to next main which has sub and update M member          */
/*----------------------------------------------------------------------------*/
bool CardIter::bEachOp_by2Sub(eCK eSub1, eCK eSub2)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(! Card_OP::bIsMain(eSub1));
  RASSERT(! Card_OP::bIsMain(eSub2));

  while (bEachOp()) {
    if (! (G.p_Sub = G.p_Op->pFindSubCard(eSub1)))
      continue;

    if (! (G.p_Sub2 = G.p_Op->pFindSubCard(eSub2)))
      continue;

    vFocusOnOp();

    vMember(G.e_Op, G.p_Op);
    vMember(G.p_Sub->eCard(), G.p_Sub);
    vMember(G.p_Sub2->eCard(), G.p_Sub2);
    return true;
  }

  G.p_Op  = nullptr;
  G.e_Op  = CK_NONE;
  G.p_Sub = nullptr;
  vMember(eSub1, nullptr);
  G.p_Sub2 = nullptr;
  vMember(eSub2, nullptr);
  return false;
}

/*----------------------------------------------------------------------------*/
/* step main register to next main which has sub and update M member          */
/*----------------------------------------------------------------------------*/
bool CardIter::bEachOp_by3Sub(eCK eSub1, eCK eSub2, eCK eSub3)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(! Card_OP::bIsMain(eSub1));
  RASSERT(! Card_OP::bIsMain(eSub2));
  RASSERT(! Card_OP::bIsMain(eSub3));

  while (bEachOp()) {
    if (! (G.p_Sub = G.p_Op->pFindSubCard(eSub1)))
      continue;

    if (! (G.p_Sub2 = G.p_Op->pFindSubCard(eSub2)))
      continue;

    if (! (G.p_Sub3 = G.p_Op->pFindSubCard(eSub3)))
      continue;

    vFocusOnOp();

    vMember(G.e_Op, G.p_Op);
    vMember(G.p_Sub->eCard(), G.p_Sub);
    vMember(G.p_Sub2->eCard(), G.p_Sub2);
    vMember(G.p_Sub3->eCard(), G.p_Sub3);
    return true;
  }

  G.p_Op  = nullptr;
  G.e_Op  = CK_NONE;
  G.p_Sub = nullptr;
  vMember(eSub1, nullptr);
  G.p_Sub2 = nullptr;
  vMember(eSub2, nullptr);
  G.p_Sub3 = nullptr;
  vMember(eSub3, nullptr);
  return false;
}

/*----------------------------------------------------------------------------*/
/* end of phase                                                               */
/*----------------------------------------------------------------------------*/
void CardIter::vEndPhase(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  m_pBench->vEndPhase(this);

  vValidateBench();

  RASSERT(0 == nItems());
}

/*----------------------------------------------------------------------------*/
/* end of transformation                                                      */
/*----------------------------------------------------------------------------*/
void CardIter::vEndTransform(i8 iLineNo)
{
  _precondition_( _MAGIC_, _FOOTER_);

  if (G.pBoard) {

    G.p_deck->vCheckCorrectness();

    if (!  G.pBoard->pState()->bHasName(aXF[m_pBench->eXform()]))
      G.pBoard->pState()->vInsert(aXF[m_pBench->eXform()]);

    for (Kid xDeck(G.p_deck); xDeck.bEach(); /**/) {
      xDeck.pChild()->vEnsureFirst(CK_s_LEVEL);
      xDeck.pChild()->vEnsureLast(CK_s_CONSTRAINT);
      xDeck.pChild()->vEnsureLast(CK_s_NEEDED);
    }

    vPrintBoard(m_pBench->szXfunc()->azChar(),
                m_pBench->szXfile()->azChar(),
                iLineNo);

  }

  vValidateBench();

  G.xKid.vEnded();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_OP* CardIter::pFind_from_By_tblSyn(Symbol* pSym_tblSyn, i8 i_Before)
{
  _precondition_( _MAGIC_, _FOOTER_);

  if (C_UNKNOWN == i_Before)
    i_Before = i_Child();

  for (i8 i_Main = i_Before - 1; i_Main >= 0; i_Main--) {
    Card_OP *p_OP = (Card_OP*)G.p_deck->pGetChild(i_Main);
    if (p_OP->bIsCard(CK_o_FROM)) {
      Card_Table* p_table_test = p_OP->pSub_table();
      //p_table_test->vDump(__func__);
      if (p_table_test->bSame(pSym_tblSyn)) {
        return p_OP;
      }
    }
  }

  return nullptr;
}
/*----------------------------------------------------------------------------*/
/* focus on main                                                              */
/*----------------------------------------------------------------------------*/
void CardIter::vFocusOnOp(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

//  if (! G.p_Op->bIsMain())
//    return;
//
//  RASSERT(G.p_Op->bIsMain());

  MemLib::vZero(G.md_Coord, sizeof(G.md_Coord), CN_CARDITER);
  StrLib::vCopy(G.md_Coord, sizeof(G.md_Coord),G.p_Op->szCoord()->azChar());

  G.e_Op = G.p_Op->eCard();
  G.i_Op = this->i_Child();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool CardIter::bForwards(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  return G.xKid.bForwards();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vForwards(bool bForward)
{
  _precondition_( _MAGIC_, _FOOTER_);

  G.xKid.vForwards(bForward);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardIter::pChild(i8 i_Child)
{
  _precondition_(_MAGIC_, _FOOTER_);

  Card* pChild = G.xKid.pParent()->pGetChild(i_Child);

  return pChild;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vInsertAfter(Card* pCard, Card* pAfter)
{
  _precondition_(_MAGIC_, _FOOTER_);

  G.p_deck->vInsertChildAfter(pCard, pAfter);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vInsertBefore(Card* pCard, Card* pBefore)
{
  _precondition_(_MAGIC_, _FOOTER_);

  G.p_deck->vInsertChildBefore(pCard, pBefore);
}

/*----------------------------------------------------------------------------*/
/* fetch M member by decoding given eMember                                   */
/*----------------------------------------------------------------------------*/
Card* CardIter::pMember(eCK eMember)
{
  _precondition_(_MAGIC_, _FOOTER_);

  switch (eMember) {

    case CK_c_ABSJOIN:      return G.p_absJoin;
    case CK_ADDRESS:        return G.p_address;
    case CK_o_AGGREG:       return G.p_aggreg;
    case CK_ANDCONJ:        return G.p_andConj;

    case CK_BOOLPRED:       return G.p_boolPred;
    case CK_BOUND:          return G.p_bound;
    case CK_BOUNDS:         return G.p_bounds;

    case CK_o_CALL:         return G.p_call;
    case CK_CALLER:         return G.p_caller;
    case CK_CALLOP:         return G.p_callOp;
    case CK_o_CAST:         return G.p_cast;
    case CK_CLAUSE:         return G.p_clause;
    case CK_c_COMBINED:     return G.p_combined;
    case CK_s_CONSTRAINT:   return G.p_constraint;
    case CK_CORRCOLS:       return G.p_corrCols;
    case CK_o_CORRELATION:  return G.p_correlation;
    case CK_COSTJOINING:    return G.p_costJoining;
    case CK_COSTNETWORK:    return G.p_costNetwork;
    case CK_COSTSTREAM:     return G.p_costQuery;
    case CK_COSTUNICAST:    return G.p_costUnicast;
    case CK_COUNT:          return G.p_count;
    case CK_o_CROSSJOIN:    return G.p_crossJoin;

    case CK_DECISION:       return G.p_decision;
    case CK_DIRECTION:      return G.p_direction;
    case CK_DISPOSITION:    return G.p_disposition;

    case CK_t_EMIT:         return G.p_emit;
    case CK_EXPR:           return G.p_expr;

    case CK_o_FILTER:       return G.p_filter;
    case CK_FILTPRED:       return G.p_filtPred;
    case CK_FILTPREDLIST:   return G.p_filtPredList;
    case CK_c_FLAT:         return G.p_flat;
    case CK_c_FOLD:         return G.p_fold;
    case CK_o_FROM:         return G.p_from;

    case CK_o_GBYAGG:       return G.p_gbyAgg;

    case CK_o_HASH:         return G.p_hash;
    case CK_s_HASHING:      return G.p_hashing;
    case CK_o_HAVING:       return G.p_having;
    case CK_t_HSHCOL:       return G.p_hshCol;
    case CK_s_HSHCOLLIST:   return G.p_hshColList;

    case CK_IDXCOL:         return G.p_idxCol;
    case CK_IDXCOLLIST:     return G.p_idxColList;
    case CK_INDEX:          return G.p_index;
    case CK_o_INDEXJOIN:    return G.p_indexJoin;
    case CK_t_INDEXLIST:    return G.p_indexList;
    case CK_INDEXNM:        return G.p_indexNm;
    case CK_o_INDEXSCAN:    return G.p_indexScan;
    case CK_o_INFLATE:      return G.p_inflate;
    case CK_s_INJECTABLE:   return G.p_injectable;

    case CK_JOINCAST:       return G.p_joinCast;
    case CK_JOINESTIMATE:   return G.p_joinEstimate;
    case CK_JOINORDER:      return G.p_joinOrder;
    case CK_JOINORDERING:   return G.p_joinOrdering;
    case CK_JOINPREDLIST:   return G.p_joinPredList;

    case CK_s_KEY:          return G.p_key;
    case CK_KEYCELLS:       return G.p_keyCells;
    case CK_o_KEYEXISTS:    return G.p_keyExists;
    case CK_o_KEYNOTEXISTS: return G.p_keyNotExists;

    case CK_s_LEVEL:        return G.p_level;
    case CK_o_LIMIT:        return G.p_limit;

    case CK_MAP:            return G.p_map;
    case CK_s_MAPPING:      return G.p_mapping;

    case CK_s_NEEDED:       return G.p_needed;
    case CK_NOTE:           return G.p_note;
    case CK_NOTCONJ:        return G.p_notConj;

    case CK_ON:             return G.p_on;
    case CK_ONCLAUSE:       return G.p_onClause;
    case CK_o_ORDAGG:       return G.p_ordAgg;
    case CK_ORCONJ:         return G.p_orConj;
    case CK_OUTERCOLNM:     return G.p_outerColNm;
    case CK_OUTERJOIN:      return G.p_outerJoin;

    case CK_PAYLOADCELLS:   return G.p_payloadCells;
    case CK_o_PROJECT:      return G.p_project;

    case CK_RELNOP:         return G.p_relnOp;
    case CK_REQUIRED:       return G.p_required;
    case CK_RLNCONJ:        return G.p_rlnConj;
    case CK_ROWCOUNT:       return G.p_rowCount;
    case CK_ROWSIZE:        return G.p_rowSize;
    case CK_RPNCALC:        return G.p_rpnCalc;

    case CK_o_SCAN:         return G.p_scan;
    case CK_SECTION:        return G.p_section;
    case CK_c_SEND:         return G.p_send;
    case CK_SPARE:          return G.p_spare;
    case CK_o_SORT:         return G.p_sort;
    case CK_SQYCALLS:       return G.p_sqyCalls;
    case CK_SQYINFO:        return G.p_sqyInfo;
    case CK_SQYNO:          return G.p_sqyNo;
    case CK_c_STAGE:        return G.p_stage;
    case CK_s_STREAM:       return G.p_stream;
    case CK_STREAMHEAD:     return G.p_streamHead;
    case CK_STREAMJOIN:     return G.p_streamJoin;
    case CK_STREAMKEY:      return G.p_streamKey;
    case CK_STREAMPER:      return G.p_streamPer;
    case CK_STREAMPTN:      return G.p_streamPtn;
    case CK_STREAMSET:      return G.p_streamSet;
    case CK_SUBQUERY:       return G.p_subQuery;
    case CK_SUBSET:         return G.p_subSet;
    case CK_SYNTHESIS:      return G.p_synthesis;

    case CK_s_TABLE:        return G.p_table;
    case CK_t_TABLEINFO:    return G.p_tableInfo;
    case CK_TABLELIST:      return G.p_tableList;
    case CK_t_TABLENM:      return G.p_tableNm;
    case CK_TABLEPTN:       return G.p_tablePtn;
    case CK_TABLESIZE:      return G.p_tableSize;
    case CK_TERM:           return G.p_term;
    case CK_o_TOP:          return G.p_top;
    case CK_TOPOLOGY:       return G.p_topology;

    case CK_UNIQUE:         return G.p_unique;

    case CK_o_WHERE:        return G.p_where;

    case CK_XCARD:          return G.p_xcard;

    default:
      NEVER_REACH
  }
}

/*----------------------------------------------------------------------------*/
/* set member card                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vMember(eCK eMember, Card* pCard)
{
  _precondition_(_MAGIC_, _FOOTER_);

  RASSERT(!pCard || pCard->bIsCard(eMember));

  switch (eMember) {

    case CK_NONE: break;

    case CK_c_ABSJOIN:     G.p_absJoin           = _pCard_AbsJoin(pCard); break;
    case CK_ADDRESS:       G.p_address           = _pCard_Address(pCard); break;
    case CK_o_AGGREG:      G.p_aggreg            = _pCard_Aggreg(pCard);  break;
    case CK_ANDCONJ:       G.p_andConj           = _pCard_AndConj(pCard);break;

    case CK_BOOLPRED:      G.p_boolPred          = _pCard_BoolPred(pCard); break;
    case CK_BOUND:         G.p_bound             = _pCard_Bound(pCard); break;
    case CK_BOUNDS:        G.p_bounds            = _pCard_Bounds(pCard); break;

    case CK_o_CALL:        G.p_call              = _pCard_Call(pCard); break;
    case CK_CALLER:        G.p_caller            = _pCard_Caller(pCard); break;
    case CK_CALLOP:        G.p_callOp            = _pCard_CallOp(pCard); break;
    case CK_o_CAST:        G.p_cast              = _pCard_Cast(pCard); break;
    case CK_CLAUSE:        G.p_clause            = _pCard_Clause(pCard); break;

    case CK_c_COMBINED:    G.p_combined          = _pCard_Combined(pCard); break;
    case CK_s_CONSTRAINT:  G.p_constraint        = _pCard_Constraint(pCard); break;
    case CK_CORRCOLS:      G.p_corrCols          = _pCard_CorrCols(pCard); break;
    case CK_o_CORRELATION: G.p_correlation       = _pCard_Correlation(pCard); break;
    case CK_COSTJOINING:   G.p_costJoining       = _pCard_CostJoining(pCard); break;
    case CK_COSTNETWORK:   G.p_costNetwork       = _pCard_CostNetwork(pCard); break;
    case CK_COSTSTREAM:    G.p_costQuery         = _pCard_CostStream(pCard); break;
    case CK_COSTUNICAST:   G.p_costUnicast       = _pCard_CostUnicast(pCard); break;
    case CK_COUNT:         G.p_count             = _pCard_Count(pCard); break;
    case CK_o_CROSSJOIN:   G.p_crossJoin         = _pCard_CrossJoin(pCard); break;

    case CK_DECK:          G.p_deck               = _pCard_Deck(pCard); break;
    case CK_DECISION:      G.p_decision          = _pCard_Decision(pCard); break;
    case CK_DIRECTION:     G.p_direction         = _pCard_Direction(pCard); break;
    case CK_DISPOSITION:   G.p_disposition       = _pCard_Disposition(pCard); break;

    case CK_t_EMIT:        G.p_emit              = _pCard_Emit(pCard); break;
    case CK_EXPR:          G.p_expr              = _pCard_Expr(pCard); break;

    case CK_o_FILTER:      G.p_filter            = _pCard_Filter(pCard); break;
    case CK_FILTPRED:      G.p_filtPred          = _pCard_FiltPred(pCard); break;
    case CK_FILTPREDLIST:  G.p_filtPredList      = _pCard_FiltPredList(pCard); break;
    case CK_c_FLAT:        G.p_flat              = _pCard_Flat(pCard); break;
    case CK_c_FOLD:        G.p_fold              = _pCard_Fold(pCard); break;
    case CK_o_FROM:        G.p_from              = _pCard_From(pCard); break;

    case CK_o_GBYAGG:      G.p_gbyAgg            = _pCard_GbyAgg(pCard); break;

    case CK_o_HASH:        G.p_hash              = _pCard_Hash(pCard); break;
    case CK_s_HASHING:     G.p_hashing           = _pCard_Hashing(pCard); break;
    case CK_o_HAVING:      G.p_having            = _pCard_Having(pCard); break;
    case CK_t_HSHCOL:      G.p_hshCol            = _pCard_HshCol(pCard); break;
    case CK_s_HSHCOLLIST:  G.p_hshColList        = _pCard_HshColList(pCard); break;

    case CK_IDXCOL:        G.p_idxCol            = _pCard_IdxCol(pCard); break;
    case CK_IDXCOLLIST:    G.p_idxColList        = _pCard_IdxColList(pCard); break;
    case CK_INDEX:         G.p_index             = _pCard_Index(pCard); break;
    case CK_o_INDEXJOIN:   G.p_indexJoin         = _pCard_IndexJoin(pCard); break;
    case CK_t_INDEXLIST:   G.p_indexList         = _pCard_IndexList(pCard); break;
    case CK_INDEXNM:       G.p_indexNm           = _pCard_IndexNm(pCard); break;
    case CK_o_INDEXSCAN:   G.p_indexScan         = _pCard_IndexScan(pCard); break;
    case CK_s_INJECTABLE:  G.p_injectable        = _pCard_Injectable(pCard); break;
    case CK_o_INFLATE:     G.p_inflate           = _pCard_Inflate(pCard); break;

    case CK_JOINCAST:      G.p_joinCast          = _pCard_JoinCast(pCard); break;
    case CK_JOINESTIMATE:  G.p_joinEstimate      = _pCard_JoinEstimate(pCard); break;
    case CK_JOINORDER:     G.p_joinOrder         = _pCard_JoinOrder(pCard); break;
    case CK_JOINORDERING:  G.p_joinOrdering      = _pCard_JoinOrdering(pCard); break;
    case CK_JOINPREDLIST:  G.p_joinPredList      = _pCard_JoinPredList(pCard); break;

    case CK_s_KEY:         G.p_key               = _pCard_Key(pCard); break;
    case CK_KEYCELLS:      G.p_keyCells          = _pCard_KeyCells(pCard); break;
    case CK_o_KEYEXISTS:   G.p_keyExists         = _pCard_KeyExists(pCard); break;
    case CK_o_KEYNOTEXISTS:G.p_keyNotExists      = _pCard_KeyNotExists(pCard); break;

    case CK_s_LEVEL:       G.p_level             = _pCard_Level(pCard); break;
    case CK_o_LIMIT:       G.p_limit             = _pCard_Limit(pCard); break;

    case CK_MAP:           G.p_map               = _pCard_Map(pCard); break;
    case CK_s_MAPPING:     G.p_mapping           = _pCard_Mapping(pCard); break;

    case CK_s_NEEDED:      G.p_needed            = _pCard_Needed(pCard); break;
    case CK_NOTE:          G.p_note              = _pCard_Note(pCard); break;
    case CK_NOTCONJ:       G.p_notConj           = _pCard_NotConj(pCard);break;

    case CK_ON:            G.p_on                = _pCard_On(pCard); break;
    case CK_ONCLAUSE:      G.p_onClause          = _pCard_OnClause(pCard); break;
    case CK_ORCONJ:        G.p_orConj            = _pCard_OrConj(pCard);break;
    case CK_o_ORDAGG:      G.p_ordAgg            = _pCard_OrdAgg(pCard); break;
    case CK_OUTERCOLNM:    G.p_outerColNm        = _pCard_OuterColNm(pCard); break;
    case CK_OUTERJOIN:     G.p_outerJoin         = _pCard_OuterJoin(pCard); break;

    case CK_PAYLOADCELLS:  G.p_payloadCells      = _pCard_PayloadCells(pCard); break;
    case CK_o_PROJECT:     G.p_project           = _pCard_Project(pCard); break;

    case CK_RELNOP:        G.p_relnOp            = _pCard_RelnOp(pCard); break;
    case CK_REQUIRED:      G.p_required          = _pCard_Required(pCard); break;
    case CK_RLNCONJ:       G.p_rlnConj           = _pCard_RlnConj(pCard);break;
    case CK_ROWCOUNT:      G.p_rowCount          = _pCard_RowCount(pCard); break;
    case CK_ROWSIZE:       G.p_rowSize           = _pCard_RowSize(pCard); break;
    case CK_RPNCALC:       G.p_rpnCalc           = _pCard_RpnCalc(pCard); break;

    case CK_o_SCAN:        G.p_scan              = _pCard_Scan(pCard); break;
    case CK_SECTION:       G.p_section           = _pCard_Section(pCard); break;
    case CK_c_SEND:        G.p_send              = _pCard_Send(pCard); break;
    case CK_o_SORT:        G.p_sort              = _pCard_Sort(pCard); break;
    case CK_SPARE:         G.p_spare             = _pCard_Spare(pCard); break;
    case CK_SQYCALLS:      G.p_sqyCalls          = _pCard_SqyCalls(pCard); break;
    case CK_SQYINFO:       G.p_sqyInfo           = _pCard_SqyInfo(pCard); break;
    case CK_SQYNO:         G.p_sqyNo             = _pCard_SqyNo(pCard); break;
    case CK_c_STAGE:       G.p_stage             = _pCard_Stage(pCard); break;
    case CK_s_STREAM:      G.p_stream            = _pCard_Stream(pCard); break;
    case CK_STREAMHEAD:    G.p_streamHead        = _pCard_StreamHead(pCard); break;
    case CK_STREAMJOIN:    G.p_streamJoin        = _pCard_StreamJoin(pCard); break;
    case CK_STREAMKEY:     G.p_streamKey         = _pCard_StreamKey(pCard); break;
    case CK_STREAMPER:     G.p_streamPer         = _pCard_StreamPer(pCard); break;
    case CK_STREAMPTN:     G.p_streamPtn         = _pCard_StreamPtn(pCard); break;
    case CK_STREAMSET:     G.p_streamSet         = _pCard_StreamSet(pCard); break;
    case CK_SUBQUERY:      G.p_subQuery          = _pCard_SubQuery(pCard); break;
    case CK_SUBSET:        G.p_subSet            = _pCard_SubSet(pCard); break;
    case CK_SYNTHESIS:     G.p_synthesis         = _pCard_Synthesis(pCard); break;

    case CK_s_TABLE:       G.p_table             = _pCard_Table(pCard); break;
    case CK_t_TABLEINFO:   G.p_tableInfo         = _pCard_TableInfo(pCard); break;
    case CK_TABLELIST:     G.p_tableList         = _pCard_TableList(pCard); break;
    case CK_t_TABLENM:     G.p_tableNm           = _pCard_TableNm(pCard); break;
    case CK_TABLEPTN:      G.p_tablePtn          = _pCard_TablePtn(pCard); break;
    case CK_TABLESIZE:     G.p_tableSize         = _pCard_TableSize(pCard); break;
    case CK_TERM:          G.p_term              = _pCard_Term(pCard); break;
    case CK_o_TOP:         G.p_top               = _pCard_Top(pCard); break;
    case CK_TOPOLOGY:      G.p_topology          = _pCard_Topology(pCard); break;

    case CK_UNIQUE:        G.p_unique            = _pCard_Unique(pCard); break;

    case CK_o_WHERE:       G.p_where             = _pCard_Where(pCard); break;

    case CK_XCARD:         G.p_xcard             = _pCard_XCard(pCard); break;

    default:
      NEVER_REACH
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool CardIter::bMore(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(G.xKid.bWithinRange());

  if (G.xKid.bMore())
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* create a new card and optionally set member                                */
/*----------------------------------------------------------------------------*/
void CardIter::vNewMember(eCK eMember)
{
  _precondition_(_MAGIC_, _FOOTER_);

  Card* pNew = new Card(eMember,C_UNKNOWN);

  vMember(eMember, pNew);
}

/*----------------------------------------------------------------------------*/
/* step to next main and set member                                           */
/*----------------------------------------------------------------------------*/
void CardIter::vNewMember(eCK eMember, Symbol xSymbol)
{
  _precondition_(_MAGIC_, _FOOTER_);

  Card* pNew = new Card(eMember,C_UNKNOWN);

  vMember(eMember, pNew);

  pNew->pSymbol()->vApplyCastFacets(&xSymbol);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardIter::pPeekNext(void)
{
  _precondition_(_MAGIC_, _FOOTER_);

  i8 i_Next = G.xKid.i_Child() + 1;

  if (i_Next >= G.xKid.pParent()->nChildCards())
    return nullptr;

  Card* pNextMain = G.xKid.pParent()->pGetChild(i_Next);

  return pNextMain;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardIter::pPeekNextIf(eCK eCard)
{
  _precondition_(_MAGIC_, _FOOTER_);

  Card* pNextMain = this->pPeekNext();
  if (! pNextMain || eCard != pNextMain->eCard())
    return nullptr;

  return pNextMain;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardIter::pPeekPrev(void)
{
  _precondition_(_MAGIC_, _FOOTER_);

  Card* pChild = G.xKid.pPrev();

  return pChild;
}

/*----------------------------------------------------------------------------*/
/* position on given card of sequence                                         */
/*----------------------------------------------------------------------------*/
bool CardIter::bPositionOn(eCK eKind_on, i8 /* eCK */  iKind_First, ...)
{
  _precondition_(_MAGIC_, _FOOTER_);

  Card* pCard_found = nullptr;

  va_list xaArgs;
  va_start(xaArgs,iKind_First);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // find first card

  eCK eKind_first = (eCK)iKind_First;
  while (bEachOp()) {
    if (eKind_first == G.e_Op)
      break;
  }

  if (! G.e_Op) {
    va_end(xaArgs);
    return false;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // each card in sequence

  eCK eKind_each;

  if (eKind_on == G.e_Op) {
    pCard_found = G.p_Op;
  }

  for (eKind_each = (eCK)va_arg(xaArgs, i8);
       CK_NONE != eKind_each;
       eKind_each = (eCK)va_arg(xaArgs, i8)) {

    if (! bEachOp()) {
      va_end(xaArgs);
      return false;
    }

    if (! pCard_found && eKind_on == G.e_Op) {
      pCard_found = G.p_Op;
    }
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // did we find card in valid sequence?

  va_end(xaArgs);

  if (pCard_found) {
    vChild(pCard_found);
    G.p_Op = G.xKid.pChild();
    vFocusOnOp();
    return true;
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* print the board                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vPrintBoard(const char* azFunc, const char* azFile, i8 iLineNo, bool bForce)
{
  _precondition_(_MAGIC_, _FOOTER_);

  if (bForce)
    vDebugPoint();

  if (! AM::JOB::bHasTag(C_OPTIMISER_TRACE) && ! bForce)
    return;

  G.pBoard->vRenumberCards();

  G.pBoard->vPrintBoard(this);


  s32 sOT;

  bool bPrinted = false;
  i8 nPrinted = 0;
  for (i8 i_OT = 0; i_OT < OT_ENUMS; i_OT++) {
    Stz *szOptimisation = aOT[i_OT];

    if (! G.pBoard->pState()->bHasName(szOptimisation))
      continue;

    vTrace("%lls "_Stz,szOptimisation);
    bPrinted = true;
    if (++nPrinted == 8) {
      vTrace("\n"_Stz);
      bPrinted = false;
    }
  }
  if (bPrinted)
    vTrace("\n"_Stz);


  s256 sWhere;
    sWhere.vCatSprint("EndTransform: %s %s() %s:%lld"_Stz,
                      aXF[m_pBench->eXform()],
                      azFunc,
                      azFile,
                      iLineNo);

  vTrace("%s\n"_Stz, sWhere.azChar());
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vPushChild(Card* pParent, eCK eChild)
{
  _precondition_(_MAGIC_, _FOOTER_);

  Card* pChild = new Card(eChild,C_UNKNOWN);

  pParent->vPushChild(pChild);
  vMember(eChild, pChild);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool CardIter::bRecurseActive(void)
{
  _precondition_(_MAGIC_,_FOOTER_);

  CardIterMembers* pPeg = pPeek(0);
  if (! pPeg)
    return false;
  if (pPeg->m_i_RecursionDepth != m_i_RecursionDepth)
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vRecurseBegin(Card* pCard)
{
  _precondition_(_MAGIC_, _FOOTER_);

  vSaveMembers();

  m_i_RecursionDepth++;
  G.m_i_RecursionDepth = m_i_RecursionDepth;

  vUseCard(pCard);
  vDontDescend(false);

  G.xKid.vForwards(true);

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardIter::pRecurseEach(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // descend into current child

  if (G.p_Op && (! G.m_bDontDescend) ) {
    if (G.p_Op->nChildCards()) {

      vSaveMembers();
      vUseCard(G.p_Op);
      bEachOp();

      return G.p_Op;
    }
  }

  G.m_bDontDescend = false;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // visit next child

  if (bEachOp())
    return G.p_Op;

  while (bRecurseActive()) {
    vRestoreMembers();
    if (bEachOp())
      return G.p_Op;
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardIter::pRecurseEach(eCK eCard)
{
  _precondition_( _MAGIC_, _FOOTER_);

  while (Card* p_card = pRecurseEach())
    if (p_card->bIsCard(eCard))
      return p_card;

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vRecurseEnd(void)
{
  _precondition_(_MAGIC_, _FOOTER_);

  vRestoreMembers();

  m_i_RecursionDepth--;

  RASSERT(G.m_i_RecursionDepth == m_i_RecursionDepth);
}

/*----------------------------------------------------------------------------*/
/* remove main card from deck and delete it; clear M member to null           */
/*----------------------------------------------------------------------------*/
void CardIter::vRemoveDeleteMain(eCK eCrossCheck)
{
  _precondition_(_MAGIC_, _FOOTER_);

  RASSERT(G.xKid.bWithinRange());
  RASSERT(eCrossCheck == G.xKid.eChild());

  Card* pChild = G.xKid.pChild();
  G.xKid.pParent()->vRemoveChild(pChild);
  delete pChild;

  G.xKid.vBackstep();

  G.p_Op = nullptr;
  G.e_Op = CK_NONE;

  vMember(eCrossCheck, nullptr);
}

/*----------------------------------------------------------------------------*/
/* clear the M members                                                        */
/*----------------------------------------------------------------------------*/
void CardIter::vResetMembers(void)
{
  _precondition_( _MAGIC_, _FOOTER_);


  G.p_absJoin          = nullptr;
  G.p_address          = nullptr;
  G.p_aggreg           = nullptr;
  G.p_andConj          = nullptr;

  G.p_boolPred         = nullptr;
  G.p_bound            = nullptr;
  G.p_bounds           = nullptr;

  G.p_call             = nullptr;
  G.p_cast             = nullptr;
  G.p_rlnConj          = nullptr;
  G.p_combined         = nullptr;
  G.p_constraint       = nullptr;
  G.p_corrCols         = nullptr;
  G.p_correlation      = nullptr;
  G.p_costJoining      = nullptr;
  G.p_costNetwork      = nullptr;
  G.p_costQuery        = nullptr;
  G.p_costUnicast      = nullptr;
  G.p_count            = nullptr;
  G.p_crossJoin        = nullptr;

  G.p_decision         = nullptr;
  G.p_direction        = nullptr;
  G.p_disposition      = nullptr;

  G.p_emit             = nullptr;
  G.p_expr             = nullptr;

  G.p_filter           = nullptr;
  G.p_filtPred         = nullptr;
  G.p_filtPredList     = nullptr;
  G.p_flat             = nullptr;
  G.p_fold             = nullptr;
  G.p_from             = nullptr;

  G.p_gbyAgg           = nullptr;

  G.p_hash             = nullptr;
  G.p_hashing          = nullptr;
  G.p_having           = nullptr;
  G.p_hshCol           = nullptr;
  G.p_hshColList       = nullptr;

  G.p_idxCol           = nullptr;
  G.p_idxColList       = nullptr;
  G.p_index            = nullptr;
  G.p_indexJoin        = nullptr;
  G.p_indexList        = nullptr;
  G.p_indexNm          = nullptr;
  G.p_indexScan        = nullptr;
  G.p_inflate          = nullptr;
  G.p_injectable       = nullptr;

  G.p_joinCast         = nullptr;
  G.p_joinOrder        = nullptr;
  G.p_joinOrdering     = nullptr;
  G.p_joinPredList     = nullptr;

  G.p_key              = nullptr;
  G.p_keyCells         = nullptr;
  G.p_keyExists        = nullptr;
  G.p_keyNotExists     = nullptr;

  G.p_level            = nullptr;
  G.p_limit            = nullptr;

  G.p_joinEstimate     = nullptr;

  G.p_map              = nullptr;
  G.p_mapping          = nullptr;

  G.p_needed           = nullptr;
  G.p_note             = nullptr;
  G.p_notConj          = nullptr;

  G.p_on               = nullptr;
  G.p_onClause         = nullptr;
  G.p_ordAgg           = nullptr;
  G.p_orConj           = nullptr;
  G.p_outerColNm       = nullptr;
  G.p_outerJoin        = nullptr;

  G.p_payload          = nullptr;
  G.p_payloadCells     = nullptr;
  G.p_project          = nullptr;

  G.p_relnOp           = nullptr;
  G.p_required         = nullptr;
  G.p_rowCount         = nullptr;
  G.p_rowSize          = nullptr;
  G.p_rpnCalc          = nullptr;

  G.p_scan             = nullptr;
  G.p_section          = nullptr;
  G.p_send             = nullptr;
  G.p_spare            = nullptr;
  G.p_sort             = nullptr;
  G.p_sqyCalls         = nullptr;
  G.p_sqyInfo          = nullptr;
  G.p_sqyNo            = nullptr;
  G.p_stream           = nullptr;
  G.p_streamKey        = nullptr;
  G.p_streamPer        = nullptr;
  G.p_streamPtn        = nullptr;
  G.p_streamSet        = nullptr;
  G.p_stage            = nullptr;
  G.p_streamHead       = nullptr;
  G.p_streamJoin       = nullptr;
  G.p_subQuery         = nullptr;
  G.p_subSet           = nullptr;
  G.p_synthesis        = nullptr;

  G.p_table            = nullptr;
  G.p_tableInfo        = nullptr;
  G.p_tableList        = nullptr;
  G.p_tableNm          = nullptr;
  G.p_tablePtn         = nullptr;
  G.p_tableSize        = nullptr;
  G.p_term             = nullptr;
  G.p_top              = nullptr;
  G.p_topology         = nullptr;

  G.p_unique           = nullptr;

  G.p_where            = nullptr;

  G.p_xcard            = nullptr;
}

/*----------------------------------------------------------------------------*/
/* resolve prior emit (find right-to-left)                                    */
/*----------------------------------------------------------------------------*/
Card_Emit* CardIter::pResolvePrior_emit(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  Card_Emit* p_emit = Card_Emit::pResolvePrior_emit(this);

  return p_emit;
}

/*----------------------------------------------------------------------------*/
/* resolve prior expr of given term (find right-to-left)                      */
/*----------------------------------------------------------------------------*/
Card_Expr* CardIter::pResolvePrior_expr(Card_Term* p_term)
{
  _precondition_( _MAGIC_, _FOOTER_);

  Card_Emit* p_arriving = pResolvePrior_emit();
  if (! p_arriving)
    return nullptr;

  //RASSERT(p_arriving == this->Flux.m_pArriving);

  Card_Expr* p_expr_resolved = p_arriving->pResolve_expr(p_term);
  return p_expr_resolved;
}

/*----------------------------------------------------------------------------*/
/* step main register next main and update M member                           */
/*----------------------------------------------------------------------------*/
bool CardIter::bRewindOp(void)
{
  _precondition_(_MAGIC_, _FOOTER_);

  G.xKid.vBackstep();

  if ((G.p_Op = G.xKid.pChild())) {
    vFocusOnOp();
    vMember(G.e_Op, G.p_Op);
    return true;
  }

  G.e_Op = CK_NONE;
  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vRestoreMembers(void)
{
  _precondition_(_MAGIC_, _FOOTER_);

  CardIterMembers* pMembers_popped = pPop();

  G.vCopyFrom(pMembers_popped);

  delete pMembers_popped;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vResynchOp(void)
{
  _precondition_(_MAGIC_, _FOOTER_);

  G.xKid.vReposition(G.p_Op);
  RASSERT(G.p_Op == G.xKid.pChild());
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vResynchOp(Card* pCard)
{
  _precondition_(_MAGIC_, _FOOTER_);

  G.p_Op = pCard;
  vResynchOp();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vSaveMembers(void)
{
  CardIterMembers* pMembers_save = new CardIterMembers;

  pMembers_save->vCopyFrom(&G);

  vPush(pMembers_save);
}

/*----------------------------------------------------------------------------*/
/* capture prior projection                                                   */
/*----------------------------------------------------------------------------*/
void CardIter::vSnapArriving(void)
{
  _precondition_(_MAGIC_, _FOOTER_);

  if (Card_Emit* p_emit = G.p_Op->pSub_emit()) {
    Flux.m_pArriving = p_emit;
    Dic.m_p_expr = p_emit;
  }
}

/*----------------------------------------------------------------------------*/
/* begin and record transformation information                                */
/*----------------------------------------------------------------------------*/
void CardIter::vTransform(const char* azXfile, const char* azXfunc, eXF eXform)
{
  _precondition_( _MAGIC_, _FOOTER_);

  m_pBench->vTransform(azXfile, azXfunc, eXform);
}

/*----------------------------------------------------------------------------*/
/* which board to use                                                         */
/*----------------------------------------------------------------------------*/
void CardIter::vUseBoard(Board* pBoard)
{
  _precondition_( _MAGIC_, _FOOTER_);

  if (! pBoard)
    pBoard = m_pBench->pGetBoard(0);

  G.pBoard = pBoard;
  G.p_deck  = G.pBoard->pDeck();

  vUseCard(G.pBoard->pDeck());
}

/*----------------------------------------------------------------------------*/
/* which card to use                                                          */
/*----------------------------------------------------------------------------*/
void CardIter::vUseCard(Card* pCard)
{
  _precondition_(_MAGIC_, _FOOTER_);

  G.xKid.vParent(pCard);

  G.p_Op = nullptr;
  vMember(pCard->eCard(),pCard);

  //vBegin();
}

/*----------------------------------------------------------------------------*/
/* validate children                                                          */
/*----------------------------------------------------------------------------*/
void CardIter::vValidateBench(void)
{
  _precondition_(_MAGIC_, _FOOTER_);

  m_pBench->vValidate(this);
}

/*----------------------------------------------------------------------------*/
/* validate children                                                          */
/*----------------------------------------------------------------------------*/
void CardIter::vValidateChildren(Card* pCard, i8 iDepth)
{
  _precondition_(_MAGIC_, _FOOTER_);

  i8 nAncestors_start = nAncestors();

  vAncestorPush(pCard);
  i8 nAncestors_plusOne = nAncestors();
  RASSERT(nAncestors_start + 1 == nAncestors_plusOne);

  for (Kid xKid(pCard); xKid.bEach(); /**/) {

    Card* pChild = xKid.pChild();

    eCK eChild = pChild->eCard();

    vMember(eChild, pChild);

    switch (eChild) {

      case CK_c_ABSJOIN:      G.p_absJoin->vValidate(this, iDepth, pCard);  break;
      case CK_ADDRESS:        G.p_address->vValidate(this, iDepth, pCard);  break;
      case CK_o_AGGREG:       G.p_aggreg->vValidate(this, iDepth, pCard);  break;
      case CK_ANDCONJ:        G.p_andConj->vValidate(this, iDepth, pCard);  break;

      case CK_BOOLPRED:       G.p_boolPred->vValidate(this, iDepth, pCard);  break;
      case CK_BOUND:          G.p_bound->vValidate(this, iDepth, pCard);  break;
      case CK_BOUNDS:         G.p_bounds->vValidate(this, iDepth, pCard);  break;

      case CK_o_CALL:         G.p_call->vValidate(this, iDepth, pCard);  break;
      case CK_CALLER:         G.p_caller->vValidate(this, iDepth, pCard);  break;
      case CK_CALLOP:         G.p_callOp->vValidate(this, iDepth, pCard);  break;
      case CK_o_CAST:         G.p_cast->vValidate(this, iDepth, pCard);  break;
      case CK_CLAUSE:         G.p_clause->vValidate(this, iDepth, pCard);  break;
      case CK_c_COMBINED:     G.p_combined->vValidate(this, iDepth, pCard);  break;
      case CK_s_CONSTRAINT:   G.p_constraint->vValidate(this, iDepth, pCard);  break;
      case CK_CORRCOLS:       G.p_corrCols->vValidate(this, iDepth, pCard);  break;
      case CK_o_CORRELATION:  G.p_correlation->vValidate(this, iDepth, pCard);  break;
      case CK_COSTJOINING:    G.p_costJoining->vValidate(this, iDepth, pCard);  break;
      case CK_COSTNETWORK:    G.p_costNetwork->vValidate(this, iDepth, pCard);  break;
      case CK_COSTSTREAM:     G.p_costQuery->vValidate(this, iDepth, pCard);  break;
      case CK_COSTUNICAST:    G.p_costUnicast->vValidate(this, iDepth, pCard);  break;
      case CK_COUNT:          G.p_count->vValidate(this, iDepth, pCard);  break;
      case CK_o_CROSSJOIN:    G.p_crossJoin->vValidate(this, iDepth, pCard);  break;

      case CK_DECISION:       G.p_decision->vValidate(this, iDepth, pCard);  break;
      case CK_DIRECTION:      G.p_direction->vValidate(this, iDepth, pCard);  break;
      case CK_DISPOSITION:    G.p_disposition->vValidate(this, iDepth, pCard);  break;

      case CK_t_EMIT:         G.p_emit->vValidate(this, iDepth, pCard);  break;
      case CK_EXPR:           G.p_expr->vValidate(this, iDepth, pCard);  break;

      case CK_o_FILTER:       G.p_filter->vValidate(this, iDepth, pCard);  break;
      case CK_FILTPRED:       G.p_filtPred->vValidate(this, iDepth, pCard);  break;
      case CK_FILTPREDLIST:   G.p_filtPredList->vValidate(this, iDepth, pCard);  break;
      case CK_c_FLAT:         G.p_flat->vValidate(this, iDepth, pCard);  break;
      case CK_c_FOLD:         G.p_fold->vValidate(this, iDepth, pCard);  break;
      case CK_o_FROM:         G.p_from->vValidate(this, iDepth, pCard);  break;

      case CK_o_GBYAGG:       G.p_gbyAgg->vValidate(this, iDepth, pCard);  break;

      case CK_o_HASH:         G.p_hash->vValidate(this, iDepth, pCard);  break;
      case CK_s_HASHING:      G.p_hashing->vValidate(this, iDepth, pCard);  break;
      case CK_o_HAVING:       G.p_having->vValidate(this, iDepth, pCard);  break;
      case CK_t_HSHCOL:       G.p_hshCol->vValidate(this, iDepth, pCard);  break;
      case CK_s_HSHCOLLIST:   G.p_hshColList->vValidate(this, iDepth, pCard);  break;

      case CK_IDXCOL:         G.p_idxCol->vValidate(this, iDepth, pCard);  break;
      case CK_IDXCOLLIST:     G.p_idxColList->vValidate(this, iDepth, pCard);  break;
      case CK_INDEX:          G.p_index->vValidate(this, iDepth, pCard);  break;
      case CK_o_INDEXJOIN:    G.p_indexJoin->vValidate(this, iDepth, pCard);  break;
      case CK_t_INDEXLIST:    G.p_indexList->vValidate(this, iDepth, pCard);  break;
      case CK_INDEXNM:        G.p_indexNm->vValidate(this, iDepth, pCard);  break;
      case CK_o_INDEXSCAN:    G.p_indexScan->vValidate(this, iDepth, pCard);  break;
      case CK_o_INFLATE:      G.p_inflate->vValidate(this, iDepth, pCard);  break;
      case CK_s_INJECTABLE:   G.p_injectable->vValidate(this, iDepth, pCard);  break;

      case CK_JOINCAST:       G.p_joinCast->vValidate(this, iDepth, pCard);  break;
      case CK_JOINESTIMATE:   G.p_joinEstimate->vValidate(this, iDepth, pCard);  break;
      case CK_JOINORDER:      G.p_joinOrder->vValidate(this, iDepth, pCard);  break;
      case CK_JOINORDERING:   G.p_joinOrdering->vValidate(this, iDepth, pCard);  break;
      case CK_JOINPREDLIST:   G.p_joinPredList->vValidate(this, iDepth, pCard);  break;

      case CK_s_KEY:          G.p_key->vValidate(this, iDepth, pCard);  break;
      case CK_KEYCELLS:       G.p_keyCells->vValidate(this, iDepth, pCard);  break;
      case CK_o_KEYEXISTS:    G.p_keyExists->vValidate(this, iDepth, pCard);  break;
      case CK_o_KEYNOTEXISTS: G.p_keyNotExists->vValidate(this, iDepth, pCard);  break;

      case CK_s_LEVEL:        G.p_level->vValidate(this, iDepth, pCard);  break;
      case CK_o_LIMIT:        G.p_limit->vValidate(this, iDepth, pCard);  break;

      case CK_MAP:            G.p_map->vValidate(this, iDepth, pCard);  break;
      case CK_s_MAPPING:      G.p_mapping->vValidate(this, iDepth, pCard);  break;

      case CK_s_NEEDED:       G.p_needed->vValidate(this, iDepth, pCard);  break;
      case CK_NOTE:           G.p_note->vValidate(this, iDepth, pCard);  break;
      case CK_NOTCONJ:        G.p_notConj->vValidate(this, iDepth, pCard);  break;

      case CK_ON:             G.p_on->vValidate(this, iDepth, pCard);  break;
      case CK_ONCLAUSE:       G.p_onClause->vValidate(this, iDepth, pCard);  break;
      case CK_o_ORDAGG:       G.p_ordAgg->vValidate(this, iDepth, pCard);  break;
      case CK_ORCONJ:         G.p_orConj->vValidate(this, iDepth, pCard);  break;
      case CK_OUTERCOLNM:     G.p_outerColNm->vValidate(this, iDepth, pCard);  break;
      case CK_OUTERJOIN:      G.p_outerJoin->vValidate(this, iDepth, pCard);  break;

      case CK_PAYLOADCELLS:   G.p_payloadCells->vValidate(this, iDepth, pCard);  break;
      case CK_o_PROJECT:      G.p_project->vValidate(this, iDepth, pCard);  break;

      case CK_RELNOP:         G.p_relnOp->vValidate(this, iDepth, pCard);  break;
      case CK_REQUIRED:       G.p_required->vValidate(this, iDepth, pCard);  break;
      case CK_RLNCONJ:        G.p_rlnConj->vValidate(this, iDepth, pCard);  break;
      case CK_ROWCOUNT:       G.p_rowCount->vValidate(this, iDepth, pCard);  break;
      case CK_ROWSIZE:        G.p_rowSize->vValidate(this, iDepth, pCard);  break;
      case CK_RPNCALC:        G.p_rpnCalc->vValidate(this, iDepth, pCard);  break;

      case CK_o_SCAN:         G.p_scan->vValidate(this, iDepth, pCard);  break;
      case CK_SECTION:        G.p_section->vValidate(this, iDepth, pCard);  break;
      case CK_c_SEND:         G.p_send->vValidate(this, iDepth, pCard);  break;
      case CK_SPARE:          G.p_spare->vValidate(this, iDepth, pCard);  break;
      case CK_o_SORT:         G.p_sort->vValidate(this, iDepth, pCard);  break;
      case CK_SQYCALLS:       G.p_sqyCalls->vValidate(this, iDepth, pCard);  break;
      case CK_SQYINFO:        G.p_sqyInfo->vValidate(this, iDepth, pCard);  break;
      case CK_SQYNO:          G.p_sqyNo->vValidate(this, iDepth, pCard);  break;
      case CK_c_STAGE:        G.p_stage->vValidate(this, iDepth, pCard);  break;
      case CK_s_STREAM:       G.p_stream->vValidate(this, iDepth, pCard);  break;
      case CK_STREAMHEAD:     G.p_streamHead->vValidate(this, iDepth, pCard);  break;
      case CK_STREAMJOIN:     G.p_streamJoin->vValidate(this, iDepth, pCard);  break;
      case CK_STREAMKEY:      G.p_streamKey->vValidate(this, iDepth, pCard);  break;
      case CK_STREAMPER:      G.p_streamPer->vValidate(this, iDepth, pCard);  break;
      case CK_STREAMPTN:      G.p_streamPtn->vValidate(this, iDepth, pCard);  break;
      case CK_STREAMSET:      G.p_streamSet->vValidate(this, iDepth, pCard);  break;
      case CK_SUBQUERY:       G.p_subQuery->vValidate(this, iDepth, pCard);  break;
      case CK_SUBSET:         G.p_subSet->vValidate(this, iDepth, pCard);  break;
      case CK_SYNTHESIS:      G.p_synthesis->vValidate(this, iDepth, pCard);  break;

      case CK_s_TABLE:        G.p_table->vValidate(this, iDepth, pCard);  break;
      case CK_t_TABLEINFO:    G.p_tableInfo->vValidate(this, iDepth, pCard);  break;
      case CK_TABLELIST:      G.p_tableList->vValidate(this, iDepth, pCard);  break;
      case CK_t_TABLENM:      G.p_tableNm->vValidate(this, iDepth, pCard);  break;
      case CK_TABLEPTN:       G.p_tablePtn->vValidate(this, iDepth, pCard);  break;
      case CK_TABLESIZE:      G.p_tableSize->vValidate(this, iDepth, pCard);  break;
      case CK_TERM:           G.p_term->vValidate(this, iDepth, pCard);  break;
      case CK_o_TOP:          G.p_top->vValidate(this, iDepth, pCard);  break;
      case CK_TOPOLOGY:       G.p_topology->vValidate(this, iDepth, pCard);  break;

      case CK_UNIQUE:         G.p_unique->vValidate(this, iDepth, pCard);  break;

      case CK_o_WHERE:        G.p_where->vValidate(this, iDepth, pCard);  break;

      case CK_XCARD:          G.p_xcard->vValidate(this, iDepth, pCard);  break;

      default:
        NEVER_REACH
    }

  }

  vAncestorPop();
  i8 nAncestors_end = nAncestors();
  RASSERT(nAncestors_end == nAncestors_start);

}


/*----------------------------------------------------------------------------*/
/*-- Protected ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/