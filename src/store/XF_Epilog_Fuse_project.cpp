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

  m_pCI->vTransform(__func__, XF_EPILOG_FUSE_PROJECT, "fuse project"_Stz);

  m_pCI->vClearRestart();
  while (m_pCI->G_bEachMain_NONE()) {

    // test for injecting op
    if (! m_pCI->G.p_MainCard->bIsOne(CK_SCAN, CK_INDEXSCAN, CK_INDEXJOIN, CK_CROSSJOIN, CK_NONE))
      goto L_UPDATE_PRIOR_EMITTED;

    m_pCI->Next.m_pProject = _pCard_Project(m_pCI->pNextChildIf(CK_PROJECT));

    m_pCI->G.p_stream = m_pCI->G.p_MainCard->pSub_stream();

    // test for viable next <project>
    if (! m_pCI->Next.m_pProject ||
        m_pCI->Next.m_pProject->bIsCompound() ||
        m_pCI->Next.m_pProject->bIsLiterals()) {
      m_pCI->G.p_stream->vFuse_project_NoViable(m_pCI);

      goto L_UPDATE_PRIOR_EMITTED;
    }

    m_pCI->G.p_stream->vFuse_project_Viable(m_pCI);
    m_pCI->G.pDeck->vDeleteChild(m_pCI->Next.m_pProject);
    m_pCI->G.p_OP->pSymbol()->Y_vSet(P_FusedProject);

    L_UPDATE_PRIOR_EMITTED:
    if (m_pCI->G.p_MainCard->pSub_emitted())
      m_pCI->Prior.m_pEmitted = m_pCI->G.p_MainCard->pSub_emitted();
  }

  m_pCI->vEndTransform();
}
/*----------------------------------------------------------------------------*/
void XF_Epilog_Fuse_project::vFuse_project__(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_FUSE_PROJECT, _FOOTER_);

  m_pCI->vTransform(__func__, XF_EPILOG_FUSE_PROJECT, "fuse project"_Stz);

  m_pCI->vClearRestart();
  while (m_pCI->G_bEachMain(CK_SCAN)) {
    m_pCI->Next.m_pProject = _pCard_Project(m_pCI->pNextChildIf(CK_PROJECT));
    if (! m_pCI->Next.m_pProject ||
        m_pCI->Next.m_pProject->bIsCompound() ||
        m_pCI->Next.m_pProject->bIsLiterals()) {
      m_pCI->G.p_stream = m_pCI->G.p_scan->pSub_stream();
      m_pCI->G.p_stream->vFuse_project_NoViable(m_pCI);
      continue;
    }

    m_pCI->G.p_stream = m_pCI->G.p_scan->pSub_stream();
    m_pCI->G.p_stream->vFuse_project_Viable(m_pCI);
    m_pCI->G.pDeck->vDeleteChild(m_pCI->Next.m_pProject);
    m_pCI->G.p_OP->pSymbol()->Y_vSet(P_FusedProject);
  }

  m_pCI->vClearRestart();
  while (m_pCI->G_bEachMain(CK_INDEXSCAN)) {
    m_pCI->Next.m_pProject = _pCard_Project(m_pCI->pNextChildIf(CK_PROJECT));
    if (! m_pCI->Next.m_pProject ||
        m_pCI->Next.m_pProject->bIsCompound() ||
        m_pCI->Next.m_pProject->bIsLiterals()) {
      m_pCI->G.p_stream = m_pCI->G.p_indexScan->pSub_stream();
      m_pCI->G.p_stream->vFuse_project_NoViable(m_pCI);
      continue;
    }

    m_pCI->G.p_stream = m_pCI->G.p_indexScan->pSub_stream();
    m_pCI->G.p_stream->vFuse_project_Viable(m_pCI);
    m_pCI->G.pDeck->vDeleteChild(m_pCI->Next.m_pProject);
    m_pCI->G.p_OP->pSymbol()->Y_vSet(P_FusedProject);
  }

  m_pCI->vClearRestart();
  while (m_pCI->G_bEachMain_NONE()) {

    if (CK_INDEXJOIN != m_pCI->G.e_MainCard) {
      if (m_pCI->G.p_MainCard->pSub_emitted())
        m_pCI->Prior.m_pEmitted = m_pCI->G.p_MainCard->pSub_emitted();
      continue;
    }

    m_pCI->Next.m_pProject = _pCard_Project(m_pCI->pNextChildIf(CK_PROJECT));
    if (! m_pCI->Next.m_pProject ||
        m_pCI->Next.m_pProject->bIsCompound() ||
        m_pCI->Next.m_pProject->bIsLiterals()) {
      m_pCI->G.p_stream = m_pCI->G.p_indexJoin->pSub_stream();
      m_pCI->G.p_stream->vFuse_project_NoViable(m_pCI);
      m_pCI->Prior.m_pEmitted = m_pCI->G.p_MainCard->pSub_emitted();
      continue;
    }

    // can fuse next projection
    m_pCI->G.p_stream = m_pCI->G.p_indexJoin->pSub_stream();
    m_pCI->G.p_stream->vFuse_project_Viable(m_pCI);
    m_pCI->G.pDeck->vDeleteChild(m_pCI->Next.m_pProject);
    m_pCI->G.p_OP->pSymbol()->Y_vSet(P_FusedProject);
    m_pCI->Prior.m_pEmitted = m_pCI->G.p_MainCard->pSub_emitted();
  }

  m_pCI->vClearRestart();
  while (m_pCI->G_bEachMain_NONE()) {

    if (CK_CROSSJOIN != m_pCI->G.e_MainCard) {
      if (m_pCI->G.p_MainCard->pSub_emitted())
        m_pCI->Prior.m_pEmitted = m_pCI->G.p_MainCard->pSub_emitted();
      continue;
    }

    m_pCI->Next.m_pProject = _pCard_Project(m_pCI->pNextChildIf(CK_PROJECT));
    if (! m_pCI->Next.m_pProject ||
        m_pCI->Next.m_pProject->bIsCompound() ||
        m_pCI->Next.m_pProject->bIsLiterals()) {
      m_pCI->G.p_stream = m_pCI->G.p_crossJoin->pSub_stream();
      m_pCI->G.p_stream->vFuse_project_NoViable(m_pCI);
      m_pCI->Prior.m_pEmitted = m_pCI->G.p_MainCard->pSub_emitted();
      continue;
    }

    // can fuse next projection
    m_pCI->G.p_stream = m_pCI->G.p_crossJoin->pSub_stream();
    m_pCI->G.p_stream->vFuse_project_Viable(m_pCI);
    m_pCI->G.pDeck->vDeleteChild(m_pCI->Next.m_pProject);
    m_pCI->G.p_OP->pSymbol()->Y_vSet(P_FusedProject);
  }

  m_pCI->vEndTransform();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
