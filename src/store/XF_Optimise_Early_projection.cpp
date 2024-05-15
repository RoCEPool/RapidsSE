/*============================================================================*/
/* XF_Optimise_Early_projection.cpp -- Early projection                       */
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

  b1 = pXF_Early_projection->bEarly_rloop();

  pXF_Early_projection->vOptimised(b1);

  delete pXF_Early_projection;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* reverse loop through deck creating early arc's                             */
/*----------------------------------------------------------------------------*/
bool XF_Optimise_Early_projection::bEarly_rloop(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_EARLY_PROJECTION, _FOOTER_);

  bool bOptRun = false;

  if (AM::JOB::bHasTag(C_OPTIMISER_EP_DISABLE))
    return bOptRun;

  m_pCI->vTransform(__FILE__, __func__, XF_OPTIMISE_EARLY_PROJECTION);

  RASSERT(nullptr == m_pCI->Early.m_pNeeded_later);

  m_pCI->vClearRegisters();
  m_pCI->vBeginEach_tail();

  for (i8 iRoot = 0; bPriorRoot(); iRoot++) {
    RASSERT(m_pCI->Early.m_pNeeded_later);
    vEarly_root(iRoot);
    vEarly_tip(iRoot);
    bOptRun = true;
  }

  m_pCI->vEndTransform(__LINE__);

  return bOptRun;
}

/*----------------------------------------------------------------------------*/
/* scan backward looking for prior arc root                                   */
/*----------------------------------------------------------------------------*/
bool XF_Optimise_Early_projection::bPriorRoot(void)
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
/* process an early arc from this root                                        */
/*----------------------------------------------------------------------------*/
void XF_Optimise_Early_projection::vEarly_root(i8 iRoot)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_EARLY_PROJECTION, _FOOTER_);

  //m_pCI->E.p_Op->vDumpShort(__FUNCTION__,__LINE__,">> root"_Stz,iRoot);
  RASSERT(! m_pCI->pEarlyTip());
  RASSERT(m_pCI->Early.m_pNeeded_later);
  RASSERT(! m_pCI->Arriving.m_pEmitted_prior);

  Card_Project* p_project_early;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // where are starting?

  if (m_pCI->E.p_Op->bIsCard(CK_o_PROJECT)) {
    // - starting new arc from a project

    m_pCI->E.p_Op->vAppendChild(m_pCI->Early.m_pNeeded_later->pDupCardRecursive());
  }
  else {
    // - starting new arc from some other op
    m_pCI->E.p_Op->vDecision(m_pCI,XD_ADD_EARLY_PROJECT,__FUNCTION__ ,__LINE__);

    p_project_early = m_pCI->Early.m_pNeeded_later->pNew_project_early(m_pCI);
    p_project_early->pSymbol()->Y_vSet(P_Line,__LINE__);
    m_pCI->vInsertBefore(p_project_early, m_pCI->E.p_Op);
    m_pCI->vRepositionOp();
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // recursively process body of arc leftwards to tip

  vEarly_body(iRoot, 0, __FUNCTION__, __LINE__);
  RASSERT(m_pCI->pEarlyTip());
  RASSERT(m_pCI->Early.m_pNeeded_later);
  RASSERT(m_pCI->Arriving.m_pEmitted_prior);

  m_pCI->E.p_Op->vRestate_emit(m_pCI);
  m_pCI->vSnapIf_arriving();

  m_pCI->vSnap_arriving(nullptr,nullptr);

  //m_pCI->E.p_Op->vDumpShort(__FUNCTION__,__LINE__,"<< root"_Stz,iRoot);
}

