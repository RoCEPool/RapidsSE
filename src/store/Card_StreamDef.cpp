/*============================================================================*/
/* Card_StreamDef.cpp -- Stream Definition Card                               */
/*============================================================================*/

#include "Card_StreamDef.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Card_StreamDef,CN_CARD_STREAMDEF)
__aNewFunction(Card_StreamDef,CN_CARD_STREAMDEF)
__vDeleteFunction(Card_StreamDef,CN_CARD_STREAMDEF)
__aFooterFunction(Card_StreamDef)

__bIsClass(STORE,Card_StreamDef,CN_CARD_STREAMDEF)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_StreamDef::Card_StreamDef(StreamDef* pStreamDef)
: Card(CN_CARD_STREAMDEF,"StreamDef"_Stz)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_STREAMDEF,_FOOTER_);
      
  m_fpDeleteThis = &Card_StreamDef::vDeleteThis_;
  m_fpBoardPrint = &Card_StreamDef::vBoardPrint_;
  
  m_pStreamDef = pStreamDef;

  _checkInitialised_(CN_CARD_STREAMDEF);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD_STREAMDEF,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_StreamDef::~Card_StreamDef(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_STREAMDEF,_FOOTER_);

  delete m_pStreamDef;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get stream definition                                                      */
/*----------------------------------------------------------------------------*/
StreamDef* Card_StreamDef::pStreamDef(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_STREAMDEF,_FOOTER_);
  
  return m_pStreamDef;
}

/*----------------------------------------------------------------------------*/
/* set stream definition                                                      */
/*----------------------------------------------------------------------------*/
void Card_StreamDef::vStreamDef(StreamDef* pStreamDef)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_STREAMDEF,_FOOTER_);
  
  m_pStreamDef = pStreamDef;
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
void Card_StreamDef::vDeleteThis_(Card* pCard)
{
  _static_
  
  delete (Card_StreamDef*)pCard;
}

/*----------------------------------------------------------------------------*/
/* print stream def piece                                                     */
/*----------------------------------------------------------------------------*/
void Card_StreamDef::vBoardPrint_(Card* pCard, Ribbon* pRibbon, i8 iDepth)
{
  _static_

  Card_StreamDef* pCard_StreamDef = _pCard_StreamDef(pCard);
  StreamDef* pStreamDef = pCard_StreamDef->m_pStreamDef;
  if (! pStreamDef)
    return;

  s32 sOrder;
  
  i8 nOrders = pStreamDef->nOrders();
  for (i8 i_Ord = 0; i_Ord < nOrders; i_Ord++) {
    Column* pColumn = pStreamDef->pGetOrder(i_Ord);
  
//    sOrder.vSprint("[%s]"_Stz,pColumn->szColNm());
//
//    pRibbon->vSprint("%15s"_Stz
//                     ,sOrder.szStz()
//                     );
    
    pRibbon->vSput(FR_ATOM_SQUARE
                     ,pColumn->szColNm()
                     );
  }

  
  i8 nColumns = pStreamDef->nColumns();
  for (i8 i_Col = 0; i_Col < nColumns; i_Col++) {
    Column* pColumn = pStreamDef->pGetColumn(i_Col);
    
    pRibbon->vSput(FR_SDEF_COLUMN
                     ,pColumn->szColNm()
                     ,pColumn->cTypeShort()
                     );
  }

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
