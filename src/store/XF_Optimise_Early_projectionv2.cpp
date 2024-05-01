/*============================================================================*/
/* XF_Optimise_Early_projectionv2.cpp -- Early projection                                */
/*============================================================================*/

#include "XF_Optimise_Early_projectionv2.hpp"
#include "eOT.h"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Optimise_Early_projectionv2, CN_XF_OPTIMISE_EARLY_PROJECTION)
__aNewFunction(XF_Optimise_Early_projectionv2, CN_XF_OPTIMISE_EARLY_PROJECTION)
__vDeleteFunction(XF_Optimise_Early_projectionv2, CN_XF_OPTIMISE_EARLY_PROJECTION)
__aFooterFunction(XF_Optimise_Early_projectionv2)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Optimise_Early_projectionv2::XF_Optimise_Early_projectionv2(CardIter* pCI)
: XF_OPTIMISE(CN_XF_OPTIMISE_EARLY_PROJECTION, OT_EARLY_PROJECTION, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_EARLY_PROJECTION, _FOOTER_);

  _checkInitialised_(CN_XF_OPTIMISE_EARLY_PROJECTION);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_EARLY_PROJECTION, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Optimise_Early_projectionv2::~XF_Optimise_Early_projectionv2(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_EARLY_PROJECTION, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Optimise_Early_projectionv2::vOptimise(CardIter* pCI)
{
  _static_
  UNUSED(pCI);
  
#ifdef Z_NEEDEDv2

  XF_Optimise_Early_projectionv2* pXF_Early_projection = new XF_Optimise_Early_projectionv2(pCI);

  bool b1 = true;

  pXF_Early_projection->vEarly_needed_loop();

  pXF_Early_projection->vOptimised(b1);

  delete pXF_Early_projection;
#endif
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* loop thru operators / early projection                                     */
/*----------------------------------------------------------------------------*/
void XF_Optimise_Early_projectionv2::vEarly_needed_loop(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_EARLY_PROJECTION, _FOOTER_);

  m_pCI->vTransform(__FILE__, __func__, XF_OPTIMISE_EARLY_PROJECTION);

  RASSERT(nullptr == m_pCI->Early.m_pNeeded_later);

  m_pCI->vBeginEach_tail();

  while (m_pCI->bEachOp_reverse())
    vEarly_needed_OP();

  m_pCI->vEndTransform(__LINE__);

  m_pCI->Early.m_pNeeded_later = nullptr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Optimise_Early_projectionv2::vEarly_needed_OP(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_EARLY_PROJECTION, _FOOTER_);

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
