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
CardIter::CardIter(Card*  pCard)
: Object(CN_CARDITER)
{
  _precondition_(_MAGIC_,_FOOTER_);

  m_pCard      = pCard;
  m_i_Child    = C_UNKNOWN;
  m_pCurrChild = nullptr;

  _checkInitialised_(CN_CARDITER);
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
CardIter::CardIter(eCN eClass, Card*  pCard)
    : Object(eClass)
{
  _precondition_(_MAGIC_,_FOOTER_);

  m_pCard      = pCard;
  m_i_Child    = C_UNKNOWN;
  m_pCurrChild = nullptr;

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
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
i8 CardIter::i_Child(void)
{
  _precondition_( _MAGIC_, CN_BOARDITER, _FOOTER_);

  return m_i_Child;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardIter::pCurrChild(void)
{
  _precondition_( _MAGIC_, CN_BOARDITER, _FOOTER_);

  return m_pCurrChild;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardIter::pEachChild(void)
{
  _precondition_( _MAGIC_, CN_BOARDITER, _FOOTER_);

  i8 i_Main;

  m_pCurrChild = pEachChild(i_Main);

  return m_pCurrChild;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardIter::pEachChild(i8& ri_Main)
{
  _precondition_( _MAGIC_, CN_BOARDITER, _FOOTER_);

  if (C_UNKNOWN == m_i_Child)
    m_i_Child = 0;
  else
    m_i_Child++;

  if (m_i_Child >= m_pCard->nChildCards()) {
    m_pCurrChild = nullptr;
    ri_Main = C_UNKNOWN;
    return nullptr;
  }

  m_pCurrChild = m_pCard->pGetChild(m_i_Child);
  ri_Main = m_i_Child;

  return m_pCurrChild;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardIter::pEachChild(eCK eCard)
{
  _precondition_( _MAGIC_, CN_BOARDITER, _FOOTER_);

  i8 i_Main;

  Card* pMain = pEachChild(eCard, i_Main);

  return pMain;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardIter::pEachChild(eCK eCard, i8& ri_Main)
{
  _precondition_( _MAGIC_, CN_BOARDITER, _FOOTER_);

  if (C_UNKNOWN == m_i_Child)
    m_i_Child = 0;
  else
    m_i_Child++;

  for (;;) {

    if (m_i_Child >= m_pCard->nChildCards()) {
      m_pCurrChild = nullptr;
      ri_Main = C_UNKNOWN;
      return nullptr;
    }

    m_pCurrChild = m_pCard->pGetChild(m_i_Child);

    eCK eCardMain = m_pCurrChild->eCard();
    if (eCardMain == eCard) {
      ri_Main = m_i_Child;
      return m_pCurrChild;
    }
    m_i_Child++;
  }

  NEVER_REACH
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardIter::pEachChild(i8& ri_Main, eCK& reCard)
{
  _precondition_( _MAGIC_, CN_BOARDITER, _FOOTER_);

  if (C_UNKNOWN == m_i_Child)
    m_i_Child = 0;
  else
    m_i_Child++;

  if (m_i_Child >= m_pCard->nChildCards()) {
    m_pCurrChild = nullptr;
    ri_Main = C_UNKNOWN;
    reCard = CK_NONE;
    return nullptr;
  }

  m_pCurrChild = m_pCard->pGetChild(m_i_Child);

  reCard = m_pCurrChild->eCard();
  ri_Main = m_i_Child;

  return m_pCurrChild;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vForward(void)
{
  _precondition_( _MAGIC_, CN_BOARDITER, _FOOTER_);

  Card* pChild = pNextChild();

  UNUSED(pChild);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardIter::pGetChild(i8 i_Child)
{
  _precondition_(_MAGIC_, CN_BOARDITER, _FOOTER_);

  Card* pChild = m_pCard->pGetChild(i_Child);

  return pChild;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool CardIter::bLess(void)
{
  _precondition_( _MAGIC_, CN_BOARDITER, _FOOTER_);

  if (m_i_Child - 1 >= 0)
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool CardIter::bMore(void)
{
  _precondition_( _MAGIC_, CN_BOARDITER, _FOOTER_);

  if (m_i_Child + 1 < m_pCard->nChildCards())
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardIter::pNextChild(void)
{
  _precondition_(_MAGIC_, CN_BOARDITER, _FOOTER_);

  i8 i_Next = m_i_Child + 1;

  if (i_Next >= m_pCard->nChildCards())
    return nullptr;

  Card* pNextMain = m_pCard->pGetChild(i_Next);

  return pNextMain;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardIter::pNextChildIf(eCK eCard)
{
  _precondition_(_MAGIC_, CN_BOARDITER, _FOOTER_);

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
  _precondition_(_MAGIC_, CN_BOARDITER, _FOOTER_);

  i8 i_Prev = m_i_Child - 1;

  if (i_Prev < 0)
    return nullptr;

  Card* pChild = m_pCard->pGetChild(i_Prev);

  return pChild;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vRemoveChild(Card* pChild)
{
  _precondition_(_MAGIC_, CN_BOARDITER, _FOOTER_);

  if (pChild == m_pCurrChild)
    vRemoveCurr(m_pCurrChild->eCard());
  else
    m_pCard->vRemoveChild(pChild);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vRemoveDeleteChild(Card*& rpChild)
{
  _precondition_(_MAGIC_, CN_BOARDITER, _FOOTER_);

  vRemoveChild(rpChild);

  delete rpChild;

  rpChild = nullptr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vRemoveCurr(eCK eCrossCheck)
{
  _precondition_(_MAGIC_, CN_BOARDITER, _FOOTER_);

  RASSERT(eCrossCheck == m_pCurrChild->eCard());
  m_pCard->vRemoveChild(m_pCurrChild);
  m_i_Child--;
  if (m_i_Child < m_pCard->nChildCards())
    m_pCurrChild = m_pCard->pGetChild(m_i_Child);
  else
    m_pCurrChild = nullptr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vRemoveDeleteCurr(eCK eCrossCheck)
{
  _precondition_(_MAGIC_, CN_BOARDITER, _FOOTER_);

  RASSERT(eCrossCheck == m_pCurrChild->eCard());
  m_pCard->vRemoveChild(m_pCurrChild);
  delete m_pCurrChild;
  m_i_Child--;
  if (m_i_Child < m_pCard->nChildCards())
    m_pCurrChild = m_pCard->pGetChild(m_i_Child);
  else
    m_pCurrChild = nullptr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vRestart(void)
{
  _precondition_(_MAGIC_, CN_BOARDITER, _FOOTER_);

  m_i_Child    = C_UNKNOWN;
  m_pCurrChild = nullptr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vRewind(void)
{
  _precondition_( _MAGIC_, CN_BOARDITER, _FOOTER_);

  Card* pChild = pPrevChild();

  UNUSED(pChild);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vSet(Card* pChild)
{
  _precondition_(_MAGIC_, CN_BOARDITER, _FOOTER_);

  m_i_Child = m_pCard->iFindChild(pChild);

  if (C_UNKNOWN == m_i_Child)
    m_pCurrChild = nullptr;
  else
    m_pCurrChild = pChild;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vSet(i8 iChild, Card* pChild)
{
  _precondition_(_MAGIC_, CN_BOARDITER, _FOOTER_);

  m_i_Child = iChild;
  m_pCurrChild = pChild;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
