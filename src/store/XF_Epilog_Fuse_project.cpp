/*============================================================================*/
/* XF_Epilog_Fuse_project.cpp -- Fuse Project                                 */
/*============================================================================*/

#include "XF_Epilog_Fuse_project.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Epilog_Fuse_project, CN_XF_EPILOG_FUSE_PROJECT)
__aNewFunction(XF_Epilog_Fuse_project, CN_XF_EPILOG_FUSE_PROJECT)
__vDeleteFunction(XF_Epilog_Fuse_project, CN_XF_EPILOG_FUSE_PROJECT)
__aFooterFunction(XF_Epilog_Fuse_project)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Epilog_Fuse_project::XF_Epilog_Fuse_project(CardIter* pCI)
: XF_EPILOG(CN_XF_EPILOG_FUSE_PROJECT, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_FUSE_PROJECT, _FOOTER_);

  _checkInitialised_(CN_XF_EPILOG_FUSE_PROJECT);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_FUSE_PROJECT, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Epilog_Fuse_project::~XF_Epilog_Fuse_project(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_FUSE_PROJECT, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Epilog_Fuse_project::vFuse(CardIter* pCI)
{
  _static_

  XF_Epilog_Fuse_project* pFuse_project = new XF_Epilog_Fuse_project(pCI);

  pFuse_project->vFuse_project();

  delete pFuse_project;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* fuse <OP injecting> <project/emitted> -> <OP/stream/emitted>               */
/*----------------------------------------------------------------------------*/
void XF_Epilog_Fuse_project::vFuse_project(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_FUSE_PROJECT, _FOOTER_);

  m_pCI->vTransform(__FILE__, __func__, XF_EPILOG_FUSE_PROJECT);

  m_pCI->vClearRegisters();
  m_pCI->vBeginEach();
  while (m_pCI->bEachOp()) {

    // test for injecting op
    if (! m_pCI->E.p_Op->bEither(CK_o_SCAN, CK_o_INDEXSCAN, CK_o_INDEXJOIN, CK_o_CROSSJOIN, CK_NONE))
      goto L_UPDATE_PRIOR_EMITTED;

    m_pCI->Next.m_pProject = _pCard_Project(m_pCI->pPeekNextIf(CK_o_PROJECT));

    m_pCI->E.p_stream = m_pCI->E.p_Op->pSub_stream();

    // test for viable next <project>
    if (! m_pCI->Next.m_pProject ||
        m_pCI->Next.m_pProject->bIsCompound() ||
        m_pCI->Next.m_pProject->bIsLiterals(m_pCI)) {

      goto L_UPDATE_PRIOR_EMITTED;
    }

    m_pCI->E.p_stream->vFuse_project_Viable(m_pCI);
    m_pCI->E.p_deck->vDeleteChild(m_pCI->Next.m_pProject);
    m_pCI->E.p_Op->pSymbol()->Y_vSet(P_FusedProject);

    L_UPDATE_PRIOR_EMITTED:
    if (m_pCI->E.p_Op->pSub_emit())
      m_pCI->Dic.m_p_expr = m_pCI->Flux.m_pArriving = m_pCI->E.p_Op->pSub_emit();
  }

  m_pCI->vEndTransform(__LINE__);
}
/*----------------------------------------------------------------------------*/
void XF_Epilog_Fuse_project::vFuse_project__(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_FUSE_PROJECT, _FOOTER_);

  m_pCI->vTransform(__FILE__, __func__, XF_EPILOG_FUSE_PROJECT);

  m_pCI->vClearRegisters();
  m_pCI->vBeginEach();
  while (m_pCI->bEachOp(CK_o_SCAN)) {
    m_pCI->Next.m_pProject = _pCard_Project(m_pCI->pPeekNextIf(CK_o_PROJECT));
    if (! m_pCI->Next.m_pProject ||
        m_pCI->Next.m_pProject->bIsCompound() ||
        m_pCI->Next.m_pProject->bIsLiterals(m_pCI)) {
      m_pCI->E.p_stream = m_pCI->E.p_scan->pSub_stream();
      continue;
    }

    m_pCI->E.p_stream = m_pCI->E.p_scan->pSub_stream();
    m_pCI->E.p_stream->vFuse_project_Viable(m_pCI);
    m_pCI->E.p_deck->vDeleteChild(m_pCI->Next.m_pProject);
    m_pCI->E.p_Op->pSymbol()->Y_vSet(P_FusedProject);
  }

  m_pCI->vClearRegisters();
  m_pCI->vBeginEach();
  while (m_pCI->bEachOp(CK_o_INDEXSCAN)) {
    m_pCI->Next.m_pProject = _pCard_Project(m_pCI->pPeekNextIf(CK_o_PROJECT));
    if (! m_pCI->Next.m_pProject ||
        m_pCI->Next.m_pProject->bIsCompound() ||
        m_pCI->Next.m_pProject->bIsLiterals(m_pCI)) {
      m_pCI->E.p_stream = m_pCI->E.p_indexScan->pSub_stream();
      continue;
    }

    m_pCI->E.p_stream = m_pCI->E.p_indexScan->pSub_stream();
    m_pCI->E.p_stream->vFuse_project_Viable(m_pCI);
    m_pCI->E.p_deck->vDeleteChild(m_pCI->Next.m_pProject);
    m_pCI->E.p_Op->pSymbol()->Y_vSet(P_FusedProject);
  }

  m_pCI->vClearRegisters();
  m_pCI->vBeginEach();
  while (m_pCI->bEachOp()) {

    if (CK_o_INDEXJOIN != m_pCI->E.e_Op) {
      if (m_pCI->E.p_Op->pSub_emit())
        m_pCI->Dic.m_p_expr = m_pCI->Flux.m_pArriving = m_pCI->E.p_Op->pSub_emit();
      continue;
    }

    m_pCI->Next.m_pProject = _pCard_Project(m_pCI->pPeekNextIf(CK_o_PROJECT));
    if (! m_pCI->Next.m_pProject ||
        m_pCI->Next.m_pProject->bIsCompound() ||
        m_pCI->Next.m_pProject->bIsLiterals(m_pCI)) {
      m_pCI->E.p_stream = m_pCI->E.p_indexJoin->pSub_stream();
      m_pCI->Dic.m_p_expr = m_pCI->Flux.m_pArriving = m_pCI->E.p_Op->pSub_emit();
      continue;
    }

    // can fuse next projection
    m_pCI->E.p_stream = m_pCI->E.p_indexJoin->pSub_stream();
    m_pCI->E.p_stream->vFuse_project_Viable(m_pCI);
    m_pCI->E.p_deck->vDeleteChild(m_pCI->Next.m_pProject);
    m_pCI->E.p_Op->pSymbol()->Y_vSet(P_FusedProject);
    m_pCI->Dic.m_p_expr = m_pCI->Flux.m_pArriving = m_pCI->E.p_Op->pSub_emit();
  }

  m_pCI->vClearRegisters();
  m_pCI->vBeginEach();
  while (m_pCI->bEachOp()) {

    if (CK_o_CROSSJOIN != m_pCI->E.e_Op) {
      if (m_pCI->E.p_Op->pSub_emit())
        m_pCI->Dic.m_p_expr = m_pCI->Flux.m_pArriving = m_pCI->E.p_Op->pSub_emit();
      continue;
    }

    m_pCI->Next.m_pProject = _pCard_Project(m_pCI->pPeekNextIf(CK_o_PROJECT));
    if (! m_pCI->Next.m_pProject ||
        m_pCI->Next.m_pProject->bIsCompound() ||
        m_pCI->Next.m_pProject->bIsLiterals(m_pCI)) {
      m_pCI->E.p_stream = m_pCI->E.p_crossJoin->pSub_stream();
      m_pCI->Dic.m_p_expr = m_pCI->Flux.m_pArriving = m_pCI->E.p_Op->pSub_emit();
      continue;
    }

    // can fuse next projection
    m_pCI->E.p_stream = m_pCI->E.p_crossJoin->pSub_stream();
    m_pCI->E.p_stream->vFuse_project_Viable(m_pCI);
    m_pCI->E.p_deck->vDeleteChild(m_pCI->Next.m_pProject);
    m_pCI->E.p_Op->pSymbol()->Y_vSet(P_FusedProject);
  }

  m_pCI->vEndTransform(__LINE__);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
