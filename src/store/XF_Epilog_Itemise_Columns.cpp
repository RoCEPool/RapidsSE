/*============================================================================*/
/* XF_Epilog_Itemise_Columns.cpp -- De-Duplicate As (column name)                          */
/*============================================================================*/

#include "XF_Epilog_Itemise_Columns.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Epilog_Itemise_Columns, CN_XF_EPILOG_ITEMISE_COLUMNS)
__aNewFunction(XF_Epilog_Itemise_Columns, CN_XF_EPILOG_ITEMISE_COLUMNS)
__vDeleteFunction(XF_Epilog_Itemise_Columns, CN_XF_EPILOG_ITEMISE_COLUMNS)
__aFooterFunction(XF_Epilog_Itemise_Columns)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Epilog_Itemise_Columns::XF_Epilog_Itemise_Columns(CardIter* pCI)
: XF_EPILOG(CN_XF_EPILOG_ITEMISE_COLUMNS, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_ITEMISE_COLUMNS, _FOOTER_);

  _checkInitialised_(CN_XF_EPILOG_ITEMISE_COLUMNS);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_ITEMISE_COLUMNS, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Epilog_Itemise_Columns::~XF_Epilog_Itemise_Columns(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_ITEMISE_COLUMNS, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* de-duplicate as (column names)                                             */
/*----------------------------------------------------------------------------*/
void XF_Epilog_Itemise_Columns::vItemise(CardIter* pCI)
{
  _static_

  XF_Epilog_Itemise_Columns* pXF_Dedupe_colNm = new XF_Epilog_Itemise_Columns(pCI);

  pXF_Dedupe_colNm->vItemise_Columns();

  delete pXF_Dedupe_colNm;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* de-duplicate as (column names)                                             */
/*----------------------------------------------------------------------------*/
void XF_Epilog_Itemise_Columns::vItemise_Columns(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_ITEMISE_COLUMNS, _FOOTER_);

  m_pCI->vTransform(__FILE__, __func__, XF_EPILOG_ITEMISE_COLUMNS);

  CardIter* pCI = m_pCI;
  auto bfItemise_colNm = [pCI](Card* pCard)
  {
    //pCI->vAncestorDump(pCard);

    if (! pCard->bIsCard(CK_EXPR))
      return C_CONTINUE_RECURSION;

    Card_Expr* p_expr = _pCard_Expr(pCard);
    if (! p_expr->pSymbol()->bHasFacet(SF_NMD_COL))
      return C_CONTINUE_RECURSION;

    i8 i_Ctr = pCI->iAncestorCounterInc(pCI->pAncestorParent());
    Symbol oSymbol_itemise;
    oSymbol_itemise.vApplyFacet(SF_ITM_COL, i_Ctr, p_expr->pSymbol());

    p_expr->pSymbol()->vApplyFacet(oSymbol_itemise.aFacet(SF_ITM_COL));

    return C_CONTINUE_RECURSION;
  };

  //pCI->bLambdaWalk(pCI->E.p_deck, CK_NONE, bfItemise_colNm);
  pCI->E.p_deck->bLambdaWalk(CK_NONE, bfItemise_colNm,pCI);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bfItemise_colNm2 = [pCI](Card* pCard)
  {
    UNUSED(pCI);

//    if (AM::JOB::bHasTag(C_OPTIMISER_TRACE))
//      vDebugPoint();

    if (!pCard->bIsCard(CK_TERM))
      return C_CONTINUE_RECURSION;

    if (pCI->bAncestor(CK_o_KEYEXISTS) || pCI->bAncestor(CK_o_KEYNOTEXISTS))
      return C_CONTINUE_RECURSION;

    Card_Term *p_term = _pCard_Term(pCard);
    Symbol *pSymbol_term = p_term->pSymbol();

    if ((pCI->E.p_indexJoin = _pCard_IndexJoin(pCI->pAncestor(CK_o_INDEXJOIN))) &&
        (pCI->E.p_constraint = _pCard_Constraint(pCI->pAncestor(CK_p_CONSTRAINT)))) {

      if (!pSymbol_term->bHasFacet(SF_NMD_COL))
        return C_CONTINUE_RECURSION;

      pCI->E.p_emit = pCI->E.p_indexJoin->pSub_emit();
      pCI->E.p_expr = pCI->E.p_emit->pFind_expr(pSymbol_term);
      pSymbol_term->vApplyFacet(SF_ITM_COL, pCI->E.p_expr->szFacet(SF_ITM_COL));
      return C_CONTINUE_RECURSION;
    }

    if (pSymbol_term->bHasFacet(SF_EXP_COL))
      return C_CONTINUE_RECURSION;
    if (! pSymbol_term->bHasFacet(SF_NMD_COL))
      return C_CONTINUE_RECURSION;

    if (! (pCI->E.p_expr = _pCard_Expr(pCI->pAncestor(CK_EXPR))))
      return C_CONTINUE_RECURSION;

    i8 i_Ctr = pCI->iAncestorCounterInc(pCI->pAncestorParent());
    pSymbol_term->vApplyFacet(SF_ITM_COL, i_Ctr);
    return C_CONTINUE_RECURSION;
  };

  //pCI->bLambdaWalk(pCI->E.p_deck, CK_NONE, bfItemise_colNm2);
  pCI->E.p_deck->bLambdaWalk(CK_NONE, bfItemise_colNm2,pCI);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_pCI->vEndTransform(__LINE__);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
