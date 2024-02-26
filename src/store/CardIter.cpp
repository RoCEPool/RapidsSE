/*============================================================================*/
/* CardIter.cpp --                                                            */
/*============================================================================*/

#include "CardIter.hpp"
#include "Cue.hpp"
#include "OptimiserFun.hpp"
#include "OptimiserCatalog.hpp"
#include "ShortForm.hpp"

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

  Sqy.m_pCard_sqyCopy = nullptr;

  Sqy.m_pExpr_fold = nullptr;
  Sqy.m_pExpr_corr = nullptr;

  Sqy.m_pCorrelation = nullptr;
  Sqy.m_i_SubQuery   = C_UNKNOWN;
  Sqy.m_pSubQuery    = nullptr;
  Sqy.m_pTerm_sqyArg  = nullptr;
  Sqy.m_pTerm_sqyCall = nullptr;
  Sqy.m_pConj_sqyCall = nullptr;

  // the M iterator - - - - - - - - - - - - - - - - - - - - - -

  G.pBoard    = nullptr;
  G.pDeck     = nullptr;

  m_nAncestors = 0;
  MemLib::vZero(m_apAncestry,sizeof(m_apAncestry),CN_CARDITER);

  vNullMembers();
  G_vClearRegisters();

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
    bPopRecurseIter();

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
void CardIter::G_vAppendChild(eCK eParent, eCK eChild)
{
  _precondition_( _MAGIC_, _FOOTER_);

  Card* pParent = G_pGetMember(eParent);
  Card* pChild  = G_pGetMember(eChild);

  pParent->vAppendChild(pChild);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::G_vAppendChild(eCK eParent, Card* pChild)
{
  _precondition_(_MAGIC_,_FOOTER_);

  Card* pParent = G_pGetMember(eParent);

  pParent->vAppendChild(pChild);

  G_vMember(pChild->eCard(), pChild, __func__);

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::G_vAppendNewChild(eCK eParent, eCK eChild)
{
  _precondition_( _MAGIC_, _FOOTER_);

  Card* pParent = G_pGetMember(eParent);
  Card* pChild = new Card(eChild,C_UNKNOWN);

  pParent->vAppendChild(pChild);
  G_vMember(eChild, pChild, __func__);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::G_vAppendNewChild(Card* pParent, eCK eChild)
{
  _precondition_(_MAGIC_, _FOOTER_);

  Card* pChild = new Card(eChild,C_UNKNOWN);

  pParent->vAppendChild(pChild);
  G_vMember(eChild, pChild, __func__);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::G_vAppendNewChild(Card* pParent, eCK eChild, Symbol* pSymbol)
{
  _precondition_(_MAGIC_,_FOOTER_);

  Card* pChild = new Card(eChild,C_UNKNOWN);

  pChild->pSymbol()->vApplyCastFacetsPpy(pSymbol);

  pParent->vAppendChild(pChild);
  G_vMember(eChild, pChild, __func__);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::G_vAppendNewChild(Card* pParent, eCK eChild, Symbol xSymbol)
{
  _precondition_(_MAGIC_,_FOOTER_);

  Card* pChild = new Card(eChild,C_UNKNOWN);

  pChild->pSymbol()->vApplyCastFacets(&xSymbol);

  pParent->vAppendChild(pChild);
  G_vMember(eChild, pChild, __func__);
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
/* find ancestor                                                             */
/*----------------------------------------------------------------------------*/
Card* CardIter::pAncestorFind(eCK eCard, i8* ai_Ancestor)
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
/* test ancestry                                                              */
/*----------------------------------------------------------------------------*/
bool CardIter::bAncestorIncludes(eCK eCard)
{
  _precondition_(_MAGIC_, _FOOTER_);

  if (pAncestorFind(eCard))
    return true;

  return false;
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
/* get board                                                                  */
/*----------------------------------------------------------------------------*/
Board* CardIter::pBoard(void)
{
  _precondition_(_MAGIC_,_FOOTER_);

  return G.pBoard;
}

/*----------------------------------------------------------------------------*/
/* set board                                                                  */
/*----------------------------------------------------------------------------*/
void CardIter::vBoard(Board* pBoard)
{
  _precondition_(_MAGIC_,_FOOTER_);

  G.pBoard = pBoard;
}

/*----------------------------------------------------------------------------*/
/* setup board iteration                                                      */
/*----------------------------------------------------------------------------*/
void CardIter::vBoardIteration(void)
{
  _precondition_(_MAGIC_,_FOOTER_);

  G.i_Board = C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/* burst OP                                                                   */
/*----------------------------------------------------------------------------*/
void CardIter::vBurst(Card* pCard)
{
  _precondition_(_MAGIC_,_FOOTER_);

  for (Kid xCard(pCard); xCard.bEach(); /**/)
    vMember(xCard.pChild());
}

/*----------------------------------------------------------------------------*/
/* burst OP                                                                   */
/*----------------------------------------------------------------------------*/
void CardIter::vBurstOP(void)
{
  _precondition_(_MAGIC_,_FOOTER_);

  vBurst(G.p_OP);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vChangeCard(Card* pCard)
{
  _precondition_(_MAGIC_, _FOOTER_);

  G.xKid.vParent(pCard);

  G.p_MainCard = nullptr;
  vMember(pCard);

  //vBegin();
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
/* clear the general M registers                                              */
/*----------------------------------------------------------------------------*/
void CardIter::G_vClearRegisters(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  MemLib::vZero(G.md_Coord,sizeof(G.md_Coord),CN_CARDITER);

  G.p_MainCard         = nullptr;
  G.e_MainCard         = CK_NONE;
  G.i_MainCard         = C_UNKNOWN;
  G.p_SubOfMain        = nullptr;
  G.e_SubOfMain        = CK_NONE;
  G.p_Sub2OfMain       = nullptr;
  G.e_Sub2OfMain       = CK_NONE;
  G.p_Sub3OfMain       = nullptr;
  G.e_Sub3OfMain       = CK_NONE;

  Sqy.m_pTerm_corrCol  = nullptr;

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

  Sqy.m_i_FoldCard = C_UNKNOWN;

}

/*----------------------------------------------------------------------------*/
/* clear the general M registers if they match given pointer                  */
/*----------------------------------------------------------------------------*/
void CardIter::G_vClearRegistersIf(Card* pCard)
{
  _precondition_(_MAGIC_, _FOOTER_);

  if (pCard == G.p_MainCard) {
    G.p_MainCard = nullptr;
    G.e_MainCard = CK_NONE;
  }
  if (pCard == G.p_SubOfMain) {
    G.p_SubOfMain = nullptr;
    G.e_SubOfMain = CK_NONE;
  }
  if (pCard == G.p_Sub2OfMain) {
    G.p_Sub2OfMain = nullptr;
    G.e_Sub2OfMain = CK_NONE;
  }
  if (pCard == G.p_Sub3OfMain) {
    G.p_Sub3OfMain = nullptr;
    G.e_Sub3OfMain = CK_NONE;
  }
}

/*----------------------------------------------------------------------------*/
/* restart the M iterator                                                     */
/*----------------------------------------------------------------------------*/
void CardIter::vClearRestart(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  this->vBeginEach();

  vNullMembers();
  G_vClearRegisters();
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
void CardIter::G_vDeleteMember(Card* pMember)
{
  _precondition_(_MAGIC_, _FOOTER_);

  eCK eMember = pMember->eCard();

  G_vClearRegistersIf(pMember);
  G_vMember(eMember, nullptr, __func__);

  DELETE_CARD(pMember);
}

/*----------------------------------------------------------------------------*/
/* delete main when given sub is empty                                        */
/*----------------------------------------------------------------------------*/
void CardIter::G_vDeleteWhenEmptySub(eCK eMain, eCK eSub)
{
  _precondition_(_MAGIC_, _FOOTER_);

  if (! G_bEachMainWithSub(eMain,eSub))
    return;

  G_vMember(G.e_MainCard, G.p_MainCard, __func__);
  G.e_SubOfMain = G.p_SubOfMain->eCard();


  if (G.p_SubOfMain->nChildCards())
    return;

  G_vRemoveDeleteMain(eMain);
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
    vUsingBoard(G.i_Board);
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

  vChangeCard(G.pBoard->pDeck());

  return G.pBoard;
}

/*----------------------------------------------------------------------------*/
/* conditionally step main register to next main and update M member          */
/*----------------------------------------------------------------------------*/
bool CardIter::G_bEachMain_NONE(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(G.xKid.bBegun());
  RASSERT(! G.xKid.bEnded());

  G.p_OP = nullptr;

  while (G.xKid.bEach()) {
    if (G.xKid.bParentIs(CK_DECK) && ! G.xKid.pChild()->bIsMain())
      continue;

    G.p_MainCard = G.xKid.pChild();
    vFocusOnMain();
    G_vMember(G.p_MainCard->eCard(), G.p_MainCard, __func__);
    return true;
  }

  G.p_MainCard = nullptr;
  G.xKid.vEnded();
  return false;
}

/*----------------------------------------------------------------------------*/
/* conditionally step main register to next main and update M member          */
/*----------------------------------------------------------------------------*/
bool CardIter::G_bEachMain(eCK eOfMainKind)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(G.xKid.bBegun());
  RASSERT(! G.xKid.bEnded());

  RASSERT(Card_MAIN::bIsMain(eOfMainKind));

  G.p_OP = nullptr;

  while (G.xKid.bEach()) {

    if (G.xKid.pChild()->eCard() != eOfMainKind)
      continue;

    G.p_MainCard = G.xKid.pChild();
    vFocusOnMain();
    G_vMember(G.p_MainCard->eCard(), G.p_MainCard, __func__);
    return true;
  }

  G.p_MainCard = nullptr;
  G.xKid.vEnded();
  return false;
}

/*----------------------------------------------------------------------------*/
/* conditionally step main register to next main and update M member          */
/*----------------------------------------------------------------------------*/
bool CardIter::G_bEachMain_foldBreak(eCK eOfMainKind)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(G.xKid.bBegun());
  RASSERT(! G.xKid.bEnded());

  RASSERT(Card_MAIN::bIsMain(eOfMainKind));

  G.p_OP = nullptr;

  while (G.xKid.bEach()) {

    if (G.xKid.pChild()->bIsCard(CK_c_FOLD))
      return false;

    if (G.xKid.pChild()->eCard() != eOfMainKind)
      continue;

    G.p_MainCard = G.xKid.pChild();
    vFocusOnMain();
    G_vMember(G.p_MainCard->eCard(), G.p_MainCard, __func__);
    return true;
  }

  G.p_MainCard = nullptr;
  G.xKid.vEnded();
  return false;

}

/*----------------------------------------------------------------------------*/
/* conditionally step main register to next main which has sub and update M   */
/*----------------------------------------------------------------------------*/
bool CardIter::G_bEachMainWithSub(eCK eOfMainKind, eCK eAndSubKind)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(! Card_MAIN::bIsMain(eAndSubKind) || CK_o_FILTER == eAndSubKind);

  while (G_bEachMain(eOfMainKind)) {
    if (! (G.p_SubOfMain = G.p_MainCard->pFindSubCard(eAndSubKind)))
      continue;

    G.e_SubOfMain = G.p_SubOfMain->eCard();
    G_vMember(G.e_SubOfMain, G.p_SubOfMain, __func__);
    return true;
  }

  RASSERT(G.xKid.bEnded());

  G.p_SubOfMain = nullptr;
  G.e_SubOfMain = CK_NONE;
  G_vMember(eAndSubKind, nullptr, __func__);

  return false;
}

/*----------------------------------------------------------------------------*/
/* conditionally step main register to next main which has sub and update M   */
/*----------------------------------------------------------------------------*/
bool CardIter::G_bEachMainWithSub(eCK eOfMainKind, eCK eAndSubKind, Stz* szSubValue)
{
  _precondition_( _MAGIC_, _FOOTER_);

  while (G_bEachMainWithSub(eOfMainKind, eAndSubKind)) {
    if (G.p_SubOfMain->pSymbol()->szFacet((i8)0)->bSame(szSubValue)) {
      RASSERT(G.p_SubOfMain->pSymbol()->bSingleFaceted());
      return true;
    }
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* conditionally step main register to next main which has sub and update M   */
/*----------------------------------------------------------------------------*/
bool CardIter::G_bEachMainWith2Sub(eCK eOfMainKind,
                                   eCK eAndSubKind1,
                                   eCK eAndSubKind2)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(! Card_MAIN::bIsMain(eAndSubKind2) || CK_o_FILTER == eAndSubKind2);

  while (G_bEachMainWithSub(eOfMainKind,eAndSubKind1)) {
    if (! (G.p_Sub2OfMain = G.p_MainCard->pFindSubCard(eAndSubKind2)))
      continue;

    G.e_Sub2OfMain = G.p_Sub2OfMain->eCard();
    G_vMember(G.e_Sub2OfMain, G.p_Sub2OfMain, __func__);
    return true;
  }

  RASSERT(G.xKid.bEnded());

  G.p_Sub2OfMain = nullptr;
  G.e_Sub2OfMain = CK_NONE;
  G_vMember(eAndSubKind2, nullptr, __func__);

  return false;
}

/*----------------------------------------------------------------------------*/
/* conditionally step main register to next main which has sub and update M   */
/*----------------------------------------------------------------------------*/
bool CardIter::G_bEachMainWith3Sub(eCK eOfMainKind,
                                   eCK eAndSubKind1,
                                   eCK eAndSubKind2,
                                   eCK eAndSubKind3)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(! Card_MAIN::bIsMain(eAndSubKind3));

  while (G_bEachMainWith2Sub(eOfMainKind,eAndSubKind1,eAndSubKind2)) {
    if (! (G.p_Sub3OfMain = G.p_MainCard->pFindSubCard(eAndSubKind3)))
      continue;

    G.e_Sub3OfMain = G.p_Sub3OfMain->eCard();
    G_vMember(G.e_Sub3OfMain, G.p_Sub3OfMain, __func__);
    return true;
  }

  RASSERT(G.xKid.bEnded());

  G.p_Sub3OfMain = nullptr;
  G.e_Sub3OfMain = CK_NONE;
  G_vMember(eAndSubKind3, nullptr, __func__);

  return false;

}

/*----------------------------------------------------------------------------*/
/* conditionally step main register to next main and update M member          */
/*----------------------------------------------------------------------------*/
bool CardIter::G_bEachMain_reverse_NONE(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(!bForwards());

  RASSERT(G.xKid.bBegun());
  RASSERT(! G.xKid.bEnded());

  G.p_OP = nullptr;

  while (G.xKid.bEach_reverse()) {
    if (G.xKid.bParentIs(CK_DECK) && ! G.xKid.pChild()->bIsMain())
      continue;

    G.p_MainCard = G.xKid.pChild();
    vFocusOnMain();
    G_vMember(G.p_MainCard->eCard(), G.p_MainCard, __func__);
    return true;
  }

  G.p_MainCard = nullptr;
  G.xKid.vEnded();
  return false;
}

/*----------------------------------------------------------------------------*/
/* conditionally step main register to next main and update M member          */
/*----------------------------------------------------------------------------*/
bool CardIter::G_bEachMain_reverse_b(eCK eOfMainKind)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(!bForwards());

  RASSERT(G.xKid.bBegun());
  RASSERT(! G.xKid.bEnded());

  RASSERT(Card_MAIN::bIsMain(eOfMainKind));

  G.p_OP = nullptr;

  while (G.xKid.bEach_reverse()) {
    if (G.xKid.pChild()->eCard() != eOfMainKind)
      continue;

    G.p_MainCard = G.xKid.pChild();
    vFocusOnMain();
    G_vMember(G.p_MainCard->eCard(), G.p_MainCard, __func__);
    return true;
  }

  G.p_MainCard = nullptr;
  G.xKid.vEnded();
  return false;
}

/*----------------------------------------------------------------------------*/
/* conditionally step main register to next main and update M member          */
/*----------------------------------------------------------------------------*/
bool CardIter::G_bEachMain_reverse_foldBreak_b(eCK eOfMainKind)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(!bForwards());

  RASSERT(G.xKid.bBegun());
  RASSERT(! G.xKid.bEnded());

  RASSERT(Card_MAIN::bIsMain(eOfMainKind));

  G.p_OP = nullptr;

  while (G.xKid.bEach_reverse()) {

    if (G.xKid.pChild()->bIsCard(CK_c_FOLD))
      return false;

    if (G.xKid.pChild()->eCard() != eOfMainKind)
      continue;

    G.p_MainCard = G.xKid.pChild();
    vFocusOnMain();
    G_vMember(G.p_MainCard->eCard(), G.p_MainCard, __func__);
    return true;
  }

  G.p_MainCard = nullptr;
  G.xKid.vEnded();
  return false;
}

/*----------------------------------------------------------------------------*/
/* conditionally step main register to next main which has sub and update G   */
/*----------------------------------------------------------------------------*/
bool CardIter::G_bEachMainWithSub_reverse(eCK eOfMainKind, eCK eAndSubKind)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(! Card_MAIN::bIsMain(eAndSubKind));

  while (G_bEachMain_reverse_b(eOfMainKind)) {
    if (! (G.p_SubOfMain = G.p_MainCard->pFindSubCard(eAndSubKind)))
      continue;

    vFocusOnMain();

    G_vMember(G.e_MainCard, G.p_MainCard, __func__);
    G.e_SubOfMain = G.p_SubOfMain->eCard();
    G_vMember(G.e_SubOfMain, G.p_SubOfMain, __func__);
    return true;
  }

  G.p_MainCard  = nullptr;
  G.e_MainCard  = CK_NONE;
  G_vMember(eOfMainKind, nullptr, __func__);
  G.p_SubOfMain = nullptr;
  G.e_SubOfMain = CK_NONE;
  G_vMember(eAndSubKind, nullptr, __func__);
  return false;
}

/*----------------------------------------------------------------------------*/
/* conditionally step main register to next main which has sub and update G   */
/*----------------------------------------------------------------------------*/
bool CardIter::G_bEachMainWithSub_reverse_foldBreak_b(eCK eOfMainKind, eCK eAndSubKind)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(! Card_MAIN::bIsMain(eAndSubKind) || CK_o_FILTER == eAndSubKind);

  while (G_bEachMain_reverse_foldBreak_b(eOfMainKind)) {
    if (! (G.p_SubOfMain = G.p_MainCard->pFindSubCard(eAndSubKind)))
      continue;

    vFocusOnMain();

    G_vMember(G.e_MainCard, G.p_MainCard, __func__);
    G.e_SubOfMain = G.p_SubOfMain->eCard();
    G_vMember(G.e_SubOfMain, G.p_SubOfMain, __func__);
    return true;
  }

  // did not find card
  G.p_MainCard  = nullptr;
  G.e_MainCard  = CK_NONE;
  G_vMember(eOfMainKind, nullptr, __func__);
  G.p_SubOfMain = nullptr;
  G.e_SubOfMain = CK_NONE;
  G_vMember(eAndSubKind, nullptr, __func__);
  return false;
}

/*----------------------------------------------------------------------------*/
/* step main register to next main which has sub and update M member          */
/*----------------------------------------------------------------------------*/
bool CardIter::G_bEachSubOfMain(eCK eSubKind)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(! Card_MAIN::bIsMain(eSubKind));

  while (G_bEachMain_NONE()) {
    if (! (G.p_SubOfMain = G.p_MainCard->pFindSubCard(eSubKind)))
      continue;

    vFocusOnMain();

    G_vMember(G.e_MainCard, G.p_MainCard, __func__);
    G.e_SubOfMain = G.p_SubOfMain->eCard();
    G_vMember(G.e_SubOfMain, G.p_SubOfMain, __func__);
    return true;
  }

  G.p_MainCard  = nullptr;
  G.e_MainCard  = CK_NONE;
  G.p_SubOfMain = nullptr;
  G.e_SubOfMain = CK_NONE;
  G_vMember(eSubKind, nullptr, __func__);
  return false;
}

/*----------------------------------------------------------------------------*/
/* step main register to next main which has sub and update M member          */
/*----------------------------------------------------------------------------*/
bool CardIter::G_bEach2SubOfMain(eCK eSubKind1, eCK eSubKind2)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(! Card_MAIN::bIsMain(eSubKind1));
  RASSERT(! Card_MAIN::bIsMain(eSubKind2));

  while (G_bEachMain_NONE()) {
    if (! (G.p_SubOfMain = G.p_MainCard->pFindSubCard(eSubKind1)))
      continue;

    if (! (G.p_Sub2OfMain = G.p_MainCard->pFindSubCard(eSubKind2)))
      continue;

    vFocusOnMain();

    G_vMember(G.e_MainCard, G.p_MainCard, __func__);
    G.e_SubOfMain = G.p_SubOfMain->eCard();
    G_vMember(G.e_SubOfMain, G.p_SubOfMain, __func__);
    G.e_Sub2OfMain = G.p_Sub2OfMain->eCard();
    G_vMember(G.e_Sub2OfMain, G.p_Sub2OfMain, __func__);
    return true;
  }

  G.p_MainCard  = nullptr;
  G.e_MainCard  = CK_NONE;
  G.p_SubOfMain = nullptr;
  G.e_SubOfMain = CK_NONE;
  G_vMember(eSubKind1, nullptr, __func__);
  G.p_Sub2OfMain = nullptr;
  G.e_Sub2OfMain = CK_NONE;
  G_vMember(eSubKind2, nullptr, __func__);
  return false;
}

/*----------------------------------------------------------------------------*/
/* step main register to next main which has sub and update M member          */
/*----------------------------------------------------------------------------*/
bool CardIter::G_bEach3SubOfMain(eCK eSubKind1, eCK eSubKind2, eCK eSubKind3)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(! Card_MAIN::bIsMain(eSubKind1));
  RASSERT(! Card_MAIN::bIsMain(eSubKind2));
  RASSERT(! Card_MAIN::bIsMain(eSubKind3));

  while (G_bEachMain_NONE()) {
    if (! (G.p_SubOfMain = G.p_MainCard->pFindSubCard(eSubKind1)))
      continue;

    if (! (G.p_Sub2OfMain = G.p_MainCard->pFindSubCard(eSubKind2)))
      continue;

    if (! (G.p_Sub3OfMain = G.p_MainCard->pFindSubCard(eSubKind3)))
      continue;

    vFocusOnMain();

    G_vMember(G.e_MainCard, G.p_MainCard, __func__);
    G.e_SubOfMain = G.p_SubOfMain->eCard();
    G_vMember(G.e_SubOfMain, G.p_SubOfMain, __func__);
    G.e_Sub2OfMain = G.p_Sub2OfMain->eCard();
    G_vMember(G.e_Sub2OfMain, G.p_Sub2OfMain, __func__);
    G.e_Sub3OfMain = G.p_Sub3OfMain->eCard();
    G_vMember(G.e_Sub3OfMain, G.p_Sub3OfMain, __func__);
    return true;
  }

  G.p_MainCard  = nullptr;
  G.e_MainCard  = CK_NONE;
  G.p_SubOfMain = nullptr;
  G.e_SubOfMain = CK_NONE;
  G_vMember(eSubKind1, nullptr, __func__);
  G.p_Sub2OfMain = nullptr;
  G.e_Sub2OfMain = CK_NONE;
  G_vMember(eSubKind2, nullptr, __func__);
  G.p_Sub3OfMain = nullptr;
  G.e_Sub3OfMain = CK_NONE;
  G_vMember(eSubKind3, nullptr, __func__);
  return false;
}

/*----------------------------------------------------------------------------*/
/* end of phase                                                               */
/*----------------------------------------------------------------------------*/
void CardIter::vEnd(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  G.xKid.vEnded();
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

    G.pDeck->vCheckCorrectness();

    //G.pBoard->pState()->vDumpSet();

    if (!  G.pBoard->pState()->bHasName(aXF[m_pBench->eXform()]))
      G.pBoard->pState()->vInsert(aXF[m_pBench->eXform()]);

    for (Kid xDeck(G.pDeck); xDeck.bEach(); /**/) {
      xDeck.pChild()->vEnsureFirst(CK_s_LEVEL);
      xDeck.pChild()->vEnsureLast(CK_s_CONSTRAINT);
      xDeck.pChild()->vEnsureLast(CK_s_NEEDED);
    }

    vPrintBoard(__func__,iLineNo);

//    if (AM::JOB::bHasTag(C_OPTIMISER_TRACE)) {
//      ShortForm *pShortForm = new ShortForm(this);
//      pShortForm->vDump(0);
//      delete pShortForm;
//    }
  }

  vValidateBench();

  G.xKid.vEnded();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Expr* CardIter::pExpr_For_col(Card_Term* p_term_arg, i8 i_Before)
{
  _precondition_( _MAGIC_, _FOOTER_);

  Symbol* pSym_term_arg = p_term_arg->pSymbol();

  if (! p_term_arg->pSymbol()->bIs_Col())
    return nullptr;

  Card_Expr* p_expr_col = nullptr;

  for (i8 i_Main = i_Before - 1; i_Main >= 0; i_Main--) {
    Card *pMain = this->G.pDeck->pGetChild(i_Main);
    Card_Emit* p_emit = pMain->pSub_emit();

    if (!p_emit)
      continue;

    i8 nChildren_emit = p_emit->nChildCards();
    for (i8 i_Child = 0; i_Child < nChildren_emit; i_Child++) {
      Card *pChild_prj = p_emit->pGetChild(i_Child);
      if (!pChild_prj->bIsCard(CK_EXPR))
        continue;
      Card_Expr* p_expr_prj = _pCard_Expr(pChild_prj);
      Card_RpnCalc* p_rpnCalc = p_expr_prj->pSub_rpnCalc();
      if (! p_rpnCalc)
        continue;
      if (1 != p_rpnCalc->nChildCards())
        continue;

      Symbol* pSym_rpnCalc_term0 = p_rpnCalc->pGetTerm(0)->pSymbol();
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
Card_Expr* CardIter::pExpr_For_colNm(Card_Term* p_term, i8 i_Before)
{
  _precondition_( _MAGIC_, _FOOTER_);

  if (C_UNKNOWN == i_Before)
    i_Before = this->i_Child();

  Card_Expr* p_expr_col = nullptr;

  for (i8 i_Main = i_Before - 1; i_Main >= 0; i_Main--) {
    Card *pMain = this->G.pDeck->pGetChild(i_Main);

    if (Card_Emit* p_emit = pMain->pSub_emit()) {
      i8 nChildren_prj = p_emit->nChildCards();
      for (i8 i_Child = 0; i_Child < nChildren_prj; i_Child++) {
        Card *pChild_prj = p_emit->pGetChild(i_Child);
        if (pChild_prj->bIsCard(CK_EXPR)) {
          Card_Expr* p_expr_prj = _pCard_Expr(pChild_prj);
          if (p_expr_prj->pSymbol()->bMatches(p_term->pSymbol())) {
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
/* focus on main                                                              */
/*----------------------------------------------------------------------------*/
void CardIter::vFocusOnMain(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

//  if (! G.p_MainCard->bIsMain())
//    return;
//
//  RASSERT(G.p_MainCard->bIsMain());

  MemLib::vZero(G.md_Coord, sizeof(G.md_Coord), CN_CARDITER);
  StrLib::vCopy(G.md_Coord, sizeof(G.md_Coord),G.p_MainCard->szCoord()->azChar());

  G.e_MainCard = G.p_MainCard->eCard();
  G.i_MainCard = this->i_Child();
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
Card* CardIter::pGetChild(i8 i_Child)
{
  _precondition_(_MAGIC_, _FOOTER_);

  Card* pChild = G.xKid.pParent()->pGetChild(i_Child);

  return pChild;
}

/*----------------------------------------------------------------------------*/
/* fetch M member by decoding given eMember                                   */
/*----------------------------------------------------------------------------*/
Card* CardIter::G_pGetMember(eCK eMember)
{
  _precondition_(_MAGIC_, _FOOTER_);

  switch (eMember) {

    case CK_c_ABSJOIN:          return G.p_absJoin;
    case CK_ADDRESS:          return G.p_address;
    case CK_o_AGGREG:           return G.p_aggreg;
    case CK_ANDCONJ:          return G.p_andConj;

    case CK_BOOLPRED:         return G.p_boolPred;
    case CK_BOUND:            return G.p_bound;
    case CK_BOUNDS:           return G.p_bounds;

    case CK_o_CALL:             return G.p_call;
    case CK_CALLER:           return G.p_caller;
    case CK_CALLOP:           return G.p_callOp;
    case CK_o_CAST:             return G.p_cast;
    case CK_CLAUSE:           return G.p_clause;
    case CK_c_COMBINED:         return G.p_combined;
    case CK_s_CONSTRAINT:       return G.p_constraint;
    case CK_CORRCOLS:         return G.p_corrCols;
    case CK_o_CORRELATION:      return G.p_correlation;
    case CK_COSTJOINING:      return G.p_costJoining;
    case CK_COSTNETWORK:      return G.p_costNetwork;
    case CK_COSTSTREAM:       return G.p_costQuery;
    case CK_COSTUNICAST:      return G.p_costUnicast;
    case CK_COUNT:            return G.p_count;
    case CK_o_CROSSJOIN:        return G.p_crossJoin;

    case CK_DECISION:         return G.p_decision;
    case CK_DIRECTION:        return G.p_direction;
    case CK_DISPOSITION:      return G.p_disposition;

    case CK_t_EMIT:          return G.p_emit;
    case CK_EXPR:             return G.p_expr;

    case CK_o_FILTER:           return G.p_filter;
    case CK_FILTPRED:         return G.p_filtPred;
    case CK_FILTPREDLIST:     return G.p_filtPredList;
    case CK_c_FLAT:             return G.p_flat;
    case CK_c_FOLD:             return G.p_fold;
    case CK_o_FROM:             return G.p_from;

    case CK_o_GBYAGG:           return G.p_gbyAgg;

    case CK_o_HASH:             return G.p_hash;
    case CK_s_HASHING:          return G.p_hashing;
    case CK_o_HAVING:           return G.p_having;
    case CK_t_HSHCOL:           return G.p_hshCol;
    case CK_s_HSHCOLLIST:       return G.p_hshColList;

    case CK_IDXCOL:           return G.p_idxCol;
    case CK_IDXCOLLIST:       return G.p_idxColList;
    case CK_INDEX:            return G.p_index;
    case CK_o_INDEXJOIN:        return G.p_indexJoin;
    case CK_t_INDEXLIST:        return G.p_indexList;
    case CK_INDEXNM:          return G.p_indexNm;
    case CK_o_INDEXSCAN:        return G.p_indexScan;
    case CK_o_INFLATE:          return G.p_inflate;
    case CK_s_INJECTABLE:         return G.p_injectable;

    case CK_JOINCAST:         return G.p_joinCast;
    case CK_JOINESTIMATE:     return G.p_joinEstimate;
    case CK_JOINORDER:        return G.p_joinOrder;
    case CK_JOINORDERING:     return G.p_joinOrdering;
    case CK_JOINPREDLIST:     return G.p_joinPredList;

    case CK_s_KEY:              return G.p_key;
    case CK_KEYCELLS:         return G.p_keyCells;
    case CK_o_KEYEXISTS:        return G.p_keyExists;
    case CK_o_KEYNOTEXISTS:     return G.p_keyNotExists;

    case CK_s_LEVEL:            return G.p_level;
    case CK_o_LIMIT:            return G.p_limit;

    case CK_MAP:              return G.p_map;
    case CK_s_MAPPING:          return G.p_mapping;

    case CK_s_NEEDED:           return G.p_needed;
    case CK_NOTE:             return G.p_note;
    case CK_NOTCONJ:          return G.p_notConj;

    case CK_ON:               return G.p_on;
    case CK_o_ORDAGG:           return G.p_ordAgg;
    case CK_ORCONJ:           return G.p_orConj;
    case CK_OUTERCOLNM:       return G.p_outerColNm;
    case CK_OUTERJOIN:        return G.p_outerJoin;

    case CK_PAYLOADCELLS:     return G.p_payloadCells;
    case CK_o_PROJECT:          return G.p_project;

    case CK_RELNOP:           return G.p_relnOp;
    case CK_REQUIRED:         return G.p_required;
    case CK_RLNCONJ:          return G.p_rlnConj;
    case CK_ROWCOUNT:         return G.p_rowCount;
    case CK_ROWSIZE:          return G.p_rowSize;
    case CK_RPNCALC:          return G.p_rpnCalc;

    case CK_o_SCAN:             return G.p_scan;
    case CK_SECTION:          return G.p_section;
    case CK_c_SEND:             return G.p_send;
    case CK_SPARE:            return G.p_spare;
    case CK_o_SORT:             return G.p_sort;
    case CK_SQYCALLS:         return G.p_sqyCalls;
    case CK_SQYINFO:          return G.p_sqyInfo;
    case CK_SQYNO:            return G.p_sqyNo;
    case CK_c_STAGE:            return G.p_stage;
    case CK_s_STREAM:           return G.p_stream;
    case CK_STREAMHEAD:       return G.p_streamHead;
    case CK_STREAMJOIN:       return G.p_streamJoin;
    case CK_STREAMKEY:        return G.p_streamKey;
    case CK_STREAMPER:        return G.p_streamPer;
    case CK_STREAMPTN:        return G.p_streamPtn;
    case CK_STREAMSET:        return G.p_streamSet;
    case CK_SUBQUERY:         return G.p_subQuery;
    case CK_SUBSET:           return G.p_subSet;
    case CK_SYNTHESIS:        return G.p_synthesis;

    case CK_s_TABLE:            return G.p_table;
    case CK_t_TABLEINFO:        return G.p_tableInfo;
    case CK_TABLELIST:        return G.p_tableList;
    case CK_t_TABLENM:          return G.p_tableNm;
    case CK_TABLEPTN:         return G.p_tablePtn;
    case CK_TABLESIZE:        return G.p_tableSize;
    case CK_TERM:             return G.p_term;
    case CK_o_TOP:              return G.p_top;
    case CK_TOPOLOGY:         return G.p_topology;

    case CK_UNIQUE:           return G.p_unique;

    case CK_o_WHERE:            return G.p_where;

    case CK_XCARD:            return G.p_xcard;

    default:
      NEVER_REACH
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::G_vInsertAfter(Card* pCard, Card* pAfter)
{
  _precondition_(_MAGIC_, _FOOTER_);

  G.pDeck->vInsertChildAfter(pCard,pAfter);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::G_vInsertBefore(Card* pCard, Card* pBefore)
{
  _precondition_(_MAGIC_, _FOOTER_);

  G.pDeck->vInsertChildBefore(pCard,pBefore);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool CardIter::G_bIsNextMain(eCK eMember)
{
  _precondition_(_MAGIC_, _FOOTER_);

  if (this->pNextChildIf(eMember))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::G_vMainResynch(void)
{
  _precondition_(_MAGIC_, _FOOTER_);

  G.xKid.vReposition(G.p_MainCard);
  RASSERT(G.p_MainCard == G.xKid.pChild());

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::G_vMainResynch(Card* pCard)
{
  _precondition_(_MAGIC_, _FOOTER_);

  G.p_MainCard = pCard;
  G_vMainResynch();
}

/*----------------------------------------------------------------------------*/
/* set member card                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::G_vMember(eCK eMember, Card* pCard, const char* asFuncNm)
{
  _precondition_(_MAGIC_, _FOOTER_);
  UNUSED(asFuncNm);

  RASSERT(!pCard || pCard->bIsCard(eMember));

  switch (eMember) {

    case CK_NONE: break;

    case CK_c_ABSJOIN: G.p_absJoin          = _pCard_AbsJoin(pCard); break;
    case CK_ADDRESS:          G.p_address          = _pCard_Address(pCard); break;
    case CK_o_AGGREG: G.p_aggreg           = _pCard_Aggreg(pCard);
                              G.p_OP               = (Card_OP*)pCard; break;
    case CK_ANDCONJ:          G.p_andConj          = _pCard_AndConj(pCard);
                              G.p_CONJ             = (Card_CONJ*)pCard; break;

    case CK_BOOLPRED: G.p_boolPred         = _pCard_BoolPred(pCard); break;
    case CK_BOUND:            G.p_bound            = _pCard_Bound(pCard); break;
    case CK_BOUNDS:           G.p_bounds           = _pCard_Bounds(pCard); break;

    case CK_o_CALL: G.p_call              = _pCard_Call(pCard);
                              G.p_OP                = (Card_OP*)pCard; break;
    case CK_CALLER:           G.p_caller            = _pCard_Caller(pCard); break;
    case CK_CALLOP:           G.p_callOp            = _pCard_CallOp(pCard); break;
    case CK_o_CAST: G.p_cast              = _pCard_Cast(pCard);
                              G.p_OP                = (Card_OP*)pCard; break;
    case CK_CLAUSE:           G.p_clause            = _pCard_Clause(pCard); break;

    case CK_c_COMBINED: G.p_combined          = _pCard_Combined(pCard);
                              G.p_OP                = (Card_OP*)pCard; break;
    case CK_s_CONSTRAINT: G.p_constraint        = _pCard_Constraint(pCard); break;
    case CK_CORRCOLS:         G.p_corrCols          = _pCard_CorrCols(pCard); break;
    case CK_o_CORRELATION: G.p_correlation       = _pCard_Correlation(pCard); ;
                              G.p_OP                = (Card_OP*)pCard; break;
    case CK_COSTJOINING:      G.p_costJoining       = _pCard_CostJoining(pCard); break;
    case CK_COSTNETWORK:      G.p_costNetwork       = _pCard_CostNetwork(pCard); break;
    case CK_COSTSTREAM:       G.p_costQuery         = _pCard_CostStream(pCard); break;
    case CK_COSTUNICAST:      G.p_costUnicast       = _pCard_CostUnicast(pCard); break;
    case CK_COUNT:            G.p_count             = _pCard_Count(pCard); break;
    case CK_o_CROSSJOIN: G.p_crossJoin         = _pCard_CrossJoin(pCard);
                              G.p_OP                = (Card_OP*)pCard; break;

    case CK_DECK:             G.pDeck               = _pCard_Deck(pCard); break;
    case CK_DECISION:         G.p_decision          = _pCard_Decision(pCard); break;
    case CK_DIRECTION:        G.p_direction         = _pCard_Direction(pCard); break;
    case CK_DISPOSITION:      G.p_disposition       = _pCard_Disposition(pCard); break;

    case CK_t_EMIT: G.p_emit              = _pCard_Emit(pCard); break;
    case CK_EXPR:             G.p_expr              = _pCard_Expr(pCard); break;

    case CK_o_FILTER: G.p_filter            = _pCard_Filter(pCard);
                              G.p_OP                = (Card_OP*)pCard; break;
    case CK_FILTPRED:         G.p_filtPred          = _pCard_FiltPred(pCard); break;
    case CK_FILTPREDLIST:     G.p_filtPredList      = _pCard_FiltPredList(pCard); break;
    case CK_c_FLAT: G.p_flat              = _pCard_Flat(pCard); break;
    case CK_c_FOLD: G.p_fold              = _pCard_Fold(pCard); break;
    case CK_o_FROM: G.p_from              = _pCard_From(pCard);
                              G.p_OP                = (Card_OP*)pCard; break;

    case CK_o_GBYAGG: G.p_gbyAgg            = _pCard_GbyAgg(pCard);
                              G.p_OP                = (Card_OP*)pCard; break;

    case CK_o_HASH: G.p_hash              = _pCard_Hash(pCard);
                              G.p_OP                = (Card_OP*)pCard; break;
    case CK_s_HASHING: G.p_hashing           = _pCard_Hashing(pCard); break;
    case CK_o_HAVING: G.p_having            = _pCard_Having(pCard);
                              G.p_OP                = (Card_OP*)pCard; break;
    case CK_t_HSHCOL: G.p_hshCol            = _pCard_HshCol(pCard); break;
    case CK_s_HSHCOLLIST: G.p_hshColList        = _pCard_HshColList(pCard); break;

    case CK_IDXCOL:           G.p_idxCol            = _pCard_IdxCol(pCard); break;
    case CK_IDXCOLLIST:       G.p_idxColList        = _pCard_IdxColList(pCard); break;
    case CK_INDEX:            G.p_index             = _pCard_Index(pCard); break;
    case CK_o_INDEXJOIN: G.p_indexJoin         = _pCard_IndexJoin(pCard);
                              G.p_OP                = (Card_OP*)pCard; break;
    case CK_t_INDEXLIST: G.p_indexList         = _pCard_IndexList(pCard); break;
    case CK_INDEXNM:          G.p_indexNm           = _pCard_IndexNm(pCard); break;
    case CK_o_INDEXSCAN: G.p_indexScan         = _pCard_IndexScan(pCard);
                              G.p_OP                = (Card_OP*)pCard; break;
    case CK_s_INJECTABLE: G.p_injectable          = _pCard_Injectable(pCard); break;
    case CK_o_INFLATE: G.p_inflate           = _pCard_Inflate(pCard);
                              G.p_OP                = (Card_OP*)pCard; break;

    case CK_JOINCAST:         G.p_joinCast          = _pCard_JoinCast(pCard); break;
    case CK_JOINESTIMATE:     G.p_joinEstimate      = _pCard_JoinEstimate(pCard); break;
    case CK_JOINORDER:        G.p_joinOrder         = _pCard_JoinOrder(pCard); break;
    case CK_JOINORDERING:     G.p_joinOrdering      = _pCard_JoinOrdering(pCard); break;
    case CK_JOINPREDLIST:     G.p_joinPredList      = _pCard_JoinPredList(pCard); break;

    case CK_s_KEY: G.p_key               = _pCard_Key(pCard); break;
    case CK_KEYCELLS:         G.p_keyCells          = _pCard_KeyCells(pCard); break;
    case CK_o_KEYEXISTS: G.p_keyExists         = _pCard_KeyExists(pCard);
                              G.p_OP                = (Card_OP*)pCard; break;
    case CK_o_KEYNOTEXISTS: G.p_keyNotExists      = _pCard_KeyNotExists(pCard);
                              G.p_OP                = (Card_OP*)pCard; break;

    case CK_s_LEVEL: G.p_level             = _pCard_Level(pCard); break;
    case CK_o_LIMIT: G.p_limit             = _pCard_Limit(pCard);
                              G.p_OP                = (Card_OP*)pCard; break;

    case CK_MAP:              G.p_map               = _pCard_Map(pCard); break;
    case CK_s_MAPPING: G.p_mapping           = _pCard_Mapping(pCard); break;

    case CK_s_NEEDED: G.p_needed            = _pCard_Needed(pCard); break;
    case CK_NOTE:             G.p_note              = _pCard_Note(pCard); break;
    case CK_NOTCONJ:          G.p_notConj           = _pCard_NotConj(pCard);
                              G.p_CONJ              = (Card_CONJ*)pCard; break;

    case CK_ON:               G.p_on                = _pCard_On(pCard); break;
    case CK_ORCONJ:           G.p_orConj            = _pCard_OrConj(pCard);
                              G.p_CONJ              = (Card_CONJ*)pCard; break;
    case CK_o_ORDAGG: G.p_ordAgg            = _pCard_OrdAgg(pCard);
                              G.p_OP                = (Card_OP*)pCard; break;
    case CK_OUTERCOLNM:       G.p_outerColNm        = _pCard_OuterColNm(pCard); break;
    case CK_OUTERJOIN:        G.p_outerJoin         = _pCard_OuterJoin(pCard); break;

    case CK_PAYLOADCELLS:     G.p_payloadCells      = _pCard_PayloadCells(pCard); break;
    case CK_o_PROJECT: G.p_project           = _pCard_Project(pCard);
                              G.p_OP                = (Card_OP*)pCard; break;

    case CK_RELNOP:           G.p_relnOp            = _pCard_RelnOp(pCard); break;
    case CK_REQUIRED:         G.p_required          = _pCard_Required(pCard); break;
    case CK_RLNCONJ:          G.p_rlnConj           = _pCard_RlnConj(pCard);
                              G.p_CONJ              = (Card_CONJ*)pCard; break;
    case CK_ROWCOUNT:         G.p_rowCount          = _pCard_RowCount(pCard); break;
    case CK_ROWSIZE:          G.p_rowSize           = _pCard_RowSize(pCard); break;
    case CK_RPNCALC:          G.p_rpnCalc           = _pCard_RpnCalc(pCard); break;

    case CK_o_SCAN: G.p_scan              = _pCard_Scan(pCard);
                              G.p_OP                = (Card_OP*)pCard; break;
    case CK_SECTION:          G.p_section           = _pCard_Section(pCard); break;
    case CK_c_SEND: G.p_send              = _pCard_Send(pCard);
                              G.p_OP                = (Card_OP*)pCard; break;
    case CK_o_SORT: G.p_sort              = _pCard_Sort(pCard);
                              G.p_OP                = (Card_OP*)pCard; break;
    case CK_SPARE:            G.p_spare             = _pCard_Spare(pCard);
                              G.p_OP                = (Card_OP*)pCard; break;
    case CK_SQYCALLS:         G.p_sqyCalls          = _pCard_SqyCalls(pCard); break;
    case CK_SQYINFO:          G.p_sqyInfo           = _pCard_SqyInfo(pCard); break;
    case CK_SQYNO:            G.p_sqyNo             = _pCard_SqyNo(pCard); break;
    case CK_c_STAGE: G.p_stage             = _pCard_Stage(pCard);
                              G.p_OP                = (Card_OP*)pCard; break;
    case CK_s_STREAM: G.p_stream            = _pCard_Stream(pCard); break;
    case CK_STREAMHEAD:       G.p_streamHead        = _pCard_StreamHead(pCard); break;
    case CK_STREAMJOIN:       G.p_streamJoin        = _pCard_StreamJoin(pCard); break;
    case CK_STREAMKEY:        G.p_streamKey         = _pCard_StreamKey(pCard); break;
    case CK_STREAMPER:        G.p_streamPer         = _pCard_StreamPer(pCard); break;
    case CK_STREAMPTN:        G.p_streamPtn         = _pCard_StreamPtn(pCard); break;
    case CK_STREAMSET:        G.p_streamSet         = _pCard_StreamSet(pCard); break;
    case CK_SUBQUERY:         G.p_subQuery          = _pCard_SubQuery(pCard); break;
    case CK_SUBSET:           G.p_subSet            = _pCard_SubSet(pCard); break;
    case CK_SYNTHESIS:        G.p_synthesis         = _pCard_Synthesis(pCard); break;

    case CK_s_TABLE: G.p_table             = _pCard_Table(pCard);
                              G.p_OP                = (Card_OP*)pCard; break;
    case CK_t_TABLEINFO: G.p_tableInfo         = _pCard_TableInfo(pCard); break;
    case CK_TABLELIST:        G.p_tableList         = _pCard_TableList(pCard); break;
    case CK_t_TABLENM: G.p_tableNm           = _pCard_TableNm(pCard); break;
    case CK_TABLEPTN:         G.p_tablePtn          = _pCard_TablePtn(pCard); break;
    case CK_TABLESIZE:        G.p_tableSize         = _pCard_TableSize(pCard); break;
    case CK_TERM:             G.p_term              = _pCard_Term(pCard); break;
    case CK_o_TOP: G.p_top               = _pCard_Top(pCard);
                              G.p_OP                = (Card_OP*)pCard; break;
    case CK_TOPOLOGY:         G.p_topology          = _pCard_Topology(pCard); break;

    case CK_UNIQUE:           G.p_unique            = _pCard_Unique(pCard); break;

    case CK_o_WHERE: G.p_where             = _pCard_Where(pCard);
                              G.p_OP                = (Card_OP*)pCard; break;

    case CK_XCARD:            G.p_xcard             = _pCard_XCard(pCard); break;

    default:
      NEVER_REACH
  }
}

/*----------------------------------------------------------------------------*/
/* set member card                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vMember(Card* pCard)
{
  _precondition_(_MAGIC_, _FOOTER_);

  G_vMember(pCard->eCard(),pCard,__func__);
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
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardIter::pNextChild(void)
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
Card* CardIter::pNextChildIf(eCK eCard)
{
  _precondition_(_MAGIC_, _FOOTER_);

  Card* pNextMain = this->pNextChild();
  if (! pNextMain || eCard != pNextMain->eCard())
    return nullptr;

  return pNextMain;
}

/*----------------------------------------------------------------------------*/
/* create a new card and optionally set member                                */
/*----------------------------------------------------------------------------*/
void CardIter::G_vNewMember(eCK eMember)
{
  _precondition_(_MAGIC_, _FOOTER_);

  G.p_NEW = new Card(eMember,C_UNKNOWN);

  G_vMember(eMember, G.p_NEW, __func__);
}

/*----------------------------------------------------------------------------*/
/* step to next main and set member                                           */
/*----------------------------------------------------------------------------*/
void CardIter::G_vNewMember(eCK eMember, Symbol xSymbol)
{
  _precondition_(_MAGIC_, _FOOTER_);

  G_vNewMember(eMember);

  G.p_NEW->pSymbol()->vApplyCastFacets(&xSymbol);
}

/*----------------------------------------------------------------------------*/
/* clear the M members                                                        */
/*----------------------------------------------------------------------------*/
void CardIter::vNullMembers(bool bNull_OP)
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

  G.p_emit          = nullptr;
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
  G.p_injectable         = nullptr;

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

  G.p_CARD             = nullptr;
  G.p_NEW              = nullptr;
  G.p_CONJ             = nullptr;

  if (bNull_OP)
    G.p_OP = nullptr;
}

/*----------------------------------------------------------------------------*/
/* past given phase?                                                          */
/*----------------------------------------------------------------------------*/
bool CardIter::bPastPhase(eOH ePhase)
{
  _precondition_( _MAGIC_, _FOOTER_);

  return m_pBench->bPastPhase(this,ePhase);
}

/*----------------------------------------------------------------------------*/
/* begin and record transformation information                                */
/*----------------------------------------------------------------------------*/
void CardIter::vPhase(eOH ePhase)
{
  _precondition_( _MAGIC_, _FOOTER_);

  m_pBench->vPhase(this,ePhase);
}

/*----------------------------------------------------------------------------*/
/* pop/restore M iterator from iter stack                                     */
/*----------------------------------------------------------------------------*/
void CardIter::G_vPopBoard(void)
{
  _precondition_(_MAGIC_, _FOOTER_);

  this->vRestore();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardIter::pPrevChild(void)
{
  _precondition_(_MAGIC_, _FOOTER_);

//  RASSERT(G.xKid.bWithinRange());
//
//  i8 i_Prev = G.xKid.i_Child() - 1;
//
//  if (i_Prev < 0)
//    return nullptr;
//
//  Card* pChild = G.xKid.pParent()->pGetChild(i_Prev);

  Card* pChild = G.xKid.pPrev();

  return pChild;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardIter::pPrevChildIf(eCK eCard)
{
  _precondition_(_MAGIC_, _FOOTER_);

 RASSERT(G.xKid.bWithinRange());

  Card* pPrevMain = this->pPrevChild();
  if (! pPrevMain || eCard != pPrevMain->eCard())
    return nullptr;

  return pPrevMain;
}

/*----------------------------------------------------------------------------*/
/* print the board                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vPrintBoard(const char* azFunc, i8 iLineNo, bool bForce)
{
  _precondition_(_MAGIC_, _FOOTER_);

  if (! AM::JOB::bHasTag(C_OPTIMISER_TRACE) && ! bForce)
    return;

  G.pBoard->vRenumberCards();

  G.pBoard->vPrintBoard(this,szXdesc());


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
    sWhere.vCatSprint("%s :: %s:%lld"_Stz, aXF[m_pBench->eXform()], azFunc, iLineNo);

  vTrace("%s\n"_Stz, sWhere.azChar());
}

/*----------------------------------------------------------------------------*/
/* capture prior projection                                                   */
/*----------------------------------------------------------------------------*/
void CardIter::vSnapFluxArriving(void)
{
  _precondition_(_MAGIC_, _FOOTER_);

  if (Card_Emit* p_emit = G.p_OP->pSub_emit()) {
    Flux.m_pArriving = p_emit;
    Dic.m_p_expr = p_emit;
  }
}

/*----------------------------------------------------------------------------*/
/* push current M iterator onto iter stack                                    */
/*----------------------------------------------------------------------------*/
void CardIter::G_vPushBoard(Board* pBoard)
{
  _precondition_(_MAGIC_, _FOOTER_);

  G_vPushIter();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  G_vSetBoard(pBoard);
}

/*----------------------------------------------------------------------------*/
/* find and push board onto iter stack                                        */
/*----------------------------------------------------------------------------*/
bool CardIter::G_bPushBoard(Symbol* pSymbol)
{
  _precondition_(_MAGIC_, _FOOTER_);

  Board*  pBoard = m_pBench->pFindBoard(pSymbol);
  if (! pBoard)
    return false;

  G_vPushBoard(pBoard);

  return true;
}

/*----------------------------------------------------------------------------*/
/* find and push board onto iter stack                                        */
/*----------------------------------------------------------------------------*/
bool CardIter::G_bPushBoard(Stz* szSqyNo)
{
  _precondition_(_MAGIC_, _FOOTER_);

  Board*  pBoard = m_pBench->pFindBoard_By_sqyNo(szSqyNo);
  if (! pBoard)
     return false;

  G_vPushBoard(pBoard);

  return true;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::G_vPushNewChild(Card* pParent, eCK eChild)
{
  _precondition_(_MAGIC_, _FOOTER_);

  Card* pChild = new Card(eChild,C_UNKNOWN);

  pParent->vPushChild(pChild);
  G_vMember(eChild, pChild, __func__);
}

/*----------------------------------------------------------------------------*/
/* push current M iterator onto iter stack                                    */
/*----------------------------------------------------------------------------*/
void CardIter::G_vPushIter(void)
{
  _precondition_(_MAGIC_, _FOOTER_);

//  Xsentinals* pTransformIterSave = new Xsentinals;
//
//  pTransformIterSave->vCopyFrom(this->aXsentinels());
//
//  m_pTransformIterList->vPush(pTransformIterSave);

  this->vSave();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vRecurseBegin(Card* pCard)
{
  _precondition_(_MAGIC_, _FOOTER_);

  vSave();

  m_i_RecursionDepth++;
  G.m_i_RecursionDepth = m_i_RecursionDepth;

  vChangeCard(pCard);
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

  if (G.p_MainCard && (! G.m_bDontDescend) ) {
    if (G.p_MainCard->nChildCards()) {

      vPushRecurseIter();
      vChangeCard(G.p_MainCard);
      G_bEachMain_NONE();

      return G.p_MainCard;
    }
  }

  G.m_bDontDescend = false;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // visit next child

  if (G_bEachMain_NONE())
    return G.p_MainCard;

  while (bPopRecurseIter()) {
    if (G_bEachMain_NONE())
      return G.p_MainCard;
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

  vRestore();

  m_i_RecursionDepth--;

  RASSERT(G.m_i_RecursionDepth == m_i_RecursionDepth);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vRemoveChild(Card* pChild)
{
  _precondition_(_MAGIC_, _FOOTER_);

  RASSERT(G.xKid.bWithinRange());

  if (pChild == G.xKid.pChild())
    vRemoveCurr(G.xKid.eChild());
  else
    G.xKid.pParent()->vRemoveChild(pChild);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vRemoveDeleteChild(Card*& rpChild)
{
  _precondition_(_MAGIC_, _FOOTER_);

  RASSERT(G.xKid.bWithinRange());

  vRemoveChild(rpChild);

  delete rpChild;

  rpChild = nullptr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vRemoveCurr(eCK eCrossCheck)
{
  _precondition_(_MAGIC_, _FOOTER_);

  RASSERT(G.xKid.bWithinRange());

  RASSERT(eCrossCheck == G.xKid.eChild());

  Card* pChild = G.xKid.pChild();

  G.xKid.pParent()->vRemoveChild(pChild);

  G.xKid.vBackstep();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vRemoveDeleteCurr(eCK eCrossCheck)
{
  _precondition_(_MAGIC_, _FOOTER_);

  RASSERT(G.xKid.bWithinRange());

  RASSERT(eCrossCheck == G.xKid.eChild());

  Card* pChild = G.xKid.pChild();

  G.xKid.pParent()->vRemoveChild(pChild);

  delete pChild;

  G.xKid.vBackstep();
}

/*----------------------------------------------------------------------------*/
/* remove main card from deck and delete it; clear M member to null           */
/*----------------------------------------------------------------------------*/
void CardIter::G_vRemoveDeleteMain(eCK eCrossCheck)
{
  _precondition_(_MAGIC_, _FOOTER_);

  this->vRemoveDeleteCurr(eCrossCheck);

  G.p_MainCard = nullptr;
  G.e_MainCard = CK_NONE;

  G_vMember(eCrossCheck, nullptr, __func__);
}

/*----------------------------------------------------------------------------*/
/* remove main card from deck (do not clear M members or registers)           */
/*----------------------------------------------------------------------------*/
void CardIter::G_vRemoveMain(eCK eCrossCheck)
{
  _precondition_( _MAGIC_, _FOOTER_);

  this->vRemoveCurr(eCrossCheck);
}

/*----------------------------------------------------------------------------*/
/* resolve prior expr of given term (find right-to-left)                      */
/*----------------------------------------------------------------------------*/
Card_Expr* CardIter::pResolvePrior_expr(Card_Term* p_term)
{
  _precondition_( _MAGIC_, _FOOTER_);

  Card_Emit* p_arriving = pResolvePrior_projection();
  if (! p_arriving)
    return nullptr;

  //RASSERT(p_arriving == this->Flux.m_pArriving);

  Card_Expr* p_expr_resolved = p_arriving->pResolve_expr(p_term);
  return p_expr_resolved;
}

/*----------------------------------------------------------------------------*/
/* resolve prior expr of given term (find right-to-left)                      */
/*----------------------------------------------------------------------------*/
Card_Expr* CardIter::pResolvePrior_expr_prior(Card_Term* p_term)
{
  _precondition_( _MAGIC_, _FOOTER_);

  if (! this->Flux.m_pArriving)
    return nullptr;

  Card_Expr* p_expr_resolved = this->Flux.m_pArriving->pResolve_expr(p_term);
  return p_expr_resolved;
}

/*----------------------------------------------------------------------------*/
/* resolve prior projection (find right-to-left)                              */
/*----------------------------------------------------------------------------*/
Card_Emit* CardIter::pResolvePrior_projection(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  Card_Emit* p_arriving = Card_Emit::pResolvePriorProjection(this);

  return p_arriving;
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
void CardIter::vBeginEachReverse(void)
{
  _precondition_(_MAGIC_, _FOOTER_);

  G.xKid.vBegin_reverse();
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardIter::pRewindChild(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

//  if (G.xKid.m_i_Child == 0) {
//    G.xKid.m_i_Child= C_UNKNOWN;
//    return nullptr;
//  }
//
//  G.xKid.m_i_Child--;

  G.xKid.vBackstep();

  return G.xKid.pChild();
}

/*----------------------------------------------------------------------------*/
/* step main register next main and update M member                           */
/*----------------------------------------------------------------------------*/
bool CardIter::G_bRewindMain(void)
{
  _precondition_(_MAGIC_, _FOOTER_);

  if ((G.p_MainCard = this->pRewindChild())) {
    vFocusOnMain();
    G_vMember(G.e_MainCard, G.p_MainCard, __func__);
    return true;
  }

  G.e_MainCard = CK_NONE;
  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vRestore(void)
{
  CardIterMembers* pRestore = pPop();

  G.vCopyFrom(pRestore);

  delete pRestore;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vSave(void)
{
  CardIterMembers* pSave = new CardIterMembers;

  pSave->vCopyFrom(&G);

  vPush(pSave);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vSet(Card* pChild)
{
  _precondition_(_MAGIC_, _FOOTER_);

  RASSERT(G.xKid.bWithinRange());

  G.xKid.vReposition(G.xKid.pParent()->iFindChild(pChild));
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vSet(CardIter* pCardIter_src)
{
  _precondition_(_MAGIC_,_FOOTER_);

  G.xKid.vParent(pCardIter_src->G.xKid.pParent());
  G.xKid.vReposition(pCardIter_src->G.xKid.i_Child());
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vSet(i8 i_Child)
{
  _precondition_(_MAGIC_, _FOOTER_);

  RASSERT(G.xKid.bWithinRange());

  G.xKid.vReposition(i_Child);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::G_vSetBoard(Board* pBoard)
{
  _precondition_(_MAGIC_, _FOOTER_);

  G.pBoard = pBoard;
  G.pDeck  = G.pBoard->pDeck();
  //Sqy.m_pSubQuery = G.pBoard->pSubQuery();

  this->vChangeCard(G.pBoard->pDeck());
}

/*----------------------------------------------------------------------------*/
/* begin and record transformation information                                */
/*----------------------------------------------------------------------------*/
void CardIter::vTransform(const char* azFunc, eXF eXform, Stz* szXdesc)
{
  _precondition_( _MAGIC_, _FOOTER_);

  m_pBench->vTransform(azFunc, eXform, szXdesc);

  //this->vBeginEach();
}

/*----------------------------------------------------------------------------*/
/* set up which board to use                                                  */
/*----------------------------------------------------------------------------*/
void CardIter::vUsingBoard(i8 i_Board)
{
  _precondition_( _MAGIC_, _FOOTER_);

  Board* pBoard = m_pBench->pGetBoard(i_Board);

  vUsingBoard(pBoard);
}

/*----------------------------------------------------------------------------*/
/* set up which board to use                                                  */
/*----------------------------------------------------------------------------*/
void CardIter::vUsingBoard(Board* pBoard)
{
  _precondition_( _MAGIC_, _FOOTER_);

  G.pBoard = pBoard;
  G.pDeck  = G.pBoard->pDeck();

  this->vChangeCard(G.pBoard->pDeck());
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

    vMember(pChild);

    switch (eChild) {

      case CK_c_ABSJOIN:      G.p_absJoin->vValidate(this, iDepth, pCard);  break;
      case CK_ADDRESS:      G.p_address->vValidate(this, iDepth, pCard);  break;
      case CK_o_AGGREG:       G.p_aggreg->vValidate(this, iDepth, pCard);  break;
      case CK_ANDCONJ:      G.p_andConj->vValidate(this, iDepth, pCard);  break;

      case CK_BOOLPRED:     G.p_boolPred->vValidate(this, iDepth, pCard);  break;
      case CK_BOUND:        G.p_bound->vValidate(this, iDepth, pCard);  break;
      case CK_BOUNDS:       G.p_bounds->vValidate(this, iDepth, pCard);  break;

      case CK_o_CALL:         G.p_call->vValidate(this, iDepth, pCard);  break;
      case CK_CALLER:       G.p_caller->vValidate(this, iDepth, pCard);  break;
      case CK_CALLOP:       G.p_callOp->vValidate(this, iDepth, pCard);  break;
      case CK_o_CAST:         G.p_cast->vValidate(this, iDepth, pCard);  break;
      case CK_CLAUSE:       G.p_clause->vValidate(this, iDepth, pCard);  break;
      case CK_c_COMBINED:     G.p_combined->vValidate(this, iDepth, pCard);  break;
      case CK_s_CONSTRAINT:   G.p_constraint->vValidate(this, iDepth, pCard);  break;
      case CK_CORRCOLS:     G.p_corrCols->vValidate(this, iDepth, pCard);  break;
      case CK_o_CORRELATION:  G.p_correlation->vValidate(this, iDepth, pCard);  break;
      case CK_COSTJOINING:  G.p_costJoining->vValidate(this, iDepth, pCard);  break;
      case CK_COSTNETWORK:  G.p_costNetwork->vValidate(this, iDepth, pCard);  break;
      case CK_COSTSTREAM:   G.p_costQuery->vValidate(this, iDepth, pCard);  break;
      case CK_COSTUNICAST:  G.p_costUnicast->vValidate(this, iDepth, pCard);  break;
      case CK_COUNT:        G.p_count->vValidate(this, iDepth, pCard);  break;
      case CK_o_CROSSJOIN:    G.p_crossJoin->vValidate(this, iDepth, pCard);  break;

      case CK_DECISION:     G.p_decision->vValidate(this, iDepth, pCard);  break;
      case CK_DIRECTION:    G.p_direction->vValidate(this, iDepth, pCard);  break;
      case CK_DISPOSITION:  G.p_disposition->vValidate(this, iDepth, pCard);  break;

      case CK_t_EMIT:      G.p_emit->vValidate(this, iDepth, pCard);  break;
      case CK_EXPR:         G.p_expr->vValidate(this, iDepth, pCard);  break;

      case CK_o_FILTER:       G.p_filter->vValidate(this, iDepth, pCard);  break;
      case CK_FILTPRED:     G.p_filtPred->vValidate(this, iDepth, pCard);  break;
      case CK_FILTPREDLIST: G.p_filtPredList->vValidate(this, iDepth, pCard);  break;
      case CK_c_FLAT:         G.p_flat->vValidate(this, iDepth, pCard);  break;
      case CK_c_FOLD:         G.p_fold->vValidate(this, iDepth, pCard);  break;
      case CK_o_FROM:         G.p_from->vValidate(this, iDepth, pCard);  break;

      case CK_o_GBYAGG:       G.p_gbyAgg->vValidate(this, iDepth, pCard);  break;

      case CK_o_HASH:         G.p_hash->vValidate(this, iDepth, pCard);  break;
      case CK_s_HASHING:      G.p_hashing->vValidate(this, iDepth, pCard);  break;
      case CK_o_HAVING:       G.p_having->vValidate(this, iDepth, pCard);  break;
      case CK_t_HSHCOL:       G.p_hshCol->vValidate(this, iDepth, pCard);  break;
      case CK_s_HSHCOLLIST:   G.p_hshColList->vValidate(this, iDepth, pCard);  break;

      case CK_IDXCOL:       G.p_idxCol->vValidate(this, iDepth, pCard);  break;
      case CK_IDXCOLLIST:   G.p_idxColList->vValidate(this, iDepth, pCard);  break;
      case CK_INDEX:        G.p_index->vValidate(this, iDepth, pCard);  break;
      case CK_o_INDEXJOIN:    G.p_indexJoin->vValidate(this, iDepth, pCard);  break;
      case CK_t_INDEXLIST:    G.p_indexList->vValidate(this, iDepth, pCard);  break;
      case CK_INDEXNM:      G.p_indexNm->vValidate(this, iDepth, pCard);  break;
      case CK_o_INDEXSCAN:    G.p_indexScan->vValidate(this, iDepth, pCard);  break;
      case CK_o_INFLATE:      G.p_inflate->vValidate(this, iDepth, pCard);  break;
      case CK_s_INJECTABLE:     G.p_injectable->vValidate(this, iDepth, pCard);  break;

      case CK_JOINCAST:     G.p_joinCast->vValidate(this, iDepth, pCard);  break;
      case CK_JOINESTIMATE: G.p_joinEstimate->vValidate(this, iDepth, pCard);  break;
      case CK_JOINORDER:    G.p_joinOrder->vValidate(this, iDepth, pCard);  break;
      case CK_JOINORDERING: G.p_joinOrdering->vValidate(this, iDepth, pCard);  break;
      case CK_JOINPREDLIST: G.p_joinPredList->vValidate(this, iDepth, pCard);  break;

      case CK_s_KEY:          G.p_key->vValidate(this, iDepth, pCard);  break;
      case CK_KEYCELLS:     G.p_keyCells->vValidate(this, iDepth, pCard);  break;
      case CK_o_KEYEXISTS:    G.p_keyExists->vValidate(this, iDepth, pCard);  break;
      case CK_o_KEYNOTEXISTS: G.p_keyNotExists->vValidate(this, iDepth, pCard);  break;

      case CK_s_LEVEL:        G.p_level->vValidate(this, iDepth, pCard);  break;
      case CK_o_LIMIT:        G.p_limit->vValidate(this, iDepth, pCard);  break;

      case CK_MAP:          G.p_map->vValidate(this, iDepth, pCard);  break;
      case CK_s_MAPPING:      G.p_mapping->vValidate(this, iDepth, pCard);  break;

      case CK_s_NEEDED:       G.p_needed->vValidate(this, iDepth, pCard);  break;
      case CK_NOTE:         G.p_note->vValidate(this, iDepth, pCard);  break;
      case CK_NOTCONJ:      G.p_notConj->vValidate(this, iDepth, pCard);  break;

      case CK_ON:           G.p_on->vValidate(this, iDepth, pCard);  break;
      case CK_o_ORDAGG:       G.p_ordAgg->vValidate(this, iDepth, pCard);  break;
      case CK_ORCONJ:       G.p_orConj->vValidate(this, iDepth, pCard);  break;
      case CK_OUTERCOLNM:   G.p_outerColNm->vValidate(this, iDepth, pCard);  break;
      case CK_OUTERJOIN:    G.p_outerJoin->vValidate(this, iDepth, pCard);  break;

      case CK_PAYLOADCELLS: G.p_payloadCells->vValidate(this, iDepth, pCard);  break;
      case CK_o_PROJECT:      G.p_project->vValidate(this, iDepth, pCard);  break;

      case CK_RELNOP:       G.p_relnOp->vValidate(this, iDepth, pCard);  break;
      case CK_REQUIRED:     G.p_required->vValidate(this, iDepth, pCard);  break;
      case CK_RLNCONJ:      G.p_rlnConj->vValidate(this, iDepth, pCard);  break;
      case CK_ROWCOUNT:     G.p_rowCount->vValidate(this, iDepth, pCard);  break;
      case CK_ROWSIZE:      G.p_rowSize->vValidate(this, iDepth, pCard);  break;
      case CK_RPNCALC:      G.p_rpnCalc->vValidate(this, iDepth, pCard);  break;

      case CK_o_SCAN:       G.p_scan->vValidate(this, iDepth, pCard);  break;
      case CK_SECTION:    G.p_section->vValidate(this, iDepth, pCard);  break;
      case CK_c_SEND:       G.p_send->vValidate(this, iDepth, pCard);  break;
      case CK_SPARE:      G.p_spare->vValidate(this, iDepth, pCard);  break;
      case CK_o_SORT:       G.p_sort->vValidate(this, iDepth, pCard);  break;
      case CK_SQYCALLS:   G.p_sqyCalls->vValidate(this, iDepth, pCard);  break;
      case CK_SQYINFO:    G.p_sqyInfo->vValidate(this, iDepth, pCard);  break;
      case CK_SQYNO:      G.p_sqyNo->vValidate(this, iDepth, pCard);  break;
      case CK_c_STAGE:      G.p_stage->vValidate(this, iDepth, pCard);  break;
      case CK_s_STREAM:     G.p_stream->vValidate(this, iDepth, pCard);  break;
      case CK_STREAMHEAD: G.p_streamHead->vValidate(this, iDepth, pCard);  break;
      case CK_STREAMJOIN: G.p_streamJoin->vValidate(this, iDepth, pCard);  break;
      case CK_STREAMKEY:  G.p_streamKey->vValidate(this, iDepth, pCard);  break;
      case CK_STREAMPER:  G.p_streamPer->vValidate(this, iDepth, pCard);  break;
      case CK_STREAMPTN:  G.p_streamPtn->vValidate(this, iDepth, pCard);  break;
      case CK_STREAMSET:  G.p_streamSet->vValidate(this, iDepth, pCard);  break;
      case CK_SUBQUERY:   G.p_subQuery->vValidate(this, iDepth, pCard);  break;
      case CK_SUBSET:     G.p_subSet->vValidate(this, iDepth, pCard);  break;
      case CK_SYNTHESIS:  G.p_synthesis->vValidate(this, iDepth, pCard);  break;

      case CK_s_TABLE:     G.p_table->vValidate(this, iDepth, pCard);  break;
      case CK_t_TABLEINFO: G.p_tableInfo->vValidate(this, iDepth, pCard);  break;
      case CK_TABLELIST: G.p_tableList->vValidate(this, iDepth, pCard);  break;
      case CK_t_TABLENM:   G.p_tableNm->vValidate(this, iDepth, pCard);  break;
      case CK_TABLEPTN:  G.p_tablePtn->vValidate(this, iDepth, pCard);  break;
      case CK_TABLESIZE: G.p_tableSize->vValidate(this, iDepth, pCard);  break;
      case CK_TERM:      G.p_term->vValidate(this, iDepth, pCard);  break;
      case CK_o_TOP:       G.p_top->vValidate(this, iDepth, pCard);  break;
      case CK_TOPOLOGY:  G.p_topology->vValidate(this, iDepth, pCard);  break;

      case CK_UNIQUE:    G.p_unique->vValidate(this, iDepth, pCard);  break;

      case CK_o_WHERE:     G.p_where->vValidate(this, iDepth, pCard);  break;

      case CK_XCARD:     G.p_xcard->vValidate(this, iDepth, pCard);  break;

      default:
        NEVER_REACH
    }

  }

  vAncestorPop();
  i8 nAncestors_end = nAncestors();
  RASSERT(nAncestors_end == nAncestors_start);

}

/*----------------------------------------------------------------------------*/
/* get transform number                                                       */
/*----------------------------------------------------------------------------*/
i8 CardIter::iXform(void)
{
  _precondition_(_MAGIC_,_FOOTER_);

  return m_pBench->iXform();
}

/*----------------------------------------------------------------------------*/
/* get description of transformation                                          */
/*----------------------------------------------------------------------------*/
Stz* CardIter::szXdesc(void)
{
  _precondition_(_MAGIC_,_FOOTER_);

  return m_pBench->szXdesc();
}

/*----------------------------------------------------------------------------*/
/* get transformation function                                                */
/*----------------------------------------------------------------------------*/
Stz* CardIter::szXfunc(void)
{
  _precondition_(_MAGIC_,_FOOTER_);

  return m_pBench->szXfunc();
}

/*----------------------------------------------------------------------------*/
/* get transformation phase                                                   */
/*----------------------------------------------------------------------------*/
Stz* CardIter::szXhand(void)
{
  _precondition_(_MAGIC_,_FOOTER_);

  return m_pBench->szXhand();
}

/*----------------------------------------------------------------------------*/
/*-- Protected ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool CardIter::bPopRecurseIter(void)
{
  _precondition_(_MAGIC_,_FOOTER_);

  CardIterMembers* pPeg = pPeek(0);
  if (! pPeg || pPeg->m_i_RecursionDepth != m_i_RecursionDepth)
    return false;

  vRestore();

  return true;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vPushRecurseIter(void)
{
  _precondition_(_MAGIC_,_FOOTER_);

  RASSERT(G.m_i_RecursionDepth);

  vSave();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/