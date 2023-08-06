/*============================================================================*/
/* Card_TablePtn.cpp -- Table Partition Key                                   */
/*============================================================================*/

#include "Card_TablePtn.hpp"
#include "Card_Table.hpp"

#include "Table.hpp"
#include "PtnDef.hpp"
#include "Card_TableNm.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_TablePtn,CK_TABLEPTN)
__aNewFunction(Card_TablePtn,CN_CARD)
__vDeleteFunction(Card_TablePtn,CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_TablePtn::Card_TablePtn(void)
: Card_symbol(CK_TABLEPTN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_TablePtn::~Card_TablePtn(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* produce <tablePtn> of table                                                */
/*----------------------------------------------------------------------------*/
Card_TablePtn* Card_TablePtn::pDerive(Card_Table* p_table)
{
  _static_

  Card_TableNm* p_tableNm = p_table->pSub_tableNm();
  Table* pTable = Card_Table::pAccess_Table(p_tableNm->szSymbol());
  char cTableSyn = p_table->cSymbol();

  PtnDef* pPtnDef = pTable->pPtnDef();
  Column* pPtnColumn = pPtnDef->pGetColumn(0);

  StreamDef* pStreamDef = pTable->pStreamDef();
  Column* pStmColumn = pStreamDef->pFindColumn(pPtnColumn->szColNm());
  i8 iStmColNo = pStmColumn->iColumnNo();

  Card_TablePtn* p_tablePtn = new Card_TablePtn;
  Symbol xSymbol_tablePtn;
  xSymbol_tablePtn.vSet_Col_Tc(cTableSyn, iStmColNo + 1);
  p_tablePtn->pSymbol()->vUpdate(&xSymbol_tablePtn);

  return p_tablePtn;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_TablePtn::bColumnMatchesPtn(Symbol* p_Y_tblPtn, Symbol* p_Y_tblCol)
{
  _static_

  if (p_Y_tblCol->bSame(p_Y_tblPtn))
    return true;

  return false;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
