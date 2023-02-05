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
void Card_Expr::vBump_Rightwards(i8 nBumpBy)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (Card_BoolCalc* p_boolCalc = pSub_boolCalc())
    p_boolCalc->vBump_Rightwards(nBumpBy);
  else
  if (Card_RpnCalc* p_rpnCalc = pSub_rpnCalc())
    p_rpnCalc->vBump_Rightwards(nBumpBy);

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
void Card_Expr::vNumberValue(BASE::i8 i_Expr)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
