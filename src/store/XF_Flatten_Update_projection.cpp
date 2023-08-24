/*============================================================================*/
/* XF_Flatten_Update_projection.cpp -- Update <projection>                            */
/*============================================================================*/

#include "XF_Flatten_Update_projection.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Flatten_Update_projection, CN_XF_UPDATE_PROJECTION)
__aNewFunction(XF_Flatten_Update_projection, CN_XF_UPDATE_PROJECTION)
__vDeleteFunction(XF_Flatten_Update_projection, CN_XF_UPDATE_PROJECTION)
__aFooterFunction(XF_Flatten_Update_projection)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Flatten_Update_projection::XF_Flatten_Update_projection(CardIter* pCI)
: XF_TRANSFORM(CN_XF_UPDATE_PROJECTION, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_UPDATE_PROJECTION, _FOOTER_);

  _checkInitialised_(CN_XF_UPDATE_PROJECTION);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_UPDATE_PROJECTION, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Flatten_Update_projection::~XF_Flatten_Update_projection(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_UPDATE_PROJECTION, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten_Update_projection::vXF_Update_projection(CardIter* pCI)
{
  _static_

  XF_Flatten_Update_projection* pXF_Update_projection = new XF_Flatten_Update_projection(pCI);

  pXF_Update_projection->vXF_Update_projection();

  delete pXF_Update_projection;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* all $n become $c.n                                                         */
/*----------------------------------------------------------------------------*/
void XF_Flatten_Update_projection::vXF_Update_projection(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_UPDATE_PROJECTION, _FOOTER_);

  m_pCI->vTransform(__func__, XF_FLATTEN_UPDATE_PROJECTION, "Update <projection>"_Stz);

  m_pCI->G.pDeck->vProjectionUpdate(m_pCI);
  m_pCI->vRenumberCards();

  m_pCI->vEndTransform();

  vValidate();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
