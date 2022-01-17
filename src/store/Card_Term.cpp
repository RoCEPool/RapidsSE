/*============================================================================*/
/* Card_Term.cpp -- Term Card                                                 */
/*============================================================================*/

#include "Card_Term.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_Term,CK_TERM)
__aNewFunction(Card_Term,CN_CARD_TERM)
__vDeleteFunction(Card_Term,CN_CARD_TERM)

__AssertFunction(STORE,Card_Term,CN_CARD_TERM)
__bIsClass(STORE,Card_Term,CN_CARD_TERM)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Term::Card_Term(eCK eKind, Stz* szTerm)
: Card(CK_TERM,CN_CARD_TERM)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
      
  vTerm(szTerm);
  m_bPrintNm = false;
  
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Term::~Card_Term(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
