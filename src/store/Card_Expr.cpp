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

  Card_Expr*    p_gbyExpr = new Card_Expr;
  Card_RpnCalc* p_rpnCalc = new Card_RpnCalc;
  Card_Term*    p_term    = pExpr_corr->pSub_term()->pClone();

  p_gbyExpr->vAppend(p_rpnCalc);
  p_rpnCalc->vAppend(p_term);

  p_rpnCalc->pSymbol()->Y_vSet(P_Type, pExpr_corr->szP_Type());
  p_term->pSymbol()->Y_vSet(P_Type, pExpr_corr->szP_Type());

  return p_gbyExpr;
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

  p_expr_simple->vAppend(p_expr->pSub_as()->pDupCardRecursive());

  return p_expr_simple;
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

    Stz *szType = p_rpnCalc->pSymbol()->szType();

    eCT eColType = Card::eTo_eCT_char(szType);

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
Card_Expr* Card_Expr::pCraft_expr_early(CardIter* pCI)
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

  Card_RpnCalc* p_rpnCalc_early = _pCard_RpnCalc(p_rpnCalc->pDupCardRecursive());
  Card_Term* p_term_0_early = p_rpnCalc_early->pGetTerm(0);
  if (p_term_0_early->pSymbol()->Y_bHas(P_TblCol)) {
    p_term_0_early->vSymbol(p_term_0_early->pSymbol()->Y_szGet(P_TblCol));
  }
  else {
    vDebugPoint();
  }
  //p_term_0_early->vDump();
  p_expr_early->vAppend(p_rpnCalc_early);

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
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Term* Card_Expr::pGetTerm(i8 i_Term)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_Term* p_term_i = pSub_rpnCalc()->pGetTerm(i_Term);

  return p_term_i;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_Expr::bOf(Symbol* pSymbol)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Symbol* pSymbol_0 = pGetTerm(0)->pSymbol();

  if (pSymbol_0->bSameAs(pSymbol))
    return true;

  return false;
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
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Expr::vRenumberForm_keyOdn(BASE::i8 i_Expr)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Symbol xSymbol;
  xSymbol.vSetKeyOrdinal(i_Expr);

  vSymbol(xSymbol);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Expr::vRenumberForm_stmOdn(BASE::i8 i_Expr)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Symbol xSymbol;
  xSymbol.vSetStreamOrdinal(i_Expr);

  vSymbol(xSymbol);
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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
