/*============================================================================*/
/* XF_Prolog_Analyse_table.cpp -- Analyse Table                               */
/*============================================================================*/

#include "XF_Prolog_Analyse_table.hpp"
#include "Table.hpp"
#include "Error.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Prolog_Analyse_table, CN_XF_ANALYSE_TABLE)
__aNewFunction(XF_Prolog_Analyse_table, CN_XF_ANALYSE_TABLE)
__vDeleteFunction(XF_Prolog_Analyse_table, CN_XF_ANALYSE_TABLE)
__aFooterFunction(XF_Prolog_Analyse_table)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Prolog_Analyse_table::XF_Prolog_Analyse_table(CardIter* pCI)
: XF_PROLOG(CN_XF_ANALYSE_TABLE, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_ANALYSE_TABLE, _FOOTER_);

  _checkInitialised_(CN_XF_ANALYSE_TABLE);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_ANALYSE_TABLE, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Prolog_Analyse_table::~XF_Prolog_Analyse_table(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_ANALYSE_TABLE, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Prolog_Analyse_table::vProlog(CardIter* pCI)
{
  _static_

	XF_Prolog_Analyse_table* pXF_table_Analysis = nullptr;

	try {
		pXF_table_Analysis = new XF_Prolog_Analyse_table(pCI);

		pXF_table_Analysis->vXF_table_Analysis(&pCI->m_xSymbol_tableSyn);

		delete pXF_table_Analysis;
	}
	catch (Exception xException) {
		delete pXF_table_Analysis;
	}
	catch (Error xError) {
		delete pXF_table_Analysis;
	}
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Prolog_Analyse_table::vXF_table_Analysis_passThru(CardIter* pCI)
{
  _static_

	XF_Prolog_Analyse_table* pXF_table_Analysis = nullptr;

	try {
    pXF_table_Analysis = new XF_Prolog_Analyse_table(pCI);

    pXF_table_Analysis->vXF_table_Analysis_passThru(&pCI->m_xSymbol_tableSyn);

    delete pXF_table_Analysis;
	}
	catch (Exception xException) {
		delete pXF_table_Analysis;
		throw xException;
	}
	catch (Error xError) {
		delete pXF_table_Analysis;
		throw xError;
	}
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Prolog_Analyse_table::vXF_table_Analysis(Symbol* pSymbol_tableSyn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_ANALYSE_TABLE, _FOOTER_);

  m_pCI->vTransform(__func__, XF_PROLOG_TABLE_ANALYSIS, "add a <tableSyn> to all <table>'s"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Affix Table synonyms To All Table
  m_pCI->vBeginEach();
  while (m_pCI->G_bEachMainWith2Sub(CK_FROM, CK_TABLE, CK_TABLENM))  {
    m_pCI->G.p_table->vSet_Syn(m_pCI, pSymbol_tableSyn);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Produce Table Info Under All Table
  m_pCI->vClearRestart();
  while (m_pCI->G_bEachMainWith2Sub(CK_FROM, CK_TABLE, CK_TABLENM)) {

    Card_TableNm* p_tableNm = m_pCI->G.p_table->pSub_tableNm();
    Table* pTable = Card_Table::pAccess_Table(p_tableNm->szSymbol(SF_NONE));
    m_pCI->G.p_from->pSymbol()->Y_vSet(P_Pointer,pTable);

    i8 jTableSize;
    i8 jRowSize;
    i8 nRowCount;
    Card_Table::vGetSizes(pTable,jTableSize,jRowSize,nRowCount);

    m_pCI->G.p_tablePtn  = Card_TablePtn::pDerive(m_pCI->G.p_table);
    m_pCI->G.p_tableSize = (new Card_TableSize) ->SYM(SF_INT_LIT,jTableSize);
    m_pCI->G.p_rowSize   = (new Card_RowSize)   ->SYM(SF_INT_LIT,jRowSize);
    m_pCI->G.p_rowCount  = (new Card_RowCount)  ->SYM(SF_INT_LIT,nRowCount);

    m_pCI->G.p_tableInfo = new Card_TableInfo;

    m_pCI->G.p_tableInfo->vAppendChild(m_pCI->G.p_tablePtn);
    m_pCI->G.p_tableInfo->vAppendChild(m_pCI->G.p_tableSize);
    m_pCI->G.p_tableInfo->vAppendChild(m_pCI->G.p_rowSize);
    m_pCI->G.p_tableInfo->vAppendChild(m_pCI->G.p_rowCount);

    m_pCI->G.p_table->vAppendChild(m_pCI->G.p_tableInfo);
  }

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Prolog_Analyse_table::vXF_table_Analysis_passThru(Symbol* pSymbol_tableSyn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_ANALYSE_TABLE, _FOOTER_);

  m_pCI->vTransform(__func__, XF_PROLOG_TABLE_ANALYSIS, "add a <tableSyn> to all <table>'s"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Affix Table Syntax To All Table
  Card_Table::vAffixTableSyn_passThru(m_pCI, pSymbol_tableSyn);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Produce Table Partition Under All Table
//  m_pCI->vClearRestart();
//  Card_Table::vProduceTablePtns(m_pCI);

  m_pCI->vEndTransform();
//vValidateBench();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
