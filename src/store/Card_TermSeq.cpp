/*============================================================================*/
/* Card_TermSeq.cpp -- Sequence of Terms                                      */
/*============================================================================*/

#include "Card_TermSeq.hpp"
#include "Card_Term.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

//__pCastFunction(STORE,Card_TermSeq,CN_CARD_TERMLIST)
__pCastFunction_Card(STORE,Card_TermSeq,CK_TERMSEQ)
__aNewFunction(Card_TermSeq,CN_CARD)
__vDeleteFunction(Card_TermSeq,CN_CARD)

__bIsCard(STORE,Card_TermSeq,CK_TERMSEQ)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_TermSeq::Card_TermSeq(void)
: Card_SUB(CK_TERMSEQ)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_TermSeq::~Card_TermSeq(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* evaluate top term                                                          */
/*----------------------------------------------------------------------------*/
eTE Card_TermSeq::eTopTermEval(void)
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
