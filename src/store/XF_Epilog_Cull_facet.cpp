/*============================================================================*/
/* XF_Epilog_Cull_facet.cpp -- Cull Facet                                     */
/*============================================================================*/

#include "XF_Epilog_Cull_facet.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Epilog_Cull_facet, CN_XF_EPILOG_CULL_FACET)
__aNewFunction(XF_Epilog_Cull_facet, CN_XF_EPILOG_CULL_FACET)
__vDeleteFunction(XF_Epilog_Cull_facet, CN_XF_EPILOG_CULL_FACET)
__aFooterFunction(XF_Epilog_Cull_facet)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Epilog_Cull_facet::XF_Epilog_Cull_facet(CardIter* pCI)
: XF_EPILOG(CN_XF_EPILOG_CULL_FACET, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_CULL_FACET, _FOOTER_);

  _checkInitialised_(CN_XF_EPILOG_CULL_FACET);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_CULL_FACET, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Epilog_Cull_facet::~XF_Epilog_Cull_facet(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_CULL_FACET, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Epilog_Cull_facet::vCull(CardIter* pCI)
{
  _static_

  XF_Epilog_Cull_facet* pCull_facet = new XF_Epilog_Cull_facet(pCI);

  pCull_facet->vCull_facet();

  delete pCull_facet;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* cull loop                                                                  */
/*----------------------------------------------------------------------------*/
void XF_Epilog_Cull_facet::vCull_facet(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_CULL_FACET, _FOOTER_);

  m_pCI->vTransform(__FILE__, __func__, XF_EPILOG_CULL_FACET);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  CardIter* pCI = m_pCI;

  auto bLambdaFn = [pCI](Card* pCard)
  {
    UNUSED(pCI);

    Card_Term* p_term = _pCard_Term(pCard);
    Symbol* pSym_term = p_term->pSymbol();

    if (pCI->bAncestor(CK_s_INJECTABLE))
      return true;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    if (pCI->bAncestor(CK_o_KEYEXISTS) || pCI->bAncestor(CK_o_KEYNOTEXISTS)) {
      if (pCI->bAncestor(CK_s_CONSTRAINT)) {
        if (pSym_term->bIs_Col()) {
          if (pSym_term->bHasFacet(SF_EXP_COL))
            pSym_term->vCullFacet_except(SF_EXP_COL);
          else
          if (pSym_term->bHasFacet(SF_NMD_COL))
            pSym_term->vCullFacet_except(SF_NMD_COL);
        }
      }
      else {
        if (pSym_term->bIs_Col()) {
          if (pSym_term->bHasFacet(SF_IDX_COL))
            pSym_term->vCullFacet_except(SF_IDX_COL);
          else
          if (pSym_term->bHasFacet(SF_EXP_COL))
            pSym_term->vCullFacet_except(SF_EXP_COL);
        }
      }
      if (pSym_term->bHasFacet(SF_NMD_COL))
        pSym_term->vCullFacet_except(SF_NMD_COL);


      RASSERT(1 == pSym_term->nFacets());
      return true;
    };

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef Z_NO_EXPR_TBLCOL
    if (pSym_term->bHasFacet(SF_ITM_COL))
      pSym_term->vCullFacet_leaving(SF_ITM_COL,SF_TBL_COL);
    else
    if (pSym_term->bHasFacet(SF_KEY_COL))
      pSym_term->vCullFacet_leaving(SF_KEY_COL,SF_TBL_COL);
    else
    if (pSym_term->bHasFacet(SF_EXP_COL))
      pSym_term->vCullFacet_leaving(SF_EXP_COL,SF_TBL_COL);
#else
    if (pSym_term->bHasFacet(SF_ITM_COL))
      pSym_term->vCullFacet_except(SF_ITM_COL);
    else
    if (pSym_term->bHasFacet(SF_KEY_COL))
      pSym_term->vCullFacet_except(SF_KEY_COL);
    else
    if (pSym_term->bHasFacet(SF_EXP_COL))
      pSym_term->vCullFacet_except(SF_EXP_COL);

    RASSERT(1 == pSym_term->nFacets());
#endif

    return true;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pCI->bLambdaWalk(pCI->E.p_deck, CK_TERM, bLambdaFn);

  m_pCI->vEndTransform(__LINE__);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