/*----------------------------------------------------------------------------*/
/* process non-root operator sequence of an early arc                         */
/*----------------------------------------------------------------------------*/
void XF_Optimise_Early_projection::vEarly_body(i8 iRoot, i8 iDepth,
                                               const char* azCaller, i8 iLine)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_EARLY_PROJECTION, _FOOTER_);

  RASSERT(m_pCI->Early.m_pNeeded_later);
  RASSERT(! m_pCI->Arriving.m_pEmitted_prior);

  m_pCI->E.p_deck->vRenumberRoot(0);
  m_pCI->E.p_Op->vDumpCaller(__FUNCTION__,">>"_Stz,azCaller,iLine);

  //m_pCI->E.p_Op->pSymbol()->Y_vSet(P_EarlyArc,iRoot);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // back step to prev op

  if (!m_pCI->pPeekPrev()) {
    m_pCI->vEarlyTip(m_pCI->E.p_Op);
    return;
  }

  m_pCI->vPushEach();
  RASSERT(m_pCI->bEachOp_reverse());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // have we arrived at the tip of this arc?

  if (bIsTip()) {
    m_pCI->vSnapIf_arriving();
    m_pCI->vPopEach();
    //m_pCI->E.p_Op->vDumpShort(__FUNCTION__,__LINE__,"<< arcBody"_Stz,iRoot,iDepth);
    m_pCI->vEarlyTip(m_pCI->E.p_Op);
    return;
  }

  // = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
  
  if (m_pCI->E.p_Op->bIsCard(CK_o_PROJECT)) {
    RASSERT (m_pCI->E.p_project->pSymbol()->Y_bHas(P_Early));
    RASSERT(m_pCI->E.p_project->pSub_needed());

    vEarly_body(iRoot, iDepth + 1, __FUNCTION__, __LINE__);
    RASSERT(m_pCI->pEarlyTip());

    m_pCI->E.p_needed = m_pCI->E.p_project->pSub_needed();

    m_pCI->E.p_emit = m_pCI->E.p_project->pCraft_emit_early(m_pCI);
    m_pCI->E.p_project->vAppendChild(m_pCI->E.p_emit);

    m_pCI->vSnapIf_arriving(false);
    m_pCI->vPopEach();
    return;
  }

  // = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

  Card_Needed* p_needed_save = m_pCI->Early.m_pNeeded_later->pClone();

  // which card did we arrive at?
  switch (m_pCI->E.e_Op) {

    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    case CK_o_CROSSJOIN:
      break;

    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    case CK_o_FILTER:
      m_pCI->E.p_filter->vAdd_transients(m_pCI);
      break;

    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    case CK_o_INDEXJOIN:
      m_pCI->E.p_indexJoin->vAdd_transients(m_pCI);
      vInsert_project_early();
      m_pCI->vRepositionOp();
      break;

    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    case CK_o_KEYEXISTS:
      m_pCI->E.p_keyExists->vAdd_transients(m_pCI);
      vInsert_project_early();
      m_pCI->vRepositionOp();
      break;

    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    case CK_o_KEYNOTEXISTS:
      m_pCI->E.p_keyNotExists->vAdd_transients(m_pCI);
      vInsert_project_early();
      m_pCI->vRepositionOp();
      break;

    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    case CK_o_WHERE:
      m_pCI->E.p_where->vAdd_transients(m_pCI);
      break;

    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    default:
      m_pCI->E.p_Op->vDump(__func__);
      NEVER_REACH
  }

  vEarly_body(iRoot, iDepth + 1, __FUNCTION__, __LINE__);
  RASSERT(m_pCI->pEarlyTip());
  RASSERT(m_pCI->Early.m_pNeeded_later);

  m_pCI->E.p_Op->vRestate_emit(m_pCI);

  m_pCI->vSnapIf_arriving();
  m_pCI->vPopEach();

  delete m_pCI->Early.m_pNeeded_later;
  m_pCI->Early.m_pNeeded_later = p_needed_save;

  //m_pCI->E.p_Op->vDumpShort(__FUNCTION__,__LINE__,"<< arcBody"_Stz,iRoot,iDepth);
}

/*----------------------------------------------------------------------------*/
/* is operator the tip/end of an early needed arc?                            */
/*----------------------------------------------------------------------------*/
void XF_Optimise_Early_projection::vEarly_tip(i8 iRoot)
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
/* insert a new arly project                                                  */
/*----------------------------------------------------------------------------*/
void XF_Optimise_Early_projection::vInsert_project_early(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_EARLY_PROJECTION, _FOOTER_);

  Card_Project* p_project_early;

  m_pCI->E.p_Op->vDecision(m_pCI,XD_ADD_EARLY_PROJECT,__FUNCTION__ ,__LINE__);

  p_project_early = m_pCI->Early.m_pNeeded_later->pNew_project_early(m_pCI);
  p_project_early->pSymbol()->Y_vSet(P_Line,__LINE__);

  m_pCI->vInsertBefore(p_project_early, m_pCI->E.p_Op);
}

/*----------------------------------------------------------------------------*/
/* is operator a viable root of early needed arc?                             */
/*----------------------------------------------------------------------------*/
bool XF_Optimise_Early_projection::bIsRoot(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_EARLY_PROJECTION, _FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // operators which could be an early needed root

  switch (m_pCI->E.e_Op) {

    case CK_o_AGGREG:
      if (! m_pCI->E.p_Op->bIsPartitionLevel())
        return false;
      break;

    case CK_o_GBYAGG:
      if (! m_pCI->E.p_Op->bIsPartitionLevel())
        return false;
      break;

    case CK_o_INDEXJOIN:
      RASSERT (m_pCI->E.p_Op->bIsPartitionLevel());
      break;

    case CK_o_ORDAGG:
      RASSERT (m_pCI->E.p_Op->bIsPartitionLevel());
      break;

    case CK_o_PROJECT:
      if (! m_pCI->E.p_Op->bIsPartitionLevel())
        return false;
      break;

    default:
      return false;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // if the previous card is project, assume for the moment it's what's needed

  if (m_pCI->pPeekPrevIf(CK_o_PROJECT)) {
    RASSERT(! m_pCI->pPeekPrev()->pSymbol()->Y_bHas(P_Early));
    return false;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // can a needed root be formed using the emit of this operator?

  m_pCI->E.p_emit = m_pCI->E.p_Op->pSub_emit();
  m_pCI->Early.m_pNeeded_later = m_pCI->E.p_emit->pCraft_needed_root(m_pCI);
  if (! m_pCI->Early.m_pNeeded_later) {
    return false;
  }

  return true;
}

/*----------------------------------------------------------------------------*/
/* is operator the tip of an early needed arc?                                */
/*----------------------------------------------------------------------------*/
bool XF_Optimise_Early_projection::bIsTip(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_EARLY_PROJECTION, _FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // a scan/index-scan is the data source of a stream, so end of this arc

  if (m_pCI->E.p_Op->bIsCard(CK_o_INDEXSCAN) ||
      m_pCI->E.p_Op->bIsCard(CK_o_SCAN))
    return true;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // a prev project is a new arc, so end of this arc

  if (m_pCI->pPeekPrevIf(CK_o_PROJECT))
    return true;

  return false;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
