/*============================================================================*/
/* XF_Delete_flat.cpp -- Delete flat                                          */
/*============================================================================*/

#include "XF_Delete_flat.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Delete_flat, CN_XF_DELETE_FLAT)
__aNewFunction(XF_Delete_flat, CN_XF_DELETE_FLAT)
__vDeleteFunction(XF_Delete_flat, CN_XF_DELETE_FLAT)
__aFooterFunction(XF_Delete_flat)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Delete_flat::XF_Delete_flat(CardIter* pCI)
: XF_TRANSFORM(CN_XF_DELETE_FLAT, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_DELETE_FLAT, _FOOTER_);

  _checkInitialised_(CN_XF_DELETE_FLAT);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_DELETE_FLAT, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Delete_flat::~XF_Delete_flat(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_DELETE_FLAT, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Delete_flat::vXF_Delete_flat(CardIter* pCI)
{
  _static_

  XF_Delete_flat* pXF_Delete_flat = new XF_Delete_flat(pCI);

  pXF_Delete_flat->vXF_Delete_flat();

  delete pXF_Delete_flat;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Delete_flat::vXF_Delete_flat(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_DELETE_FLAT, _FOOTER_);

  m_pCI->vRestart();
  while (m_pCI->G_bEachMain(CK_FLAT))
    m_pCI->G.pDeck->vDeleteChild(m_pCI->G.p_flat);

  vDebugPoint();

  vValidate();

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
