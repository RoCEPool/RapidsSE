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
: Card_CONDITION(CK_WHERE, 1)
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

  pCI->vBeginEachReverse();

  while (pCI->G_bEachMainWithSub_reverse_foldBreak_b(CK_WHERE, CK_BOOLCALC)) {
    pCI->G.p_boolCalc->nCullPred(MARK_AT);
  }
}

/*----------------------------------------------------------------------------*/
/* delete empty where                                                         */
/*----------------------------------------------------------------------------*/
void Card_Where::vDeleteEmpyWhere(CardIter* pCI) {

  _static_

  pCI->vBeginEachReverse();

  while (pCI->G_bEachMainWithSub_reverse_foldBreak_b(CK_WHERE, CK_BOOLCALC)) {
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

  pCI->vBeginEachReverse();

  while (pCI->G_bEachMainWithSub_reverse_foldBreak_b(CK_WHERE, CK_BOOLCALC)) {
    pCI->G.p_boolCalc->vMarkWrt_table(pCI);
  }
}

/*----------------------------------------------------------------------------*/
/* move where predicate to table filter                                       */
/*----------------------------------------------------------------------------*/
void Card_Where::vMoveWherePredsToTableFilter(CardIter* pCI) {

  _static_

  pCI->vBeginEachReverse();

  while (pCI->G_bEachMainWithSub_reverse_foldBreak_b(CK_WHERE, CK_BOOLCALC)) {
    pCI->G.p_boolCalc->vCopyMarkedPredsToTable(pCI);
  }
}

/*----------------------------------------------------------------------------*/
/* create new <where>/<constraint>/<expr $1>/<rpnCalc>                        */
/*----------------------------------------------------------------------------*/
Card_Where* Card_Where::pNewWhereAssembly(void)
{
  _static_

  Card_Where* p_where = new Card_Where;

  Card_Filter* p_filter = new Card_Filter;
  p_where->vAppendChild(p_filter);

  Card_RpnCalc* p_rpnCalc = new Card_RpnCalc;
  p_filter->vAppendChild(p_rpnCalc);

  return p_where;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

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

 // pCI->vPrintBoard();

  // save the current projection needed
  //   we can restore to this save if projection needed get extended
  Card_Needed* p_needed_save = pCI->Early.m_pNeeded_later->pClone();

  // check for additional/temporary transients
  vEarlyProject_add_transients(pCI);

  Card_OP::vEarlyProject_backstep(pCI);

  /// ... arrived back ... ... ... ... ... ... ... ... ... ... ... ... ... ...

 // pCI->vPrintBoard();

  vUpdate_emitted(pCI);

 // pCI->vPrintBoard();

  vCorrectStmCol(pCI->Prior.m_pEmitted);

  // restore needed
  delete pCI->Early.m_pNeeded_later;
  pCI->Early.m_pNeeded_later = p_needed_save;

 // pCI->vPrintBoard();

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
    pCI->G.p_cmpConj = pCI->G.p_where->pSub_cmpPred();
    pCI->G.p_cmpConj->vMorph_SqlOp_To_Eq(pCI);
  }

  if (pCarryForward->Where_preAgg.bReplace_sqCall_With_result) {
    //Symbol x_Symbol_stmCol1;
    //x_Symbol_stmCol1.vSet_Col_No(1);

    // replace sqCall with sqResult
    pCI->Sqy.m_pTerm_sqCall->pSymbol()->vDeleteFacet(SF_SQY_NM);
    pCI->Sqy.m_pTerm_sqCall->SYM(SF_STM_COL, (i8)1);
  }

  pCI->G.p_where->vUpdate_emitted(pCI);

 // pCI->vRenumberCards();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Where::vUpdate_emitted(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if ((pCI->G.p_stream = pCI->G.p_where->pSub_stream())) {
    if ((pCI->G.p_emitted = pCI->G.p_stream->pSub_emitted())) {
      pCI->G.p_stream->vDeleteChild(pCI->G.p_emitted);
      pCI->G.p_stream->vAppendChild(pCI->Prior.m_pEmitted->pClone());
    }
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Where::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  vValidate_mustHave(CK_LEVEL,CK_NONE);

  UNUSED(pParent);
  Card::vValidate(pCI,iDepth,pParent);
  pCI->vValidateChildren(this,iDepth+1);
  vValidationPassed();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
