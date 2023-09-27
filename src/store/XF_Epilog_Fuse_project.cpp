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

  pFuse_project->vFuse();

  delete pFuse_project;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* fuse <scan> <project/projection> -> <scan/projection>                      */
/*----------------------------------------------------------------------------*/
void XF_Epilog_Fuse_project::vFuse(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_FUSE_PROJECT, _FOOTER_);

  m_pCI->vTransform(__func__, XF_EPILOG_FUSE_PROJECT, "fuse project"_Stz);

  m_pCI->vClearRestart();
  while (m_pCI->G_bEachMain(CK_SCAN)) {
    m_pCI->G.p_project = _pCard_Project(m_pCI->pNextChildIf(CK_PROJECT));
    if (! m_pCI->G.p_project) {
      m_pCI->G.p_stream = m_pCI->G.p_scan->pSub_stream();
      m_pCI->G.p_stream->vFuseSame_projection(m_pCI);
      continue;
    }

    m_pCI->G.p_stream = m_pCI->G.p_scan->pSub_stream();
    m_pCI->G.p_stream->vFuseNext_projection(m_pCI);
    m_pCI->G.pDeck->vDeleteChild(m_pCI->G.p_project);
  }

  m_pCI->vClearRestart();
  while (m_pCI->G_bEachMain(CK_INDEXSCAN)) {
    m_pCI->G.p_project = _pCard_Project(m_pCI->pNextChildIf(CK_PROJECT));
    if (! m_pCI->G.p_project) {
      m_pCI->G.p_stream = m_pCI->G.p_indexScan->pSub_stream();
      m_pCI->G.p_stream->vFuseSame_projection(m_pCI);
      continue;
    }

    m_pCI->G.p_stream = m_pCI->G.p_indexScan->pSub_stream();
    m_pCI->G.p_stream->vFuseNext_projection(m_pCI);
    m_pCI->G.pDeck->vDeleteChild(m_pCI->G.p_project);
  }

  m_pCI->vClearRestart();
  while (m_pCI->G_bEachMain_NONE()) {

    if (CK_INDEXJOIN != m_pCI->G.e_MainCard) {
      if (m_pCI->G.p_MainCard->pSub_projection())
        m_pCI->Prior.m_pProjection = m_pCI->G.p_MainCard->pSub_projection();
      continue;
    }

    m_pCI->G.p_project = _pCard_Project(m_pCI->pNextChildIf(CK_PROJECT));
    if (! m_pCI->G.p_project) {
      m_pCI->G.p_stream = m_pCI->G.p_indexJoin->pSub_stream();
      m_pCI->G.p_stream->vFuseSame_projection(m_pCI);
      m_pCI->Prior.m_pProjection = m_pCI->G.p_MainCard->pSub_projection();
      continue;
    }

    m_pCI->G.p_stream = m_pCI->G.p_indexJoin->pSub_stream();
    m_pCI->G.p_stream->vFuseNext_projection(m_pCI);
    m_pCI->G.pDeck->vDeleteChild(m_pCI->G.p_project);
  }

  m_pCI->vEndTransform();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
