/*============================================================================*/
/* XF_Epilog_Append_synthesis.cpp -- Column Mapping                                    */
/*============================================================================*/

#include "XF_Epilog_Append_synthesis.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Epilog_Append_synthesis, CN_XF_EPILOG_APPEND_SYNTHESIS)
__aNewFunction(XF_Epilog_Append_synthesis, CN_XF_EPILOG_APPEND_SYNTHESIS)
__vDeleteFunction(XF_Epilog_Append_synthesis, CN_XF_EPILOG_APPEND_SYNTHESIS)
__aFooterFunction(XF_Epilog_Append_synthesis)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Epilog_Append_synthesis::XF_Epilog_Append_synthesis(CardIter* pCI)
: XF_EPILOG(CN_XF_EPILOG_APPEND_SYNTHESIS, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_APPEND_SYNTHESIS, _FOOTER_);

  _checkInitialised_(CN_XF_EPILOG_APPEND_SYNTHESIS);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_APPEND_SYNTHESIS, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Epilog_Append_synthesis::~XF_Epilog_Append_synthesis(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_APPEND_SYNTHESIS, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Epilog_Append_synthesis::vAppend(CardIter* pCI)
{
  _static_

  XF_Epilog_Append_synthesis* pEpilog_Collapse_op = new XF_Epilog_Append_synthesis(pCI);

  pEpilog_Collapse_op->vAppend_synthesis();

  delete pEpilog_Collapse_op;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add synthesis                                                              */
/*----------------------------------------------------------------------------*/
void XF_Epilog_Append_synthesis::vAppend_synthesis(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_APPEND_SYNTHESIS, _FOOTER_);

  m_pCI->vTransform(__FILE__, __func__, XF_EPILOG_APPEND_SYNTHESIS);

  m_pCI->vClearRestart();
  while (m_pCI->G_bEachMain_NONE()) {
    if (m_pCI->G.p_MainCard->bIsCard(CK_c_STAGE))
      continue;

    m_pCI->G_vAppendNewChild(m_pCI->G.p_MainCard, CK_SYNTHESIS);
  }

  m_pCI->vEndTransform(__LINE__);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
