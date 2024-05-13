/*============================================================================*/
/* XF_Flatten_Flatten_pipeline.cpp -- Flatten Pipeline of SubQueries          */
/*============================================================================*/

#include "XF_Flatten_Flatten_pipeline.hpp"

#include "Board.hpp"
#include "CardIter.hpp"
#include "Optimiser.hpp"

#include "eXF.h"
#include "XF_Flatten_Reorder_join.hpp"

#include "XF_Flatten_Fold_subquery.hpp"

#include "XF_Flatten_Delete_fold.hpp"

#include "XF_Flatten_X_expCol_To_tblCol.hpp"
#include "XF_Utility.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Flatten_Flatten_pipeline, CN_XF_FLATTEN_FLATTEN_PIPELINE)
__aNewFunction(XF_Flatten_Flatten_pipeline, CN_XF_FLATTEN_FLATTEN_PIPELINE)
__vDeleteFunction(XF_Flatten_Flatten_pipeline, CN_XF_FLATTEN_FLATTEN_PIPELINE)
__aFooterFunction(XF_Flatten_Flatten_pipeline)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Flatten_Flatten_pipeline::XF_Flatten_Flatten_pipeline(CardIter* pCI)
: XF_FLATTEN(CN_XF_FLATTEN_FLATTEN_PIPELINE, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_FLATTEN_PIPELINE, _FOOTER_);

  m_pCI = pCI;

  _checkInitialised_(CN_XF_FLATTEN_FLATTEN_PIPELINE);

  m_pCI->vUseBoard(nullptr);


  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_FLATTEN_PIPELINE, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Flatten_Flatten_pipeline::~XF_Flatten_Flatten_pipeline(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_FLATTEN_PIPELINE, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten_Flatten_pipeline::vFlatten(CardIter* pCI)
{
  _static_

  XF_Flatten_Flatten_pipeline* pMakeFlat = new XF_Flatten_Flatten_pipeline(pCI);

  pMakeFlat->vFlatten();

  delete pMakeFlat;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten_Flatten_pipeline::vFlatten(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_FLATTEN_PIPELINE, _FOOTER_);

  m_pCI->vUseBoard(nullptr);

  vFlatten_recurse(nullptr,0);

  XF_Flatten_Delete_fold::vXF_Delete_fold(m_pCI);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten_Flatten_pipeline::vFlatten_recurse(Symbol* pSym_sqyNo_inner, i8 iDepth)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_FLATTEN_PIPELINE, _FOOTER_);

  Board *pBoard_outer = nullptr;
  if (iDepth) {
    pBoard_outer = m_pCI->E.pBoard;
    SubQuery *pSqy_inner = m_pCI->m_pBench->pFindSubQuery(pSym_sqyNo_inner);
    m_pCI->vPushEach();
    m_pCI->vUseBoard(pSqy_inner->pBoard());
  }

  // v v v v v v v v v v v v v v v v v v v v v v v v v v v v v v v v v v v v v v
  // v v v v v v v v v v v v v v v v v v v v v v v v v v v v v v v v v v v v v v
  // v v v v v v v v v v v v v v v v v v v v v v v v v v v v v v v v v v v v v v

  // things before sub-queries

  XF_Flatten_X_expCol_To_tblCol::vFlatten(m_pCI);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // flatten the sub-queries of this outer query

  for (;;) {
    m_pCI->vBeginEach();
    if (! m_pCI->bEachOp(CK_o_CALL))
      break;

    Symbol* pSym_sqyNo_nested = m_pCI->E.p_call->pSymbol();
    vFlatten_recurse(pSym_sqyNo_nested, iDepth + 1);
    XF_Flatten_Fold_subquery::vDecisionTree(m_pCI, pSym_sqyNo_nested);
  }

  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

  bool bFound;

  do {
    m_pCI->vBeginEach_tail();
    if (!m_pCI->bEachOp_reverse_foldBreak(CK_o_WHERE, CK_PREDICATE))
      break;

    auto bLambdaFn = [&bFound,this, iDepth](Card_Term* p_term)
    {
       if (! p_term->pSymbol()->bHasFacet(SF_SQY_NO))
         return C_CONTINUE_RECURSION;

      Symbol* pSym_sqyNo_nested = p_term->pSymbol();
      vFlatten_recurse(pSym_sqyNo_nested, iDepth + 1);
      XF_Flatten_Fold_subquery::vDecisionTree(m_pCI, pSym_sqyNo_nested);

      bFound = true;
      return C_TERMINATE_RECURSION;
    };


    bFound = false;
    bool b = m_pCI->E.p_predicate->bLambdaWalk_term(bLambdaFn);
    UNUSED(b);
    vDebugPoint();
  } while (bFound);


  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

  do {
    m_pCI->vBeginEach_tail();

    if (!m_pCI->bEachOp_reverse_foldBreak(CK_o_HAVING, CK_PREDICATE))
      break;

    auto bLambdaFn = [&bFound,this,iDepth](Card_Term* p_term)
    {
      if (! p_term->pSymbol()->bHasFacet(SF_SQY_NO))
        return C_CONTINUE_RECURSION;

      Symbol* pSym_sqyNo_nested = p_term->pSymbol();
      vFlatten_recurse(pSym_sqyNo_nested, iDepth + 1);
      XF_Flatten_Fold_subquery::vDecisionTree(m_pCI, pSym_sqyNo_nested);

      bFound = true;
      return C_TERMINATE_RECURSION;
    };


    bFound = false;
    bool b = m_pCI->E.p_predicate->bLambdaWalk_term(bLambdaFn);
    UNUSED(b);
    vDebugPoint();
  } while (bFound);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // things to do to this outer query after flattening it's sub-queries

  XF_Utility::vRemove_LeftMost(m_pCI);
  XF_Flatten_Reorder_join::vXF_Reorder_join(m_pCI);
  vRotate_Next();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // sub-query has been flattened

  //Card_Fold::vMorph_to_flat(m_pCI);

  // m_pCI->PRINTBOARD_dbg();

  // ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^
  // ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^
  // ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^

  if (iDepth) {
    m_pCI->vPopEach();
    m_pCI->vUseBoard(pBoard_outer);
  }

  vDebugPoint();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/