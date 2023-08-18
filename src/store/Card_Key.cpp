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
: Card_TIER2_ELIST(CK_KEY)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Key::Card_Key(eCK eCard)
    : Card_TIER2_ELIST(eCard)
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
  p_expr_new->vRenumberForm_keyOdn(i_Key);

  if (Card_As* p_as = p_expr->pSub_as())
    p_expr_new->vAppend(p_as->pDupCardRecursive());

  Card_RpnCalc* p_rpnCalc = new Card_RpnCalc;
  if (p_expr->pSub_rpnCalc())
    p_rpnCalc->vAppend(p_expr->pSub_rpnCalc()->pGetTerm(0)->pDupCardRecursive());
  p_expr_new->vAppend(p_rpnCalc);

  p_rpnCalc->pSymbol()->Y_vColType(p_expr->szP_Type());
  p_expr_new->pSymbol()->Y_vColType(p_expr->szP_Type());

  if (p_expr->pSub_direction()) {
    Card_Direction *p_direction = (new Card_Direction) ->SYM(SF_DIRECTION, p_expr->pSub_direction()->szSymbol(SF_NONE));
    p_expr_new->vAppend(p_direction);
  }
  else
  if (bAsc) {
    Card_Direction *p_direction = (new Card_Direction) ->SYM(SF_DIRECTION, "ASC"_Stz);
    p_expr_new->vAppend(p_direction);
  }

  vAppend(p_expr_new);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_Key::bIsEqual(Card_Key* p_key_arg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  RASSERT(this->szP_Type());

  if (! szP_Type()->bSame(p_key_arg->szP_Type()))
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
void Card_Key::vTypes_For_stmColOdn(CardIter *pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  for (Kid xKid(this); xKid.bEach(CK_EXPR); /**/)
    xKid.p_expr()->vEnsureTypes_For_stmOdn(pCI);
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

    x_expr.p_expr()->vFactorAll_valOdn_IntoDistinct_expr(pCI);
    p_subSet->vTransferChildren(pCI->G.p_subSet);

    DELETE_CARD(pCI->G.p_subSet);
  }

  pCI->G.p_subSet = p_subSet;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Stz* Card_Key::szP_Type(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Stz* szP_Type = nullptr;

  if (Card_Expr* p_expr = pSub_expr())
    szP_Type = p_expr->szP_Type();

  return szP_Type;
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

    Stz* szColNm  = p_term->szSymbol(SF_NONE);
    eCT  eColType = p_term->pSymbol()->Y_eColType();

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
void Card_Key::vX(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Key::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);
  UNUSED(pParent);

  Card_TIER2_ELIST::vValidate(pCI, iDepth, pParent);

  vValidate_mustHave(CK_EXPR,CK_NONE);
  pCI->vValidateChildren(this,iDepth+1);
  vValidationPassed();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
