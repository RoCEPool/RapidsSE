/*============================================================================*/
/* CardIter.cpp --                                                            */
/*============================================================================*/

#include "CardIter.hpp"
#include "Cue.hpp"
#include "OptimiserFun.hpp"
#include "OptimiserCatalog.hpp"
#include "Optimiser.hpp"

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
  m_xSymbol_tableSyn.vInit_Tbl_Syn('a');

  m_i_RecursionDepth = 0;

  E.xEachOp.vParent(nullptr);

  E.m_bDontDescend = false;

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
  MemLib::vZero(m_paAncestry, sizeof(m_paAncestry), CN_CARDITER);
  MemLib::vZero(m_iaAncestry, sizeof(m_iaAncestry), CN_CARDITER);

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
    vPopEach();

  RASSERT(0 == nItems());

}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

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

  i8 i_GetAncestor = i_Ancestor;
  if (i_GetAncestor < 0)
    i_GetAncestor = m_nAncestors -1 + i_Ancestor;

  if (i_GetAncestor < 0 || i_GetAncestor > m_nAncestors)
    return nullptr;

  return m_paAncestry[i_GetAncestor];
}

/*----------------------------------------------------------------------------*/
/* get ancestor                                                               */
/*----------------------------------------------------------------------------*/
i8 CardIter::i_Ancestor(Card* pAncestor)
{
  _precondition_(_MAGIC_,_FOOTER_);

  for (i8 i_Ancestor = m_nAncestors-1; i_Ancestor >= 0; i_Ancestor--) {
    if (pAncestor == m_paAncestry[i_Ancestor])
      return i_Ancestor;
  }

  return C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/* find ancestor                                                             */
/*----------------------------------------------------------------------------*/
Card* CardIter::pAncestor(eCK eCard, i8* ai_Ancestor)
{
  _precondition_(_MAGIC_, _FOOTER_);

  for (i8 i_Ancestor = m_nAncestors-1; i_Ancestor >= 0; i_Ancestor--) {
    Card *pCard_ancestor = m_paAncestry[i_Ancestor];

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
/* dump ancestry                                                              */
/*----------------------------------------------------------------------------*/
void CardIter::vAncestorDump(Card* pCard)
{
  _precondition_(_MAGIC_,_FOOTER_);

  vTrace("^^ Ancestor Dump\n"_Stz);
  for (i8 i_Ancestor = 0; i_Ancestor < m_nAncestors; i_Ancestor++) {
    Card* pCard_ancestor = m_paAncestry[i_Ancestor];
    vTrace("  [%lld] %p %s /%lld %s\n"_Stz,
           i_Ancestor,
           pCard_ancestor,
           aCK[pCard_ancestor->eCard()],
           iAncestorCounter(i_Ancestor),
           pCard_ancestor->szCoord());
  }
  vTrace("$$ Ancestor Dump"_Stz);

  if (pCard)
    vTrace(" %p %s\n"_Stz,
           pCard,
           aCK[pCard->eCard()]);
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

  return m_paAncestry[m_nAncestors - 1];
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

  m_paAncestry[m_nAncestors++] = pCard;
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
/* return counter                                                             */
/*----------------------------------------------------------------------------*/
i8 CardIter::iAncestorCounter(i8 i_Ancestor)
{
  _precondition_(_MAGIC_,_FOOTER_);

  i8 iCounter = m_iaAncestry[i_Ancestor];

  return iCounter;
}

/*----------------------------------------------------------------------------*/
/* return counter                                                             */
/*----------------------------------------------------------------------------*/
i8 CardIter::iAncestorCounterInc(Card* pAncestor, i8 iInc)
{
  _precondition_(_MAGIC_,_FOOTER_);

  i8 i_Ancestor = CardIter::i_Ancestor(pAncestor);

  i8 iCounter = m_iaAncestry[i_Ancestor];

  m_iaAncestry[i_Ancestor] += iInc;

  return iCounter;
}

/*----------------------------------------------------------------------------*/
/* return next counter                                                        */
/*----------------------------------------------------------------------------*/
void CardIter::vAncestorCounterInc(Card* pAncestor, i8 iInc)
{
  _precondition_(_MAGIC_,_FOOTER_);

  i8 i_Ancestor = CardIter::i_Ancestor(pAncestor);

  m_iaAncestry[i_Ancestor] += iInc;
}

/*----------------------------------------------------------------------------*/
/* reset counter                                                              */
/*----------------------------------------------------------------------------*/
void CardIter::vAncestorCounterReset(i8 iTo)
{
  _precondition_(_MAGIC_,_FOOTER_);

  m_iaAncestry[m_nAncestors - 1] = iTo;
}

/*----------------------------------------------------------------------------*/
/* reset counter                                                              */
/*----------------------------------------------------------------------------*/
void CardIter::vAncestorCounterResetAll(i8 iTo)
{
  _precondition_(_MAGIC_,_FOOTER_);

  for (i8 i_Ancestor = 0; i_Ancestor < C_CARD_ANCESTRY; i_Ancestor++)
    m_iaAncestry[i_Ancestor] = iTo;
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
    Card *pMain = this->E.p_deck->pGetChild(i_Main);
    Card_Emit* p_emit = pMain->pSub_emit();

    if (!p_emit)
      continue;

    //p_emit->vDump(__func__);

    i8 nChildren_emit = p_emit->nChildCards();
    for (i8 i_Child = 0; i_Child < nChildren_emit; i_Child++) {
      Card *pChild_emit = p_emit->pGetChild(i_Child);
      if (!pChild_emit->bIsCard(CK_EXPR))
        continue;
      Card_Expr* p_expr_emit = _pCard_Expr(pChild_emit);
      if (1 != p_expr_emit->nChildCards())
        continue;

      Symbol* pSym_emit_term0 = p_expr_emit->pGet_term(0)->pSymbol();
      RASSERT(pSym_emit_term0->bIs_Col());

      if (pSym_emit_term0->bMatches(pSym_term_arg)) {
        p_expr_col = p_expr_emit;
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
    Card *pMain = this->E.p_deck->pGetChild(i_Main);

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
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vBackstepTarget(void)
{
  _precondition_(_MAGIC_, _FOOTER_);

  E.xTarget.vBackstep();
}

/*----------------------------------------------------------------------------*/
/* setup board iteration                                                      */
/*----------------------------------------------------------------------------*/
void CardIter::vBeginBoard(void)
{
  _precondition_(_MAGIC_,_FOOTER_);

  E.i_Board = C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vBeginEach(eCK eOp)
{
  _precondition_(_MAGIC_, _FOOTER_);

  vResetMembers();

  E.xEachOp.vBegin();

  if (CK_NONE != eOp)
    (void)bEachOp(eOp);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vBeginEach_tail(void)
{
  _precondition_(_MAGIC_, _FOOTER_);

  E.xEachOp.vBegin_tail();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vBeginTarget(eCK ePrimary, eCK eTarget)
{
  _precondition_(_MAGIC_, _FOOTER_);

  Card* pPrimary = E.p_Op->pFindSubCard(ePrimary);

  vBeginTarget(pPrimary, eTarget);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vBeginTarget(Card* pPrimary, eCK eTarget)
{
  _precondition_(_MAGIC_, _FOOTER_);

  vPushEach();

  eCK ePrimary = pPrimary->eCard();

  E.Iter.ePrimary = ePrimary;
  E.Iter.eTarget  = eTarget;

  if (CK_p_KEY == ePrimary && CK_EXPR == eTarget) {
    E.Iter.eSecondary = CK_NONE;
    E.xTarget.vBegin();
    E.xTarget.vParent(pPrimary);

  }
  else
  if (CK_p_STREAMKEY == ePrimary && CK_EXPR == eTarget) {
    E.Iter.eSecondary = CK_NONE;
    E.xTarget.vBegin();
    E.xTarget.vParent(pPrimary);
  }
  else
  if (CK_EXPR == ePrimary && CK_TERM == eTarget) {
    E.Iter.eSecondary = CK_NONE;
    E.xTarget.vBegin();
    E.xTarget.vParent(pPrimary);

  }
  else {
    NEVER_REACH
  }
}

/*----------------------------------------------------------------------------*/
/* get board                                                                  */
/*----------------------------------------------------------------------------*/
Board* CardIter::pBoard(void)
{
  _precondition_(_MAGIC_,_FOOTER_);

  return E.pBoard;
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

  return E.xEachOp.i_Child();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardIter::pChild(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  return E.xEachOp.pChild();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vChild(Card* pChild)
{
  _precondition_(_MAGIC_, _FOOTER_);

  RASSERT(E.xEachOp.bWithinRange());

  E.xEachOp.vReposition(E.xEachOp.pParent()->iFindChild(pChild));
}

/*----------------------------------------------------------------------------*/
/* clear the general M registers                                              */
/*----------------------------------------------------------------------------*/
void CardIter::vClearRegisters(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  MemLib::vZero(E.md_Coord, sizeof(E.md_Coord), CN_CARDITER);

  Arriving.m_pEmitted_prior = nullptr;
  Arriving.m_pStreamKey = nullptr;

  Dic.m_p_expr = nullptr;
  Dic.m_p_key = nullptr;

  E.p_Op   = nullptr;
  E.e_Op   = CK_NONE;
  E.i_Op   = C_UNKNOWN;
  E.p_Sub  = nullptr;
  E.p_Sub2 = nullptr;
  E.p_Sub3 = nullptr;

  E.Iter.ePrimary = CK_NONE;
  E.Iter.eTarget = CK_NONE;
  E.Iter.eSecondary = CK_NONE;
  E.Iter.eTertiary = CK_NONE;

  Early.m_pOp_tip = nullptr;
  Early.m_pNeeded_later = nullptr;

  Flat.m_i_fold = C_UNKNOWN;
  //Flat.m_p_term_crlKey  = nullptr;

  Next.m_pProject = nullptr;
  Next.m_pEmitted = nullptr;

  StreamOrder.m_pStreamKey = nullptr;
  StreamOrder.m_pEmit = nullptr;

  X.m_pExprList_a = nullptr;
  X.m_pExprList_i = nullptr;
  X.m_pExprList_k = nullptr;

  Smooth.Where_preAgg.m_bMorph_sqlOp_To_EQ = false;
  Smooth.Where_preAgg.bReplace_sqCall_With_result = false;

  Smooth.Aggreg.m_bMorph_To_gbyAgg = false;

  Smooth.GbyAgg.m_bInject_foldExpr = false;

  Smooth.Project_postAgg.m_bSeen = false;

  Smooth.Having.m_bCorrect_After_Inject = false;

  Smooth.Sort.m_bCorrect_After_Inject = false;

  Smooth.Unproject.m_bUnproject_foldExpr = false;
}

/*----------------------------------------------------------------------------*/
/* cue user                                                                   */
/*----------------------------------------------------------------------------*/
eOA CardIter::eCueUser(String* psPrompt)
{
  _precondition_(_MAGIC_, _FOOTER_);

  TokenVec* pTknVec = nullptr;

  psPrompt->vConcat(" >> "_Stz);


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

  return eOptAct;
}

/*----------------------------------------------------------------------------*/
/* delete main when given sub is empty                                        */
/*----------------------------------------------------------------------------*/
void CardIter::vDeleteWhenEmptySub(eCK eOp, eCK eSub)
{
  _precondition_(_MAGIC_, _FOOTER_);

  if (!bEachOp(eOp, eSub))
    return;

  vMember(E.e_Op, E.p_Op);

  if (E.p_Sub->nChildCards())
    return;

  vRemoveDeleteMain(eOp);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vDontDescend(bool bDontDescend)
{
  _precondition_(_MAGIC_, _FOOTER_);

  E.m_bDontDescend = bDontDescend;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool CardIter::bEachBoard(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  if (pEachBoard()) {
    Board* pBoard = m_pBench->pGetBoard(E.i_Board);
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

  E.pBoard = pEachBoard(i_Board);

  return E.pBoard;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Board* CardIter::pEachBoard(i8& ri_Board)
{
  _precondition_( _MAGIC_, _FOOTER_);

  if (C_UNKNOWN == E.i_Board)
    E.i_Board = 0;
  else
    E.i_Board++;

  if (E.i_Board >= m_pBench->nBoards()) {
    E.pBoard = nullptr;
    ri_Board  = C_UNKNOWN;
    E.i_Board = 0;

    return nullptr;
  }

  E.pBoard = m_pBench->pGetBoard(E.i_Board);
  ri_Board = E.i_Board;

  vUseCard(E.pBoard->pDeck());

  return E.pBoard;
}

/*----------------------------------------------------------------------------*/
/* conditionally step main register to next main and update M member          */
/*----------------------------------------------------------------------------*/
bool CardIter::bEachOp(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(E.xEachOp.bBegun());
  RASSERT(! E.xEachOp.bEnded());

  while (E.xEachOp.bEach()) {
    if (E.xEachOp.bParentIs(CK_DECK) && ! E.xEachOp.pChild()->bIsOp())
      continue;

    E.p_Op = E.xEachOp.p_op();
    vFocusOnOp();
    vMember(E.p_Op->eCard(), E.p_Op);
    return true;
  }

  E.p_Op = nullptr;
  E.e_Op = CK_NONE;
  E.xEachOp.vEnded();
  return false;
}

/*----------------------------------------------------------------------------*/
/* conditionally step main register to next main and update M member          */
/*----------------------------------------------------------------------------*/
bool CardIter::bEachOp(eCK eOp)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(E.xEachOp.bBegun());
  RASSERT(! E.xEachOp.bEnded());

  RASSERT(Card_OP::bIsOp(eOp));

  E.p_Op = nullptr;

  while (E.xEachOp.bEach()) {

    if (E.xEachOp.pChild()->eCard() != eOp)
      continue;

    E.p_Op = E.xEachOp.p_op();
    vFocusOnOp();
    vMember(E.p_Op->eCard(), E.p_Op);
    return true;
  }

  E.p_Op = nullptr;
  E.xEachOp.vEnded();
  return false;
}

/*----------------------------------------------------------------------------*/
/* conditionally step main register to next main and update M member          */
/*----------------------------------------------------------------------------*/
bool CardIter::bEachOp_break(eCK eOp, eCK eBreak)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(E.xEachOp.bBegun());
  RASSERT(! E.xEachOp.bEnded());

  RASSERT(Card_OP::bIsOp(eOp));

  E.p_Op = nullptr;

  while (E.xEachOp.bEach()) {

    if (E.xEachOp.pChild()->bIsCard(eBreak))
      return false;

    if (E.xEachOp.pChild()->eCard() != eOp)
      continue;

    E.p_Op = E.xEachOp.p_op();
    vFocusOnOp();
    vMember(E.p_Op->eCard(), E.p_Op);
    return true;
  }

  E.p_Op = nullptr;
  E.xEachOp.vEnded();
  return false;

}

/*----------------------------------------------------------------------------*/
/* conditionally step main register to next main which has sub and update M   */
/*----------------------------------------------------------------------------*/
bool CardIter::bEachOp(eCK eOp, eCK eSub)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(! Card_OP::bIsOp(eSub) || CK_o_FILTER == eSub);

  while (bEachOp(eOp)) {
    if (! (E.p_Sub = E.p_Op->pFindSubCard(eSub)))
      continue;

    vMember(E.p_Sub->eCard(), E.p_Sub);
    return true;
  }

  RASSERT(E.xEachOp.bEnded());

  E.p_Sub = nullptr;
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
    if (E.p_Sub->pSymbol()->bHasFacet(szSubFacet)) {
      RASSERT(E.p_Sub->pSymbol()->bSingleFaceted());
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

  RASSERT(! Card_OP::bIsOp(eSub2) || CK_o_FILTER == eSub2);

  while (bEachOp(eOp, eSub1)) {
    if (! (E.p_Sub2 = E.p_Op->pFindSubCard(eSub2)))
      continue;

    vMember(E.p_Sub2->eCard(), E.p_Sub2);
    return true;
  }

  RASSERT(E.xEachOp.bEnded());

  E.p_Sub2 = nullptr;
  vMember(eSub2, nullptr);

  return false;
}

/*----------------------------------------------------------------------------*/
/* conditionally step main register to next main which has sub and update M   */
/*----------------------------------------------------------------------------*/
bool CardIter::bEachOp(eCK eOp, eCK eSub1, eCK eSub2, eCK eSub3)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(! Card_OP::bIsOp(eSub3));

  while (bEachOp(eOp, eSub1, eSub2)) {
    if (! (E.p_Sub3 = E.p_Op->pFindSubCard(eSub3)))
      continue;

    vMember(E.p_Sub3->eCard(), E.p_Sub3);
    return true;
  }

  RASSERT(E.xEachOp.bEnded());

  E.p_Sub3 = nullptr;
  vMember(eSub3, nullptr);

  return false;

}

/*----------------------------------------------------------------------------*/
/* conditionally step main register to next main and update M member          */
/*----------------------------------------------------------------------------*/
bool CardIter::bEachOp_reverse(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(E.xEachOp.bBegun());
  RASSERT(! E.xEachOp.bEnded());

  E.p_Op = nullptr;

  while (E.xEachOp.bEach_reverse()) {
    if (E.xEachOp.bParentIs(CK_DECK) && ! E.xEachOp.pChild()->bIsOp())
      continue;

    E.p_Op = E.xEachOp.p_op();
    vFocusOnOp();
    vMember(E.p_Op->eCard(), E.p_Op);
    return true;
  }

  E.p_Op = nullptr;
  E.xEachOp.vEnded();
  return false;
}

/*----------------------------------------------------------------------------*/
/* conditionally step main register to next main and update M member          */
/*----------------------------------------------------------------------------*/
bool CardIter::bEachOp_reverse(eCK eOp)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(E.xEachOp.bBegun());
  RASSERT(! E.xEachOp.bEnded());

  RASSERT(Card_OP::bIsOp(eOp));

  E.p_Op = nullptr;

  while (E.xEachOp.bEach_reverse()) {
    if (E.xEachOp.pChild()->eCard() != eOp)
      continue;

    E.p_Op = E.xEachOp.p_op();
    vFocusOnOp();
    vMember(E.p_Op->eCard(), E.p_Op);
    return true;
  }

  E.p_Op = nullptr;
  E.xEachOp.vEnded();
  return false;
}

/*----------------------------------------------------------------------------*/
/* conditionally step main register to next main and update M member          */
/*----------------------------------------------------------------------------*/
bool CardIter::bEachOp_reverse_foldBreak(eCK eOp)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(E.xEachOp.bBegun());
  RASSERT(! E.xEachOp.bEnded());

  RASSERT(Card_OP::bIsOp(eOp));

  E.p_Op = nullptr;

  while (E.xEachOp.bEach_reverse()) {

    if (E.xEachOp.pChild()->bIsCard(CK_o_FOLD))
      return false;

    if (E.xEachOp.pChild()->eCard() != eOp)
      continue;

    E.p_Op = E.xEachOp.p_op();
    vFocusOnOp();
    vMember(E.p_Op->eCard(), E.p_Op);
    return true;
  }

  E.p_Op = nullptr;
  E.xEachOp.vEnded();
  return false;
}

/*----------------------------------------------------------------------------*/
/* conditionally step main register to next main which has sub and update G   */
/*----------------------------------------------------------------------------*/
bool CardIter::bEachOp_reverse(eCK eOp, eCK eSub)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(! Card_OP::bIsOp(eSub));

  while (bEachOp_reverse(eOp)) {
    if (! (E.p_Sub = E.p_Op->pFindSubCard(eSub)))
      continue;

    vFocusOnOp();

    vMember(E.e_Op, E.p_Op);
    vMember(E.p_Sub->eCard(), E.p_Sub);
    return true;
  }

  E.p_Op  = nullptr;
  E.e_Op  = CK_NONE;
  vMember(eOp, nullptr);
  E.p_Sub = nullptr;
  vMember(eSub, nullptr);
  return false;
}

/*----------------------------------------------------------------------------*/
/* conditionally step main register to next main which has sub and update G   */
/*----------------------------------------------------------------------------*/
bool CardIter::bEachOp_reverse_foldBreak(eCK eOp, eCK eSub)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(! Card_OP::bIsOp(eSub) || CK_o_FILTER == eSub);

  while (bEachOp_reverse_foldBreak(eOp)) {
    if (! (E.p_Sub = E.p_Op->pFindSubCard(eSub)))
      continue;

    vFocusOnOp();

    vMember(E.e_Op, E.p_Op);
    vMember(E.p_Sub->eCard(), E.p_Sub);
    return true;
  }

  // did not find card
  E.p_Op  = nullptr;
  E.e_Op  = CK_NONE;
  vMember(eOp, nullptr);
  E.p_Sub = nullptr;
  vMember(eSub, nullptr);
  return false;
}

/*----------------------------------------------------------------------------*/
/* step main register to next main which has sub and update M member          */
/*----------------------------------------------------------------------------*/
bool CardIter::bEachOp_bySub(eCK eSub)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(! Card_OP::bIsOp(eSub));

  while (bEachOp()) {
    if (! (E.p_Sub = E.p_Op->pFindSubCard(eSub)))
      continue;

    vFocusOnOp();

    vMember(E.e_Op, E.p_Op);
    vMember(E.p_Sub->eCard(), E.p_Sub);
    return true;
  }

  E.p_Op  = nullptr;
  E.e_Op  = CK_NONE;
  E.p_Sub = nullptr;
  vMember(eSub, nullptr);
  return false;
}

/*----------------------------------------------------------------------------*/
/* step main register to next main which has sub and update M member          */
/*----------------------------------------------------------------------------*/
bool CardIter::bEachOp_by2Sub(eCK eSub1, eCK eSub2)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(! Card_OP::bIsOp(eSub1));
  RASSERT(! Card_OP::bIsOp(eSub2));

  while (bEachOp()) {
    if (! (E.p_Sub = E.p_Op->pFindSubCard(eSub1)))
      continue;

    if (! (E.p_Sub2 = E.p_Op->pFindSubCard(eSub2)))
      continue;

    vFocusOnOp();

    vMember(E.e_Op, E.p_Op);
    vMember(E.p_Sub->eCard(), E.p_Sub);
    vMember(E.p_Sub2->eCard(), E.p_Sub2);
    return true;
  }

  E.p_Op  = nullptr;
  E.e_Op  = CK_NONE;
  E.p_Sub = nullptr;
  vMember(eSub1, nullptr);
  E.p_Sub2 = nullptr;
  vMember(eSub2, nullptr);
  return false;
}

/*----------------------------------------------------------------------------*/
/* step main register to next main which has sub and update M member          */
/*----------------------------------------------------------------------------*/
bool CardIter::bEachOp_by3Sub(eCK eSub1, eCK eSub2, eCK eSub3)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(! Card_OP::bIsOp(eSub1));
  RASSERT(! Card_OP::bIsOp(eSub2));
  RASSERT(! Card_OP::bIsOp(eSub3));

  while (bEachOp()) {
    if (! (E.p_Sub = E.p_Op->pFindSubCard(eSub1)))
      continue;

    if (! (E.p_Sub2 = E.p_Op->pFindSubCard(eSub2)))
      continue;

    if (! (E.p_Sub3 = E.p_Op->pFindSubCard(eSub3)))
      continue;

    vFocusOnOp();

    vMember(E.e_Op, E.p_Op);
    vMember(E.p_Sub->eCard(), E.p_Sub);
    vMember(E.p_Sub2->eCard(), E.p_Sub2);
    vMember(E.p_Sub3->eCard(), E.p_Sub3);
    return true;
  }

  E.p_Op  = nullptr;
  E.e_Op  = CK_NONE;
  E.p_Sub = nullptr;
  vMember(eSub1, nullptr);
  E.p_Sub2 = nullptr;
  vMember(eSub2, nullptr);
  E.p_Sub3 = nullptr;
  vMember(eSub3, nullptr);
  return false;
}

/*----------------------------------------------------------------------------*/
/* conditionally step main register to next main and update M member          */
/*----------------------------------------------------------------------------*/
i8 CardIter::i_Target(void)
{
  _precondition_(_MAGIC_, _FOOTER_);

  return E.xTarget.i_Child();
}

/*----------------------------------------------------------------------------*/
/* conditionally step main register to next main and update M member          */
/*----------------------------------------------------------------------------*/
bool CardIter::bEachTarget(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(E.xTarget.bBegun());
  RASSERT(! E.xTarget.bEnded());

  if (CK_EXPR    == E.Iter.ePrimary &&
      CK_TERM    == E.Iter.eTarget) {
    while (E.xTarget.bEach_opaque(E.Iter.eTarget)) {
      Card *pTarget = E.xTarget.pChild();
      vMember(E.Iter.eTarget, pTarget);
      return true;
    }
  }
  else
  if (CK_NONE != E.Iter.eSecondary) {

    while (E.xTarget.bEach(E.Iter.eSecondary)) {
      vMember(E.xTarget.eChild(), E.xTarget.pChild());

      Card *pTarget = E.xTarget.pChild()->pFindSubCard(E.Iter.eTarget);
      vMember(E.Iter.eTarget, pTarget);

      return true;
    }
  }
  else {
    while (E.xTarget.bEach(E.Iter.eTarget)) {
      Card *pTarget = E.xTarget.pChild();
      vMember(E.Iter.eTarget, pTarget);

      return true;
    }
  }

  E.xTarget.vEnded();

  vPopEach();

  return false;
}

/*----------------------------------------------------------------------------*/
/* set early tip                                                              */
/*----------------------------------------------------------------------------*/
Card_OP* CardIter::pEarlyTip(void)
{
  _precondition_(_MAGIC_, _FOOTER_);

  return Early.m_pOp_tip;
}

/*----------------------------------------------------------------------------*/
/* set early tip                                                              */
/*----------------------------------------------------------------------------*/
void CardIter::vEarlyTip(Card_OP* p_Op_tip)
{
  _precondition_(_MAGIC_, _FOOTER_);

  if (! p_Op_tip) {
    Early.m_pOp_tip = nullptr;
    return;
  }

  RASSERT(! Early.m_pOp_tip);
  Early.m_pOp_tip = p_Op_tip;
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
/* end member                                                                 */
/*----------------------------------------------------------------------------*/
Card* CardIter::pEndTarget(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  Card* pTarget_end = pMember(E.Iter.eTarget);

  vEndTarget();

  return pTarget_end;
}

/*----------------------------------------------------------------------------*/
/* end member                                                                 */
/*----------------------------------------------------------------------------*/
void CardIter::vEndTarget(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(E.xTarget.bBegun());
  RASSERT(! E.xTarget.bEnded());

  E.xTarget.vEnded();

  vPopEach();

}

/*----------------------------------------------------------------------------*/
/* end of transformation                                                      */
/*----------------------------------------------------------------------------*/
void CardIter::vEndTransform(i8 iLineNo)
{
  _precondition_( _MAGIC_, _FOOTER_);

  if (E.pBoard) {

    if (!  E.pBoard->pState()->bHasName(aXF[m_pBench->eXform()]))
      E.pBoard->pState()->vInsert(aXF[m_pBench->eXform()]);

    for (Kid xDeck(E.p_deck); xDeck.bEach(); /**/) {
      xDeck.pChild()->vEnsureFirst(CK_p_LEVEL);
      xDeck.pChild()->vEnsureLast(CK_p_CONSTRAINT);
      xDeck.pChild()->vEnsureLast(CK_p_NEEDED);
    }

    vPrintBoard(m_pBench->szXfunc()->azChar(),
                m_pBench->szXfile()->azChar(),
                iLineNo);

  }

  vValidateBench();

  E.xEachOp.vEnded();
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
    Card_OP *p_OP = (Card_OP*)E.p_deck->pGetChild(i_Main);
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

  MemLib::vZero(E.md_Coord, sizeof(E.md_Coord), CN_CARDITER);
  StrLib::vCopy(E.md_Coord, sizeof(E.md_Coord), E.p_Op->szCoord()->azChar());

  E.e_Op = E.p_Op->eCard();
  E.i_Op = this->i_Child();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardIter::pChild(i8 i_Child)
{
  _precondition_(_MAGIC_, _FOOTER_);

  Card* pChild = E.xEachOp.pParent()->pGetChild(i_Child);

  return pChild;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool CardIter::bIsQuery(BASE::Stz *szQueryNm)
{
  _precondition_(_MAGIC_, _FOOTER_);

  if (m_pBench->pOptimiser()->szQueryNm()->bSame(szQueryNm))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vInsertAfter(Card* pCard, Card* pAfter)
{
  _precondition_(_MAGIC_, _FOOTER_);

  E.p_deck->vInsertChildAfter(pCard, pAfter);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vInsertBefore(Card* pCard, Card* pBefore)
{
  _precondition_(_MAGIC_, _FOOTER_);

  E.p_deck->vInsertChildBefore(pCard, pBefore);
}

/*----------------------------------------------------------------------------*/
/* fetch M member by decoding given eCard                                   */
/*----------------------------------------------------------------------------*/
Card* CardIter::pMember(eCK eCard)
{
  _precondition_(_MAGIC_, _FOOTER_);

  switch (eCard) {

    case CK_o_ABSJOIN:      return E.p_absJoin;
    case CK_ADDRESS:        return E.p_address;
    case CK_o_AGGREG:       return E.p_aggreg;
    case CK_ANDCONJ:        return E.p_andConj;

    case CK_PREDICATE:       return E.p_predicate;
    case CK_BOUND:          return E.p_bound;
    case CK_BOUNDS:         return E.p_bounds;

    case CK_o_CALL:         return E.p_call;
    case CK_CALLER:         return E.p_caller;
    case CK_CALLOP:         return E.p_callOp;
    case CK_o_CAST:         return E.p_cast;
    case CK_CLAUSE:         return E.p_clause;
    case CK_p_CONSTRAINT:   return E.p_constraint;
    case CK_CORRCOLS:       return E.p_corrCols;
    case CK_o_CORRELATION:  return E.p_correlation;
    case CK_COSTJOINING:    return E.p_costJoining;
    case CK_COSTNETWORK:    return E.p_costNetwork;
    case CK_COSTSTREAM:     return E.p_costQuery;
    case CK_COSTUNICAST:    return E.p_costUnicast;
    case CK_COUNT:          return E.p_count;
    case CK_o_CROSSJOIN:    return E.p_crossJoin;

    case CK_p_DECISION:      return E.p_decision;
    case CK_DISPOSITION:    return E.p_disposition;

    case CK_p_EMIT:         return E.p_emit;
    case CK_EXPR:           return E.p_expr;

    case CK_o_FILTER:       return E.p_filter;
    case CK_FILTPRED:       return E.p_filtPred;
    case CK_FILTPREDLIST:   return E.p_filtPredList;
    case CK_o_FOLD:         return E.p_fold;
    case CK_o_FROM:         return E.p_from;

    case CK_o_GBYAGG:       return E.p_gbyAgg;
    case CK_o_GLOBALHASH:   return E.p_globalHash;

    case CK_o_HASH:         return E.p_hash;
    case CK_p_HASHING:      return E.p_hashing;
    case CK_o_HAVING:       return E.p_having;
    case CK_HSHCOL:         return E.p_hshCol;
    case CK_p_HSHCOLLIST:   return E.p_hshColList;

    case CK_IDXCOL:         return E.p_idxCol;
    case CK_IDXCOLLIST:     return E.p_idxColList;
    case CK_INDEX:          return E.p_index;
    case CK_o_INDEXJOIN:    return E.p_indexJoin;
    case CK_INDEXLIST:    return E.p_indexList;
    case CK_INDEXNM:        return E.p_indexNm;
    case CK_o_INDEXSCAN:    return E.p_indexScan;
    case CK_o_INFLATE:      return E.p_inflate;
    case CK_p_INJECTABLE:   return E.p_injectable;

    case CK_JOINCAST:       return E.p_joinCast;
    case CK_JOINESTIMATE:   return E.p_joinEstimate;
    case CK_JOINORDER:      return E.p_joinOrder;
    case CK_JOINORDERING:   return E.p_joinOrdering;
    case CK_JOINPREDLIST:   return E.p_joinPredList;

    case CK_p_KEY:          return E.p_key;
    case CK_KEYCELLS:       return E.p_keyCells;
    case CK_o_KEYEXISTS:    return E.p_keyExists;
    case CK_o_KEYNOTEXISTS: return E.p_keyNotExists;

    case CK_p_LEVEL:        return E.p_level;
    case CK_o_LIMIT:        return E.p_limit;

    case CK_MAP:            return E.p_map;
    case CK_p_MAPPING:      return E.p_mapping;

    case CK_p_NEEDED:       return E.p_needed;
    case CK_p_NEEDEDv2:     return E.p_neededv2;
    case CK_NOTE:           return E.p_note;
    case CK_NOTCONJ:        return E.p_notConj;

    case CK_ON:             return E.p_on;
    case CK_o_ORDAGG:       return E.p_ordAgg;
    case CK_ORCONJ:         return E.p_orConj;
    case CK_OUTERCOLNM:     return E.p_outerColNm;
    case CK_OUTERJOIN:      return E.p_outerJoin;

    case CK_PAYLOADCELLS:   return E.p_payloadCells;
    case CK_o_PDSROW:       return E.p_pdsRow;
    case CK_o_PROJECT:      return E.p_project;

    case CK_RELNOP:         return E.p_relnOp;
    case CK_RLNCONJ:        return E.p_rlnConj;
    case CK_ROWCOUNT:       return E.p_rowCount;
    case CK_ROWSIZE:        return E.p_rowSize;
    case CK_o_RRFJOIN:      return E.p_rrfJoin;

    case CK_o_SCAN:         return E.p_scan;
    case CK_SECTION:        return E.p_section;
    case CK_o_SEND:         return E.p_send;
    case CK_SPARE:          return E.p_spare;
    case CK_o_SORT:         return E.p_sort;
    case CK_SQYCALLS:       return E.p_sqyCalls;
    case CK_SQYINFO:        return E.p_sqyInfo;
    case CK_SQYNO:          return E.p_sqyNo;
    case CK_o_STAGE:        return E.p_stage;
    case CK_STREAMHEAD:     return E.p_streamHead;
    case CK_STREAMJOIN:     return E.p_streamJoin;
    case CK_p_STREAMKEY:    return E.p_streamKey;
    case CK_STREAMPER:      return E.p_streamPer;
    case CK_STREAMPTN:      return E.p_streamPtn;
    case CK_STREAMSET:      return E.p_streamSet;
    case CK_SUBQUERY:       return E.p_subQuery;
    case CK_SUBSET:         return E.p_subSet;

    case CK_p_TABLE:        return E.p_table;
    case CK_TABLELIST:      return E.p_tableList;
    case CK_TABLENM:      return E.p_tableNm;
    case CK_TABLEPTN:       return E.p_tablePtn;
    case CK_TABLESIZE:      return E.p_tableSize;
    case CK_TERM:           return E.p_term;
    case CK_o_TOP:          return E.p_top;
    case CK_TOPOLOGY:       return E.p_topology;

    case CK_UNIQUE:         return E.p_unique;

    case CK_o_WHERE:        return E.p_where;

    case CK_XCARD:          return E.p_xcard;

    default:
      NEVER_REACH
  }
}

/*----------------------------------------------------------------------------*/
/* set member card                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vMember(eCK eCard, Card* pCard)
{
  _precondition_(_MAGIC_, _FOOTER_);

  RASSERT(!pCard || pCard->bIsCard(eCard));

  switch (eCard) {

    case CK_NONE: break;

    case CK_o_ABSJOIN: E.p_absJoin           = _pCard_AbsJoin(pCard); break;
    case CK_ADDRESS: E.p_address             = _pCard_Address(pCard); break;
    case CK_o_AGGREG: E.p_aggreg             = _pCard_Aggreg(pCard);  break;
    case CK_ANDCONJ: E.p_andConj             = _pCard_AndConj(pCard);break;

    case CK_PREDICATE: E.p_predicate         = _pCard_Predicate(pCard); break;
    case CK_BOUND: E.p_bound                 = _pCard_Bound(pCard); break;
    case CK_BOUNDS: E.p_bounds               = _pCard_Bounds(pCard); break;

    case CK_o_CALL: E.p_call                 = _pCard_Call(pCard); break;
    case CK_CALLER: E.p_caller               = _pCard_Caller(pCard); break;
    case CK_CALLOP: E.p_callOp               = _pCard_CallOp(pCard); break;
    case CK_o_CAST: E.p_cast                 = _pCard_Cast(pCard); break;
    case CK_CLAUSE: E.p_clause               = _pCard_Clause(pCard); break;

    case CK_p_CONSTRAINT: E.p_constraint     = _pCard_Constraint(pCard); break;
    case CK_CORRCOLS: E.p_corrCols           = _pCard_CorrCols(pCard); break;
    case CK_o_CORRELATION: E.p_correlation   = _pCard_Correlation(pCard); break;
    case CK_COSTJOINING: E.p_costJoining     = _pCard_CostJoining(pCard); break;
    case CK_COSTNETWORK: E.p_costNetwork     = _pCard_CostNetwork(pCard); break;
    case CK_COSTSTREAM: E.p_costQuery        = _pCard_CostStream(pCard); break;
    case CK_COSTUNICAST: E.p_costUnicast     = _pCard_CostUnicast(pCard); break;
    case CK_COUNT: E.p_count                 = _pCard_Count(pCard); break;
    case CK_o_CROSSJOIN: E.p_crossJoin       = _pCard_CrossJoin(pCard); break;

    case CK_DECK: E.p_deck                   = _pCard_Deck(pCard); break;
    case CK_p_DECISION: E.p_decision         = _pCard_Decision(pCard); break;
    case CK_DISPOSITION: E.p_disposition     = _pCard_Disposition(pCard); break;

    case CK_p_EMIT: E.p_emit                 = _pCard_Emit(pCard); break;
    case CK_EXPR: E.p_expr                   = _pCard_Expr(pCard); break;

    case CK_o_FILTER: E.p_filter             = _pCard_Filter(pCard); break;
    case CK_FILTPRED: E.p_filtPred           = _pCard_FiltPred(pCard); break;
    case CK_FILTPREDLIST: E.p_filtPredList   = _pCard_FiltPredList(pCard); break;
    case CK_o_FOLD: E.p_fold                 = _pCard_Fold(pCard); break;
    case CK_o_FROM: E.p_from                 = _pCard_From(pCard); break;

    case CK_o_GBYAGG: E.p_gbyAgg             = _pCard_GbyAgg(pCard); break;
    case CK_o_GLOBALHASH: E.p_globalHash     = _pCard_GlobalHash(pCard); break;

    case CK_o_HASH: E.p_hash                 = _pCard_Hash(pCard); break;
    case CK_p_HASHING: E.p_hashing           = _pCard_Hashing(pCard); break;
    case CK_o_HAVING: E.p_having             = _pCard_Having(pCard); break;
    case CK_HSHCOL: E.p_hshCol               = _pCard_HshCol(pCard); break;
    case CK_p_HSHCOLLIST: E.p_hshColList     = _pCard_HshColList(pCard); break;

    case CK_IDXCOL: E.p_idxCol               = _pCard_IdxCol(pCard); break;
    case CK_IDXCOLLIST: E.p_idxColList       = _pCard_IdxColList(pCard); break;
    case CK_INDEX: E.p_index                 = _pCard_Index(pCard); break;
    case CK_o_INDEXJOIN: E.p_indexJoin       = _pCard_IndexJoin(pCard); break;
    case CK_INDEXLIST: E.p_indexList         = _pCard_IndexList(pCard); break;
    case CK_INDEXNM: E.p_indexNm             = _pCard_IndexNm(pCard); break;
    case CK_o_INDEXSCAN: E.p_indexScan       = _pCard_IndexScan(pCard); break;
    case CK_p_INJECTABLE: E.p_injectable     = _pCard_Injectable(pCard); break;
    case CK_o_INFLATE: E.p_inflate           = _pCard_Inflate(pCard); break;

    case CK_JOINCAST: E.p_joinCast           = _pCard_JoinCast(pCard); break;
    case CK_JOINESTIMATE: E.p_joinEstimate   = _pCard_JoinEstimate(pCard); break;
    case CK_JOINORDER: E.p_joinOrder         = _pCard_JoinOrder(pCard); break;
    case CK_JOINORDERING: E.p_joinOrdering   = _pCard_JoinOrdering(pCard); break;
    case CK_JOINPREDLIST: E.p_joinPredList   = _pCard_JoinPredList(pCard); break;

    case CK_p_KEY: E.p_key                   = _pCard_Key(pCard); break;
    case CK_KEYCELLS: E.p_keyCells           = _pCard_KeyCells(pCard); break;
    case CK_o_KEYEXISTS: E.p_keyExists       = _pCard_KeyExists(pCard); break;
    case CK_o_KEYNOTEXISTS:E.p_keyNotExists  = _pCard_KeyNotExists(pCard); break;

    case CK_p_LEVEL: E.p_level               = _pCard_Level(pCard); break;
    case CK_o_LIMIT: E.p_limit               = _pCard_Limit(pCard); break;

    case CK_MAP: E.p_map                     = _pCard_Map(pCard); break;
    case CK_p_MAPPING: E.p_mapping           = _pCard_Mapping(pCard); break;

    case CK_p_NEEDED: E.p_needed             = _pCard_Needed(pCard); break;
    case CK_p_NEEDEDv2: E.p_neededv2         = _pCard_Neededv2(pCard); break;
    case CK_NOTE: E.p_note                   = _pCard_Note(pCard); break;
    case CK_NOTCONJ: E.p_notConj             = _pCard_NotConj(pCard);break;

    case CK_ON: E.p_on                       = _pCard_On(pCard); break;
    case CK_ORCONJ: E.p_orConj               = _pCard_OrConj(pCard);break;
    case CK_o_ORDAGG: E.p_ordAgg             = _pCard_OrdAgg(pCard); break;
    case CK_OUTERCOLNM: E.p_outerColNm       = _pCard_OuterColNm(pCard); break;
    case CK_OUTERJOIN: E.p_outerJoin         = _pCard_OuterJoin(pCard); break;

    case CK_PAYLOADCELLS: E.p_payloadCells   = _pCard_PayloadCells(pCard); break;
    case CK_o_PDSROW: E.p_pdsRow             = _pCard_PdsRow(pCard); break;
    case CK_o_PROJECT: E.p_project           = _pCard_Project(pCard); break;

    case CK_RELNOP: E.p_relnOp               = _pCard_RelnOp(pCard); break;
    case CK_RLNCONJ: E.p_rlnConj             = _pCard_RlnConj(pCard);break;
    case CK_ROWCOUNT: E.p_rowCount           = _pCard_RowCount(pCard); break;
    case CK_ROWSIZE: E.p_rowSize             = _pCard_RowSize(pCard); break;
    case CK_o_RRFJOIN: E.p_rrfJoin           = _pCard_RrfJoin(pCard); break;

    case CK_o_SCAN: E.p_scan                 = _pCard_Scan(pCard); break;
    case CK_SECTION: E.p_section             = _pCard_Section(pCard); break;
    case CK_o_SEND: E.p_send                 = _pCard_Send(pCard); break;
    case CK_o_SORT: E.p_sort                 = _pCard_Sort(pCard); break;
    case CK_SPARE: E.p_spare                 = _pCard_Spare(pCard); break;
    case CK_SQYCALLS: E.p_sqyCalls           = _pCard_SqyCalls(pCard); break;
    case CK_SQYINFO: E.p_sqyInfo             = _pCard_SqyInfo(pCard); break;
    case CK_SQYNO: E.p_sqyNo                 = _pCard_SqyNo(pCard); break;
    case CK_o_STAGE: E.p_stage               = _pCard_Stage(pCard); break;
    case CK_STREAMHEAD: E.p_streamHead       = _pCard_StreamHead(pCard); break;
    case CK_STREAMJOIN: E.p_streamJoin       = _pCard_StreamJoin(pCard); break;
    case CK_p_STREAMKEY: E.p_streamKey       = _pCard_StreamKey(pCard); break;
    case CK_STREAMPER: E.p_streamPer         = _pCard_StreamPer(pCard); break;
    case CK_STREAMPTN: E.p_streamPtn         = _pCard_StreamPtn(pCard); break;
    case CK_STREAMSET: E.p_streamSet         = _pCard_StreamSet(pCard); break;
    case CK_SUBQUERY: E.p_subQuery           = _pCard_SubQuery(pCard); break;
    case CK_SUBSET: E.p_subSet               = _pCard_SubSet(pCard); break;

    case CK_p_TABLE: E.p_table               = _pCard_Table(pCard); break;
    case CK_TABLELIST: E.p_tableList         = _pCard_TableList(pCard); break;
    case CK_TABLENM: E.p_tableNm             = _pCard_TableNm(pCard); break;
    case CK_TABLEPTN: E.p_tablePtn           = _pCard_TablePtn(pCard); break;
    case CK_TABLESIZE: E.p_tableSize         = _pCard_TableSize(pCard); break;
    case CK_TERM: E.p_term                   = _pCard_Term(pCard); break;
    case CK_o_TOP: E.p_top                   = _pCard_Top(pCard); break;
    case CK_TOPOLOGY: E.p_topology           = _pCard_Topology(pCard); break;

    case CK_UNIQUE: E.p_unique               = _pCard_Unique(pCard); break;

    case CK_o_WHERE: E.p_where               = _pCard_Where(pCard); break;

    case CK_XCARD: E.p_xcard                 = _pCard_XCard(pCard); break;

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

  RASSERT(E.xEachOp.bWithinRange());

  if (E.xEachOp.bMore())
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool CardIter::bMoreTarget(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(E.xTarget.bWithinRange());

  if (E.xTarget.bMore())
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* create a new card and optionally set member                                */
/*----------------------------------------------------------------------------*/
void CardIter::vNewMember(eCK eCard)
{
  _precondition_(_MAGIC_, _FOOTER_);

  Card* pNew = new Card(eCard,C_UNKNOWN);

  vMember(eCard, pNew);
}

/*----------------------------------------------------------------------------*/
/* step to next main and set member                                           */
/*----------------------------------------------------------------------------*/
void CardIter::vNewMember(eCK eCard, Symbol xSymbol)
{
  _precondition_(_MAGIC_, _FOOTER_);

  Card* pNew = new Card(eCard,C_UNKNOWN);

  vMember(eCard, pNew);

  pNew->pSymbol()->vApplyCastFacets(&xSymbol);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
eCK CardIter::ePeekNext(void)
{
  _precondition_(_MAGIC_, _FOOTER_);

  if (Card* pNext = pPeekNext())
    return pNext->eCard();

  return CK_NONE;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardIter::pPeekNext(void)
{
  _precondition_(_MAGIC_, _FOOTER_);

  i8 i_Next = E.xEachOp.i_Child() + 1;

  if (i_Next >= E.xEachOp.pParent()->nChildCards())
    return nullptr;

  Card* pNextMain = E.xEachOp.pParent()->pGetChild(i_Next);

  return pNextMain;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardIter::pPeekNextIf(eCK eCard)
{
  _precondition_(_MAGIC_, _FOOTER_);

  Card* pNextChild = this->pPeekNext();
  if (! pNextChild || eCard != pNextChild->eCard())
    return nullptr;

  return pNextChild;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardIter::pPeekPrevIf(eCK eCard)
{
  _precondition_(_MAGIC_, _FOOTER_);

  Card* pPrevChild = this->pPeekPrev();
  if (! pPrevChild || eCard != pPrevChild->eCard())
    return nullptr;

  return pPrevChild;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardIter::pPeekPrev(void)
{
  _precondition_(_MAGIC_, _FOOTER_);

  Card* pChild = E.xEachOp.pPrev();

  return pChild;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vPopEach(void)
{
  _precondition_(_MAGIC_, _FOOTER_);

  EachMembers* pMembers_popped = pPop();

  E.vCopyFrom(pMembers_popped);

  delete pMembers_popped;
}

/*----------------------------------------------------------------------------*/
/* position on given card of sequence                                         */
/*----------------------------------------------------------------------------*/
bool CardIter::bPositionAt(eCK eKind_at, i8 /* eCK */  iKind_First, ...)
{
  _precondition_(_MAGIC_, _FOOTER_);

  Card* pCard_found = nullptr;

  va_list xaArgs;
  va_start(xaArgs,iKind_First);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // find first card

  eCK eKind_first = (eCK)iKind_First;
  while (bEachOp()) {
    if (eKind_first == E.e_Op)
      break;
  }

  if (! E.e_Op) {
    va_end(xaArgs);
    return false;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // each card in sequence

  eCK eKind_each;

  if (eKind_at == E.e_Op) {
    pCard_found = E.p_Op;
  }

  for (eKind_each = (eCK)va_arg(xaArgs, i8);
       CK_NONE != eKind_each;
       eKind_each = (eCK)va_arg(xaArgs, i8)) {

    if (! bEachOp()) {
      va_end(xaArgs);
      return false;
    }

    if (! pCard_found && eKind_at == E.e_Op) {
      pCard_found = E.p_Op;
    }
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // did we find card in valid sequence?

  va_end(xaArgs);

  if (pCard_found) {
    vChild(pCard_found);
    E.p_Op = E.xEachOp.p_op();
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

  E.pBoard->vRenumberCards();

  E.pBoard->vPrintBoard(this);


  s32 sOT;

  bool bPrinted = false;
  i8 nPrinted = 0;
  for (i8 i_OT = 0; i_OT < OT_ENUMS; i_OT++) {
    Stz *szOptimisation = aOT[i_OT];

    if (! E.pBoard->pState()->bHasName(szOptimisation))
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
/* resolve prior emit (find right-to-left)                                    */
/*----------------------------------------------------------------------------*/
Card_Emit* CardIter::pPrior_emit(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  Card_Emit* p_emit = Card_Emit::pPrior_emit(this);

  return p_emit;
}

/*----------------------------------------------------------------------------*/
/* resolve prior expr of given term (find right-to-left)                      */
/*----------------------------------------------------------------------------*/
Card_Expr* CardIter::pPrior_expr(Symbol* pSymbol_arg)
{
  _precondition_( _MAGIC_, _FOOTER_);

  Card_Emit* p_arriving = pPrior_emit();
  if (! p_arriving)
    return nullptr;

  //RASSERT(p_arriving == this->Arriving.m_pEmitted_prior);

  Card_Expr* p_expr_resolved = p_arriving->pFind_expr(pSymbol_arg);
  return p_expr_resolved;
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
void CardIter::vPushEach(void)
{
  EachMembers* pMembers_save = new EachMembers;

  pMembers_save->vCopyFrom(&E);

  vPush(pMembers_save);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vPushEach(Board* pBoard)
{
  vPushEach();
  vUseBoard(pBoard);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vPushEach(Card* pParent)
{
  vPushEach();
  vUseCard(pParent);
}

/*----------------------------------------------------------------------------*/
/* remove main card from deck and delete it; clear M member to null           */
/*----------------------------------------------------------------------------*/
void CardIter::vRemoveDeleteMain(eCK eCrossCheck)
{
  _precondition_(_MAGIC_, _FOOTER_);

  RASSERT(E.xEachOp.bWithinRange());
  RASSERT(eCrossCheck == E.xEachOp.eChild());

  Card* pChild = E.xEachOp.pChild();
  E.xEachOp.pParent()->vRemoveChild(pChild);
  delete pChild;

  E.xEachOp.vBackstep();

  E.p_Op = nullptr;
  E.e_Op = CK_NONE;

  vMember(eCrossCheck, nullptr);
}

/*----------------------------------------------------------------------------*/
/* clear the M members                                                        */
/*----------------------------------------------------------------------------*/
void CardIter::vResetMembers(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  E.p_absJoin          = nullptr;
  E.p_address          = nullptr;
  E.p_aggreg           = nullptr;
  E.p_andConj          = nullptr;

  E.p_bound            = nullptr;
  E.p_bounds           = nullptr;

  E.p_call             = nullptr;
  E.p_cast             = nullptr;
  E.p_rlnConj          = nullptr;
  E.p_constraint       = nullptr;
  E.p_corrCols         = nullptr;
  E.p_correlation      = nullptr;
  E.p_costJoining      = nullptr;
  E.p_costNetwork      = nullptr;
  E.p_costQuery        = nullptr;
  E.p_costUnicast      = nullptr;
  E.p_count            = nullptr;
  E.p_crossJoin        = nullptr;

  E.p_decision         = nullptr;
  E.p_disposition      = nullptr;

  E.p_emit             = nullptr;
  E.p_expr             = nullptr;

  E.p_filter           = nullptr;
  E.p_filtPred         = nullptr;
  E.p_filtPredList     = nullptr;
  E.p_fold             = nullptr;
  E.p_from             = nullptr;

  E.p_gbyAgg           = nullptr;
  E.p_globalHash       = nullptr;

  E.p_hash             = nullptr;
  E.p_hashing          = nullptr;
  E.p_having           = nullptr;
  E.p_hshCol           = nullptr;
  E.p_hshColList       = nullptr;

  E.p_idxCol           = nullptr;
  E.p_idxColList       = nullptr;
  E.p_index            = nullptr;
  E.p_indexJoin        = nullptr;
  E.p_indexList        = nullptr;
  E.p_indexNm          = nullptr;
  E.p_indexScan        = nullptr;
  E.p_inflate          = nullptr;
  E.p_injectable       = nullptr;

  E.p_joinCast         = nullptr;
  E.p_joinOrder        = nullptr;
  E.p_joinOrdering     = nullptr;
  E.p_joinPredList     = nullptr;

  E.p_key              = nullptr;
  E.p_keyCells         = nullptr;
  E.p_keyExists        = nullptr;
  E.p_keyNotExists     = nullptr;

  E.p_level            = nullptr;
  E.p_limit            = nullptr;

  E.p_joinEstimate     = nullptr;

  E.p_map              = nullptr;
  E.p_mapping          = nullptr;

  E.p_needed           = nullptr;
  E.p_neededv2         = nullptr;
  E.p_note             = nullptr;
  E.p_notConj          = nullptr;

  E.p_on               = nullptr;
  E.p_ordAgg           = nullptr;
  E.p_orConj           = nullptr;
  E.p_outerColNm       = nullptr;
  E.p_outerJoin        = nullptr;

  E.p_payload          = nullptr;
  E.p_payloadCells     = nullptr;
  E.p_pdsRow           = nullptr;
  E.p_predicate        = nullptr;
  E.p_project          = nullptr;

  E.p_relnOp           = nullptr;
  E.p_required         = nullptr;
  E.p_rowCount         = nullptr;
  E.p_rowSize          = nullptr;
  E.p_rrfJoin          = nullptr;

  E.p_scan             = nullptr;
  E.p_section          = nullptr;
  E.p_send             = nullptr;
  E.p_spare            = nullptr;
  E.p_sort             = nullptr;
  E.p_sqyCalls         = nullptr;
  E.p_sqyInfo          = nullptr;
  E.p_sqyNo            = nullptr;
  E.p_streamKey        = nullptr;
  E.p_streamPer        = nullptr;
  E.p_streamPtn        = nullptr;
  E.p_streamSet        = nullptr;
  E.p_stage            = nullptr;
  E.p_streamHead       = nullptr;
  E.p_streamJoin       = nullptr;
  E.p_subQuery         = nullptr;
  E.p_subSet           = nullptr;

  E.p_table            = nullptr;
  E.p_tableList        = nullptr;
  E.p_tableNm          = nullptr;
  E.p_tablePtn         = nullptr;
  E.p_tableSize        = nullptr;
  E.p_term             = nullptr;
  E.p_top              = nullptr;
  E.p_topology         = nullptr;

  E.p_unique           = nullptr;

  E.p_where            = nullptr;

  E.p_xcard            = nullptr;
}

/*----------------------------------------------------------------------------*/
/* step main register next main and update M member                           */
/*----------------------------------------------------------------------------*/
bool CardIter::bRewindOp(void)
{
  _precondition_(_MAGIC_, _FOOTER_);

  E.xEachOp.vBackstep();

  if ((E.p_Op = E.xEachOp.p_op())) {
    vFocusOnOp();
    vMember(E.e_Op, E.p_Op);
    return true;
  }

  E.e_Op = CK_NONE;
  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vRepositionOp(void)
{
  _precondition_(_MAGIC_, _FOOTER_);

  E.xEachOp.vReposition(E.p_Op);
  RASSERT(E.p_Op == E.xEachOp.pChild());
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vRepositionOp(Card_OP* p_Op)
{
  _precondition_(_MAGIC_, _FOOTER_);

  E.p_Op = p_Op;
  vRepositionOp();
}

/*----------------------------------------------------------------------------*/
/* snap emit                                                                  */
/*----------------------------------------------------------------------------*/
void CardIter::vSnap_arriving(Card_Emit* p_emit, Card_StreamKey* pStreamKey)
{
  _precondition_(_MAGIC_, _FOOTER_);

  Arriving.m_pEmitted_prior = p_emit;
  Dic.m_p_expr = p_emit;

  Arriving.m_pStreamKey = pStreamKey;
}

/*----------------------------------------------------------------------------*/
/* snap emit                                                                  */
/*----------------------------------------------------------------------------*/
void CardIter::vSnapIf_arriving(Card_Emit* p_emit, Card_StreamKey * pStreamKey)
{
  _precondition_(_MAGIC_, _FOOTER_);

  if (p_emit) {
    Arriving.m_pEmitted_prior = p_emit;
    Dic.m_p_expr = p_emit;

    Arriving.m_pStreamKey = pStreamKey;
  }
}

/*----------------------------------------------------------------------------*/
/* snap emit                                                                  */
/*----------------------------------------------------------------------------*/
void CardIter::vSnapIf_arriving(bool bSub_emit)
{
  _precondition_(_MAGIC_, _FOOTER_);

  if (E.p_Op->bIsCard(CK_o_FOLD))
    return;

  if (bSub_emit) {
    vSnapIf_arriving(E.p_Op->pSub_emit(),E.p_Op->pSub_streamKey());
  }
  else {
    vSnapIf_arriving(E.p_emit,E.p_Op->pSub_streamKey());
  }
}

/*----------------------------------------------------------------------------*/
/* snap emit                                                                  */
/*----------------------------------------------------------------------------*/
void CardIter::vSnapIf_arriving_till(eCK eTill)
{
  _precondition_(_MAGIC_, _FOOTER_);

  for (vBeginEach(); bEachOp(); /**/) {

    if (eTill == E.e_Op)
      break;

    vSnapIf_arriving();
  }

  RASSERT(eTill == E.e_Op);
}

/*----------------------------------------------------------------------------*/
/* begin and record transformation information                                */
/*----------------------------------------------------------------------------*/
void CardIter::vTransform(const char* azXfile, const char* azXfunc, eXF eXform)
{
  _precondition_( _MAGIC_, _FOOTER_);

  vClearRegisters();

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

  E.pBoard = pBoard;
  E.p_deck  = E.pBoard->pDeck();

  vUseCard(E.pBoard->pDeck());
}

/*----------------------------------------------------------------------------*/
/* which card to use                                                          */
/*----------------------------------------------------------------------------*/
void CardIter::vUseCard(Card* pCard)
{
  _precondition_(_MAGIC_, _FOOTER_);

  E.xEachOp.vParent(pCard);

  E.p_Op = nullptr;
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

  for (Kid xCard(pCard); xCard.bEach_opaque(); /**/) {

    Card* pChild = xCard.pChild();

    eCK eChild = pChild->eCard();

    vMember(eChild, pChild);

    switch (eChild) {

      case CK_o_ABSJOIN:      E.p_absJoin->vValidate(this, iDepth, pCard);  break;
      case CK_ADDRESS:        E.p_address->vValidate(this, iDepth, pCard);  break;
      case CK_o_AGGREG:       E.p_aggreg->vValidate(this, iDepth, pCard);  break;
      case CK_ANDCONJ:        E.p_andConj->vValidate(this, iDepth, pCard);  break;

      case CK_PREDICATE:      E.p_predicate->vValidate(this, iDepth, pCard);  break;
      case CK_BOUND:          E.p_bound->vValidate(this, iDepth, pCard);  break;
      case CK_BOUNDS:         E.p_bounds->vValidate(this, iDepth, pCard);  break;

      case CK_o_CALL:         E.p_call->vValidate(this, iDepth, pCard);  break;
      case CK_CALLER:         E.p_caller->vValidate(this, iDepth, pCard);  break;
      case CK_CALLOP:         E.p_callOp->vValidate(this, iDepth, pCard);  break;
      case CK_o_CAST:         E.p_cast->vValidate(this, iDepth, pCard);  break;
      case CK_CLAUSE:         E.p_clause->vValidate(this, iDepth, pCard);  break;
      case CK_p_CONSTRAINT:   E.p_constraint->vValidate(this, iDepth, pCard);  break;
      case CK_CORRCOLS:       E.p_corrCols->vValidate(this, iDepth, pCard);  break;
      case CK_o_CORRELATION:  E.p_correlation->vValidate(this, iDepth, pCard);  break;
      case CK_COSTJOINING:    E.p_costJoining->vValidate(this, iDepth, pCard);  break;
      case CK_COSTNETWORK:    E.p_costNetwork->vValidate(this, iDepth, pCard);  break;
      case CK_COSTSTREAM:     E.p_costQuery->vValidate(this, iDepth, pCard);  break;
      case CK_COSTUNICAST:    E.p_costUnicast->vValidate(this, iDepth, pCard);  break;
      case CK_COUNT:          E.p_count->vValidate(this, iDepth, pCard);  break;
      case CK_o_CROSSJOIN:    E.p_crossJoin->vValidate(this, iDepth, pCard);  break;

      case CK_p_DECISION:     E.p_decision->vValidate(this, iDepth, pCard);  break;
      case CK_DISPOSITION:    E.p_disposition->vValidate(this, iDepth, pCard);  break;

      case CK_p_EMIT:         E.p_emit->vValidate(this, iDepth, pCard);  break;
      case CK_EXPR:           E.p_expr->vValidate(this, iDepth, pCard);  break;

      case CK_o_FILTER:       E.p_filter->vValidate(this, iDepth, pCard);  break;
      case CK_FILTPRED:       E.p_filtPred->vValidate(this, iDepth, pCard);  break;
      case CK_FILTPREDLIST:   E.p_filtPredList->vValidate(this, iDepth, pCard);  break;
      case CK_o_FOLD:         E.p_fold->vValidate(this, iDepth, pCard);  break;
      case CK_o_FROM:         E.p_from->vValidate(this, iDepth, pCard);  break;

      case CK_o_GBYAGG:       E.p_gbyAgg->vValidate(this, iDepth, pCard);  break;
      case CK_o_GLOBALHASH:   E.p_globalHash->vValidate(this, iDepth, pCard);  break;

      case CK_o_HASH:         E.p_hash->vValidate(this, iDepth, pCard);  break;
      case CK_p_HASHING:      E.p_hashing->vValidate(this, iDepth, pCard);  break;
      case CK_o_HAVING:       E.p_having->vValidate(this, iDepth, pCard);  break;
      case CK_HSHCOL:         E.p_hshCol->vValidate(this, iDepth, pCard);  break;
      case CK_p_HSHCOLLIST:   E.p_hshColList->vValidate(this, iDepth, pCard);  break;

      case CK_IDXCOL:         E.p_idxCol->vValidate(this, iDepth, pCard);  break;
      case CK_IDXCOLLIST:     E.p_idxColList->vValidate(this, iDepth, pCard);  break;
      case CK_INDEX:          E.p_index->vValidate(this, iDepth, pCard);  break;
      case CK_o_INDEXJOIN:    E.p_indexJoin->vValidate(this, iDepth, pCard);  break;
      case CK_INDEXLIST:      E.p_indexList->vValidate(this, iDepth, pCard);  break;
      case CK_INDEXNM:        E.p_indexNm->vValidate(this, iDepth, pCard);  break;
      case CK_o_INDEXSCAN:    E.p_indexScan->vValidate(this, iDepth, pCard);  break;
      case CK_o_INFLATE:      E.p_inflate->vValidate(this, iDepth, pCard);  break;
      case CK_p_INJECTABLE:   E.p_injectable->vValidate(this, iDepth, pCard);  break;

      case CK_JOINCAST:       E.p_joinCast->vValidate(this, iDepth, pCard);  break;
      case CK_JOINESTIMATE:   E.p_joinEstimate->vValidate(this, iDepth, pCard);  break;
      case CK_JOINORDER:      E.p_joinOrder->vValidate(this, iDepth, pCard);  break;
      case CK_JOINORDERING:   E.p_joinOrdering->vValidate(this, iDepth, pCard);  break;
      case CK_JOINPREDLIST:   E.p_joinPredList->vValidate(this, iDepth, pCard);  break;

      case CK_p_KEY:          E.p_key->vValidate(this, iDepth, pCard);  break;
      case CK_KEYCELLS:       E.p_keyCells->vValidate(this, iDepth, pCard);  break;
      case CK_o_KEYEXISTS:    E.p_keyExists->vValidate(this, iDepth, pCard);  break;
      case CK_o_KEYNOTEXISTS: E.p_keyNotExists->vValidate(this, iDepth, pCard);  break;

      case CK_p_LEVEL:        E.p_level->vValidate(this, iDepth, pCard);  break;
      case CK_o_LIMIT:        E.p_limit->vValidate(this, iDepth, pCard);  break;

      case CK_MAP:            E.p_map->vValidate(this, iDepth, pCard);  break;
      case CK_p_MAPPING:      E.p_mapping->vValidate(this, iDepth, pCard);  break;

      case CK_p_NEEDED:       E.p_needed->vValidate(this, iDepth, pCard);  break;
      case CK_p_NEEDEDv2:     E.p_neededv2->vValidate(this, iDepth, pCard);  break;
      case CK_NOTE:           E.p_note->vValidate(this, iDepth, pCard);  break;
      case CK_NOTCONJ:        E.p_notConj->vValidate(this, iDepth, pCard);  break;

      case CK_ON:             E.p_on->vValidate(this, iDepth, pCard);  break;
      case CK_o_ORDAGG:       E.p_ordAgg->vValidate(this, iDepth, pCard);  break;
      case CK_ORCONJ:         E.p_orConj->vValidate(this, iDepth, pCard);  break;
      case CK_OUTERCOLNM:     E.p_outerColNm->vValidate(this, iDepth, pCard);  break;
      case CK_OUTERJOIN:      E.p_outerJoin->vValidate(this, iDepth, pCard);  break;

      case CK_PAYLOADCELLS:   E.p_payloadCells->vValidate(this, iDepth, pCard);  break;
      case CK_o_PDSROW:       E.p_pdsRow->vValidate(this, iDepth, pCard);  break;
      case CK_o_PROJECT:      E.p_project->vValidate(this, iDepth, pCard);  break;

      case CK_RELNOP:         E.p_relnOp->vValidate(this, iDepth, pCard);  break;
      case CK_RLNCONJ:        E.p_rlnConj->vValidate(this, iDepth, pCard);  break;
      case CK_ROWCOUNT:       E.p_rowCount->vValidate(this, iDepth, pCard);  break;
      case CK_ROWSIZE:        E.p_rowSize->vValidate(this, iDepth, pCard);  break;
      case CK_o_RRFJOIN:      E.p_rrfJoin->vValidate(this, iDepth, pCard);  break;

      case CK_o_SCAN:         E.p_scan->vValidate(this, iDepth, pCard);  break;
      case CK_SECTION:        E.p_section->vValidate(this, iDepth, pCard);  break;
      case CK_o_SEND:         E.p_send->vValidate(this, iDepth, pCard);  break;
      case CK_SPARE:          E.p_spare->vValidate(this, iDepth, pCard);  break;
      case CK_o_SORT:         E.p_sort->vValidate(this, iDepth, pCard);  break;
      case CK_SQYCALLS:       E.p_sqyCalls->vValidate(this, iDepth, pCard);  break;
      case CK_SQYINFO:        E.p_sqyInfo->vValidate(this, iDepth, pCard);  break;
      case CK_SQYNO:          E.p_sqyNo->vValidate(this, iDepth, pCard);  break;
      case CK_o_STAGE:        E.p_stage->vValidate(this, iDepth, pCard);  break;
      case CK_STREAMHEAD:     E.p_streamHead->vValidate(this, iDepth, pCard);  break;
      case CK_STREAMJOIN:     E.p_streamJoin->vValidate(this, iDepth, pCard);  break;
      case CK_p_STREAMKEY:    E.p_streamKey->vValidate(this, iDepth, pCard);  break;
      case CK_STREAMPER:      E.p_streamPer->vValidate(this, iDepth, pCard);  break;
      case CK_STREAMPTN:      E.p_streamPtn->vValidate(this, iDepth, pCard);  break;
      case CK_STREAMSET:      E.p_streamSet->vValidate(this, iDepth, pCard);  break;
      case CK_SUBQUERY:       E.p_subQuery->vValidate(this, iDepth, pCard);  break;
      case CK_SUBSET:         E.p_subSet->vValidate(this, iDepth, pCard);  break;

      case CK_p_TABLE:        E.p_table->vValidate(this, iDepth, pCard);  break;
      case CK_TABLELIST:      E.p_tableList->vValidate(this, iDepth, pCard);  break;
      case CK_TABLENM:        E.p_tableNm->vValidate(this, iDepth, pCard);  break;
      case CK_TABLEPTN:       E.p_tablePtn->vValidate(this, iDepth, pCard);  break;
      case CK_TABLESIZE:      E.p_tableSize->vValidate(this, iDepth, pCard);  break;
      case CK_TERM:           E.p_term->vValidate(this, iDepth, pCard);  break;
      case CK_o_TOP:          E.p_top->vValidate(this, iDepth, pCard);  break;
      case CK_TOPOLOGY:       E.p_topology->vValidate(this, iDepth, pCard);  break;

      case CK_UNIQUE:         E.p_unique->vValidate(this, iDepth, pCard);  break;

      case CK_o_WHERE:        E.p_where->vValidate(this, iDepth, pCard);  break;

      case CK_XCARD:          E.p_xcard->vValidate(this, iDepth, pCard);  break;

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