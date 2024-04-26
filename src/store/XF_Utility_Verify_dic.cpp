/*============================================================================*/
/* XF_Utility_Verify_dic.cpp -- Verify dic                                    */
/*============================================================================*/

#include "XF_Utility_Verify_dic.hpp"

#include "Error.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Utility_Verify_dic, CN_XF_PROLOG_VALIDATE_BOARD)
__aNewFunction(XF_Utility_Verify_dic, CN_XF_PROLOG_VALIDATE_BOARD)
__vDeleteFunction(XF_Utility_Verify_dic, CN_XF_PROLOG_VALIDATE_BOARD)
__aFooterFunction(XF_Utility_Verify_dic)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Utility_Verify_dic::XF_Utility_Verify_dic(CardIter* pCI)
: XF_PROLOG(CN_XF_PROLOG_VALIDATE_BOARD, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_VALIDATE_BOARD, _FOOTER_);

  _checkInitialised_(CN_XF_PROLOG_VALIDATE_BOARD);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_VALIDATE_BOARD, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Utility_Verify_dic::~XF_Utility_Verify_dic(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_VALIDATE_BOARD, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Utility_Verify_dic::vVerify(CardIter* pCI)
{
  _static_

  XF_Utility_Verify_dic* pXF_Validate = new XF_Utility_Verify_dic(pCI);

  try {
    pXF_Validate->vVerify_dic_loop();
  }
  catch (Error xError) {
    delete pXF_Validate;
    throw xError;
  }
  delete pXF_Validate;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Utility_Verify_dic::vVerify_dic_loop(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_VALIDATE_BOARD, _FOOTER_);

  m_pCI->vTransform(__FILE__, __func__, XF_UTILITY_VERIFY_DIC);

  m_pCI->vClearRegisters();
  m_pCI->vBeginEach();

  while (m_pCI->bEachOp()) {

    vVerify_dic_OP();

    m_pCI->vSnapIf_arriving();
  }

  m_pCI->vEndTransform(__LINE__);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Utility_Verify_dic::vVerify_dic_OP(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_VALIDATE_BOARD, _FOOTER_);

  //===========================================================================
  // phase 1 : aspects
  //===========================================================================

  RASSERT(m_pCI->E.p_Op->bIsOp());
  switch (m_pCI->E.e_Op) {

    case CK_o_AGGREG:
    case CK_o_CALL:
    case CK_o_CAST:
    case CK_o_CORRELATION:
    case CK_o_CROSSJOIN:
    case CK_o_FILTER:
    case CK_o_FROM:
    case CK_o_GBYAGG:
    case CK_o_HASH:
    case CK_o_HAVING:
    case CK_o_INDEXSCAN:
    case CK_o_INDEXJOIN:
    case CK_o_INFLATE:
    case CK_o_KEYEXISTS:
    case CK_o_KEYNOTEXISTS:
    case CK_o_LIMIT:
    case CK_o_ORDAGG:
    case CK_o_PROJECT:
    case CK_o_SCAN:
    case CK_o_SORT:
    case CK_o_TOP:
    case CK_o_WHERE:
      break;

    default:
      NEVER_REACH
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // aspects 1 filter

  if ((m_pCI->E.p_filter = m_pCI->E.p_Op->pSub_filter())) {
    RASSERT(m_pCI->E.p_Op->nSubCards(CK_o_FILTER) == 1);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // aspects 1 injectable

  if ((m_pCI->E.p_injectable = m_pCI->E.p_Op->pSub_injectable())) {
    RASSERT(m_pCI->E.p_Op->nSubCards(CK_c_INJECTABLE) == 1);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // aspects 1 key

  if ((m_pCI->E.p_key = m_pCI->E.p_Op->pSub_key())) {
    RASSERT(m_pCI->E.p_Op->nSubCards(CK_c_KEY) == 1);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // aspects 1 constraint

  if ((m_pCI->E.p_constraint = m_pCI->E.p_Op->pSub_constraint())) {
    RASSERT(m_pCI->E.p_Op->nSubCards(CK_c_CONSTRAINT) == 1);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // aspects 1 hshColList

  if ((m_pCI->E.p_hshColList = m_pCI->E.p_Op->pSub_hshColList())) {
    RASSERT(m_pCI->E.p_Op->nSubCards(CK_c_HSHCOLLIST) == 1);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // aspects 1 indexList

  if ((m_pCI->E.p_indexList = m_pCI->E.p_Op->pSub_indexList())) {
    RASSERT(m_pCI->E.p_Op->nSubCards(CK_INDEXLIST) == 1);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // aspects 1 emit

  if ((m_pCI->E.p_emit = m_pCI->E.p_Op->pSub_emit())) {
    RASSERT(m_pCI->E.p_Op->nSubCards(CK_c_EMIT) == 1);
  }

  vDebugPoint();

  //===========================================================================
  // phase 2 : facets
  //===========================================================================

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // facets 2 injectable

  if ((m_pCI->E.p_injectable = m_pCI->E.p_Op->pSub_injectable())) {
    for (Kid xInject(m_pCI->E.p_injectable); xInject.bEach(CK_EXPR); /**/) {
      m_pCI->E.p_expr = xInject.p_expr();
      //m_pCI->G.p_expr->vDump(__func__);
      RASSERT(m_pCI->E.p_expr->pSymbol()->bHasFacets(SF_EXP_COL,
                                                     SF_NMD_COL,
                                                     SF_NONE));
      RASSERT(m_pCI->E.p_expr->bIsScalarCol());
      for (m_pCI->vBeginTarget(m_pCI->E.p_expr,CK_TERM); m_pCI->bEachTarget(); /**/) {
        m_pCI->E.p_term = m_pCI->E.xTarget.p_term();
        RASSERT(m_pCI->E.p_term->pSymbol()->bSingleFaceted(SF_TBL_COL));
      }
    }
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // facets 2 key

  if ((m_pCI->E.p_key = m_pCI->E.p_Op->pSub_key())) {
    for (Kid xKey(m_pCI->E.p_key); xKey.bEach(CK_EXPR); /**/) {
      m_pCI->E.p_expr = xKey.p_expr();
      //m_pCI->G.p_expr->vDump(__func__);
      RASSERT(m_pCI->E.p_expr->pSymbol()->bHasFacets(SF_EXP_COL,
                                                     SF_NMD_COL,
                                                     SF_TBL_COL,
                                                     SF_NONE)
              ||
              m_pCI->E.p_expr->pSymbol()->bHasFacets(SF_KEY_COL,
                                                     SF_EXP_COL,
                                                     SF_TBL_COL,
                                                     SF_NONE)
              ||
              m_pCI->E.p_expr->pSymbol()->bHasFacets(SF_KEY_COL,
                                                     SF_TBL_COL,
                                                     SF_NONE)
              ||
              m_pCI->E.p_expr->pSymbol()->bHasFacets(SF_KEY_COL,
                                                     SF_NONE));

      for (m_pCI->vBeginTarget(m_pCI->E.p_expr,CK_TERM); m_pCI->bEachTarget(); /**/) {
        m_pCI->E.p_term = m_pCI->E.xTarget.p_term();
        RASSERT(m_pCI->E.p_term->pSymbol()->bSingleFaceted(SF_TBL_COL)
                ||
                m_pCI->E.p_term->pSymbol()->bSingleFaceted(SF_EXP_COL)
                ||
                m_pCI->E.p_term->pSymbol()->bHasFacets(SF_TBL_COL,
                                                       SF_NMD_COL,
                                                       SF_NONE) );
      }
    }
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // facets 2 indexList

  if ((m_pCI->E.p_indexList = m_pCI->E.p_Op->pSub_indexList())) {
    for (Kid xIndexList(m_pCI->E.p_indexList); xIndexList.bEach(CK_INDEX); /**/) {
      m_pCI->E.p_index = xIndexList.p_index();
      //m_pCI->G.p_index->vDump(__func__);

      // indexList/idxColList
      m_pCI->E.p_idxColList = m_pCI->E.p_index->pSub_idxColList(C_NOT_NULL);
      for (Kid xIdxColList(m_pCI->E.p_idxColList); xIdxColList.bEach(CK_IDXCOL); /**/) {
        m_pCI->E.p_idxCol = xIdxColList.p_idxCol();
        RASSERT(m_pCI->E.p_idxCol->pSymbol()->bHasFacets(SF_NMD_COL,
                                                         SF_TBL_COL,
                                                         SF_NONE));
      }

      // indexList/bounds
      if (!(m_pCI->E.p_bounds = m_pCI->E.p_index->pSub_bounds()))
        continue;
      for (Kid xBound(m_pCI->E.p_bounds); xBound.bEach(CK_BOUND); /**/) {
        m_pCI->E.p_bound = xBound.p_bound();
        //m_pCI->G.p_bound->vDump(__func__);
        vDebugPoint();
        for (Kid xBound(m_pCI->E.p_bound); xBound.bEach(CK_TERM); /**/) {
          m_pCI->E.p_term = xBound.p_term();
          if (! m_pCI->E.p_term->pSymbol()->bIs_Col())
            continue;

          RASSERT(m_pCI->E.p_term->pSymbol()->bHasFacet(SF_IDX_COL)
                  ||
                  m_pCI->E.p_term->pSymbol()->bHasFacet(SF_TBL_COL)
                  ||
                  m_pCI->E.p_term->pSymbol()->bHasFacet(SF_EXP_COL)
                  ||
                  m_pCI->E.p_term->pSymbol()->bIs_Lit()
                  );
        }
      }
    }

    vDebugPoint();
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // facets 2 constraint

  if ((m_pCI->E.p_constraint = m_pCI->E.p_Op->pSub_constraint())) {
    //m_pCI->G.p_constraint->vDump(__func__);
    m_pCI->E.p_predicate = m_pCI->E.p_constraint->pSub_predicate(C_NOT_NULL);

    auto bLambdaFn = [](Card_Term* p_term)
    {
      if (! p_term->pSymbol()->bIs_Col())
        return C_CONTINUE_RECURSION;

      //p_term->vDump(__func__);
      RASSERT(
          p_term->pSymbol()->bHasFacets(SF_EXP_COL,
                                        SF_NMD_COL,
                                        SF_NONE)
          ||
          p_term->pSymbol()->bHasFacets(SF_EXP_COL,
                                        SF_NMD_COL,
                                        SF_SQY_NO,
                                        SF_NONE)
          ||
          p_term->pSymbol()->bHasFacets(SF_NMD_COL,
                                        SF_TBL_COL,
                                        SF_NONE)
          ||
          p_term->pSymbol()->bHasFacets(SF_TBL_COL,
                                        SF_NONE));

      return C_CONTINUE_RECURSION;
    };

    (void) m_pCI->E.p_predicate->bLambdaWalk_term(bLambdaFn);

    vDebugPoint();
   }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // facets 2 hshColList

  if ((m_pCI->E.p_hshColList = m_pCI->E.p_Op->pSub_hshColList())) {
    NEVER_REACH // todo: why? [297]
    m_pCI->E.p_hshColList->vDump(__func__);
    for (Kid xHshColList(m_pCI->E.p_hshColList); xHshColList.bEach(CK_HSHCOL); /**/) {
      m_pCI->E.p_hshCol = xHshColList.p_hshCol();

    }

    vDebugPoint();
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // facets 2 filter

  if ((m_pCI->E.p_filter = m_pCI->E.p_Op->pSub_filter())) {
    m_pCI->E.p_predicate = m_pCI->E.p_filter->pSub_predicate(C_NOT_NULL);
    //m_pCI->E.p_predicate->vDump(__func__);

    //CardIter* pCI = m_pCI;
    auto bLambdaFn = [](Card_Term* p_term)
    {
       if (! p_term->pSymbol()->bIs_Col())
         return C_CONTINUE_RECURSION;

      //p_term->vDump(__func__);
      RASSERT(
               p_term->pSymbol()->bHasFacets(SF_EXP_COL,
                                             SF_NMD_COL,
                                             SF_NONE)
               ||
               p_term->pSymbol()->bHasFacets(SF_EXP_COL,
                                             SF_NMD_COL,
                                             SF_SQY_NO,
                                             SF_NONE)
               ||
               p_term->pSymbol()->bHasFacets(SF_NMD_COL,
                                             SF_TBL_COL,
                                             SF_NONE)
               ||
               p_term->pSymbol()->bHasFacets(SF_TBL_COL,
                                             SF_NONE));

      return C_CONTINUE_RECURSION;
    };

    (void) m_pCI->E.p_predicate->bLambdaWalk_term(bLambdaFn);

    vDebugPoint();
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // facets 2 emit

  if ((m_pCI->E.p_emit = m_pCI->E.p_Op->pSub_emit())) {
    for (Kid xEmit(m_pCI->E.p_emit); xEmit.bEach(CK_EXPR); /**/) {
      m_pCI->E.p_expr = xEmit.p_expr();
      //m_pCI->G.p_expr->vDump(__func__);

      RASSERT(
              m_pCI->E.p_expr->pSymbol()->bHasFacets(SF_EXP_COL,
                                                     SF_NMD_COL,
                                                     SF_NONE)
              ||
              m_pCI->E.p_expr->pSymbol()->bHasFacets(SF_EXP_COL,
                                                     SF_NMD_COL,
                                                     SF_TBL_COL,
                                                     SF_NONE)
                                                     );

      for (m_pCI->vBeginTarget(m_pCI->E.p_expr,CK_TERM); m_pCI->bEachTarget(); /**/) {
        m_pCI->E.p_term = m_pCI->E.xTarget.p_term();
        if (! m_pCI->E.p_term->pSymbol()->bIs_Col())
          continue;

        RASSERT(
                m_pCI->E.p_term->pSymbol()->bHasFacets(SF_EXP_COL,
                                                       SF_NONE)
                ||
                m_pCI->E.p_term->pSymbol()->bHasFacets(SF_KEY_COL,
                                                       SF_NONE)
                ||
                m_pCI->E.p_term->pSymbol()->bHasFacets(SF_TBL_COL,
                                                       SF_NONE)
        );

      }
    }

    vDebugPoint();
  }

  //===========================================================================
  // phase 3 : dictionaries
  //===========================================================================

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // dictionaries 3 injectable

  if ((m_pCI->E.p_injectable = m_pCI->E.p_Op->pSub_injectable())) {
    for (Kid xInject(m_pCI->E.p_injectable); xInject.bEach(CK_EXPR); /**/) {
      m_pCI->E.p_expr = xInject.p_expr();
      for (m_pCI->vBeginTarget(m_pCI->E.p_expr,CK_TERM); m_pCI->bEachTarget(); /**/) {
        m_pCI->E.p_term = m_pCI->E.xTarget.p_term();

        Symbol* pSym_term = m_pCI->E.p_term->pSymbol();
        if (m_pCI->E.p_injectable->pFind_expr(pSym_term))
          continue;

        m_pCI->E.p_injectable->vDump(__func__);
        m_pCI->E.p_term->vDump(__func__);
        NEVER_REACH
      }
    }
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //dictionaries 3 key

  if ((m_pCI->E.p_key = m_pCI->E.p_Op->pSub_key())) {
    for (Kid xKey(m_pCI->E.p_key); xKey.bEach(CK_EXPR); /**/) {
      m_pCI->E.p_expr = xKey.p_expr();
      //m_pCI->G.p_expr->vDump(__func__);
      for (m_pCI->vBeginTarget(m_pCI->E.p_expr,CK_TERM); m_pCI->bEachTarget(); /**/) {
        m_pCI->E.p_term = m_pCI->E.xTarget.p_term();

        if (m_pCI->Arriving.m_pEmit->pFind_expr(m_pCI->E.p_term->pSymbol()))
          continue;

        m_pCI->Arriving.m_pEmit->vDump(__func__);
        m_pCI->E.p_term->vDump(__func__);
        NEVER_REACH
      }
    }
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // dictionaries 3 filter

  if ((m_pCI->E.p_filter = m_pCI->E.p_Op->pSub_filter())) {

    m_pCI->E.p_predicate = m_pCI->E.p_filter->pSub_predicate(C_NOT_NULL);
    //m_pCI->G.p_predicate->vDump(__func__);

    CardIter* pCI = m_pCI;
    auto bLambdaFn = [pCI](Card_Term* p_term)
    {
      if (! p_term->pSymbol()->bIs_Col())
        return C_CONTINUE_RECURSION;

      if (pCI->Arriving.m_pEmit) {
        if (pCI->Arriving.m_pEmit->pFind_expr(p_term->pSymbol())) {
          return C_CONTINUE_RECURSION;
        }
      }

     if (pCI->E.p_injectable) {
       if (pCI->E.p_injectable->pFind_expr(p_term->pSymbol())) {
         return C_CONTINUE_RECURSION;
       }
     }

      pCI->E.p_predicate->vDump(__func__);
      p_term->vDump(__func__);
      NEVER_REACH
    };

    (void) m_pCI->E.p_predicate->bLambdaWalk_term(bLambdaFn);

    vDebugPoint();
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // dictionaries 3 constraint

  if ((m_pCI->E.p_constraint = m_pCI->E.p_Op->pSub_constraint())) {
    //m_pCI->G.p_constraint->vDump(__func__);
    RASSERT(m_pCI->E.p_injectable);
    m_pCI->E.p_predicate = m_pCI->E.p_constraint->pSub_predicate(C_NOT_NULL);

    CardIter* pCI = m_pCI;
    auto bLambdaFn = [pCI](Card_Term* p_term)
    {
      if (! p_term->pSymbol()->bIs_Col())
        return C_CONTINUE_RECURSION;

      if (pCI->E.p_injectable->pFind_expr(p_term->pSymbol()))
        return C_CONTINUE_RECURSION;

      pCI->E.p_predicate->vDump(__func__);
      pCI->E.p_term->vDump(__func__);
      NEVER_REACH
    };

    (void) m_pCI->E.p_predicate->bLambdaWalk_term(bLambdaFn);

    vDebugPoint();
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // dictionaries 3 indexList

  if ((m_pCI->E.p_indexList = m_pCI->E.p_Op->pSub_indexList())) {
    for (Kid xIndexList(m_pCI->E.p_indexList); xIndexList.bEach(CK_INDEX); /**/) {
      m_pCI->E.p_index = xIndexList.p_index();
      //m_pCI->G.p_index->vDump(__func__);

      // indexList/colList
      m_pCI->E.p_idxColList = m_pCI->E.p_index->pSub_idxColList(C_NOT_NULL);
      for (Kid xIdxColList(m_pCI->E.p_idxColList); xIdxColList.bEach(CK_IDXCOL); /**/) {
        m_pCI->E.p_idxCol = xIdxColList.p_idxCol();

        if (m_pCI->E.p_injectable->pFind_expr(m_pCI->E.p_idxCol->pSymbol()))
          continue;

        m_pCI->E.p_index->vDump(__func__);
        m_pCI->E.p_idxCol->vDump(__func__);
        NEVER_REACH
      }

      // indexList/bounds
      if (!(m_pCI->E.p_bounds = m_pCI->E.p_index->pSub_bounds()))
        continue;
      for (Kid xBounds(m_pCI->E.p_bounds); xBounds.bEach(CK_BOUND); /**/) {
        m_pCI->E.p_bound = xBounds.p_bound();
        //m_pCI->G.p_bound->vDump(__func__);
        vDebugPoint();
        for (Kid xBound(m_pCI->E.p_bound); xBound.bEach(CK_TERM); /**/) {
          m_pCI->E.p_term = xBound.p_term();
          if (! m_pCI->E.p_term->pSymbol()->bIs_Col())
            continue;
          if (m_pCI->Arriving.m_pEmit) {
            if (m_pCI->Arriving.m_pEmit->pFind_expr(m_pCI->E.p_term->pSymbol()))
              continue;
          }

          // todo: seems the bound is invalid [298]
//          m_pCI->PRINTBOARD_dbg();
//          m_pCI->G.p_bound->vDump(__func__);
//          m_pCI->G.p_term->vDump(__func__);
//          NEVER_REACH

        }
      }

    }

    vDebugPoint();
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // dictionaries 3 hshColList

  if ((m_pCI->E.p_hshColList = m_pCI->E.p_Op->pSub_hshColList())) {
    NEVER_REACH // todo: why? [297]
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 3 emit

  if ((m_pCI->E.p_emit = m_pCI->E.p_Op->pSub_emit())) {
    for (Kid xEmit(m_pCI->E.p_emit); xEmit.bEach(CK_EXPR); /**/) {
      m_pCI->E.p_expr = xEmit.p_expr();
      for (m_pCI->vBeginTarget(m_pCI->E.p_expr,CK_TERM); m_pCI->bEachTarget(); /**/) {
        m_pCI->E.p_term = m_pCI->E.xTarget.p_term();
        if (! m_pCI->E.p_term->pSymbol()->bIs_Col())
          continue;

        if (m_pCI->Arriving.m_pEmit) {
          if (m_pCI->Arriving.m_pEmit->pFind_expr(m_pCI->E.p_term->pSymbol()))
            continue;
        }

        if (m_pCI->E.p_injectable) {
          if (m_pCI->E.p_injectable->pFind_expr(m_pCI->E.p_term->pSymbol()))
            continue;
        }

        if (m_pCI->E.p_key) {
//Z_KEY_DIRECTION
          bool bMatched = false;
          for (m_pCI->vBeginTarget(m_pCI->E.p_key,CK_EXPR); m_pCI->bEachTarget(); /**/) {
            if (m_pCI->E.p_expr->pSymbol()->bMatches(m_pCI->E.p_term->pSymbol())) {
              bMatched = true;
              m_pCI->vEndTarget();
              break;
            }
          }
          if (bMatched)
            continue;
        }

        m_pCI->E.p_expr->vDump(__func__);
        m_pCI->E.p_term->vDump(__func__);
        NEVER_REACH
      }
    }
    vDebugPoint();
  }

  //===========================================================================
  // phase 4 : cast
  //===========================================================================

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cast 4 injectable

  if ((m_pCI->E.p_injectable = m_pCI->E.p_Op->pSub_injectable())) {
    for (Kid xInject(m_pCI->E.p_injectable); xInject.bEach(CK_EXPR); /**/) {
      m_pCI->E.p_expr = xInject.p_expr();
      for (m_pCI->vBeginTarget(m_pCI->E.p_expr,CK_TERM); m_pCI->bEachTarget(); /**/) {
        m_pCI->E.p_term = m_pCI->E.xTarget.p_term();

        if (m_pCI->E.p_term->pSymbol()->bValidCast())
          continue;

        m_pCI->E.p_injectable->vDump(__func__);
        m_pCI->E.p_term->vDump(__func__);
        NEVER_REACH
      }
    }
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cast 4 key

  if ((m_pCI->E.p_key = m_pCI->E.p_Op->pSub_key())) {
    for (Kid xKey(m_pCI->E.p_key); xKey.bEach(CK_EXPR); /**/) {
      m_pCI->E.p_expr = xKey.p_expr();
      //m_pCI->G.p_expr->vDump(__func__);
      for (m_pCI->vBeginTarget(m_pCI->E.p_expr,CK_TERM); m_pCI->bEachTarget(); /**/) {
        m_pCI->E.p_term = m_pCI->E.xTarget.p_term();

        if (m_pCI->E.p_term->pSymbol()->bValidCast())
          continue;

        m_pCI->Arriving.m_pEmit->vDump(__func__);
        m_pCI->E.p_term->vDump(__func__);
        NEVER_REACH
      }
    }
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cast 4 filter

  if ((m_pCI->E.p_filter = m_pCI->E.p_Op->pSub_filter())) {

    m_pCI->E.p_predicate = m_pCI->E.p_filter->pSub_predicate(C_NOT_NULL);
    //m_pCI->G.p_predicate->vDump(__func __);

    CardIter* pCI = m_pCI;
    auto bLambdaFn = [pCI](Card_Term* p_term)
    {
      if (p_term->pSymbol()->bIs_Col()) {
        if (p_term->pSymbol()->bValidCast())
          return C_CONTINUE_RECURSION;
        else
          vDebugPoint();
      }
      else
      if (p_term->pSymbol()->bIs_Lit()) {
        // todo: filter literals to be cast [300]
//        if (p_term->pSymbol()->bValidCast())
//          return C_CONTINUE_RECURSION;
//        else
//          vDebugPoint();
        return C_CONTINUE_RECURSION;
      }
      else {
        return C_CONTINUE_RECURSION;
      }

      pCI->E.p_predicate->vDump(__func__);
      p_term->vDump(__func__);
      NEVER_REACH
    };

    (void) m_pCI->E.p_predicate->bLambdaWalk_term(bLambdaFn);

    vDebugPoint();
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cast 4 constraint

  if ((m_pCI->E.p_constraint = m_pCI->E.p_Op->pSub_constraint())) {
    //m_pCI->G.p_constraint->vDump(__func__);
    RASSERT(m_pCI->E.p_injectable);
    m_pCI->E.p_predicate = m_pCI->E.p_constraint->pSub_predicate(C_NOT_NULL);

    CardIter* pCI = m_pCI;
    auto bLambdaFn = [pCI](Card_Term* p_term)
    {
      if (p_term->pSymbol()->bIs_Col()) {
        if (p_term->pSymbol()->bValidCast())
          return C_CONTINUE_RECURSION;
        else
          vDebugPoint();
      }
      else
      if (p_term->pSymbol()->bIs_Lit()) {
        // todo: constraint literals to be cast [301]
//        if (p_term->pSymbol()->bValidCast())
//          return C_CONTINUE_RECURSION;
//        else
//          vDebugPoint();
        return C_CONTINUE_RECURSION;
      }
      else {
        return C_CONTINUE_RECURSION;
      }

      pCI->E.p_predicate->vDump(__func__);
      p_term->vDump(__func__);
      NEVER_REACH
    };

    (void) m_pCI->E.p_predicate->bLambdaWalk_term(bLambdaFn);

    vDebugPoint();
  }


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cast 4 indexList

  if ((m_pCI->E.p_indexList = m_pCI->E.p_Op->pSub_indexList())) {
    for (Kid xIndexList(m_pCI->E.p_indexList); xIndexList.bEach(CK_INDEX); /**/) {
      m_pCI->E.p_index = xIndexList.p_index();
      //m_pCI->G.p_index->vDump(__func__);

      // indexList/colList
      // todo: should idxCols be cast? 302]
//      m_pCI->G.p_idxColList = m_pCI->G.p_index->pSub_idxColList(C_NOT_NULL);
//      for (Kid xIdxColList(m_pCI->G.p_idxColList); xIdxColList.bEach(CK_IDXCOL); /**/) {
//        m_pCI->G.p_idxCol = xIdxColList.p_idxCol();
//
//        if (m_pCI->G.p_idxCol->pSymbol()->bValidCast())
//          continue;
//
//        m_pCI->G.p_index->vDump(__func__);
//        m_pCI->G.p_idxCol->vDump(__func__);
//        NEVER_REACH
//      }

      // indexList/bounds
      if (!(m_pCI->E.p_bounds = m_pCI->E.p_index->pSub_bounds()))
        continue;
      for (Kid xBounds(m_pCI->E.p_bounds); xBounds.bEach(CK_BOUND); /**/) {
        m_pCI->E.p_bound = xBounds.p_bound();
        // m_pCI->G.p_bound->vDump(__func__);
        vDebugPoint();
        for (Kid xBound(m_pCI->E.p_bound); xBound.bEach(CK_TERM); /**/) {
          m_pCI->E.p_term = xBound.p_term();

          if (m_pCI->E.p_term->pSymbol()->bIs_Col()) {
            if (m_pCI->E.p_term->pSymbol()->bValidCast())
              continue;
            else
              vDebugPoint();
          }
          else
          if (m_pCI->E.p_term->pSymbol()->bIs_Lit()) {
            // todo: bound literals to be cast [303]
//            if (m_pCI->G.p_term->pSymbol()->bValidCast())
//              continue;
//            else
//             vDebugPoint();
            continue;
          }
          else {
            continue;
          }

          m_pCI->E.p_index->vDump(__func__);
          m_pCI->E.p_term->vDump(__func__);
          NEVER_REACH

        }
      }

    }

    vDebugPoint();
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cast 4 hshColList

  if ((m_pCI->E.p_hshColList = m_pCI->E.p_Op->pSub_hshColList())) {
    NEVER_REACH // todo: why? [297]
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cast 4 emit

  if ((m_pCI->E.p_emit = m_pCI->E.p_Op->pSub_emit())) {
    for (Kid xEmit(m_pCI->E.p_emit); xEmit.bEach(CK_EXPR); /**/) {
      m_pCI->E.p_expr = xEmit.p_expr();
      for (m_pCI->vBeginTarget(m_pCI->E.p_expr,CK_TERM); m_pCI->bEachTarget(); /**/) {
        m_pCI->E.p_term = m_pCI->E.xTarget.p_term();
        if (! m_pCI->E.p_term->pSymbol()->bIs_Col())
          continue;

        if (m_pCI->E.p_term->pSymbol()->bIs_Col()) {
          if (m_pCI->E.p_term->pSymbol()->bValidCast())
            continue;
          else
            vDebugPoint();
        }
        else
        if (m_pCI->E.p_term->pSymbol()->bIs_Lit()) {
          if (m_pCI->E.p_term->pSymbol()->bValidCast())
            continue;
          else
           vDebugPoint();
        }
        else {
          continue;
        }

        m_pCI->E.p_expr->vDump(__func__);
        m_pCI->E.p_term->vDump(__func__);
        NEVER_REACH
      }
    }
    vDebugPoint();
  }

  //===========================================================================
  // phase 5 : cross-cast
  //===========================================================================

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cross-cast 5 injectable

  if ((m_pCI->E.p_injectable = m_pCI->E.p_Op->pSub_injectable())) {
    for (Kid xInject(m_pCI->E.p_injectable); xInject.bEach(CK_EXPR); /**/) {
      m_pCI->E.p_expr = xInject.p_expr();
      for (m_pCI->vBeginTarget(m_pCI->E.p_expr,CK_TERM); m_pCI->bEachTarget(); /**/) {
        m_pCI->E.p_term = m_pCI->E.xTarget.p_term();

        if (Card_Expr* p_expr_f = m_pCI->E.p_injectable->pFind_expr(m_pCI->E.p_term->pSymbol())) {
          if (p_expr_f->pSymbol()->bSameCast(m_pCI->E.p_term->pSymbol()))
            continue;
        }

        m_pCI->E.p_injectable->vDump(__func__);
        m_pCI->E.p_term->vDump(__func__);
        NEVER_REACH
      }
    }
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cross-cast 5 key

  if ((m_pCI->E.p_key = m_pCI->E.p_Op->pSub_key())) {
    for (Kid xKey(m_pCI->E.p_key); xKey.bEach(CK_EXPR); /**/) {
      m_pCI->E.p_expr = xKey.p_expr();
      //m_pCI->G.p_expr->vDump(__func__);
      for (m_pCI->vBeginTarget(m_pCI->E.p_expr,CK_TERM); m_pCI->bEachTarget(); /**/) {
        m_pCI->E.p_term = m_pCI->E.xTarget.p_term();

        if (Card_Expr* p_expr_f = m_pCI->Arriving.m_pEmit->pFind_expr(m_pCI->E.p_term->pSymbol())) {
          if (p_expr_f->pSymbol()->bSameCast(m_pCI->E.p_term->pSymbol()))
            continue;
        }

        m_pCI->Arriving.m_pEmit->vDump(__func__);
        m_pCI->E.p_term->vDump(__func__);
        NEVER_REACH
      }
    }
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cross-cast 5 filter

  if ((m_pCI->E.p_filter = m_pCI->E.p_Op->pSub_filter())) {

    m_pCI->E.p_predicate = m_pCI->E.p_filter->pSub_predicate(C_NOT_NULL);
    //m_pCI->G.p_predicate->vDump(__func__);

    CardIter* pCI = m_pCI;
    auto bLambdaFn = [pCI](Card_Term* p_term)
    {
      if (! p_term->pSymbol()->bIs_Col())
        return C_CONTINUE_RECURSION;

      if (pCI->Arriving.m_pEmit) {
        if (Card_Expr* p_expr_f = pCI->Arriving.m_pEmit->pFind_expr(p_term->pSymbol())) {
          if (p_expr_f->pSymbol()->bSameCast(p_term->pSymbol()))
            return C_CONTINUE_RECURSION;
        }
      }

      if (pCI->E.p_injectable) {
        if (Card_Expr* p_expr_f = pCI->E.p_injectable->pFind_expr(p_term->pSymbol())) {
          if (p_expr_f->pSymbol()->bSameCast(p_term->pSymbol()))
            return C_CONTINUE_RECURSION;
        }
      }

      // todo: regLoop : having.test fails [306]
//      pCI->G.p_predicate->vDump(__func__);
//      p_term->vDump(__func__);
//      NEVER_REACH
      return C_CONTINUE_RECURSION;
    };

    (void) m_pCI->E.p_predicate->bLambdaWalk_term(bLambdaFn);

    vDebugPoint();
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cross-cast 5 constraint

  if ((m_pCI->E.p_constraint = m_pCI->E.p_Op->pSub_constraint())) {
    //m_pCI->G.p_constraint->vDump(__func__);
    RASSERT(m_pCI->E.p_injectable);
    m_pCI->E.p_predicate = m_pCI->E.p_constraint->pSub_predicate(C_NOT_NULL);

    CardIter* pCI = m_pCI;
    auto bLambdaFn = [pCI](Card_Term* p_term)
    {
      if (! p_term->pSymbol()->bIs_Col())
        return C_CONTINUE_RECURSION;

      if (Card_Expr* p_expr_f = pCI->E.p_injectable->pFind_expr(p_term->pSymbol()))
        if (p_expr_f->pSymbol()->bSameCast(p_term->pSymbol()))
          return C_CONTINUE_RECURSION;

      pCI->E.p_predicate->vDump(__func__);
      pCI->E.p_term->vDump(__func__);
      NEVER_REACH
    };

    (void) m_pCI->E.p_predicate->bLambdaWalk_term(bLambdaFn);

    vDebugPoint();
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cross-cast 5 indexList

  if ((m_pCI->E.p_indexList = m_pCI->E.p_Op->pSub_indexList())) {
    for (Kid xIndexList(m_pCI->E.p_indexList); xIndexList.bEach(CK_INDEX); /**/) {
      m_pCI->E.p_index = xIndexList.p_index();
      //m_pCI->G.p_index->vDump(__func__);

      // indexList/colList
      m_pCI->E.p_idxColList = m_pCI->E.p_index->pSub_idxColList(C_NOT_NULL);
      for (Kid xIdxColList(m_pCI->E.p_idxColList); xIdxColList.bEach(CK_IDXCOL); /**/) {
        m_pCI->E.p_idxCol = xIdxColList.p_idxCol();

        if (m_pCI->E.p_injectable->pFind_expr(m_pCI->E.p_idxCol->pSymbol()))
          continue;
//          todo: idxCol should be cast [304]
//      if (Card_Expr* p_expr_f = m_pCI->G.p_injectable->pFind_expr(m_pCI->G.p_idxCol->pSymbol()))
//          if (p_expr_f->pSymbol()->bSameCast(m_pCI->G.p_idxCol->pSymbol()))
//            continue;

        m_pCI->E.p_index->vDump(__func__);
        m_pCI->E.p_idxCol->vDump(__func__);
        NEVER_REACH
      }

      // indexList/bounds
      if (!(m_pCI->E.p_bounds = m_pCI->E.p_index->pSub_bounds()))
        continue;
      for (Kid xBounds(m_pCI->E.p_bounds); xBounds.bEach(CK_BOUND); /**/) {
        m_pCI->E.p_bound = xBounds.p_bound();
        //m_pCI->G.p_bound->vDump(__func__);
        vDebugPoint();
        for (Kid xBound(m_pCI->E.p_bound); xBound.bEach(CK_TERM); /**/) {
          m_pCI->E.p_term = xBound.p_term();
          if (! m_pCI->E.p_term->pSymbol()->bIs_Col())
            continue;

          if (m_pCI->Arriving.m_pEmit) {
            if (Card_Expr* p_expr_f = m_pCI->Arriving.m_pEmit->pFind_expr(m_pCI->E.p_term->pSymbol()))
              if (p_expr_f->pSymbol()->bSameCast(m_pCI->E.p_term->pSymbol()))
              continue;
          }

          // todo: seems the bound is invalid [298]
//          m_pCI->PRINTBOARD_dbg();
//          m_pCI->G.p_bound->vDump(__func__);
//          m_pCI->G.p_term->vDump(__func__);
//          NEVER_REACH

        }
      }

    }

    vDebugPoint();
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cross-cast 5 hshColList

  if ((m_pCI->E.p_hshColList = m_pCI->E.p_Op->pSub_hshColList())) {
    NEVER_REACH // todo: why? [297]
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cross-cast 5 emit

  if ((m_pCI->E.p_emit = m_pCI->E.p_Op->pSub_emit())) {
    for (Kid xEmit(m_pCI->E.p_emit); xEmit.bEach(CK_EXPR); /**/) {
      m_pCI->E.p_expr = xEmit.p_expr();
      for (m_pCI->vBeginTarget(m_pCI->E.p_expr,CK_TERM); m_pCI->bEachTarget(); /**/) {
        m_pCI->E.p_term = m_pCI->E.xTarget.p_term();
        if (! m_pCI->E.p_term->pSymbol()->bIs_Col())
          continue;

        if (m_pCI->Arriving.m_pEmit) {
          if (Card_Expr* p_expr_f = m_pCI->Arriving.m_pEmit->pFind_expr(m_pCI->E.p_term->pSymbol())) {
            if (p_expr_f->pSymbol()->bSameCast(m_pCI->E.p_term->pSymbol())) {
              continue;
            }
            else {
              //m_pCI->Arriving.m_pEmit->vDump(__func__);
              vDebugPoint();
            }
          }

        }

        if (m_pCI->E.p_injectable) {
          if (Card_Expr* p_expr_f = m_pCI->E.p_injectable->pFind_expr(m_pCI->E.p_term->pSymbol())) {
            if (p_expr_f->pSymbol()->bSameCast(m_pCI->E.p_term->pSymbol())) {
              continue;
            }
            else {
              //m_pCI->G.p_injectable->vDump(__func__);
              vDebugPoint();
            }
          }
        }

        if (m_pCI->E.p_key) {
          if (Card_Expr* p_expr_f = m_pCI->E.p_key->pFind_expr_key_Matches(m_pCI,m_pCI->E.p_term->pSymbol())) {
            if (p_expr_f->pSymbol()->bSameCast(m_pCI->E.p_term->pSymbol())) {
              continue;
            }

            // todo: query 18 $K2 type is wrong [
//          m_pCI->G.p_expr->vDump(__func__);
//          m_pCI->G.p_key->vDump(__func__);
//          m_pCI->G.p_term->vDump(__func__);
            vDebugPoint();
            continue;
          }
        }

        m_pCI->E.p_expr->vDump(__func__);
        m_pCI->E.p_term->vDump(__func__);
        NEVER_REACH
      }
    }
    vDebugPoint();
  }

  //===========================================================================
  // complete
  //===========================================================================

  vDebugPoint();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
