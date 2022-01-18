/*============================================================================*/
/* Card_Stage.cpp -- Stage                                                    */
/*============================================================================*/

#include "Card_Stage.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_Stage,CK_STAGE)
__aNewFunction(Card_Stage,CN_CARD)
__vDeleteFunction(Card_Stage,CN_CARD)

__bIsCard(STORE,Card_Stage,CK_STAGE)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Stage::Card_Stage(Stz* szStageNm, eYG eStageTopology)
: Card_UNIT(CK_STAGE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  vStageNm(szStageNm);
  vStageTopology(eStageTopology);
  
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Stage::~Card_Stage(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
