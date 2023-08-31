/*============================================================================*/
/* XF_Optimise_Early_projection.cpp -- Early projection                                */
/*============================================================================*/

#include "XF_Optimise_Early_projection.hpp"
#include "eOT.h"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Optimise_Early_projection, CN_XF_OT_EARLY_PROJECTION)
__aNewFunction(XF_Optimise_Early_projection, CN_XF_OT_EARLY_PROJECTION)
__vDeleteFunction(XF_Optimise_Early_projection, CN_XF_OT_EARLY_PROJECTION)
__aFooterFunction(XF_Optimise_Early_projection)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Optimise_Early_projection::XF_Optimise_Early_projection(CardIter* pCI)
: XF_OPTIMISE(CN_XF_OT_EARLY_PROJECTION, OT_EARLY_PROJECTION, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OT_EARLY_PROJECTION, _FOOTER_);

  _checkInitialised_(CN_XF_OT_EARLY_PROJECTION);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_OT_EARLY_PROJECTION, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Optimise_Early_projection::~XF_Optimise_Early_projection(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OT_EARLY_PROJECTION, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Optimise_Early_projection::vOptimise(CardIter* pCI)
{
  _static_

  XF_Optimise_Early_projection* pXF_Early_projection = new XF_Optimise_Early_projection(pCI);

  bool b1 = pXF_Early_projection->bOptimise();

  pXF_Early_projection->vOptimised(b1);

  delete pXF_Early_projection;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool XF_Optimise_Early_projection::bOptimise(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OT_EARLY_PROJECTION, _FOOTER_);

  bool bOptRun = false;

  if (AM::JOB::bHasTag(C_OPTIMISER_EP_DISABLE))
    return bOptRun;

  m_pCI->vTransform(__func__, XF_OPTIMISE_EARLY_PROJECTION, "Early projections"_Stz);

  RASSERT(nullptr == m_pCI->Early.m_pNeeded_later);

  m_pCI->vForward(false);
  m_pCI->vBeginEachReverse();

  for (;;) {

    m_pCI->Early.m_pOP_root = nullptr;
    m_pCI->Early.m_pOP_leaf = nullptr;

    bool bEarlyRoot = false;

    while (m_pCI->G_bEachMain_reverse_NONE()) {
      if (Card_OP::bIsOp(m_pCI->G.p_MainCard) && m_pCI->G.p_OP->bIsEarlyRoot()) {
        bEarlyRoot = true;
        break;
      }
    }
    if (! bEarlyRoot)
      break;

    m_pCI->G.p_OP->vEarlyProject_root(m_pCI);

    if (m_pCI->Early.m_pNeeded_later) {
      DELETE_CARD(m_pCI->Early.m_pNeeded_later);
    }

    bOptRun = true;


    m_pCI->G_vMainResynch(m_pCI->Early.m_pOP_leaf);
  }

  m_pCI->vEndTransform();
//vValidateBench();

  return bOptRun;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
