/*============================================================================*/
/* XF_Epilog_Map_Columns.cpp -- Column Mapping                                    */
/*============================================================================*/

#include "XF_Epilog_Map_Columns.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Epilog_Map_Columns, CN_XF_EPILOG_MAP_COLUMNS)
__aNewFunction(XF_Epilog_Map_Columns, CN_XF_EPILOG_MAP_COLUMNS)
__vDeleteFunction(XF_Epilog_Map_Columns, CN_XF_EPILOG_MAP_COLUMNS)
__aFooterFunction(XF_Epilog_Map_Columns)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Epilog_Map_Columns::XF_Epilog_Map_Columns(CardIter* pCI)
: XF_EPILOG(CN_XF_EPILOG_MAP_COLUMNS, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_MAP_COLUMNS, _FOOTER_);

  _checkInitialised_(CN_XF_EPILOG_MAP_COLUMNS);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_MAP_COLUMNS, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Epilog_Map_Columns::~XF_Epilog_Map_Columns(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_MAP_COLUMNS, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Epilog_Map_Columns::vMap(CardIter* pCI)
{
  _static_

  XF_Epilog_Map_Columns* pXF_Column_Mapping = new XF_Epilog_Map_Columns(pCI);

  pXF_Column_Mapping->vMap_Columns();

  delete pXF_Column_Mapping;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* column mapping                                                             */
/*----------------------------------------------------------------------------*/
void XF_Epilog_Map_Columns::vMap_Columns(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_MAP_COLUMNS, _FOOTER_);

  m_pCI->vTransform(__func__, XF_EPILOG_MAP_COLUMNS, "column mapping"_Stz);

  m_pCI->vClearRestart();
  while (m_pCI->G_bEachMainWithSub(CK_m_GBYAGG, CK_SYNTHESIS)) {
    m_pCI->G.p_OP->vMapColumns(m_pCI);
  }

  m_pCI->vClearRestart();
  while (m_pCI->G_bEachMainWithSub(CK_m_SORT, CK_SYNTHESIS)) {
    m_pCI->G.p_OP->vMapColumns(m_pCI);
  }

  m_pCI->vClearRestart();
  while (m_pCI->G_bEachMainWithSub(CK_m_TOP, CK_SYNTHESIS)) {
    m_pCI->G.p_OP->vMapColumns(m_pCI);
  }

  m_pCI->vEndTransform(__LINE__);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
