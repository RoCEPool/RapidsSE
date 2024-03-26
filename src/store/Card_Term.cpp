/*============================================================================*/
/* Card_Term.cpp -- Term                                                      */
/*============================================================================*/

#include "Card_Term.hpp"

#include "Card_IdxColList.hpp"
#include "Card_IdxCol.hpp"
#include "CardIter.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_Term,CK_TERM)
__aNewFunction(Card_Term,CN_CARD)
__vDeleteFunction(Card_Term,CN_CARD)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Term::Card_Term(void)
    : Card_symbol(CK_TERM, C_UNKNOWN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Term::~Card_Term(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Term::vItemise_colNm(CardIter* pCI)
{
  _static_

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bfItemise_colNm = [pCI](Card* pCard)
  {
    UNUSED(pCI);

    if (AM::JOB::bHasTag(C_OPTIMISER_TRACE))
      vDebugPoint();

    if (!pCard->bIsCard(CK_TERM))
      return C_CONTINUE_RECURSION;

    if (pCI->bAncestor(CK_o_KEYEXISTS) || pCI->bAncestor(CK_o_KEYNOTEXISTS))
      return C_CONTINUE_RECURSION;

    Card_Term *p_term = _pCard_Term(pCard);
    Symbol *pSymbol_term = p_term->pSymbol();

    if ((pCI->E.p_indexJoin = _pCard_IndexJoin(pCI->pAncestor(CK_o_INDEXJOIN))) &&
        (pCI->E.p_constraint = _pCard_Constraint(pCI->pAncestor(CK_s_CONSTRAINT)))) {

      if (!pSymbol_term->bHasFacet(SF_NMD_COL))
        return C_CONTINUE_RECURSION;

      pCI->E.p_emit = pCI->E.p_indexJoin->pSub_emit();
      pCI->E.p_expr = pCI->E.p_emit->pFind_expr_Matches(pSymbol_term);
      pSymbol_term->vApplyFacet(SF_ITM_COL, pCI->E.p_expr->szFacet(SF_ITM_COL));
      return C_CONTINUE_RECURSION;
    }

    if (pSymbol_term->bHasFacet(SF_EXP_COL))
      return C_CONTINUE_RECURSION;
    if (! pSymbol_term->bHasFacet(SF_NMD_COL))
      return C_CONTINUE_RECURSION;

    if (! (pCI->E.p_expr = _pCard_Expr(pCI->pAncestor(CK_EXPR))))
      return C_CONTINUE_RECURSION;

    i8 i_Item;
    if (pCI->pAncestorParent()->bIsCard(CK_DIRECTION))
      i_Item = pCI->pAncestor(-1)->iCounter();
    else
      i_Item = pCI->pAncestorParent()->iCounter();
    pSymbol_term->vApplyFacet(SF_ITM_COL, i_Item);
    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pCI->bLambdaWalk(pCI->E.p_deck, CK_NONE, bfItemise_colNm);

}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* ensure all terms have a type                                               */
/*----------------------------------------------------------------------------*/
void Card_Term::vApplyCast(CardIter* pCI, Card_Emit* p_Dic_expr, Cast& rxCast_last)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);
  UNUSED(p_Dic_expr);

  Symbol* pSym_term = pSymbol();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (pSym_term->bIs_Lit()) {
    rxCast_last.m_xTypeId = pSym_term->xCast().m_xTypeId;
    return;
  }

  if (pSym_term->bHasFacet(SF_COL_TYPE)) {
    Stz* szFacet_colType = pSym_term->szFacet(SF_COL_TYPE);

    if (szFacet_colType->bSame("BOOLEAN"_Stz))
      rxCast_last.m_xTypeId = TypeId::xIdOf(CT_BOOLEAN);
    else
    if (szFacet_colType->bSame("DATE"_Stz))
      rxCast_last.m_xTypeId = TypeId::xIdOf(CT_DATE);
    else
    if (szFacet_colType->bSame("DECIMAL"_Stz)) {
      rxCast_last.m_xTypeId = TypeId::xIdOf(CT_DECIMAL);
      rxCast_last.m_nTypeLength = 19;
      rxCast_last.m_nTypeScale  = 2;
    }
    else
    if (szFacet_colType->bSame("INTEGER"_Stz)) {
      rxCast_last.m_xTypeId = TypeId::xIdOf(CT_INTEGER);
      rxCast_last.m_nTypeLength = 0;
      rxCast_last.m_nTypeScale  = 0;
    }
    else
    if (szFacet_colType->bSame("INTERVAL_DT"_Stz))
      rxCast_last.m_xTypeId = TypeId::xIdOf(CT_INTERVAL_DT);
    else
    if (szFacet_colType->bSame("INTERVAL_YM"_Stz))
      rxCast_last.m_xTypeId = TypeId::xIdOf(CT_INTERVAL_YM);
    else
    if (szFacet_colType->bSame("FLOAT"_Stz))
      rxCast_last.m_xTypeId = TypeId::xIdOf(CT_FLOAT);
    else
    if (szFacet_colType->bSame("TIME"_Stz))
      rxCast_last.m_xTypeId = TypeId::xIdOf(CT_TIME);
    else
    if (szFacet_colType->bSame("TIMESTAMP"_Stz))
      rxCast_last.m_xTypeId = TypeId::xIdOf(CT_TIMESTAMP);
    else
    if (szFacet_colType->bSame("VARCHAR"_Stz))
      rxCast_last.m_xTypeId = TypeId::xIdOf(CT_VARCHAR);
    else
      NEVER_REACH

    return;
  }

  if (pSym_term->bHasFacet(SF_AGG_FN)) {
    Stz* szFacet_aggFn = pSym_term->szFacet(SF_AGG_FN);

    if (szFacet_aggFn->bSame("COUNT"_Stz)) {
      rxCast_last.m_xTypeId = TypeId::xIdOf(CT_INTEGER);
      rxCast_last.m_nTypeLength = 0;
      rxCast_last.m_nTypeScale  = 0;
    }
    else
    if (szFacet_aggFn->bSame("COUNTALL"_Stz)) {
      rxCast_last.m_xTypeId = TypeId::xIdOf(CT_INTEGER);
      rxCast_last.m_nTypeLength = 0;
      rxCast_last.m_nTypeScale  = 0;
    }
    else
    if (szFacet_aggFn->bSame("SUM"_Stz)) {
    }
    else
    if (szFacet_aggFn->bSame("MAX"_Stz)) {
    }
    else
    if (szFacet_aggFn->bSame("MIN"_Stz)) {
    }
    else {
      NEVER_REACH
    }

    return;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (pCI->E.p_key && pCI->E.p_expr && pCI->E.p_rpnCalc) {
    if (! pCI->Dic.m_p_expr)
      return;

    Card_Expr* p_expr_matching = pCI->Dic.m_p_expr->pFind_expr_Matches(pSym_term);
    if (! p_expr_matching)
/**Z_KEY_DIRECTION*/
      p_expr_matching = pCI->E.p_key->pFind_expr_key_Matches(pSym_term);

    if (! p_expr_matching)
      return;

    pSym_term->vApplyCast(p_expr_matching->pSymbol()->xCast());
    rxCast_last = pSym_term->xCast();
    return;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Card_Expr *p_expr_matched = nullptr;
  if (pSym_term->bHasFacet(SF_KEY_COL) && ! pSym_term->bHasFacet(SF_EXP_COL)) {
    p_expr_matched = pCI->E.p_key->pFind_expr_Matches(pSym_term);
  }
  else {
    if (pCI->Dic.m_p_expr)
      p_expr_matched = pCI->Dic.m_p_expr->pFind_expr_Matches(pSym_term);
    if (! p_expr_matched && pCI->E.p_injectable) {
      vDebugPoint();
      p_expr_matched = pCI->E.p_injectable->pFind_expr_Matches(pSym_term);
    }
  }
  if (! p_expr_matched)
    return;

  pSym_term->vApplyCast(p_expr_matched->pSymbol()->xCast());

  if (pSym_term->bValidType())
    rxCast_last = pSym_term->xCast();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Term::vBump_ordCol(eSF eFacet, i8 nBy)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  i8 iKeyCol = pSymbol()->iTo_ordinal(eFacet);

  this->SYM(eFacet,iKeyCol + nBy);

  if (this->pSymbol()->Y_bHas(P_BumpedBy)) {
    i8 nBumpedBy = this->pSymbol()->Y_iGet(P_BumpedBy);
    this->pSymbol()->Y_vSet(P_BumpedBy, nBumpedBy + nBy);
  }
  else {
    this->pSymbol()->Y_vSet(P_BumpedBy, (i8)nBy);
  }
}

/*----------------------------------------------------------------------------*/
/* evaluate term                                                              */
/*----------------------------------------------------------------------------*/
eTE Card_Term::eEvalTerm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (1 == pSymbol()->nFacets()) {
         if (pSymbol()->bHasFacet("and"_Stz)) return TE_AND;
    else if (pSymbol()->bHasFacet("or"_Stz)) return TE_OR;
    else if (pSymbol()->bHasFacet("not"_Stz)) return TE_NOT;
    else if (pSymbol()->bHasFacet("}"_Stz)) return TE_EXECUTE;
    else if (pSymbol()->bHasFacet("exists"_Stz)) return TE_EXISTS;
    else if (pSymbol()->bHasFacet("in"_Stz)) return TE_IN;
    else if (pSymbol()->bHasFacet("notExists"_Stz)) return TE_NOTEXISTS;
    else if (pSymbol()->bHasFacet("notIn"_Stz)) return TE_NOTIN;
  }

       if (pSymbol()->bHasFacet(SF_RLN_OP))     return TE_RELNOP;
  else if (pSymbol()->bHasFacet(SF_BOL_FN))     return TE_BOOLFN;

  else return TE_OTHER;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Term* Card_Term::pClone(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  return _pCard_Term(pDupCardRecursive());
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Term* Card_Term::pClone(Symbol* pDiffSymbol)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_Term* p_term_clone = pClone();

  p_term_clone->pSymbol()->vApplyCastFacets(pDiffSymbol);

  return p_term_clone;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Term::vPrint(Ribbon* pRibbon, i8 iDepth)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  UNUSED(iDepth);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  s32 sType_short = szCT_char[eCTVal()];

  if (! m_xSymbol.nFacets())
    return;

  for (i8 i_Facet = 0; i_Facet < m_xSymbol.nFacets(); i_Facet++) {
    Facet* aFacet = m_xSymbol.aFacet(i_Facet);

    pRibbon->vSprintSnippet(true,"%s~%lld%s%lld"_Stz,
                            aFacet->m_sFacet.azChar(),
                            i_Facet,
                            sType_short.szStz(),
                            m_xSymbol.xCast().m_nTypeLength);
  }
}

/*----------------------------------------------------------------------------*/
/* delete facet if                                                            */
/*----------------------------------------------------------------------------*/
void Card_Term::vDeleteFacetIf(eSF eFacet, eSF eAlsoFacet, char cTblSyn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Symbol* pSym = pSymbol();
  pSym->vDeleteFacetIf(eFacet, eAlsoFacet, cTblSyn);
}

/*----------------------------------------------------------------------------*/
/* correct stmCol                                                             */
/*----------------------------------------------------------------------------*/
void Card_Term::vUpdate_expCol(Card_Emit* p_Dic_expr, bool bAlt)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Symbol* pSym_term = pSymbol();
  if (! pSym_term->bHasFacet(SF_EXP_COL))
    return;

  if (AM::JOB::bHasTag(C_OPTIMISER_TRACE))
    vDebugPoint();

  Card_Expr* p_expr_dic;
  if (bAlt)
    p_expr_dic = p_Dic_expr->pFind_expr_Matches(pSym_term);
  else
    p_expr_dic = p_Dic_expr->pFindExprOfSymbol(pSym_term);

  if (p_expr_dic) {
    pSym_term->vApplyCastFacets(p_expr_dic->pSymbol());
    vMark1(MARK_CHANGED);
  }
}

/*----------------------------------------------------------------------------*/
/* convert term (if stream-column) to table-column                            */
/*----------------------------------------------------------------------------*/
void Card_Term::vX_expCol_To_tblCol(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Symbol* pSymbol_this = this->pSymbol();
  RASSERT(pSymbol_this->bHasFacet(SF_EXP_COL));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (Card_Term* p_term0 = pCI->Flux.m_pArriving->pDecode_expr_term0(pSymbol_this)) {
    Symbol* pSymbol_term0 = p_term0->pSymbol();

    if (pSymbol_term0->bHasFacet(SF_TBL_COL)) {
      pSymbol_this->vDeleteFacet(SF_EXP_COL);
      pSymbol_this->vApplyCastFacets(pSymbol_term0);
      vMark1(MARK_CHANGED);

      if (pCI->E.p_expr && pCI->E.p_expr->bIsScalarCol() && pCI->E.p_rpnCalc) {
        pCI->E.p_expr->pSymbol()->vApplyFacet(SF_TBL_COL, pSymbol_term0->szFacet(SF_TBL_COL));

        if (pSymbol_term0->bHasFacet(SF_NMD_COL))
          pCI->E.p_expr->pSymbol()->vApplyFacet(SF_NMD_COL, pSymbol_term0->szFacet(SF_NMD_COL));

      }

    }
  }
}

/*----------------------------------------------------------------------------*/
/* convert table-column term to stream-column                                 */
/*----------------------------------------------------------------------------*/
void Card_Term::vX_tblCol_To_expCol(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  RASSERT(pCI->E.p_boolPred || pCI->E.p_bound || pCI->E.p_expr || pCI->E.p_hshCol || pCI->E.p_rpnCalc);

  Symbol* pSymbol_this = this->pSymbol();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // ../<key>/<expr>|?|/<rpnCalc>/<term>|?|

  if (pCI->E.p_key && pCI->E.p_expr) {
    Symbol* pSymbol_expr = pCI->E.p_expr->pSymbol();

    if (pSymbol_this->bHasFacet(SF_KEY_COL) &&
        ! pSymbol_expr->bHasFacet(SF_KEY_COL))
      pSymbol_expr->vApplyFacet(SF_KEY_COL,pSymbol_this->szFacet(SF_KEY_COL));
    if (pSymbol_this->bHasFacet(SF_TBL_COL) &&
        ! pSymbol_expr->bHasFacet(SF_TBL_COL))
      pSymbol_expr->vApplyFacet(SF_TBL_COL,pSymbol_this->szFacet(SF_TBL_COL));
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (! pCI->X.m_pExprList_a)
    return;
  if (! pSymbol_this->bHasFacet(SF_TBL_COL))
    return;

  if (pSymbol_this->bHasFacet(SF_EXP_COL))
    return;
  if (pSymbol_this->bHasFacet(SF_KEY_COL)) {
    if (CK_o_GBYAGG == pCI->E.e_Op)
      return;
    if (CK_o_INDEXJOIN == pCI->E.e_Op)
      return;
    if (CK_o_SORT == pCI->E.e_Op)
      return;
  }
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // can scalar expression of this symbol be found in ARRIVED ?

  Symbol xSym_tblCol;
  xSym_tblCol.vApplyFacet(SF_TBL_COL, pSymbol_this->szFacet(SF_TBL_COL));

  if (Card_Expr *p_expr_a = pCI->X.m_pExprList_a->pFind_expr_Matches(&xSym_tblCol)) {

    Stz* szExpCol_expr_a = p_expr_a->szFacet(SF_EXP_COL);
    pSymbol_this->vApplyFacet(SF_EXP_COL, szExpCol_expr_a);
    this->vMark1(MARK_CHANGED);

    return;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // additionally use nmdCol as basis for determining expCol

  if (! pSymbol_this->bHasFacet(SF_NMD_COL))
    return;

  Symbol xSym_nmdCol;
  xSym_nmdCol.vApplyFacet(SF_NMD_COL, pSymbol_this->szFacet(SF_NMD_COL));

  if (Card_Expr *p_expr_scalar_a = pCI->X.m_pExprList_a->pFind_expr_Matches(&xSym_nmdCol)) {
    Stz *szStmCol_expr_a = p_expr_scalar_a->szFacet(SF_EXP_COL);
    pSymbol_this->vApplyFacet(SF_EXP_COL, szStmCol_expr_a);
    this->vMark1(MARK_CHANGED);
    return;
  }


//  if (pCI->X.m_pExprList_i) {
//    if (Card_Expr *p_expr_i = pCI->X.m_pExprList_i->pFind_expr_Matches(&xSym_tblCol)) {
//
//      Stz *szExpCol_expr_a = p_expr_i->szFacet(SF_EXP_COL);
//      pSymbol_this->vApplyFacet(SF_EXP_COL, szExpCol_expr_a);
//      this->vMark1(MARK_CHANGED);
//
//      return;
//    }
//  }


  // not converted
//  if (!pCI->X.m_pExprList_i)
//    pCI->PRINTBOARD_dbg();
//    pSymbol_this->vDump();
//  RASSERT(pCI->X.m_pExprList_i->pFind_expr_Matches(&xSym_tblCol));

  vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/* validate                                                                   */
/*----------------------------------------------------------------------------*/
void Card_Term::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);
  UNUSED(pCI);
  UNUSED(iDepth);
  UNUSED(pParent);

  Card::vValidate_noChildren();

  (void)Card::eValidate_parentEither(pCI, CK_t_HSHCOL,// todo: NO!?
                                     CK_ANDCONJ,
                                     CK_BOUND,
                                     CK_BOOLPRED,
                                     CK_RLNCONJ,
                                     CK_s_HASHING,
                                     CK_NOTCONJ,
                                     CK_ORCONJ,
                                     CK_RPNCALC,
                                     CK_NONE);

  Symbol* pSym_term = pSymbol();
  if (pSym_term->bIs_Col()) {
    if (pSym_term->bHasFacet(SF_EXP_COL)) {
      vDebugPoint();
    }
    if (pSym_term->bHasFacet(SF_ITM_COL)) {
      vDebugPoint();
    }
    if (pSym_term->bHasFacet(SF_KEY_COL)) {
      vDebugPoint();
    }
    if (pSym_term->bHasFacet(SF_NMD_COL)) {
      vDebugPoint();
    }
    if (pSym_term->bHasFacet(SF_TBL_COL)) {
      vDebugPoint();
    }
  }


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (pCI->E.pBoard->pState()->bHasName(aXF[XF_EPILOG_TYPE_CAST])) {
    if (m_xSymbol.bIs_Col() && ! pCI->bAncestor(CK_s_NEEDED)) {
      if (!m_xSymbol.bValidType()) {
         vMark("?"_Stz,"t"_Stz);
         pCI->pAncestorParent()->vMark("?"_Stz,"t"_Stz);
         pCI->PRINTBOARD_dbg();
         vDump(__func__);
         vValidationFailed("Type Missing : %p"_Stz,this);
      }
      if (m_xSymbol.xTypeId() == TypeId::xIdOf(CT_DECIMAL)) {
        if (m_xSymbol.nTypeLength() == 0) {
          vMark("?"_Stz,"l"_Stz);
          pCI->pAncestorParent()->vMark("?"_Stz,"l"_Stz);
          pCI->PRINTBOARD_dbg();
          vDump(__func__);
          vValidationFailed("Decimal length is 0 : %p"_Stz, this);
        }
      }
    }
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//  if (pCI->G.pBoard->pState()->bHasName(aXF[XF_EPILOG_X_TBLCOL_TO_EXPCOL]) ) {
//    i8 i_Ancestor;
//    if ((pCI->G.p_expr = _pCard_Expr(pCI->pAncestor(CK_EXPR, &i_Ancestor)))) {
//      Card* p_ANCESTOR = pCI->pAncestor(i_Ancestor - 1);
//      eCK eAncestor = p_ANCESTOR->eCard();
//      RASSERT(CK_t_EMIT == eAncestor ||
//              CK_BOOLPRED == eAncestor  ||
//              CK_o_FILTER == eAncestor  ||
//              CK_s_KEY == eAncestor  ||
//              CK_s_NEEDED == eAncestor  ||
//              CK_STREAMKEY == eAncestor  ||
//              CK_s_INJECTABLE == eAncestor);
//      Symbol* pSymbol_this = pSymbol();
//      if (CK_s_KEY != eAncestor && CK_STREAMKEY != eAncestor && pCI->G.p_expr->bIsScalarCol()
//           && (pSymbol_this->nFacets() > 1 || ! pSymbol_this->bHasFacet(SF_EXP_COL))
//           ) {
//        Card_EXPRLIST* p_EXPRLIST = (Card_EXPRLIST*)p_ANCESTOR;
//        if (! p_EXPRLIST->bTestFind_expr_Matching(pCI->G.p_expr,this->pSymbol(),SF_EXP_COL)) {
//          pCI->PRINTBOARD_dbg();
//          p_EXPRLIST->vDump(__func__ );
//          pCI->G.p_expr->vDump(__func__ );
//          NEVER_REACH
//        }
//      }
//    }
//  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  vValidationPassed();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
