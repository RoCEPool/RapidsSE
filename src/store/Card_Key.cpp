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

/**Z_KEY_DIRECTION add key*/
  i8 i_Key = nChildCards() + 1;

  Card_Direction *p_direction_new;
  if (p_expr->pSub_direction()) {
    p_direction_new = (new Card_Direction) ->SYM(SF_DIRECTION, p_expr->pSub_direction()->szFacet(SF_DIRECTION));
  }
  else
  if (bAsc) {
    p_direction_new = (new Card_Direction) ->SYM(SF_DIRECTION, C_ASC);
  }
  else {
    p_direction_new = (new Card_Direction) ->SYM(SF_DIRECTION, C_DESC);
  }
  vAppendChild(p_direction_new);

  Card_Expr* p_expr_new = new Card_Expr;
  p_expr_new->vRenumberForm_keyCol(i_Key);
  p_direction_new->vAppendChild(p_expr_new);

  Card_RpnCalc* p_rpnCalc = new Card_RpnCalc;
  if (p_expr->pSub_rpnCalc(C_NOT_NULL))
    p_rpnCalc->vAppendChild(p_expr->pGet_term(0)->pDupCardRecursive());
  else
    NEVER_REACH
  p_expr_new->vAppendChild(p_rpnCalc);

  p_rpnCalc->vApplyCast_x(p_expr->xCast());
  p_expr_new->vApplyCast_x(p_expr->xCast());

  if (0 == i_Key)
    vApplyCast_x(p_expr->xCast());

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
  pCI->E.p_direction = nullptr;

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

  Card_Direction* p_direction = pGet_direction(i_Element);

  return p_direction->szFacet(SF_DIRECTION);
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
/* find <expr> which has same symbol as given $n stream-column                */
/*----------------------------------------------------------------------------*/
Card_Expr* Card_Key::pFind_expr_key_Matches(eSF eFacet_arg, Symbol* pSymbol_arg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  for (Kid xKey(this); xKey.bEach(CK_DIRECTION); /**/) {
    Card_Direction* p_direction = xKey.p_direction();
    Card_Expr* p_expr = p_direction->pSub_expr();
    if (p_expr->pSymbol()->bMatches(eFacet_arg, pSymbol_arg))
      return p_expr;
  }

  return nullptr;
}


/*----------------------------------------------------------------------------*/
/* get a particular direction                                                 */
/*----------------------------------------------------------------------------*/
Card_Direction* Card_Key::pGet_direction(i8 i_Element)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_Direction* p_direction = _pCard_Direction(pGetChild(i_Element));

  return p_direction;
}

/*----------------------------------------------------------------------------*/
/* get a particular direction/expr                                            */
/*----------------------------------------------------------------------------*/
Card_Expr* Card_Key::pGet_expr(i8 i_Element)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_Direction* p_direction = _pCard_Direction(pGetChild(i_Element));
  Card_Expr* p_expr = p_direction->pSub_expr();

  return p_expr;
}

/*----------------------------------------------------------------------------*/
/* is key element ascending?                                                  */
/*----------------------------------------------------------------------------*/
bool Card_Key::bIsAsc(i8 i_Element)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_Direction* p_direction = pGet_direction(i_Element);

  if (p_direction->szFacet(SF_DIRECTION)->bSame(C_ASC))
    return true;

  return false;
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

//Z_KEY_DIRECTION
  for (pCI->vBeginTarget(this,CK_EXPR); pCI->bEachTarget(); /**/) {
    pCI->vAppendNewChild(pCI->E.p_keyCells, CK_MAP, pCI->E.p_expr->pSymbol());
    pCI->E.p_map->pSymbol()->Y_vSet(P_Cell, ++i_Cell);
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
    if (pGet_direction(pCI->E.xTarget.i_Child())->pSymbol()->bHasFacet(C_DESC)) {
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
  vValidate_mustHave(CK_DIRECTION,CK_NONE);


  Card::vValidate(pCI,iDepth,pParent);
  pCI->vValidateChildren(this,iDepth+1);
  vValidationPassed();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
