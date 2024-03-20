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

#ifdef Z_KEY_DIRECTION
  i8 i_Key = nChildCards() + 1;

  Card_Expr* p_expr_new = new Card_Expr;
  p_expr_new->vRenumberForm_keyCol(i_Key);

  Card_RpnCalc* p_rpnCalc = new Card_RpnCalc;
  if (p_expr->pSub_rpnCalc(C_NOT_NULL))
    p_rpnCalc->vAppendChild(p_expr->pGet_term(0)->pDupCardRecursive());
  else
    NEVER_REACH

  p_expr_new->vAppendChild(p_rpnCalc);

  p_rpnCalc->vApplyCast_x(p_expr->xCast());
  p_expr_new->vApplyCast_x(p_expr->xCast());

  if (p_expr->pSub_direction()) {
    Card_Direction *p_direction = (new Card_Direction) ->SYM(SF_DIRECTION, p_expr->pSub_direction()->szFacet(SF_DIRECTION));
    p_expr_new->vAppendChild(p_direction);
  }
  else
  if (bAsc) {
    Card_Direction *p_direction = (new Card_Direction) ->SYM(SF_DIRECTION, "ASC"_Stz);
    p_expr_new->vAppendChild(p_direction);
  }

  if (0 == i_Key)
    vApplyCast_x(p_expr->xCast());

#else

  i8 i_Key = nChildCards() + 1;

  Card_Expr* p_expr_new = new Card_Expr;
  p_expr_new->vRenumberForm_keyCol(i_Key);

  Card_RpnCalc* p_rpnCalc = new Card_RpnCalc;
  if (p_expr->pSub_rpnCalc(C_NOT_NULL))
    p_rpnCalc->vAppendChild(p_expr->pGet_term(0)->pDupCardRecursive());
  else
    NEVER_REACH

  p_expr_new->vAppendChild(p_rpnCalc);

  p_rpnCalc->vApplyCast_x(p_expr->xCast());
  p_expr_new->vApplyCast_x(p_expr->xCast());

  if (p_expr->pSub_direction()) {
    Card_Direction *p_direction = (new Card_Direction) ->SYM(SF_DIRECTION, p_expr->pSub_direction()->szFacet(SF_DIRECTION));
    p_expr_new->vAppendChild(p_direction);
  }
  else
  if (bAsc) {
    Card_Direction *p_direction = (new Card_Direction) ->SYM(SF_DIRECTION, "ASC"_Stz);
    p_expr_new->vAppendChild(p_direction);
  }

  if (0 == i_Key)
    vApplyCast_x(p_expr->xCast());

  vAppendChild(p_expr_new);
#endif
}

/*----------------------------------------------------------------------------*/
/* ensure cast info                                                           */
/*----------------------------------------------------------------------------*/
void Card_Key::vApplyCast(CardIter *pCI, Card_Emit* p_Dic_expr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

#ifdef Z_KEY_DIRECTION
  for (Kid xKey(this); xKey.bEach(CK_DIRECTION); /**/) {
    pCI->G.p_direction = xKey.p_direction();
    pCI->G.p_expr = pCI->G.p_direction->pSub_expr();
    pCI->G.p_expr->vApplyCast(pCI,/*??*/p_Dic_expr);
  }
#else
  for (Kid xKey(this); xKey.bEach(CK_EXPR); /**/) {
    pCI->G.p_expr = xKey.p_expr();
    pCI->G.p_expr->vApplyCast(pCI,/*??*/p_Dic_expr);
  }
#endif

  pCI->G.p_expr = nullptr;
  pCI->G.p_direction = nullptr;

}

