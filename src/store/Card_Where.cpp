/*============================================================================*/
/* Card_Where.cpp -- Where                                                    */
/*============================================================================*/

#include "Card_Where.hpp"

#include "Card_OuterJoin.hpp"
#include "CardIter.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_Where, CK_o_WHERE)
__aNewFunction(Card_Where,CN_CARD)
__vDeleteFunction(Card_Where,CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Where::Card_Where(void)
: Card_CONDITION(CK_o_WHERE, 1)
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
/* create new <where>/<filter>/<expr>/<rpnCalc>                               */
/*----------------------------------------------------------------------------*/
Card_Where* Card_Where::pNewAssembly(CardIter* pCI)
{
  _static_

  Card_Where* p_where = new Card_Where;

  Card_Filter* p_filter = new Card_Filter;
  p_where->vAppendChild(p_filter);

  Card_Expr* p_expr = new Card_Expr;
  p_filter->vAppendChild(p_expr);

  Card_RpnCalc* p_rpnCalc = new Card_RpnCalc;
  p_expr->vAppendChild(p_rpnCalc);

  if (pCI) {
    pCI->G.p_where   = p_where;
    pCI->G.p_filter  = p_filter;
    pCI->G.p_expr    = p_expr;
    pCI->G.p_rpnCalc = p_rpnCalc;
  }

  return p_where;
}

/*----------------------------------------------------------------------------*/
/* cull marked cmp predicate                                                  */
/*----------------------------------------------------------------------------*/
void Card_Where::vCullMarked_rlnConj(CardIter* pCI) {

  _static_

  pCI->vBeginEach_reverse();

  while (pCI->bEachOp_reverse_foldBreak(CK_o_WHERE, CK_BOOLPRED)) {
    (void)pCI->G.p_boolPred->nCullMarked_rlnConj(MARK_AT);
  }
}

/*----------------------------------------------------------------------------*/
/* delete empty where                                                         */
/*----------------------------------------------------------------------------*/
void Card_Where::vDeleteEmpyWhere(CardIter* pCI) {

  _static_

  pCI->vBeginEach_reverse();

  while (pCI->bEachOp_reverse_foldBreak(CK_o_WHERE, CK_BOOLPRED)) {
    if (0 == pCI->G.p_boolPred->nChildCards()) {
      pCI->vRemoveDeleteMain(CK_o_WHERE);
    }
  }
}

/*----------------------------------------------------------------------------*/
/* mark where predicate with respect to table                                 */
/*----------------------------------------------------------------------------*/
void Card_Where::vMarkWhereConjWrtTable(CardIter* pCI) {

  _static_

  pCI->vBeginEach_reverse();

  while (pCI->bEachOp_reverse_foldBreak(CK_o_WHERE, CK_BOOLPRED)) {
    pCI->G.p_boolPred->vMark_rlnConj_Wrt_table(pCI, MARK_AT);
  }
}

/*----------------------------------------------------------------------------*/
/* move where predicate to table filter                                       */
/*----------------------------------------------------------------------------*/
void Card_Where::vMoveWhereConjToTableFilter(CardIter* pCI, Stz* szWithMark) {

  _static_

  pCI->vBeginEach_reverse();

  while (pCI->bEachOp_reverse_foldBreak(CK_o_WHERE, CK_BOOLPRED)) {
    pCI->G.p_boolPred->vCopyMarked_rlnConj_To_table_filter(pCI, szWithMark);
  }
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* burst <where>/<filter>/<expr>/<rpnCalc>                                    */
/*----------------------------------------------------------------------------*/
void Card_Where::vBurstAssembly(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  pCI->G.p_where   = this;
  pCI->G.p_filter  = pCI->G.p_where->pSub_filter(C_NOT_NULL);
  pCI->G.p_expr    = pCI->G.p_filter->pSub_expr(C_NOT_NULL);
  pCI->G.p_rpnCalc = pCI->G.p_expr->pSub_rpnCalc(C_NOT_NULL);
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

 // pCI->PRINTBOARD_dbg();

  // save the current projection needed
  //   we can restore to this save if projection needed get extended
  Card_Needed* p_needed_save = pCI->Early.m_pNeeded_later->pClone();

  // check for additional/temporary transients
  vEarlyProject_add_transients(pCI);

  Card_OP::vEarlyProject_backstep(pCI);

  /// ... arrived back ... ... ... ... ... ... ... ... ... ... ... ... ... ...

 // pCI->PRINTBOARD_dbg();

  pCI->G.p_Op = pCI->G.p_where;
  ((Card_OP*)pCI->G.p_Op)->vRestate_emit(pCI);

 // pCI->PRINTBOARD_dbg();

  // restore needed
  delete pCI->Early.m_pNeeded_later;
  pCI->Early.m_pNeeded_later = p_needed_save;
}


/*----------------------------------------------------------------------------*/
/* smooth                                                                     */
/*----------------------------------------------------------------------------*/
void Card_Where::vSmooth(CardIter* pCI, CarryForward* pCarryForward)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  pCI->G.p_where->pSymbol()->Y_vSet(P_Smoothed);

  if (pCarryForward->Where_preAgg.m_bMorph_sqlOp_To_EQ) {
    pCI->Flat.m_p_conj_iqyCall->vMorph_sqlOp_To_EQ(pCI);
  }

  if (pCarryForward->Where_preAgg.bReplace_sqCall_With_result) {

    // replace sqCall with sqResult
    pCI->Flat.m_p_term_sqyCall->pSymbol()->vDeleteFacet(SF_SQY_NO);
    pCI->Flat.m_p_term_sqyCall->SYM(SF_SQY_RSL, (i8)1);
    pCI->Flat.m_p_term_sqyCall->SYM(SF_EXP_COL, (i8)1);

    Card_Expr* p_expr_0 = pCI->Flux.m_pArriving->pGet_expr((i8) 0);
    pCI->Flat.m_p_term_sqyCall->vApplyCast_x(p_expr_0->xCast());

  }

  RASSERT(pCI->G.p_Op == pCI->G.p_where);
  pCI->G.p_where->vRestate_emit(pCI);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Where::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  vValidate_mustHave(CK_s_LEVEL, CK_NONE);

  if (!pSub_boolPred())
    vDump(__func__);

  vValidate_mustHave(CK_o_FILTER, CK_NONE);

  UNUSED(pParent);
  Card::vValidate(pCI,iDepth,pParent);
  pCI->vValidateChildren(this,iDepth+1);
  vValidationPassed();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
