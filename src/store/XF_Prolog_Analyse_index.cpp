/*============================================================================*/
/* XF_Prolog_Analyse_index.cpp -- As _review                                              */
/*============================================================================*/

#include "XF_Prolog_Analyse_index.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Prolog_Analyse_index, CN_XF_PROLOG_ANALYSE_INDEX)
__aNewFunction(XF_Prolog_Analyse_index, CN_XF_PROLOG_ANALYSE_INDEX)
__vDeleteFunction(XF_Prolog_Analyse_index, CN_XF_PROLOG_ANALYSE_INDEX)
__aFooterFunction(XF_Prolog_Analyse_index)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Prolog_Analyse_index::XF_Prolog_Analyse_index(CardIter* pCI)
: XF_PROLOG(CN_XF_PROLOG_ANALYSE_INDEX, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_ANALYSE_INDEX, _FOOTER_);

  _checkInitialised_(CN_XF_PROLOG_ANALYSE_INDEX);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_ANALYSE_INDEX, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Prolog_Analyse_index::~XF_Prolog_Analyse_index(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_ANALYSE_INDEX, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Prolog_Analyse_index::vAnalyse(CardIter* pCI)
{
  _static_

  XF_Prolog_Analyse_index* pXF_index_Analysis = new XF_Prolog_Analyse_index(pCI);

  pXF_index_Analysis->vXF_Analyse_index();

  delete pXF_index_Analysis;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Prolog_Analyse_index::vXF_index_Analysis_passThru(CardIter* pCI)
{
  _static_

  XF_Prolog_Analyse_index* pXF_index_Analysis = new XF_Prolog_Analyse_index(pCI);

  pXF_index_Analysis->vXF_Analyse_index_passThru();

  delete pXF_index_Analysis;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* index analysis                                                             */
/*----------------------------------------------------------------------------*/
void XF_Prolog_Analyse_index::vXF_Analyse_index(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_ANALYSE_INDEX, _FOOTER_);

  m_pCI->vTransform(__func__, XF_PROLOG_ANALYSE_INDEX, "index analysis"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Append Index List Under All Table
  Card_Table::vAppendIndexList(m_pCI);

 // m_pCI->vPrintBoard();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Affix TblCol To All IdxCol

  m_pCI->vClearRestart();
  Card_Table::vAffixTableColOrd(m_pCI);

  m_pCI->vEndTransform();
//vValidateBench();
}

/*----------------------------------------------------------------------------*/
/* index analysis                                                             */
/*----------------------------------------------------------------------------*/
void XF_Prolog_Analyse_index::vXF_Analyse_index_passThru(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_ANALYSE_INDEX, _FOOTER_);

  m_pCI->vTransform(__func__, XF_PROLOG_ANALYSE_INDEX, "index analysis"_Stz);

  while (m_pCI->G_bEachMain(CK_INDEXSCAN)) {
    m_pCI->G.p_indexScan->vUpdate_emitted(m_pCI);

    m_pCI->G.p_index = m_pCI->G.p_indexScan->pSub_index();
    m_pCI->G.p_index->pSymbol()->Y_vSet(P_ChosenIndex);
  }

  m_pCI->vBeginEach();
  while (m_pCI->G_bEachMain(CK_INDEXJOIN)) {
    m_pCI->G.p_indexJoin->vUpdate_emitted(m_pCI);

    m_pCI->G.p_index = m_pCI->G.p_indexJoin->pSub_index();
    m_pCI->G.p_index->pSymbol()->Y_vSet(P_ChosenIndex);

    m_pCI->G.p_bound = m_pCI->G.p_index->pSub_bound();
    m_pCI->G.p_bound->pSymbol()->Y_vSet(P_ChosenBound);
  }

  m_pCI->vBeginEach();
  while (m_pCI->G_bEachMain(CK_KEYEXISTS)) {
    m_pCI->G.p_keyExists->vUpdate_emitted(m_pCI);

    m_pCI->G.p_index = m_pCI->G.p_keyExists->pSub_index();
    m_pCI->G.p_index->pSymbol()->Y_vSet(P_ChosenIndex);
  }

  m_pCI->vBeginEach();
  while (m_pCI->G_bEachMain(CK_KEYNOTEXISTS)) {
    m_pCI->G.p_keyNotExists->vUpdate_emitted(m_pCI);

    m_pCI->G.p_index = m_pCI->G.p_keyNotExists->pSub_index();
    m_pCI->G.p_index->pSymbol()->Y_vSet(P_ChosenIndex);
  }

  m_pCI->vEndTransform();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
