/*============================================================================*/
/* CardIter.cpp --                                                            */
/*============================================================================*/

#include "CardIter.hpp"
#include "Cue.hpp"
#include "OptimiserFun.hpp"
#include "OptimiserCatalog.hpp"

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
CardIter::CardIter(Card*  pCard)
: Object(CN_CARDITER)
{
  _precondition_(_MAGIC_,_FOOTER_);

  vInit();

  G.xKid.vParent(pCard);

  G.xKid.m_bActive = true;

  _checkInitialised_(CN_CARDITER);

  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
CardIter::CardIter(CardIter* pCardIter_src)
    : Object(CN_CARDITER)
{
  _precondition_(_MAGIC_,_FOOTER_);

  vInit();

  m_pBench = pCardIter_src->m_pBench;
  vSet(pCardIter_src);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
CardIter::CardIter(Bench* pBench)
    : Object(CN_CARDITER)
{
  _precondition_(_MAGIC_,_FOOTER_);

  vInit();

  m_pBench   = pBench;
  m_pCardCtl = m_pBench->pCardCtl();

  _checkInitialised_(CN_CARDITER);

  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
CardIter::~CardIter(void) noexcept(false)
{
  _precondition_(_MAGIC_,_FOOTER_);

}

/*----------------------------------------------------------------------------*/
/* initialiser                                                                */
/*----------------------------------------------------------------------------*/
void CardIter::vInit(void)
{
  _precondition_(_MAGIC_,_FOOTER_);

  m_pBench   = nullptr;
  m_pCardCtl = nullptr;

  G.xKid.vParent(nullptr);

  G.m_bDontDescend = false;

  G.xKid.m_bActive = false;


  Sqy.m_pCard_sqCopy   = nullptr;

  Sqy.m_pExpr_fold = nullptr;
  Sqy.m_pExpr_corr = nullptr;

  Sqy.m_pCorrelation = nullptr;
  Sqy.m_i_SubQuery = C_UNKNOWN;
  Sqy.m_pSubQuery = nullptr;

  // the M iterator - - - - - - - - - - - - - - - - - - - - - -

  G.pBoard    = nullptr;
  G.pDeck     = nullptr;
  Sqy.m_pSubQuery = nullptr;
  Sqy.m_pTerm_sqCall       = nullptr;


  vNullMembers();
  G_vClearRegisters();
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::G_vAppendChild(eCK eParent, eCK eChild)
{
  _precondition_( _MAGIC_, _FOOTER_);

  Card* pParent = G_pGetMember(eParent);
  Card* pChild  = G_pGetMember(eChild);

  pParent->vAppend(pChild);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::G_vAppendChild(eCK eParent, Card* pChild)
{
  _precondition_(_MAGIC_,_FOOTER_);

  Card* pParent = G_pGetMember(eParent);

  pParent->vAppend(pChild);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::G_vAppendNewChild(Card* pParent, eCK eChild)
{
  _precondition_(_MAGIC_, _FOOTER_);

  Card* pChild = new Card(eChild);

  pParent->vAppend(pChild);
  G_vMember(eChild, pChild, __func__);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::G_vAppendNewChild(Card* pParent, eCK eChild, Symbol* pSymbol)
{
  _precondition_(_MAGIC_,_FOOTER_);

  Card* pChild = new Card(eChild);

  pChild->pSymbol()->vSymPpy(pSymbol);

  pParent->vAppend(pChild);
  G_vMember(eChild, pChild, __func__);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::G_vAppendNewChild(Card* pParent, eCK eChild, Symbol xSymbol)
{
  _precondition_(_MAGIC_,_FOOTER_);

  Card* pChild = new Card(eChild);

  pChild->pSymbol()->vSym(&xSymbol);

  pParent->vAppend(pChild);
  G_vMember(eChild, pChild, __func__);
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

  vBeginEach();
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

  G.p_MainCard         = nullptr;
  G.e_MainCard         = CK_NONE;
  G.i_MainCard         = C_UNKNOWN;
  G.p_SubOfMain        = nullptr;
  G.e_SubOfMain        = CK_NONE;
  G.p_Sub2OfMain       = nullptr;
  G.e_Sub2OfMain       = CK_NONE;
  G.p_Sub3OfMain       = nullptr;
  G.e_Sub3OfMain       = CK_NONE;


  Sqy.m_pTerm_corrCol      = nullptr;

  Prior.m_pProjection = nullptr;

  Early.m_pOP_root   = nullptr;
  Early.m_pOP_leaf   = nullptr;
  Early.m_pNeeded_later   = nullptr;

  StreamOrder.m_pProjection_running = nullptr;
  StreamOrder.m_pKey_running        = nullptr;


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
eOA CardIter::eCueUser(Stz* szPrompt)
{
  _precondition_(_MAGIC_, _FOOTER_);

  TokenVec* pTknVec = nullptr;

  if (m_pCardCtl->bCueDisabled())
    return OA_CONTINUE_TRANSFORMING;

  szPrompt->vConcat(" >> "_Stz);

  m_pCardCtl->vCued(true);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  L_Input:

  s4K sInput;
  Cue::vGetInput(szPrompt,sInput);

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

  RASSERT(this->bJustRestarted());

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
void CardIter::vDontDescend(void)
{
  _precondition_(_MAGIC_, _FOOTER_);

  G.m_bDontDescend = true;
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
bool CardIter::G_bEachMain(eCK eOfMainKind)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(G.xKid.m_bActive);

  G.xKid.vBeginForward();

  for (;;) {

    if (! G.xKid.bInRange()) {
      G.p_MainCard = nullptr;
      G.xKid.m_bActive = false;
      G_vMember(eOfMainKind, nullptr, __func__);
      return false;
    }

    G.p_MainCard = G.xKid.pChild();

    if (CK_NONE == eOfMainKind || eOfMainKind == G.xKid.eChild()) {
      vFocusOnMain();
      G_vMember(G.p_MainCard->eCard(), G.p_MainCard, __func__);
      return true;
    }

    G.xKid.vForward();
  }

  NEVER_REACH
}

/*----------------------------------------------------------------------------*/
/* conditionally step main register to next main and update M member          */
/*----------------------------------------------------------------------------*/
bool CardIter::G_bEachMain_foldBreak(eCK eOfMainKind)
{
  _precondition_( _MAGIC_, _FOOTER_);

  G.xKid.vBeginForward();

  for (;;) {

    if (! G.xKid.bInRange()) {
      return false;
    }

    G.p_MainCard = G.xKid.pChild();

    vFocusOnMain();
    G_vMember(G.e_MainCard, G.p_MainCard, __func__);

    if (G.xKid.pChild()->bIsCard(CK_FOLD)) {
      //G.xKid.m_i_Child= C_UNKNOWN;
      return false;
    }

    if (CK_NONE == eOfMainKind) {
      return true;
    }

    eCK eCardMain = G.xKid.eChild();
    if (eCardMain == eOfMainKind) {
      return true;
    }

    G.xKid.vForward();
  }

  NEVER_REACH
}

/*----------------------------------------------------------------------------*/
/* conditionally step main register to next main and update M member          */
/*----------------------------------------------------------------------------*/
bool CardIter::G_bEachMain_reverse(eCK eOfMainKind)
{
  _precondition_( _MAGIC_, _FOOTER_);

  G.xKid.vBeginReverse();

  for (;;) {

    if (! G.xKid.bInRange()) {
      return false;
    }

    G.p_MainCard = G.xKid.pChild();

    vFocusOnMain();
    G_vMember(G.e_MainCard, G.p_MainCard, __func__);

    if (CK_NONE == eOfMainKind) {
      return true;
    }

    eCK eCardMain = G.xKid.eChild();
    if (eCardMain == eOfMainKind) {
      return G.xKid.pChild();
    }

    G.xKid.vReverse();
  }

  NEVER_REACH
}

/*----------------------------------------------------------------------------*/
/* conditionally step main register to next main and update M member          */
/*----------------------------------------------------------------------------*/
bool CardIter::G_bEachMain_reverse_foldBreak(eCK eOfMainKind)
{
  _precondition_( _MAGIC_, _FOOTER_);

  G.xKid.vBeginReverse();

  for (;;) {

    if (! G.xKid.bInRange()) {
      return false;
    }

    G.p_MainCard = G.xKid.pChild();

    if (G.xKid.pChild()->bIsCard(CK_FOLD)) {
      //G.xKid.m_i_Child = C_UNKNOWN;
      return false;
    }

    if (CK_NONE == eOfMainKind) {
      return true;
    }

    vFocusOnMain();
    G_vMember(G.e_MainCard, G.p_MainCard, __func__);

    if (G.e_MainCard == eOfMainKind) {
      return true;
    }

    G.xKid.vReverse();
 }

  NEVER_REACH
}

/*----------------------------------------------------------------------------*/
/* conditionally step main register to next main which has sub and update M   */
/*----------------------------------------------------------------------------*/
bool CardIter::G_bEachMainWithSub(eCK eOfMainKind, eCK eAndSubKind)
{
  _precondition_( _MAGIC_, _FOOTER_);

  while (G_bEachMain(eOfMainKind)) {
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
bool CardIter::G_bEachMainWithSub_reverse(eCK eOfMainKind, eCK eAndSubKind)
{
  _precondition_( _MAGIC_, _FOOTER_);

  while (G_bEachMain_reverse(eOfMainKind)) {
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
bool CardIter::G_bEachMainWithSub_reverse_foldBreak(eCK eOfMainKind, eCK eAndSubKind)
{
  _precondition_( _MAGIC_, _FOOTER_);

  while (G_bEachMain_reverse_foldBreak(eOfMainKind)) {
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
/* conditionally step main register to next main which has sub and update M   */
/*----------------------------------------------------------------------------*/
bool CardIter::G_bEachMainWithSub(eCK eOfMainKind, eCK eAndSubKind, Stz* szSubValue)
{
  _precondition_( _MAGIC_, _FOOTER_);

  while (G_bEachMainWithSub(eOfMainKind, eAndSubKind)) {
    if (G.p_SubOfMain->szSymbol()->bSame(szSubValue))
      return true;
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

  while (G_bEachMain(eOfMainKind)) {
    if (! (G.p_SubOfMain = G.p_MainCard->pFindSubCard(eAndSubKind1)))
      continue;
    if (! (G.p_Sub2OfMain = G.p_MainCard->pFindSubCard(eAndSubKind2)))
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
  G_vMember(eOfMainKind, nullptr, __func__);
  G.p_SubOfMain = nullptr;
  G.e_SubOfMain = CK_NONE;
  G_vMember(eAndSubKind1, nullptr, __func__);
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

  while (G_bEachMain(eOfMainKind)) {
    if (! (G.p_SubOfMain = G.p_MainCard->pFindSubCard(eAndSubKind1)))
      continue;
    if (! (G.p_Sub2OfMain = G.p_MainCard->pFindSubCard(eAndSubKind2)))
      continue;
    if (! (G.p_Sub3OfMain = G.p_MainCard->pFindSubCard(eAndSubKind3)))
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
  G_vMember(eOfMainKind, nullptr, __func__);
  G.p_SubOfMain = nullptr;
  G.e_SubOfMain = CK_NONE;
  G_vMember(eAndSubKind1, nullptr, __func__);
  G.p_Sub2OfMain = nullptr;
  G.e_Sub2OfMain = CK_NONE;
  G_vMember(eAndSubKind2, nullptr, __func__);
  G.p_Sub3OfMain = nullptr;
  G.e_Sub3OfMain = CK_NONE;
  G_vMember(eAndSubKind3, nullptr, __func__);
  return false;
}

/*----------------------------------------------------------------------------*/
/* step main register to next main which has sub and update M member          */
/*----------------------------------------------------------------------------*/
bool CardIter::G_bEachSubOfMain(eCK eSubKind)
{
  _precondition_( _MAGIC_, _FOOTER_);

  while (G_bEachMain()) {
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
bool CardIter::G_bEachSubOfMain_reverse(eCK eSubKind)
{
  _precondition_( _MAGIC_, _FOOTER_);

  while (G_bEachMain_reverse()) {
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

  while (G_bEachMain()) {
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

  while (G_bEachMain()) {
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
/* end of transformation                                                      */
/*----------------------------------------------------------------------------*/
void CardIter::vEndTransform(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  vRenumberCards();

  //m_pCardCtl->vUseXform();
  // UNDO: Card::vConsistencyCheck(m_pDeck, m_iXform);

  vPrintBoard();

  //m_pCardCtl->vUseDefault();

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Expr* CardIter::pExpr_For_col(Card_Term* p_term, i8 i_Before)
{
  _precondition_( _MAGIC_, _FOOTER_);


  if (C_UNKNOWN == i_Before)
    i_Before = this->i_Child();

  Card_Expr* p_expr_col = nullptr;

  for (i8 i_Main = i_Before - 1; i_Main >= 0; i_Main--) {
    Card *pMain = this->G.pDeck->pGetChild(i_Main);

    //pMain->vDump();

    if (Card_Projection* p_projection = pMain->pSub_projection()) {
      i8 nChildren_prj = p_projection->nChildCards();
      for (i8 i_Child = 0; i_Child < nChildren_prj; i_Child++) {
        Card *pChild_prj = p_projection->pGetChild(i_Child);
        if (pChild_prj->bIsCard(CK_EXPR)) {
          Card_Expr* p_expr_prj = _pCard_Expr(pChild_prj);
          if (Card_RpnCalc* p_rpnCalc = p_expr_prj->pSub_rpnCalc()) {
            //p_rpnCalc->vDump();
            if (1 == p_rpnCalc->nChildCards() &&
                p_rpnCalc->pGetTerm(0)->szSymbol()->bSame(p_term->szSymbol())) {
              p_expr_col = p_expr_prj;
              break;
            }
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
Card_Expr* CardIter::pExpr_For_colNm(Card_Term* p_term, i8 i_Before)
{
  _precondition_( _MAGIC_, _FOOTER_);

  if (C_UNKNOWN == i_Before)
    i_Before = this->i_Child();

  Card_Expr* p_expr_col = nullptr;

  for (i8 i_Main = i_Before - 1; i_Main >= 0; i_Main--) {
    Card *pMain = this->G.pDeck->pGetChild(i_Main);

    if (Card_Projection* p_projection = pMain->pSub_projection()) {
      i8 nChildren_prj = p_projection->nChildCards();
      for (i8 i_Child = 0; i_Child < nChildren_prj; i_Child++) {
        Card *pChild_prj = p_projection->pGetChild(i_Child);
        if (pChild_prj->bIsCard(CK_EXPR)) {
          Card_Expr* p_expr_prj = _pCard_Expr(pChild_prj);
          if (Card_As* p_as = p_expr_prj->pSub_as()) {
            //p_as->vDump();
            if (p_as->bSameSymbol(p_term->szSymbol())) {
              p_expr_col = p_expr_prj;
              break;
            }
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

  G.e_MainCard = G.p_MainCard->eCard();
  G.i_MainCard = this->i_Child();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vForward(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(G.xKid.m_bActive);

  Card* pChild = pNextChild();

  UNUSED(pChild);
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

    case CK_ABSJOIN:          return G.p_absJoin;
    case CK_AGGREG:           return G.p_aggreg;
    case CK_ANDPRED:          return G.p_andPred;
    case CK_AS:               return G.p_as;

    case CK_BOOLCALC:         return G.p_boolCalc;
    case CK_BOUND:            return G.p_bound;
    case CK_BOUNDS:           return G.p_bounds;

    case CK_CALL:             return G.p_call;
    case CK_CAST:             return G.p_cast;
    case CK_CMPPRED:          return G.p_cmpPred;
    case CK_COMBINED:         return G.p_combined;
    case CK_CONSTRAINT:       return G.p_constraint;
    case CK_CORRELATION:      return G.p_correlation;
    case CK_COSTJOINING:      return G.p_costJoining;
    case CK_COSTNETWORK:      return G.p_costNetwork;
    case CK_COSTSTREAM:       return G.p_costQuery;
    case CK_COSTUNICAST:      return G.p_costUnicast;
    case CK_COUNT:            return G.p_count;
    case CK_CROSSJOIN:        return G.p_crossJoin;

    case CK_DIRECTION:        return G.p_direction;

    case CK_EXPR:             return G.p_expr;

    case CK_FILTER:           return G.p_filter;
    case CK_FILTPRED:         return G.p_filtPred;
    case CK_FILTPREDLIST:     return G.p_filtPredList;
    case CK_FLAT:             return G.p_flat;
    case CK_FOLD:             return G.p_fold;
    case CK_FOLDELIST:        return G.p_foldElist;
    case CK_FROM:             return G.p_from;

    case CK_GBYAGG:           return G.p_gbyAgg;

    case CK_HASH:             return G.p_hash;
    case CK_HAVING:           return G.p_having;
    case CK_HSHCOL:           return G.p_hshCol;

    case CK_IDXCOL:           return G.p_idxCol;
    case CK_IDXCOLLIST:       return G.p_idxColList;
    case CK_INDEX:            return G.p_index;
    case CK_INDEXJOIN:        return G.p_indexJoin;
    case CK_INDEXLIST:        return G.p_indexList;
    case CK_INDEXNM:          return G.p_indexNm;
    case CK_INDEXSCAN:        return G.p_indexScan;

    case CK_JOINCAST:         return G.p_joinCast;
    case CK_JOINESTIMATE:     return G.p_joinEstimate;
    case CK_JOINORDER:        return G.p_joinOrder;
    case CK_JOINORDERING:     return G.p_joinOrdering;
    case CK_JOINPREDLIST:     return G.p_joinPredList;

    case CK_KEY:              return G.p_key;
    case CK_KEYCELLS:         return G.p_keyCells;
    case CK_KEYEXISTS:        return G.p_keyExists;
    case CK_KEYNOTEXISTS:     return G.p_keyNotExists;

    case CK_LEVEL:            return G.p_level;
    case CK_LIMIT:            return G.p_limit;

    case CK_MAP:              return G.p_map;
    case CK_MAPPING:          return G.p_mapping;

    case CK_NCORRCOLS:        return G.p_nCorrCols;
    case CK_NEEDED:           return G.p_needed;
    case CK_NOTE:             return G.p_note;
    case CK_NOTPRED:          return G.p_notPred;

    case CK_ON:               return G.p_on;
    case CK_ORDAGG:           return G.p_ordAgg;
    case CK_ORPRED:           return G.p_orPred;
    case CK_OUTERCOLNM:       return G.p_outerColNm;
    case CK_OUTERJOIN:        return G.p_outerJoin;

    case CK_PAYLOADCELLS:     return G.p_payloadCells;
    case CK_PROJECT:          return G.p_project;
    case CK_PROJECTION:       return G.p_projection;

    case CK_ROWCOUNT:         return G.p_rowCount;
    case CK_ROWSIZE:          return G.p_rowSize;
    case CK_RPNCALC:          return G.p_rpnCalc;

    case CK_SCAN:             return G.p_scan;
    case CK_SECTION:          return G.p_section;
    case CK_SEND:             return G.p_send;
    case CK_STREAMHEAD:       return G.p_streamHead;
    case CK_STREAMJOIN:       return G.p_streamJoin;
    case CK_STREAMKEY:        return G.p_streamKey;
    case CK_STREAMPER:        return G.p_streamPer;
    case CK_STREAMPTN:        return G.p_streamPtn;
    case CK_STREAMSET:        return G.p_streamSet;
    case CK_SORT:             return G.p_sort;
    case CK_STAGE:            return G.p_stage;
    case CK_SUBSET:           return G.p_subSet;

    case CK_TABLE:            return G.p_table;
    case CK_TABLELIST:        return G.p_tableList;
    case CK_TABLENM:          return G.p_tableNm;
    case CK_TABLEPTN:         return G.p_tablePtn;
    case CK_TABLESIZE:        return G.p_tableSize;
    case CK_TERM:             return G.p_term;
    case CK_TOP:              return G.p_top;
    case CK_TOPOLOGY:         return G.p_topology;

    case CK_UNIQUE:           return G.p_unique;

    case CK_WHERE:            return G.p_where;

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

  G.pDeck->vInsertAfter(pCard,pAfter);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::G_vInsertBefore(Card* pCard, Card* pBefore)
{
  _precondition_(_MAGIC_, _FOOTER_);

  G.pDeck->vInsertBefore(pCard,pBefore);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool CardIter::G_bIsBoardNm(Stz* szBoardNm)
{
  _precondition_(_MAGIC_, _FOOTER_);

  if (m_pBench->pGetBoard(szBoardNm))
    return true;

  return false;
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
bool CardIter::bJustRestarted(void)
{
  _precondition_(_MAGIC_, _FOOTER_);

  if (C_UNKNOWN == G.xKid.i_Child() &&
      nullptr == G.xKid.pChild() &&
      true == G.xKid.m_bActive)
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::G_vMainResynch(void)
{
  _precondition_(_MAGIC_, _FOOTER_);

  G.xKid.m_bActive = true;

  for (G.xKid.vChild(-1); G.xKid.bEach(); /**/)
    if (G.p_MainCard == G.xKid.pChild())
      break;

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
/* set member card                                                         */
/*----------------------------------------------------------------------------*/
void CardIter::G_vMember(eCK eMember, Card* pCard, const char* asFuncNm)
{
  _precondition_(_MAGIC_, _FOOTER_);

  RASSERT(!pCard || pCard->bIsCard(eMember));

  switch (eMember) {

    case CK_NONE: break;

    case CK_ABSJOIN:          *((Card**)&G.p_absJoin)          = pCard; break;
    case CK_AGGREG:           *((Card**)&G.p_aggreg)           = pCard;
                              *((Card**)&G.p_OP)               = pCard; break;
    case CK_ANDPRED:          *((Card**)&G.p_andPred)          = pCard;
                              *((Card**)&G.p_PRED)             = pCard; break;
    case CK_AS:               *((Card**)&G.p_as)               = pCard; break;

    case CK_BOOLCALC:         *((Card**)&G.p_boolCalc)         = pCard; break;
    case CK_BOUND:            *((Card**)&G.p_bound)            = pCard; break;
    case CK_BOUNDS:           *((Card**)&G.p_bounds)           = pCard; break;

    case CK_CALL:             *((Card**)&G.p_call)             = pCard; break;
    case CK_CAST:             *((Card**)&G.p_cast)             = pCard;
                              *((Card**)&G.p_OP)               = pCard; break;
    case CK_CMPPRED:          *((Card**)&G.p_cmpPred)          = pCard;
                              *((Card**)&G.p_PRED)             = pCard; break;

    case CK_COMBINED:         *((Card**)&G.p_combined)         = pCard;
                              *((Card**)&G.p_OP)               = pCard; break;
    case CK_CONSTRAINT:       *((Card**)&G.p_constraint)       = pCard; break;
    case CK_CORRELATION:      *((Card**)&G.p_correlation)      = pCard; break;
    case CK_COSTJOINING:      *((Card**)&G.p_costJoining)      = pCard; break;
    case CK_COSTNETWORK:      *((Card**)&G.p_costNetwork)      = pCard; break;
    case CK_COSTSTREAM:       *((Card**)&G.p_costQuery)        = pCard; break;
    case CK_COSTUNICAST:      *((Card**)&G.p_costUnicast)      = pCard; break;
    case CK_COUNT:            *((Card**)&G.p_count)            = pCard; break;
    case CK_CROSSJOIN:        *((Card**)&G.p_crossJoin)        = pCard;
                              *((Card**)&G.p_OP)               = pCard; break;

    case CK_DIRECTION:        *((Card**)&G.p_direction)        = pCard; break;

    case CK_EXPR:             *((Card**)&G.p_expr)             = pCard; break;

    case CK_FILTER:           *((Card**)&G.p_filter)           = pCard;
                              *((Card**)&G.p_OP)               = pCard; break;
    case CK_FILTPRED:         *((Card**)&G.p_filtPred)         = pCard; break;
    case CK_FILTPREDLIST:     *((Card**)&G.p_filtPredList)     = pCard; break;
    case CK_FLAT:             *((Card**)&G.p_flat)             = pCard; break;
    case CK_FOLD:             *((Card**)&G.p_fold)             = pCard; break;
    case CK_FOLDELIST:        *((Card**)&G.p_foldElist)        = pCard; break;
    case CK_FROM:             *((Card**)&G.p_from)             = pCard; break;

    case CK_GBYAGG:           *((Card**)&G.p_gbyAgg)           = pCard;
                              *((Card**)&G.p_OP)               = pCard; break;

    case CK_HASH:             *((Card**)&G.p_hash)             = pCard;
                              *((Card**)&G.p_OP)               = pCard; break;
    case CK_HAVING:           *((Card**)&G.p_having)           = pCard;
                              *((Card**)&G.p_OP)               = pCard; break;
    case CK_HSHCOL:           *((Card**)&G.p_hshCol)           = pCard; break;

    case CK_IDXCOL:           *((Card**)&G.p_idxCol)           = pCard; break;
    case CK_IDXCOLLIST:       *((Card**)&G.p_idxColList)       = pCard; break;
    case CK_INDEX:            *((Card**)&G.p_index)            = pCard; break;
    case CK_INDEXJOIN:        *((Card**)&G.p_indexJoin)        = pCard;
                              *((Card**)&G.p_OP)               = pCard; break;
    case CK_INDEXLIST:        *((Card**)&G.p_indexList)        = pCard; break;
    case CK_INDEXNM:          *((Card**)&G.p_indexNm)          = pCard; break;
    case CK_INDEXSCAN:        *((Card**)&G.p_indexScan)        = pCard;
                              *((Card**)&G.p_OP)               = pCard; break;

    case CK_JOINCAST:         *((Card**)&G.p_joinCast)         = pCard; break;
    case CK_JOINESTIMATE:     *((Card**)&G.p_joinEstimate)     = pCard; break;
    case CK_JOINORDER:        *((Card**)&G.p_joinOrder)        = pCard; break;
    case CK_JOINORDERING:     *((Card**)&G.p_joinOrdering)     = pCard; break;
    case CK_JOINPREDLIST:     *((Card**)&G.p_joinPredList)     = pCard; break;

    case CK_KEY:              *((Card**)&G.p_key)              = pCard; break;
    case CK_KEYCELLS:         *((Card**)&G.p_keyCells)         = pCard; break;
    case CK_KEYEXISTS:        *((Card**)&G.p_keyExists)        = pCard;
                              *((Card**)&G.p_OP)               = pCard; break;
    case CK_KEYNOTEXISTS:     *((Card**)&G.p_keyNotExists)     = pCard;
                              *((Card**)&G.p_OP)               = pCard; break;

    case CK_LEVEL:            *((Card**)&G.p_level)            = pCard; break;
    case CK_LIMIT:            *((Card**)&G.p_limit)            = pCard;
                              *((Card**)&G.p_OP)               = pCard; break;

    case CK_MAP:              *((Card**)&G.p_map)              = pCard; break;
    case CK_MAPPING:          *((Card**)&G.p_mapping)          = pCard; break;

    case CK_NCORRCOLS:        *((Card**)&G.p_nCorrCols)        = pCard; break;
    case CK_NEEDED:           *((Card**)&G.p_needed)           = pCard; break;
    case CK_NOTE:             *((Card**)&G.p_note)             = pCard; break;
    case CK_NOTPRED:          *((Card**)&G.p_notPred)          = pCard;
                              *((Card**)&G.p_PRED)             = pCard; break;

    case CK_ON:               *((Card**)&G.p_on)               = pCard; break;
    case CK_ORDAGG:           *((Card**)&G.p_ordAgg)           = pCard;
                              *((Card**)&G.p_OP)               = pCard; break;
    case CK_ORPRED:           *((Card**)&G.p_orPred)           = pCard;
                              *((Card**)&G.p_PRED)             = pCard; break;
    case CK_OUTERCOLNM:       *((Card**)&G.p_outerColNm)       = pCard; break;
    case CK_OUTERJOIN:        *((Card**)&G.p_outerJoin)        = pCard; break;

    case CK_PAYLOADCELLS:     *((Card**)&G.p_payloadCells)     = pCard; break;
    case CK_PROJECT:          *((Card**)&G.p_project)          = pCard;
                              *((Card**)&G.p_OP)               = pCard; break;
    case CK_PROJECTION:       *((Card**)&G.p_projection)       = pCard; break;

    case CK_ROWCOUNT:         *((Card**)&G.p_rowCount)         = pCard; break;
    case CK_ROWSIZE:          *((Card**)&G.p_rowSize)          = pCard; break;
    case CK_RPNCALC:          *((Card**)&G.p_rpnCalc)          = pCard; break;

    case CK_SCAN:             *((Card**)&G.p_scan)             = pCard;
                              *((Card**)&G.p_OP)               = pCard; break;
    case CK_SECTION:          *((Card**)&G.p_section)          = pCard; break;
    case CK_SEND:             *((Card**)&G.p_send)             = pCard;
                              *((Card**)&G.p_OP)               = pCard; break;
    case CK_SORT:             *((Card**)&G.p_sort)             = pCard;
                              *((Card**)&G.p_OP)               = pCard; break;
    case CK_STAGE:            *((Card**)&G.p_stage)            = pCard;
                              *((Card**)&G.p_OP)               = pCard; break;
    case CK_STREAMHEAD:       *((Card**)&G.p_streamHead)       = pCard; break;
    case CK_STREAMJOIN:       *((Card**)&G.p_streamJoin)       = pCard; break;
    case CK_STREAMKEY:        *((Card**)&G.p_streamKey)        = pCard; break;
    case CK_STREAMPER:        *((Card**)&G.p_streamPer)        = pCard; break;
    case CK_STREAMPTN:        *((Card**)&G.p_streamPtn)        = pCard; break;
    case CK_STREAMSET:        *((Card**)&G.p_streamSet)        = pCard; break;
    case CK_SUBSET:           *((Card**)&G.p_subSet)           = pCard; break;
    case CK_TABLE:            *((Card**)&G.p_table)            = pCard;
                              *((Card**)&G.p_OP)               = pCard; break;

    case CK_TABLELIST:        *((Card**)&G.p_tableList)        = pCard; break;
    case CK_TABLENM:          *((Card**)&G.p_tableNm)          = pCard; break;
    case CK_TABLEPTN:         *((Card**)&G.p_tablePtn)         = pCard; break;
    case CK_TABLESIZE:        *((Card**)&G.p_tableSize)        = pCard; break;
    case CK_TERM:             *((Card**)&G.p_term)             = pCard; break;
    case CK_TOP:              *((Card**)&G.p_top)              = pCard;
                              *((Card**)&G.p_OP)               = pCard; break;
    case CK_TOPOLOGY:         *((Card**)&G.p_topology)         = pCard; break;

    case CK_UNIQUE:           *((Card**)&G.p_unique)           = pCard; break;

    case CK_WHERE:            *((Card**)&G.p_where)            = pCard;
                              *((Card**)&G.p_OP)               = pCard; break;
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

  RASSERT(G.xKid.m_bActive);

  if (G.xKid.i_Child() + 1 < G.xKid.pParent()->nChildCards())
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardIter::pNextChild(void)
{
  _precondition_(_MAGIC_, _FOOTER_);

  RASSERT(G.xKid.m_bActive);

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

  RASSERT(G.xKid.m_bActive);

  Card* pNextMain = this->pNextChild();
  if (! pNextMain || eCard != pNextMain->eCard())
    return nullptr;

  return pNextMain;
}

/*----------------------------------------------------------------------------*/
/* create a new card and optionally set member                                */
/*----------------------------------------------------------------------------*/
void CardIter::G_vNewMember(eCK eMember, bool bUpdateMember)
{
  _precondition_(_MAGIC_, _FOOTER_);

  G.p_NEW = new Card(eMember);

  if (bUpdateMember)
    G_vMember(eMember, G.p_NEW, __func__);
}

/*----------------------------------------------------------------------------*/
/* step to next main and set member                                           */
/*----------------------------------------------------------------------------*/
void CardIter::G_vNewMember(eCK eMember, Stz* szValue, bool bUpdateMember)
{
  _precondition_(_MAGIC_, _FOOTER_);

  G_vNewMember(eMember, bUpdateMember);

  G.p_NEW->vUpdateSymbol(Symbol(SF_OTHER,szValue));
}

/*----------------------------------------------------------------------------*/
/* clear the M members                                                        */
/*----------------------------------------------------------------------------*/
void CardIter::vNullMembers(bool bNull_OP)
{
  _precondition_( _MAGIC_, _FOOTER_);

  G.p_absJoin          = nullptr;
  G.p_aggreg           = nullptr;
  G.p_andPred          = nullptr;
  G.p_as               = nullptr;

  G.p_boolCalc         = nullptr;
  G.p_bound            = nullptr;
  G.p_bounds           = nullptr;

  G.p_call             = nullptr;
  G.p_cast             = nullptr;
  G.p_cmpPred          = nullptr;
  G.p_combined         = nullptr;
  G.p_constraint       = nullptr;
  G.p_correlation      = nullptr;
  G.p_costJoining      = nullptr;
  G.p_costNetwork      = nullptr;
  G.p_costQuery        = nullptr;
  G.p_costUnicast      = nullptr;
  G.p_count            = nullptr;
  G.p_crossJoin        = nullptr;

  G.p_direction        = nullptr;

  G.p_expr             = nullptr;

  G.p_filter           = nullptr;
  G.p_filtPred         = nullptr;
  G.p_filtPredList     = nullptr;
  G.p_flat             = nullptr;
  G.p_fold             = nullptr;
  G.p_foldElist        = nullptr;
  G.p_from             = nullptr;

  G.p_gbyAgg           = nullptr;

  G.p_hash             = nullptr;
  G.p_having           = nullptr;
  G.p_hshCol           = nullptr;

  G.p_idxCol           = nullptr;
  G.p_idxColList       = nullptr;
  G.p_index            = nullptr;
  G.p_indexJoin        = nullptr;
  G.p_indexList        = nullptr;
  G.p_indexNm          = nullptr;
  G.p_indexScan        = nullptr;

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

  G.p_nCorrCols        = nullptr;
  G.p_needed           = nullptr;
  G.p_note             = nullptr;
  G.p_notPred          = nullptr;

  G.p_on               = nullptr;
  G.p_ordAgg           = nullptr;
  G.p_orPred           = nullptr;
  G.p_outerColNm       = nullptr;
  G.p_outerJoin        = nullptr;

  G.p_payloadCells     = nullptr;
  G.p_project          = nullptr;
  G.p_projection       = nullptr;

  G.p_rowCount         = nullptr;
  G.p_rowSize          = nullptr;
  G.p_rpnCalc          = nullptr;

  G.p_scan             = nullptr;
  G.p_section          = nullptr;
  G.p_send             = nullptr;
  G.p_streamKey        = nullptr;
  G.p_streamPer        = nullptr;
  G.p_streamPtn        = nullptr;
  G.p_streamSet        = nullptr;
  G.p_sort             = nullptr;
  G.p_stage            = nullptr;
  G.p_streamHead       = nullptr;
  G.p_streamJoin       = nullptr;
  G.p_subSet           = nullptr;

  G.p_table            = nullptr;
  G.p_tableList        = nullptr;
  G.p_tableNm          = nullptr;
  G.p_tablePtn         = nullptr;
  G.p_tableSize        = nullptr;
  G.p_term             = nullptr;
  G.p_top              = nullptr;
  G.p_topology         = nullptr;

  G.p_unique           = nullptr;

  G.p_where            = nullptr;

  G.p_NEW              = nullptr;
  G.p_PRED             = nullptr;

  if (bNull_OP)
    G.p_OP = nullptr;
}
/*----------------------------------------------------------------------------*/
/* pop/restore M iterator from iter stack                                     */
/*----------------------------------------------------------------------------*/
void CardIter::G_vPopIter(void)
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

  RASSERT(G.xKid.m_bActive);

  i8 i_Prev = G.xKid.i_Child() - 1;

  if (i_Prev < 0)
    return nullptr;

  Card* pChild = G.xKid.pParent()->pGetChild(i_Prev);

  return pChild;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardIter::pPrevChildIf(eCK eCard)
{
  _precondition_(_MAGIC_, _FOOTER_);

  RASSERT(G.xKid.m_bActive);

  Card* pPrevMain = this->pPrevChild();
  if (! pPrevMain || eCard != pPrevMain->eCard())
    return nullptr;

  return pPrevMain;
}

/*----------------------------------------------------------------------------*/
/* print the board                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vPrintBoard(i8 iLineNo)
{
  _precondition_(_MAGIC_, _FOOTER_);

  if (! AM::JOB::bHasTag(C_GLEAN_INTERACTIVE))
    return;

  G.pBoard->vPrintBoard(szXdesc());

  s256 sWhere;
  sWhere.vCatSprint("%s :: %s"_Stz, szXhand(), szXfunc());

  vTrace("%s\n"_Stz, sWhere.azChar());
}
/*----------------------------------------------------------------------------*/
//void CardIter::vPrintBoard(i8 iLineNo)
//{
//  _precondition_(_MAGIC_,_FOOTER_);
//
//  if (! m_pCardCtl->bBoardPrint())
//    return;
//
//  char cPrefix = m_pBench->cPrefix();
//
//  s256 sPrompt;
//
//  i8 nBoards = m_pBench->nItems();
//
//  for (;;) {
//    G.pBoard->vPrintBoard(szXdesc());
//
//    if (! m_pCardCtl->bCueUser(szXhand(), szXfunc())) {
//      m_pCardCtl->vFormPrompt(sPrompt,
//                              cPrefix, szXhand(), szXfunc(),
//                              G.pBoard->i_Board(), nBoards, iLineNo);
//      printf("%s\n", sPrompt.azChar());
//      break;
//    }
//
//    m_pCardCtl->vFormPrompt(sPrompt,
//                            cPrefix, szXhand(), szXfunc(),
//                            G.pBoard->i_Board(), nBoards, iLineNo);
//    if (OA_REPRINT_BOARD != eCueUser(sPrompt))
//      break;
//  }
//}

/*----------------------------------------------------------------------------*/
/* capture prior projection                                                   */
/*----------------------------------------------------------------------------*/
void CardIter::vPriorProjection(void)
{
  _precondition_(_MAGIC_, _FOOTER_);

  if (Card_Projection* p_projection = G.p_OP->pSub_projection())
    Prior.m_pProjection = p_projection;
}

/*----------------------------------------------------------------------------*/
/* push current M iterator onto iter stack                                    */
/*----------------------------------------------------------------------------*/
void CardIter::G_vPushBoard(Stz* szBoardNm)
{
  _precondition_(_MAGIC_, _FOOTER_);

  Board* pBoard = m_pBench->pGetBoard(szBoardNm);

  G_vPushBoard(pBoard);
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
/* push board                                                                 */
/*----------------------------------------------------------------------------*/
bool CardIter::G_bPushBoard(i8 i_Board)
{
  _precondition_(_MAGIC_, _FOOTER_);

  Board* pBoard = m_pBench->pGetBoard(i_Board);
  RASSERT(pBoard);

  if (pBoard) {
    G_vPushBoard(pBoard);
    return true;
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* push current M iterator onto iter stack                                    */
/*----------------------------------------------------------------------------*/
bool CardIter::G_bPushBoard(Stz* szBoardNm)
{
  _precondition_(_MAGIC_, _FOOTER_);

  Board* pSubQueryBoard = m_pBench->pGetBoard(szBoardNm);
  if (! pSubQueryBoard) {
     return false;
  }

  G_vPushBoard(pSubQueryBoard);

  return true;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::G_vPushNewChild(Card* pParent, eCK eChild)
{
  _precondition_(_MAGIC_, _FOOTER_);

  Card* pChild = new Card(eChild);

  pParent->vPush(pChild);
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
Card* CardIter::pRecurseEach(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // descend into current child

  if (G.p_MainCard && (! G.m_bDontDescend) ) {
    if (G.p_MainCard->nChildCards()) {

      G.m_bRecursed = true;
      vPushRecurseIter();
      vChangeCard(G.p_MainCard);
      G_bEachMain();

      return G.p_MainCard;
    }
  }

  G.m_bDontDescend = false;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // visit next child

  if (G_bEachMain())
    return G.p_MainCard;

  while (bPopRecurseIter()) {
    if (G_bEachMain())
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
void CardIter::vRemoveChild(Card* pChild)
{
  _precondition_(_MAGIC_, _FOOTER_);

  RASSERT(G.xKid.m_bActive);

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

  RASSERT(G.xKid.m_bActive);

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

  RASSERT(G.xKid.m_bActive);

  RASSERT(eCrossCheck == G.xKid.eChild());

  Card* pChild = G.xKid.pChild();

  G.xKid.pParent()->vRemoveChild(pChild);

  G.xKid.vPrev();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vRemoveDeleteCurr(eCK eCrossCheck)
{
  _precondition_(_MAGIC_, _FOOTER_);

  RASSERT(G.xKid.m_bActive);

  RASSERT(eCrossCheck == G.xKid.eChild());

  Card* pChild = G.xKid.pChild();

  G.xKid.pParent()->vRemoveChild(pChild);

  delete pChild;

  G.xKid.vPrev();
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
/* remove main card from deck (do not clear M members or registers)           */
/*----------------------------------------------------------------------------*/
void CardIter::vRenumberCards(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  m_pBench->vRenumberCards();
}

/*----------------------------------------------------------------------------*/
/* resolve prior expr of given term (find right-to-left)                      */
/*----------------------------------------------------------------------------*/
Card_Expr* CardIter::pResolvePrior_expr(Card_Term* p_term)
{
  _precondition_( _MAGIC_, _FOOTER_);

  Card_Projection* p_projection_prior = pResolvePrior_projection();
  if (! p_projection_prior)
    return nullptr;

   if (p_term->pSymbol()->bIsStreamOrdinal()) {
    Card_Expr *p_expr_prior = p_projection_prior->pDecodeExpr(p_term->pSymbol());
    return p_expr_prior;
  }
  else
  if (p_term->pSymbol()->bIsColumnOrdinal()) {
    Card_Expr *p_expr_prior = p_projection_prior->pFindExpr(p_term->pSymbol());
    return p_expr_prior;
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* resolve prior projection (find right-to-left)                              */
/*----------------------------------------------------------------------------*/
Card_Projection* CardIter::pResolvePrior_projection(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  Card_Projection* p_projection_prior = Card_Projection::pResolvePriorProjection(this);

  return p_projection_prior;
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
void CardIter::vRewind(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(G.xKid.m_bActive);

//  if (G.xKid.m_i_Child == 0) {
//    G.xKid.m_i_Child = C_UNKNOWN;
//    return;
//  }
//
//  G.xKid.m_i_Child--;

  G.xKid.vPrev();
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

  G.xKid.vPrev();

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

  RASSERT(G.xKid.m_bActive);

  G.xKid.vChild(G.xKid.pParent()->iFindChild(pChild));
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vSet(CardIter* pCardIter_src)
{
  _precondition_(_MAGIC_,_FOOTER_);

  G.xKid.vParent(pCardIter_src->G.xKid.pParent());
  G.xKid.vChild(pCardIter_src->G.xKid.i_Child());

  G.xKid.m_bActive = pCardIter_src->G.xKid.m_bActive;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vSet(i8 i_Child)
{
  _precondition_(_MAGIC_, _FOOTER_);

  RASSERT(G.xKid.m_bActive);

  G.xKid.vChild(i_Child);
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

  this->vBeginEach();
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

  CardIterMembers* pXsentinals2 = pPeek(0);
  if (! pXsentinals2 || ! pXsentinals2->m_bRecursed)
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

  RASSERT(G.m_bRecursed);

  vSave();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/