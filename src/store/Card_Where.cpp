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
: Card_COND(CK_WHERE)
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

  pCI->vBeginEach();

  while (pCI->G_bEachMainWithSub_reverse_foldBreak(CK_WHERE, CK_BOOLCALC)) {
    pCI->G.p_boolCalc->vCullPred(MARK_AT);
  }
}

/*----------------------------------------------------------------------------*/
/* delete empty where                                                         */
/*----------------------------------------------------------------------------*/
void Card_Where::vDeleteEmpyWhere(CardIter* pCI) {

  _static_

  pCI->vBeginEach();

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

  pCI->vClearRestart();

  while (pCI->G_bEachMainWithSub_reverse_foldBreak(CK_WHERE, CK_BOOLCALC)) {
    pCI->G.p_boolCalc->vMarkWrt_table(pCI);
  }
}

/*----------------------------------------------------------------------------*/
/* move where predicate to table filter                                       */
/*----------------------------------------------------------------------------*/
void Card_Where::vMoveWherePredsToTableFilter(CardIter* pCI) {

  _static_

  pCI->vClearRestart();

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

  pCI->vBeginEach();
  if (pCI->G_bEachMain(CK_WHERE))
    pCI->G.p_rpnCalc = pCI->G.p_where->pSub_rpnCalc();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // for each <absJoin>/<on>/<rpnCalc>
  //   move/append to <where>/<rpnCalc>

  pCI->vBeginEach();
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
        pCI->G_vNewMember(CK_OUTERJOIN, Symbol(SF_CROSS,"LEFT_OUTER"_Stz));
        pCI->G.p_table->vAppend(pCI->G.p_outerJoin);
      }
      pCI->G_vPopIter();
    }

    Card_Term *p_term_0 = p_rpnCalc_on->pGetTerm(0);
    if (p_term_0->pSymbol()->bSame("CROSS"_Stz))
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

  Card_RpnCalc* p_rpnCalc = new Card_RpnCalc;
  p_where->vAppend(p_rpnCalc);

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

  vAddTransients(pCI);
}


/*----------------------------------------------------------------------------*/
/* early project                                                              */
/*----------------------------------------------------------------------------*/
void Card_Where::vEarlyProject_recurse_update(CardIter* pCI)
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

  pCI->vPrintBoard();

  vUpdate_projection(pCI);

  pCI->vPrintBoard();

  vCorrectStmOdn(pCI->Prior.m_pProjection);

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
    //Symbol x_Symbol_stmCol1;
    //x_Symbol_stmCol1.vSet_Col_No(1);

    // replace sqCall with sqResult
    pCI->Sqy.m_pTerm_sqCall->SYM(SF_COL_NO,(i8)1);
  }

  pCI->G.p_where->vUpdate_projection(pCI);

  pCI->vRenumberCards();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Where::vUpdate_projection(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if ((pCI->G.p_projection = pCI->G.p_where->pSub_projection())) {

    pCI->G.p_where->vDeleteChild(pCI->G.p_projection);
    pCI->G.p_where->vAppend(pCI->Prior.m_pProjection->pClone());

  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Where::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  vValidate_mustHave(CK_LEVEL,CK_NONE);
  pSub_level()->vValidate(pCI,iDepth+1,this);

  if (CK_BOOLCALC == eValidate_mustEither(CK_BOOLCALC,CK_RPNCALC,CK_NONE))
    pSub_boolCalc()->vValidate(pCI,iDepth+1,this);
  else
    pSub_rpnCalc()->vValidate(pCI,iDepth+1,this);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
