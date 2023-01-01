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

  //MemLib::vZero(&G,sizeof(G),CN_CARDITER);

  G.m_pCard   = nullptr;
  G.m_i_Child = C_UNKNOWN;
  G.m_pChild  = nullptr;

  G.m_bDontDescend = false;

  G.m_bActive = false;

  //m_pRecurseStack = nullptr;


  _checkInitialised_(CN_CARDITER);

  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
CardIter::CardIter(eCN eClass)
    : Object(eClass)
{
  _precondition_(_MAGIC_,_FOOTER_);

  //MemLib::vZero(&G,sizeof(G),CN_CARDITER);

  G.m_pCard   = nullptr;
  G.m_i_Child = C_UNKNOWN;
  G.m_pChild  = nullptr;

  G.m_bDontDescend = false;

  G.m_bActive = false;

  //m_pRecurseStack = nullptr;


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

  //MemLib::vZero(&G,sizeof(G),CN_CARDITER);

  G.m_pCard   = pCard;
  G.m_i_Child = C_UNKNOWN;
  G.m_pChild  = nullptr;

  G.m_bActive = true;

  //m_pRecurseStack = nullptr;


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

  //MemLib::vZero(&G,sizeof(G),CN_CARDITER);

  //m_pRecurseStack = nullptr;

  vSet(pCardIter_src);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
CardIter::CardIter(eCN eClass, Card*  pCard)
    : Object(eClass)
{
  _precondition_(_MAGIC_,_FOOTER_);

  //MemLib::vZero(&G,sizeof(G),CN_CARDITER);

  G.m_pCard   = pCard;
  G.m_i_Child = C_UNKNOWN;
  G.m_pChild  = nullptr;

  G.m_bActive = true;

  //m_pRecurseStack = nullptr;


  _checkInitialised_(CN_CARDITER);

  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
CardIter::~CardIter(void) noexcept(false)
{
  _precondition_(_MAGIC_,_FOOTER_);

  //delete m_pRecurseStack;
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

  G.m_pCard = pCard;

  vRestart();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
i8 CardIter::i_Child(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  return G.m_i_Child;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardIter::pChild(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  return G.m_pChild;
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
bool CardIter::bEachChild(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(G.m_bActive);

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

  RASSERT(G.m_bActive);

  i8 i_Main;

  G.m_pChild = pEachChild(i_Main);

  return G.m_pChild;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardIter::pEachChild(i8& ri_Main)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(G.m_bActive);

  if (C_UNKNOWN == G.m_i_Child)
    G.m_i_Child = 0;
  else
    G.m_i_Child++;

  if (G.m_i_Child >= G.m_pCard->nChildCards()) {
    G.m_pChild = nullptr;
    ri_Main  = C_UNKNOWN;
    G.m_bActive = false;

    return nullptr;
  }

  G.m_pChild = G.m_pCard->pGetChild(G.m_i_Child);
  ri_Main = G.m_i_Child;

  return G.m_pChild;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardIter::pEachChild(eCK eCard)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(G.m_bActive);

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

  RASSERT(G.m_bActive);

  if (C_UNKNOWN == G.m_i_Child)
    G.m_i_Child = 0;
  else
    G.m_i_Child++;

  for (;;) {

    if (G.m_i_Child >= G.m_pCard->nChildCards()) {
      G.m_pChild  = nullptr;
      ri_Main   = C_UNKNOWN;
      G.m_bActive = false;
      return nullptr;
    }

    G.m_pChild = G.m_pCard->pGetChild(G.m_i_Child);

    eCK eCardMain = G.m_pChild->eCard();
    if (eCardMain == eCard) {
      ri_Main = G.m_i_Child;
      return G.m_pChild;
    }
    G.m_i_Child++;
  }

  NEVER_REACH
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardIter::pEachChild(i8& ri_Main, eCK& reCard)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(G.m_bActive);

  if (C_UNKNOWN == G.m_i_Child)
    G.m_i_Child = 0;
  else
    G.m_i_Child++;

  if (G.m_i_Child >= G.m_pCard->nChildCards()) {
    G.m_pChild  = nullptr;
    ri_Main   = C_UNKNOWN;
    reCard    = CK_NONE;
    G.m_bActive = false;
    return nullptr;
  }

  G.m_pChild = G.m_pCard->pGetChild(G.m_i_Child);

  reCard = G.m_pChild->eCard();
  ri_Main = G.m_i_Child;

  return G.m_pChild;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardIter::pEachChild_reverse(eCK eCard)
{
  _precondition_( _MAGIC_, _FOOTER_);

  if (C_UNKNOWN == G.m_i_Child)
    G.m_i_Child = G.m_pCard->nChildCards() - 1;
  else
    G.m_i_Child--;

  for (;;) {

    if (G.m_i_Child < 0) {
      G.m_pChild = nullptr;
      return nullptr;
    }

    G.m_pChild = G.m_pCard->pGetChild(G.m_i_Child);

    eCK eCardMain = G.m_pChild->eCard();
    if (eCardMain == eCard) {
      return G.m_pChild;
    }

    G.m_i_Child--;
  }

  NEVER_REACH
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vForward(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(G.m_bActive);

  Card* pChild = pNextChild();

  UNUSED(pChild);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardIter::pGetChild(i8 i_Child)
{
  _precondition_(_MAGIC_, _FOOTER_);

  Card* pChild = G.m_pCard->pGetChild(i_Child);

  return pChild;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool CardIter::bJustRestarted(void)
{
  _precondition_(_MAGIC_, _FOOTER_);

  if (C_UNKNOWN == G.m_i_Child &&
      nullptr == G.m_pChild &&
      true == G.m_bActive)
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool CardIter::bLess(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(G.m_bActive);

  if (G.m_i_Child - 1 >= 0)
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool CardIter::bMore(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(G.m_bActive);

  if (G.m_i_Child + 1 < G.m_pCard->nChildCards())
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardIter::pNextChild(void)
{
  _precondition_(_MAGIC_, _FOOTER_);

  RASSERT(G.m_bActive);

  i8 i_Next = G.m_i_Child + 1;

  if (i_Next >= G.m_pCard->nChildCards())
    return nullptr;

  Card* pNextMain = G.m_pCard->pGetChild(i_Next);

  return pNextMain;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardIter::pNextChildIf(eCK eCard)
{
  _precondition_(_MAGIC_, _FOOTER_);

  RASSERT(G.m_bActive);

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

  RASSERT(G.m_bActive);

  i8 i_Prev = G.m_i_Child - 1;

  if (i_Prev < 0)
    return nullptr;

  Card* pChild = G.m_pCard->pGetChild(i_Prev);

  return pChild;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardIter::pPrevChildIf(eCK eCard)
{
  _precondition_(_MAGIC_, _FOOTER_);

  RASSERT(G.m_bActive);

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

  if (G.m_pChild && (! G.m_bDontDescend) ) {
    if (G.m_pChild->nChildCards()) {

      G.m_bRecursed = true;
      vPushRecurseIter();
      vChangeCard(G.m_pChild);
      G.m_pChild = pEachChild();

      return G.m_pChild;
    }
  }

  G.m_bDontDescend = false;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // visit next child

  if (bEachChild())
    return G.m_pChild;

  while (bPopRecurseIter()) {
    if (bEachChild())
      return G.m_pChild;
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

  RASSERT(G.m_bActive);

  if (pChild == G.m_pChild)
    vRemoveCurr(G.m_pChild->eCard());
  else
    G.m_pCard->vRemoveChild(pChild);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vRemoveDeleteChild(Card*& rpChild)
{
  _precondition_(_MAGIC_, _FOOTER_);

  RASSERT(G.m_bActive);

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

  RASSERT(G.m_bActive);

  RASSERT(eCrossCheck == G.m_pChild->eCard());
  G.m_pCard->vRemoveChild(G.m_pChild);
  G.m_i_Child--;
  if (G.m_i_Child < G.m_pCard->nChildCards())
    G.m_pChild = G.m_pCard->pGetChild(G.m_i_Child);
  else
    G.m_pChild = nullptr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vRemoveDeleteCurr(eCK eCrossCheck)
{
  _precondition_(_MAGIC_, _FOOTER_);

  RASSERT(G.m_bActive);

  RASSERT(eCrossCheck == G.m_pChild->eCard());
  G.m_pCard->vRemoveChild(G.m_pChild);
  delete G.m_pChild;
  G.m_i_Child--;
  if (G.m_i_Child < G.m_pCard->nChildCards())
    G.m_pChild = G.m_pCard->pGetChild(G.m_i_Child);
  else
    G.m_pChild = nullptr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vRestart(void)
{
  _precondition_(_MAGIC_, _FOOTER_);

  G.m_i_Child = C_UNKNOWN;
  G.m_pChild  = nullptr;
  G.m_bActive = true;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vRewind(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(G.m_bActive);

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

  if (G.m_i_Child == 0) {
    G.m_i_Child = C_UNKNOWN;
    return nullptr;
  }

  G.m_i_Child--;

  if (G.m_i_Child < G.m_pCard->nChildCards())
    G.m_pChild =  G.m_pCard->pGetChild(G.m_i_Child);
  else
    G.m_pChild = nullptr;

  return G.m_pChild;
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

  RASSERT(G.m_bActive);

  G.m_i_Child = G.m_pCard->iFindChild(pChild);

  if (C_UNKNOWN == G.m_i_Child)
    G.m_pChild = nullptr;
  else
    G.m_pChild = pChild;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vSet(CardIter* pCardIter_src)
{
  _precondition_(_MAGIC_,_FOOTER_);

  G.m_pCard   = pCardIter_src->G.m_pCard;
  G.m_i_Child = pCardIter_src->G.m_i_Child;
  G.m_pChild  = pCardIter_src->G.m_pChild;

  G.m_bActive = pCardIter_src->G.m_bActive;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vSet(i8 iChild, Card* pChild)
{
  _precondition_(_MAGIC_, _FOOTER_);

  RASSERT(G.m_bActive);

  G.m_i_Child = iChild;
  G.m_pChild = pChild;
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

//  if (! m_pRecurseStack || ! m_pRecurseStack->nItems())
//    return false;

  CardIterMembers* pXsentinals2 = pPeek(0);
  if (! pXsentinals2 || ! pXsentinals2->m_bRecursed)
    return false;

//  pXsentinals2 = pPop();
//
//
//  vSet(pCardIter_copy);
//
//  delete pCardIter_copy;

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

//  if (! m_pRecurseStack)
//    m_pRecurseStack = new ListExo2_t<CardIter>;
//
//  CardIter* pCardIter_copy = new CardIter((CardIter*)this);
//
//  m_pRecurseStack->vPush(pCardIter_copy);
  vSave();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
