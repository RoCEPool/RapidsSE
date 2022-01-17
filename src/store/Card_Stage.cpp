/*============================================================================*/
/* Card_Stage.cpp -- Stage                                                    */
/*============================================================================*/

#include "Card_Stage.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_Stage,CK_STAGE)
__aNewFunction(Card_Stage,CN_CARD_STAGE)
__vDeleteFunction(Card_Stage,CN_CARD_STAGE)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Stage::Card_Stage(eCK eKind, Stz* szStageNm, eYG eStageTopology)
: Card_Unit(CK_STAGE,CN_CARD_STAGE)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
      
  vStageNm(szStageNm);
  vStageTopology(eStageTopology);
  
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Stage::~Card_Stage(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
