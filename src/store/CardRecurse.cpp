/*============================================================================*/
/* CardRecurse.cpp --                                                         */
/*============================================================================*/

#include "CardRecurse.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,CardRecurse,CN_CARDRECURSE)
__aNewFunction(CardRecurse,CN_CARDRECURSE)
__vDeleteFunction(CardRecurse,CN_CARDRECURSE)
__aFooterFunction(CardRecurse)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
CardRecurse::CardRecurse(Card*  pCard)
: Object(CN_CARDRECURSE)
{
  _precondition_(_MAGIC_,_FOOTER_);

  m_pCard   = pCard;
  m_i_Child = C_UNKNOWN;
  m_pChild  = nullptr;

  m_bActive = true;

  _checkInitialised_(CN_CARDRECURSE);
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
CardRecurse::CardRecurse(CardRecurse* pCardRecurse_src)
    : Object(CN_CARDRECURSE)
{
  _precondition_(_MAGIC_,_FOOTER_);

  m_pCard   = pCardRecurse_src->m_pCard;
  m_i_Child = pCardRecurse_src->m_i_Child;
  m_pChild  = pCardRecurse_src->m_pChild;

  m_bActive = pCardRecurse_src->m_bActive;

  _checkInitialised_(CN_CARDRECURSE);

  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
CardRecurse::CardRecurse(eCN eClass, Card*  pCard)
    : Object(eClass)
{
  _precondition_(_MAGIC_,_FOOTER_);

  m_pCard   = pCard;
  m_i_Child = C_UNKNOWN;
  m_pChild  = nullptr;

  m_bActive = true;

  _checkInitialised_(CN_CARDRECURSE);

  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
CardRecurse::~CardRecurse(void) noexcept(false)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardRecurse::vChangeCard(Card* pCard)
{
  _precondition_(_MAGIC_, _FOOTER_);

  m_pCard = pCard;

  vRestart();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
i8 CardRecurse::i_Child(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  return m_i_Child;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardRecurse::pChild(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  return m_pChild;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool CardRecurse::bEachChild(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(m_bActive);

  if (pEachChild())
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardRecurse::pEachChild(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(m_bActive);

  i8 i_Main;

  m_pChild = pEachChild(i_Main);

  return m_pChild;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardRecurse::pEachChild(i8& ri_Main)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(m_bActive);

  if (C_UNKNOWN == m_i_Child)
    m_i_Child = 0;
  else
    m_i_Child++;

  if (m_i_Child >= m_pCard->nChildCards()) {
    m_pChild = nullptr;
    ri_Main  = C_UNKNOWN;
    m_bActive = false;

    return nullptr;
  }

  m_pChild = m_pCard->pGetChild(m_i_Child);
  ri_Main = m_i_Child;

  return m_pChild;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardRecurse::pEachChild(eCK eCard)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(m_bActive);

  i8 i_Main;

  Card* pMain = pEachChild(eCard, i_Main);

  return pMain;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardRecurse::pEachChild(eCK eCard, i8& ri_Main)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(m_bActive);

  if (C_UNKNOWN == m_i_Child)
    m_i_Child = 0;
  else
    m_i_Child++;

  for (;;) {

    if (m_i_Child >= m_pCard->nChildCards()) {
      m_pChild  = nullptr;
      ri_Main   = C_UNKNOWN;
      m_bActive = false;
      return nullptr;
    }

    m_pChild = m_pCard->pGetChild(m_i_Child);

    eCK eCardMain = m_pChild->eCard();
    if (eCardMain == eCard) {
      ri_Main = m_i_Child;
      return m_pChild;
    }
    m_i_Child++;
  }

  NEVER_REACH
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardRecurse::pEachChild(i8& ri_Main, eCK& reCard)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(m_bActive);

  if (C_UNKNOWN == m_i_Child)
    m_i_Child = 0;
  else
    m_i_Child++;

  if (m_i_Child >= m_pCard->nChildCards()) {
    m_pChild  = nullptr;
    ri_Main   = C_UNKNOWN;
    reCard    = CK_NONE;
    m_bActive = false;
    return nullptr;
  }

  m_pChild = m_pCard->pGetChild(m_i_Child);

  reCard = m_pChild->eCard();
  ri_Main = m_i_Child;

  return m_pChild;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardRecurse::vForward(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(m_bActive);

  Card* pChild = pNextChild();

  UNUSED(pChild);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardRecurse::pGetChild(i8 i_Child)
{
  _precondition_(_MAGIC_, _FOOTER_);

  Card* pChild = m_pCard->pGetChild(i_Child);

  return pChild;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool CardRecurse::bJustRestarted(void)
{
  _precondition_(_MAGIC_, _FOOTER_);

  if (C_UNKNOWN == m_i_Child &&
      nullptr   == m_pChild  &&
      true      == m_bActive)
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool CardRecurse::bLess(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  if (m_i_Child - 1 >= 0)
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool CardRecurse::bMore(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  if (m_i_Child + 1 < m_pCard->nChildCards())
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardRecurse::pNextChild(void)
{
  _precondition_(_MAGIC_, _FOOTER_);

  RASSERT(m_bActive);

  i8 i_Next = m_i_Child + 1;

  if (i_Next >= m_pCard->nChildCards())
    return nullptr;

  Card* pNextMain = m_pCard->pGetChild(i_Next);

  return pNextMain;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardRecurse::pNextChildIf(eCK eCard)
{
  _precondition_(_MAGIC_, _FOOTER_);

  RASSERT(m_bActive);

  Card* pNextMain = this->pNextChild();
  if (! pNextMain || eCard != pNextMain->eCard())
    return nullptr;

  return pNextMain;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardRecurse::pPrevChild(void)
{
  _precondition_(_MAGIC_, _FOOTER_);

  RASSERT(m_bActive);

  i8 i_Prev = m_i_Child - 1;

  if (i_Prev < 0)
    return nullptr;

  Card* pChild = m_pCard->pGetChild(i_Prev);

  return pChild;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardRecurse::vRemoveChild(Card* pChild)
{
  _precondition_(_MAGIC_, _FOOTER_);

  RASSERT(m_bActive);

  if (pChild == m_pChild)
    vRemoveCurr(m_pChild->eCard());
  else
    m_pCard->vRemoveChild(pChild);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardRecurse::vRemoveDeleteChild(Card*& rpChild)
{
  _precondition_(_MAGIC_, _FOOTER_);

  RASSERT(m_bActive);

  vRemoveChild(rpChild);

  delete rpChild;

  rpChild = nullptr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardRecurse::vRemoveCurr(eCK eCrossCheck)
{
  _precondition_(_MAGIC_, _FOOTER_);

  RASSERT(m_bActive);

  RASSERT(eCrossCheck == m_pChild->eCard());
  m_pCard->vRemoveChild(m_pChild);
  m_i_Child--;
  if (m_i_Child < m_pCard->nChildCards())
    m_pChild = m_pCard->pGetChild(m_i_Child);
  else
    m_pChild = nullptr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardRecurse::vRemoveDeleteCurr(eCK eCrossCheck)
{
  _precondition_(_MAGIC_, _FOOTER_);

  RASSERT(m_bActive);

  RASSERT(eCrossCheck == m_pChild->eCard());
  m_pCard->vRemoveChild(m_pChild);
  delete m_pChild;
  m_i_Child--;
  if (m_i_Child < m_pCard->nChildCards())
    m_pChild = m_pCard->pGetChild(m_i_Child);
  else
    m_pChild = nullptr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardRecurse::vRestart(void)
{
  _precondition_(_MAGIC_, _FOOTER_);

  m_i_Child = C_UNKNOWN;
  m_pChild  = nullptr;
  m_bActive = true;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardRecurse::vRewind(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(m_bActive);

  Card* pChild = pPrevChild();

  UNUSED(pChild);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardRecurse::vSet(Card* pChild)
{
  _precondition_(_MAGIC_, _FOOTER_);

  RASSERT(m_bActive);

  m_i_Child = m_pCard->iFindChild(pChild);

  if (C_UNKNOWN == m_i_Child)
    m_pChild = nullptr;
  else
    m_pChild = pChild;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardRecurse::vSet(i8 iChild, Card* pChild)
{
  _precondition_(_MAGIC_, _FOOTER_);

  RASSERT(m_bActive);

  m_i_Child = iChild;
  m_pChild = pChild;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
