/*============================================================================*/
/* Card_Expr.cpp -- Typed Expression                                          */
/*============================================================================*/

#include "Card_Expr.hpp"

#include "Card_Predicate.hpp"
#include "CardIter.hpp"
#include "TypeSystem.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_Expr,CK_EXPR)
__aNewFunction(Card_Expr,CN_CARD)
__vDeleteFunction(Card_Expr,CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Expr::Card_Expr(void)
: Card_symbol(CK_EXPR, C_UNKNOWN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}



/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Expr::~Card_Expr(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Expr::vAssemble_injCrlRsl(CardIter* pCI)
{
  _static_

  eAL e_Level     = (eAL) pCI->E.p_level->pSymbol()->Y_iGet(P_Level);

  if (AL_PARTITION == e_Level) {
    pCI->E.p_expr = Card_Expr::pCraft_expr_keyCol_Of_term(pCI->E.p_expr->pGet_term0());
  }
  else {
    Symbol xSymbol_KeyCol(SF_KEY_COL,1);

    pCI->vNewMember(CK_EXPR);
    i8 i_Board = pCI->Flat.m_pSqy_inner->pBoard()->i_Board();
    pCI->E.p_expr->pSymbol()->vApplyFacet_varg(SF_NMD_COL, C_CRLKEY, i_Board);
    pCI->vAppendNewChild(pCI->E.p_expr, CK_TERM, &xSymbol_KeyCol);

    pCI->E.p_term->vApplyCast_x(pCI->E.p_key->xCast());
    pCI->E.p_expr->vApplyCast_x(pCI->E.p_key->xCast());

    pCI->E.p_emit->vBump_expCol(pCI);
    pCI->E.p_key->pSymbol()->Y_vSet(P_ProjBumped);
  }

  pCI->E.p_expr->pSymbol()->Y_vSet(P_InjCrlKey);

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Expr* Card_Expr::pCraft_expr_Of_expr(Card_Expr* p_expr, i8 iExpCol)
{
  _static_

  RASSERT(CT_NONE != p_expr->eCTVal());

  Symbol xSymbol_ExpCol(SF_EXP_COL, iExpCol, p_expr->xCast());
  Card_Expr* p_expr_new = pCraft_expr_Of_symbol(&xSymbol_ExpCol);

  return p_expr_new;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Expr* Card_Expr::pCraft_expr_Of_symbol(Symbol* pSymbol)
{
  _static_

  Card_Expr* p_expr_new = new Card_Expr;
  p_expr_new->vApplyCast_x(pSymbol->xCast());

  Card_Term* p_term_new = (new Card_Term) ->SYM(pSymbol);
  p_term_new->vApplyCast_x(pSymbol->xCast());
  p_expr_new->vAppendChild(p_term_new);

  return p_expr_new;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Expr* Card_Expr::pCraft_expr_Of_term(Card_Term* p_term)
{
  _static_

  Cast xCast = p_term->pSymbol()->xCast();

  Card_Expr* p_expr_new = new Card_Expr;
  p_expr_new->vApplyCast_x(xCast);

  Card_Term* p_term_new = p_term->pClone();
  p_term_new->vApplyCast_x(xCast);
  p_expr_new->vAppendChild(p_term_new);

  return p_expr_new;
}

/*----------------------------------------------------------------------------*/
/* form project expression from gby key expression                            */
/*----------------------------------------------------------------------------*/
Card_Expr* Card_Expr::pCraft_expr_keyCol_Of_term(Card_Term* p_term)
{
  _static_

  Card_Expr* p_expr = pCraft_expr_Of_term(p_term);

  Symbol xSymbol_Kn(SF_KEY_COL, C_UNKNOWN);
  p_expr->pGet_term0()->pSymbol()->vApplyFacets(&xSymbol_Kn);

  return p_expr;
}



/*----------------------------------------------------------------------------*/
/* craft a casted expr for given column                                       */
/*----------------------------------------------------------------------------*/
Card_Expr* Card_Expr::pCraft_expr_of_nmdCol(Column *pColumn, i8 iExpCol,
                                            char cTblSyn, i8 iTblCol)
{
  _static_

  Cast xCast;
  xCast.m_xTypeId       = pColumn->xColumnTypeId();
  xCast.m_nTypeLength   = pColumn->iColumnLength();
  xCast.m_nTypeScale    = pColumn->iColumnScale();
  xCast.m_bTypeNullable = pColumn->bIsNullable();

  Symbol xSym(SF_TBL_COL, cTblSyn, iTblCol);
  xSym.vApplyCast(xCast);

  Card_Expr* p_expr_new = pCraft_expr_Of_symbol(&xSym);

  Stz* szNmdCol = pColumn->szColNm();
  p_expr_new->pSymbol()->vApplyFacet(SF_NMD_COL, szNmdCol);
  p_expr_new->pSymbol()->vApplyFacet(SF_EXP_COL, iExpCol);
  p_expr_new->pSymbol()->vApplyCast(xCast);

  return p_expr_new;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Expr::vApplyCast(CardIter* pCI, Card_EXPRLIST* p_Dic_expr)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Cast xCast_last;
  xCast_last.m_xTypeId = TypeId();
  xCast_last.m_nTypeLength = 0;
  xCast_last.m_nTypeScale = 0;
  RASSERT(!xCast_last.m_xTypeId.bIsValid());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  for (pCI->vBeginTarget(this,CK_TERM); pCI->bEachTarget(); /**/) {
    pCI->E.p_term = pCI->E.xTarget.p_term();
    pCI->E.p_term->vApplyCast(pCI, /*??*/p_Dic_expr, xCast_last);
  }
  if (m_xSymbol.xCast().m_xTypeId.eCTVal() == CT_VARCHAR &&
      xCast_last.m_xTypeId.eCTVal() == CT_VARCHAR) {
    if (xCast_last.m_nTypeLength)
      m_xSymbol.vApplyCast(xCast_last);
  }
  else
  if (m_xSymbol.xCast().m_xTypeId.eCTVal() == CT_DECIMAL &&
      xCast_last.m_xTypeId.eCTVal() == CT_DECIMAL &&
      xCast_last.m_nTypeLength) {
    m_xSymbol.vApplyCast(xCast_last);
  }

    pCI->E.p_term = nullptr;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Expr* Card_Expr::pClone(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_Expr* p_clone = _pCard_Expr(pDupCardRecursive());

  return p_clone;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Expr::vDeleteIf_facet(CardIter* pCI, eSF eFacet, eSF eAlsoFacet, char cTblSyn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  for (pCI->vBeginTarget(this,CK_TERM); pCI->bEachTarget(); /**/)
    pCI->E.xTarget.p_term()->vDeleteFacetIf(eFacet, eAlsoFacet, cTblSyn);
}

/*----------------------------------------------------------------------------*/
/* ensure expression has a SF_NMD_COL                                         */
/*----------------------------------------------------------------------------*/
void Card_Expr::vEnsure_nmdCol(i8 iExpCol)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Symbol* pSym_expr = pSymbol();
  if (pSym_expr->bHasFacet(SF_NMD_COL))
    return;

  if (! bIsCompound()) {
    Card_Term* p_term0 = pGet_term0();
    Symbol* pSym_term0 = p_term0->pSymbol();
    if (pSym_term0->bHasFacet(SF_NMD_COL)) {
      pSym_expr->vApplyFacet(SF_NMD_COL, pSym_term0);
      return;
    }
  }

  // synthetic column name
  pSym_expr->vApplyFacet(SF_NMD_COL,iExpCol);
}

/*----------------------------------------------------------------------------*/
/* does expression have term ?                                                */
/*----------------------------------------------------------------------------*/
bool Card_Expr::bFind_term(CardIter* pCI, Symbol* pSym_term)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  for (pCI->vBeginTarget(this,CK_TERM); pCI->bEachTarget(); /**/) {
    if (pCI->E.xTarget.p_term()->pSymbol()->bMatches(pSym_term)) {
      pCI->vEndTarget();
      return true;
    }
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* return first key col in this rpn sequence                                  */
/*----------------------------------------------------------------------------*/
Symbol* Card_Expr::pFirst_colSym(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  for (pCI->vBeginTarget(this,CK_TERM); pCI->bEachTarget(); /**/)
    if (pCI->E.xTarget.p_term()->pSymbol()->bHasFacet(SF_EXP_COL) ||
        pCI->E.xTarget.p_term()->pSymbol()->bHasFacet(SF_TBL_COL))
      return pCI->pEndTarget()->pSymbol();

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get first stm col                                                          */
/*----------------------------------------------------------------------------*/
Symbol* Card_Expr::pFirst_expCol(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  for (pCI->vBeginTarget(this,CK_TERM); pCI->bEachTarget(); /**/)
    if (pCI->E.xTarget.p_term()->pSymbol()->bHasFacet(SF_EXP_COL))
      return pCI->pEndTarget()->pSymbol();

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get first key col                                                          */
/*----------------------------------------------------------------------------*/
Symbol* Card_Expr::pFirst_keyCol(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  for (pCI->vBeginTarget(this,CK_TERM); pCI->bEachTarget(); /**/)
    if (pCI->E.xTarget.p_term()->pSymbol()->bHasFacet(SF_KEY_COL))
      return pCI->pEndTarget()->pSymbol();

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get first tbl col                                                          */
/*----------------------------------------------------------------------------*/
Symbol* Card_Expr::pFirst_tblCol(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  for (pCI->vBeginTarget(this,CK_TERM); pCI->bEachTarget(); /**/)
    if (pCI->E.xTarget.p_term()->pSymbol()->bHasFacet(SF_TBL_COL))
      return pCI->pEndTarget()->pSymbol();

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get symbol of given term                                                   */
/*----------------------------------------------------------------------------*/
Symbol* Card_Expr::pGet_Symbol(i8 i_Term)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_Term* p_term_i = pGet_term(i_Term);
  if (! p_term_i)
    return nullptr;

  Symbol* p_symbol_i = p_term_i->pSymbol();

  return p_symbol_i;
}

/*----------------------------------------------------------------------------*/
/* get given term                                                             */
/*----------------------------------------------------------------------------*/
Card_Term* Card_Expr::pGet_term(i8 i_Term)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (i_Term >= nChildCards())
    return nullptr;

  Card_Term* p_term = _pCard_Term(pGetChild(i_Term));

  return p_term;
}

/*----------------------------------------------------------------------------*/
/* get term 0 (of a scalar expr)                                              */
/*----------------------------------------------------------------------------*/
Card_Term* Card_Expr::pGet_term0(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  RASSERT(1 == nChildCards());

  Card_Term* p_term0 = pGet_term(0);

  return p_term0;
}

/*----------------------------------------------------------------------------*/
/* get symbol of first/head sub-card                                          */
/*----------------------------------------------------------------------------*/
Symbol* Card_Expr::pHead_symbol(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (Card* pCard_head = pPeekChild()) {
    Symbol* pSym_head = pCard_head->pSymbol();
    return pSym_head;
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get symbol of first/head sub-card                                          */
/*----------------------------------------------------------------------------*/
Symbol* Card_Expr::pHeadIf_symbol(eSF eFacet)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (Card* pCard_head = pPeekChild()) {
    Symbol* pSym_head = pCard_head->pSymbol();
    if (pSym_head->bHasFacet(eFacet))
      return pSym_head;
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* induct this arriving expr                                                  */
/*----------------------------------------------------------------------------*/
Card_Expr * Card_Expr::pInduct(CardIter* pCI, i8 iExp)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_Expr* p_expr_inducted = new Card_Expr;
  Symbol* pSymbol_expr_inducted = p_expr_inducted->pSymbol();

  //vDump(__func__);
  p_expr_inducted->vApplyCast_p(this);

  Symbol* pSymbol_expr_arriving = pSymbol();

  RASSERT(pSymbol_expr_arriving->bHasFacet(SF_EXP_COL));

  if (pSymbol_expr_arriving->bHasFacet(SF_NMD_COL))
    pSymbol_expr_inducted->vApplyFacet(SF_NMD_COL,pSymbol_expr_arriving);
  else
    p_expr_inducted->pSymbol()->vApplyFacet(SF_NMD_COL,iExp);

  if (pSymbol_expr_arriving->bHasFacet(SF_SQY_RSL))
    pSymbol_expr_inducted->vApplyFacet(SF_SQY_RSL,pSymbol_expr_arriving);

//  Card_RpnCalc* p_rpnCalc_inducted = p_rpnCalc_arriving->vInduct(this);
//  p_expr_inducted->vAppendChild(p_rpnCalc_inducted);


  Card_Term* p_term_inducted = new Card_Term;
  p_term_inducted->vApplyCast_p(this);

  if (bIsScalarCol() && pFirst_colSym(pCI)->bHasFacet(SF_TBL_COL)) {
    p_term_inducted ->SYM(SF_TBL_COL, pFirst_colSym(pCI)->szFacet(SF_TBL_COL));
  }
  if (pSymbol()->bHasFacet(SF_EXP_COL)) {
    p_term_inducted ->SYM(SF_EXP_COL, szFacet(SF_EXP_COL));
  }

  p_expr_inducted->vAppendChild(p_term_inducted);

  return p_expr_inducted;
}

/*----------------------------------------------------------------------------*/
/* is this an expression of a key                                             */
/*----------------------------------------------------------------------------*/
bool Card_Expr::bIsKeyCol(void) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Symbol *pSymbol_0 = pGet_Symbol(0);

  if (pSymbol_0->bHasFacet(SF_KEY_COL))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a compound expr?                                                   */
/*----------------------------------------------------------------------------*/
bool Card_Expr::bIsCompound(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (nChildCards() > 1)
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a literals expr?                                                   */
/*----------------------------------------------------------------------------*/
bool Card_Expr::bIsLiterals(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  for (pCI->vBeginTarget(this,CK_TERM); pCI->bEachTarget(); /**/) {
    if (pCI->E.xTarget.p_term()->pSymbol()->bHasFacet(SF_EXP_COL) ||
        pCI->E.xTarget.p_term()->pSymbol()->bHasFacet(SF_TBL_COL)) {
      pCI->vEndTarget();
      return false;
    }
  }

  return true;
}

/*----------------------------------------------------------------------------*/
/* is this expr a simple expression of a column?                              */
/*----------------------------------------------------------------------------*/
bool Card_Expr::bIsScalarCol(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (nChildCards() > 1)
    return false;

  Card_Term* p_term_0 = pGet_term(0);
  if (! p_term_0->pSymbol()->bHasFacet(SF_EXP_COL) &&  !p_term_0->pSymbol()->bHasFacet(SF_TBL_COL))
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/* is this an expression of given symbol ?                                    */
/*----------------------------------------------------------------------------*/
bool Card_Expr::bMatches(Card_Expr* p_expr_arg)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Symbol* pSymbol_arg = p_expr_arg->pGet_Symbol(0);
  Symbol* pSymbol_this = this->pGet_Symbol(0);

  if (pSymbol_this->bMatches(pSymbol_arg))
    return true;
  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_Expr::bNeedsBrace(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  for (pCI->vBeginTarget(this,CK_TERM); pCI->bEachTarget(); /**/) {
    if (pCI->E.xTarget.p_term()->pSymbol()->bNeedsBrace()) {
      pCI->vEndTarget();
      return true;
    }
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_Expr::bEqualTo(Card_Expr* pExpr_arg)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  i8 nChildren = nChildCards();

  if (nChildren != pExpr_arg->nChildCards())
    return false;

  for (i8 i_Child = 0; i_Child != nChildren; i_Child++) {
    Card_Term* p_term_this = pGet_term(i_Child);
    Card_Term* p_term_arg = pExpr_arg->pGet_term(i_Child);

    if (p_term_this->bSymbolMatches(p_term_arg))
      continue;

    return false;
  }


  return true;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Expr::vUpdate_expCol(CardIter* pCI, Card_Emit* p_arriving)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  for (pCI->vBeginTarget(this,CK_TERM); pCI->bEachTarget(); /**/)
    pCI->E.xTarget.p_term()->vUpdate_expCol(pCI,p_arriving);
}

/*----------------------------------------------------------------------------*/
/* convert stmCol to tblCol                                                   */
/*----------------------------------------------------------------------------*/
void Card_Expr::vX_expCol_To_tblCol(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  for (pCI->vBeginTarget(this,CK_TERM); pCI->bEachTarget(); /**/) {
    if (! pCI->E.xTarget.p_term()->pSymbol()->bHasFacet(SF_EXP_COL))
      continue;

    pCI->E.p_term = pCI->E.xTarget.p_term();
    pCI->E.p_term->vX_expCol_To_tblCol(pCI);
  }

  pCI->E.p_term = nullptr;
}

/*----------------------------------------------------------------------------*/
/* convert tblCol to stmCol                                                   */
/*----------------------------------------------------------------------------*/
void Card_Expr::vX_tblCol_To_expCol(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  i8 nAncestors = pCI->nAncestors();
  pCI->vAncestorPush(this);

  for (pCI->vBeginTarget(this,CK_TERM); pCI->bEachTarget(); /**/) {
    pCI->E.p_term = pCI->E.xTarget.p_term();
    pCI->E.p_term->vX_tblCol_To_expCol(pCI);
  }

  pCI->E.p_term = nullptr;

  pCI->vAncestorPop();
  RASSERT(pCI->nAncestors() == nAncestors);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Expr::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Symbol* pSymbol_expr = pSymbol();
  RASSERT(pSymbol_expr->xCast().m_xTypeId.bIsValid());
  if (m_xSymbol.xTypeId() == TypeId::xIdOf(CT_DECIMAL)) {
    if (m_xSymbol.nTypeLength() == 0) {
      vMark("?"_Stz,"l"_Stz);
      pCI->pAncestorParent()->vMark("?"_Stz,"l"_Stz);
      pCI->PRINTBOARD_dbg();
      vDump(__func__);
      vValidationFailed("Decimal length is 0 : %p"_Stz, this);
    }
  }

/**Z_KEY_DIRECTION validation*/
  (void)Card::eValidate_parentEither(pCI,
                                     CK_s_CONSTRAINT,
                                     CK_t_EMIT,
                                     CK_s_KEY,

                                     CK_c_FLAT,
                                     CK_c_FOLD,
                                     CK_s_INJECTABLE,
                                     CK_s_NEEDED,
                                     CK_STREAMKEY,
                                     CK_NONE);

  vValidate_mustHave(CK_TERM, CK_NONE);

  if (pCI->bAncestor(CK_s_KEY)) {
    //vDump(__func__);
    if (! m_xSymbol.bHasFacet(SF_KEY_COL) || ! m_xSymbol.bValidate()) {
      vDump(__func__);
      vValidationFailed("SF_KEY_COL Missing : %p"_Stz,this);
    }

    RASSERT(bIsScalarCol());
  }
  else {
    if (! (m_xSymbol.bHasFacet(SF_EXP_COL) || m_xSymbol.bHasFacet(SF_KEY_COL))) {
      vDump(__func__);
      vValidationFailed("SF_EXP/KEY_COL Missing : %p"_Stz,this);
    }
  }

  RASSERT(! m_xSymbol.bHasFacet(SF_TBL_COL));


  Card::vValidate(pCI,iDepth,pParent);
  pCI->vValidateChildren(this,iDepth+1);
  vValidationPassed();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
