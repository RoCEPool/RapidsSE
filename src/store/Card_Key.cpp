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
/* ensure cast info                                                           */
/*----------------------------------------------------------------------------*/
void Card_Key::vApplyCast(CardIter *pCI, Card_Emit* p_Dic_expr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

//Z_KEY_DIRECTION
  for (pCI->vBeginTarget(this,CK_EXPR); pCI->bEachTarget(); /**/) {
    pCI->E.p_expr->vApplyCast(pCI,/*??*/p_Dic_expr);
  }

  pCI->E.p_expr = nullptr;
}

/*----------------------------------------------------------------------------*/
/* find expression which expresses the given symbol                           */
/*----------------------------------------------------------------------------*/
Card_Expr* Card_Key::pDecode_scalar_expr_Via_symbol(CardIter *pCI, Symbol* pSymbol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  UNUSED(pCI);

//Z_KEY_DIRECTION
  for (pCI->vBeginTarget(CK_s_KEY,CK_EXPR); pCI->bEachTarget(); /**/) {
    Symbol* pSym_term0 = pCI->E.p_expr->pGet_Symbol(0);
    if (pSym_term0->bMatches(pSymbol))
      return _pCard_Expr(pCI->pEndTarget());
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* return direction string of key element                                     */
/*----------------------------------------------------------------------------*/
Stz* Card_Key::szDirection(i8 i_Element)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_Expr* p_expr = pGet_expr(i_Element);

  return p_expr->szFacet(SF_DIRECTION);
}

/*----------------------------------------------------------------------------*/
/* return number of key elements                                              */
/*----------------------------------------------------------------------------*/
i8 Card_Key::nElements(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  i8 nElements = nChildCards();

  return nElements;
}

/*----------------------------------------------------------------------------*/
/* find <expr> which has same symbol as given $n stream-column                */
/*----------------------------------------------------------------------------*/
Card_Expr* Card_Key::pFind_expr_key_Matches(CardIter *pCI, Symbol *pSymbol_arg)

{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  //Z_KEY_DIRECTION
  for (pCI->vBeginTarget(this,CK_EXPR); pCI->bEachTarget(); /**/) {
    if (pCI->E.p_expr->pSymbol()->bMatches(pSymbol_arg))
      return _pCard_Expr(pCI->pEndTarget());
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* find <expr> which has same symbol as given $n stream-column                */
/*----------------------------------------------------------------------------*/
Card_Expr* Card_Key::pFind_expr_key_Matches(CardIter *pCI, eSF eFacet_arg, Symbol* pSymbol_arg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  //Z_KEY_DIRECTION
  for (pCI->vBeginTarget(this,CK_EXPR); pCI->bEachTarget(); /**/) {
    if (pCI->E.p_expr->pSymbol()->bMatches(eFacet_arg, pSymbol_arg))
      return _pCard_Expr(pCI->pEndTarget());
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get a particular direction/expr                                            */
/*----------------------------------------------------------------------------*/
Card_Expr* Card_Key::pGet_expr(i8 i_Element)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

/**Z_KEY_DIRECTION get expr*/

  Card_Expr* p_expr = _pCard_Expr(pGetChild(i_Element));

  return p_expr;
}

/*----------------------------------------------------------------------------*/
/* test if this key equal to given arg                                        */
/*----------------------------------------------------------------------------*/
bool Card_Key::bIsEqual(CardIter *pCI, Card_Key* p_key_arg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  UNUSED(pCI);
  i8 nExpr = this->nChildCards();
  if (nExpr != p_key_arg->nChildCards())
    return false;

  //Z_KEY_DIRECTION
  for (pCI->vBeginTarget(this,CK_EXPR); pCI->bEachTarget(); /**/) {

    Card_Expr *p_expr_arg = p_key_arg->pGet_expr(pCI->E.xTarget.i_Child());

    if (! pCI->E.p_expr->bSameAs(p_expr_arg)) {
      pCI->vEndTarget();
      return false;
    }

    if (! Symbol::bSame(pCI->E.p_expr->pSymbol()->aFacet(SF_DIRECTION),
                        p_expr_arg->pSymbol()->aFacet(SF_DIRECTION))) {
      pCI->vEndTarget();
      return false;
    }
  }

  return true;
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
