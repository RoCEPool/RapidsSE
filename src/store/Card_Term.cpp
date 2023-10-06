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

      pCI->G.p_projection  = pCI->G.p_indexJoin->pSub_projection();
      if ((pCI->G.p_expr = pCI->G.p_projection->pFind_expr_By_as_stripItem(p_term->pSymbol()))) {
        pCI->G.p_as = pCI->G.p_expr->pSub_as();
        p_term->pSymbol()->vFacets(pCI->G.p_as->pSymbol());
      }
      return C_CONTINUE_RECURSION;
    }
    else
    if (Card_BoolCalc* p_boolCalc = _pCard_BoolCalc(pCI->pAncestorFind(CK_BOOLCALC))) {
      i8 iItemise = p_boolCalc->iItemise();
      p_term->pSymbol()->vFacet(SF_COL_NM_dis, iItemise);
      return C_CONTINUE_RECURSION;
    }
    else
    if (Card_RpnCalc* p_rpnCalc = _pCard_RpnCalc(pCI->pAncestorFind(CK_RPNCALC))) {
      i8 iItemise = p_rpnCalc->iItemise();
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
void Card_Term::vBumpKeyOdn(i8 nBy)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  i8 iKeyOdn = pSymbol()->iSF_ordinal();

  this->SYM(SF_KEY_COL,iKeyOdn + nBy);

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
void Card_Term::vBump_stmOdn(i8 nBy)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  i8 iStmColOdn = pSymbol()->iSF_ordinal();

 SYM(SF_COL_NO,iStmColOdn + nBy);

  if (this->pSymbol()->Y_bHas(P_BumpedBy)) {
    i8 nBumpedBy = this->pSymbol()->Y_iGet(P_BumpedBy);
    this->pSymbol()->Y_vSet(P_BumpedBy, nBumpedBy + nBy);
  }
  else {
    this->pSymbol()->Y_vSet(P_BumpedBy, (i8)nBy);
  }
}

