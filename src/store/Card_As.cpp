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

  auto bfItemise_colNm = [pCI](Card* pCard)
  {
    UNUSED(pCI);

    //pCI->vAncestorDump(pCard);

    if (! pCard->bIsCard(CK_AS))
      return C_CONTINUE_RECURSION;

    Card_As* p_as = _pCard_As(pCard);

    if (! p_as->pSymbol_AS()->bIs_Nmd_Col())
      return C_CONTINUE_RECURSION;

    Card_Expr* p_expr = _pCard_Expr(pCI->pAncestorFind(CK_EXPR));
    if (! p_expr)
      return C_CONTINUE_RECURSION;

    //p_expr->vDump(__func__ ,"ITEMISE\n"_Stz);
    i8 iItemise = p_expr->iItemise();
    p_as->pSymbol_AS()->vFacet(SF_ITM_COL,iItemise);
    //p_expr->vDump(__func__ ,"ITEMISE\n"_Stz);

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
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_As::bIs_SQCRLKEY(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (pSymbol_AS()->bSame(C_SQCRLKEY))
    return true;

  return false;
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

  vValidationPassed();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
