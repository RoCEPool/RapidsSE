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

  b1 = pXF_Early_projection->bEarly_needed_loop();
  b1 = pXF_Early_projection->bEarly_projection_loop();

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

  m_pCI->vForwards(false);
  m_pCI->vBeginEachReverse();

  for (;;) {

    m_pCI->Early.m_pOP_root = nullptr;
    m_pCI->Early.m_pOP_leaf = nullptr;

    bool bEarlyRoot = false;

    while (m_pCI->bEachMain_reverse_NONE()) {
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


    m_pCI->vMainResynch(m_pCI->Early.m_pOP_leaf);
  }

  m_pCI->vEndTransform(__LINE__);

  return bOptRun;
}

/*----------------------------------------------------------------------------*/
/* loop thru operators / early projection                                     */
/*----------------------------------------------------------------------------*/
bool XF_Optimise_Early_projection::bEarly_needed_loop(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_EARLY_PROJECTION, _FOOTER_);

  bool bOptRun = false;

  m_pCI->vTransform(__FILE__, __func__, XF_OPTIMISE_EARLY_PROJECTION);

  RASSERT(nullptr == m_pCI->Early.m_pNeeded_later);

  m_pCI->vForwards(false);
  m_pCI->vBeginEachReverse();

  while (m_pCI->bEachMain_reverse_NONE()) {
    bOptRun = bEarly_needed_OP();
  }

  m_pCI->vEndTransform(__LINE__);

  m_pCI->Early.m_pNeeded_later = nullptr;

  return bOptRun;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool XF_Optimise_Early_projection::bEarly_needed_OP(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_EARLY_PROJECTION, _FOOTER_);

  bool bOptRun = false;

  RASSERT(m_pCI->G.p_MainCard->bIsOp());
  switch (m_pCI->G.e_MainCard) {

    case CK_o_AGGREG:
    case CK_o_CALL:
    case CK_o_CAST:
    case CK_c_COMBINED:
    case CK_o_CORRELATION:
    case CK_o_CROSSJOIN:
    case CK_o_FILTER:
    case CK_o_FROM:
    case CK_o_GBYAGG:
    case CK_o_HASH:
    case CK_o_HAVING:
    case CK_o_INDEXSCAN:
    case CK_o_INDEXJOIN:
    case CK_o_INFLATE:
    case CK_o_KEYEXISTS:
    case CK_o_KEYNOTEXISTS:
    case CK_o_LIMIT:
    case CK_o_ORDAGG:
    case CK_o_PROJECT:
    case CK_o_SCAN:
    case CK_o_SORT:
    case CK_o_TOP:
    case CK_o_WHERE:
      break;

    default:
      break;
  }

  if (m_pCI->G.p_OP->bIsEarlyRoot()) {

    m_pCI->G.p_emit = m_pCI->G.p_OP->pSub_emit();
    m_pCI->G.p_key  = m_pCI->G.p_OP->pSub_key();

    switch (m_pCI->G.e_MainCard) {
      case CK_o_AGGREG:
        m_pCI->G.p_aggreg->vCraft_needed_root(m_pCI);
        break;

      case CK_o_GBYAGG:
        m_pCI->G.p_gbyAgg->vCraft_needed_root(m_pCI);
        break;

      case CK_o_ORDAGG:
        m_pCI->G.p_ordAgg->vCraft_needed_root(m_pCI);
        break;

        // -- -- -- -- -- -- --
      case CK_o_PROJECT:
        m_pCI->G.p_project->vCraft_needed_root(m_pCI);
        break;

      case CK_o_INDEXJOIN:
        m_pCI->G.p_indexJoin->vCraft_needed_root(m_pCI);
        break;

      default:
        NEVER_REACH
    }
  }

  return bOptRun;
}

/*----------------------------------------------------------------------------*/
/* loop thru operators / early projection                                     */
/*----------------------------------------------------------------------------*/
bool XF_Optimise_Early_projection::bEarly_projection_loop(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_EARLY_PROJECTION, _FOOTER_);

  bool bOptRun = false;

  m_pCI->vTransform(__FILE__, __func__, XF_OPTIMISE_EARLY_PROJECTION);

  RASSERT(nullptr == m_pCI->Early.m_pNeeded_later);

  m_pCI->vBeginEach();

  while (m_pCI->bEachMain()) {
    bOptRun = bEarly_projection_OP();
  }

  m_pCI->vEndTransform(__LINE__);

  m_pCI->Early.m_pNeeded_later = nullptr;

  return bOptRun;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool XF_Optimise_Early_projection::bEarly_projection_OP(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_EARLY_PROJECTION, _FOOTER_);

  bool bOptRun = false;

  RASSERT(m_pCI->G.p_MainCard->bIsOp());
  switch (m_pCI->G.e_MainCard) {

    case CK_o_AGGREG:
    case CK_o_CALL:
    case CK_o_CAST:
    case CK_c_COMBINED:
    case CK_o_CORRELATION:
    case CK_o_CROSSJOIN:
    case CK_o_FILTER:
    case CK_o_FROM:
    case CK_o_GBYAGG:
    case CK_o_HASH:
    case CK_o_HAVING:
    case CK_o_INDEXSCAN:
    case CK_o_INDEXJOIN:
    case CK_o_INFLATE:
    case CK_o_KEYEXISTS:
    case CK_o_KEYNOTEXISTS:
    case CK_o_LIMIT:
    case CK_o_ORDAGG:
    case CK_o_PROJECT:
    case CK_o_SCAN:
    case CK_o_SORT:
    case CK_o_TOP:
    case CK_o_WHERE:
      break;

    default:
      break;
  }

  return bOptRun;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
