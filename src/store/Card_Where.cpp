/*============================================================================*/
/* Card_Where.cpp -- Where                                                    */
/*============================================================================*/

#include "Card_Where.hpp"

#include "Card_OuterJoin.hpp"
#include "CardIter.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_Where,CK_WHERE)
__aNewFunction(Card_Where,CN_CARD)
__vDeleteFunction(Card_Where,CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Where::Card_Where(void)
: Card_OP(CK_WHERE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Where::~Card_Where(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* cull marked cmp predicate                                                  */
/*----------------------------------------------------------------------------*/
void Card_Where::vCullMarkedCmpPred(CardIter* pCI) {

  _static_

  pCI->vRestart();

  while (pCI->G_bEachMainWithSub_reverse_foldBreak(CK_WHERE, CK_BOOLCALC)) {
    pCI->G.p_boolCalc->vCullPred(MARK_AT);
  }
}

/*----------------------------------------------------------------------------*/
/* delete empty where                                                         */
/*----------------------------------------------------------------------------*/
void Card_Where::vDeleteEmpyWhere(CardIter* pCI) {

  _static_

  pCI->vRestart();

  while (pCI->G_bEachMainWithSub_reverse_foldBreak(CK_WHERE, CK_BOOLCALC)) {
    if (0 == pCI->G.p_boolCalc->nChildCards()) {
      pCI->G_vRemoveDeleteMain(CK_WHERE);
    }
  }
}

/*----------------------------------------------------------------------------*/
/* mark where predicate with respect to table                                 */
/*----------------------------------------------------------------------------*/
void Card_Where::vMarkWherePredWrtTable(CardIter* pCI) {

  _static_

  pCI->G_vRestart();

  while (pCI->G_bEachMainWithSub_reverse_foldBreak(CK_WHERE, CK_BOOLCALC)) {
    pCI->G.p_boolCalc->vMarkWrt_table(pCI);
  }
}

/*----------------------------------------------------------------------------*/
/* move where predicate to table filter                                       */
/*----------------------------------------------------------------------------*/
void Card_Where::vMoveWherePredsToTableFilter(CardIter* pCI) {

  _static_

  pCI->G_vRestart();

  while (pCI->G_bEachMainWithSub_reverse_foldBreak(CK_WHERE, CK_BOOLCALC)) {
    pCI->G.p_boolCalc->vCopyMarkedPredsToTable(pCI->G.pDeck);
  }
}

/*----------------------------------------------------------------------------*/
/* move all <absJoin>/<on>/<rpnCalc> to <where>                               */
/*----------------------------------------------------------------------------*/
void Card_Where::vMove_All_on_To_where(CardIter* pCI)
{
  _static_

  // find the <where> (and /<rpnCalc> in the pipeline
  // n.b. there might not be a <where> yet

  pCI->vRestart();
  if (pCI->G_bEachMain(CK_WHERE))
    pCI->G.p_rpnCalc = pCI->G.p_where->pSub_rpnCalc();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // for each <absJoin>/<on>/<rpnCalc>
  //   move/append to <where>/<rpnCalc>

  pCI->vRestart();
  while (pCI->G_bEachMainWithSub_reverse(CK_ABSJOIN,CK_ON)) {

    // ignore
    Card_RpnCalc *p_rpnCalc_on = pCI->G.p_on->pSub_rpnCalc();
    if (! p_rpnCalc_on->nChildCards())
      continue;

    // remove LEFT_OUTER (temp)
    i8 nTrimed = p_rpnCalc_on->nTrimChildren(CK_TERM,"LEFT_OUTER"_Stz);
    if (nTrimed) {
      pCI->G_vPushIter();
      if (pCI->G_bEachMain_reverse(CK_SCAN)) {
        pCI->G.p_table = pCI->G.p_scan->pSub_table();
        pCI->G_vNewMember(CK_OUTERJOIN, "left"_Stz);
        pCI->G.p_table->vAppend(pCI->G.p_outerJoin);
      }
      pCI->G_vPopIter();
    }

    Card_Term *p_term_0 = p_rpnCalc_on->pGetTerm(0);
    if (p_term_0->bIsSymbol("CROSS"_Stz))
      continue;

    if (! pCI->G.p_where) {
      // create a new <where> if none exists
      //pCI->G_vNewMember(CK_WHERE);
      pCI->G.p_where = Card_Where::pNewWhereConstruct();
      pCI->G.pDeck->vInsertAfter(pCI->G.p_where,pCI->G.p_absJoin);

      pCI->G.p_rpnCalc = pCI->G.p_where->pSub_rpnCalc();
      // create an initial empty /<rpnCalc>
      //pCI->G_vAppendNewChild(pCI->G.p_where,CK_RPNCALC);
    }

    // append the <absJoin>/<on>/<rpnCalc> to the <where>/<rpnCalc>
    pCI->G.p_rpnCalc->vConcat(p_rpnCalc_on);

    // remove and delete <absJoin>/<on>
    pCI->G.p_absJoin->vRemoveChild(pCI->G.p_on);
    DELETE_CARD(pCI->G.p_on);
  }
}

/*----------------------------------------------------------------------------*/
/* create new <where>/<constraint>/<expr $1>/<rpnCalc>                        */
/*----------------------------------------------------------------------------*/
Card_Where* Card_Where::pNewWhereConstruct(void)
{
  _static_

  Card_Where* p_where = new Card_Where;

  Card_Constraint* p_constraint = new Card_Constraint;
  p_where->vAppend(p_constraint);

  Card_Expr* p_expr = new Card_Expr;
  Symbol x_Symbol_stmCol1;
  x_Symbol_stmCol1.vSetStreamOrdinal(1);

  p_expr->vSymbol(x_Symbol_stmCol1);
  p_constraint->vAppend(p_expr);

  Card_RpnCalc* p_rpnCalc = new Card_RpnCalc;
  p_expr->vAppend(p_rpnCalc);

  return p_where;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Where::vConvert_stmColOdn_terms_To_tblColOdn(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

//  if (Card_Constraint* p_constraint = pSub_constraint())
//    p_constraint->vConvertAll_stmOdn_To_colOdn(pCI);
  if (Card_BoolCalc* p_boolCalc = pSub_boolCalc())
    p_boolCalc->vConvertAll_stmOdn_To_colOdn(pCI);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Where::vEarlyProject_add_transients(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambda = [pCI] (Card_Term* p_term) {

    if (!p_term->bIsValOdn())
      return C_CONTINUE_RECURSION;

    if (pCI->Early.m_pNeeded_later->pFindExpr(p_term->pSymbol()))
      return C_CONTINUE_RECURSION;

    if (Card_Expr* p_expr_defn = pCI->pResolvePrior_expr(p_term)) {
      //p_expr_defn->vDump();
      Card_Expr* p_expr_clone = p_expr_defn->pClone();
      pCI->Early.m_pNeeded_later->vAppend(p_expr_clone);
      pCI->Early.m_pNeeded_later->vRenumberAll_expr();
    }

    return C_CONTINUE_RECURSION;
  };

  pCI->G.p_boolCalc = pSub_boolCalc();

  pCI->G.p_boolCalc->bApplyRecursive_term(bLambda);

}


/*----------------------------------------------------------------------------*/
/* early project                                                              */
/*----------------------------------------------------------------------------*/
void Card_Where::vEarlyProject_recurse_passthru(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  pCI->vPrintBoard();

  // save the current projection needed
  //   we can restore to this save if projection needed get extended
  Card_Needed* p_needed_save = pCI->Early.m_pNeeded_later->pClone();

  // check for additional/temporary transients
  vEarlyProject_add_transients(pCI);

  Card_OP::vEarlyProject_backstep(pCI);

  /// ... arrived back ... ... ... ... ... ... ... ... ... ... ... ... ... ...

  // restore needed
  delete pCI->Early.m_pNeeded_later;
  pCI->Early.m_pNeeded_later = p_needed_save;

  pCI->vPrintBoard();

  vDebugPoint();
}


/*----------------------------------------------------------------------------*/
/* smooth                                                                     */
/*----------------------------------------------------------------------------*/
void Card_Where::vSmooth(CardIter* pCI, CarryForward* pCarryForward)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  pCI->G.p_where->pSymbol()->Y_vSet(P_Smoothed);

  if (pCarryForward->Where_preAgg.m_bMorph_sqlOp_To_Eq) {
    pCI->G.p_cmpPred = pCI->G.p_where->pSub_cmpPred();
    pCI->G.p_cmpPred->vMorph_SqlOp_To_Eq(pCI);
  }

  if (pCarryForward->Where_preAgg.bReplace_sqCall_With_result) {
    Symbol x_Symbol_stmCol1;
    x_Symbol_stmCol1.vSetStreamOrdinal(1);

    // replace sqCall with sqResult
    pCI->Sqy.m_pTerm_sqCall->vSymbol(x_Symbol_stmCol1);
  }

  pCI->G.pDeck->vRenumberRecursive();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Where::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  vValidate_mustHave(CK_LEVEL,CK_BOOLCALC,CK_NONE);

  pSub_level()->vValidate(pCI,iDepth+1,this);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
