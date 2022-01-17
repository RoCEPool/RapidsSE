/*============================================================================*/
/* Card_TermList.cpp -- List of Atom Cards                                    */
/*============================================================================*/

#include "Card_TermList.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

//__pCastFunction(STORE,Card_TermList,CN_CARD_TERMLIST)
__pCastFunction_Card(STORE,Card_TermList,CK_TERMLIST)
__aNewFunction(Card_TermList,CN_CARD)
__vDeleteFunction(Card_TermList,CN_CARD)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_TermList::Card_TermList(eCK eKind)
: Card(CK_TERMLIST)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_TermList::~Card_TermList(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  vDeleteAll();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
