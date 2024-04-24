/*============================================================================*/
/* Card_Key.cpp -- Key                                                        */
/*============================================================================*/

#include "Card_Key.hpp"

#include "Card_Expr.hpp"
#include "Card_HshCol.hpp"
#include "Card_Term.hpp"
#include "CardIter.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_Key, CK_s_KEY)
__aNewFunction(Card_Key,CN_CARD)
__vDeleteFunction(Card_Key,CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Key::Card_Key(void)
: Card_EXPRLIST(CK_s_KEY, 2)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Key::Card_Key(eCK eCard, i8 iMaxDepth)
    : Card_EXPRLIST(eCard, iMaxDepth)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Key::~Card_Key(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void Card_Key::vAssemble_injCrlKey(CardIter* pCI)
{
  _static_

/**Z_KEY_DIRECTION assemble*/

  pCI->E.p_key = new Card_Key;
  pCI->E.p_key->pSymbol()->Y_vSet(P_InjCrlKey);

  eAL e_Level = (eAL) pCI->E.p_level->pSymbol()->Y_iGet(P_Level);
  if (AL_PARTITION == e_Level) {
    pCI->E.p_expr = Card_Expr::pCraft_expr_Of_term(pCI->Flat.m_p_expr_crlKey->pGet_term0());
  }
  else {
    Symbol xSymbol(SF_EXP_COL, "$1"_Stz, pCI->Flat.m_p_expr_crlKey->xCast());
    pCI->E.p_expr = Card_Expr::pCraft_expr_Of_symbol(&xSymbol);
  }
  pCI->E.p_expr->pSymbol()->Y_vSet(P_InjCrlKey);
  pCI->E.p_key->vAppendChild(pCI->E.p_expr);

  pCI->E.p_key->vApplyCast_x(pCI->E.p_expr->xCast());

  pCI->E.p_key->vRenumberAll_expr(SF_KEY_COL);

}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add a key expr to this key based on given projection expr                  */
/*----------------------------------------------------------------------------*/
void Card_Key::vAdd_key_expr_BasedOn_expr(CardIter *pCI, Card_Expr* p_expr, bool bAsc, bool bAppend)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

/**Z_KEY_DIRECTION add key*/

  pCI->E.p_expr = p_expr->pClone();
  pCI->E.p_expr->pSymbol()->vResetFacets();
  pCI->E.p_expr->pSymbol()->vApplyFacet(SF_KEY_COL,C_UNKNOWN);
  pCI->E.p_expr->pSymbol()->vApplyFacet(SF_DIRECTION,bAsc?C_ASC:C_DESC);

  if (0 == nChildCards())
    vApplyCast_x(p_expr->xCast());

  if (bAppend)
    vAppendChild(pCI->E.p_expr);
  else
    vPushChild(pCI->E.p_expr);

  vRenumberAll_expr(SF_KEY_COL);
}

/*----------------------------------------------------------------------------*/
/* return direction string of key element                                     */
/*----------------------------------------------------------------------------*/
Stz* Card_Key::szDirection(i8 i_Expr)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_Expr* p_expr = pGet_expr(i_Expr);

  return p_expr->szFacet(SF_DIRECTION);
}

/*----------------------------------------------------------------------------*/
/* map the columns of this key                                                */
/*----------------------------------------------------------------------------*/
void Card_Key::vMapColumns(CardIter *pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  i8 i_Cell = 0;

//Z_KEY_DIRECTION
  for (pCI->vBeginTarget(this,CK_EXPR); pCI->bEachTarget(); /**/) {
    pCI->vAppendNewChild(pCI->E.p_keyCells, CK_MAP, pCI->E.p_expr->pSymbol());
    pCI->E.p_map->pSymbol()->Y_vSet(P_Cell, ++i_Cell);
  }

}

/*----------------------------------------------------------------------------*/
/* convert key to indexDef                                                    */
/*----------------------------------------------------------------------------*/
IndexDef* Card_Key::pTo_IndexDef(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  bool bHasDescending = false;

  IndexDef* pIndexDef = new IndexDef;

//Z_KEY_DIRECTION
  for (pCI->vBeginTarget(this,CK_EXPR); pCI->bEachTarget(); /**/) {
    Card_Term* p_term = pCI->E.p_expr->pGet_term(0);
    if (! p_term)
      continue;

    Stz* szColNm  = p_term->szFacet(SF_EXP_COL);
    TypeId xTypeId = p_term->xTypeId();

    if (! szColNm || CT_NONE == xTypeId.eCTVal())
      continue;

    bool bAscending = true;
    if (pGet_expr(pCI->E.xTarget.i_Child())->pSymbol()->bHasFacet(C_DESC)) {
      bAscending = false;
      bHasDescending = true;
    }
    pIndexDef->vAddColumn(szColNm,xTypeId,bAscending);
  }


  if (bHasDescending)
    pIndexDef->vHasDescending(true);

  if (pIndexDef->nColumns())
    return pIndexDef;

  delete pIndexDef;
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* validate                                                                   */
/*----------------------------------------------------------------------------*/
void Card_Key::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  (void)Card::eValidate_parentEither(pCI,
                                     CK_o_GBYAGG,
                                     CK_o_INDEXJOIN,
                                     CK_o_SORT,
                                     CK_NONE);

/**Z_KEY_DIRECTION validation*/
  vValidate_mustHave(CK_EXPR,CK_NONE);

  Card::vValidate(pCI,iDepth,pParent);
  pCI->vValidateChildren(this,iDepth+1);
  vValidationPassed();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