/*----------------------------------------------------------------------------*/
/* convert table-column term to stream-column                                 */
/*----------------------------------------------------------------------------*/
void Card_Term::vConvert_colOdn_To_stmOdn(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Symbol* pSymbol_term = this->pSymbol();
  RASSERT(pSymbol_term->bIs_Col_Tc());


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // is the expression synthetic?
  if (Card_Expr* p_expr = _pCard_Expr(pCI->pAncestorFind(CK_EXPR))) {
    if (Card_As* p_as = p_expr->pSub_as()) {
      if (p_as->pSymbol()->Y_bHas(P_Synthetic)) {
        //p_expr->vDump(__func__);
        vDebugPoint();
      }
    }
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (pCI->Prior.m_pProjection) {

    if (Card_Expr *p_expr_col = pCI->Prior.m_pProjection->pFind_expr_Of_symbol(pSymbol_term)) {
      pSymbol_term->vFacet(SF_COL_NO, p_expr_col->szSymbol(SF_NONE));
      vMark1(MARK_CHANGED);
      return;
    }

    if (pCI->G.p_key) {
      if (Card_Expr *p_expr_col_key = pCI->G.p_key->pFind_expr_Of_symbol(pSymbol_term)) {
        Symbol *pSymbol_key = p_expr_col_key->pSymbol();
        RASSERT(pSymbol_key->bIs_Key_Col());

        if (Card_Expr *p_expr_col_ind = pCI->Prior.m_pProjection->pFind_expr_Of_symbol(pSymbol_key)) {
          pSymbol_term->vFacet(SF_COL_NO, p_expr_col_ind->szSymbol(SF_NONE));
          vMark1(MARK_CHANGED);
          return;
        }
      }
    }

    if (pCI->G.p_OP->bIsCard(CK_INDEXJOIN)) {
      if ((pCI->G.p_arriving = pCI->G.p_OP->pSub_arriving())) {
        if (Card_Expr *p_expr_col = pCI->G.p_arriving->pFind_expr_Of_symbol(pSymbol_term)) {
          pSymbol_term->vFacet(SF_COL_NO, p_expr_col->szSymbol(SF_NONE));
          vMark1(MARK_CHANGED);
          return;
        }
      }
    }


  }

  // arrives here as col_tc is not in prior projection
  // probably introduced into this stream by the operator

  i8 nCols_injectedByThisCard;
  pCI->G.p_projection = pCI->G.p_MainCard->pSub_projection();
  if (pCI->G.p_MainCard->bIsCard(CK_SCAN) ||
      pCI->G.p_MainCard->bIsCard(CK_INDEXSCAN)) {
    nCols_injectedByThisCard = 0;

//    RASSERT(this->pSymbol()->bIs_Col_Tc());
//    this->pSymbol()->Y_vSet(P_TblCol, szSymbol(SF_NONE));
//    return;
  }
  else
  if (pCI->G.p_MainCard->bIsCard(CK_CROSSJOIN) ||
      pCI->G.p_MainCard->bIsCard(CK_INDEXJOIN)) {
    nCols_injectedByThisCard = 0;

//    nCols_injectedByThisCard = pCI->Prior.m_pProjection->nChildCards();

//    RASSERT(this->pSymbol()->bIs_Col_Tc());
//    this->pSymbol()->Y_vSet(P_TblCol, szSymbol(SF_NONE));
//    return;
  }
  else
  if (pCI->G.p_MainCard->bIsCard(CK_KEYEXISTS) ||
      pCI->G.p_MainCard->bIsCard(CK_KEYNOTEXISTS)) {
    nCols_injectedByThisCard = pCI->G.p_projection->nChildCards();
  }
  else {
    return;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // term is introduced

  i8 i_Col = this->iGetOdn();
  i8 i_ColOdn = nCols_injectedByThisCard + i_Col;

  Symbol xSymbol_stmColOdn(SF_COL_NO, i_ColOdn);
  this->pSymbol()->vFacets(&xSymbol_stmColOdn);

  this->vMark1(MARK_CHANGED);
}


/*----------------------------------------------------------------------------*/
/* convert term (if stream-column) to table-column                            */
/*----------------------------------------------------------------------------*/
void Card_Term::vConvert_stmOdn_To_colOdn(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Symbol* pSymbol = this->pSymbol();
  RASSERT(pSymbol->bIs_Col_No());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (Card_Expr* p_expr_prior = pCI->Prior.m_pProjection->pDecodeExpr(pSymbol)) {
    Symbol* pSymbol_term0 = p_expr_prior->pGetTerm(0)->pSymbol();

    if (pSymbol_term0->bIs_Col_Tc()) {
      pSymbol->vFacets(pSymbol_term0);
      vMark1(MARK_CHANGED);
    }
    else {
      eSF eSymbol = pSymbol->eFacet();
      switch (eSymbol) {
        case SF_COL_NO:
          break;

        default:
          vDebugPoint();
      }
    }
  }
}

/*----------------------------------------------------------------------------*/
/* ensure all terms have a type                                               */
/*----------------------------------------------------------------------------*/
void Card_Term::vEnsureTypeFor_stmOdn(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_Expr *p_expr_found;

  // rpnCalc is a set of terms
  if (pSymbol()->Y_bHas(P_Type))
    return;

  i8 iOdn = iGetOdn();

  if (bIsKeyOdn())
    p_expr_found = pCI->G.p_key->pGetExpr(iOdn - 1);
  else
  if (bIsStmOdn())
    p_expr_found = pCI->Prior.m_pProjection->pGetExpr(iOdn - 1);
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

       if (pSymbol()->bSame("and"_Stz))        return TE_AND;
  else if (pSymbol()->bSame("or"_Stz))         return TE_OR;
  else if (pSymbol()->bSame("not"_Stz))        return TE_NOT;
  else if (pSymbol()->bSame("}"_Stz))          return TE_EXECUTE;
  else if (pSymbol()->bSame("exists"_Stz))     return TE_EXISTS;
  else if (pSymbol()->bSame("in"_Stz))         return TE_IN;
  else if (pSymbol()->bSame("notExists"_Stz))  return TE_NOTEXISTS;
  else if (pSymbol()->bSame("notIn"_Stz))      return TE_NOTIN;

  else if (pSymbol()->bIs_Rln_Op())     return TE_RELNOP;
  else if (pSymbol()->bIs_Bol_Fn())     return TE_BOOLFN;

  else return TE_OTHER;
}


/*----------------------------------------------------------------------------*/
/* get ordinal number                                                         */
/*----------------------------------------------------------------------------*/
i8 Card_Term::iGetOdn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  i8 i_Ord = StrLib::iGleanInt(this->szSymbol(SF_NONE)->azChar());

  return i_Ord;
}

/*----------------------------------------------------------------------------*/
/* get table synonym c (i.e. $c.n)                                            */
/*----------------------------------------------------------------------------*/
char Card_Term::cGetTblSyn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Stz* szTerm = this->szSymbol(SF_NONE);

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
/* is this term a column-value ordinal? $n.c                                  */
/*----------------------------------------------------------------------------*/
bool Card_Term::bIsColOdn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (pSymbol()->bIs_Col_Tc())
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this term a key ordinal? $Kn                                            */
/*----------------------------------------------------------------------------*/
bool Card_Term::bIsKeyOdn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  bool b = pSymbol()->bIs_Key_Col();

  return b;
}

