/*============================================================================*/
/* XF_Epilog_Columns_required.cpp -- Column Mapping                                    */
/*============================================================================*/

#include "XF_Epilog_Columns_required.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Epilog_Columns_required, CN_XF_COLUMN_MAPPING)
__aNewFunction(XF_Epilog_Columns_required, CN_XF_COLUMN_MAPPING)
__vDeleteFunction(XF_Epilog_Columns_required, CN_XF_COLUMN_MAPPING)
__aFooterFunction(XF_Epilog_Columns_required)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Epilog_Columns_required::XF_Epilog_Columns_required(CardIter* pCI)
: XF_EPILOG(CN_XF_COLUMN_MAPPING, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_COLUMN_MAPPING, _FOOTER_);

  _checkInitialised_(CN_XF_COLUMN_MAPPING);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_COLUMN_MAPPING, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Epilog_Columns_required::~XF_Epilog_Columns_required(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_COLUMN_MAPPING, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Epilog_Columns_required::vEpilog(CardIter* pCI)
{
  _static_

  XF_Epilog_Columns_required* pXF_Column_Mapping = new XF_Epilog_Columns_required(pCI);

  pXF_Column_Mapping->vXF_Columns_required();

  delete pXF_Column_Mapping;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* columns required                                                           */
/*----------------------------------------------------------------------------*/
void XF_Epilog_Columns_required::vXF_Columns_required(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_COLUMN_MAPPING, _FOOTER_);

  m_pCI->vTransform(__func__, XF_EPILOG_COLUMN_MAPPING, "column mapping"_Stz);

  m_pCI->vEndTransform();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
