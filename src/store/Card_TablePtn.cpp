/*============================================================================*/
/* Card_TablePtn.cpp -- Table Partition Key                                   */
/*============================================================================*/

#include "Card_TablePtn.hpp"
#include "Card_Table.hpp"
#include "Card_TablePtr.hpp"
#include "PtnDef.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_TablePtn,CK_TABLEPTN)
__aNewFunction(Card_TablePtn,CN_CARD)
__vDeleteFunction(Card_TablePtn,CN_CARD)

__bIsCard(STORE,Card_TablePtn,CK_TABLEPTN)
__pSubFunction_Card(STORE,TablePtn,CK_TABLEPTN)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_TablePtn::Card_TablePtn(void)
: Card(CK_TABLEPTN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_TablePtn::Card_TablePtn(Card_Table* p_table)
: Card(CK_TABLEPTN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

  vAdd_TablePtn(p_table);

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
/*-- private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
void Card_TablePtn::vAdd_TablePtn(Card_Table* p_table)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  char cTableSyn = p_table->cPpy(PPY_TableSyn);

  Card_TablePtr* p_tablePtr = _pSub_TablePtr(p_table);
  Table* pTable = p_tablePtr->pTable();

  PtnDef* pPtnDef = pTable->pPtnDef();
  Column* pPtnColumn = pPtnDef->pGetColumn(0);

  StreamDef* pStreamDef = pTable->pStreamDef();
  Column* pStmColumn = pStreamDef->pFindColumn(pPtnColumn->szColNm());
  i8 iStmColNo = pStmColumn->iColumnNo();
  s32 sTablePtn;
  sTablePtn.vSprint("$$%c.%lld"_Stz, cTableSyn, iStmColNo + 1);

  this->vValue(sTablePtn);
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
