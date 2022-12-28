/*============================================================================*/
/* BoardIter.cpp --                                                           */
/*============================================================================*/

#include "Board.hpp"

#include "BoardIter.hpp"
#include "Card_Deck.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,BoardIter,CN_BOARDITER)
__aNewFunction(BoardIter,CN_BOARDITER)
__vDeleteFunction(BoardIter,CN_BOARDITER)
__aFooterFunction(BoardIter)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
BoardIter::BoardIter(void)
    : CardIter(CN_BOARDITER, nullptr)
{
  _precondition_(_MAGIC_,CN_BOARDITER,_FOOTER_);

  m_pCard  = nullptr;

  _checkInitialised_(CN_BOARDITER);

  _postcondition_(_MAGIC_,CN_BOARDITER,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
BoardIter::BoardIter(Card* pCard)
: CardIter(CN_BOARDITER, pCard)
{
  _precondition_(_MAGIC_,CN_BOARDITER,_FOOTER_);

  _checkInitialised_(CN_BOARDITER);
  
  _postcondition_(_MAGIC_,CN_BOARDITER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
BoardIter::~BoardIter(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_BOARDITER,_FOOTER_);
  
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* BoardIter::pEachMain(void)
{
  _precondition_( _MAGIC_, CN_BOARDITER, _FOOTER_);

  i8 i_Main;

  m_pChild = pEachChild(i_Main);

  return m_pChild;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
//Card* BoardIter::pEachMain(i8& ri_Main)
//{
//  _precondition_( _MAGIC_, CN_BOARDITER, _FOOTER_);
//
//  if (C_UNKNOWN == m_i_Child)
//    m_i_Child = 0;
//  else
//    m_i_Child++;
//
//  if (m_i_Child >= m_pCard->nChildCards()) {
//    m_pChild = nullptr;
//    ri_Main = C_UNKNOWN;
//    return nullptr;
//  }
//
//  m_pChild = m_pCard->pGetChild(m_i_Child);
//  ri_Main = m_i_Child;
//
//  return m_pChild;
//}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* BoardIter::pEachMain(eCK eCard)
{
  _precondition_( _MAGIC_, CN_BOARDITER, _FOOTER_);

  if (C_UNKNOWN == m_i_Child)
    m_i_Child = 0;
  else
    m_i_Child++;

  for (;;) {

    if (m_i_Child >= m_pCard->nChildCards()) {
      m_pChild = nullptr;
      return nullptr;
    }

    m_pChild =  m_pCard->pGetChild(m_i_Child);

    eCK eCardMain = m_pChild->eCard();
    if (eCardMain == eCard) {
      return m_pChild;
    }
    m_i_Child++;
  }

  NEVER_REACH
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* BoardIter::pEachMain_reverse(eCK eCard)
{
  _precondition_( _MAGIC_, CN_BOARDITER, _FOOTER_);

  if (C_UNKNOWN == m_i_Child)
    m_i_Child = m_pCard->nChildCards() - 1;
  else
    m_i_Child--;

  for (;;) {

    if (m_i_Child < 0) {
      m_pChild = nullptr;
      return nullptr;
    }

    m_pChild = m_pCard->pGetChild(m_i_Child);

    eCK eCardMain = m_pChild->eCard();
    if (eCardMain == eCard) {
      return m_pChild;
    }

    m_i_Child--;
  }

  NEVER_REACH
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
//Card* BoardIter::pNextMain(void)
//{
//  _precondition_(_MAGIC_, CN_BOARDITER, _FOOTER_);
//
//  i8 i_Next = m_i_Child + 1;
//
//  if (i_Next >= m_pCard->nChildCards())
//    return nullptr;
//
//  Card* pNextMain =  m_pCard->pGetChild(i_Next);
//
//  return pNextMain;
//}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
//Card* BoardIter::pNextMainIf(eCK eCard)
//{
//  _precondition_(_MAGIC_, CN_BOARDITER, _FOOTER_);
//
//  Card* pNextMain = this->pNextMain();
//  if (! pNextMain || eCard != pNextMain->eCard())
//    return nullptr;
//
//  return pNextMain;
//}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
//Card* BoardIter::pPrevMain(void)
//{
//  _precondition_(_MAGIC_, CN_BOARDITER, _FOOTER_);
//
//  i8 i_Prev = m_i_Child - 1;
//
//  if (i_Prev < 0)
//    return nullptr;
//
//  Card* pPrevMain =  m_pCard->pGetChild(i_Prev);
//
//  return pPrevMain;
//}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* BoardIter::pPrevMainIf(eCK eCard)
{
  _precondition_(_MAGIC_, CN_BOARDITER, _FOOTER_);

  Card* pPrevMain = this->pPrevChild();
  if (! pPrevMain || eCard != pPrevMain->eCard())
    return nullptr;

  return pPrevMain;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool BoardIter::bMore(void)
{
  _precondition_( _MAGIC_, CN_BOARDITER, _FOOTER_);

  if (m_i_Child + 1 < m_pCard->nChildCards())
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void BoardIter::vForward(void)
{
  _precondition_( _MAGIC_, CN_BOARDITER, _FOOTER_);

  m_i_Child++;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void BoardIter::vRemoveCard(Card* pCard)
{
  _precondition_(_MAGIC_, CN_BOARDITER, _FOOTER_);

  if (pCard == m_pChild) {
    vRemoveCurr(m_pChild->eCard());
    return;
  }

  m_pCard->vRemoveChild(pCard);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void BoardIter::vRemoveCurr(eCK eCrossCheck)
{
  _precondition_(_MAGIC_, CN_BOARDITER, _FOOTER_);

  RASSERT(eCrossCheck == m_pChild->eCard());
  m_pCard->vRemoveChild(m_pChild);
  m_i_Child--;
  if (m_i_Child < m_pCard->nChildCards())
    m_pChild =  m_pCard->pGetChild(m_i_Child);
  else
    m_pChild = nullptr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void BoardIter::vRemoveDeleteCurr(eCK eCrossCheck)
{
  _precondition_(_MAGIC_, CN_BOARDITER, _FOOTER_);

  RASSERT(eCrossCheck == m_pChild->eCard());
  m_pCard->vRemoveChild(m_pChild);
  delete m_pChild;
  m_i_Child--;
  if (m_i_Child < m_pCard->nChildCards())
    m_pChild =  m_pCard->pGetChild(m_i_Child);
  else
    m_pChild = nullptr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* BoardIter::pRewindMain(void)
{
  _precondition_( _MAGIC_, CN_BOARDITER, _FOOTER_);

  if (m_i_Child == 0) {
    m_i_Child = C_UNKNOWN;
    return nullptr;
  }

  m_i_Child--;

  if (m_i_Child < m_pCard->nChildCards())
    m_pChild =  m_pCard->pGetChild(m_i_Child);
  else
    m_pChild = nullptr;

  return m_pChild;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void BoardIter::vSet(Card* pCurrMain)
{
  _precondition_(_MAGIC_, CN_BOARDITER, _FOOTER_);

  m_i_Child = m_pCard->iFindChild(pCurrMain);

  if (C_UNKNOWN == m_i_Child)
    m_pChild = nullptr;
  else
    m_pChild = pCurrMain;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void BoardIter::vSet(i8 i_Main, Card* pCurrMain)
{
  _precondition_(_MAGIC_, CN_BOARDITER, _FOOTER_);

  m_i_Child = i_Main;
  m_pChild = pCurrMain;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
