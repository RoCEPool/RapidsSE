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
Card_StreamDef::Card_StreamDef(eCK eKind, StreamDef* pStreamDef)
: Card(eKind,CN_CARD_STREAMDEF,"StreamDef"_Stz,"pStreamDef"_Stz,pStreamDef)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_STREAMDEF,_FOOTER_);
      
  m_fpDeleteThis = &Card_StreamDef::vDeleteThis_;
  
  _checkInitialised_(CN_CARD_STREAMDEF);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD_STREAMDEF,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_StreamDef::~Card_StreamDef(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_STREAMDEF,_FOOTER_);

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
  
  StreamDef* pStreamDef = _pStreamDef(pGetProperty("pStreamDef"_Stz));
  
  return pStreamDef;
}

/*----------------------------------------------------------------------------*/
/* set stream definition                                                      */
/*----------------------------------------------------------------------------*/
void Card_StreamDef::vStreamDef(StreamDef* pStreamDef)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_STREAMDEF,_FOOTER_);
  
  vAddProperty("pStreamDef"_Stz,pStreamDef);
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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
