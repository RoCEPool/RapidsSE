/*============================================================================*/
/* XF_Flatten_Delete_fold.cpp -- Delete fold                                  */
/*============================================================================*/

#include "XF_Flatten_Delete_fold.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Flatten_Delete_fold, CN_XF_FLATTEN_DELETE_FLAT)
__aNewFunction(XF_Flatten_Delete_fold, CN_XF_FLATTEN_DELETE_FLAT)
__vDeleteFunction(XF_Flatten_Delete_fold, CN_XF_FLATTEN_DELETE_FLAT)
__aFooterFunction(XF_Flatten_Delete_fold )


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Flatten_Delete_fold::XF_Flatten_Delete_fold(CardIter* pCI)
: XF_TRANSFORM(CN_XF_FLATTEN_DELETE_FLAT, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_DELETE_FLAT, _FOOTER_);

  _checkInitialised_(CN_XF_FLATTEN_DELETE_FLAT);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_DELETE_FLAT, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Flatten_Delete_fold::~XF_Flatten_Delete_fold(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_DELETE_FLAT, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten_Delete_fold::vXF_Delete_fold(CardIter* pCI)
{
  _static_

  XF_Flatten_Delete_fold* pXF_Delete_flat = new XF_Flatten_Delete_fold(pCI);

  pXF_Delete_flat->vXF_Delete_fold();

  delete pXF_Delete_flat;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten_Delete_fold::vXF_Delete_fold(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_DELETE_FLAT, _FOOTER_);

  m_pCI->vTransform(__FILE__, __func__, XF_FLATTEN_DELETE_FOLD);

  m_pCI->vBeginEach();
  while (m_pCI->bEachOp(CK_o_FOLD))
    m_pCI->E.p_deck->vDeleteChild(m_pCI->E.p_fold);

  m_pCI->vEndTransform(__LINE__);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
