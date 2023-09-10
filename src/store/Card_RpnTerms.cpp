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


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_RpnTerms::Card_RpnTerms(void)
: Card_symbol(CK_RPNTERMS, C_UNKNOWN)
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
/* test for in-list                                                           */
/*----------------------------------------------------------------------------*/
bool Card_RpnTerms::bIsInList(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  i8 nTerms = nItems();
  if (nTerms < 2)
    return false;

  Card_Term* p_term_2ndLast = _pCard_Term(pGetChild(nTerms-2));
  if (! p_term_2ndLast)
    return false;

  char* azTerm = p_term_2ndLast->szSymbol(SF_NONE)->azChar();
  if ('#' == azTerm[0])
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/* pop top term                                                               */
/*----------------------------------------------------------------------------*/
Card_Term* Card_RpnTerms::pPop_term(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  return _pCard_Term(pPopChild());
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
