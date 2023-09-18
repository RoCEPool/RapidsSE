/*============================================================================*/
/* XF_Epilog_Collapse_op.cpp -- Column Mapping                                    */
/*============================================================================*/

#include "XF_Epilog_Collapse_op.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Epilog_Collapse_op, CN_XF_COLLAPSE_OP)
__aNewFunction(XF_Epilog_Collapse_op, CN_XF_COLLAPSE_OP)
__vDeleteFunction(XF_Epilog_Collapse_op, CN_XF_COLLAPSE_OP)
__aFooterFunction(XF_Epilog_Collapse_op)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Epilog_Collapse_op::XF_Epilog_Collapse_op(CardIter* pCI)
: XF_EPILOG(CN_XF_COLLAPSE_OP, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_COLLAPSE_OP, _FOOTER_);

  _checkInitialised_(CN_XF_COLLAPSE_OP);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_COLLAPSE_OP, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Epilog_Collapse_op::~XF_Epilog_Collapse_op(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_COLLAPSE_OP, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Epilog_Collapse_op::vEpilog(CardIter* pCI)
{
  _static_

  XF_Epilog_Collapse_op* pEpilog_Collapse_op = new XF_Epilog_Collapse_op(pCI);

  pEpilog_Collapse_op->vCollapse_op();

  delete pEpilog_Collapse_op;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* mark collapsable ops                                                       */
/*----------------------------------------------------------------------------*/
void XF_Epilog_Collapse_op::vCollapse_op(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_COLLAPSE_OP, _FOOTER_);

  m_pCI->vTransform(__func__, XF_EPILOG_COLLAPSE_OP, "collapse op"_Stz);

  m_pCI->vClearRestart();
  while (m_pCI->G_bEachMain(CK_SCAN)) {
    if ((m_pCI->G.p_project = _pCard_Project(m_pCI->pNextChildIf(CK_PROJECT))))
      m_pCI->G.p_project->pSymbol()->Y_vSet(P_Collapsed);
  }

  m_pCI->vClearRestart();
  while (m_pCI->G_bEachMain(CK_INDEXSCAN)) {
    if ((m_pCI->G.p_project = _pCard_Project(m_pCI->pNextChildIf(CK_PROJECT))))
      m_pCI->G.p_project->pSymbol()->Y_vSet(P_Collapsed);
  }

  m_pCI->vEndTransform();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
