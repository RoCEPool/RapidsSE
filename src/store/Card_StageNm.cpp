/*============================================================================*/
/* Card_StageNm.cpp -- Name                                                      */
/*============================================================================*/

#include "Card_StageNm.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_StageNm, CK_STAGENM)
__aNewFunction(Card_StageNm, CN_CARD)
__vDeleteFunction(Card_StageNm, CN_CARD)

__bIsCard(STORE, Card_StageNm, CK_STAGENM)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_StageNm::Card_StageNm(Stz* szName)
: Card(CK_STAGENM,szName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  //vValue(szName);
    
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_StageNm::~Card_StageNm(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
