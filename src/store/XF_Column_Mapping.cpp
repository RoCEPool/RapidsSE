/*============================================================================*/
/* XF_Column_Mapping.cpp -- Column Mapping                                    */
/*============================================================================*/

#include "XF_Column_Mapping.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Column_Mapping, CN_XF_COLUMN_MAPPING)
__aNewFunction(XF_Column_Mapping, CN_XF_COLUMN_MAPPING)
__vDeleteFunction(XF_Column_Mapping, CN_XF_COLUMN_MAPPING)
__aFooterFunction(XF_Column_Mapping)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Column_Mapping::XF_Column_Mapping(CardIter* pCI)
: XF_TRANSFORM(CN_XF_COLUMN_MAPPING, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_COLUMN_MAPPING, _FOOTER_);

  _checkInitialised_(CN_XF_COLUMN_MAPPING);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_COLUMN_MAPPING, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Column_Mapping::~XF_Column_Mapping(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_COLUMN_MAPPING, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Column_Mapping::vXF_Column_Mapping(CardIter* pCI)
{
  _static_

  XF_Column_Mapping* pXF_Column_Mapping = new XF_Column_Mapping(pCI);

  pXF_Column_Mapping->vXF_Column_Mapping();

  delete pXF_Column_Mapping;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* column mapping                                                             */
/*----------------------------------------------------------------------------*/
void XF_Column_Mapping::vXF_Column_Mapping(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_COLUMN_MAPPING, _FOOTER_);

  m_pCI->vTransform(__func__, XF_NONE, "column mapping for gbyAgg"_Stz);
  m_pCI->G_vRestart();
  while (m_pCI->G_bEachMain(CK_GBYAGG)) {
    m_pCI->G.p_gbyAgg->vColumn_Mapping(m_pCI);
  }
  m_pCI->vEndTransform();


  m_pCI->vTransform(__func__, XF_NONE, "column mapping for sort"_Stz);
  m_pCI->G_vRestart();
  while (m_pCI->G_bEachMain(CK_SORT)) {
    m_pCI->G.p_sort->vColumn_Mapping(m_pCI);
  }
  m_pCI->vEndTransform();


  m_pCI->vTransform(__func__, XF_NONE, "column mapping for top"_Stz);
  m_pCI->G_vRestart();
  while (m_pCI->G_bEachMain(CK_TOP)) {
    m_pCI->G.p_top->vColumn_Mapping(m_pCI);
  }
  m_pCI->vEndTransform();

  vValidate();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
