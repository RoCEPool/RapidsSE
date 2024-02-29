/*============================================================================*/
/* XF_Epilog_Itemise_Columns.cpp -- De-Duplicate As (column name)                          */
/*============================================================================*/

#include "XF_Epilog_Itemise_Columns.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Epilog_Itemise_Columns, CN_XF_EPILOG_ITEMISE_COLUMNS)
__aNewFunction(XF_Epilog_Itemise_Columns, CN_XF_EPILOG_ITEMISE_COLUMNS)
__vDeleteFunction(XF_Epilog_Itemise_Columns, CN_XF_EPILOG_ITEMISE_COLUMNS)
__aFooterFunction(XF_Epilog_Itemise_Columns)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Epilog_Itemise_Columns::XF_Epilog_Itemise_Columns(CardIter* pCI)
: XF_EPILOG(CN_XF_EPILOG_ITEMISE_COLUMNS, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_ITEMISE_COLUMNS, _FOOTER_);

  _checkInitialised_(CN_XF_EPILOG_ITEMISE_COLUMNS);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_ITEMISE_COLUMNS, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Epilog_Itemise_Columns::~XF_Epilog_Itemise_Columns(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_ITEMISE_COLUMNS, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* de-duplicate as (column names)                                             */
/*----------------------------------------------------------------------------*/
void XF_Epilog_Itemise_Columns::vItemise(CardIter* pCI)
{
  _static_

  XF_Epilog_Itemise_Columns* pXF_Dedupe_colNm = new XF_Epilog_Itemise_Columns(pCI);

  pXF_Dedupe_colNm->vItemise_Columns();

  delete pXF_Dedupe_colNm;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* de-duplicate as (column names)                                             */
/*----------------------------------------------------------------------------*/
void XF_Epilog_Itemise_Columns::vItemise_Columns(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_ITEMISE_COLUMNS, _FOOTER_);

  m_pCI->vTransform(__FILE_NAME__,__func__, XF_EPILOG_ITEMISE_COLUMNS, "de-duplicate as (column names)"_Stz);

  Card_As::vItemise_colNm(m_pCI);
  Card_Term::vItemise_colNm(m_pCI);

  m_pCI->vEndTransform(__LINE__);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
