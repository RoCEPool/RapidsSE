/*============================================================================*/
/* Card_Key.cpp -- Key                                                        */
/*============================================================================*/

#include "Card_Key.hpp"

#include "Card_As.hpp"
#include "Card_Direction.hpp"
#include "Card_Expr.hpp"
#include "Card_RpnCalc.hpp"
#include "Card_HshCol.hpp"
#include "Card_Term.hpp"
#include "CardIter.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_Key,CK_KEY)
__aNewFunction(Card_Key,CN_CARD)
__vDeleteFunction(Card_Key,CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Key::Card_Key(void)
: Card_EXPRLIST(CK_KEY, 2)
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
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add a key expr to this key based on given projection expr                  */
/*----------------------------------------------------------------------------*/
void Card_Key::vAdd_key_expr_BasedOn_expr(Card_Expr* p_expr, bool bAsc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  i8 i_Key = nChildCards() + 1;

  Card_Expr* p_expr_new = new Card_Expr;
  p_expr_new->vRenumberForm_KeyCol(i_Key);

  Card_RpnCalc* p_rpnCalc = new Card_RpnCalc;
  if (p_expr->pSub_rpnCalc())
    p_rpnCalc->vAppendChild(p_expr->pSub_rpnCalc()->pGetTerm(0)->pDupCardRecursive());
  p_expr_new->vAppendChild(p_rpnCalc);

  p_rpnCalc->pSymbol()->vColTypeId(p_expr->xColTypeId());
  p_expr_new->pSymbol()->vColTypeId(p_expr->xColTypeId());

  if (p_expr->pSub_direction()) {
    Card_Direction *p_direction = (new Card_Direction) ->SYM(SF_DIRECTION, p_expr->pSub_direction()->szFacet(SF_NONE));
    p_expr_new->vAppendChild(p_direction);
  }
  else
  if (bAsc) {
    Card_Direction *p_direction = (new Card_Direction) ->SYM(SF_DIRECTION, "ASC"_Stz);
    p_expr_new->vAppendChild(p_direction);
  }

  vAppendChild(p_expr_new);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_Key::bIsEqual(Card_Key* p_key_arg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  RASSERT(CT_NONE != this->xColTypeId().eCTVal());

  if (xColTypeId() != p_key_arg->xColTypeId())
    return false;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Card_RpnCalc* p_rpnCalc_this = pSub_rpnCalc();
  Card_RpnCalc* p_rpnCalc_arg  = p_key_arg->pSub_rpnCalc();

  if (p_rpnCalc_this) {
    if (p_rpnCalc_arg) {
      if (!p_rpnCalc_this->bSameAs(p_rpnCalc_arg))
        return false;
    }
    else {
      return false;
    }
  }
  else
  if (p_rpnCalc_arg) {
    return false;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Card_Direction* p_direction_this = pSub_direction();
  Card_Direction* p_direction_arg  = p_key_arg->pSub_direction();

  if (p_direction_this) {
    if (p_direction_arg) {
      if (! p_direction_this->bIsEqual(p_direction_arg))
        return false;
    }
    else {
      return false;
    }
  }
  else
  if (p_direction_arg) {
    return false;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  return true;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Key::vTypes_For_stmCol(CardIter *pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  for (Kid xKid(this); xKid.bEach(CK_EXPR); /**/)
    xKid.p_expr()->vEnsureTypes_For_stmCol(pCI);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Key::vFactorAll_col_IntoDistinct_expr(CardIter *pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  RASSERT(! pCI->G.p_subSet);

  Card_SubSet* p_subSet = new Card_SubSet;

  // key is a set of expr
  for (Kid x_expr(this); x_expr.bEach(CK_EXPR); /**/) {

    x_expr.p_expr()->vFactorAll_ColSym_IntoDistinct_expr(pCI);
    p_subSet->vTransferChildren(pCI->G.p_subSet);

    DELETE_CARD(pCI->G.p_subSet);
  }

  pCI->G.p_subSet = p_subSet;
}

/*----------------------------------------------------------------------------*/
/* map the columns of this key                                                */
/*----------------------------------------------------------------------------*/
void Card_Key::vMapColumns(CardIter *pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  i8 i_Cell = 0;

  for (Kid xKey(this); xKey.bEach(CK_EXPR); /**/) {
    Card_Expr *p_expr_key = xKey.p_expr();

    i_Cell++;
    pCI->G_vAppendNewChild(pCI->G.p_keyCells,CK_MAP,p_expr_key->pSymbol());
    pCI->G.p_map->pSymbol()->Y_vSet(P_Cell, i_Cell);
  }

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Parser::TypeId Card_Key::xColTypeId(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

	Parser::TypeId xColTypeId;

  if (Card_Expr* p_expr = pSub_expr())
		xColTypeId = p_expr->xColTypeId();

  return xColTypeId;
}

/*----------------------------------------------------------------------------*/
/* short form                                                                 */
/*----------------------------------------------------------------------------*/
void Card_Key::vShortForm(String* pString)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  pString->vCopy("k:"_Stz);
  Card_EXPRLIST::vShortForm(pString);
  //pString->vConcat("}"_Stz);

}

/*----------------------------------------------------------------------------*/
/* convert key to indexDef                                                    */
/*----------------------------------------------------------------------------*/
IndexDef* Card_Key::pToIndexDef(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  bool bHasDescending = false;

  IndexDef* pIndexDef = new IndexDef;

  Kid xKey(this);
  while (xKey.bEach(CK_EXPR)) {
    Card_Expr* p_expr = xKey.p_expr();

    Card_RpnCalc* p_rpnCalc = p_expr->pSub_rpnCalc();

    Card_Term* p_term = p_rpnCalc->pGetTerm(0);
    if (! p_term)
      continue;

    Stz* szColNm  = p_term->szFacet(SF_NONE);
    eCT  eColType = p_term->pSymbol()->xColTypeId().eCTVal();

    if (! szColNm || CT_NONE == eColType)
      continue;

    bool bAscending = true;
    if (p_expr->bIsDescending()) {
      bAscending = false;
      bHasDescending = true;
    }
    pIndexDef->vAddColumn(szColNm,eColType,bAscending);
  }

  if (bHasDescending)
    pIndexDef->vHasDescending(true);

  if (pIndexDef->nColumns())
    return pIndexDef;

  delete pIndexDef;
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Key::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);
  UNUSED(pParent);

  vValidate_mustHave(CK_EXPR,CK_NONE);
  Card::vValidate(pCI,iDepth,pParent);
  pCI->vValidateChildren(this,iDepth+1);
  vValidationPassed();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
