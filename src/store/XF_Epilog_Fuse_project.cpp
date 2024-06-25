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
    if (! m_pCI->E.p_Op->bEither(CK_o_CROSSJOIN,
                                 CK_o_INDEXSCAN,
                                 CK_o_INDEXJOIN,
                                 CK_o_RRFJOIN,
                                 CK_o_SCAN,
                                 CK_NONE))
      goto L_UPDATE_PRIOR_EMITTED;

    m_pCI->Next.m_pProject = _pCard_Project(m_pCI->pPeekNextIf(CK_o_PROJECT));

    // test for viable next <project>
    if (! m_pCI->Next.m_pProject ||
        m_pCI->Next.m_pProject->pSub_emit()->bIsCompound() ||
        m_pCI->Next.m_pProject->pSub_emit()->bIsLiterals(m_pCI)) {

      goto L_UPDATE_PRIOR_EMITTED;
    }

    m_pCI->E.p_Op->vFuse_project_Viable(m_pCI);
    m_pCI->E.p_deck->vDeleteChild(m_pCI->Next.m_pProject);
    m_pCI->E.p_Op->pSymbol()->Y_vSet(P_FusedProject);

    L_UPDATE_PRIOR_EMITTED:
    m_pCI->vSnapIf_arriving();
  }

  m_pCI->vEndTransform(__LINE__);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
