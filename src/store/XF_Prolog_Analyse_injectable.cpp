/*============================================================================*/
/* XF_projection_Analysis.cpp -- Analyse Injected                             */
/*============================================================================*/

#include "XF_Prolog_Analyse_injectable.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Prolog_Analyse_injectable, CN_XF_PROLOG_ANALYSE_INJECTABLE)
__aNewFunction(XF_Prolog_Analyse_injectable, CN_XF_PROLOG_ANALYSE_INJECTABLE)
__vDeleteFunction(XF_Prolog_Analyse_injectable, CN_XF_PROLOG_ANALYSE_INJECTABLE)
__aFooterFunction(XF_Prolog_Analyse_injectable)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Prolog_Analyse_injectable::XF_Prolog_Analyse_injectable(CardIter* pCI)
: XF_PROLOG(CN_XF_PROLOG_ANALYSE_INJECTABLE, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_ANALYSE_INJECTABLE, _FOOTER_);

  _checkInitialised_(CN_XF_PROLOG_ANALYSE_INJECTABLE);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_ANALYSE_INJECTABLE, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Prolog_Analyse_injectable::~XF_Prolog_Analyse_injectable(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_ANALYSE_INJECTABLE, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Prolog_Analyse_injectable::vAnalyse(CardIter* pCI)
{
  _static_

  XF_Prolog_Analyse_injectable* pXF_projection_Analysis = new XF_Prolog_Analyse_injectable(pCI);

  pXF_projection_Analysis->vAnalyse_injectable();

  delete pXF_projection_Analysis;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Prolog_Analyse_injectable::vAnalyse_injected_passThru(CardIter* pCI)
{
  _static_

  XF_Prolog_Analyse_injectable* pXF_projection_Analysis = new XF_Prolog_Analyse_injectable(pCI);

  pXF_projection_Analysis->vAnalyse_injectable_passThru();

  delete pXF_projection_Analysis;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* projection analysis                                                        */
/*----------------------------------------------------------------------------*/
void XF_Prolog_Analyse_injectable::vAnalyse_injectable(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_ANALYSE_INJECTABLE, _FOOTER_);

  m_pCI->vTransform(__FILE__, __func__, XF_PROLOG_ANALYSE_INJECTABLE);

  m_pCI->vBeginEach();

  while (m_pCI->bEachOp(CK_o_FROM, CK_p_TABLE)) {
    m_pCI->E.p_from->vAnalyse_injectable(m_pCI);
  }

  m_pCI->vEndTransform(__LINE__);
}

/*----------------------------------------------------------------------------*/
/* projection analysis                                                        */
/*----------------------------------------------------------------------------*/
void XF_Prolog_Analyse_injectable::vAnalyse_injectable_passThru(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_ANALYSE_INJECTABLE, _FOOTER_);

  m_pCI->vTransform(__FILE__, __func__, XF_PROLOG_ANALYSE_INJECTABLE);

  m_pCI->vBeginEach();
  while (m_pCI->bEachOp(CK_o_SCAN, CK_p_TABLE)) {
    m_pCI->E.p_scan->vAnalyse_injectable(m_pCI);
  }

  m_pCI->vBeginEach();
  while (m_pCI->bEachOp(CK_o_INDEXSCAN, CK_p_TABLE)) {
    m_pCI->E.p_indexScan->vAnalyse_injectable(m_pCI);
  }

  m_pCI->vBeginEach();
  while (m_pCI->bEachOp(CK_o_INDEXJOIN, CK_p_TABLE)) {
    m_pCI->E.p_indexJoin->vAnalyse_injectable(m_pCI);
  }

  m_pCI->vBeginEach();
  while (m_pCI->bEachOp(CK_o_KEYEXISTS, CK_p_TABLE)) {
    m_pCI->E.p_keyExists->vAnalyse_injectable(m_pCI);
  }

  m_pCI->vBeginEach();
  while (m_pCI->bEachOp(CK_o_KEYNOTEXISTS, CK_p_TABLE)) {
    m_pCI->E.p_keyNotExists->vAnalyse_injectable(m_pCI);
  }

  m_pCI->vEndTransform(__LINE__);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
