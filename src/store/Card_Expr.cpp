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

  Card_Expr*    p_expr    = new Card_Expr;
  Card_Type*    p_type    = new Card_Type(pExpr_corr->pSub_type()->szValue());
  Card_RpnCalc* p_rpnCalc = new Card_RpnCalc;
  Card_Term*    p_term    = new Card_Term(pExpr_corr->szValue());

  p_expr->vAppend(p_type);
  p_expr->vAppend(p_rpnCalc);
  p_rpnCalc->vAppend(p_term);

  return p_expr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Expr* Card_Expr::pGby_expr_From_stmCol(Card_StmCol* p_stmCol)
{
  _static_

  Card_Expr*    p_expr    = new Card_Expr;
  Card_Type*    p_type    = new Card_Type(p_stmCol->pSub_type()->szValue());
  Card_RpnCalc* p_rpnCalc = new Card_RpnCalc;
  Card_Term*    p_term    = new Card_Term(p_stmCol->pSub_stmColOdn()->szValue());

  p_expr->vAppend(p_type);
  p_expr->vAppend(p_rpnCalc);
  p_rpnCalc->vAppend(p_term);

  return p_expr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
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
  Card_Term*    p_term    = new Card_Term(sKn.szStz());

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
Card_Expr* Card_Expr::pProj_expr_From_stmCol(Card_StmCol* p_stmCol)
{
  _static_

  Card_Expr*    p_expr    = new Card_Expr;
  Card_Type*    p_type    = new Card_Type(p_stmCol->pSub_type()->szValue());
  Card_RpnCalc* p_rpnCalc = new Card_RpnCalc;
  Card_Term*    p_term    = new Card_Term(p_stmCol->pSub_stmColOdn()->szValue());

  p_expr->vAppend(p_type);
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

  //Card_Expr* p_expr_simple = _pCard_Expr(p_expr->pDupCardAndSubCards());

  Card_Expr* p_expr_simple = new Card_Expr;

  p_expr_simple->vAppend(p_expr->pSub_type()->pDupCardAndSubCards());

  Card_RpnCalc* p_rpnCalc = new Card_RpnCalc;
  Card_Term* p_term = new Card_Term;
  p_term->vValueSprint("$%lld"_Stz,i_Col);
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
/*--------------------------------------2--------------------------------------*/
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
Stz* Card_Expr::szProjectedCol(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Stz* szPrjCol = pSub_rpnCalc()->szProjectedCol();

  return szPrjCol;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Term* Card_Expr::szProjectedTerm(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_Term* szPrjTerm = pSub_rpnCalc()->pGetTerm(0);

  return szPrjTerm;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Expr::vRenumber(BASE::i8 i_Expr, bool bIsKeyExpr)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (bIsKeyExpr)
    vValueSprint("$K%lld"_Stz,i_Expr);
  else
    vValueSprint("$%lld"_Stz,i_Expr);
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
  while (Card_Term* p_term = _pCard_Term(xCI.pEachChild())) {
    if (Card::bIsKeyOrdinal(p_term->szValue())) {
      i8 i_Key = Card::iGetColOdn(p_term->szValue());
      return i_Key;
    }
  }

  return C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_TblColOdn* Card_Expr::pTblColOdn(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_TblColOdn* pTblColOdn = pSub_tblColOdn();

  return pTblColOdn;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Expr* Card_Expr::pToCarry_key(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_Expr* p_expr_carry = _pCard_Expr(pDupCardAndSubCards());

  return p_expr_carry;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
