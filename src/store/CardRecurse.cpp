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
: CardIter(CN_CARDRECURSE,pCard)
{
  _precondition_(_MAGIC_,_FOOTER_);

//  m_bDontDescend = false;

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
void CardRecurse::vDontDescend(void)
{
  _precondition_(_MAGIC_, _FOOTER_);

  m_bDontDescend = true;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardRecurse::pRecurseEach(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // descend into current child

  if (m_pChild && (! m_bDontDescend) ) {
    if (m_pChild->nChildCards()) {

      vPushIter();
      vChangeCard(m_pChild);
      m_pChild = pEachChild();

      return m_pChild;
    }
  }

  m_bDontDescend = false;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // visit next child

  if (bEachChild())
    return m_pChild;

  while (bPopIter()) {
    if (bEachChild())
      return m_pChild;
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardRecurse::pRecurseEach(eCK eCard)
{
  _precondition_( _MAGIC_, _FOOTER_);

  while (Card* p_card = pRecurseEach())
    if (p_card->bIsCard(eCard))
      return p_card;

  return nullptr;
}
/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool CardRecurse::bPopIter(void)
{
  if (! nItems())
    return false;

  CardIter* pCardIter_copy = pPop();

  vSet(pCardIter_copy);

  delete pCardIter_copy;

  return true;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardRecurse::vPushIter(void)
{
  CardIter* pCardIter_copy = new CardIter((CardIter*)this);

  vPush(pCardIter_copy);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
