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
Card_Stage::Card_Stage(Stz* szStageNm, eYG eStageTopology)
: Card_Unit(CN_CARD_STAGE, "Stage"_Stz)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_STAGE,_FOOTER_);
      
  m_fpDeleteThis = &Card_Stage::vDeleteThis_;
  m_fpBoardPrint = &Card_Stage::vBoardPrint_;

  m_sStageNm       = szStageNm;
  m_eStageTopology = eStageTopology;
  
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
  
  return m_sStageNm;
}

/*----------------------------------------------------------------------------*/
/* set stage name                                                             */
/*----------------------------------------------------------------------------*/
void Card_Stage::vStageNm(Stz* szStageNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_STAGE,_FOOTER_);
  
  m_sStageNm = szStageNm;
}

/*----------------------------------------------------------------------------*/
/* get stage topology                                                         */
/*----------------------------------------------------------------------------*/
eYG Card_Stage::eStageTopology(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_STAGE,_FOOTER_);
  
  return m_eStageTopology;
}

/*----------------------------------------------------------------------------*/
/* set stage topology                                                         */
/*----------------------------------------------------------------------------*/
void Card_Stage::vStageTopology(eYG  eStageTopology)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_STAGE,_FOOTER_);
  
  m_eStageTopology = eStageTopology;
}

/*----------------------------------------------------------------------------*/
/* print index                                                                */
/*----------------------------------------------------------------------------*/
void Card_Stage::vPrintStage(Ribbon* pRibbon)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_STAGE,_FOOTER_);
  
  pRibbon->vSput(FR_NAME,m_sStageNm.azChar());
  pRibbon->vSput(FR_TEXT,aYG_15s[m_eStageTopology]);
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

/*----------------------------------------------------------------------------*/
/* print                                                                      */
/*----------------------------------------------------------------------------*/
void Card_Stage::vBoardPrint_(Card* pCard, Ribbon* pRibbon, i8 iDepth)
{
  _static_
  
  Card_Stage* pCard_Stage = _pCard_Stage(pCard);
  
  pCard_Stage->vPrintStage(pRibbon);

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
