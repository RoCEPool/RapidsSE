/*============================================================================*/
/* Card_Table.cpp -- Table Card                                              */
/*============================================================================*/

#include "Card_Table.hpp"
#include "ShardDef.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Card_Table,CN_CARD_TABLE)
__aNewFunction(Card_Table,CN_CARD_TABLE)
__vDeleteFunction(Card_Table,CN_CARD_TABLE)
__aFooterFunction(Card_Table)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Table::Card_Table(Table* pTable)
: Card(CN_CARD_TABLE,"Table"_Stz)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_TABLE,_FOOTER_);
      
  m_fpDeleteThis = &Card_Table::vDeleteThis_;
  m_fpBoardPrint = &Card_Table::vBoardPrint_;

  m_pTable = pTable;
  m_jTableSize = C_UNKNOWN;

  _checkInitialised_(CN_CARD_TABLE);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD_TABLE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Table::~Card_Table(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_TABLE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* return table                                                               */
/*----------------------------------------------------------------------------*/
Table* Card_Table::pTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_TABLE,_FOOTER_);
  
  return m_pTable;
}

/*----------------------------------------------------------------------------*/
/* return table size                                                          */
/*----------------------------------------------------------------------------*/
i8 Card_Table::jTableSize(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_TABLE,_FOOTER_);
  
  i8 jSize = m_pTable->pStreamDef()->jTableSize();
  
  return jSize;
}

/*----------------------------------------------------------------------------*/
/* print table                                                                */
/*----------------------------------------------------------------------------*/
void Card_Table::vRemaindersPrint(Ribbon* pRibbon)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_TABLE,_FOOTER_);
  
  vPrintTable(pRibbon);
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* print table                                                                */
/*----------------------------------------------------------------------------*/
void Card_Table::vPrintTable(Ribbon* pRibbon)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_TABLE,_FOOTER_);
  
  pRibbon->vSprint("%15s"_Stz,m_pTable->szTableNm());
}

/*----------------------------------------------------------------------------*/
/* print partition key                                                        */
/*----------------------------------------------------------------------------*/
void Card_Table::vPrintPartition(Ribbon* pRibbon)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_TABLE,_FOOTER_);

  s32 sPtnKey;
  
  ShardDef* pShardDef = m_pTable->pShardDef();
  Column* pColumn = pShardDef->pGetColumn(0);

  sPtnKey.vSprint("/%s/"_Stz,pColumn->szColNm());
  pRibbon->vSprint("%15s"_Stz,sPtnKey.szStz());
}

/*----------------------------------------------------------------------------*/
/* print primary key                                                          */
/*----------------------------------------------------------------------------*/
void Card_Table::vPrintPrimary(Ribbon* pRibbon)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_TABLE,_FOOTER_);

  s32 sPmyKey;
  
  IdxVec*   pIdxVec   = m_pTable->pIdxVec();
  Index*    pIndex    = pIdxVec->pGet(0);
  IndexDef* pIndexDef = pIndex->pIndexDef();
  Column*   pColumn   = pIndexDef->pGetColumn(0);

  sPmyKey.vSprint("[%s]"_Stz,pColumn->szColNm());
  pRibbon->vSprint("%15s"_Stz,sPmyKey.szStz());
}

/*----------------------------------------------------------------------------*/
/* print table size                                                           */
/*----------------------------------------------------------------------------*/
void Card_Table::vPrintSize(Ribbon* pRibbon)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_TABLE,_FOOTER_);
  
  StreamDef* pStreamDef = m_pTable->pStreamDef();

  pRibbon->vSprint("%15lld"_Stz,pStreamDef->jTableSize());
}

/*----------------------------------------------------------------------------*/
/* print columns                                                              */
/*----------------------------------------------------------------------------*/
void Card_Table::vPrintColumns(Ribbon* pRibbon)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_TABLE,_FOOTER_);
  
  StreamDef* pStreamDef = m_pTable->pStreamDef();
  
  i8 nColumns = pStreamDef->nColumns();
  for (i8 i_Col = 0; i_Col < nColumns; i_Col++) {
    Column* pColumn = pStreamDef->pGetColumn(i_Col);
    
    pRibbon->vSprint("%15s %c"_Stz,pColumn->szColNm(),pColumn->cTypeShort());
  }
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* delete                                                                    */
/*----------------------------------------------------------------------------*/
void Card_Table::vDeleteThis_(Card* pCard)
{
  _static_
  
  delete (Card_Table*)pCard;
}

/*----------------------------------------------------------------------------*/
/* print table                                                                */
/*----------------------------------------------------------------------------*/
void Card_Table::vBoardPrint_(Card* pCard, Ribbon* pRibbon, i8 iDepth)
{
  _static_

  Card_Table* pCardTable = _pCard_Table(pCard);

  pCardTable->vPrintTable(pRibbon);
  
  if (pRibbon->iArea())
    return;
  
  pCardTable->vPrintPartition(pRibbon);
  pCardTable->vPrintSize(pRibbon);
  pCardTable->vPrintPrimary(pRibbon);
  //pCardTable->vPrintColumns(pRibbon);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
