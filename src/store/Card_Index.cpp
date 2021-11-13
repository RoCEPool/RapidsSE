/*============================================================================*/
/* Card_Index.cpp -- Index Card                                               */
/*============================================================================*/

#include "Card_Index.hpp"
#include "ShardDef.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Card_Index,CN_CARD_TABLE)
__aNewFunction(Card_Index,CN_CARD_TABLE)
__vDeleteFunction(Card_Index,CN_CARD_TABLE)
__aFooterFunction(Card_Index)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Index::Card_Index(Index* pIndex)
: Card(CN_CARD_TABLE,"Index"_Stz)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_TABLE,_FOOTER_);
      
  m_fpDeleteThis = &Card_Index::vDeleteThis_;
  m_fpBoardPrint = &Card_Index::vBoardPrint_;

  m_pIndex = pIndex;

  _checkInitialised_(CN_CARD_TABLE);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD_TABLE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Index::~Card_Index(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_TABLE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* return table                                                               */
/*----------------------------------------------------------------------------*/
Index* Card_Index::pIndex(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_TABLE,_FOOTER_);
  
  return m_pIndex;
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* print index                                                                */
/*----------------------------------------------------------------------------*/
void Card_Index::vPrintIndex(Ribbon* pRibbon)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_TABLE,_FOOTER_);
  
  s32 sIdxKey;

  pRibbon->vSprint("%15s"_Stz,m_pIndex->pTable()->szTableNm());
  pRibbon->vSprint("%15s"_Stz,m_pIndex->szIndexNm());
  
  IndexDef* pIndexDef = m_pIndex->pIndexDef();
  i8 nColumns = pIndexDef->nColumns();
  for (i8 i_Col = 0; i_Col < nColumns; i_Col++) {
    Column* pColumn = pIndexDef->pGetColumn(i_Col);
    
    sIdxKey.vSprint("[%s]"_Stz,pColumn->szColNm());
    pRibbon->vSprint("%15s"_Stz,sIdxKey.szStz());
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
void Card_Index::vDeleteThis_(Card* pCard)
{
  _static_
  
  delete (Card_Index*)pCard;
}

/*----------------------------------------------------------------------------*/
/* print table                                                                */
/*----------------------------------------------------------------------------*/
void Card_Index::vBoardPrint_(Card* pCard, Ribbon* pRibbon, i8 iDepth)
{
  _static_

  Card_Index* pCardIndex = _pCard_Index(pCard);
  
  pCardIndex->vPrintIndex(pRibbon);

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
