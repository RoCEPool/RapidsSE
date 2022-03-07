/*============================================================================*/
/* Card_RpnTerms.cpp -- RPN Sequence of Terms                                 */
/*============================================================================*/

#include "Card_RpnTerms.hpp"
#include "Card_Term.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

//__pCastFunction(STORE,Card_RpnTerms,CN_CARD_TERMLIST)
__pCastFunction_Card(STORE,Card_RpnTerms,CK_RPNTERMS)
__aNewFunction(Card_RpnTerms,CN_CARD)
__vDeleteFunction(Card_RpnTerms,CN_CARD)

__bIsCard(STORE,Card_RpnTerms,CK_RPNTERMS)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_RpnTerms::Card_RpnTerms(void)
: Card_SUB(CK_RPNTERMS)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_RpnTerms::~Card_RpnTerms(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* evaluate top term                                                          */
/*----------------------------------------------------------------------------*/
eTE Card_RpnTerms::eTopTermEval(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  Card_Term* pCard_Term = _pCard_Term(pHead());
  
  if (! pCard_Term)
    return TE_NONE;
  
  eTE eTermEval = pCard_Term->eTermEval();
  
  return eTermEval;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
