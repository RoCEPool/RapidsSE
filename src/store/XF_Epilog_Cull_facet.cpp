/*============================================================================*/
/* XF_Epilog_Cull_facet.cpp -- Fuse Project                                 */
/*============================================================================*/

#include "XF_Epilog_Cull_facet.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Epilog_Cull_facet, CN_XF_EPILOG_FUSE_PROJECT)
__aNewFunction(XF_Epilog_Cull_facet, CN_XF_EPILOG_FUSE_PROJECT)
__vDeleteFunction(XF_Epilog_Cull_facet, CN_XF_EPILOG_FUSE_PROJECT)
__aFooterFunction(XF_Epilog_Cull_facet)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Epilog_Cull_facet::XF_Epilog_Cull_facet(CardIter* pCI)
: XF_EPILOG(CN_XF_EPILOG_FUSE_PROJECT, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_FUSE_PROJECT, _FOOTER_);

  _checkInitialised_(CN_XF_EPILOG_FUSE_PROJECT);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_FUSE_PROJECT, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Epilog_Cull_facet::~XF_Epilog_Cull_facet(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_FUSE_PROJECT, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Epilog_Cull_facet::vFuse(CardIter* pCI)
{
  _static_

  XF_Epilog_Cull_facet* pFuse_project = new XF_Epilog_Cull_facet(pCI);

  pFuse_project->vFuse_project();

  delete pFuse_project;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* fuse <OP injecting> <project/emitted> -> <OP/stream/emitted>               */
/*----------------------------------------------------------------------------*/
void XF_Epilog_Cull_facet::vFuse_project(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_FUSE_PROJECT, _FOOTER_);

  m_pCI->vTransform(__func__, XF_EPILOG_FUSE_PROJECT, "fuse project"_Stz);


  m_pCI->vEndTransform(__LINE__);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
