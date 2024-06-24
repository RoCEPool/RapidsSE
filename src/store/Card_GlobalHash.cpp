/*============================================================================*/
/* Card_GlobalHash.cpp -- Global Hash                                         */
/*============================================================================*/

#include "Card_GlobalHash.hpp"

#include "CardIter.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_GlobalHash,CK_o_GLOBALHASH)
__aNewFunction(Card_GlobalHash,CN_CARD)
__vDeleteFunction(Card_GlobalHash,CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_GlobalHash::Card_GlobalHash(void)
: Card_symbol(CK_o_GLOBALHASH, C_UNKNOWN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_GlobalHash::~Card_GlobalHash(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Expr* Card_GlobalHash::pNew_expr_rrf(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_Expr* p_expr_rrf = new Card_Expr;
  p_expr_rrf->pSymbol()->vApplyFacet(SF_NMD_COL, "ROWREF"_Stz);
  p_expr_rrf->pSymbol()->vApplyFacet(SF_EXP_COL, (i8)99);
  Cast xCast_new;
  xCast_new.m_xTypeId = TypeId::xIdOf(CT_INTEGER);
  xCast_new.m_nTypeLength = 19;
  xCast_new.m_nTypeScale  = 0;
  p_expr_rrf->pSymbol()->vApplyCast(xCast_new);

  Card_Term* p_term_rrf = (new Card_Term) ->SYM(SF_INT_LIT,C_UNKNOWN);

  p_expr_rrf->vAppendChild(p_term_rrf);

  return p_expr_rrf;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_GlobalHash::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  UNUSED(pParent);
  Card::vValidate(pCI,iDepth,pParent);
  pCI->vValidateChildren(this,iDepth+1);
  vValidationPassed();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
