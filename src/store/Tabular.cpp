/*============================================================================*/
/* Tabular.cpp --                                                           */
/*============================================================================*/

#include "Tabular.hpp"
#include "Table.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Tabular,CN_TABULAR)
__aNewFunction(Tabular,CN_TABULAR)
__vDeleteFunction(Tabular,CN_TABULAR)
__aFooterFunction(Tabular)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Tabular::Tabular(Bench* pBench)
: TransformerTool(CN_TABULAR, pBench)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABULAR,_FOOTER_);

  _checkInitialised_(CN_TABULAR);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_TABULAR,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Tabular::~Tabular(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABULAR,_FOOTER_);

}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add table partitions <tablePtn> to all <table>                             */
/*----------------------------------------------------------------------------*/
void Tabular::vAdd_tablePtn_To_table(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABULAR,_FOOTER_);

  vTransformation(OT_vAdd_tablePtn_To_table);
  vExplain("Blah"_Stz);

  char cTableSyn = 'a';

  m_pBoard = m_pBench->pGetBoard(BK_QUERY);

  BoardIter xBI(m_pBoard);

  while ( (m_pMainCard = xBI.pEachMain()) ) {
    if (! (m_p_table = _pSub_Table(m_pMainCard)) )
      continue;

    Card_TablePtn* p_tablePtn = new Card_TablePtn(m_p_table);
    m_p_table->vAppend(p_tablePtn);
  }

  vPrintBoard();
}


/*----------------------------------------------------------------------------*/
/* add table synonyms <tableSyn> to all <table>                               */
/*----------------------------------------------------------------------------*/
void Tabular::vAdd_tableSyn_To_table(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABULAR,_FOOTER_);

  vTransformation(OT_vAdd_tableSyn_To_table);
  vExplain("Blah"_Stz);

  char cTableSyn = 'a';

  BoardIter xBI(m_pBoard);

  while ( (m_pMainCard = xBI.pEachMain()) ) {
    if (! (m_p_table = _pSub_Table(m_pMainCard)) )
      continue;

    m_p_tablePtr = _pSub_TablePtr(m_p_table);
    m_pTable = m_p_tablePtr->pTable();

    m_p_tableSyn = new Card_TableSyn;

    if (m_pTable->bIsReplicated())
      m_p_tableSyn->vTableSyn(cTableSyn+'A'-'a');
    else
      m_p_tableSyn->vTableSyn(cTableSyn);
    cTableSyn++;

    m_p_table->vAppend(m_p_tableSyn);
  }

  vPrintBoard();
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
