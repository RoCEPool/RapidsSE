/*============================================================================*/
/* Card_Index.cpp -- Index Card                                               */
/*============================================================================*/

#include "Card_Index.hpp"
#include "ShardDef.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Card_Index,CN_CARD_INDEX)
__aNewFunction(Card_Index,CN_CARD_INDEX)
__vDeleteFunction(Card_Index,CN_CARD_INDEX)
__aFooterFunction(Card_Index)

__bIsClass(STORE,Card_Index,CN_CARD_INDEX)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Index::Card_Index(Index* pIndex)
: Card(CK_INDEX,CN_CARD_INDEX,"Index"_Stz)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_INDEX,_FOOTER_);
      
  m_fpDeleteThis = &Card_Index::vDeleteThis_;
  m_fpBoardPrint = &Card_Index::vBoardPrint_;

  m_pIndex = pIndex;

  _checkInitialised_(CN_CARD_INDEX);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD_INDEX,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Index::~Card_Index(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_INDEX,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* return table                                                               */
/*----------------------------------------------------------------------------*/
Index* Card_Index::pIndex(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_INDEX,_FOOTER_);
  
  return m_pIndex;
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* print index                                                                */
/*----------------------------------------------------------------------------*/
void Card_Index::vPrintIndex(Ribbon* pRibbon)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_INDEX,_FOOTER_);
  

  pRibbon->vSput(FR_NAME,m_pIndex->szIndexNm());
  
  IndexDef* pIndexDef = m_pIndex->pIndexDef();
  pRibbon->vSput(FR_TEXT
                 ,pIndexDef->bIsUnique()
                    ? "unique"_Stz : "not unique"_Stz
                 );
  
  i8 nColumns = pIndexDef->nColumns();
  for (i8 i_Col = 0; i_Col < nColumns; i_Col++) {
    Column* pColumn = pIndexDef->pGetColumn(i_Col);
    
    pRibbon->vSput(FR_NAME,pColumn->szColNm());
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
