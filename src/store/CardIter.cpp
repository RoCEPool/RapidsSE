/*============================================================================*/
/* CardIter.cpp --                                                            */
/*============================================================================*/

#include "CardIter.hpp"

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
CardIter::CardIter(void)
    : Object(CN_CARDITER)
{
  _precondition_(_MAGIC_,_FOOTER_);

  //MemLib::vZero(&G2,sizeof(G2),CN_CARDITER);

  G2.m_pCard   = nullptr;
  G2.m_i_Child = C_UNKNOWN;
  G2.m_pChild  = nullptr;

  G2.m_bDontDescend = false;

  G2.m_bActive = false;

  m_pRecurseStack = nullptr;


  _checkInitialised_(CN_CARDITER);

  _postcondition_(_MAGIC_,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
CardIter::CardIter(Card*  pCard)
: Object(CN_CARDITER)
{
  _precondition_(_MAGIC_,_FOOTER_);

  //MemLib::vZero(&G2,sizeof(G2),CN_CARDITER);

  G2.m_pCard   = pCard;
  G2.m_i_Child = C_UNKNOWN;
  G2.m_pChild  = nullptr;

  G2.m_bActive = true;

  m_pRecurseStack = nullptr;


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

  //MemLib::vZero(&G2,sizeof(G2),CN_CARDITER);

  m_pRecurseStack = nullptr;

  vSet(pCardIter_src);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
CardIter::CardIter(eCN eClass, Card*  pCard)
    : Object(eClass)
{
  _precondition_(_MAGIC_,_FOOTER_);

  //MemLib::vZero(&G2,sizeof(G2),CN_CARDITER);

  G2.m_pCard   = pCard;
  G2.m_i_Child = C_UNKNOWN;
  G2.m_pChild  = nullptr;

  G2.m_bActive = true;

  m_pRecurseStack = nullptr;


  _checkInitialised_(CN_CARDITER);

  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
CardIter::~CardIter(void) noexcept(false)
{
  _precondition_(_MAGIC_,_FOOTER_);

  delete m_pRecurseStack;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vChangeCard(Card* pCard)
{
  _precondition_(_MAGIC_, _FOOTER_);

  G2.m_pCard = pCard;

  vRestart();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
i8 CardIter::i_Child(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  return G2.m_i_Child;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardIter::pChild(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  return G2.m_pChild;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vDontDescend(void)
{
  _precondition_(_MAGIC_, _FOOTER_);

  G2.m_bDontDescend = true;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool CardIter::bEachChild(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(G2.m_bActive);

  if (pEachChild())
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardIter::pEachChild(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(G2.m_bActive);

  i8 i_Main;

  G2.m_pChild = pEachChild(i_Main);

  return G2.m_pChild;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardIter::pEachChild(i8& ri_Main)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(G2.m_bActive);

  if (C_UNKNOWN == G2.m_i_Child)
    G2.m_i_Child = 0;
  else
    G2.m_i_Child++;

  if (G2.m_i_Child >= G2.m_pCard->nChildCards()) {
    G2.m_pChild = nullptr;
    ri_Main  = C_UNKNOWN;
    G2.m_bActive = false;

    return nullptr;
  }

  G2.m_pChild = G2.m_pCard->pGetChild(G2.m_i_Child);
  ri_Main = G2.m_i_Child;

  return G2.m_pChild;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardIter::pEachChild(eCK eCard)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(G2.m_bActive);

  i8 i_Main;

  Card* pMain = pEachChild(eCard, i_Main);

  return pMain;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardIter::pEachChild(eCK eCard, i8& ri_Main)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(G2.m_bActive);

  if (C_UNKNOWN == G2.m_i_Child)
    G2.m_i_Child = 0;
  else
    G2.m_i_Child++;

  for (;;) {

    if (G2.m_i_Child >= G2.m_pCard->nChildCards()) {
      G2.m_pChild  = nullptr;
      ri_Main   = C_UNKNOWN;
      G2.m_bActive = false;
      return nullptr;
    }

    G2.m_pChild = G2.m_pCard->pGetChild(G2.m_i_Child);

    eCK eCardMain = G2.m_pChild->eCard();
    if (eCardMain == eCard) {
      ri_Main = G2.m_i_Child;
      return G2.m_pChild;
    }
    G2.m_i_Child++;
  }

  NEVER_REACH
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardIter::pEachChild(i8& ri_Main, eCK& reCard)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(G2.m_bActive);

  if (C_UNKNOWN == G2.m_i_Child)
    G2.m_i_Child = 0;
  else
    G2.m_i_Child++;

  if (G2.m_i_Child >= G2.m_pCard->nChildCards()) {
    G2.m_pChild  = nullptr;
    ri_Main   = C_UNKNOWN;
    reCard    = CK_NONE;
    G2.m_bActive = false;
    return nullptr;
  }

  G2.m_pChild = G2.m_pCard->pGetChild(G2.m_i_Child);

  reCard = G2.m_pChild->eCard();
  ri_Main = G2.m_i_Child;

  return G2.m_pChild;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardIter::pEachChild_reverse(eCK eCard)
{
  _precondition_( _MAGIC_, _FOOTER_);

  if (C_UNKNOWN == G2.m_i_Child)
    G2.m_i_Child = G2.m_pCard->nChildCards() - 1;
  else
    G2.m_i_Child--;

  for (;;) {

    if (G2.m_i_Child < 0) {
      G2.m_pChild = nullptr;
      return nullptr;
    }

    G2.m_pChild = G2.m_pCard->pGetChild(G2.m_i_Child);

    eCK eCardMain = G2.m_pChild->eCard();
    if (eCardMain == eCard) {
      return G2.m_pChild;
    }

    G2.m_i_Child--;
  }

  NEVER_REACH
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vForward(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(G2.m_bActive);

  Card* pChild = pNextChild();

  UNUSED(pChild);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardIter::pGetChild(i8 i_Child)
{
  _precondition_(_MAGIC_, _FOOTER_);

  Card* pChild = G2.m_pCard->pGetChild(i_Child);

  return pChild;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool CardIter::bJustRestarted(void)
{
  _precondition_(_MAGIC_, _FOOTER_);

  if (C_UNKNOWN == G2.m_i_Child &&
      nullptr == G2.m_pChild &&
      true == G2.m_bActive)
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool CardIter::bLess(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(G2.m_bActive);

  if (G2.m_i_Child - 1 >= 0)
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool CardIter::bMore(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(G2.m_bActive);

  if (G2.m_i_Child + 1 < G2.m_pCard->nChildCards())
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardIter::pNextChild(void)
{
  _precondition_(_MAGIC_, _FOOTER_);

  RASSERT(G2.m_bActive);

  i8 i_Next = G2.m_i_Child + 1;

  if (i_Next >= G2.m_pCard->nChildCards())
    return nullptr;

  Card* pNextMain = G2.m_pCard->pGetChild(i_Next);

  return pNextMain;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardIter::pNextChildIf(eCK eCard)
{
  _precondition_(_MAGIC_, _FOOTER_);

  RASSERT(G2.m_bActive);

  Card* pNextMain = this->pNextChild();
  if (! pNextMain || eCard != pNextMain->eCard())
    return nullptr;

  return pNextMain;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardIter::pPrevChild(void)
{
  _precondition_(_MAGIC_, _FOOTER_);

  RASSERT(G2.m_bActive);

  i8 i_Prev = G2.m_i_Child - 1;

  if (i_Prev < 0)
    return nullptr;

  Card* pChild = G2.m_pCard->pGetChild(i_Prev);

  return pChild;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardIter::pPrevChildIf(eCK eCard)
{
  _precondition_(_MAGIC_, _FOOTER_);

  RASSERT(G2.m_bActive);

  Card* pPrevMain = this->pPrevChild();
  if (! pPrevMain || eCard != pPrevMain->eCard())
    return nullptr;

  return pPrevMain;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardIter::pRecurseEach(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // descend into current child

  if (G2.m_pChild && (! G2.m_bDontDescend) ) {
    if (G2.m_pChild->nChildCards()) {

      G2.m_bRecursed = true;
      vPushRecurseIter();
      vChangeCard(G2.m_pChild);
      G2.m_pChild = pEachChild();

      return G2.m_pChild;
    }
  }

  G2.m_bDontDescend = false;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // visit next child

  if (bEachChild())
    return G2.m_pChild;

  while (bPopRecurseIter()) {
    if (bEachChild())
      return G2.m_pChild;
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

  RASSERT(G2.m_bActive);

  if (pChild == G2.m_pChild)
    vRemoveCurr(G2.m_pChild->eCard());
  else
    G2.m_pCard->vRemoveChild(pChild);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vRemoveDeleteChild(Card*& rpChild)
{
  _precondition_(_MAGIC_, _FOOTER_);

  RASSERT(G2.m_bActive);

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

  RASSERT(G2.m_bActive);

  RASSERT(eCrossCheck == G2.m_pChild->eCard());
  G2.m_pCard->vRemoveChild(G2.m_pChild);
  G2.m_i_Child--;
  if (G2.m_i_Child < G2.m_pCard->nChildCards())
    G2.m_pChild = G2.m_pCard->pGetChild(G2.m_i_Child);
  else
    G2.m_pChild = nullptr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vRemoveDeleteCurr(eCK eCrossCheck)
{
  _precondition_(_MAGIC_, _FOOTER_);

  RASSERT(G2.m_bActive);

  RASSERT(eCrossCheck == G2.m_pChild->eCard());
  G2.m_pCard->vRemoveChild(G2.m_pChild);
  delete G2.m_pChild;
  G2.m_i_Child--;
  if (G2.m_i_Child < G2.m_pCard->nChildCards())
    G2.m_pChild = G2.m_pCard->pGetChild(G2.m_i_Child);
  else
    G2.m_pChild = nullptr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vRestart(void)
{
  _precondition_(_MAGIC_, _FOOTER_);

  G2.m_i_Child = C_UNKNOWN;
  G2.m_pChild  = nullptr;
  G2.m_bActive = true;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vRewind(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(G2.m_bActive);

//  Card* pChild = pPrevChild();
//
//  UNUSED(pChild);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardIter::pRewindChild(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  if (G2.m_i_Child == 0) {
    G2.m_i_Child = C_UNKNOWN;
    return nullptr;
  }

  G2.m_i_Child--;

  if (G2.m_i_Child < G2.m_pCard->nChildCards())
    G2.m_pChild =  G2.m_pCard->pGetChild(G2.m_i_Child);
  else
    G2.m_pChild = nullptr;

  return G2.m_pChild;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vRestore(void)
{
  _Xsentinals2* pRestore = pPop();

  G2.vCopyFrom(pRestore);

  delete pRestore;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vSave(void)
{
  _Xsentinals2* pSave = new _Xsentinals2;

  pSave->vCopyFrom(&G2);

  vPush(pSave);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
i8 CardIter::iSentinalsDepth(void)
{

  return this->nItems();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vSet(Card* pChild)
{
  _precondition_(_MAGIC_, _FOOTER_);

  RASSERT(G2.m_bActive);

  G2.m_i_Child = G2.m_pCard->iFindChild(pChild);

  if (C_UNKNOWN == G2.m_i_Child)
    G2.m_pChild = nullptr;
  else
    G2.m_pChild = pChild;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vSet(CardIter* pCardIter_src)
{
  _precondition_(_MAGIC_,_FOOTER_);

  G2.m_pCard   = pCardIter_src->G2.m_pCard;
  G2.m_i_Child = pCardIter_src->G2.m_i_Child;
  G2.m_pChild  = pCardIter_src->G2.m_pChild;

  G2.m_bActive = pCardIter_src->G2.m_bActive;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vSet(i8 iChild, Card* pChild)
{
  _precondition_(_MAGIC_, _FOOTER_);

  RASSERT(G2.m_bActive);

  G2.m_i_Child = iChild;
  G2.m_pChild = pChild;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool CardIter::bPopRecurseIter(void)
{
  _precondition_(_MAGIC_,_FOOTER_);

  if (! m_pRecurseStack || ! m_pRecurseStack->nItems())
    return false;

  CardIter* pCardIter_copy = m_pRecurseStack->pPeek(0);
  if (!pCardIter_copy->G2.m_bRecursed)
    return false;

  pCardIter_copy = m_pRecurseStack->pPop();


  vSet(pCardIter_copy);

  delete pCardIter_copy;

  return true;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vPushRecurseIter(void)
{
  _precondition_(_MAGIC_,_FOOTER_);

  RASSERT(G2.m_bRecursed);

  if (! m_pRecurseStack)
    m_pRecurseStack = new ListExo2_t<CardIter>;

  CardIter* pCardIter_copy = new CardIter((CardIter*)this);

  m_pRecurseStack->vPush(pCardIter_copy);
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
