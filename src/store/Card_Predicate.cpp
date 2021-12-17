/*============================================================================*/
/* Card_Predicate.cpp -- Predicate                                            */
/*============================================================================*/

#include "Card_Predicate.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Card_Predicate,CN_CARD_PROJECTPRED)
__aNewFunction(Card_Predicate,CN_CARD_PROJECTPRED)
__vDeleteFunction(Card_Predicate,CN_CARD_PROJECTPRED)
__aFooterFunction(Card_Predicate)

__bIsClass(STORE,Card_Predicate,CN_CARD_PROJECTPRED)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Predicate::Card_Predicate(eCK eKind)
: Card(eKind,CN_CARD_PROJECTPRED, "Predicate"_Stz)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_PROJECTPRED,_FOOTER_);
      
  m_fpDeleteThis = &Card_Predicate::vDeleteThis_;
  m_fpBoardPrint = &Card_Predicate::vBoardPrint_;

  m_bPrintNm = false;
  
  _checkInitialised_(CN_CARD_PROJECTPRED);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD_PROJECTPRED,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Predicate::~Card_Predicate(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_PROJECTPRED,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* delete                                                                     */
/*----------------------------------------------------------------------------*/
void Card_Predicate::vDeleteThis_(Card* pCard)
{
  _static_

  delete (Card_Predicate*)pCard;
}

/*----------------------------------------------------------------------------*/
/* print                                                                      */
/*----------------------------------------------------------------------------*/
void Card_Predicate::vBoardPrint_(Card* pCard, Ribbon* pRibbon, i8 iDepth)
{
  _static_
  
  Card_Predicate* pCard_Predicate = _pCard_Predicate(pCard);

  pRibbon->vSput(FR_DOTS
                 ,Stz::szDots(iDepth)
                 ,pCard_Predicate->m_iCardNo
                 ,pCard_Predicate->m_sCardNm.szStz()
                 );

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
