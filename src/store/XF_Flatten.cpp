/*============================================================================*/
/* XF_Flatten.cpp -- Flatten SubQueries                                */
/*============================================================================*/

#include "XF_Flatten.hpp"

#include "Board.hpp"
#include "CardIter.hpp"
#include "Optimiser.hpp"
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

#include "XF_stmCol_To_tblCol.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Flatten, CN_XF_FLATTEN)
__aNewFunction(XF_Flatten, CN_XF_FLATTEN)
__vDeleteFunction(XF_Flatten, CN_XF_FLATTEN)
__aFooterFunction(XF_Flatten)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Flatten::XF_Flatten(CardIter* pCI)
: XF_TRANSFORM(CN_XF_FLATTEN)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN, _FOOTER_);

  m_pCI = pCI;

  _checkInitialised_(CN_XF_FLATTEN);

  m_pCI->vUsingBoard((i8)0);
  m_pCI->Sqy.m_pSubQuery = m_pCI->G.pBoard->pSubQuery();


  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Flatten::~XF_Flatten(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten::vFlatten(CardIter* pCI)
{
  _static_

  XF_Flatten* pMakeFlat = new XF_Flatten(pCI);

  pMakeFlat->vFlatten();

  delete pMakeFlat;

}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten::vFlatten(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN, _FOOTER_);

  m_pCI->G.pBoard = m_pCI->m_pBench->pGetBoard((i8)0);
  m_pCI->Sqy.m_pSubQuery = m_pCI->G.pBoard->pSubQuery();

//  vFlatten_recurse();
  vFlatten_recurse(0);

  XF_Flatten_Delete_flat::vXF_Delete_flat(m_pCI);

  m_pCI->m_pBench->vMadeFlat(true);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten::vFlatten_recurse(i8 iDepth)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN, _FOOTER_);

  // things before sub-queries


  XF_stmCol_To_tblCol::vXF_stmCol_To_tblCol(m_pCI);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // flatten the sub-queries of this outer query

  for (;;) {

    m_pCI->vBeginEach();
    if (! m_pCI->G_bEachMain(CK_CALL))
      break;

    s256 sSubQueryNm = m_pCI->G.p_call->szSymbol(SF_NONE);

    Board *pOuterBoard_save = m_pCI->G.pBoard;
    m_pCI->Sqy.m_pSubQuery = m_pCI->m_pBench->pGetSubQuery(sSubQueryNm);
    m_pCI->G_vPushBoard(m_pCI->Sqy.m_pSubQuery->pBoard());
    m_pCI->Sqy.m_pSubQuery = m_pCI->G.pBoard->pSubQuery();

    vFlatten_recurse(iDepth + 1);

    m_pCI->G_vPopIter();
    m_pCI->G_vSetBoard(pOuterBoard_save);
    SubQuery *pSubQuery = m_pCI->Sqy.m_pSubQuery = m_pCI->m_pBench->pGetSubQuery(sSubQueryNm);
    UNUSED(pSubQuery);
    m_pCI->vPrintBoard(__LINE__);

    vFlattenByDecisionTree();

    m_pCI->vPrintBoard(__LINE__);
  }

  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

  bool bFound;

  do {
    m_pCI->vBeginEach();

    if (! m_pCI->G_bEachMainWithSub_reverse_foldBreak(CK_WHERE, CK_BOOLCALC))
      break;

    CardIter* pCI = m_pCI;

    auto bLambdaFn = [&bFound,this,pCI, iDepth](Card_Term* p_term)
    {
       if (!pCI->G_bIsBoardNm(p_term->szSymbol(SF_NONE)))
         return C_CONTINUE_RECURSION;

      s256 sSubQueryNm = p_term->szSymbol(SF_NONE);

      Board* pOuterBoard_save = pCI->G.pBoard;
      pCI->Sqy.m_pSubQuery = pCI->m_pBench->pGetSubQuery(sSubQueryNm);
      pCI->G_vPushBoard(pCI->Sqy.m_pSubQuery->pBoard());
      pCI->Sqy.m_pSubQuery = pCI->G.pBoard->pSubQuery();

      vFlatten_recurse(iDepth + 1);

      pCI->G_vPopIter();
      pCI->G_vSetBoard(pOuterBoard_save);
      SubQuery* pSubQuery = pCI->Sqy.m_pSubQuery = pCI->m_pBench->pGetSubQuery(sSubQueryNm);
      UNUSED(pSubQuery);
      pCI->vPrintBoard(__LINE__);

      vFlattenByDecisionTree();

      pCI->vPrintBoard(__LINE__);

      bFound = true;
      return C_TERMINATE_RECURSION;
    };


    bFound = false;
    bool b = m_pCI->G.p_boolCalc->bApplyRecursive_term(bLambdaFn);
    UNUSED(b);
    vDebugPoint();
  } while (bFound);


  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

  do {
    m_pCI->vBeginEach();

    if (! m_pCI->G_bEachMainWithSub_reverse_foldBreak(CK_HAVING, CK_BOOLCALC))
      break;

    CardIter* pCI = m_pCI;

    auto bLambdaFn = [&bFound,this,pCI, iDepth](Card_Term* p_term)
    {
      if (!pCI->G_bIsBoardNm(p_term->szSymbol(SF_NONE)))
        return C_CONTINUE_RECURSION;

      s256 sSubQueryNm = p_term->szSymbol(SF_NONE);

      Board* pOuterBoard_save = pCI->G.pBoard;
      pCI->Sqy.m_pSubQuery = pCI->m_pBench->pGetSubQuery(sSubQueryNm);
      pCI->G_vPushBoard(pCI->Sqy.m_pSubQuery->pBoard());
      pCI->Sqy.m_pSubQuery = pCI->G.pBoard->pSubQuery();

      vFlatten_recurse(iDepth + 1);

      pCI->G_vPopIter();
      pCI->G_vSetBoard(pOuterBoard_save);
      SubQuery* pSubQuery = pCI->Sqy.m_pSubQuery = pCI->m_pBench->pGetSubQuery(sSubQueryNm);
      UNUSED(pSubQuery);
      pCI->vPrintBoard(__LINE__);

      vFlattenByDecisionTree();

      pCI->vPrintBoard(__LINE__);

      bFound = true;
      return C_TERMINATE_RECURSION;
    };


    bFound = false;
    bool b = m_pCI->G.p_boolCalc->bApplyRecursive_term(bLambdaFn);
    UNUSED(b);
    vDebugPoint();
  } while (bFound);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // things to do to this outer query after flattening it's sub-queries

  XF_Flatten_Join_Ordering::vXF_Join_Ordering(m_pCI);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // sub-query has been flattened

  Card_Fold::vMorph_to_flat(m_pCI);

  m_pCI->vPrintBoard(__LINE__);

  vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten::vFlattenByDecisionTree(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN, _FOOTER_);

  m_pCI->vTransform(__func__, XF_FLATTEN, "Flatten by decision tree (recursive model)"_Stz);

  if (m_pCI->Sqy.m_pSubQuery->bViaFrom())                /* Fsq_      Q7,8,9 */ XF_Flatten_10_F::vXF_Flatten_10_F(m_pCI);
  else
  if (m_pCI->Sqy.m_pSubQuery->bViaWhere()) {
    if (m_pCI->Sqy.m_pSubQuery->bIsAggregation()) {
      if (m_pCI->Sqy.m_pSubQuery->bIsCorrelated()) {                                /* 20*/
             if (m_pCI->Sqy.m_pSubQuery->bByExists())    /* W_A_C_E          */ NEVER_REACH
        else if (m_pCI->Sqy.m_pSubQuery->bByNotExists()) /* W_A_C_NE         */ NEVER_REACH
        else if (m_pCI->Sqy.m_pSubQuery->bByIn())        /* W_A_C_I          */ NEVER_REACH
        else if (m_pCI->Sqy.m_pSubQuery->bByNotIn())     /* W_A_C_NI         */ NEVER_REACH
        else                                             /* W_A_C_ro   Q2,17 */ XF_Flatten_24_W_A_C_ro::vXF_Flatten_24_W_A_C_ro(m_pCI);
      } else {                                                                      /* 30*/
             if (m_pCI->Sqy.m_pSubQuery->bByExists())    /* W_A_NC_E         */ NEVER_REACH
        else if (m_pCI->Sqy.m_pSubQuery->bByNotExists()) /* W_A_NC_NE        */ NEVER_REACH
        else if (m_pCI->Sqy.m_pSubQuery->bByIn())        /* W_A_NC_I     Q18 */ XF_Flatten_32_W_A_NC_I::vXF_Flatten_32_W_A_NC_I(m_pCI);
        else if (m_pCI->Sqy.m_pSubQuery->bByNotIn())     /* W_A_NC_NI        */ NEVER_REACH
        else                                             /* W_A_NC_ro    Q15 */ XF_Flatten_34_W_A_NC_ro::vXF_Flatten_34_W_A_NC_ro(m_pCI);
      }
    } else {
      if (m_pCI->Sqy.m_pSubQuery->bIsCorrelated()) {                                /* 40*/
             if (m_pCI->Sqy.m_pSubQuery->bByExists())    /* W_L_C_E       Q4 */ XF_Flatten_40_W_L_C_E::vXF_Flatten_40_W_L_C_E(m_pCI);
        else if (m_pCI->Sqy.m_pSubQuery->bByNotExists()) /* W_L_C_NE         */ NEVER_REACH
        else if (m_pCI->Sqy.m_pSubQuery->bByIn())        /* W_L_C_I          */ NEVER_REACH
        else if (m_pCI->Sqy.m_pSubQuery->bByNotIn())     /* W_L_C_NI         */ NEVER_REACH
        else                                             /* W_L_C_ro         */ NEVER_REACH
      } else {                                                                      /* 50*/
             if (m_pCI->Sqy.m_pSubQuery->bByExists())    /* W_L_NC_E         */ NEVER_REACH
        else if (m_pCI->Sqy.m_pSubQuery->bByNotExists()) /* W_L_NC_NE        */ NEVER_REACH
        else if (m_pCI->Sqy.m_pSubQuery->bByIn())        /* W_L_NC_I         */ XF_Flatten_52_W_L_NC_I_join::vXF_Flatten_52_W_L_NC_I_join(m_pCI);
        else if (m_pCI->Sqy.m_pSubQuery->bByNotIn())     /* W_L_NC_NI    Q16 */ XF_Flatten_53_W_L_NC_NI_join::vXF_Flatten_53_W_L_NC_NI_join(m_pCI);
        else                                             /* W_L_NC_ro        */ NEVER_REACH
      }
    }
  }
  else
  if (m_pCI->Sqy.m_pSubQuery->bViaHaving()) {
    if (m_pCI->Sqy.m_pSubQuery->bIsAggregation()) {
      if (m_pCI->Sqy.m_pSubQuery->bIsCorrelated()) {                                /* 60*/
             if (m_pCI->Sqy.m_pSubQuery->bByExists())    /* H_A_C_E          */ NEVER_REACH
        else if (m_pCI->Sqy.m_pSubQuery->bByNotExists()) /* H_A_C_NE         */ NEVER_REACH
        else if (m_pCI->Sqy.m_pSubQuery->bByIn())        /* H_A_C_I          */ NEVER_REACH
        else if (m_pCI->Sqy.m_pSubQuery->bByNotIn())     /* H_A_C_NI         */ NEVER_REACH
        else                                             /* H_A_C_ro         */ NEVER_REACH
      } else {                                                                      /* 70*/
             if (m_pCI->Sqy.m_pSubQuery->bByExists())    /* H_A_NC_E         */ NEVER_REACH
        else if (m_pCI->Sqy.m_pSubQuery->bByNotExists()) /* H_A_NC_NE        */ NEVER_REACH
        else if (m_pCI->Sqy.m_pSubQuery->bByIn())        /* H_A_NC_I         */ NEVER_REACH
        else if (m_pCI->Sqy.m_pSubQuery->bByNotIn())     /* H_A_NC_NI        */ NEVER_REACH
        else                                             /* H_A_NC_ro    Q11 */ XF_Flatten_74_H_A_NC_ro::vXF_Flatten_74_H_A_NC_ro(m_pCI);
      }
    } else {
      if (m_pCI->Sqy.m_pSubQuery->bIsCorrelated()) {                                /* 80*/
             if (m_pCI->Sqy.m_pSubQuery->bByExists())    /* H_L_C_E          */ NEVER_REACH
        else if (m_pCI->Sqy.m_pSubQuery->bByNotExists()) /* H_L_C_NE         */ NEVER_REACH
        else if (m_pCI->Sqy.m_pSubQuery->bByIn())        /* H_L_C_I          */ NEVER_REACH
        else if (m_pCI->Sqy.m_pSubQuery->bByNotIn())     /* H_L_C_NI         */ NEVER_REACH
        else                                             /* H_L_C_ro         */ NEVER_REACH
      } else {                                                                      /* 90*/
             if (m_pCI->Sqy.m_pSubQuery->bByExists())    /* H_L_NC_E         */ NEVER_REACH
        else if (m_pCI->Sqy.m_pSubQuery->bByNotExists()) /* H_L_NC_NE        */ NEVER_REACH
        else if (m_pCI->Sqy.m_pSubQuery->bByIn())        /* H_L_NC_I         */ NEVER_REACH
        else if (m_pCI->Sqy.m_pSubQuery->bByNotIn())     /* H_L_NC_NI        */ NEVER_REACH
        else                                             /* H_L_NC_ro        */ NEVER_REACH
      }
    }
  }
  else
    NEVER_REACH;

  m_pCI->vEndTransform();

  Bench::vValidate(m_pCI);

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/