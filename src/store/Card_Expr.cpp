/*============================================================================*/
/* Card_Expr.cpp -- Typed Expression                                          */
/*============================================================================*/

#include "Card_Expr.hpp"

#include "Card_Type.hpp"
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
Card_Expr::Card_Expr(Card_Expr* p_expr_arg)
: Card(CK_EXPR)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Type*  p_type    = nullptr;
  Card_As  *  p_as      = nullptr;
  Card_Term*  p_term_0  = nullptr;

  vDeconstruct(p_type,p_as,p_term_0);

  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}



/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Expr::Card_Expr(Card_Type*    p_type_arg,
                     Card_As*      p_as_arg,
                     Card_Term*    p_term_0_arg)
: Card(CK_EXPR)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Type*    p_type    = new Card_Type(p_type_arg);
  Card_As  *    p_as      = new Card_As(p_as_arg);
  Card_RpnCalc* p_rpnCalc = new Card_RpnCalc;
  Card_Term*    p_term    = p_term_0_arg->pClone();

  vAppend(p_type);
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

  Card_Expr*    p_gbyExpr = new Card_Expr;
  Card_Type*    p_type    = new Card_Type(pExpr_corr->pSub_type()->szValue());
  Card_RpnCalc* p_rpnCalc = new Card_RpnCalc;
  Card_Term*    p_term    = pExpr_corr->pSub_term()->pClone();

  p_gbyExpr->vAppend(p_type);
  p_gbyExpr->vAppend(p_rpnCalc);
  p_rpnCalc->vAppend(p_term);

  return p_gbyExpr;
}

