/*============================================================================*/
/* XF_Epilog_Add_cast.cpp -- Stage Cast                                            */
/*============================================================================*/

#include "XF_Epilog_Add_cast.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Epilog_Add_cast, CN_XF_ADD_CAST)
__aNewFunction(XF_Epilog_Add_cast, CN_XF_ADD_CAST)
__vDeleteFunction(XF_Epilog_Add_cast, CN_XF_ADD_CAST)
__aFooterFunction(XF_Epilog_Add_cast)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Epilog_Add_cast::XF_Epilog_Add_cast(CardIter* pCI)
: XF_EPILOG(CN_XF_ADD_CAST, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_ADD_CAST, _FOOTER_);

  _checkInitialised_(CN_XF_ADD_CAST);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_ADD_CAST, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Epilog_Add_cast::~XF_Epilog_Add_cast(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_ADD_CAST, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Epilog_Add_cast::vEpilog(CardIter* pCI)
{
  _static_

  XF_Epilog_Add_cast* pXF_stage_cast = new XF_Epilog_Add_cast(pCI);

  pXF_stage_cast->vXF_stage_cast();

  delete pXF_stage_cast;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Epilog_Add_cast::vXF_stage_cast(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_ADD_CAST, _FOOTER_);

  m_pCI->vTransform(__func__, XF_EPILOG_ADD_CAST, "add stage casts"_Stz);

  m_pCI->G.pDeck->vAddStageCasts(m_pCI);

  m_pCI->vEndTransform();
//vValidateBench();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
