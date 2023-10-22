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

  auto bfItemise_colNm = [pCI](Card* pCard)
  {
    UNUSED(pCI);

    if (! pCard->bIsCard(CK_TERM))
      return C_CONTINUE_RECURSION;

    if (pCI->pAncestorFind(CK_KEYEXISTS) || pCI->pAncestorFind(CK_KEYNOTEXISTS))
      return C_CONTINUE_RECURSION;
//    if (pCI->pAncestorFind(CK_CONSTRAINT))ø
//      return C_CONTINUE_RECURSION;

    Card_Term* p_term = _pCard_Term(pCard);

    if (p_term->pSymbol()->szSF_chrSeq(SF_NONE)->bSame("O_COMMENT"_Stz))
      vDebugPoint();

    if (! pCard->pSymbol()->bIs_Col_Nm())
      return C_CONTINUE_RECURSION;

    if ( (pCI->G.p_indexJoin  = _pCard_IndexJoin(pCI->pAncestorFind(CK_INDEXJOIN))) &&
         (pCI->G.p_constraint = _pCard_Constraint(pCI->pAncestorFind(CK_CONSTRAINT))) ) {
      //pCI->G.p_indexJoin->vDump(__func__);

      pCI->G.p_emitted  = pCI->G.p_indexJoin->pSub_emitted();
      if ((pCI->G.p_expr = pCI->G.p_emitted->pFind_expr_Via_as_stripItem(p_term->pSymbol()))) {
        pCI->G.p_as = pCI->G.p_expr->pSub_as();
        p_term->pSymbol()->vFacets(pCI->G.p_as->pSymbol());
      }
      return C_CONTINUE_RECURSION;
    }
    else
    if (Card_BoolPred* p_boolPred = _pCard_BoolPred(pCI->pAncestorFind(CK_BOOLPRED))) {
      i8 iItemise = p_boolPred->iItemise();
      p_term->pSymbol()->vFacet(SF_COL_NM_dis, iItemise);
      return C_CONTINUE_RECURSION;
    }
    else
		// getting build error on AWS. ALka changed this : 22/10/23
    //if (Card_RpnCalc* p_rpnCalc = _pCard_RpnCalc(pCI->pAncestorFind(CK_RPNCALC))) {
		if (_pCard_RpnCalc(pCI->pAncestorFind(CK_RPNCALC))) {
      Card_Expr* p_expr = _pCard_Expr(pCI->pAncestorFind(CK_EXPR));
      i8 iItemise = p_expr->iItemise();
      p_term->pSymbol()->vFacet(SF_COL_NM_dis, iItemise);
      return C_CONTINUE_RECURSION;
    }

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

    if (! pCard->pSymbol()->bIs_Col_Nm())
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

  i8 iKeyCol = pSymbol()->iSF_ordinal();

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
void Card_Term::vBump_stmCol(i8 nBy)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  i8 iStmCol = pSymbol()->iSF_ordinal();

 SYM(SF_STM_COL, iStmCol + nBy);

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
void Card_Term::vEnsureTypeFor_stmCol(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_Expr *p_expr_found;

  // rpnCalc is a set of terms
  if (pSymbol()->bHasType())
    return;

  i8 iOdn = iGetOdn();

  if (bIsKeyCol())
    p_expr_found = pCI->G.p_key->pGet_expr(iOdn - 1);
  else
  if (bIsStmCol())
    p_expr_found = pCI->Flux.m_pArriving->pGet_expr(iOdn - 1);
  else
    return;

  Card_RpnCalc *p_rpnCalc_found = p_expr_found->pSub_rpnCalc();

  vColType(p_rpnCalc_found);
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
      vTrace("%s(%lld): unexpected eEvalRelnOp %lld %s\n"_Stz
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

  eRO eRelOp = pSymbol()->eEvalRelnOp();

  return eRelOp;
}

/*----------------------------------------------------------------------------*/
/* evaluate term                                                              */
/*----------------------------------------------------------------------------*/
eTE Card_Term::eEvalTerm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (1 == pSymbol()->nFacets()) {
         if (pSymbol()->bSame("and"_Stz)) return TE_AND;
    else if (pSymbol()->bSame("or"_Stz)) return TE_OR;
    else if (pSymbol()->bSame("not"_Stz)) return TE_NOT;
    else if (pSymbol()->bSame("}"_Stz)) return TE_EXECUTE;
    else if (pSymbol()->bSame("exists"_Stz)) return TE_EXISTS;
    else if (pSymbol()->bSame("in"_Stz)) return TE_IN;
    else if (pSymbol()->bSame("notExists"_Stz)) return TE_NOTEXISTS;
    else if (pSymbol()->bSame("notIn"_Stz)) return TE_NOTIN;
  }

       if (pSymbol()->bIs_Rln_Op())     return TE_RELNOP;
  else if (pSymbol()->bIs_Bol_Fn())     return TE_BOOLFN;

  else return TE_OTHER;
}


/*----------------------------------------------------------------------------*/
/* get ordinal number                                                         */
/*----------------------------------------------------------------------------*/
i8 Card_Term::iGetOdn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  i8 i_Ord = StrLib::iGleanInt(this->szFacet(SF_NONE)->azChar());

  return i_Ord;
}

