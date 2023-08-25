/*============================================================================*/
/* XF_Epilog_Reinstate_stage.cpp -- Reinstate Stage                                  */
/*============================================================================*/

#include "XF_Epilog_Reinstate_stage.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Epilog_Reinstate_stage, CN_XF_REINSTATE_STAGE)
__aNewFunction(XF_Epilog_Reinstate_stage, CN_XF_REINSTATE_STAGE)
__vDeleteFunction(XF_Epilog_Reinstate_stage, CN_XF_REINSTATE_STAGE)
__aFooterFunction(XF_Epilog_Reinstate_stage)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Epilog_Reinstate_stage::XF_Epilog_Reinstate_stage(CardIter* pCI)
: XF_EPILOG(CN_XF_REINSTATE_STAGE, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_REINSTATE_STAGE, _FOOTER_);

  _checkInitialised_(CN_XF_REINSTATE_STAGE);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_REINSTATE_STAGE, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Epilog_Reinstate_stage::~XF_Epilog_Reinstate_stage(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_REINSTATE_STAGE, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Epilog_Reinstate_stage::vXF_Reinstate_stage(CardIter* pCI)
{
  _static_

  XF_Epilog_Reinstate_stage* pXF_Reinstate_stage = new XF_Epilog_Reinstate_stage(pCI);

  pXF_Reinstate_stage->vXF_Reinstate_stage();

  delete pXF_Reinstate_stage;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* reinstate stages                                                           */
/*----------------------------------------------------------------------------*/
void XF_Epilog_Reinstate_stage::vXF_Reinstate_stage(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_REINSTATE_STAGE, _FOOTER_);

  m_pCI->vTransform(__func__, XF_EPILOG_REINSTATE_STAGE, "reinstate stages"_Stz);

  Card_Stage::vReinstate_stages(m_pCI);

  m_pCI->vEndTransform();
  vValidateBench();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
