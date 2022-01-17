/*============================================================================*/
/* Card_StreamDef.cpp -- Stream Definition Card                               */
/*============================================================================*/

#include "Card_StreamDef.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_StreamDef,CK_STREAMDEF)
__aNewFunction(Card_StreamDef,CN_CARD_STREAMDEF)
__vDeleteFunction(Card_StreamDef,CN_CARD_STREAMDEF)

__bIsClass(STORE,Card_StreamDef,CN_CARD_STREAMDEF)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_StreamDef::Card_StreamDef(eCK eKind, StreamDef* pStreamDef)
: Card(CK_STREAMDEF)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
      
  vStreamDef(pStreamDef);
  
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_StreamDef::~Card_StreamDef(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
