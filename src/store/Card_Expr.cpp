/*============================================================================*/
/* Card_Expr.cpp -- Typed Expression                                          */
/*============================================================================*/

#include "Card_Expr.hpp"

#include "Card_As.hpp"
#include "Card_RpnCalc.hpp"
#include "Card_BoolPred.hpp"
#include "CardIter.hpp"

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
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Expr::Card_Expr(Symbol* pSymbol)
: Card_symbol(CK_EXPR, C_UNKNOWN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_RpnCalc* p_rpnCalc = new Card_RpnCalc(pSymbol);
  vAppendChild(p_rpnCalc);

  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Expr::Card_Expr(Card_As*      p_as_arg,
                     Card_Term*    p_term_0_arg)
: Card_symbol(CK_EXPR, C_UNKNOWN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_As  *    p_as      = (new Card_As) ->SYM(SF_NMD_COL, p_as_arg->szFacet(SF_NONE));
  Card_RpnCalc* p_rpnCalc = new Card_RpnCalc;
  Card_Term*    p_term    = p_term_0_arg->pClone();

  vAppendChild(p_as);


  vAppendChild(p_rpnCalc);
  p_rpnCalc->vAppendChild(p_term);
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
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Expr* Card_Expr::pGby_expr_From_expr_corr(Card_Expr* pExpr_corr)
{
  _static_

  RASSERT(CT_NONE != pExpr_corr->eColType());

  Card_Expr*    p_expr_gby = new Card_Expr;
  Card_RpnCalc* p_rpnCalc = new Card_RpnCalc;
  Card_Term*    p_term    = pExpr_corr->pSub_term()->pClone();

  p_expr_gby->vAppendChild(p_rpnCalc);
  p_rpnCalc->vAppendChild(p_term);

  p_rpnCalc->pSymbol()->vColType(pExpr_corr->eColType());
  p_term->pSymbol()->vColType(pExpr_corr->eColType());
  p_expr_gby->pSymbol()->vColType(pExpr_corr->eColType());

  return p_expr_gby;
}

/*----------------------------------------------------------------------------*/
/* form project expression from gby key expression                            */
/*----------------------------------------------------------------------------*/
Card_Expr* Card_Expr::pProj_expr_From_Gby_expr(Card_Expr* p_expr_gby, Stz* szAs)
{
  _static_

  RASSERT(CT_NONE != p_expr_gby->eColType());

  Symbol xSymbol_Kn(SF_KEY_COL,p_expr_gby->i_Child() + 1);

  Card_Expr*    p_expr    = new Card_Expr;
  Card_As*      p_as      = szAs ? (new Card_As) ->SYM(SF_NMD_COL,szAs) : (new Card_As);
  Card_RpnCalc* p_rpnCalc = new Card_RpnCalc;
  Card_Term*    p_term    = p_expr_gby->pSub_term()->pClone(&xSymbol_Kn);

  if (p_as)
    p_expr->vAppendChild(p_as);
  p_expr->vAppendChild(p_rpnCalc);
  p_rpnCalc->vAppendChild(p_term);

  p_rpnCalc->pSymbol()->vColType(p_expr_gby->eColType());
  p_term->pSymbol()->vColType(p_expr_gby->eColType());
  p_expr->pSymbol()->vColType(p_expr_gby->eColType());

  return p_expr;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Expr* Card_Expr::pProj_Simple_expr_Of(Card_Expr* p_expr, i8 i_Col)
{
  _static_

  //p_expr->vDump();

  RASSERT(CT_NONE != p_expr->eColType());

  Symbol xSymbol_Dn(SF_STM_COL, i_Col);

  Card_Expr* p_expr_simple = new Card_Expr;

  Card_RpnCalc* p_rpnCalc = new Card_RpnCalc;
  p_rpnCalc->pSymbol()->vColType(p_expr->eColType());

  Card_Term* p_term = p_expr->pSub_term()->pClone(&xSymbol_Dn);
  p_term->pSymbol()->vColType(p_expr->eColType());
  //p_term->vDump();

  p_rpnCalc->vAppendChild(p_term);


  p_expr_simple->vAppendChild(p_rpnCalc);
  p_expr_simple->vColType(p_rpnCalc);

  p_expr_simple->vAppendChild(p_expr->pSub_as()->pDupCardRecursive());

  return p_expr_simple;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Expr* Card_Expr::pProj_Simple_expr_Of_key(Card_Expr* p_expr_key)
{
  _static_
  UNUSED(p_expr_key);
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Expr::vBump_stmCol(i8 nBumpBy, bool bSkipFoldExpr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (Card_BoolPred* p_boolPred = pSub_boolPred())
    p_boolPred->vBump_stmCol(nBumpBy, bSkipFoldExpr);
  else
  if (Card_RpnCalc* p_rpnCalc = pSub_rpnCalc())
    p_rpnCalc->vBump_stmCol(nBumpBy, bSkipFoldExpr);

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
eCT Card_Expr::eColumnType(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (Card_RpnCalc* p_rpnCalc = pSub_rpnCalc()) {

    eCT eColType = p_rpnCalc->eColumnType();

    return eColType;
  }

  return CT_NONE;
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
Card_Expr* Card_Expr::pCraft_expr_early(CardIter* pCI, i8 i_Expr)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);
  UNUSED(pCI);

  /*
   * produce the projection of this expression
   */

  Card_As*      p_as;
  Card_RpnCalc* p_rpnCalc;
  Card_Term*    p_term_0;

  vDeconstruct(p_as,p_term_0);

  p_rpnCalc = pSub_rpnCalc();

  Card_Expr* p_expr_early = new Card_Expr;

  if (p_as)
    p_expr_early->vAppendChild(p_as->pClone());
  else {
    Card_As* p_as_new = (new Card_As) ->SYM(SF_NMD_COL,i_Expr);
    p_expr_early->vAppendChild(p_as_new);
  }

  Card_RpnCalc* p_rpnCalc_early = _pCard_RpnCalc(p_rpnCalc->pDupCardRecursive());
  p_expr_early->vAppendChild(p_rpnCalc_early);

  p_expr_early->vColType(p_rpnCalc_early);
  return p_expr_early;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Expr::vDeconstruct(Card_As*&      rp_as,
                             Card_Term*&    rp_term_0)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  rp_as   = pSub_as();
  Card_RpnCalc* p_rpnCalc = pSub_rpnCalc();
  rp_term_0  = p_rpnCalc ? p_rpnCalc->pSub_term() : nullptr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Expr::vDeleteFacetIf(eSF eFacet, eSF eAlsoFacet, char cTblSyn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (Card_BoolPred* p_boolPred = pSub_boolPred())
    p_boolPred->vDeleteFacetIf(eFacet, eAlsoFacet, cTblSyn);
  else
  if (Card_RpnCalc* p_rpnCalc = pSub_rpnCalc())
    p_rpnCalc->vDeleteFacetIf(eFacet, eAlsoFacet, cTblSyn);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Expr::vEnsureTypes_For_stmCol(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  pCI->G.p_rpnCalc = pSub_rpnCalc();

  pCI->G.p_rpnCalc->vEnsureTypesFor_stmCol(pCI);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Expr::vFactorAll_ColSym_IntoDistinct_expr(CardIter *pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  RASSERT(! pCI->G.p_subSet);

  Card_SubSet* p_subSet = new Card_SubSet;

  // an expr is an rpnCalc (shouldn't be boolPred)
  if ( (pCI->G.p_rpnCalc = pSub_rpnCalc()) ) {

    pCI->G.p_rpnCalc->vFactorAll_ColSym_IntoDistinct_expr(pCI);
    p_subSet->vTransferChildren(pCI->G.p_subSet);

    DELETE_CARD(pCI->G.p_subSet);
  }

  pCI->G.p_subSet = p_subSet;
}

/*----------------------------------------------------------------------------*/
/* get first col symbol                                                       */
/*----------------------------------------------------------------------------*/
Symbol* Card_Expr::pFirstColSym(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Symbol* p_colSym = pSub_rpnCalc()->pFirstColSym();

  return p_colSym;
}

/*----------------------------------------------------------------------------*/
/* get first key col                                                          */
/*----------------------------------------------------------------------------*/
Symbol* Card_Expr::pFirstKeyCol(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Symbol* p_keyCol = pSub_rpnCalc()->pFirstKeyCol();

  return p_keyCol;
}

/*----------------------------------------------------------------------------*/
/* get first stm col                                                          */
/*----------------------------------------------------------------------------*/
Symbol* Card_Expr::pFirstStmCol(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Symbol* p_stmCol = pSub_rpnCalc()->pFirstStmCol();

  return p_stmCol;
}

/*----------------------------------------------------------------------------*/
/* get first tbl col                                                          */
/*----------------------------------------------------------------------------*/
Symbol* Card_Expr::pFirstTblCol(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Symbol* p_tblCol = pSub_rpnCalc()->pFirstTblCol();

  return p_tblCol;
}

/*----------------------------------------------------------------------------*/
/* get symbol of given term                                                   */
/*----------------------------------------------------------------------------*/
Symbol* Card_Expr::pGet_Symbol(i8 i_Term)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_Term* p_term_i = pSub_rpnCalc()->pGetTerm(i_Term);
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

  Card_Term* p_term_i = pSub_rpnCalc()->pGetTerm(i_Term);

  return p_term_i;
}

/*----------------------------------------------------------------------------*/
/* get term 0                                                                 */
/*----------------------------------------------------------------------------*/
Card_Term* Card_Expr::pGet_term0(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_Term* p_term0 = pSub_rpnCalc()->pGetTerm(0);

  return p_term0;
}

/*----------------------------------------------------------------------------*/
/* is this expression an aggregate function ?                                 */
/*----------------------------------------------------------------------------*/
bool Card_Expr::bIsAggCalc(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (Card_RpnCalc* p_rpnCalc = pSub_rpnCalc()) {
    return p_rpnCalc->bIsAggCalc();
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_Expr::bIsDescending(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (Card_Direction* p_direction = pSub_direction())
    if (p_direction->pSymbol()->bSame("DESC"_Stz))
      return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this an expression of a key                                             */
/*----------------------------------------------------------------------------*/
bool Card_Expr::bIsExpressionOfKey(void) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Symbol *pSymbol_0 = pGet_Symbol(0);

  if (pSymbol_0->bIs_Key_Col())
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a compound expr?                                                   */
/*----------------------------------------------------------------------------*/
bool Card_Expr::bIsCompound(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  bool bCompound = false;
  if (Card_BoolPred* p_boolPred = pSub_boolPred()) {
    bCompound = p_boolPred->bIsCompound();
  }
  else
  if (Card_RpnCalc* p_rpnCalc = pSub_rpnCalc()) {
    bCompound = p_rpnCalc->bIsCompound();
  }

  return bCompound;
}

/*----------------------------------------------------------------------------*/
/* is this a literals expr?                                                   */
/*----------------------------------------------------------------------------*/
bool Card_Expr::bIsLiterals(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  bool bCompound = false;
  if (Card_BoolPred* p_boolPred = pSub_boolPred()) {
    bCompound = p_boolPred->bIsLiterals();
  }
  else
  if (Card_RpnCalc* p_rpnCalc = pSub_rpnCalc()) {
    bCompound = p_rpnCalc->bIsLiterals();
  }

  return bCompound;
}

/*----------------------------------------------------------------------------*/
/* is this expr a simple expression of a column?                              */
/*----------------------------------------------------------------------------*/
bool Card_Expr::bIsScalarCol(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  bool bScalarCol = false;
  if (Card_BoolPred* p_boolPred = pSub_boolPred()) {
    bScalarCol = p_boolPred->bIsScalarCol();
  }
  else
  if (Card_RpnCalc* p_rpnCalc = pSub_rpnCalc()) {
    bScalarCol = p_rpnCalc->bIsScalarCol();
  }

  return bScalarCol;
}

/*----------------------------------------------------------------------------*/
/* is this expr a simple expression of a key?                                 */
/*----------------------------------------------------------------------------*/
bool Card_Expr::bIsScalarKey(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  bool bScalarKey = false;
  if (Card_BoolPred* p_boolPred = pSub_boolPred()) {
    bScalarKey = p_boolPred->bIsScalarKey();
  }
  else
  if (Card_RpnCalc* p_rpnCalc = pSub_rpnCalc()) {
    bScalarKey = p_rpnCalc->bIsScalarKey();
  }

  return bScalarKey;
}

/*----------------------------------------------------------------------------*/
/* is this an expression of given symbol ?                                    */
/*----------------------------------------------------------------------------*/
bool Card_Expr::bMatching_term0(Symbol* pSymbol_arg, eSF* aeFacet)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Symbol* pSymbol_0 = pGet_term(0)->pSymbol();

  if (pSymbol_0->bMatches(pSymbol_arg,aeFacet))
    return true;
  return false;
}

/*----------------------------------------------------------------------------*/
/* does expression have term ?                                                */
/*----------------------------------------------------------------------------*/
bool Card_Expr::bHasTerm(Symbol* pSymbol)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  bool bHas = false;

  if (Card_BoolPred* p_boolPred = pSub_boolPred())
    bHas = p_boolPred->bHasTerm(pSymbol);
  else
  if (Card_RpnCalc* p_rpnCalc = pSub_rpnCalc())
    bHas = p_rpnCalc->bHasTerm(pSymbol);

  return bHas;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Expr::vPrint(Ribbon* pRibbon, i8 iDepth)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  s32 sType_short = szCT_char[pSymbol()->eColType()];

  pRibbon->vSprintSnippet(false, "%c%c%s%lld]%s %s %s"_Stz,
                          cMark1(),
                          cMark2(),
                          Stz::szDots(iDepth - 1),
                          m_i_Child,
                          szCardNm(),
                          szFacet(SF_NONE),
                          sType_short.szStz());
}

/*----------------------------------------------------------------------------*/
/* craft propagated version of this expression                                */
/*----------------------------------------------------------------------------*/
Card_Expr* Card_Expr::pPropagate(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_As*      p_as;
  Card_RpnCalc* p_rpnCalc;
  Card_Term*    p_term_0;

  vDeconstruct(p_as,p_term_0);

  p_rpnCalc = pSub_rpnCalc();

  Card_Expr* p_expr_propagation = new Card_Expr;

  //vDump(__func__);
  p_expr_propagation->vColType(this);

  if (p_as)
    p_expr_propagation->vAppendChild(p_as->pClone());
  else {
    Card_As* p_as_new = (new Card_As) ->SYM(SF_NMD_COL, this->szFacet(SF_NONE));
    p_expr_propagation->vAppendChild(p_as_new);
  }



//  Card_RpnCalc* p_rpnCalc_early = _pCard_RpnCalc(p_rpnCalc->pDupCardRecursive());
//  p_expr_propagation->vAppendChild(p_rpnCalc_early);
//  p_expr_propagation->vSetType(p_rpnCalc_early);

  Card_RpnCalc* p_rpnCalc_propagated = p_rpnCalc->pPropagate(this);

  p_expr_propagation->vAppendChild(p_rpnCalc_propagated);

  return p_expr_propagation;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Expr::vReconstruct(Card_As*    p_as_arg,
                             Card_Term*  p_term_0_arg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  vDeleteAllChildren();

  Card_RpnCalc* p_rpnCalc = new Card_RpnCalc;
  Card_Term*    p_term    = p_term_0_arg->pClone();

  vAppendChild(p_as_arg);
  vAppendChild(p_rpnCalc);
  p_rpnCalc->vAppendChild(p_term);
}

/*----------------------------------------------------------------------------*/
/* return a reduced copy of this expr                                         */
/*----------------------------------------------------------------------------*/
Card_Expr * Card_Expr::pReducedCopy(i8 i_Expr)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Symbol xExprNo(SF_STM_COL, i_Expr + 1);

  Card_Expr* p_expr_reduced = new Card_Expr;
  p_expr_reduced->pSymbol()->vFacets(&xExprNo);
  p_expr_reduced->vColType(this);

  bool bIs_SQCRLKEY = false;
  if (Card_As* p_as = pSub_as()) {
    p_expr_reduced->vAppendChild(p_as->pClone());
    bIs_SQCRLKEY = p_as->bIs_SQCRLKEY();
  }
  Card_RpnCalc* p_rpnCalc_reduced;
  if (bIsScalarKey())
    p_rpnCalc_reduced = pSub_rpnCalc()->pReducedCopy_key();
  else
    p_rpnCalc_reduced = pSub_rpnCalc()->pReducedCopy(i_Expr,bIs_SQCRLKEY);

  p_expr_reduced->vAppendChild(p_rpnCalc_reduced);

  return p_expr_reduced;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Expr::vRenumberForm_KeyCol(BASE::i8 i_Expr)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);


  SYM(SF_KEY_COL,i_Expr);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Expr::vRenumberForm_stmCol(BASE::i8 i_Expr)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  SYM(SF_STM_COL, i_Expr);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_Expr::bSameAs(Card_Expr* p_expr_arg)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (eColType() != p_expr_arg->eColType())
    return false;

  Card_RpnCalc* p_rpnCalc_this = this->pSub_rpnCalc();
  Card_RpnCalc* p_rpnCalc_arg  = p_expr_arg->pSub_rpnCalc();
  if (!p_rpnCalc_this->bSameAs(p_rpnCalc_arg))
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/* short form                                                                 */
/*----------------------------------------------------------------------------*/
void Card_Expr::vShortForm(String* pString, Stz* szSep)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (Card_BoolPred* p_boolPred = pSub_boolPred())
    p_boolPred->vShortForm(pString);
  else
  if (Card_RpnCalc* p_rpnCalc = pSub_rpnCalc())
    p_rpnCalc->vShortForm(pString,szSep);

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
eCT Card_Expr::eColType(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  eCT eColType = CT_NONE;

  if (Card_BoolPred* p_boolPred = pSub_boolPred())
    eColType = p_boolPred->eColType();
  else
  if (Card_RpnCalc* p_rpnCalc = pSub_rpnCalc())
    eColType = p_rpnCalc->eColType();

  return eColType;
}

/*----------------------------------------------------------------------------*/
/* convert stmCol to tblCol                                                   */
/*----------------------------------------------------------------------------*/
void Card_Expr::vX_stmCol_To_tblCol(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (Card_BoolPred* p_boolPred = pSub_boolPred())
    p_boolPred->vX_stmCol_To_tblCol(pCI);
  else
  if (Card_RpnCalc* p_rpnCalc = pSub_rpnCalc())
    p_rpnCalc->vX_stmCol_To_tblCol(pCI);
}

/*----------------------------------------------------------------------------*/
/* convert tblCol to stmCol                                                   */
/*----------------------------------------------------------------------------*/
void Card_Expr::vX_tblCol_To_stmCol(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  i8 nAncestors = pCI->nAncestors();
  pCI->vAncestorPush(this);

  if (Card_BoolPred* p_boolPred = pSub_boolPred())
    p_boolPred->vX_tblCol_To_stmCol(pCI);
  else
  if (Card_RpnCalc* p_rpnCalc = pSub_rpnCalc())
    p_rpnCalc->vX_tblCol_To_stmCol(pCI);

  pCI->vAncestorPop();
  RASSERT(pCI->nAncestors() == nAncestors);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Expr::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (! pSymbol()->bHasType()) {
    vDump(__func__);
    RASSERT(0);
  }

  // all non-key expr must have an AS
  if (pParent && ! pParent->bIsCard(CK_KEY)
              && ! pParent->bIsCard(CK_STREAMKEY)
              && ! pParent->bIsCard(CK_NEEDED)) {
    vValidate_mustHave(CK_AS,CK_NONE);
  }

  UNUSED(pParent);
  Card::vValidate(pCI,iDepth,pParent);
  pCI->vValidateChildren(this,iDepth+1);
  vValidationPassed();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
