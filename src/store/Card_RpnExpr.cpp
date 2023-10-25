/*============================================================================*/
/* Card_RpnExpr.cpp -- RPN Expression                                         */
/*============================================================================*/

#include "Card_RpnExpr.hpp"
#include "Card_Term.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_RpnExpr, CK_RPNEXPR)
__aNewFunction(Card_RpnExpr, CN_CARD)
__vDeleteFunction(Card_RpnExpr, CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_RpnExpr::Card_RpnExpr(void)
: Card_symbol(CK_RPNEXPR, C_UNKNOWN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_RpnExpr::~Card_RpnExpr(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* test for in-list                                                           */
/*----------------------------------------------------------------------------*/
bool Card_RpnExpr::bIsInList(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  i8 nTerms = nItems();
  if (nTerms < 2)
    return false;

  Card_Term* p_term_2ndLast = _pCard_Term(pGetChild(nTerms-2));
  if (! p_term_2ndLast)
    return false;

  char* azTerm = p_term_2ndLast->szFacet(SF_NONE)->azChar();
  if ('#' == azTerm[0])
    return false;

  return true;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
