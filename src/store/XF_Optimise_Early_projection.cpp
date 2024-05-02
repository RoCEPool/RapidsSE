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

  b1 = pXF_Early_projection->bNeeded_rloop();

  pXF_Early_projection->vOptimised(b1);

  delete pXF_Early_projection;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool XF_Optimise_Early_projection::bNeeded_rloop(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_EARLY_PROJECTION, _FOOTER_);

  bool bOptRun = false;

  if (AM::JOB::bHasTag(C_OPTIMISER_EP_DISABLE))
    return bOptRun;

  m_pCI->vTransform(__FILE__, __func__, XF_OPTIMISE_EARLY_PROJECTION);

  RASSERT(nullptr == m_pCI->Early.m_pNeeded_later);

  m_pCI->vClearRegisters();
  m_pCI->vBeginEach_tail();

  for (i8 iRoot = 0; bNextRoot();iRoot++) {
    vNeeded_root(iRoot);
    vNeeded_tip(iRoot);
    bOptRun = true;
  }

  m_pCI->vEndTransform(__LINE__);

  return bOptRun;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool XF_Optimise_Early_projection::bNextRoot(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_EARLY_PROJECTION, _FOOTER_);

  bool bFoundNextRoot = false;

  while (m_pCI->bEachOp_reverse()) {
    if (bIsRoot()) {
      bFoundNextRoot = true;
      break;
    }
  }

  return bFoundNextRoot;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Optimise_Early_projection::vNeeded_root(i8 iRoot)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_EARLY_PROJECTION, _FOOTER_);

  m_pCI->E.p_Op->vDumpShort(__FUNCTION__,__LINE__,">> root"_Stz,iRoot);

  Card_Project* p_project_early;

  if (m_pCI->pPeekPrev()->bIsCard(CK_o_PROJECT)) {
    m_pCI->E.p_Op->vDumpShort(__FUNCTION__,__LINE__,"<< root"_Stz,iRoot);
    m_pCI->vEarlyTip(m_pCI->E.p_Op);;
    return;
  }

  switch (m_pCI->E.e_Op) {

    // roots
    case CK_o_AGGREG:
    case CK_o_FILTER:
    case CK_o_GBYAGG:
    case CK_o_INDEXJOIN:
    case CK_o_KEYEXISTS:
    case CK_o_KEYNOTEXISTS:
    case CK_o_PROJECT:
    case CK_o_WHERE:
      break;

    default:
      m_pCI->E.p_Op->vDumpShort(__FUNCTION__,__LINE__,"<< root"_Stz,iRoot);
      return;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_pCI->E.p_emit = m_pCI->E.p_Op->pSub_emit();
  m_pCI->Early.m_pNeeded_later = m_pCI->E.p_emit->pCraft_needed_root(m_pCI);
  if (! m_pCI->Early.m_pNeeded_later) {
    m_pCI->E.p_Op->vDumpShort(__FUNCTION__,__LINE__,"<< root"_Stz,iRoot);
    m_pCI->vEarlyTip(m_pCI->E.p_Op);;
    return;
  }

  if (m_pCI->E.p_Op->bIsCard(CK_o_PROJECT)) {
    m_pCI->E.p_Op->vAppendChild(m_pCI->Early.m_pNeeded_later->pDupCardRecursive());
  }
  else {
    m_pCI->E.p_Op->vDecision(m_pCI,XD_ADD_EARLY_PROJECT,__FUNCTION__ ,__LINE__);
    p_project_early = m_pCI->Early.m_pNeeded_later->pNew_project_early(m_pCI);
    p_project_early->pSymbol()->Y_vSet(P_Line,__LINE__);
    m_pCI->vInsertBefore(p_project_early, m_pCI->E.p_Op);
    m_pCI->vRepositionOp();
  }

  vNeeded_arc(iRoot, 0, __FUNCTION__, __LINE__);

  m_pCI->E.p_Op->vRestate_emit(m_pCI);
  m_pCI->E.p_Op->vDumpShort(__FUNCTION__,__LINE__,"<< root"_Stz,iRoot);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Optimise_Early_projection::vNeeded_arc(i8 iRoot, i8 iDepth,
                                               const char* azCaller, i8 iLine)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_EARLY_PROJECTION, _FOOTER_);

  m_pCI->E.p_deck->vRenumberRoot(0);

  m_pCI->E.p_Op->vDumpCaller(__FUNCTION__,">>"_Stz,azCaller,iLine);

  RASSERT(m_pCI->Early.m_pNeeded_later);

  m_pCI->E.p_Op->pSymbol()->Y_vSet(P_EarlyArc,iRoot);

  Card_Needed*  p_needed_save;
  Card_Project* p_project_early;

  m_pCI->vPushEach();
  if (!m_pCI->bEachOp_reverse()) {
    m_pCI->vPopEach();
    m_pCI->E.p_Op->vDumpShort(__FUNCTION__,__LINE__,"<< backstep"_Stz,iRoot,iDepth);
    m_pCI->vEarlyTip(m_pCI->E.p_Op);
    return;
  }

  if (m_pCI->E.p_Op->bIsCard(CK_o_INDEXSCAN) ||
      m_pCI->E.p_Op->bIsCard(CK_o_SCAN)) {
    m_pCI->vSnapIf_arriving();
    m_pCI->vPopEach();
    m_pCI->E.p_Op->vDumpShort(__FUNCTION__,__LINE__,"<< backstep"_Stz,iRoot,iDepth);
    m_pCI->vEarlyTip(m_pCI->E.p_Op);
    return;
  }

  if (m_pCI->pPeekPrev()->bIsCard(CK_o_PROJECT)) {

    if (m_pCI->bDebugQuery("OptQ3"_Stz)) {
      if (m_pCI->pPeekPrev()->pSymbol()->Y_bHas(P_Early))
        m_pCI->PRINTBOARD_dbg();
      RASSERT(!m_pCI->pPeekPrev()->pSymbol()->Y_bHas(P_Early));
    }

    m_pCI->vSnapIf_arriving();
    m_pCI->vPopEach();
    m_pCI->E.p_Op->vDumpShort(__FUNCTION__,__LINE__,"<< backstep"_Stz,iRoot,iDepth);
    m_pCI->vEarlyTip(m_pCI->E.p_Op);
    return;
  }

  if (m_pCI->E.p_Op->bIsCard(CK_o_PROJECT)) {
    if (m_pCI->E.p_project->pSymbol()->Y_bHas(P_Early)) {
      RASSERT(m_pCI->E.p_project->pSub_needed());

      //m_pCI->PRINTBOARD_dbg();
      vNeeded_arc(iRoot, iDepth + 1, __FUNCTION__, __LINE__);
      m_pCI->E.p_needed = m_pCI->E.p_project->pSub_needed();

      m_pCI->E.p_emit = m_pCI->E.p_project->pCraft_emit_early(m_pCI);
      m_pCI->E.p_project->vAppendChild(m_pCI->E.p_emit);
      m_pCI->vSnapIf_arriving(false);
    }
    m_pCI->E.p_Op->vDumpShort(__FUNCTION__,__LINE__,"<< backstep"_Stz,iRoot,iDepth);
    m_pCI->vPopEach();
    if (! m_pCI->pEarlyTip())
      m_pCI->vEarlyTip(m_pCI->E.p_Op);;
    return;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  p_needed_save = m_pCI->Early.m_pNeeded_later->pClone();


  // which card did we arrive at?
  switch (m_pCI->E.e_Op) {

    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    case CK_o_CROSSJOIN:
      break;

    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    case CK_o_FILTER:
      m_pCI->E.p_filter->vAddTransients(m_pCI);
      break;

    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    case CK_o_INDEXJOIN:
      m_pCI->E.p_indexJoin->vEarlyProject_add_transients(m_pCI);

      if (m_pCI->bDebugQuery("OptQ3"_Stz)) {
        m_pCI->E.p_Op->vDumpShort(__FUNCTION__,__LINE__, "pre-reposition"_Stz,iRoot,iDepth);
        vDebugPoint();
      }

      m_pCI->E.p_Op->vDecision(m_pCI,XD_ADD_EARLY_PROJECT,__FUNCTION__ ,__LINE__);
      p_project_early = m_pCI->Early.m_pNeeded_later->pNew_project_early(m_pCI);
      p_project_early->pSymbol()->Y_vSet(P_Line,__LINE__);
      m_pCI->vInsertBefore(p_project_early, m_pCI->E.p_Op);
      m_pCI->vRepositionOp();
      if (m_pCI->bIsQuery("OptQ3"_Stz))
        m_pCI->E.p_Op->vDumpShort(__FUNCTION__,__LINE__,"post-reposition"_Stz,iRoot,iDepth);
      break;

    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    case CK_o_KEYEXISTS:
      m_pCI->E.p_keyExists->vEarlyProject_add_transients(m_pCI);

      m_pCI->E.p_Op->vDecision(m_pCI,XD_ADD_EARLY_PROJECT,__FUNCTION__ ,__LINE__);
      p_project_early = m_pCI->Early.m_pNeeded_later->pNew_project_early(m_pCI);
      p_project_early->pSymbol()->Y_vSet(P_Line,__LINE__);
      m_pCI->vInsertBefore(p_project_early, m_pCI->E.p_keyExists);
      m_pCI->vRepositionOp();
      break;

    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    case CK_o_KEYNOTEXISTS:
      m_pCI->E.p_keyNotExists->vEarlyProject_add_transients(m_pCI);

      m_pCI->E.p_Op->vDecision(m_pCI,XD_ADD_EARLY_PROJECT,__FUNCTION__ ,__LINE__);
      p_project_early = m_pCI->Early.m_pNeeded_later->pNew_project_early(m_pCI);
      p_project_early->pSymbol()->Y_vSet(P_Line,__LINE__);
      m_pCI->vInsertBefore(p_project_early, m_pCI->E.p_keyNotExists);
      m_pCI->vRepositionOp();
      break;

    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    case CK_o_WHERE:
      m_pCI->E.p_where->vAddTransients(m_pCI);
      break;

    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    default:
      m_pCI->E.p_Op->vDump(__func__);
      NEVER_REACH
  }

  vNeeded_arc(iRoot, iDepth + 1, __FUNCTION__, __LINE__);
  m_pCI->E.p_Op->vRestate_emit(m_pCI);
  delete m_pCI->Early.m_pNeeded_later;
  m_pCI->Early.m_pNeeded_later = p_needed_save;

  m_pCI->vPopEach();
  m_pCI->E.p_Op->vDumpShort(__FUNCTION__,__LINE__,"<< backstep"_Stz,iRoot,iDepth);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Optimise_Early_projection::vNeeded_tip(i8 iRoot)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_EARLY_PROJECTION, _FOOTER_);

  UNUSED(iRoot);

  DELETEIF_CARD(m_pCI->Early.m_pNeeded_later);

  if (m_pCI->pEarlyTip()) {
    m_pCI->vRepositionOp(m_pCI->pEarlyTip());
    m_pCI->vEarlyTip(nullptr);
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool XF_Optimise_Early_projection::bIsArc(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_EARLY_PROJECTION, _FOOTER_);

  if (m_pCI->E.p_Op->bIsCard(CK_o_INDEXSCAN) ||
      m_pCI->E.p_Op->bIsCard(CK_o_SCAN)) {
    return false;
  }

  return true;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool XF_Optimise_Early_projection::bIsRoot(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_EARLY_PROJECTION, _FOOTER_);

  if (! m_pCI->E.p_Op->bIsEarlyRoot())
    return false;

  if (m_pCI->pPeekPrev() && m_pCI->pPeekPrev()->bIsCard(CK_o_PROJECT)) {
    RASSERT(! m_pCI->pPeekPrev()->pSymbol()->Y_bHas(P_Early));
    return false;
  }

  return true;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool XF_Optimise_Early_projection::bIsTip(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_EARLY_PROJECTION, _FOOTER_);

  if (m_pCI->E.p_Op->bIsCard(CK_o_INDEXSCAN) ||
      m_pCI->E.p_Op->bIsCard(CK_o_SCAN)) {
    return true;
  }

  return false;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
