/*============================================================================*/
/* XF_Epilog_Add_synthesis.cpp -- Column Mapping                                    */
/*============================================================================*/

#include "XF_Epilog_Add_synthesis.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Epilog_Add_synthesis, CN_XF_EPILOG_ADD_SYNTHESIS)
__aNewFunction(XF_Epilog_Add_synthesis, CN_XF_EPILOG_ADD_SYNTHESIS)
__vDeleteFunction(XF_Epilog_Add_synthesis, CN_XF_EPILOG_ADD_SYNTHESIS)
__aFooterFunction(XF_Epilog_Add_synthesis)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Epilog_Add_synthesis::XF_Epilog_Add_synthesis(CardIter* pCI)
: XF_EPILOG(CN_XF_EPILOG_ADD_SYNTHESIS, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_ADD_SYNTHESIS, _FOOTER_);

  _checkInitialised_(CN_XF_EPILOG_ADD_SYNTHESIS);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_ADD_SYNTHESIS, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Epilog_Add_synthesis::~XF_Epilog_Add_synthesis(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_ADD_SYNTHESIS, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Epilog_Add_synthesis::vEpilog(CardIter* pCI)
{
  _static_

  XF_Epilog_Add_synthesis* pEpilog_Collapse_op = new XF_Epilog_Add_synthesis(pCI);

  pEpilog_Collapse_op->vAdd_synthesis();

  delete pEpilog_Collapse_op;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add synthesis                                                              */
/*----------------------------------------------------------------------------*/
void XF_Epilog_Add_synthesis::vAdd_synthesis(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_ADD_SYNTHESIS, _FOOTER_);

  m_pCI->vTransform(__func__, XF_EPILOG_ADD_SYNTHESIS, "add synthesis"_Stz);

  m_pCI->vClearRestart();
  while (m_pCI->G_bEachMain_NONE()) {
    if (m_pCI->G.p_MainCard->bIsOne(CK_STAGE,CK_NONE))
      continue;

    if (m_pCI->G.p_MainCard->pSymbol()->Y_bHas(P_Collapsed))
      continue;

    m_pCI->G_vAppendNewChild(m_pCI->G.p_MainCard, CK_SYNTHESIS);
  }

  m_pCI->vEndTransform();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
