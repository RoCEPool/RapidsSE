/*============================================================================*/
/* Card_Expr.cpp -- Typed Expression                                          */
/*============================================================================*/

#include "Card_Expr.hpp"

#include "Card_As.hpp"
#include "Card_RpnCalc.hpp"
#include "Card_BoolCalc.hpp"
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
: Card(CK_EXPR)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Expr::Card_Expr(Symbol* pSymbol)
: Card(CK_EXPR)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_RpnCalc* p_rpnCalc = new Card_RpnCalc(pSymbol);
  vAppend(p_rpnCalc);

  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Expr::Card_Expr(Card_As*      p_as_arg,
                     Card_Term*    p_term_0_arg)
: Card(CK_EXPR)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_As  *    p_as      = new Card_As(p_as_arg);
  Card_RpnCalc* p_rpnCalc = new Card_RpnCalc;
  Card_Term*    p_term    = p_term_0_arg->pClone();

  vAppend(p_as);


  vAppend(p_rpnCalc);
  p_rpnCalc->vAppend(p_term);
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

  RASSERT(pExpr_corr->szP_Type());

  Card_Expr*    p_expr_gby = new Card_Expr;
  Card_RpnCalc* p_rpnCalc = new Card_RpnCalc;
  Card_Term*    p_term    = pExpr_corr->pSub_term()->pClone();

  p_expr_gby->vAppend(p_rpnCalc);
  p_rpnCalc->vAppend(p_term);

  p_rpnCalc->pSymbol()->Y_vSet(P_Type, pExpr_corr->szP_Type());
  p_term->pSymbol()->Y_vSet(P_Type, pExpr_corr->szP_Type());
  p_expr_gby->pSymbol()->Y_vSet(P_Type, pExpr_corr->szP_Type());

  return p_expr_gby;
}

