/*============================================================================*/
/* XF_Flatten_subquery.cpp -- Flatten SubQueries                                */
/*============================================================================*/

#include "XF_Flatten_subquery.hpp"

#include "Board.hpp"
#include "CardIter.hpp"
#include "Optimiser.hpp"

#include "eXF.h"
#include "XF_Flatten_Join_Ordering.hpp"

#include "XF_Flatten_Fold_subquery.hpp"
#include "XF_Flatten_53_W_L_NC_NI_join.hpp"

#include "XF_Flatten_Delete_fold.hpp"

#include "XF_Flatten_X_expCol_To_tblCol.hpp"
#include "XF_Utility.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Flatten_subquery, CN_XF_FLATTEN_SUBQUERY)
__aNewFunction(XF_Flatten_subquery, CN_XF_FLATTEN_SUBQUERY)
__vDeleteFunction(XF_Flatten_subquery, CN_XF_FLATTEN_SUBQUERY)
__aFooterFunction(XF_Flatten_subquery)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Flatten_subquery::XF_Flatten_subquery(CardIter* pCI)
: XF_FLATTEN(CN_XF_FLATTEN_SUBQUERY,pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_SUBQUERY, _FOOTER_);

  m_pCI = pCI;

  _checkInitialised_(CN_XF_FLATTEN_SUBQUERY);

  m_pCI->vUseBoard(nullptr);


  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_SUBQUERY, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Flatten_subquery::~XF_Flatten_subquery(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_SUBQUERY, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten_subquery::vFlatten(CardIter* pCI)
{
  _static_

  XF_Flatten_subquery* pMakeFlat = new XF_Flatten_subquery(pCI);

  pMakeFlat->vFlatten();

  delete pMakeFlat;

}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten_subquery::vFlatten(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_SUBQUERY, _FOOTER_);

  m_pCI->vUseBoard(nullptr);

  vFlatten_recurse(nullptr,0);

  XF_Flatten_Delete_fold::vXF_Delete_fold(m_pCI);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten_subquery::vFlatten_recurse(Symbol* pSym_sqyNo_inner, i8 iDepth)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_SUBQUERY, _FOOTER_);

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
    vFlattenByDecisionTree(pSym_sqyNo_nested);
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
      vFlattenByDecisionTree(pSym_sqyNo_nested);

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
      vFlattenByDecisionTree(pSym_sqyNo_nested);

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
  XF_Flatten_Join_Ordering::vXF_Join_Ordering(m_pCI);
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

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten_subquery::vFlattenByDecisionTree(Symbol* pSym_sqyNo_inner)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_SUBQUERY, _FOOTER_);

  m_pCI->Flat.m_pSqy_inner = m_pCI->m_pBench->pFindSubQuery(pSym_sqyNo_inner);
  SubQuery* pIQ_called = m_pCI->Flat.m_pSqy_inner;

  if (pIQ_called->bViaFrom())                /* F       Q7,8,9 */ XF_Flatten_Fold_subquery::v10_F(m_pCI);
  else
  if (pIQ_called->bViaWhere()) {
    if (pIQ_called->bIsAggregation()) {
      if (pIQ_called->bIsCorrelated()) {     /* 20_ */
             if (pIQ_called->bByExists())    /* W_A_C_E        */ NEVER_REACH
        else if (pIQ_called->bByNotExists()) /* W_A_C_NE       */ NEVER_REACH
        else if (pIQ_called->bByIn())        /* W_A_C_I        */ NEVER_REACH
        else if (pIQ_called->bByNotIn())     /* W_A_C_NI       */ NEVER_REACH
        else                                 /* W_A_C_ro Q2,17 */ XF_Flatten_Fold_subquery::v24_W_A_C_ro(m_pCI);
      } else {                               /* 30_ */
             if (pIQ_called->bByExists())    /* W_A_NC_E       */ NEVER_REACH
        else if (pIQ_called->bByNotExists()) /* W_A_NC_NE      */ NEVER_REACH
        else if (pIQ_called->bByIn())        /* W_A_NC_I   Q18 */ XF_Flatten_Fold_subquery::v32_W_A_NC_I(m_pCI);
        else if (pIQ_called->bByNotIn())     /* W_A_NC_NI      */ NEVER_REACH
        else                                 /* W_A_NC_ro  Q15 */ XF_Flatten_Fold_subquery::v34_W_A_NC_ro(m_pCI);
      }
    } else {
      if (pIQ_called->bIsCorrelated()) {     /* 40_ */
             if (pIQ_called->bByExists())    /* W_L_C_E     Q4 */ XF_Flatten_Fold_subquery::v40_W_L_C_E(m_pCI);
        else if (pIQ_called->bByNotExists()) /* W_L_C_NE       */ NEVER_REACH
        else if (pIQ_called->bByIn())        /* W_L_C_I        */ NEVER_REACH
        else if (pIQ_called->bByNotIn())     /* W_L_C_NI       */ NEVER_REACH
        else                                 /* W_L_C_ro       */ NEVER_REACH
      } else {                               /* 50_ */
             if (pIQ_called->bByExists())    /* W_L_NC_E       */ NEVER_REACH
        else if (pIQ_called->bByNotExists()) /* W_L_NC_NE      */ NEVER_REACH
        else if (pIQ_called->bByIn())        /* W_L_NC_I       */ XF_Flatten_Fold_subquery::v52_W_L_NC_I_join(m_pCI);
        else if (pIQ_called->bByNotIn())     /* W_L_NC_NI  Q16 */ XF_Flatten_53_W_L_NC_NI_join::v53_W_L_NC_NI_join(
                   m_pCI);
        else                                 /* W_L_NC_ro      */ NEVER_REACH
      }
    }
  }
  else
  if (pIQ_called->bViaHaving()) {
    if (pIQ_called->bIsAggregation()) {
      if (pIQ_called->bIsCorrelated()) {     /* 60_ */
             if (pIQ_called->bByExists())    /* H_A_C_E        */ NEVER_REACH
        else if (pIQ_called->bByNotExists()) /* H_A_C_NE       */ NEVER_REACH
        else if (pIQ_called->bByIn())        /* H_A_C_I        */ NEVER_REACH
        else if (pIQ_called->bByNotIn())     /* H_A_C_NI       */ NEVER_REACH
        else                                 /* H_A_C_ro       */ NEVER_REACH
      } else {                               /* 70_ */
             if (pIQ_called->bByExists())    /* H_A_NC_E       */ NEVER_REACH
        else if (pIQ_called->bByNotExists()) /* H_A_NC_NE      */ NEVER_REACH
        else if (pIQ_called->bByIn())        /* H_A_NC_I       */ NEVER_REACH
        else if (pIQ_called->bByNotIn())     /* H_A_NC_NI      */ NEVER_REACH
        else                                 /* H_A_NC_ro  Q11 */ XF_Flatten_Fold_subquery::v74_H_A_NC_ro(m_pCI);
      }
    } else {
      if (pIQ_called->bIsCorrelated()) {     /* 80_ */
             if (pIQ_called->bByExists())    /* H_L_C_E        */ NEVER_REACH
        else if (pIQ_called->bByNotExists()) /* H_L_C_NE       */ NEVER_REACH
        else if (pIQ_called->bByIn())        /* H_L_C_I        */ NEVER_REACH
        else if (pIQ_called->bByNotIn())     /* H_L_C_NI       */ NEVER_REACH
        else                                 /* H_L_C_ro       */ NEVER_REACH
      } else {                               /* 90_ */
             if (pIQ_called->bByExists())    /* H_L_NC_E       */ NEVER_REACH
        else if (pIQ_called->bByNotExists()) /* H_L_NC_NE      */ NEVER_REACH
        else if (pIQ_called->bByIn())        /* H_L_NC_I       */ NEVER_REACH
        else if (pIQ_called->bByNotIn())     /* H_L_NC_NI      */ NEVER_REACH
        else                                 /* H_L_NC_ro      */ NEVER_REACH
      }
    }
  }
  else
    NEVER_REACH;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/