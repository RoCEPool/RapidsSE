/*============================================================================*/
/* XF_Optimise_Early_projection.cpp -- Early projection                                */
/*============================================================================*/

#include "XF_Optimise_Early_projection.hpp"
#include "eOT.h"

#include "Card_OP.hpp"

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
        if (m_pCI->E.p_aggreg->bEarlyProject_root(m_pCI)) {
          m_pCI->E.p_aggreg->Card_OP::vEarlyProject_backstep(m_pCI);

          RASSERT(m_pCI->Early.m_pOP_leaf);
          m_pCI->E.p_aggreg->vRestate_emit(m_pCI);
        }
        else {

        }
        break;

      case CK_o_GBYAGG:
        if (m_pCI->E.p_gbyAgg->bEarlyProject_root(m_pCI)) {
          m_pCI->E.p_gbyAgg->Card_OP::vEarlyProject_backstep(m_pCI);

          RASSERT(m_pCI->Early.m_pOP_leaf);
          m_pCI->E.p_gbyAgg->vRestate_emit(m_pCI);
        }
        else {

        }
        break;

      case CK_o_ORDAGG:
        if (m_pCI->E.p_ordAgg->bEarlyProject_root(m_pCI)) {
          m_pCI->E.p_ordAgg->Card_OP::vEarlyProject_backstep(m_pCI);

          RASSERT(m_pCI->Early.m_pOP_leaf);
          m_pCI->E.p_ordAgg->vRestate_emit(m_pCI);
        }
        else {

        }
        break;

        // -- -- -- -- -- -- --
      case CK_o_PROJECT:
        if (m_pCI->E.p_project->bEarlyProject_root(m_pCI)) {
          m_pCI->E.p_project->Card_OP::vEarlyProject_backstep(m_pCI);

          m_pCI->E.p_Op = m_pCI->E.p_project;
          m_pCI->E.e_Op = CK_o_PROJECT;

          RASSERT(m_pCI->Early.m_pOP_leaf);
          RASSERT(! m_pCI->E.p_project->pSymbol()->Y_bHas(P_Early));
          m_pCI->E.p_Op->vRestate_emit(m_pCI);
        }
        else {

        }
        break;

      case CK_o_INDEXJOIN:
        if (m_pCI->E.p_indexJoin->bEarlyProject_root(m_pCI)) {
          m_pCI->E.p_indexJoin->Card_OP::vEarlyProject_backstep(m_pCI);

          RASSERT(m_pCI->Early.m_pOP_leaf);
          m_pCI->E.p_indexJoin->vRestate_emit(m_pCI);
        }
        else {

        }
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
