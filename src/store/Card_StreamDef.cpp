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

  s32 sStreamDef;

  sStreamDef.vSprint("streamDef"_Stz);

  pRibbon->vSprint("%15s"_Stz,sStreamDef.szStz());
  
  if (! pCard_StreamDef->m_pStreamDef)
    return;

  i8 nColumns = pCard_StreamDef->m_pStreamDef->nColumns();
  for (i8 i_Col = 0; i_Col < nColumns; i_Col++) {
    Column* pColumn = pCard_StreamDef->m_pStreamDef->pGetColumn(i_Col);
    
    pRibbon->vSprint("%15s %c"_Stz
                     ,pColumn->szColNm()
                     ,pColumn->cTypeShort());
  }

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
