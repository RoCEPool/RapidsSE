/*============================================================================*/
/* XF_Epilog_Column_hashing.cpp -- Column Mapping                                    */
/*============================================================================*/

#include "XF_Epilog_Column_hashing.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Epilog_Column_hashing, CN_XF_COLUMN_HASHING)
__aNewFunction(XF_Epilog_Column_hashing, CN_XF_COLUMN_HASHING)
__vDeleteFunction(XF_Epilog_Column_hashing, CN_XF_COLUMN_HASHING)
__aFooterFunction(XF_Epilog_Column_hashing)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Epilog_Column_hashing::XF_Epilog_Column_hashing(CardIter* pCI)
: XF_EPILOG(CN_XF_COLUMN_HASHING, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_COLUMN_HASHING, _FOOTER_);

  _checkInitialised_(CN_XF_COLUMN_HASHING);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_COLUMN_HASHING, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Epilog_Column_hashing::~XF_Epilog_Column_hashing(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_COLUMN_HASHING, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Epilog_Column_hashing::vEpilog(CardIter* pCI)
{
  _static_

  XF_Epilog_Column_hashing* pXF_Column_Hashing = new XF_Epilog_Column_hashing(pCI);

  pXF_Column_Hashing->vXF_Column_Hashing();

  delete pXF_Column_Hashing;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* column mapping                                                             */
/*----------------------------------------------------------------------------*/
void XF_Epilog_Column_hashing::vXF_Column_Hashing(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_COLUMN_HASHING, _FOOTER_);

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
