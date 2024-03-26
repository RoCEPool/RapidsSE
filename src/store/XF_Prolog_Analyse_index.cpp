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
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* index analysis                                                             */
/*----------------------------------------------------------------------------*/
void XF_Prolog_Analyse_index::vXF_Analyse_index(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_ANALYSE_INDEX, _FOOTER_);

  m_pCI->vTransform(__FILE__, __func__, XF_PROLOG_ANALYSE_INDEX);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Append Index List Under All Table
  Card_Table::vAppendIndexList(m_pCI);

 // m_pCI->PRINTBOARD_dbg();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Affix TblCol To All IdxCol

  m_pCI->vClearRegisters();
  m_pCI->vBeginEach();
  Card_Table::vAffixTableColOrd(m_pCI);

  m_pCI->vEndTransform(__LINE__);
//vValidateBench();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