/*----------------------------------------------------------------------------*/
/* get table synonym c (i.e. $c.n)                                            */
/*----------------------------------------------------------------------------*/
char Card_Term::cGetTblSyn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Stz* szTerm = this->szFacet(SF_NONE);

  char cSyn = szTerm->azChar()[1];

  return cSyn;
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

  p_term_clone->pSymbol()->vFacets(pDiffSymbol);

  return p_term_clone;
}

/*----------------------------------------------------------------------------*/
/* is this term a column-name ?                                               */
/*----------------------------------------------------------------------------*/
bool Card_Term::bIsColNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (pSymbol()->bIs_Col_Nm())
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this term a value ordinal? $n or $c.n                                   */
/*----------------------------------------------------------------------------*/
bool Card_Term::bIsColSym(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (pSymbol()->bHasFacet(SF_STM_COL))
    return true;
  if (pSymbol()->bHasFacet(SF_TBL_COL))
    return true;
  return false;
}

/*----------------------------------------------------------------------------*/
/* is this term a key ordinal? $Kn                                            */
/*----------------------------------------------------------------------------*/
bool Card_Term::bIsKeyCol(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  bool b = pSymbol()->bIs_Key_Col();

  return b;
}

/*----------------------------------------------------------------------------*/
/* is this term a parameter ordinal? %n                                       */
/*----------------------------------------------------------------------------*/
bool Card_Term::bIsSqyPrm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  bool b = pSymbol()->bIs_Sqy_Prm();

  return b;
}

/*----------------------------------------------------------------------------*/
/* is this term a stream-value ordinal? $n                                    */
/*----------------------------------------------------------------------------*/
bool Card_Term::bIsStmCol(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  bool b = pSymbol()->bIs_Stm_Col();

  return b;
}

/*----------------------------------------------------------------------------*/
/* is this term a column-value ordinal? $n.c                                  */
/*----------------------------------------------------------------------------*/
bool Card_Term::bIsTblCol(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (pSymbol()->bIs_Tbl_Col())
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Term::vPrint(Ribbon* pRibbon, i8 iDepth)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  UNUSED(iDepth);

//  if (!pSymbol()->Y_bHas(P_Column)) {
//    vPrintCard_bySettings(pRibbon, iDepth);
//    return;
//  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  s256 sTerm = szFacet(SF_NONE);
  s32 sType_short = szCT_char[pSymbol()->eColType()];

  //pRibbon->vSprintSnippet(true,"%s %s"_Stz,sTerm.szStz(),sType_short.szStz());

  if (m_xSymbol.nFacets()) {
    for (i8 i_Facet = 0; i_Facet < m_xSymbol.nFacets(); i_Facet++) {
      Facet* aFacet = m_xSymbol.aFacet(i_Facet);

      if (aFacet->m_sChrSeq.bSame("$1"_Stz))
        vDebugPoint();
      pRibbon->vSprintSnippet(true,"%s~%lld%s"_Stz,
                              aFacet->m_sChrSeq.azChar(),
                              i_Facet,
                              sType_short.szStz());
    }
  }

  //vPrintCard_bySettings(pRibbon, iDepth);
}

