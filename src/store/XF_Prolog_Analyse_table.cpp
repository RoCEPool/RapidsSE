/*============================================================================*/
/* XF_Prolog_Analyse_table.cpp -- Analyse Table                               */
/*============================================================================*/

#include "XF_Prolog_Analyse_table.hpp"
#include "Table.hpp"
#include "Error.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Prolog_Analyse_table, CN_XF_PROLOG_ANALYSE_TABLE)
__aNewFunction(XF_Prolog_Analyse_table, CN_XF_PROLOG_ANALYSE_TABLE)
__vDeleteFunction(XF_Prolog_Analyse_table, CN_XF_PROLOG_ANALYSE_TABLE)
__aFooterFunction(XF_Prolog_Analyse_table)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Prolog_Analyse_table::XF_Prolog_Analyse_table(CardIter* pCI)
: XF_PROLOG(CN_XF_PROLOG_ANALYSE_TABLE, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_ANALYSE_TABLE, _FOOTER_);

  _checkInitialised_(CN_XF_PROLOG_ANALYSE_TABLE);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_ANALYSE_TABLE, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Prolog_Analyse_table::~XF_Prolog_Analyse_table(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_ANALYSE_TABLE, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Prolog_Analyse_table::vAnalyse(CardIter* pCI)
{
  _static_

	XF_Prolog_Analyse_table* pXF_table_Analysis = nullptr;

	try {
		pXF_table_Analysis = new XF_Prolog_Analyse_table(pCI);

    pXF_table_Analysis->vXF_Analyse_table(&pCI->m_xSymbol_tableSyn);

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
void XF_Prolog_Analyse_table::vXF_Analyse_table_passThru(CardIter* pCI)
{
  _static_

	XF_Prolog_Analyse_table* pXF_table_Analysis = nullptr;

	try {
    pXF_table_Analysis = new XF_Prolog_Analyse_table(pCI);

    pXF_table_Analysis->vXF_Analyse_table_passThru(&pCI->m_xSymbol_tableSyn);

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
void XF_Prolog_Analyse_table::vXF_Analyse_table(Symbol* pSymbol_tableSyn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_ANALYSE_TABLE, _FOOTER_);

  m_pCI->vTransform(__FILE__, __func__, XF_PROLOG_ANALYSE_TABLE);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Affix Table synonyms To All Table
  m_pCI->vBeginEach();
  while (m_pCI->bEachOp(CK_o_FROM, CK_s_TABLE, CK_t_TABLENM))  {
    m_pCI->E.p_table->vSet_Syn(pSymbol_tableSyn);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Produce Table Info Under All Table
  m_pCI->vClearRegisters();
  m_pCI->vBeginEach();
  while (m_pCI->bEachOp(CK_o_FROM, CK_s_TABLE, CK_t_TABLENM)) {

    Card_TableNm* p_tableNm = m_pCI->E.p_table->pSub_tableNm();
    Table* pTable = Card_Table::pAccess_Table(p_tableNm->szFacet(SF_TBL_NM));
    m_pCI->E.p_table->pSymbol()->Y_vSet(P_Pointer, pTable);

//  if (pTable->bAltFetch)
    m_pCI->E.p_table->pSymbol()->Y_vSet(P_AltFetch,"<tba>"_Stz);

    i8 jTableSize;
    i8 jRowSize;
    i8 nRowCount;
    Card_Table::vGetSizes(pTable,jTableSize,jRowSize,nRowCount);

    m_pCI->E.p_tablePtn  = Card_TablePtn::pDerive(m_pCI->E.p_table);
    m_pCI->E.p_tableSize = (new Card_TableSize) ->SYM(SF_INT_LIT, jTableSize);
    m_pCI->E.p_rowSize   = (new Card_RowSize)   ->SYM(SF_INT_LIT, jRowSize);
    m_pCI->E.p_rowCount  = (new Card_RowCount)  ->SYM(SF_INT_LIT, nRowCount);

#ifdef Z_NO_TABLEINFO
    m_pCI->E.p_table->vAppendChild(m_pCI->E.p_tablePtn);
    m_pCI->E.p_table->vAppendChild(m_pCI->E.p_tableSize);
    m_pCI->E.p_table->vAppendChild(m_pCI->E.p_rowSize);
    m_pCI->E.p_table->vAppendChild(m_pCI->E.p_rowCount);
#else
    m_pCI->E.p_tableInfo = new Card_TableInfo;

    m_pCI->E.p_tableInfo->vAppendChild(m_pCI->E.p_tablePtn);
    m_pCI->E.p_tableInfo->vAppendChild(m_pCI->E.p_tableSize);
    m_pCI->E.p_tableInfo->vAppendChild(m_pCI->E.p_rowSize);
    m_pCI->E.p_tableInfo->vAppendChild(m_pCI->E.p_rowCount);

    m_pCI->E.p_table->vAppendChild(m_pCI->E.p_tableInfo);

#endif
  }

  m_pCI->vEndTransform(__LINE__);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Prolog_Analyse_table::vXF_Analyse_table_passThru(Symbol* pSymbol_tableSyn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_ANALYSE_TABLE, _FOOTER_);

  m_pCI->vTransform(__FILE__, __func__, XF_PROLOG_ANALYSE_TABLE);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Affix Table Syntax To All Table
  Card_Table::vAffixTableSyn_passThru(m_pCI, pSymbol_tableSyn);

  m_pCI->vClearRegisters();
  m_pCI->vBeginEach();
  while (m_pCI->bEachOp_bySub(CK_s_TABLE)) {

    Card_TableNm *p_tableNm = m_pCI->E.p_table->pSub_tableNm();
    Table *pTable = Card_Table::pAccess_Table(p_tableNm->szFacet(SF_TBL_NM));
    m_pCI->E.p_table->pSymbol()->Y_vSet(P_Pointer, pTable);

//  if (pTable->bAltFetch)
    m_pCI->E.p_table->pSymbol()->Y_vSet(P_AltFetch,"<tba>"_Stz);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Produce Table Partition Under All Table
//  m_pCI->vClearRegisters();
  m_pCI->vBeginEach();
//  Card_Table::vProduceTablePtns(m_pCI);

  m_pCI->vEndTransform(__LINE__);
//vValidateBench();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
