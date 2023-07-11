/*============================================================================*/
/* XF_Column_Mapping.cpp -- Column Mapping                                    */
/*============================================================================*/

#include "XF_Column_Mapping.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Column_Mapping, CN_XF_ADD_LEVEL)
__aNewFunction(XF_Column_Mapping, CN_XF_ADD_LEVEL)
__vDeleteFunction(XF_Column_Mapping, CN_XF_ADD_LEVEL)
__aFooterFunction(XF_Column_Mapping)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Column_Mapping::XF_Column_Mapping(CardIter* pCI)
: XF_TRANSFORM(CN_XF_ADD_LEVEL, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_ADD_LEVEL, _FOOTER_);

  _checkInitialised_(CN_XF_ADD_LEVEL);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_ADD_LEVEL, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Column_Mapping::~XF_Column_Mapping(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_ADD_LEVEL, _FOOTER_);
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
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_ADD_LEVEL, _FOOTER_);

  m_pCI->vTransform(__func__, XF_NONE, "column mapping for gbyAgg"_Stz);


  m_pCI->vEndTransform();

  vValidate();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
