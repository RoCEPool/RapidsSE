/*============================================================================*/
/* XF_Optimise_Early_projection.cpp -- Early projection                                */
/*============================================================================*/

#include "XF_Optimise_Early_projection.hpp"
#include "eOT.h"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Optimise_Early_projection, CN_XF_OPTIMISE_EARLY_PROJECTION)
__aNewFunction(XF_Optimise_Early_projection, CN_XF_OPTIMISE_EARLY_PROJECTION)
__vDeleteFunction(XF_Optimise_Early_projection, CN_XF_OPTIMISE_EARLY_PROJECTION)
__aFooterFunction(XF_Optimise_Early_projection)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Optimise_Early_projection::XF_Optimise_Early_projection(CardIter* pCI)
: XF_OPTIMISE(CN_XF_OPTIMISE_EARLY_PROJECTION, OT_EARLY_PROJECTION, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_EARLY_PROJECTION, _FOOTER_);

  _checkInitialised_(CN_XF_OPTIMISE_EARLY_PROJECTION);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_EARLY_PROJECTION, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Optimise_Early_projection::~XF_Optimise_Early_projection(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_EARLY_PROJECTION, _FOOTER_);
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

  bool b1;

  b1 = pXF_Early_projection->bOptimise();

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
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_EARLY_PROJECTION, _FOOTER_);

  bool bOptRun = false;

  if (AM::JOB::bHasTag(C_OPTIMISER_EP_DISABLE))
    return bOptRun;

  m_pCI->vTransform(__FILE__, __func__, XF_OPTIMISE_EARLY_PROJECTION);

  RASSERT(nullptr == m_pCI->Early.m_pNeeded_later);

  m_pCI->vBeginEach_tail();

  for (;;) {

    m_pCI->Early.m_pOP_root = nullptr;
    m_pCI->Early.m_pOP_leaf = nullptr;

    bool bEarlyRoot = false;

    while (m_pCI->bEachOp_reverse()) {
      if (m_pCI->E.p_Op->bIsEarlyRoot()) {
        bEarlyRoot = true;
        break;
      }
    }
    if (! bEarlyRoot)
      break;

    switch (m_pCI->E.e_Op) {

      // -- -- -- -- -- -- --
      case CK_o_AGGREG:
        m_pCI->E.p_aggreg->vEarlyProject_root(m_pCI);
        break;

      case CK_o_GBYAGG:
        m_pCI->E.p_gbyAgg->vEarlyProject_root(m_pCI);
        break;

      case CK_o_ORDAGG:
        m_pCI->E.p_ordAgg->vEarlyProject_root(m_pCI);
        break;

        // -- -- -- -- -- -- --
      case CK_o_PROJECT:
        m_pCI->E.p_project->vEarlyProject_root(m_pCI);
        break;

      case CK_o_INDEXJOIN:
        m_pCI->E.p_indexJoin->vEarlyProject_root(m_pCI);
        break;

        // -- -- -- -- -- -- --
      case CK_o_CROSSJOIN:
      case CK_o_INDEXSCAN:
      case CK_o_LIMIT:
      case CK_o_SCAN:
      case CK_o_TOP:
        break;

      default:
        NEVER_REACH;
    }

    if (m_pCI->Early.m_pNeeded_later) {
      DELETE_CARD(m_pCI->Early.m_pNeeded_later);
    }

    bOptRun = true;


    m_pCI->vRepositionOp(m_pCI->Early.m_pOP_leaf);
  }

  m_pCI->vEndTransform(__LINE__);

  return bOptRun;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