/*----------------------------------------------------------------------------*/
/* find expression which expresses the given symbol                           */
/*----------------------------------------------------------------------------*/
Card_Expr* Card_Key::pDecode_scalar_expr_Via_symbol(Symbol* pSymbol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

#ifdef Z_KEY_DIRECTION
  for (Kid xKey(this); xKey.bEach(CK_DIRECTION); /**/) {
    Card_Direction* p_direction = xKey.p_direction();
    Card_Expr* p_expr = p_direction->pSub_expr();

    Symbol* pSymbol_term0 = p_expr->pGet_Symbol(0);

    if (pSymbol_term0->bMatches(pSymbol))
      return p_expr;
  }
#else
  for (Kid xKey(this); xKey.bEach(CK_EXPR); /**/) {
    Symbol* pSymbol_term0 = xKey.p_expr()->pGet_Symbol(0);

    if (pSymbol_term0->bMatches(pSymbol))
      return xKey.p_expr();
  }
#endif

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* find <expr> which has same symbol as given $n stream-column                */
/*----------------------------------------------------------------------------*/
Card_Expr* Card_Key::pFind_expr_key_Matches(Symbol *pSymbol_arg)

{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  for (Kid xKey(this); xKey.bEach(CK_DIRECTION); /**/) {
    Card_Direction* p_direction = xKey.p_direction();
    Card_Expr* p_expr = p_direction->pSub_expr();

    if (p_expr->pSymbol()->bMatches(pSymbol_arg))
      return p_expr;
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get a particular direction                                                 */
/*----------------------------------------------------------------------------*/
Card_Direction* Card_Key::pGet_direction(i8 i_Direction)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_Direction* p_direction = _pCard_Direction(pGetChild(i_Direction));

  return p_direction;
}

/*----------------------------------------------------------------------------*/
/* test if this key equal to given arg                                        */
/*----------------------------------------------------------------------------*/
bool Card_Key::bIsEqual(Card_Key* p_key_arg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  RASSERT(CT_NONE != this->eCTVal());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (! pSub_expr()->bSameAs(p_key_arg->pSub_expr())) {
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
/* map the columns of this key                                                */
/*----------------------------------------------------------------------------*/
void Card_Key::vMapColumns(CardIter *pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  i8 i_Cell = 0;

#ifdef Z_KEY_DIRECTION
  for (Kid xKey(this); xKey.bEach(CK_DIRECTION); /**/) {
    Card_Direction *p_direction_key = xKey.p_direction();
    Card_Expr *p_expr_key = p_direction_key->pSub_expr();

    i_Cell++;
    pCI->vAppendNewChild(pCI->G.p_keyCells,CK_MAP,p_expr_key->pSymbol());
    pCI->G.p_map->pSymbol()->Y_vSet(P_Cell, i_Cell);
  }
#else
  for (Kid xKey(this); xKey.bEach(CK_EXPR); /**/) {
    Card_Expr *p_expr_key = xKey.p_expr();

    i_Cell++;
    pCI->vAppendNewChild(pCI->G.p_keyCells,CK_MAP,p_expr_key->pSymbol());
    pCI->G.p_map->pSymbol()->Y_vSet(P_Cell, i_Cell);
  }
#endif

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
/* convert key to indexDef                                                    */
/*----------------------------------------------------------------------------*/
IndexDef* Card_Key::pTo_IndexDef(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  bool bHasDescending = false;

  IndexDef* pIndexDef = new IndexDef;

#ifdef Z_KEY_DIRECTION
  for (Kid xKey(this); xKey.bEach(CK_DIRECTION); /**/) {

    Card_Direction* p_direction = xKey.p_direction();
    Card_Expr* p_expr = p_direction->pSub_expr();

    Card_Term* p_term = p_expr->pGet_term(0);
    if (! p_term)
      continue;

    Stz* szColNm  = p_term->szFacet(SF_EXP_COL);
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

#else
  for (Kid xKey(this); xKey.bEach(CK_EXPR); /**/) {
    Card_Expr* p_expr = xKey.p_expr();

    Card_Term* p_term = p_expr->pGet_term(0);
    if (! p_term)
      continue;

    Stz* szColNm  = p_term->szFacet(SF_EXP_COL);
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
#endif

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

#ifdef Z_KEY_DIRECTION
  vValidate_mustHave(CK_DIRECTION,CK_NONE);
#else
  vValidate_mustHave(CK_EXPR,CK_NONE);
#endif

  Card::vValidate(pCI,iDepth,pParent);
  pCI->vValidateChildren(this,iDepth+1);
  vValidationPassed();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
