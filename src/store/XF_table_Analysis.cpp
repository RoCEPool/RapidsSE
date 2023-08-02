/*============================================================================*/
/* XF_table_Analysis.cpp -- Table Prior                                    */
/*============================================================================*/

#include "XF_table_Analysis.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_table_Analysis, CN_XF_TABLE_ANALYSIS)
__aNewFunction(XF_table_Analysis, CN_XF_TABLE_ANALYSIS)
__vDeleteFunction(XF_table_Analysis, CN_XF_TABLE_ANALYSIS)
__aFooterFunction(XF_table_Analysis)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_table_Analysis::XF_table_Analysis(CardIter* pCI, char* acTableSyn)
: XF_TRANSFORM(CN_XF_TABLE_ANALYSIS, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_TABLE_ANALYSIS, _FOOTER_);

  _checkInitialised_(CN_XF_TABLE_ANALYSIS);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_TABLE_ANALYSIS, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_table_Analysis::~XF_table_Analysis(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_TABLE_ANALYSIS, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_table_Analysis::vXF_table_Analysis(CardIter* pCI, char* acTableSyn, Symbol* pSymbol_tableSyn)
{
  _static_

  XF_table_Analysis* pXF_table_Analysis = new XF_table_Analysis(pCI,acTableSyn);

  pXF_table_Analysis->vXF_table_Analysis(pSymbol_tableSyn);

  delete pXF_table_Analysis;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_table_Analysis::vXF_table_Analysis_passThru(CardIter* pCI, char* acTableSyn, Symbol* pSymbol_tableSyn)
{
  _static_

  XF_table_Analysis* pXF_table_Analysis = new XF_table_Analysis(pCI,acTableSyn);

  pXF_table_Analysis->vXF_table_Analysis_passThru(pSymbol_tableSyn);

  delete pXF_table_Analysis;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_table_Analysis::vXF_table_Analysis(Symbol* pSymbol_tableSyn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_TABLE_ANALYSIS, _FOOTER_);

  m_pCI->vTransform(__func__, XF_TABLE_ANALYSIS, "add a <tableSyn> to all <table>'s"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Affix Table Syntax To All Table
  Card_Table::vAffixTableSyn(m_pCI, pSymbol_tableSyn);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Produce Table Partition Under All Table
  m_pCI->vClearRestart();
  Card_Table::vProduceTablePtns(m_pCI);

  m_pCI->vEndTransform();

  vValidate();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_table_Analysis::vXF_table_Analysis_passThru(Symbol* pSymbol_tableSyn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_TABLE_ANALYSIS, _FOOTER_);

  m_pCI->vTransform(__func__, XF_TABLE_ANALYSIS, "add a <tableSyn> to all <table>'s"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Affix Table Syntax To All Table
  Card_Table::vAffixTableSyn_passThru(m_pCI, pSymbol_tableSyn);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Produce Table Partition Under All Table
//  m_pCI->vClearRestart();
//  Card_Table::vProduceTablePtns(m_pCI);

  m_pCI->vEndTransform();

  vValidate();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
