/*============================================================================*/
/* XF_Prolog_index_Analysis.cpp -- As _review                                              */
/*============================================================================*/

#include "XF_Prolog_index_Analysis.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Prolog_index_Analysis, CN_XF_INDEX_ANALYSIS)
__aNewFunction(XF_Prolog_index_Analysis, CN_XF_INDEX_ANALYSIS)
__vDeleteFunction(XF_Prolog_index_Analysis, CN_XF_INDEX_ANALYSIS)
__aFooterFunction(XF_Prolog_index_Analysis)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Prolog_index_Analysis::XF_Prolog_index_Analysis(CardIter* pCI)
: XF_PROLOG(CN_XF_INDEX_ANALYSIS, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_INDEX_ANALYSIS, _FOOTER_);

  _checkInitialised_(CN_XF_INDEX_ANALYSIS);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_INDEX_ANALYSIS, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Prolog_index_Analysis::~XF_Prolog_index_Analysis(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_INDEX_ANALYSIS, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Prolog_index_Analysis::vProlog(CardIter* pCI)
{
  _static_

  XF_Prolog_index_Analysis* pXF_index_Analysis = new XF_Prolog_index_Analysis(pCI);

  pXF_index_Analysis->vXF_index_Analysis();

  delete pXF_index_Analysis;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Prolog_index_Analysis::vXF_index_Analysis_passThru(CardIter* pCI)
{
  _static_

  XF_Prolog_index_Analysis* pXF_index_Analysis = new XF_Prolog_index_Analysis(pCI);

  pXF_index_Analysis->vXF_index_Analysis_passThru();

  delete pXF_index_Analysis;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* index analysis                                                             */
/*----------------------------------------------------------------------------*/
void XF_Prolog_index_Analysis::vXF_index_Analysis(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_INDEX_ANALYSIS, _FOOTER_);

  m_pCI->vTransform(__func__, XF_PROLOG_INDEX_ANALYSIS, "index analysis"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Append Index List Under All Table
  Card_Table::vAppendIndexList(m_pCI);

 // m_pCI->vPrintBoard();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Affix TblColOdn To All IdxCol

  m_pCI->vClearRestart();
  Card_Table::vAffixTableColOrd(m_pCI);

  m_pCI->vEndTransform();
//vValidateBench();
}

/*----------------------------------------------------------------------------*/
/* index analysis                                                             */
/*----------------------------------------------------------------------------*/
void XF_Prolog_index_Analysis::vXF_index_Analysis_passThru(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_INDEX_ANALYSIS, _FOOTER_);

  m_pCI->vTransform(__func__, XF_PROLOG_INDEX_ANALYSIS, "index analysis"_Stz);

  while (m_pCI->G_bEachMain(CK_INDEXSCAN)) {
    m_pCI->G.p_indexScan->vUpdate_projection(m_pCI);

    m_pCI->G.p_index = m_pCI->G.p_indexScan->pSub_index();
    m_pCI->G.p_index->pSymbol()->Y_vSet(P_ChosenIndex);
  }

  m_pCI->vBeginEach();
  while (m_pCI->G_bEachMain(CK_INDEXJOIN)) {
    m_pCI->G.p_indexJoin->vUpdate_projection(m_pCI);

    m_pCI->G.p_index = m_pCI->G.p_indexJoin->pSub_index();
    m_pCI->G.p_index->pSymbol()->Y_vSet(P_ChosenIndex);

    m_pCI->G.p_bound = m_pCI->G.p_index->pSub_bound();
    m_pCI->G.p_bound->pSymbol()->Y_vSet(P_ChosenBound);
  }

  m_pCI->vBeginEach();
  while (m_pCI->G_bEachMain(CK_KEYEXISTS)) {
    m_pCI->G.p_keyExists->vUpdate_projection(m_pCI);

    m_pCI->G.p_index = m_pCI->G.p_keyExists->pSub_index();
    m_pCI->G.p_index->pSymbol()->Y_vSet(P_ChosenIndex);
  }

  m_pCI->vBeginEach();
  while (m_pCI->G_bEachMain(CK_KEYNOTEXISTS)) {
    m_pCI->G.p_keyNotExists->vUpdate_projection(m_pCI);

    m_pCI->G.p_index = m_pCI->G.p_keyNotExists->pSub_index();
    m_pCI->G.p_index->pSymbol()->Y_vSet(P_ChosenIndex);
  }

  m_pCI->vEndTransform();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
