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

    if (!pCard->bIsCard(CK_TERM))
      return C_CONTINUE_RECURSION;

    if (pCI->pAncestorFind(CK_o_KEYEXISTS) || pCI->pAncestorFind(CK_o_KEYNOTEXISTS))
      return C_CONTINUE_RECURSION;

    Card_Term *p_term = _pCard_Term(pCard);
    Symbol *pSymbol_term = p_term->pSymbol();

    if ((pCI->G.p_indexJoin = _pCard_IndexJoin(pCI->pAncestorFind(CK_o_INDEXJOIN))) &&
        (pCI->G.p_constraint = _pCard_Constraint(pCI->pAncestorFind(CK_s_CONSTRAINT)))) {

      if (!pSymbol_term->bHasFacet(SF_NMD_COL))
        return C_CONTINUE_RECURSION;

      pCI->G.p_emit = pCI->G.p_indexJoin->pSub_emit();
      pCI->G.p_expr = pCI->G.p_emit->pFind_expr_Matches(pSymbol_term);
      i8 iItemise = pCI->G.p_expr->iItemise();
      pSymbol_term->vSetFacet(SF_ITM_COL, iItemise);
      return C_CONTINUE_RECURSION;
    }

    if (pSymbol_term->bHasFacet(SF_EXP_COL) || ! pSymbol_term->bHasFacet(SF_NMD_COL))
      return C_CONTINUE_RECURSION;

    if (! (pCI->G.p_expr = _pCard_Expr(pCI->pAncestorFind(CK_EXPR))))
      return C_CONTINUE_RECURSION;

    i8 iItemise = pCI->G.p_expr->iItemise();
    pSymbol_term->vSetFacet(SF_ITM_COL, iItemise);
    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pCI->bApplyRecursive(pCI->G.pDeck,CK_NONE,bfItemise_colNm);

}
/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Term::vItemStrip_colNm(CardIter* pCI)
{
  _static_

  auto bfItemStrip_colNm = [pCI](Card* pCard)
  {
    UNUSED(pCI);

    if (! pCard->bIsCard(CK_TERM))
      return C_CONTINUE_RECURSION;

    //pCI->vAncestorDump(pCard);

    if (!pCard->pSymbol()->bHasFacet(SF_NMD_COL))
      return C_CONTINUE_RECURSION;

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pCI->bApplyRecursive(pCI->G.pDeck,CK_NONE,bfItemStrip_colNm);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Term::vBumpKeyCol(i8 nBy)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  i8 iKeyCol = pSymbol()->iTo_ordinal();

  this->SYM(SF_KEY_COL,iKeyCol + nBy);

  if (this->pSymbol()->Y_bHas(P_BumpedBy)) {
    i8 nBumpedBy = this->pSymbol()->Y_iGet(P_BumpedBy);
    this->pSymbol()->Y_vSet(P_BumpedBy, nBumpedBy + nBy);
  }
  else {
    this->pSymbol()->Y_vSet(P_BumpedBy, (i8)nBy);
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Term::vBump_expCol(i8 nBy)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  i8 iStmCol = pSymbol()->iTo_ordinal(SF_EXP_COL);

 SYM(SF_EXP_COL, iStmCol + nBy);

  if (this->pSymbol()->Y_bHas(P_BumpedBy)) {
    i8 nBumpedBy = this->pSymbol()->Y_iGet(P_BumpedBy);
    this->pSymbol()->Y_vSet(P_BumpedBy, nBumpedBy + nBy);
  }
  else {
    this->pSymbol()->Y_vSet(P_BumpedBy, (i8)nBy);
  }
}

/*----------------------------------------------------------------------------*/
/* ensure all terms have a type                                               */
/*----------------------------------------------------------------------------*/
void Card_Term::vEnsure_Cast(CardIter* pCI, Cast& rxCast_last)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Symbol* pSymbol_term = pSymbol();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (pSymbol_term->bIs_Lit()) {
    rxCast_last.m_xTypeId = pSymbol_term->xCast().m_xTypeId;
    return;
  }

  if (pSymbol_term->bHasFacet(SF_COL_TYPE)) {
    Stz* szFacet_colType = pSymbol_term->szFacet(SF_COL_TYPE);

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

  if (pSymbol_term->bHasFacet(SF_AGG_FN)) {
    Stz* szFacet_aggFn = pSymbol_term->szFacet(SF_AGG_FN);

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

  if (pCI->G.p_key && pCI->G.p_expr && pCI->G.p_rpnCalc) {
    if (! pCI->Flux.m_pArriving)
      return;

    Card_Expr* p_expr_matching = pCI->Flux.m_pArriving->pFind_expr_Matches(pSymbol_term);
    if (! p_expr_matching)
      p_expr_matching = pCI->G.p_key->pFind_expr_Matches(pSymbol_term);
    if (! p_expr_matching)
      return;

    pSymbol_term->vCast(p_expr_matching->pSymbol()->xCast());
    rxCast_last = pSymbol_term->xCast();
    return;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Card_Expr *p_expr_matched = nullptr;
  if (pSymbol_term->bHasFacet(SF_KEY_COL) && ! pSymbol_term->bHasFacet(SF_EXP_COL)) {
    p_expr_matched = pCI->G.p_key->pFind_expr_Matches(pSymbol_term);
  }
  else {
    if (pCI->Flux.m_pArriving)
      p_expr_matched = pCI->Flux.m_pArriving->pFind_expr_Matches(pSymbol_term);
    if (! p_expr_matched && pCI->G.p_injectable)
      p_expr_matched = pCI->G.p_injectable->pFind_expr_Matches(pSymbol_term);
  }
  if (! p_expr_matched)
    return;

  pSymbol_term->vCast(p_expr_matched->pSymbol()->xCast());

  if (pSymbol_term->bHasType())
    rxCast_last = pSymbol_term->xCast();
}

/*----------------------------------------------------------------------------*/
/* evaluate bound                                                             */
/*----------------------------------------------------------------------------*/
ePB Card_Term::eEvalBound(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  eRO eRelOp = this->eEvalRelOp();

  switch (eRelOp) {
    case RO_LESS_THAN:     return PB_UBOUND;
    case RO_LESS_EQUAL:    return PB_UBOUND;
    case RO_EQUAL_TO:      return PB_EBOUND;
    case RO_GREATER_EQUAL: return PB_LBOUND;
    case RO_GREATER_THAN:  return PB_LBOUND;

    default:
      vTrace("%s(%lld): unexpected eEval_rlnOp %lld %s\n"_Stz
          ,__func__
          ,(i8)__LINE__
          ,(i8)eRelOp
          ,aRO[eRelOp]);
      RASSERT(0);
      break;
  }
}

/*----------------------------------------------------------------------------*/
/* evaluate relational op                                                     */
/*----------------------------------------------------------------------------*/
eRO Card_Term::eEvalRelOp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  eRO eRelOp = pSymbol()->eEval_rlnOp();

  return eRelOp;
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

  p_term_clone->pSymbol()->vCastFacets(pDiffSymbol);

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

  s256 sTerm = szFacet(SF_NONE);
  s32 sType_short = szCT_char[eCTVal()];

  //pRibbon->vSprintSnippet(true,"%s %s"_Stz,sTerm.szStz(),sType_short.szStz());

  if (m_xSymbol.nFacets()) {
    for (i8 i_Facet = 0; i_Facet < m_xSymbol.nFacets(); i_Facet++) {
      Facet* aFacet = m_xSymbol.aFacet(i_Facet);

      if (aFacet->m_sChrSeq.bSame("$1"_Stz))
        vDebugPoint();
      pRibbon->vSprintSnippet(true,"%s~%lld%s%lld"_Stz,
                              aFacet->m_sChrSeq.azChar(),
                              i_Facet,
                              sType_short.szStz(),
                              m_xSymbol.xCast().m_nTypeLength);
    }
  }

  //vPrintCard_bySettings(pRibbon, iDepth);
}

/*----------------------------------------------------------------------------*/
/* correct stmCol                                                             */
/*----------------------------------------------------------------------------*/
void Card_Term::vCorrect_expCol(Card_Emit* p_arriving)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (! pSymbol()->bHasFacet(SF_EXP_COL))
     return;
  if (pSymbol()->bHasFacet(SF_KEY_COL))
    return;

  Symbol* pSym = pSymbol();
  if (Card_Expr* p_expr = p_arriving->pFindExprOfSymbol(pSym)) {
    pSym->vCastFacets(p_expr->pSymbol());
    vMark1(MARK_CHANGED);
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
/* short form                                                                 */
/*----------------------------------------------------------------------------*/
void Card_Term::vShortForm(String* pString, Stz* szSep)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  pString->vCatSprint("%s%s"_Stz, szSep,
                      szFacet(SF_NONE));
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
      pSymbol_this->vCastFacets(pSymbol_term0);
      vMark1(MARK_CHANGED);

      if (pCI->G.p_expr && pCI->G.p_expr->bIsScalarCol() && pCI->G.p_rpnCalc) {
//        if (! pCI->G.p_expr->pSymbol()->bHasFacet(SF_TBL_COL))
//          pCI->G.p_expr->pSymbol()->vSetFacet(SF_TBL_COL,pSymbol_term0->szFacet(SF_TBL_COL));
        pCI->G.p_expr->pSymbol()->vSetFacet(SF_TBL_COL,pSymbol_term0->szFacet(SF_TBL_COL));

        if (pSymbol_term0->bHasFacet(SF_NMD_COL))
          pCI->G.p_expr->pSymbol()->vSetFacet(SF_NMD_COL,pSymbol_term0->szFacet(SF_NMD_COL));

      }

    }
    else {
      eSF eSymbol = pSymbol_this->eFacet();
      switch (eSymbol) {
        case SF_EXP_COL:
          break;

        default:
          vDebugPoint();
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

  RASSERT(pCI->G.p_boolPred || pCI->G.p_bound || pCI->G.p_expr || pCI->G.p_hshCol || pCI->G.p_rpnCalc);

  Symbol* pSymbol_this = this->pSymbol();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // ../<key>/<expr>|?|/<rpnCalc>/<term>|?|

  if (pCI->G.p_key && pCI->G.p_expr) {
    Symbol* pSymbol_expr = pCI->G.p_expr->pSymbol();

//    if (pSymbol_this->bHasFacet(SF_EXP_COL) && ! pSymbol_expr->bHasFacet(SF_EXP_COL))
//      pSymbol_expr->vSetFacet(SF_EXP_COL,pSymbol_this->szFacet(SF_EXP_COL));
    if (pSymbol_this->bHasFacet(SF_KEY_COL) && ! pSymbol_expr->bHasFacet(SF_KEY_COL))
      pSymbol_expr->vSetFacet(SF_KEY_COL,pSymbol_this->szFacet(SF_KEY_COL));
    if (pSymbol_this->bHasFacet(SF_TBL_COL) && ! pSymbol_expr->bHasFacet(SF_TBL_COL))
      pSymbol_expr->vSetFacet(SF_TBL_COL,pSymbol_this->szFacet(SF_TBL_COL));
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (pSymbol_this->bHasFacet(SF_EXP_COL))
    return;

  if (! pSymbol_this->bHasFacet(SF_TBL_COL))
    return;

  if ((CK_o_GBYAGG != pCI->G.e_MainCard) ||
          (CK_o_GBYAGG == pCI->G.e_MainCard && (1 == pSymbol_this->nFacets() || pSymbol_this->bHasFacet(SF_NMD_COL)))) {

  }
  else {
    return;
  }


  Symbol xTblCol_sym;
  xTblCol_sym.vFacet(SF_TBL_COL,pSymbol_this->szFacet(SF_TBL_COL));


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // is scalar expression of this symbol in INJECTED ?

  if (pCI->X.m_pExprList_i) {
		// ALKA changed 22/10/23 (compilation error on linux)
   	if (pCI->X.m_pExprList_i->pFind_expr_Matches_ignoring(&xTblCol_sym, SF_EXP_COL)) {
      this->vMark1(MARK_FOUND);
      return;
    }
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // can scalar expression of this symbol be found in ARRIVED ?

  if (pCI->X.m_pExprList_a) {
    //pCI->X.m_pExprList_a->vDump(__func__);
    if (Card_Expr *p_expr_scalar_a = pCI->X.m_pExprList_a->pFind_expr_Matches(&xTblCol_sym)) {

      Stz* szStmCol_expr_a = p_expr_scalar_a->szFacet(SF_EXP_COL);
      pSymbol_this->vConcatFacet(SF_EXP_COL, szStmCol_expr_a);
      this->vMark1(MARK_CHANGED);

      if (pCI->G.p_key && pCI->G.p_expr) {
        if (! pCI->G.p_expr->pSymbol()->bHasFacet(SF_EXP_COL))
          pCI->G.p_expr->pSymbol()->vSetFacet(SF_EXP_COL,szStmCol_expr_a);
      }

      return;
    }
    if (Card_Expr *p_expr_scalar_a = pCI->Flux.m_pArriving->pFind_expr_Matches(&xTblCol_sym)) {
      Stz* szStmCol_expr_a = p_expr_scalar_a->szFacet(SF_EXP_COL);
      pSymbol_this->vConcatFacet(SF_EXP_COL, szStmCol_expr_a);
      this->vMark1(MARK_CHANGED);
      if (pCI->G.p_key && pCI->G.p_expr) {
        pCI->G.p_expr->pSymbol()->vSetFacet(SF_EXP_COL,szStmCol_expr_a);
      }
      return;
    }

    if (pSymbol_this->bHasFacet(SF_NMD_COL)) {
      Symbol xAsNm_sym;
      xAsNm_sym.vSetFacet(SF_NMD_COL, pSymbol_this->szFacet(SF_NMD_COL));

      if (Card_Expr *p_expr_scalar_a = pCI->Flux.m_pArriving->pFind_expr_Matches(&xAsNm_sym)) {
        Stz *szStmCol_expr_a = p_expr_scalar_a->szFacet(SF_EXP_COL);
        pSymbol_this->vConcatFacet(SF_EXP_COL, szStmCol_expr_a);
        this->vMark1(MARK_CHANGED);
        if (pCI->G.p_key && pCI->G.p_expr) {
          if (! pCI->G.p_expr->pSymbol()->bHasFacet(SF_EXP_COL))
            pCI->G.p_expr->pSymbol()->vSetFacet(SF_EXP_COL,szStmCol_expr_a);
        }
        return;
      }
    }
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // can scalar expression of this symbol be found in KEY ?

  if (pCI->X.m_pExprList_a && pCI->X.m_pExprList_k) {
    //pCI->X.m_pExprList_k->vDump(__func__);
    //pSymbol_this->vDump();
    if (Card_Expr *p_expr_scalar_k = pCI->X.m_pExprList_k->pFind_expr_Matches(&xTblCol_sym)) {
      Symbol *pSymbol_k = p_expr_scalar_k->pFirst_keyCol();
      if (! pSymbol_k)
        return;
      if (Card_Expr *p_expr_scalar_a = pCI->X.m_pExprList_a->pFind_expr_Matches_ignoring(&xTblCol_sym, SF_EXP_COL)) {
        Stz* szStmCol_expr_a = p_expr_scalar_a->szFacet(SF_EXP_COL);
        pSymbol_this->vConcatFacet(SF_EXP_COL, szStmCol_expr_a);
        this->vMark1(MARK_CHANGED);
        if (pCI->G.p_key && pCI->G.p_expr) {
          pCI->G.p_expr->pSymbol()->vSetFacet(SF_EXP_COL,szStmCol_expr_a);
        }
      }
      return;
    }
  }

  // not converted
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

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (pCI->G.pBoard->pState()->bHasName(aXF[XF_EPILOG_TYPE_CAST])) {
    if (m_xSymbol.bIs_Col() && ! pCI->pAncestorFind(CK_s_NEEDED)) {
      if (! m_xSymbol.bHasType()) {
         vMark("?"_Stz,"t"_Stz);
         pCI->pAncestorParent()->vMark("?"_Stz,"t"_Stz);
         pCI->vPrintBoard(__func__,__LINE__,true);
         vDump(__func__);
         vValidationFailed("Type Missing : %p"_Stz,this);
      }
      if (m_xSymbol.xTypeId() == TypeId::xIdOf(CT_DECIMAL)) {
        if (m_xSymbol.nTypeLength() == 0) {
          vMark("?"_Stz,"l"_Stz);
          pCI->pAncestorParent()->vMark("?"_Stz,"l"_Stz);
          pCI->vPrintBoard(__func__,__LINE__,true);
          vDump(__func__);
          vValidationFailed("Decimal length is 0 : %p"_Stz, this);
        }
      }
    }
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (pCI->G.pBoard->pState()->bHasName(aXF[XF_EPILOG_X_TBLCOL_TO_EXPCOL]) ) {
    i8 i_Ancestor;
    if ((pCI->G.p_expr = _pCard_Expr(pCI->pAncestorFind(CK_EXPR,&i_Ancestor)))) {
      Card* p_ANCESTOR = pCI->pAncestor(i_Ancestor - 1);
      eCK eAncestor = p_ANCESTOR->eCard();
      RASSERT(CK_t_EMIT == eAncestor ||
              CK_s_KEY == eAncestor  ||
              CK_s_NEEDED == eAncestor  ||
              CK_STREAMKEY == eAncestor  ||
              CK_s_INJECTABLE == eAncestor);
      Symbol* pSymbol_this = pSymbol();
      if (CK_s_KEY != eAncestor && CK_STREAMKEY != eAncestor && pCI->G.p_expr->bIsScalarCol()
           && (pSymbol_this->nFacets() > 1 || ! pSymbol_this->bHasFacet(SF_EXP_COL))
           ) {
        Card_EXPRLIST* p_EXPRLIST = (Card_EXPRLIST*)p_ANCESTOR;
        if (! p_EXPRLIST->bTestFind_expr_Matching(pCI->G.p_expr,this->pSymbol(),SF_EXP_COL)) {
          pCI->vPrintBoard(__func__,__LINE__,true);
          p_EXPRLIST->vDump(__func__ );
          pCI->G.p_expr->vDump(__func__ );
          NEVER_REACH
        }
      }
    }
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  vValidationPassed();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
