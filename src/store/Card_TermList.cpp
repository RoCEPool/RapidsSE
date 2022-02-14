/*============================================================================*/
/* Card_TermList.cpp -- List of Terms                                         */
/*============================================================================*/

#include "Card_TermList.hpp"
#include "Card_Term.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

//__pCastFunction(STORE,Card_TermList,CN_CARD_TERMLIST)
__pCastFunction_Card(STORE,Card_TermList,CK_TERMLIST)
__aNewFunction(Card_TermList,CN_CARD)
__vDeleteFunction(Card_TermList,CN_CARD)

__bIsCard(STORE,Card_TermList,CK_TERMLIST)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_TermList::Card_TermList(void)
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
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* evaluate top term                                                          */
/*----------------------------------------------------------------------------*/
aTE Card_TermList::eTopTermEval(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
  
  Card_Term* pCard_Term = _pCard_Term(pHead());
  
  if (! pCard_Term)
    return TE_NONE;
  
  aTE eTermEval = pCard_Term->eTermEval();
  
  return eTermEval;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
