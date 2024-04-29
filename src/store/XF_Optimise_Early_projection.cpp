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
          vEarlyProject_backstep(m_pCI);
          m_pCI->E.p_aggreg->vRestate_emit(m_pCI);
        }
        break;

      case CK_o_GBYAGG:
        if (m_pCI->E.p_gbyAgg->bEarlyProject_root(m_pCI)) {
          vEarlyProject_backstep(m_pCI);
          m_pCI->E.p_gbyAgg->vRestate_emit(m_pCI);
        }
        break;

      case CK_o_ORDAGG:
        if (m_pCI->E.p_ordAgg->bEarlyProject_root(m_pCI)) {
          vEarlyProject_backstep(m_pCI);
          m_pCI->E.p_ordAgg->vRestate_emit(m_pCI);
        }
        break;

        // -- -- -- -- -- -- --
      case CK_o_PROJECT:
        if (m_pCI->E.p_project->bEarlyProject_root(m_pCI)) {
          vEarlyProject_backstep(m_pCI);

          m_pCI->E.p_Op = m_pCI->E.p_project;
          m_pCI->E.e_Op = CK_o_PROJECT;
          RASSERT(! m_pCI->E.p_project->pSymbol()->Y_bHas(P_Early));

          RASSERT(m_pCI->Early.m_pOP_leaf);
          m_pCI->E.p_Op->vRestate_emit(m_pCI);
        }
        break;

      case CK_o_INDEXJOIN:
        if (m_pCI->E.p_indexJoin->bEarlyProject_root(m_pCI)) {
          vEarlyProject_backstep(m_pCI);
          m_pCI->E.p_indexJoin->vRestate_emit(m_pCI);
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

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Optimise_Early_projection::vEarlyProject_backstep(CardIter* pCI)
{
  _static_

  RASSERT(pCI->Early.m_pNeeded_later);

  //Card_OP* p_OP_leaf_maybe = pCI->G.p_Op;

  if (!pCI->bEachOp_reverse())
    return;

  Card_Needed*  p_needed_save;
  Card_Project* p_project_early;

  // which card did we arrive at?
  switch (pCI->E.e_Op) {


    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    case CK_o_GBYAGG:
      pCI->Early.m_pOP_leaf = pCI->E.p_Op;
      break;

    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    case CK_o_ORDAGG:
      pCI->Early.m_pOP_leaf = pCI->E.p_Op;
      break;

    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    case CK_o_CROSSJOIN:
      p_needed_save = pCI->Early.m_pNeeded_later->pClone();

      pCI->vPushEach();
      XF_Optimise_Early_projection::vEarlyProject_backstep(pCI);
      pCI->vPopEach();

      pCI->E.p_Op->vRestate_emit(pCI);

      delete pCI->Early.m_pNeeded_later;
      pCI->Early.m_pNeeded_later = p_needed_save;

      break;

    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    case CK_o_FILTER:
      p_needed_save = pCI->Early.m_pNeeded_later->pClone();
      pCI->E.p_filter->vEarlyProject_add_transients(pCI);

      pCI->vPushEach();
      XF_Optimise_Early_projection::vEarlyProject_backstep(pCI);
      pCI->vPopEach();

      pCI->E.p_Op->vRestate_emit(pCI);
      delete pCI->Early.m_pNeeded_later;
      pCI->Early.m_pNeeded_later = p_needed_save;

      break;

    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    case CK_o_INDEXJOIN:
      if (pCI->pPeekPrev()->bIsCard(CK_o_GBYAGG) ||
          pCI->pPeekPrev()->bIsCard(CK_o_PROJECT)) {
        pCI->Early.m_pOP_leaf = pCI->E.p_Op;
        pCI->vSnapIf_arriving();
        break;
      }

      p_needed_save = pCI->Early.m_pNeeded_later->pClone();

      // check for additional/temporary transients
      pCI->E.p_indexJoin->vEarlyProject_add_transients(pCI);

      p_project_early = pCI->Early.m_pNeeded_later->pNew_project_early(pCI);
      pCI->vInsertBefore(p_project_early, pCI->E.p_indexJoin);
      pCI->vRepositionOp();

      pCI->vPushEach();
      XF_Optimise_Early_projection::vEarlyProject_backstep(pCI);
      pCI->vPopEach();

//      pCI->E.p_table = pCI->E.p_indexJoin->pSub_table();
//      pCI->E.p_Op = pCI->E.p_indexJoin;
      pCI->E.p_indexJoin->vRestate_emit(pCI);

      delete pCI->Early.m_pNeeded_later;
      pCI->Early.m_pNeeded_later = p_needed_save;

      break;

    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    case CK_o_INDEXSCAN:
      pCI->vPushEach();
      pCI->Early.m_pOP_leaf = pCI->E.p_Op;

      pCI->vSnapIf_arriving();
      pCI->vPopEach();
      break;

    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    case CK_o_KEYEXISTS:
      if (pCI->pPeekPrev()->bIsCard(CK_o_PROJECT)) {
        pCI->Early.m_pOP_leaf = pCI->E.p_Op;
        pCI->vSnapIf_arriving();
        break;
      }

      p_needed_save = pCI->Early.m_pNeeded_later->pClone();
      pCI->E.p_keyExists->vEarlyProject_add_transients(pCI);

      p_project_early = pCI->Early.m_pNeeded_later->pNew_project_early(pCI);
      pCI->vInsertBefore(p_project_early, pCI->E.p_keyExists);
      pCI->vRepositionOp();

      pCI->vPushEach();
      XF_Optimise_Early_projection::vEarlyProject_backstep(pCI);
      pCI->vPopEach();

      pCI->E.p_keyExists->vRestate_emit(pCI);

      delete pCI->Early.m_pNeeded_later;
      pCI->Early.m_pNeeded_later = p_needed_save;

      break;

    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    case CK_o_KEYNOTEXISTS:
      if (pCI->pPeekPrev()->bIsCard(CK_o_PROJECT)) {
        pCI->Early.m_pOP_leaf = pCI->E.p_Op;
        pCI->vSnapIf_arriving();
        break;
      }

      p_needed_save = pCI->Early.m_pNeeded_later->pClone();
      pCI->E.p_keyNotExists->vEarlyProject_add_transients(pCI);

      p_project_early = pCI->Early.m_pNeeded_later->pNew_project_early(pCI);
      pCI->vInsertBefore(p_project_early, pCI->E.p_keyNotExists);
      pCI->vRepositionOp();

      pCI->vPushEach();
      XF_Optimise_Early_projection::vEarlyProject_backstep(pCI);
      pCI->vPopEach();

      pCI->E.p_keyNotExists->vRestate_emit(pCI);

      delete pCI->Early.m_pNeeded_later;
      pCI->Early.m_pNeeded_later = p_needed_save;

      break;

    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    case CK_o_PROJECT:
      if (pCI->E.p_project->pSub_needed()) {
        pCI->vPushEach();
        XF_Optimise_Early_projection::vEarlyProject_backstep(pCI);
        pCI->vPopEach();
        RASSERT(pCI->Early.m_pOP_leaf);
        RASSERT(pCI->E.p_project->pSymbol()->Y_bHas(P_Early));

        if (! (pCI->E.p_needed = pCI->E.p_project->pSub_needed()))
          return;

        pCI->E.p_emit = pCI->E.p_project->pCraft_emit_early(pCI);
        pCI->E.p_project->vAppendChild(pCI->E.p_emit);
        pCI->vSnapIf_arriving(false);
       }
      else {
         pCI->Early.m_pOP_leaf = pCI->E.p_project;
      }
      break;

    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    case CK_o_SCAN:
      pCI->vPushEach();
      pCI->Early.m_pOP_leaf = pCI->E.p_Op;
      pCI->vSnapIf_arriving();
      pCI->vPopEach();
      break;

    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    case CK_o_WHERE:
      pCI->vPushEach();
      p_needed_save = pCI->Early.m_pNeeded_later->pClone();
      pCI->E.p_where->vEarlyProject_recurse_update(pCI);
      pCI->E.p_Op = pCI->E.p_where;
      pCI->E.p_Op->vRestate_emit(pCI);
      delete pCI->Early.m_pNeeded_later;
      pCI->Early.m_pNeeded_later = p_needed_save;

      pCI->vPopEach();
      break;

    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    default:
      pCI->E.p_Op->vDump(__func__);
      NEVER_REACH
  }

  RASSERT(pCI->Early.m_pOP_leaf);
  vDebugPoint();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
