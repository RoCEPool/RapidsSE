/*============================================================================*/
/* XF_Delete_Unneeded.cpp -- Delete Unneeded                                  */
/*============================================================================*/

#include "XF_Delete_Unneeded.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Delete_Unneeded, CN_XF_DELETE_UNNEEDED)
__aNewFunction(XF_Delete_Unneeded, CN_XF_DELETE_UNNEEDED)
__vDeleteFunction(XF_Delete_Unneeded, CN_XF_DELETE_UNNEEDED)
__aFooterFunction(XF_Delete_Unneeded)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Delete_Unneeded::XF_Delete_Unneeded(CardIter* pCI)
: XF_TRANSFORM(CN_XF_DELETE_UNNEEDED, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_DELETE_UNNEEDED, _FOOTER_);

  _checkInitialised_(CN_XF_DELETE_UNNEEDED);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_DELETE_UNNEEDED, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Delete_Unneeded::~XF_Delete_Unneeded(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_DELETE_UNNEEDED, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Delete_Unneeded::vXF_Delete_Unneeded(CardIter* pCI)
{
  _static_

  XF_Delete_Unneeded* pXF_Delete_Unneeded = new XF_Delete_Unneeded(pCI);

  pXF_Delete_Unneeded->vXF_Delete_Unneeded();

  delete pXF_Delete_Unneeded;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Delete_Unneeded::vXF_Delete_Unneeded(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_DELETE_UNNEEDED, _FOOTER_);


  vValidate();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
