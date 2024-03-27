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
/*                                                                           */
/*----------------------------------------------------------------------------*/
Card_Term* Card_RpnExpr::pHead_term(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  return _pCard_Term(pHead());
}

/*----------------------------------------------------------------------------*/
/*                                                                           */
/*----------------------------------------------------------------------------*/
Facet Card_RpnExpr::xPop_facet(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  RASSERT(nChildCards());

  Card_Term* p_term = pPop_term();
  Facet xFacet = *p_term->pSymbol()->aFacet(0);
  delete p_term;

  return xFacet;
}

/*----------------------------------------------------------------------------*/
/*                                                                           */
/*----------------------------------------------------------------------------*/
Card_Term* Card_RpnExpr::pPop_term(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  return _pCard_Term(pPopChild());
}

/*----------------------------------------------------------------------------*/
/*                                                                           */
/*----------------------------------------------------------------------------*/
Card_Term* Card_RpnExpr::pPop_term_If(eSF eFacet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (bHeadSame(eFacet))
    return _pCard_Term(pPopChild());

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Term* Card_RpnExpr::pPop_term_If(eSF eFacet, Stz* szFacet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (bHeadSame(eFacet,szFacet))
    return _pCard_Term(pPopChild());

  return nullptr;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
