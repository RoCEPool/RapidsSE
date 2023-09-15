/*============================================================================*/
/* XF_Epilog_Columns_required.cpp -- Columns required                         */
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

  XF_Epilog_Columns_required* pXF_Columns_required = new XF_Epilog_Columns_required(pCI);

  pXF_Columns_required->vXF_Columns_required();

  delete pXF_Columns_required;
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

  m_pCI->vTransform(__func__, XF_EPILOG_COLUMNS_REQUIRED, "columns required"_Stz);

  m_pCI->vClearRestart();
  while (m_pCI->G_bEachMain_NONE()) {
  }

  m_pCI->vEndTransform();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
