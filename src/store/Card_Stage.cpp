/*============================================================================*/
/* Card_Stage.cpp -- Stage                                                    */
/*============================================================================*/

#include "Card_Stage.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Card_Stage,CN_CARD_STAGE)
__aNewFunction(Card_Stage,CN_CARD_STAGE)
__vDeleteFunction(Card_Stage,CN_CARD_STAGE)
__aFooterFunction(Card_Stage)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Stage::Card_Stage(eCK eKind, Stz* szStageNm, eYG eStageTopology)
: Card_Unit(CK_STAGE,CN_CARD_STAGE, "Stage"_Stz)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_STAGE,_FOOTER_);
      
  m_fpDeleteThis = &Card_Stage::vDeleteThis_;

  vAddProperty("szStageNm"_Stz,szStageNm);
  vAddProperty("eStageTopology"_Stz,eStageTopology);
  
  _checkInitialised_(CN_CARD_STAGE);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD_STAGE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Stage::~Card_Stage(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_STAGE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get stage name                                                             */
/*----------------------------------------------------------------------------*/
Stz* Card_Stage::szStageNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_STAGE,_FOOTER_);
  
  Stz* szStageNm = szGetProperty("szStageNm"_Stz);

  return szStageNm;
}

/*----------------------------------------------------------------------------*/
/* set stage name                                                             */
/*----------------------------------------------------------------------------*/
void Card_Stage::vStageNm(Stz* szStageNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_STAGE,_FOOTER_);
  
  vAddProperty("szStageNm"_Stz,szStageNm);
}

/*----------------------------------------------------------------------------*/
/* get stage topology                                                         */
/*----------------------------------------------------------------------------*/
eYG Card_Stage::eStageTopology(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_STAGE,_FOOTER_);
  
  eYG eStageTopology = (eYG)iGetProperty("eStageTopology"_Stz);

  return eStageTopology;
}

/*----------------------------------------------------------------------------*/
/* set stage topology                                                         */
/*----------------------------------------------------------------------------*/
void Card_Stage::vStageTopology(eYG  eStageTopology)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_STAGE,_FOOTER_);
  
  vAddProperty("eStageTopology"_Stz,eStageTopology);
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
void Card_Stage::vDeleteThis_(Card* pCard)
{
  _static_

  delete (Card_Stage*)pCard;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
