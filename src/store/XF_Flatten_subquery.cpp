/*============================================================================*/
/* XF_Flatten_subquery.cpp -- Flatten SubQueries                                */
/*============================================================================*/

#include "XF_Flatten_subquery.hpp"

#include "Board.hpp"
#include "CardIter.hpp"
#include "Optimiser.hpp"

#include "eXF.h"
#include "XF_Flatten_Join_Ordering.hpp"

#include "XF_Flatten_10_F.hpp"
#include "XF_Flatten_24_W_A_C_ro.hpp"
#include "XF_Flatten_32_W_A_NC_I.hpp"
#include "XF_Flatten_34_W_A_NC_ro.hpp"
#include "XF_Flatten_40_W_L_C_E.hpp"
#include "XF_Flatten_52_W_L_NC_I_exists.hpp"
#include "XF_Flatten_52_W_L_NC_I_join.hpp"
#include "XF_Flatten_53_W_L_NC_NI_exists.hpp"
#include "XF_Flatten_53_W_L_NC_NI_join.hpp"
#include "XF_Flatten_74_H_A_NC_ro.hpp"

#include "XF_Flatten_Delete_flat.hpp"
#include "XF_Flatten_Left_subQuery.hpp"

#include "XF_Flatten_X_expCol_To_tblCol.hpp"

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
: XF_TRANSFORM(CN_XF_FLATTEN_SUBQUERY)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_SUBQUERY, _FOOTER_);

  m_pCI = pCI;

  _checkInitialised_(CN_XF_FLATTEN_SUBQUERY);

  m_pCI->vUsingBoard((i8)0);


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

  m_pCI->vUsingBoard((i8)0);

  vFlatten_recurse(nullptr,0);

  XF_Flatten_Delete_flat::vXF_Delete_flat(m_pCI);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten_subquery::vFlatten_recurse(Symbol* pSym_sqyNo_inner, i8 iDepth)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_SUBQUERY, _FOOTER_);

  Board *pBoard_outer = nullptr;
  if (iDepth) {
    pBoard_outer = m_pCI->G.pBoard;
    SubQuery *pSqy_inner = m_pCI->m_pBench->pFindSubQuery(pSym_sqyNo_inner);
    m_pCI->vPushBoard(pSqy_inner->pBoard());
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
    if (! m_pCI->bEachMain(CK_o_CALL))
      break;

    Symbol* pSym_sqyNo_nested = m_pCI->G.p_call->pSymbol();
    vFlatten_recurse(pSym_sqyNo_nested, iDepth + 1);
    vFlattenByDecisionTree(pSym_sqyNo_nested);
  }

  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

  bool bFound;

  do {
    m_pCI->vBeginEachReverse();
    if (! m_pCI->bEachMainWithSub_reverse_foldBreak_b(CK_o_WHERE, CK_BOOLPRED))
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
    bool b = m_pCI->G.p_boolPred->bLambdaWalk_term(bLambdaFn);
    UNUSED(b);
    vDebugPoint();
  } while (bFound);


  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

  do {
    m_pCI->vBeginEachReverse();

    if (! m_pCI->bEachMainWithSub_reverse_foldBreak_b(CK_o_HAVING, CK_BOOLPRED))
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
    bool b = m_pCI->G.p_boolPred->bLambdaWalk_term(bLambdaFn);
    UNUSED(b);
    vDebugPoint();
  } while (bFound);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // things to do to this outer query after flattening it's sub-queries

  XF_Flatten_Left_subQuery::vRemove_LeftMost(m_pCI);
  XF_Flatten_Join_Ordering::vXF_Join_Ordering(m_pCI);
  XF_Flatten_Left_subQuery::vRotate_Next(m_pCI);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // sub-query has been flattened

  Card_Fold::vMorph_to_flat(m_pCI);

  // m_pCI->PRINTBOARD_dbg();

  // ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^
  // ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^
  // ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^

  if (iDepth) {
    m_pCI->vPopBoard();
    m_pCI->vUsingBoard(pBoard_outer);
  }

  vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten_subquery::vFlattenByDecisionTree(Symbol* pSym_sqyNo_inner)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_SUBQUERY, _FOOTER_);

  SubQuery* pSqy_called = m_pCI->m_pBench->pFindSubQuery(pSym_sqyNo_inner);

  if (pSqy_called->bViaFrom())                /* Fsq_      Q7,8,9 */ XF_Flatten_10_F::vFlatten(m_pCI);
  else
  if (pSqy_called->bViaWhere()) {
    if (pSqy_called->bIsAggregation()) {
      if (pSqy_called->bIsCorrelated()) {     /* 20*/
             if (pSqy_called->bByExists())    /* W_A_C_E          */ NEVER_REACH
        else if (pSqy_called->bByNotExists()) /* W_A_C_NE         */ NEVER_REACH
        else if (pSqy_called->bByIn())        /* W_A_C_I          */ NEVER_REACH
        else if (pSqy_called->bByNotIn())     /* W_A_C_NI         */ NEVER_REACH
        else                                  /* W_A_C_ro   Q2,17 */ XF_Flatten_24_W_A_C_ro::vFlatten(m_pCI);
      } else {                                /* 30*/
             if (pSqy_called->bByExists())    /* W_A_NC_E         */ NEVER_REACH
        else if (pSqy_called->bByNotExists()) /* W_A_NC_NE        */ NEVER_REACH
        else if (pSqy_called->bByIn())        /* W_A_NC_I     Q18 */ XF_Flatten_32_W_A_NC_I::vFlatten(m_pCI);
        else if (pSqy_called->bByNotIn())     /* W_A_NC_NI        */ NEVER_REACH
        else                                  /* W_A_NC_ro    Q15 */ XF_Flatten_34_W_A_NC_ro::vFlatten(m_pCI);
      }
    } else {
      if (pSqy_called->bIsCorrelated()) {     /* 40*/
             if (pSqy_called->bByExists())    /* W_L_C_E       Q4 */ XF_Flatten_40_W_L_C_E::vFlatten(m_pCI);
        else if (pSqy_called->bByNotExists()) /* W_L_C_NE         */ NEVER_REACH
        else if (pSqy_called->bByIn())        /* W_L_C_I          */ NEVER_REACH
        else if (pSqy_called->bByNotIn())     /* W_L_C_NI         */ NEVER_REACH
        else                                  /* W_L_C_ro         */ NEVER_REACH
      } else {                                /* 50*/
             if (pSqy_called->bByExists())    /* W_L_NC_E         */ NEVER_REACH
        else if (pSqy_called->bByNotExists()) /* W_L_NC_NE        */ NEVER_REACH
        else if (pSqy_called->bByIn())        /* W_L_NC_I         */ XF_Flatten_52_W_L_NC_I_join::vFlatten(m_pCI);
        else if (pSqy_called->bByNotIn())     /* W_L_NC_NI    Q16 */ XF_Flatten_53_W_L_NC_NI_join::vFlatten(m_pCI);
        else                                  /* W_L_NC_ro        */ NEVER_REACH
      }
    }
  }
  else
  if (pSqy_called->bViaHaving()) {
    if (pSqy_called->bIsAggregation()) {
      if (pSqy_called->bIsCorrelated()) {     /* 60*/
             if (pSqy_called->bByExists())    /* H_A_C_E          */ NEVER_REACH
        else if (pSqy_called->bByNotExists()) /* H_A_C_NE         */ NEVER_REACH
        else if (pSqy_called->bByIn())        /* H_A_C_I          */ NEVER_REACH
        else if (pSqy_called->bByNotIn())     /* H_A_C_NI         */ NEVER_REACH
        else                                  /* H_A_C_ro         */ NEVER_REACH
      } else {                                /* 70*/
             if (pSqy_called->bByExists())    /* H_A_NC_E         */ NEVER_REACH
        else if (pSqy_called->bByNotExists()) /* H_A_NC_NE        */ NEVER_REACH
        else if (pSqy_called->bByIn())        /* H_A_NC_I         */ NEVER_REACH
        else if (pSqy_called->bByNotIn())     /* H_A_NC_NI        */ NEVER_REACH
        else                                  /* H_A_NC_ro    Q11 */ XF_Flatten_74_H_A_NC_ro::vFlatten(m_pCI);
      }
    } else {
      if (pSqy_called->bIsCorrelated()) {     /* 80*/
             if (pSqy_called->bByExists())    /* H_L_C_E          */ NEVER_REACH
        else if (pSqy_called->bByNotExists()) /* H_L_C_NE         */ NEVER_REACH
        else if (pSqy_called->bByIn())        /* H_L_C_I          */ NEVER_REACH
        else if (pSqy_called->bByNotIn())     /* H_L_C_NI         */ NEVER_REACH
        else                                  /* H_L_C_ro         */ NEVER_REACH
      } else {                                /* 90*/
             if (pSqy_called->bByExists())    /* H_L_NC_E         */ NEVER_REACH
        else if (pSqy_called->bByNotExists()) /* H_L_NC_NE        */ NEVER_REACH
        else if (pSqy_called->bByIn())        /* H_L_NC_I         */ NEVER_REACH
        else if (pSqy_called->bByNotIn())     /* H_L_NC_NI        */ NEVER_REACH
        else                                  /* H_L_NC_ro        */ NEVER_REACH
      }
    }
  }
  else
    NEVER_REACH;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/