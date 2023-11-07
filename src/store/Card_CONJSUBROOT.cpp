/*============================================================================*/
/* Card_CONJSUBROOT.cpp -- Abstract Conjunction Sub-Root                      */
/*============================================================================*/

#include "Card_CONJSUBROOT.hpp"
#include "Card_Term.hpp"
#include "Card_OrConj.hpp"
#include "Card_AndConj.hpp"
#include "Card_RlnConj.hpp"
#include "CardIter.hpp"
#include "Card_Table.hpp"
#include "Card_Deck.hpp"
#include "Scope.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_CONJSUBROOT::Card_CONJSUBROOT(eCK eCard, i8 iFixedDepth)
: Card_symbol(eCard, iFixedDepth)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_CONJSUBROOT::~Card_CONJSUBROOT(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* get child andConj                                                          */
/*----------------------------------------------------------------------------*/
Card_AndConj* Card_CONJSUBROOT::pGet_andConj(i8 i_Child)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_AndConj* p_andConj = _pCard_AndConj(pGetChild(i_Child));

  return p_andConj;
}

/*----------------------------------------------------------------------------*/
/* get child notConj                                                          */
/*----------------------------------------------------------------------------*/
Card_NotConj* Card_CONJSUBROOT::pGet_notConj(i8 i_Child)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_NotConj* p_notConj = _pCard_NotConj(pGetChild(i_Child));

  return p_notConj;
}

/*----------------------------------------------------------------------------*/
/* get child orConj                                                           */
/*----------------------------------------------------------------------------*/
Card_OrConj* Card_CONJSUBROOT::pGet_orConj(i8 i_Child)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_OrConj* p_orConj = _pCard_OrConj(pGetChild(i_Child));

  return p_orConj;
}

/*----------------------------------------------------------------------------*/
/* get child rlnConj                                                          */
/*----------------------------------------------------------------------------*/
Card_RlnConj* Card_CONJSUBROOT::pGet_rlnConj(i8 i_Child)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_RlnConj* p_rlnConj = _pCard_RlnConj(pGetChild(i_Child));

  return p_rlnConj;
}

/*----------------------------------------------------------------------------*/
/* pop child andConj                                                          */
/*----------------------------------------------------------------------------*/
Card_AndConj* Card_CONJSUBROOT::pPop_andConj(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_AndConj* p_andConj = _pCard_AndConj(pPopChild());

  return p_andConj;
}

/*----------------------------------------------------------------------------*/
/* pop child notConj                                                          */
/*----------------------------------------------------------------------------*/
Card_NotConj* Card_CONJSUBROOT::pPop_notConj(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_NotConj* p_notConj = _pCard_NotConj(pPopChild());

  return p_notConj;
}

/*----------------------------------------------------------------------------*/
/* pop child orConj                                                           */
/*----------------------------------------------------------------------------*/
Card_OrConj* Card_CONJSUBROOT::pPop_orConj (void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_OrConj* p_orConj = _pCard_OrConj(pPopChild());

  return p_orConj;
}

/*----------------------------------------------------------------------------*/
/* pop child rlnConj                                                          */
/*----------------------------------------------------------------------------*/
Card_RlnConj* Card_CONJSUBROOT::pPop_rlnConj(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_RlnConj* p_rlnConj = _pCard_RlnConj(pPopChild());

  return p_rlnConj;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
