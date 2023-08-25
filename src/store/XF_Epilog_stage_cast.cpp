/*============================================================================*/
/* XF_Epilog_stage_cast.cpp -- Stage Cast                                            */
/*============================================================================*/

#include "XF_Epilog_stage_cast.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Epilog_stage_cast, CN_XF_STAGE_CAST)
__aNewFunction(XF_Epilog_stage_cast, CN_XF_STAGE_CAST)
__vDeleteFunction(XF_Epilog_stage_cast, CN_XF_STAGE_CAST)
__aFooterFunction(XF_Epilog_stage_cast)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Epilog_stage_cast::XF_Epilog_stage_cast(CardIter* pCI)
: XF_EPILOG(CN_XF_STAGE_CAST, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_STAGE_CAST, _FOOTER_);

  _checkInitialised_(CN_XF_STAGE_CAST);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_STAGE_CAST, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Epilog_stage_cast::~XF_Epilog_stage_cast(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_STAGE_CAST, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Epilog_stage_cast::vXF_stage_cast(CardIter* pCI)
{
  _static_

  XF_Epilog_stage_cast* pXF_stage_cast = new XF_Epilog_stage_cast(pCI);

  pXF_stage_cast->vXF_stage_cast();

  delete pXF_stage_cast;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Epilog_stage_cast::vXF_stage_cast(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_STAGE_CAST, _FOOTER_);

  m_pCI->vTransform(__func__, XF_EPILOG_STAGE_CAST, "add stage casts"_Stz);

  m_pCI->G.pDeck->vAddStageCasts(m_pCI);

  m_pCI->vEndTransform();

  vValidateBench();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
