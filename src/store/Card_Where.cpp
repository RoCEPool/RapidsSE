/*============================================================================*/
/* Card_Where.cpp -- Where                                                    */
/*============================================================================*/

#include "Card_Where.hpp"

#include "Card_OuterJoin.hpp"
#include "CardIter.hpp"
#include "XF_Optimise_Early_projection.hpp"

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

  if (pCI) {
    pCI->E.p_where   = p_where;
    pCI->E.p_filter  = p_filter;
  }

  return p_where;
}

/*----------------------------------------------------------------------------*/
/* cull marked cmp predicate                                                  */
/*----------------------------------------------------------------------------*/
void Card_Where::vCullMarked_rlnConj(CardIter* pCI) {

  _static_

  pCI->vBeginEach_tail();

  while (pCI->bEachOp_reverse_foldBreak(CK_o_WHERE, CK_PREDICATE)) {
    (void)pCI->E.p_predicate->nCullMarked_rlnConj(MARK_AT);
  }
}

/*----------------------------------------------------------------------------*/
/* delete empty where                                                         */
/*----------------------------------------------------------------------------*/
void Card_Where::vDeleteEmpyWhere(CardIter* pCI) {

  _static_

  pCI->vBeginEach_tail();

  while (pCI->bEachOp_reverse_foldBreak(CK_o_WHERE, CK_PREDICATE)) {
    if (0 == pCI->E.p_predicate->nChildCards()) {
      pCI->vRemoveDeleteMain(CK_o_WHERE);
    }
  }
}

/*----------------------------------------------------------------------------*/
/* mark where predicate with respect to table                                 */
/*----------------------------------------------------------------------------*/
void Card_Where::vMarkWhereConjWrtTable(CardIter* pCI) {

  _static_

  pCI->vBeginEach_tail();

  while (pCI->bEachOp_reverse_foldBreak(CK_o_WHERE, CK_PREDICATE)) {
    pCI->E.p_predicate->vMark_rlnConj_Wrt_table(pCI, MARK_AT);
  }
}

/*----------------------------------------------------------------------------*/
/* move where predicate to table filter                                       */
/*----------------------------------------------------------------------------*/
void Card_Where::vMoveWhereConjToTableFilter(CardIter* pCI, Stz* szWithMark) {

  _static_

  pCI->vBeginEach_tail();

  while (pCI->bEachOp_reverse_foldBreak(CK_o_WHERE, CK_PREDICATE)) {
    pCI->E.p_predicate->vCopyMarked_rlnConj_To_table_filter(pCI, szWithMark);
  }
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* smooth                                                                     */
/*----------------------------------------------------------------------------*/
void Card_Where::vSmooth(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  pCI->E.p_where->pSymbol()->Y_vSet(P_Smoothed);

  if (pCI->Carry.Where_preAgg.m_bMorph_sqlOp_To_EQ) {
    pCI->Flat.m_p_conj_iqyCall->vMorph_sqlOp_To_EQ(pCI);
  }

  if (pCI->Carry.Where_preAgg.bReplace_sqCall_With_result) {

    // replace sqCall with sqResult
    pCI->Flat.m_p_term_sqyCall->pSymbol()->vDeleteFacet(SF_SQY_NO);
    pCI->Flat.m_p_term_sqyCall->SYM(SF_SQY_RSL, (i8)1);
    pCI->Flat.m_p_term_sqyCall->SYM(SF_EXP_COL, (i8)1);

    Card_Expr* p_expr_0 = pCI->Arriving.m_pEmit->pGet_expr((i8) 0);
    pCI->Flat.m_p_term_sqyCall->vApplyCast_x(p_expr_0->xCast());

  }

  RASSERT(pCI->E.p_Op == pCI->E.p_where);
  pCI->E.p_where->vRestate_emit(pCI);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Where::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  vValidate_child_mustBe(CK_p_LEVEL, CK_NONE);

  if (!pSub_predicate())
    vDump(__func__);

  vValidate_child_mustBe(CK_o_FILTER, CK_NONE);

  UNUSED(pParent);
  Card::vValidate(pCI,iDepth,pParent);
  pCI->vValidateChildren(this,iDepth+1);
  vValidationPassed();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
