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

  pCull_facet->vCullLoop();

  delete pCull_facet;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* cull loop                                                                  */
/*----------------------------------------------------------------------------*/
void XF_Epilog_Cull_facet::vCullLoop(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_CULL_FACET, _FOOTER_);

  m_pCI->vTransform(__func__, XF_EPILOG_CULL_FACET, "cull facet"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  CardIter* pCI = m_pCI;

  auto bLambdaFn = [pCI](Card* pCard)
  {
    UNUSED(pCI);
    UNUSED(pCard);
    //Card_Term* p_term = _pCard_Term(pCard);
    //p_term->pSymbol()->vCullFacet(SF_TBL_COL);

    return true;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pCI->bApplyRecursive(pCI->G.pDeck,CK_TERM,bLambdaFn);

  m_pCI->vEndTransform(__LINE__);
}

/*----------------------------------------------------------------------------*/
/* cull facet                                                                 */
/*----------------------------------------------------------------------------*/
void XF_Epilog_Cull_facet::vCull_facet(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_CULL_FACET, _FOOTER_);

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
