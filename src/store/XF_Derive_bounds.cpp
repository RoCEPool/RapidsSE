/*============================================================================*/
/* XF_Derive_bounds.cpp -- Derive Bounds (from filter)                        */
/*============================================================================*/

#include "XF_Derive_bounds.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Derive_bounds, CN_XF_DERIVE_BOUNDS)
__aNewFunction(XF_Derive_bounds, CN_XF_DERIVE_BOUNDS)
__vDeleteFunction(XF_Derive_bounds, CN_XF_DERIVE_BOUNDS)
__aFooterFunction(XF_Derive_bounds)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Derive_bounds::XF_Derive_bounds(CardIter* pCI)
: XF_TRANSFORM(CN_XF_DERIVE_BOUNDS, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_DERIVE_BOUNDS, _FOOTER_);

  _checkInitialised_(CN_XF_DERIVE_BOUNDS);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_DERIVE_BOUNDS, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Derive_bounds::~XF_Derive_bounds(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_DERIVE_BOUNDS, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Derive_bounds::vXF_Derive_bounds(CardIter* pCI)
{
  _static_

  XF_Derive_bounds* pXF_Derive_bounds = new XF_Derive_bounds(pCI);

  pXF_Derive_bounds->vXF_Derive_bounds();

  delete pXF_Derive_bounds;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add level to op                                                            */
/*----------------------------------------------------------------------------*/
void XF_Derive_bounds::vXF_Derive_bounds(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_DERIVE_BOUNDS, _FOOTER_);

  m_pCI->vTransform(__func__, XF_NONE, "derive index bounds from table filter"_Stz);
  Card_Table::vDeriveBoundsFromFilter(m_pCI);
  m_pCI->vEndTransform();

  vValidate();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