/*----------------------------------------------------------------------------*/
/* form project expression from gby key expression                            */
/*----------------------------------------------------------------------------*/
Card_Expr* Card_Expr::pProj_expr_From_Gby_expr(Card_Expr* p_expr_gby, Stz* szAs)
{
  _static_

  RASSERT(p_expr_gby->szP_Type());

  Symbol xSymbol_Kn;
  xSymbol_Kn.vSetKeyOrdinal(p_expr_gby->i_Child() + 1);

  Card_Expr*    p_expr    = new Card_Expr;
  Card_As*      p_as      = szAs ? new Card_As(szAs) : nullptr;
  Card_RpnCalc* p_rpnCalc = new Card_RpnCalc;
  Card_Term*    p_term    = p_expr_gby->pSub_term()->pClone(&xSymbol_Kn);

  if (p_as)
    p_expr->vAppend(p_as);
  p_expr->vAppend(p_rpnCalc);
  p_rpnCalc->vAppend(p_term);

  p_rpnCalc->pSymbol()->Y_vSet(P_Type, p_expr_gby->szP_Type());
  p_term->pSymbol()->Y_vSet(P_Type, p_expr_gby->szP_Type());
  p_expr->pSymbol()->Y_vSet(P_Type, p_expr_gby->szP_Type());

  return p_expr;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Expr* Card_Expr::pProj_Simple_expr_Of(Card_Expr* p_expr, i8 i_Col)
{
  _static_

  //p_expr->vDump();

  RASSERT(p_expr->szP_Type());

  Symbol xSymbol_Dn;
  xSymbol_Dn.vSetStreamOrdinal(i_Col);

  Card_Expr* p_expr_simple = new Card_Expr;

  Card_RpnCalc* p_rpnCalc = new Card_RpnCalc;
  p_rpnCalc->pSymbol()->Y_vSet(P_Type,p_expr->szP_Type());

  Card_Term* p_term = p_expr->pSub_term()->pClone(&xSymbol_Dn);
  p_term->pSymbol()->Y_vSet(P_Type, p_expr->szP_Type());
  //p_term->vDump();

  p_rpnCalc->vAppend(p_term);


  p_expr_simple->vAppend(p_rpnCalc);
  p_expr_simple->vColType(p_rpnCalc);

  p_expr_simple->vAppend(p_expr->pSub_as()->pDupCardRecursive());

  return p_expr_simple;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Expr* Card_Expr::pProj_Simple_expr_Of_key(Card_Expr* p_expr_key)
{
  _static_

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Expr::vBump_stmOdn(i8 nBumpBy, bool bSkipFoldExpr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (Card_BoolCalc* p_boolCalc = pSub_boolCalc())
    p_boolCalc->vBump_stmOdn(nBumpBy, bSkipFoldExpr);
  else
  if (Card_RpnCalc* p_rpnCalc = pSub_rpnCalc())
    p_rpnCalc->vBump_stmOdn(nBumpBy, bSkipFoldExpr);

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
void Card_Expr::vConvertAll_stmOdn_To_colOdn(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (Card_BoolCalc* p_boolCalc = pSub_boolCalc())
    p_boolCalc->vConvertAll_stmOdn_To_colOdn(pCI);
  else
  if (Card_RpnCalc* p_rpnCalc = pSub_rpnCalc())
    p_rpnCalc->vConvertAll_stmOdn_To_colOdn(pCI);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Expr* Card_Expr::pCraft_expr_early(CardIter* pCI, i8 i_Expr)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  /*
   * produce the projection of this expression
   */

  Stz* szExpr_this_stmcol = szSymbol();
  Card_As*      p_as;
  Card_RpnCalc* p_rpnCalc;
  Card_Term*    p_term_0;

  vDeconstruct(p_as,p_term_0);

  p_rpnCalc = pSub_rpnCalc();

  Card_Expr* p_expr_early = new Card_Expr;

  if (p_as)
    p_expr_early->vAppend(p_as->pClone());
  else {
    Card_As* p_as_new = new Card_As(i_Expr);
    p_expr_early->vAppend(p_as_new);
  }

  Card_RpnCalc* p_rpnCalc_early = _pCard_RpnCalc(p_rpnCalc->pDupCardRecursive());
  p_expr_early->vAppend(p_rpnCalc_early);

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
void Card_Expr::vEnsureTypes_For_stmOdn(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  pCI->G.p_rpnCalc = pSub_rpnCalc();

  pCI->G.p_rpnCalc->vEnsureTypesFor_stmOdn(pCI);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Expr::vFactorAll_valOdn_IntoDistinct_expr(CardIter *pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  RASSERT(! pCI->G.p_subSet);

  Card_SubSet* p_subSet = new Card_SubSet;

  // an expr is an rpnCalc (shouldn't be boolCalc)
  if ( (pCI->G.p_rpnCalc = pSub_rpnCalc()) ) {

    pCI->G.p_rpnCalc->vFactorAll_valOdn_IntoDistinct_expr(pCI);
    p_subSet->vTransferChildren(pCI->G.p_subSet);

    DELETE_CARD(pCI->G.p_subSet);
  }

  pCI->G.p_subSet = p_subSet;
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
Symbol* Card_Expr::pGetSymbol(i8 i_Term)
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
Card_Term* Card_Expr::pGetTerm(i8 i_Term)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_Term* p_term_i = pSub_rpnCalc()->pGetTerm(i_Term);

  return p_term_i;
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

  Symbol *pSymbol_0 = pGetSymbol(0);

  if (pSymbol_0->bIsKeyOrdinal())
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this expr a simple expression of a column?                              */
/*----------------------------------------------------------------------------*/
bool Card_Expr::bIsScalarCol(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  bool bScalarCol = false;
  if (Card_BoolCalc* p_boolCalc = pSub_boolCalc()) {
    bScalarCol = p_boolCalc->bIsScalarCol();
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
  if (Card_BoolCalc* p_boolCalc = pSub_boolCalc()) {
    bScalarKey = p_boolCalc->bIsScalarKey();
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
bool Card_Expr::bExpressionOfSymbol(Symbol* pSymbol)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Symbol* pSymbol_0 = pGetTerm(0)->pSymbol();

  if (pSymbol_0->bSame(pSymbol))
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

  if (Card_BoolCalc* p_boolCalc = pSub_boolCalc())
    bHas = p_boolCalc->bHasTerm(pSymbol);
  else
  if (Card_RpnCalc* p_rpnCalc = pSub_rpnCalc())
    bHas = p_rpnCalc->bHasTerm(pSymbol);

  return bHas;
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
    p_expr_propagation->vAppend(p_as->pClone());
  else {
    Card_As* p_as_new = new Card_As(this->szSymbol());
    p_expr_propagation->vAppend(p_as_new);
  }



//  Card_RpnCalc* p_rpnCalc_early = _pCard_RpnCalc(p_rpnCalc->pDupCardRecursive());
//  p_expr_propagation->vAppend(p_rpnCalc_early);
//  p_expr_propagation->vSetType(p_rpnCalc_early);

  Card_RpnCalc* p_rpnCalc_propagated = p_rpnCalc->pPropagate(this);

  p_expr_propagation->vAppend(p_rpnCalc_propagated);

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

  vAppend(p_as_arg);
  vAppend(p_rpnCalc);
  p_rpnCalc->vAppend(p_term);
}

/*----------------------------------------------------------------------------*/
/* return a reduced copy of this expr                                         */
/*----------------------------------------------------------------------------*/
Card_Expr * Card_Expr::pReducedCopy(i8 i_Expr)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Symbol xExprNo;
  xExprNo.vSetStreamOrdinal(i_Expr + 1);

  Card_Expr* p_expr_reduced = new Card_Expr;
  p_expr_reduced->pSymbol()->vUpdate(&xExprNo);
  p_expr_reduced->vColType(this);

  if (Card_As* p_as = pSub_as())
    p_expr_reduced->vAppend(p_as->pClone());

  Card_RpnCalc* p_rpnCalc_reduced = pSub_rpnCalc()->pReducedCopy(i_Expr);

  p_expr_reduced->vAppend(p_rpnCalc_reduced);

  return p_expr_reduced;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Expr::vRenumberForm_keyOdn(BASE::i8 i_Expr)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);


  pSymbol()->vSetKeyOrdinal(i_Expr);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Expr::vRenumberForm_stmOdn(BASE::i8 i_Expr)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  pSymbol()->vSetStreamOrdinal(i_Expr);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_Expr::bSameAs(Card_Expr* p_expr_arg)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (! szP_Type()->bSame(p_expr_arg->szP_Type()))
    return false;

  Card_RpnCalc* p_rpnCalc_this = this->pSub_rpnCalc();
  Card_RpnCalc* p_rpnCalc_arg  = p_expr_arg->pSub_rpnCalc();
  if (!p_rpnCalc_this->bSameAs(p_rpnCalc_arg))
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Stz* Card_Expr::szP_Type(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Stz* szP_Type = nullptr;

  if (Card_BoolCalc* p_boolCalc = pSub_boolCalc())
    szP_Type = p_boolCalc->szP_Type();
  else
  if (Card_RpnCalc* p_rpnCalc = pSub_rpnCalc())
    szP_Type = p_rpnCalc->szP_Type();

  return szP_Type;

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Expr::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (! pSymbol()->bTest_mustHave_ppy(P_Type,nullptr)) {
    vDump(__func__);
    RASSERT(0);
  }


  eCK eHas = eValidate_mustEither(CK_BOOLCALC,CK_RPNCALC,CK_NONE);

  if (CK_BOOLCALC == eHas)
    pSub_boolCalc()->vValidate(pCI,iDepth+1,this);
  else
    pSub_rpnCalc()->vValidate(pCI,iDepth+1,this);

  // all non-key expr must have an AS
  if (pParent && ! pParent->bIsCard(CK_KEY)) {
    vValidate_mustHave(CK_AS,CK_NONE);
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
