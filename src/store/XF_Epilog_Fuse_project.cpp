/*============================================================================*/
/* XF_Epilog_Fuse_project.cpp -- Column Mapping                                    */
/*============================================================================*/

#include "XF_Epilog_Fuse_project.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Epilog_Fuse_project, CN_XF_FUSE_PROJECT)
__aNewFunction(XF_Epilog_Fuse_project, CN_XF_FUSE_PROJECT)
__vDeleteFunction(XF_Epilog_Fuse_project, CN_XF_FUSE_PROJECT)
__aFooterFunction(XF_Epilog_Fuse_project)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Epilog_Fuse_project::XF_Epilog_Fuse_project(CardIter* pCI)
: XF_EPILOG(CN_XF_FUSE_PROJECT, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FUSE_PROJECT, _FOOTER_);

  _checkInitialised_(CN_XF_FUSE_PROJECT);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_FUSE_PROJECT, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Epilog_Fuse_project::~XF_Epilog_Fuse_project(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FUSE_PROJECT, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Epilog_Fuse_project::vEpilog(CardIter* pCI)
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
/* fuse projections                                                           */
/*----------------------------------------------------------------------------*/
void XF_Epilog_Fuse_project::vFuse(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FUSE_PROJECT, _FOOTER_);

  m_pCI->vTransform(__func__, XF_EPILOG_FUSE_PROJECT, "fuse project"_Stz);

  m_pCI->vClearRestart();
  while (m_pCI->G_bEachMain(CK_SCAN)) {
    if ((m_pCI->G.p_project = _pCard_Project(m_pCI->pNextChildIf(CK_PROJECT))))
      m_pCI->G.p_project->pSymbol()->Y_vSet(P_Fused);
  }

  m_pCI->vClearRestart();
  while (m_pCI->G_bEachMain(CK_INDEXSCAN)) {
    if ((m_pCI->G.p_project = _pCard_Project(m_pCI->pNextChildIf(CK_PROJECT))))
      m_pCI->G.p_project->pSymbol()->Y_vSet(P_Fused);
  }

  m_pCI->vEndTransform();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
