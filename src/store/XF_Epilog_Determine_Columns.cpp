/*============================================================================*/
/* XF_Epilog_Determine_Columns.cpp -- Determine Columns (required)            */
/*============================================================================*/

#include "XF_Epilog_Determine_Columns.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Epilog_Determine_Columns, CN_XF_MAP_COLUMNS)
__aNewFunction(XF_Epilog_Determine_Columns, CN_XF_MAP_COLUMNS)
__vDeleteFunction(XF_Epilog_Determine_Columns, CN_XF_MAP_COLUMNS)
__aFooterFunction(XF_Epilog_Determine_Columns)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Epilog_Determine_Columns::XF_Epilog_Determine_Columns(CardIter* pCI)
: XF_EPILOG(CN_XF_MAP_COLUMNS, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_MAP_COLUMNS, _FOOTER_);

  _checkInitialised_(CN_XF_MAP_COLUMNS);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_MAP_COLUMNS, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Epilog_Determine_Columns::~XF_Epilog_Determine_Columns(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_MAP_COLUMNS, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Epilog_Determine_Columns::vEpilog(CardIter* pCI)
{
  _static_

  XF_Epilog_Determine_Columns* pXF_Columns_required = new XF_Epilog_Determine_Columns(pCI);

  pXF_Columns_required->vXF_Columns_required();

  delete pXF_Columns_required;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* columns required                                                           */
/*----------------------------------------------------------------------------*/
void XF_Epilog_Determine_Columns::vXF_Columns_required(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_MAP_COLUMNS, _FOOTER_);

  m_pCI->vTransform(__func__, XF_EPILOG_COLUMNS_REQUIRED, "columns required"_Stz);

  m_pCI->vClearRestart();
  while (m_pCI->G_bEachMain_NONE()) {
  }

  m_pCI->vEndTransform();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