/*----------------------------------------------------------------------------*/
/* form project expression from gby key expression                            */
/*----------------------------------------------------------------------------*/
Card_Expr* Card_Expr::pProj_expr_From_Gby_expr(Card_Expr* p_expr_gby, Stz* szAs)
{
  _static_

  s32 sKn;
  sKn.vSprint("$K%lld"_Stz,p_expr_gby->iCardNo()+1);

  Card_Expr*    p_expr    = new Card_Expr;
  Card_Type*    p_type    = new Card_Type(p_expr_gby->pSub_type()->szValue());
  Card_As*      p_as      = szAs ? new Card_As(szAs) : nullptr;
  Card_RpnCalc* p_rpnCalc = new Card_RpnCalc;
  Card_Term*    p_term    = p_expr_gby->pSub_term()->pClone(sKn.szStz());

  p_expr->vAppend(p_type);
  if (p_as)
    p_expr->vAppend(p_as);
  p_expr->vAppend(p_rpnCalc);
  p_rpnCalc->vAppend(p_term);

  return p_expr;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Expr* Card_Expr::pProj_Simple_expr_Of(Card_Expr* p_expr, i8 i_Col)
{
  _static_

  s32 sDn;
  sDn.vSprint("$%lld"_Stz,i_Col);

  Card_Expr* p_expr_simple = new Card_Expr;

  p_expr_simple->vAppend(p_expr->pSub_type()->pDupCardAndSubCards());

  Card_RpnCalc* p_rpnCalc = new Card_RpnCalc;
  Card_Term* p_term = p_expr->pSub_term()->pClone(sDn);
  p_rpnCalc->vAppend(p_term);

  p_expr_simple->vAppend(p_rpnCalc);

  p_expr_simple->vAppend(p_expr->pSub_as()->pDupCardAndSubCards());

  return p_expr_simple;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Expr::vBump_Rightwards(i8 nBumpBy, bool bSkipFoldExpr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (Card_BoolCalc* p_boolCalc = pSub_boolCalc())
    p_boolCalc->vBump_Rightwards(nBumpBy,bSkipFoldExpr);
  else
  if (Card_RpnCalc* p_rpnCalc = pSub_rpnCalc())
    p_rpnCalc->vBump_Rightwards(nBumpBy,bSkipFoldExpr);

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Expr* Card_Expr::pClone(bool bStmCol_flip)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_Expr* p_clone = _pCard_Expr(pDupCardAndSubCards());

  if (bStmCol_flip)
    p_clone->pProjectedTerm()->vEnsure_tblCol_flip();

  return p_clone;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Expr::vConvert_stmColOdn_terms_To_tblColOdn(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (Card_BoolCalc* p_boolCalc = pSub_boolCalc())
    p_boolCalc->vConvert_stmColOdn_terms_To_tblColOdn(pCI);
  else
  if (Card_RpnCalc* p_rpnCalc = pSub_rpnCalc())
    p_rpnCalc->vConvert_stmColOdn_terms_To_tblColOdn(pCI);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Expr::vDeconstruct(Card_Type*&    rp_type,
                             Card_As*&      rp_as,
                             Card_Term*&    rp_term_0)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  rp_type = pSub_type();
  rp_as   = pSub_as();
  Card_RpnCalc* p_rpnCalc = pSub_rpnCalc();
  rp_term_0  = p_rpnCalc ? p_rpnCalc->pSub_term() : nullptr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Stz* Card_Expr::szDollarOdn(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Stz* szVal = szValue();

  RASSERT(Card::bIsStreamColOrdinal(szVal) || Card::bIsKeyOrdinal(szVal));

  return szVal;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_Expr::bIsCalcSequence(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  bool b = ! bIsColOrdinal();

  return b;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_Expr::bIsColOrdinal(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_RpnCalc* p_rpnCalc = pSub_rpnCalc();

  bool b = p_rpnCalc->bIsSingleCol();

  return b;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_Expr::bIsProjectedCol(Stz* szProjectedCol)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Stz* szPrjCol = this->szProjectedCol();
  if (! szPrjCol)
    return false;

  if (szPrjCol->bSame(szProjectedCol))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_Expr::bHasProjectedCol(Card_Term* p_term_arg)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

 Card_Term* p_term_prj = this->pProjectedTerm();
  if (! p_term_prj)
    return false;

  if (p_term_prj->bIsSame(p_term_arg))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_Expr::bIsSame(Card_Expr* p_expr_arg)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_Type* p_type_this = this->pSub_type();
  Card_Type* p_type_arg  = p_expr_arg->pSub_type();
  if (!p_type_this->bIsSame(p_type_arg))
    return false;

  Card_RpnCalc* p_rpnCalc_this = this->pSub_rpnCalc();
  Card_RpnCalc* p_rpnCalc_arg  = p_expr_arg->pSub_rpnCalc();
  if (!p_rpnCalc_this->bIsSame(p_rpnCalc_arg))
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Stz* Card_Expr::szProjectedCol(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Stz* szPrjCol = pSub_rpnCalc()->szProjectedCol();

  return szPrjCol;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Term* Card_Expr::pProjectedTerm(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_Term* szPrjTerm = pSub_rpnCalc()->pGetTerm(0);

  return szPrjTerm;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Expr::vReconstruct(Card_Type*  p_type_arg,
                             Card_As*    p_as_arg,
                             Card_Term*  p_term_0_arg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  vRemoveDeleteAllChildren();

  Card_RpnCalc* p_rpnCalc = new Card_RpnCalc;
  Card_Term*    p_term    = p_term_0_arg->pClone();

  vAppend(p_type_arg);
  vAppend(p_as_arg);
  vAppend(p_rpnCalc);
  p_rpnCalc->vAppend(p_term);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Expr::vRenumber(BASE::i8 i_Expr, bool bIsKeyExpr)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (bIsKeyExpr)
    vRenumberAsKeyOdn(i_Expr);
  else
    vRenumberAsStmColOdn(i_Expr);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Expr::vRenumberAsKeyOdn(BASE::i8 i_Expr)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  vValueSprint("$K%lld"_Stz,i_Expr);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Expr::vRenumberAsStmColOdn(BASE::i8 i_Expr)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  vValueSprint("$%lld"_Stz,i_Expr);
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Expr* Card_Expr::pDeduce(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  /*
   * produce the projection of this expression
   */

  Card_Type*    p_type;
  Card_As*      p_as;
  Card_RpnCalc* p_rpnCalc;
  Card_Term*    p_term_0;

  vDeconstruct(p_type,p_as,p_term_0);
  p_rpnCalc = pSub_rpnCalc();


  Card_Expr* p_expr_right = new Card_Expr;

  p_expr_right->vAppend(p_type->pDupCardAndSubCards());
  if (p_as)
    p_expr_right->vAppend(p_as->pDupCardAndSubCards());
  p_expr_right->vAppend(p_rpnCalc->pDupCardAndSubCards());

//  if (pCI->ProjectionAnalysis.m_pProjection_running && pCI->EarlyProject.m_pProjection_needed)
//    p_rpnCalc->vFlipTerms(pCI->EarlyProject.m_pProjection_needed);

  return p_expr_right;
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

  Stz* szExpr_this_stmcol = szValue();
  Card_Type*    p_type;
  Card_As*      p_as;
  Card_RpnCalc* p_rpnCalc;
  Card_Term*    p_term_0;

  vDeconstruct(p_type,p_as,p_term_0);
    p_rpnCalc = pSub_rpnCalc();

  Card_Expr* p_expr_early = new Card_Expr;

  p_expr_early->vAppend(p_type->pClone());
  if (p_as)
    p_expr_early->vAppend(p_as->pClone());

  Card_RpnCalc* p_rpnCalc_early = _pCard_RpnCalc(p_rpnCalc->pDupCardAndSubCards());
  Card_Term* p_term_0_early = p_rpnCalc_early->pGetTerm(0);
  if (p_term_0_early->Y_bHas(P_TblCol)) {
    p_term_0_early->vValue(p_term_0_early->Y_szGet(P_TblCol));
  }
  else {
    vDebugPoint();
  }
  p_term_0_early->vDump();
  p_expr_early->vAppend(p_rpnCalc_early);

  return p_expr_early;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
i8 Card_Expr::iRpnCalc_WhichGbyKey(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_RpnCalc* p_rpnCalc = this->pSub_rpnCalc();
  if (! p_rpnCalc)
    return C_UNKNOWN;

  if (p_rpnCalc->nChildCards() > 1)
    return C_UNKNOWN;

  CardIter xCI(p_rpnCalc);
  while (xCI.G_bEachMain())  {
    if (Card::bIsKeyOrdinal(xCI.G.p_term->szValue())) {
      i8 i_Key = Card::iGetColOdn(xCI.G.p_term->szValue());
      return i_Key;
    }
  }

  return C_UNKNOWN;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
