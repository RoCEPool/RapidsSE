/*============================================================================*/
/* Card_Atom.cpp -- Atom Card                                                 */
/*============================================================================*/

#include "Card_Atom.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_Atom,CK_ATOM)
__aNewFunction(Card_Atom,CN_CARD_ATOM)
__vDeleteFunction(Card_Atom,CN_CARD_ATOM)

__AssertFunction(STORE,Card_Atom,CN_CARD_ATOM)
__bIsClass(STORE,Card_Atom,CN_CARD_ATOM)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Atom::Card_Atom(eCK eKind, Stz* szAtom)
: Card(CK_ATOM,CN_CARD_ATOM)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_ATOM,_FOOTER_);
        
  vAtom(szAtom);
  
  _checkInitialised_(CN_CARD_ATOM);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD_ATOM,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Atom::~Card_Atom(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_ATOM,_FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
