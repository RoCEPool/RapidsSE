/*============================================================================*/
/* XF_Flatten_Predicate_Reposition.cpp -- Predicate Reposition                        */
/*============================================================================*/

#include "XF_Flatten_Predicate_Reposition.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Flatten_Predicate_Reposition, CN_XF_FLATTEN_PREDICATE_REPOSITION)
__aNewFunction(XF_Flatten_Predicate_Reposition, CN_XF_FLATTEN_PREDICATE_REPOSITION)
__vDeleteFunction(XF_Flatten_Predicate_Reposition, CN_XF_FLATTEN_PREDICATE_REPOSITION)
__aFooterFunction(XF_Flatten_Predicate_Reposition)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Flatten_Predicate_Reposition::XF_Flatten_Predicate_Reposition(CardIter* pCI)
: XF_TRANSFORM(CN_XF_FLATTEN_PREDICATE_REPOSITION, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_PREDICATE_REPOSITION, _FOOTER_);

  _checkInitialised_(CN_XF_FLATTEN_PREDICATE_REPOSITION);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_PREDICATE_REPOSITION, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Flatten_Predicate_Reposition::~XF_Flatten_Predicate_Reposition(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_PREDICATE_REPOSITION, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten_Predicate_Reposition::vXF_Predicate_Reposition(CardIter* pCI)
{
  _static_

  XF_Flatten_Predicate_Reposition* pXF_Predicate_Reposition = new XF_Flatten_Predicate_Reposition(pCI);

  pXF_Predicate_Reposition->vXF_Predicate_Reposition();

  delete pXF_Predicate_Reposition;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add level to op                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten_Predicate_Reposition::vXF_Predicate_Reposition(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_PREDICATE_REPOSITION, _FOOTER_);

  m_pCI->vTransform(__FILE__, __func__, XF_FLATTEN_PREDICATE_REPOSITION);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // At Mark Where Pred Wrt Table

 // m_pCI->PRINTBOARD_dbg();

  m_pCI->vBeginEach_reverse();

  while (m_pCI->bEachOp_reverse_foldBreak(CK_o_FROM, CK_BOOLPRED)) {
    m_pCI->E.p_boolPred->vMark_rlnConj_Wrt_table(m_pCI, MARK_AT);
  }

 // m_pCI->PRINTBOARD_dbg();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Move At Marked Where Pred To Table Filter

  m_pCI->vClearRestart();
  m_pCI->vBeginEach_reverse();

  while (m_pCI->bEachOp_reverse_foldBreak(CK_o_FROM, CK_BOOLPRED)) {
    m_pCI->E.p_boolPred->vCopyMarked_rlnConj_To_table_filter(m_pCI, MARK_AT);
  }

 // m_pCI->PRINTBOARD_dbg();

  m_pCI->vClearRestart();
  m_pCI->vBeginEach_reverse();

  while (m_pCI->bEachOp_reverse_foldBreak(CK_o_FROM, CK_BOOLPRED)) {
    (void)m_pCI->E.p_boolPred->nCullMarked_rlnConj(MARK_AT);
  }

 // m_pCI->PRINTBOARD_dbg();


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Delete Empty Where

  m_pCI->vClearRestart();
  m_pCI->vBeginEach_reverse();

  while (m_pCI->bEachOp_reverse_foldBreak(CK_o_FROM, CK_o_FILTER)) {
    m_pCI->E.p_from->vCull_filter_IfEmpty(m_pCI->E.p_filter);
  }

 // m_pCI->PRINTBOARD_dbg();

 m_pCI->E.p_deck->vMarksClearRecursive();

 m_pCI->vEndTransform(__LINE__);
//vValidateBench();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
