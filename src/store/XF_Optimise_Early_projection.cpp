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

  m_pCI->vClearRegisters();

  m_pCI->vBeginEach_tail();

  i8 iRoot = 0;
  for (;;) {

    bool bNextRoot = false;
    while (m_pCI->bEachOp_reverse()) {
      if (m_pCI->E.p_Op->bIsEarlyRoot()) {
        bNextRoot = true;
        break;
      }
    }
    if (! bNextRoot)
      break;

    vEarlyRoot(m_pCI, iRoot++);
    DELETEIF_CARD(m_pCI->Early.m_pNeeded_later);

    if (m_pCI->pEarlyTip()) {
      m_pCI->vRepositionOp(m_pCI->pEarlyTip());
      m_pCI->vEarlyTip(nullptr);
    }

    bOptRun = true;
  }

  m_pCI->vEndTransform(__LINE__);

  return bOptRun;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Optimise_Early_projection::vBackstep(CardIter* pCI, i8 iRoot, i8 iDepth,
                                             const char* azCaller, i8 iLine)
{
  _static_

  pCI->E.p_deck->vRenumberRoot(0);

  pCI->E.p_Op->vDumpCaller(__FUNCTION__,">>"_Stz,azCaller,iLine);

  RASSERT(pCI->Early.m_pNeeded_later);

  pCI->E.p_Op->pSymbol()->Y_vSet(P_EarlyArc,iRoot);

  Card_Needed*  p_needed_save;
  Card_Project* p_project_early;

  pCI->vPushEach();
  if (!pCI->bEachOp_reverse()) {
    pCI->vPopEach();
    pCI->E.p_Op->vDumpShort(__FUNCTION__,__LINE__,"<< backstep"_Stz,iRoot,iDepth);
    pCI->vEarlyTip(pCI->E.p_Op);
    return;
  }

  if (pCI->E.p_Op->bIsCard(CK_o_INDEXSCAN) ||
      pCI->E.p_Op->bIsCard(CK_o_SCAN)) {
    pCI->vSnapIf_arriving();
    pCI->vPopEach();
    pCI->E.p_Op->vDumpShort(__FUNCTION__,__LINE__,"<< backstep"_Stz,iRoot,iDepth);
    pCI->vEarlyTip(pCI->E.p_Op);
    return;
  }

  if (pCI->pPeekPrev()->bIsCard(CK_o_PROJECT)) {

    if (pCI->bDebugQuery("OptQ3"_Stz)) {
      if (pCI->pPeekPrev()->pSymbol()->Y_bHas(P_Early))
        pCI->PRINTBOARD_dbg();
      RASSERT(!pCI->pPeekPrev()->pSymbol()->Y_bHas(P_Early));
    }

    pCI->vSnapIf_arriving();
    pCI->vPopEach();
    pCI->E.p_Op->vDumpShort(__FUNCTION__,__LINE__,"<< backstep"_Stz,iRoot,iDepth);
    pCI->vEarlyTip(pCI->E.p_Op);
    return;
  }

  if (pCI->E.p_Op->bIsCard(CK_o_PROJECT)) {
    if (pCI->E.p_project->pSymbol()->Y_bHas(P_Early)) {
      RASSERT(pCI->E.p_project->pSub_needed());

      //pCI->PRINTBOARD_dbg();
      vBackstep(pCI,iRoot,iDepth+1,__FUNCTION__ ,__LINE__);
      pCI->E.p_needed = pCI->E.p_project->pSub_needed();

      pCI->E.p_emit = pCI->E.p_project->pCraft_emit_early(pCI);
      pCI->E.p_project->vAppendChild(pCI->E.p_emit);
      pCI->vSnapIf_arriving(false);
    }
    pCI->E.p_Op->vDumpShort(__FUNCTION__,__LINE__,"<< backstep"_Stz,iRoot,iDepth);
    pCI->vPopEach();
    if (! pCI->pEarlyTip())
      pCI->vEarlyTip(pCI->E.p_Op);;
    return;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  p_needed_save = pCI->Early.m_pNeeded_later->pClone();


  // which card did we arrive at?
  switch (pCI->E.e_Op) {

    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    case CK_o_CROSSJOIN:
      break;

    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    case CK_o_FILTER:
      pCI->E.p_filter->vAddTransients(pCI);
      break;

    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    case CK_o_INDEXJOIN:
      pCI->E.p_indexJoin->vEarlyProject_add_transients(pCI);

      if (pCI->bDebugQuery("OptQ3"_Stz)) {
        pCI->E.p_Op->vDumpShort(__FUNCTION__,__LINE__, "pre-reposition"_Stz,iRoot,iDepth);
        vDebugPoint();
      }

      pCI->E.p_Op->vDecision(pCI,XD_ADD_EARLY_PROJECT,__FUNCTION__ ,__LINE__);
      p_project_early = pCI->Early.m_pNeeded_later->pNew_project_early(pCI);
      p_project_early->pSymbol()->Y_vSet(P_Line,__LINE__);
      pCI->vInsertBefore(p_project_early, pCI->E.p_Op);
      pCI->vRepositionOp();
      if (pCI->bIsQuery("OptQ3"_Stz))
        pCI->E.p_Op->vDumpShort(__FUNCTION__,__LINE__,"post-reposition"_Stz,iRoot,iDepth);
      break;

    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    case CK_o_KEYEXISTS:
      pCI->E.p_keyExists->vEarlyProject_add_transients(pCI);

      pCI->E.p_Op->vDecision(pCI,XD_ADD_EARLY_PROJECT,__FUNCTION__ ,__LINE__);
      p_project_early = pCI->Early.m_pNeeded_later->pNew_project_early(pCI);
      p_project_early->pSymbol()->Y_vSet(P_Line,__LINE__);
      pCI->vInsertBefore(p_project_early, pCI->E.p_keyExists);
      pCI->vRepositionOp();
      break;

    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    case CK_o_KEYNOTEXISTS:
      pCI->E.p_keyNotExists->vEarlyProject_add_transients(pCI);

      pCI->E.p_Op->vDecision(pCI,XD_ADD_EARLY_PROJECT,__FUNCTION__ ,__LINE__);
      p_project_early = pCI->Early.m_pNeeded_later->pNew_project_early(pCI);
      p_project_early->pSymbol()->Y_vSet(P_Line,__LINE__);
      pCI->vInsertBefore(p_project_early, pCI->E.p_keyNotExists);
      pCI->vRepositionOp();
      break;

    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    case CK_o_WHERE:
      pCI->E.p_where->vAddTransients(pCI);
      break;

    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    default:
      pCI->E.p_Op->vDump(__func__);
      NEVER_REACH
  }

  vBackstep(pCI,iRoot,iDepth+1,__FUNCTION__ ,__LINE__);
  pCI->E.p_Op->vRestate_emit(pCI);
  delete pCI->Early.m_pNeeded_later;
  pCI->Early.m_pNeeded_later = p_needed_save;

  pCI->vPopEach();
  pCI->E.p_Op->vDumpShort(__FUNCTION__,__LINE__,"<< backstep"_Stz,iRoot,iDepth);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Optimise_Early_projection::vEarlyRoot(CardIter* pCI, i8 iRoot)
{
  _static_

  pCI->E.p_Op->vDumpShort(__FUNCTION__,__LINE__,">> root"_Stz,iRoot);

  Card_Project* p_project_early;

  if (pCI->pPeekPrev()->bIsCard(CK_o_PROJECT)) {
    pCI->E.p_Op->vDumpShort(__FUNCTION__,__LINE__,"<< root"_Stz,iRoot);
    pCI->vEarlyTip(pCI->E.p_Op);;
    return;
  }

  switch (pCI->E.e_Op) {

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
      pCI->E.p_Op->vDumpShort(__FUNCTION__,__LINE__,"<< root"_Stz,iRoot);
      return;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pCI->E.p_emit = pCI->E.p_Op->pSub_emit();
  pCI->Early.m_pNeeded_later = pCI->E.p_emit->pCraft_needed_root(pCI);
  if (! pCI->Early.m_pNeeded_later) {
    pCI->E.p_Op->vDumpShort(__FUNCTION__,__LINE__,"<< root"_Stz,iRoot);
    pCI->vEarlyTip(pCI->E.p_Op);;
    return;
  }

  if (pCI->E.p_Op->bIsCard(CK_o_PROJECT)) {
    pCI->E.p_Op->vAppendChild(pCI->Early.m_pNeeded_later->pDupCardRecursive());
  }
  else {
    pCI->E.p_Op->vDecision(pCI,XD_ADD_EARLY_PROJECT,__FUNCTION__ ,__LINE__);
    p_project_early = pCI->Early.m_pNeeded_later->pNew_project_early(pCI);
    p_project_early->pSymbol()->Y_vSet(P_Line,__LINE__);
    pCI->vInsertBefore(p_project_early, pCI->E.p_Op);
    pCI->vRepositionOp();
  }

  vBackstep(pCI,iRoot,0,__FUNCTION__ ,__LINE__);

  pCI->E.p_Op->vRestate_emit(pCI);
  pCI->E.p_Op->vDumpShort(__FUNCTION__,__LINE__,"<< root"_Stz,iRoot);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
