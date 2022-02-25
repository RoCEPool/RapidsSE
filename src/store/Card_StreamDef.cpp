/*============================================================================*/
/* Card_StreamDef.cpp -- Stream Definition                                    */
/*============================================================================*/

#include "Card_StreamDef.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_StreamDef,CK_STREAMDEF)
__aNewFunction(Card_StreamDef,CN_CARD)
__vDeleteFunction(Card_StreamDef,CN_CARD)

__bIsCard(STORE,Card_StreamDef,CK_STREAMDEF)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_StreamDef::Card_StreamDef(StreamDef* pStreamDef)
: Card(CK_STREAMDEF)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  vStreamDef(pStreamDef);
  
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_StreamDef::~Card_StreamDef(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* apply the order from given streamDef to this streamDef                     */
/*----------------------------------------------------------------------------*/
void Card_StreamDef::vApplyOrder(StreamDef* pStreamDef_apply)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  StreamDef* pStreamDef = this->pStreamDef();

  i8 nOrders = pStreamDef_apply->nOrders();
  for (i8 i_Order = 0; i_Order < nOrders; i_Order++) {
    Column* pColumn_apply = pStreamDef_apply->pGetOrder(i_Order);
    Stz* szColNm_apply = pColumn_apply->szColNm();
    
    Column* pColumn = pStreamDef->pFindColumn(szColNm_apply);
    if (! pColumn)
      break;
    
    pStreamDef->vAddOrder(pColumn->iColumnNo());
  }
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
