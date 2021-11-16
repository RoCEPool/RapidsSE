/*============================================================================*/
/* Card_IndexJoin.cpp -- Index Join Card                                     */
/*============================================================================*/

#include "Card_IndexJoin.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Card_IndexJoin,CN_CARD_INDEXJOIN)
__aNewFunction(Card_IndexJoin,CN_CARD_INDEXJOIN)
__vDeleteFunction(Card_IndexJoin,CN_CARD_INDEXJOIN)
__aFooterFunction(Card_IndexJoin)

__bIsClass(STORE,Card_IndexJoin,CN_CARD_INDEXJOIN)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_IndexJoin::Card_IndexJoin(Index* pIndex)
: Card(CN_CARD_INDEXJOIN,"IndexJoin"_Stz)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_INDEXJOIN,_FOOTER_);
      
  m_fpDeleteThis = &Card_IndexJoin::vDeleteThis_;
  m_fpBoardPrint = &Card_IndexJoin::vBoardPrint_;

  m_pIndex = pIndex;
  
  _checkInitialised_(CN_CARD_INDEXJOIN);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD_INDEXJOIN,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_IndexJoin::~Card_IndexJoin(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_INDEXJOIN,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get index                                                                  */
/*----------------------------------------------------------------------------*/
Index* Card_IndexJoin::pIndex(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_INDEXJOIN,_FOOTER_);
  
  return m_pIndex;
}

/*----------------------------------------------------------------------------*/
/* set index                                                                  */
/*----------------------------------------------------------------------------*/
void Card_IndexJoin::vIndex(Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_INDEXJOIN,_FOOTER_);
  
  m_pIndex = pIndex;
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* delete                                                                     */
/*----------------------------------------------------------------------------*/
void Card_IndexJoin::vDeleteThis_(Card* pCard)
{
  _static_
  
  delete (Card_IndexJoin*)pCard;
}

/*----------------------------------------------------------------------------*/
/* print index join piece                                                     */
/*----------------------------------------------------------------------------*/
void Card_IndexJoin::vBoardPrint_(Card* pCard, Ribbon* pRibbon, i8 iDepth)
{
  _static_

//  Card_IndexJoin* pCard_IndexJoin = _pCard_IndexJoin(pCard);

//  s32 sIndexJoin;
//  sIndexJoin.vSprint("indexJoin"_Stz);
//  pRibbon->vSprint("%15s"_Stz,sIndexJoin.szStz());
  
//  if (! pCard_IndexJoin->m_pIndex)
//    return;
//
//  pRibbon->vSprint("%15s"_Stz,pCard_IndexJoin->m_pIndex->szIndexNm());
//
//  IndexDef* pIndexDef = pCard_IndexJoin->m_pIndex->pIndexDef();
//  i8 nColumns = pIndexDef->nColumns();
//  for (i8 i_Col = 0; i_Col < nColumns; i_Col++) {
//    Column* pColumn = pIndexDef->pGetColumn(i_Col);
//    pRibbon->vSprint("%15s"_Stz,pColumn->szColNm());
//  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
