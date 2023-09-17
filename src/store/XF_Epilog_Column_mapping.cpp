/*============================================================================*/
/* XF_Epilog_Column_mapping.cpp -- Column Mapping                                    */
/*============================================================================*/

#include "XF_Epilog_Column_mapping.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Epilog_Column_mapping, CN_XF_COLUMN_MAPPING)
__aNewFunction(XF_Epilog_Column_mapping, CN_XF_COLUMN_MAPPING)
__vDeleteFunction(XF_Epilog_Column_mapping, CN_XF_COLUMN_MAPPING)
__aFooterFunction(XF_Epilog_Column_mapping)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Epilog_Column_mapping::XF_Epilog_Column_mapping(CardIter* pCI)
: XF_EPILOG(CN_XF_COLUMN_MAPPING, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_COLUMN_MAPPING, _FOOTER_);

  _checkInitialised_(CN_XF_COLUMN_MAPPING);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_COLUMN_MAPPING, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Epilog_Column_mapping::~XF_Epilog_Column_mapping(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_COLUMN_MAPPING, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Epilog_Column_mapping::vEpilog(CardIter* pCI)
{
  _static_

  XF_Epilog_Column_mapping* pXF_Column_Mapping = new XF_Epilog_Column_mapping(pCI);

  pXF_Column_Mapping->vXF_Column_Mapping();

  delete pXF_Column_Mapping;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* column mapping                                                             */
/*----------------------------------------------------------------------------*/
void XF_Epilog_Column_mapping::vXF_Column_Mapping(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_COLUMN_MAPPING, _FOOTER_);

  m_pCI->vTransform(__func__, XF_EPILOG_COLUMN_MAPPING, "column mapping"_Stz);

  m_pCI->vClearRestart();
  while (m_pCI->G_bEachMain(CK_GBYAGG)) {
    m_pCI->G_vAppendNewChild(CK_GBYAGG, CK_SYNTHESIS);
    m_pCI->G.p_gbyAgg->vColumn_Mapping(m_pCI);
  }

  m_pCI->vClearRestart();
  while (m_pCI->G_bEachMain(CK_SORT)) {
    m_pCI->G_vAppendNewChild(CK_SORT, CK_SYNTHESIS);
    m_pCI->G.p_sort->vColumn_Mapping(m_pCI);
  }

  m_pCI->vClearRestart();
  while (m_pCI->G_bEachMain(CK_TOP)) {
    m_pCI->G_vAppendNewChild(CK_TOP, CK_SYNTHESIS);
    m_pCI->G.p_top->vColumn_Mapping(m_pCI);
  }

  m_pCI->vEndTransform();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
