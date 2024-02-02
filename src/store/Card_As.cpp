/*============================================================================*/
/* Card_As.cpp -- As                                                          */
/*============================================================================*/

#include "Card_As.hpp"

#include "CardIter.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_As,CK_AS)
__aNewFunction(Card_As,CN_CARD)
__vDeleteFunction(Card_As,CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_As::Card_As(void)
    : Card_symbol(CK_AS, C_UNKNOWN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_As::~Card_As(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_As::vItemise_colNm(CardIter* pCI)
{
  _static_

  UNUSED(pCI);

  auto bfItemise_colNm = [](Card* pCard)
  {
    //pCI->vAncestorDump(pCard);

//    if (! pCard->bIsCard(CK_AS))
//      return C_CONTINUE_RECURSION;

    //Card_As* p_as = _pCard_As(pCard);

//    Card_Expr* p_expr = _pCard_Expr(pCI->pAncestorFind(CK_EXPR));
//    if (! p_expr)
//      return C_CONTINUE_RECURSION;

    if (! pCard->bIsCard(CK_EXPR))
      return C_CONTINUE_RECURSION;

    Card_Expr* p_expr = _pCard_Expr(pCard);
    if (! p_expr->pSymbol()->bHasFacet(SF_NMD_COL))
      return C_CONTINUE_RECURSION;

    i8 iItemise = p_expr->iItemise();
    Symbol oSymbol_itemise;
    oSymbol_itemise.vFacet(SF_ITM_COL,iItemise,p_expr->pSymbol());

    p_expr->pSymbol()->vAppendFacet(oSymbol_itemise.aFacet(SF_ITM_COL));

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pCI->bApplyRecursive(pCI->G.pDeck,CK_NONE,bfItemise_colNm);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_As* Card_As::pClone(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  return _pCard_As(pDupCardRecursive());
}

/*----------------------------------------------------------------------------*/
/* validate                                                                   */
/*----------------------------------------------------------------------------*/
void Card_As::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  UNUSED(pCI);
  UNUSED(iDepth);
  UNUSED(pParent);

  NEVER_REACH
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
