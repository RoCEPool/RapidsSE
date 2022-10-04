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
void Tabular::vAdd_TablePtn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABULAR,_FOOTER_);

  char cTableSyn = 'a';

  m_pBoard = m_pBench->pGetBoard(BK_QUERY);

  BoardIter xBI(m_pBoard);

  while ( (m_pMainCard = xBI.pEachMain()) ) {
    if (! (m_p_table = _pGetSub_Table(m_pMainCard)) )
      continue;

    Card_TablePtn* p_tablePtn = new Card_TablePtn(m_p_table);
    m_p_table->vAppend(p_tablePtn);
  }

  if (m_bInteractive)
    m_pBoard->vPrintBoard("Tabular::vAdd_TablePtn"_Stz);
}


/*----------------------------------------------------------------------------*/
/* add table synonyms <tableSyn> to all <table>                               */
/*----------------------------------------------------------------------------*/
void Tabular::vAdd_TableSyn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABULAR,_FOOTER_);

  char cTableSyn = 'a';

  m_pBoard = m_pBench->pGetBoard(BK_QUERY);

  BoardIter xBI(m_pBoard);

  while ( (m_pMainCard = xBI.pEachMain()) ) {
    if (! (m_p_table = _pGetSub_Table(m_pMainCard)) )
      continue;

    Card_TablePtr* p_tablePtr = _pGetSub_TablePtr(m_p_table);
    Table* pTable = p_tablePtr->pTable();

    Card_TableSyn* p_tableSyn = new Card_TableSyn;

    if (pTable->bIsReplicated())
      p_tableSyn->vTableSyn(cTableSyn+'A'-'a');
    else
      p_tableSyn->vTableSyn(cTableSyn);

    m_p_table->vAppend(p_tableSyn);

    cTableSyn++;
  }

  if (m_bInteractive)
    m_pBoard->vPrintBoard("Tabular::vAdd_TableSyn"_Stz);
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
