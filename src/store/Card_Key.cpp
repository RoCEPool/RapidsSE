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
  p_expr_new->vRenumberForm_keyCol(i_Key);

  Card_RpnCalc* p_rpnCalc = new Card_RpnCalc;
  if (p_expr->pSub_rpnCalc())
    p_rpnCalc->vAppendChild(p_expr->pSub_rpnCalc()->pGetTerm(0)->pDupCardRecursive());
  p_expr_new->vAppendChild(p_rpnCalc);

  p_rpnCalc->vCast(p_expr->xCast());
  p_expr_new->vCast(p_expr->xCast());

  if (p_expr->pSub_direction()) {
    Card_Direction *p_direction = (new Card_Direction) ->SYM(SF_DIRECTION, p_expr->pSub_direction()->szFacet(SF_NONE));
    p_expr_new->vAppendChild(p_direction);
  }
  else
  if (bAsc) {
    Card_Direction *p_direction = (new Card_Direction) ->SYM(SF_DIRECTION, "ASC"_Stz);
    p_expr_new->vAppendChild(p_direction);
  }

  if (0 == i_Key)
    vCast(p_expr->xCast());

  vAppendChild(p_expr_new);
}

/*----------------------------------------------------------------------------*/
/* test if this key equal to given arg                                        */
/*----------------------------------------------------------------------------*/
bool Card_Key::bIsEqual(Card_Key* p_key_arg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  RASSERT(CT_NONE != this->eCTVal());

//  if (xTypeId() != p_key_arg->xTypeId())
//    return false;

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
/* ensure cast info                                                           */
/*----------------------------------------------------------------------------*/
void Card_Key::vEnsureTypeId_expCol(CardIter *pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  for (Kid xKid(this); xKid.bEach(CK_EXPR); /**/)
    xKid.p_expr()->vEnsureTypeId_expCol(pCI);
}

/*----------------------------------------------------------------------------*/
/* find expression which expresses the given symbol                           */
/*----------------------------------------------------------------------------*/
Card_Expr* Card_Key::pDecode_scalar_expr_Via_symbol(Symbol* pSymbol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  for (Kid xKey(this); xKey.bEach(CK_EXPR); /**/) {
    Symbol* pSymbol_term0 = xKey.p_expr()->pGet_Symbol(0);

    if (pSymbol_term0->bMatches(pSymbol))
      return xKey.p_expr();
  }

  return nullptr;
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
void Card_Key::vPrint(Ribbon* pRibbon, i8 iDepth)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  UNUSED(iDepth);

  s32 sName;

  sName.vSprint("%s%lld]"_Stz,
                Stz::szDots(iDepth - 1),
                m_i_Child);

  sName.vSprintCat("%s"_Stz,
                   szCardNm());

  pRibbon->vSprintSnippet(false,"%c%c%s %p"_Stz,
                          cMark1(),
                          cMark2(),
                          sName.szStz(),
                          ((char *) this) - (char *) AM::SYSARGS::aRegionAddr());

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
    TypeId xTypeId = p_term->xTypeId();

    if (! szColNm || CT_NONE == xTypeId.eCTVal())
      continue;

    bool bAscending = true;
    if (p_expr->bIsDescending()) {
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
  UNUSED(pParent);

  vValidate_mustHave(CK_EXPR,CK_NONE);
  Card::vValidate(pCI,iDepth,pParent);
  pCI->vValidateChildren(this,iDepth+1);
  vValidationPassed();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
