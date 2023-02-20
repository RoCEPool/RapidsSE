/*============================================================================*/
/* Card_Where.cpp -- Where                                                    */
/*============================================================================*/

#include "Card_Where.hpp"
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
: Card(CK_WHERE)
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

  while (pCI->G_bEachMainWithSub_reverse_foldBreak(CK_WHERE, CK_BOOLCALC)) {
    pCI->G.p_boolCalc->vCullPred(MARK_AT);
  }
}

/*----------------------------------------------------------------------------*/
/* delete empty where                                                         */
/*----------------------------------------------------------------------------*/
void Card_Where::vDeleteEmpyWhere(CardIter* pCI) {

  _static_

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

  while (pCI->G_bEachMainWithSub_reverse_foldBreak(CK_WHERE, CK_BOOLCALC)) {
    pCI->G.p_boolCalc->vMarkWrt_table(pCI->G.pDeck);
  }
}

/*----------------------------------------------------------------------------*/
/* move where predicate to table filter                                       */
/*----------------------------------------------------------------------------*/
void Card_Where::vMoveWherePredToTableFilter(CardIter* pCI) {

  _static_

  while (pCI->G_bEachMainWithSub_reverse_foldBreak(CK_WHERE, CK_BOOLCALC)) {
    pCI->G.p_boolCalc->vCopyMarkedPredToTable(pCI->G.pDeck);
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
    Card_Term *p_term_0 = p_rpnCalc_on->pGetTerm(0);
    if (p_term_0->bSame("CROSS"_Stz))
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
    pCI->G.p_rpnCalc->vAppendRpnCalc(p_rpnCalc_on);

    // remove and delete <absJoin>/<on>
    pCI->G.p_absJoin->vRemoveChild(pCI->G.p_on);
    DELETE_CARD(pCI->G.p_on);
  }
}

/*----------------------------------------------------------------------------*/
/* move <where> to <table keyExists>/<constraint>                             */
/*----------------------------------------------------------------------------*/
void Card_Where::vMoveWhereToTableAsConstraint(CardIter* pCI)
{
  _static_

  pCI->G.p_boolCalc = pCI->G.p_where->pSub_boolCalc();
  pCI->G.p_PRED = static_cast<Card_PRED*>(pCI->G.p_boolCalc->pGetChild(0));
  pCI->G.p_PRED->vMark1(MARK_AT);

  pCI->G.p_subSet = pCI->G.p_boolCalc->pDupSubCards();
  RASSERT(1 == pCI->G.p_subSet->nChildCards());
  pCI->G.p_PRED = static_cast<Card_PRED*>(pCI->G.p_subSet->pPop());
  DELETE_CARD(pCI->G.p_subSet);

  if ((pCI->G.p_constraint = pCI->G.p_table->pSub_constraint())) {
    pCI->G.p_boolCalc = pCI->G.p_constraint->pSub_boolCalc();
    pCI->G.p_boolCalc->vPushPred(pCI->G.p_PRED);
  }
  else {
    pCI->G_vAppendNewChild(pCI->G.p_table,CK_CONSTRAINT);
    pCI->G_vAppendNewChild(pCI->G.p_constraint,CK_BOOLCALC);
    pCI->G_vAppendChild(CK_BOOLCALC,pCI->G.p_PRED);
  }

  pCI->G.p_filter = new Card_Filter;
  pCI->G.p_table->vAppend(pCI->G.p_filter);
  pCI->G.p_filter->vAppend(pCI->G.p_boolCalc->pDupCardAndSubCards());
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
  p_expr->vValue("$1"_Stz);
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
void Card_Where::vBump_Rightwards(i8 nBumpBy)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (Card_Constraint* p_constraint = pSub_constraint())
    p_constraint->vBump_Rightwards(nBumpBy);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Where::vConvert_stmColOdn_terms_To_tblColOdn(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (Card_Constraint* p_constraint = pSub_constraint())
    p_constraint->vConvert_stmColOdn_terms_To_tblColOdn(pCI);
}

/*----------------------------------------------------------------------------*/
/* smooth                                                                     */
/*----------------------------------------------------------------------------*/
void Card_Where::vSmooth(CardIter* pCI,
                         bool bMorph_bolPred_To_cmpPred,
                         bool bReplace_sqCall_With_result,
                         i8 nBumpBy,
                         CarryForward* pCarryForward)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  pCI->G.p_where->Y_vSet(P_Smoothed);

  if (nBumpBy)
    pCI->G.p_where->vBump_Rightwards(nBumpBy);

  if (bMorph_bolPred_To_cmpPred) {
    pCI->G.p_bolPred = pCI->G.p_where->pSub_bolPred();
    pCI->G.p_bolPred->vMorph_To_cmpPred(pCI);
  }

  if (bReplace_sqCall_With_result)
    pCI->m_pTerm_sqCall->vReplace_sqCall_With_sqResult("$1"_Stz); // todo: sq result no

  pCI->G.pDeck->vRenumberRecursive();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
