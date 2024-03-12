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

  vFlatten_recurse(0);

  XF_Flatten_Delete_flat::vXF_Delete_flat(m_pCI);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten_subquery::vFlatten_recurse(i8 iDepth)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_SUBQUERY, _FOOTER_);

  // things before sub-queries


  XF_Flatten_X_expCol_To_tblCol::vFlatten(m_pCI);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // flatten the sub-queries of this outer query

  for (;;) {

    m_pCI->vBeginEach();
    if (! m_pCI->bEachMain(CK_o_CALL))
      break;

    Symbol* pSym_sqyNo_inner = m_pCI->G.p_call->pSymbol();

    Board *pBoard_outer = m_pCI->G.pBoard;
    SubQuery* pSqy_inner = m_pCI->m_pBench->pFindSubQuery(pSym_sqyNo_inner);
    m_pCI->vPushBoard(pSqy_inner->pBoard());

    vFlatten_recurse(iDepth + 1);

    m_pCI->vPopBoard();
    m_pCI->vUsingBoard(pBoard_outer);
    // m_pCI->PRINTBOARD_dbg();

    vFlattenByDecisionTree(pSym_sqyNo_inner);

    // m_pCI->PRINTBOARD_dbg();
  }

  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

  bool bFound;

  do {
    m_pCI->vBeginEachReverse();

    if (! m_pCI->bEachMainWithSub_reverse_foldBreak_b(CK_o_WHERE, CK_BOOLPRED))
      break;

    CardIter* pCI = m_pCI;

    auto bLambdaFn = [&bFound,this,pCI, iDepth](Card_Term* p_term)
    {
       if (! p_term->pSymbol()->bHasFacet(SF_SQY_NO))
         return C_CONTINUE_RECURSION;

      Symbol* pSym_sqyNo_inner = p_term->pSymbol();

      Board* pBoard_outer = pCI->G.pBoard;
      SubQuery* pSqy_inner = pCI->m_pBench->pFindSubQuery(pSym_sqyNo_inner);
      pCI->vPushBoard(pSqy_inner->pBoard());

      vFlatten_recurse(iDepth + 1);

      pCI->vPopBoard();
      pCI->vUsingBoard(pBoard_outer);
      // pCI->PRINTBOARD_dbg();

      vFlattenByDecisionTree(pSym_sqyNo_inner);

     // pCI->PRINTBOARD_dbg();

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

    CardIter* pCI = m_pCI;

    auto bLambdaFn = [&bFound,this,pCI, iDepth](Card_Term* p_term)
    {
      if (! p_term->pSymbol()->bHasFacet(SF_SQY_NO))
        return C_CONTINUE_RECURSION;

      Symbol* pSym_sqyNo_inner = p_term->pSymbol();

      Board* pBoard_outer = pCI->G.pBoard;
      SubQuery* pSqy_inner = pCI->m_pBench->pFindSubQuery(pSym_sqyNo_inner);
      pCI->vPushBoard(pSqy_inner->pBoard());

      vFlatten_recurse(iDepth + 1);

      pCI->vPopBoard();
      pCI->vUsingBoard(pBoard_outer);
     // pCI->PRINTBOARD_dbg();

      vFlattenByDecisionTree(pSym_sqyNo_inner);
      //pCI->PRINTBOARD_dbg();

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

  //RASSERT(0 == m_pCI->pBoard()->pDeck()->nChildCards(CK_SUBQUERY));

  XF_Flatten_Left_subQuery::vRemove_LeftMost(m_pCI);

  XF_Flatten_Join_Ordering::vXF_Join_Ordering(m_pCI);

  XF_Flatten_Left_subQuery::vRotate_Next(m_pCI);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // sub-query has been flattened

  Card_Fold::vMorph_to_flat(m_pCI);


  // m_pCI->PRINTBOARD_dbg();

  vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten_subquery::vFlattenByDecisionTree(Symbol* pSym_sqyNo_inner)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_SUBQUERY, _FOOTER_);

  SubQuery* pSubQuery = m_pCI->m_pBench->pFindSubQuery(pSym_sqyNo_inner);

//  m_pCI->vTransform(__FILE__, __func__, XF_FLATTEN_SUBQUERY);

  if (pSubQuery->bViaFrom())                /* Fsq_      Q7,8,9 */ XF_Flatten_10_F::vFlatten(m_pCI);
  else
  if (pSubQuery->bViaWhere()) {
    if (pSubQuery->bIsAggregation()) {
      if (pSubQuery->bIsCorrelated()) {                                /* 20*/
             if (pSubQuery->bByExists())    /* W_A_C_E          */ NEVER_REACH
        else if (pSubQuery->bByNotExists()) /* W_A_C_NE         */ NEVER_REACH
        else if (pSubQuery->bByIn())        /* W_A_C_I          */ NEVER_REACH
        else if (pSubQuery->bByNotIn())     /* W_A_C_NI         */ NEVER_REACH
        else                                             /* W_A_C_ro   Q2,17 */ XF_Flatten_24_W_A_C_ro::vFlatten(m_pCI);
      } else {                                                                      /* 30*/
             if (pSubQuery->bByExists())    /* W_A_NC_E         */ NEVER_REACH
        else if (pSubQuery->bByNotExists()) /* W_A_NC_NE        */ NEVER_REACH
        else if (pSubQuery->bByIn())        /* W_A_NC_I     Q18 */ XF_Flatten_32_W_A_NC_I::vFlatten(m_pCI);
        else if (pSubQuery->bByNotIn())     /* W_A_NC_NI        */ NEVER_REACH
        else                                             /* W_A_NC_ro    Q15 */ XF_Flatten_34_W_A_NC_ro::vFlatten(m_pCI);
      }
    } else {
      if (pSubQuery->bIsCorrelated()) {                                /* 40*/
             if (pSubQuery->bByExists())    /* W_L_C_E       Q4 */ XF_Flatten_40_W_L_C_E::vFlatten(m_pCI);
        else if (pSubQuery->bByNotExists()) /* W_L_C_NE         */ NEVER_REACH
        else if (pSubQuery->bByIn())        /* W_L_C_I          */ NEVER_REACH
        else if (pSubQuery->bByNotIn())     /* W_L_C_NI         */ NEVER_REACH
        else                                             /* W_L_C_ro         */ NEVER_REACH
      } else {                                                                      /* 50*/
             if (pSubQuery->bByExists())    /* W_L_NC_E         */ NEVER_REACH
        else if (pSubQuery->bByNotExists()) /* W_L_NC_NE        */ NEVER_REACH
        else if (pSubQuery->bByIn())        /* W_L_NC_I         */ XF_Flatten_52_W_L_NC_I_join::vFlatten(m_pCI);
        else if (pSubQuery->bByNotIn())     /* W_L_NC_NI    Q16 */ XF_Flatten_53_W_L_NC_NI_join::vFlatten(m_pCI);
        else                                             /* W_L_NC_ro        */ NEVER_REACH
      }
    }
  }
  else
  if (pSubQuery->bViaHaving()) {
    if (pSubQuery->bIsAggregation()) {
      if (pSubQuery->bIsCorrelated()) {                                /* 60*/
             if (pSubQuery->bByExists())    /* H_A_C_E          */ NEVER_REACH
        else if (pSubQuery->bByNotExists()) /* H_A_C_NE         */ NEVER_REACH
        else if (pSubQuery->bByIn())        /* H_A_C_I          */ NEVER_REACH
        else if (pSubQuery->bByNotIn())     /* H_A_C_NI         */ NEVER_REACH
        else                                             /* H_A_C_ro         */ NEVER_REACH
      } else {                                                                      /* 70*/
             if (pSubQuery->bByExists())    /* H_A_NC_E         */ NEVER_REACH
        else if (pSubQuery->bByNotExists()) /* H_A_NC_NE        */ NEVER_REACH
        else if (pSubQuery->bByIn())        /* H_A_NC_I         */ NEVER_REACH
        else if (pSubQuery->bByNotIn())     /* H_A_NC_NI        */ NEVER_REACH
        else                                             /* H_A_NC_ro    Q11 */ XF_Flatten_74_H_A_NC_ro::vFlatten(m_pCI);
      }
    } else {
      if (pSubQuery->bIsCorrelated()) {                                /* 80*/
             if (pSubQuery->bByExists())    /* H_L_C_E          */ NEVER_REACH
        else if (pSubQuery->bByNotExists()) /* H_L_C_NE         */ NEVER_REACH
        else if (pSubQuery->bByIn())        /* H_L_C_I          */ NEVER_REACH
        else if (pSubQuery->bByNotIn())     /* H_L_C_NI         */ NEVER_REACH
        else                                             /* H_L_C_ro         */ NEVER_REACH
      } else {                                                                      /* 90*/
             if (pSubQuery->bByExists())    /* H_L_NC_E         */ NEVER_REACH
        else if (pSubQuery->bByNotExists()) /* H_L_NC_NE        */ NEVER_REACH
        else if (pSubQuery->bByIn())        /* H_L_NC_I         */ NEVER_REACH
        else if (pSubQuery->bByNotIn())     /* H_L_NC_NI        */ NEVER_REACH
        else                                             /* H_L_NC_ro        */ NEVER_REACH
      }
    }
  }
  else
    NEVER_REACH;

//  m_pCI->vEndTransform(__LINE__);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/