/*----------------------------------------------------------------------------*/
/* correct stmCol                                                             */
/*----------------------------------------------------------------------------*/
void Card_Term::vCorrectStmCol(Card_Emitted* p_emitted_prior)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (!bIsStmCol())
    return;

  Symbol* pSym = pSymbol();
  if (Card_Expr* p_expr = p_emitted_prior->pFindExprOfSymbol(pSym)) {
    pSym->vFacets(p_expr->pSymbol());
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
void Card_Term::vX_stmCol_To_tblCol(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Symbol* pSymbol_this = this->pSymbol();
  RASSERT(pSymbol_this->bIs_Stm_Col());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (Card_Term* p_term0 = pCI->Flux.m_pArriving->pDecode_term0_Same_expr_stmCol(pSymbol_this)) {
    Symbol* pSymbol_term0 = p_term0->pSymbol();

    if (pSymbol_term0->bIs_Tbl_Col()) {
      pSymbol_this->vDeleteFacet(SF_STM_COL);
      pSymbol_this->vFacets(pSymbol_term0);
      vMark1(MARK_CHANGED);
    }
    else {
      eSF eSymbol = pSymbol_this->eFacet();
      switch (eSymbol) {
        case SF_STM_COL:
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
void Card_Term::vX_tblCol_To_stmCol(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Symbol* pSymbol_this = this->pSymbol();
  if (! pSymbol_this->bIs_Tbl_Col())
    return;

  Symbol xTblCol_sym;
  xTblCol_sym.vFacet(SF_TBL_COL,pSymbol_this->szSF_chrSeq(SF_TBL_COL));


  //i8 nCols_a = 0;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // is scalar expression of this symbol in INJECTED ?

  if (pCI->X.m_pExprList_i) {
		// ALKA changed 22/10/23 (compilation error on linux)
    //if (Card_Expr *p_expr_scalar_i = pCI->X.m_pExprList_i->pFind_expr_Via_term0(&xTblCol_sym)) {
		if (pCI->X.m_pExprList_i->pFind_expr_Via_term0(&xTblCol_sym)) {
      this->vMark1(MARK_FOUND);
      return;
    }
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // can scalar expression of this symbol be found in ARRIVED ?

  if (pCI->X.m_pExprList_a) {
    //nCols_a = pCI->X.m_pExprList_a->nChildCards();
    if (Card_Expr *p_expr_scalar_a = pCI->X.m_pExprList_a->pFind_expr_Via_term0(&xTblCol_sym)) {
      Stz* szStmCol_expr_a = p_expr_scalar_a->szFacet(SF_NONE);
      pSymbol_this->vFacet(SF_STM_COL, szStmCol_expr_a);
      this->vMark1(MARK_CHANGED);
      return;
    }
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // can scalar expression of this symbol be found in KEY ?

  if (pCI->X.m_pExprList_a && pCI->X.m_pExprList_k) {
    //pCI->X.m_pExprList_k->vDump(__func__);
    //pSymbol_this->vDump();
    if (Card_Expr *p_expr_scalar_k = pCI->X.m_pExprList_k->pFind_expr_Via_term0(&xTblCol_sym)) {
      Symbol *pSymbol_k = p_expr_scalar_k->pFirstKeyCol();
      if (! pSymbol_k)
        return;
      if (Card_Expr *p_expr_scalar_a = pCI->X.m_pExprList_a->pFind_expr_Via_term0(&xTblCol_sym)) {
        Stz* szStmCol_expr_a = p_expr_scalar_a->szFacet(SF_NONE);
        pSymbol_this->vFacet(SF_STM_COL, szStmCol_expr_a);
        this->vMark1(MARK_CHANGED);
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

  RASSERT(
          pParent->eCard() == CK_HSHCOL || // todo: NO!

          pParent->eCard() == CK_ANDCONJ ||
          pParent->eCard() == CK_BOUND || // todo: is a bound a boolPred?
          pParent->eCard() == CK_BOOLPRED ||
          pParent->eCard() == CK_CMPCONJ ||
          pParent->eCard() == CK_HASHING ||
          pParent->eCard() == CK_NOTCONJ ||
          pParent->eCard() == CK_ORCONJ ||
          pParent->eCard() == CK_RPNCALC);

  if (pCI->G.pBoard->pState()->bHasName(aOH[OH_OPTIMISE])) {
    if (m_xSymbol.bIs_Stm_Col() || m_xSymbol.bIs_Tbl_Col()) {
      if (! m_xSymbol.bHasType()) {
        if (! pCI->G.p_call) {
          vDump(__func__);
          vValidationFailed("Type Missing : %p"_Stz,this);
        }
      }
    }
  }
  else
  if (pCI->G.pBoard->pState()->bHasName(aXF[XF_PROLOG_ANALYSE_EMITTED]) ) {
    if (m_xSymbol.bIs_Tbl_Col()) {
      if (! m_xSymbol.bHasType()) {
        if (! pCI->G.p_call) {
          vDump(__func__);
          vValidationFailed("Type Missing : %p"_Stz,this);
        }
      }
    }
  }


//  if (pCI->G.pBoard->pState()->bHasName(aXF[XF_EPILOG_X_TBLCOL_TO_STMCOL])) {
//    if (m_xSymbol.bIs_Tbl_Col() && ! pCI->pAncestorFind(CK_INJECTABLE)) {
//      vDump(__func__);
//      vValidationFailed("Validation %s : %p"_Stz, aXF[XF_EPILOG_X_TBLCOL_TO_STMCOL], this);
//    }
//  }

  vValidationPassed();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