/*----------------------------------------------------------------------------*/
/* is this term a parameter ordinal? %n                                       */
/*----------------------------------------------------------------------------*/
bool Card_Term::bIsPrmOdn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  bool b = pSymbol()->bIs_Sqy_Prm();

  return b;
}

/*----------------------------------------------------------------------------*/
/* is this term a stream-value ordinal? $n                                    */
/*----------------------------------------------------------------------------*/
bool Card_Term::bIsStmOdn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  bool b = pSymbol()->bIs_Col_No();

  return b;
}

/*----------------------------------------------------------------------------*/
/* is this term a value ordinal? $n or $c.n                                   */
/*----------------------------------------------------------------------------*/
bool Card_Term::bIsValOdn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (bIsStmOdn())
    return true;
  else
  if (bIsColOdn())
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Term::vPrint(Ribbon* pRibbon, i8 iDepth)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (!pSymbol()->Y_bHas(P_Column)) {
    vPrintCard_bySettings(pRibbon, iDepth);
    return;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  s256 sTerm = szSymbol(SF_NONE);
  s32 sType_short = pSymbol()->Y_szColType();

  pRibbon->vSprintSnippet(true,"%s %s"_Stz,sTerm.szStz(),sType_short.szStz());
  if (pSymbol()->Y_bHas(P_TblCol))
    pRibbon->vSprintSnippet(true, "%s %s"_Stz, P_TblCol, pSymbol()->Y_szGet(P_TblCol));
  else
  if (pSymbol()->Y_bHas(P_KeyCol))
    pRibbon->vSprintSnippet(true, "%s %s"_Stz, P_KeyCol, pSymbol()->Y_szGet(P_KeyCol));

  if (m_xSymbol.nFacets()) {
    for (i8 i_Facet = 0; i_Facet < m_xSymbol.nFacets(); i_Facet++) {
      Facet* aFacet = m_xSymbol.aFacet(i_Facet);
      pRibbon->vSprintSnippet(true,"%18s~%lld"_Stz, aFacet->m_sChrSeq.azChar(),i_Facet);
    }
  }

  //vPrintCard_bySettings(pRibbon, iDepth);
}

/*----------------------------------------------------------------------------*/
/* correct stmOdn                                                             */
/*----------------------------------------------------------------------------*/

void Card_Term::vCorrectStmOdn(Card_Projection* p_projection_prior)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (! bIsStmOdn())
    return;

  Symbol* pSym = pSymbol();
  if (Card_Expr* p_expr = p_projection_prior->pFindExprOfSymbol(pSym)) {
    pSym->vFacets(p_expr->pSymbol());
    vMark1(MARK_CHANGED);
  }
}

/*----------------------------------------------------------------------------*/
/* short form                                                                 */
/*----------------------------------------------------------------------------*/
void Card_Term::vShortForm(String* pString, Stz* szSep)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  pString->vCatSprint("%s%s"_Stz,szSep,
                      szSymbol(SF_NONE));
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
          pParent->eCard() == CK_BOUND || // todo: is a bound a boolCalc?
          pParent->eCard() == CK_BOOLCALC ||
          pParent->eCard() == CK_CMPCONJ ||
          pParent->eCard() == CK_HASHING ||
          pParent->eCard() == CK_NOTCONJ ||
          pParent->eCard() == CK_ORCONJ ||
          pParent->eCard() == CK_RPNCALC);

  if (pCI->G.pBoard->pState()->bHasName(aOH[OH_OPTIMISE])) {
    if (m_xSymbol.bIs_Col_No() || m_xSymbol.bIs_Col_Tc()) {
      if (! m_xSymbol.Y_bHas(P_Type)) {
        if (! pCI->G.p_call) {
          vDump(__func__);
          vValidationFailed("Type Missing : %p"_Stz,this);
        }
      }
    }
  }
  else
  if (pCI->G.pBoard->pState()->bHasName(aXF[XF_PROLOG_ANALYSE_PROJECTION]) ) {
    if ( m_xSymbol.bIs_Col_Tc()) {
      if (! m_xSymbol.Y_bHas(P_Type)) {
        if (! pCI->G.p_call) {
          vDump(__func__);
          vValidationFailed("Type Missing : %p"_Stz,this);
        }
      }
    }
  }


//  if (pCI->G.pBoard->pState()->bHasName(aXF[XF_EPILOG_X_TBLCOL_TO_STMCOL])) {
//    if (m_xSymbol.bIs_Col_Tc() && ! pCI->pAncestorFind(CK_INJECTED)) {
//      vDump(__func__);
//      vValidationFailed("Validation %s : %p"_Stz, aXF[XF_EPILOG_X_TBLCOL_TO_STMCOL], this);
//    }
//  }

  vValidationPassed();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
