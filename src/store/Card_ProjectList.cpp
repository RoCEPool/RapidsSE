/*============================================================================*/
/* Card_ProjectList.cpp -- List of Project Cards                              */
/*============================================================================*/

#include "Card_ProjectList.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Card_ProjectList,CN_CARD_PROJECTLIST)
__aNewFunction(Card_ProjectList,CN_CARD_PROJECTLIST)
__vDeleteFunction(Card_ProjectList,CN_CARD_PROJECTLIST)
__aFooterFunction(Card_ProjectList)


__bIsClass(STORE,Card_ProjectList,CN_CARD_PROJECTLIST)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_ProjectList::Card_ProjectList(void)
: Card(CN_CARD_PROJECTLIST,"ProjectList"_Stz)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_PROJECTLIST,_FOOTER_);
    
  m_fpDeleteThis = &Card_ProjectList::vDeleteThis_;
  m_fpBoardPrint = &Card_ProjectList::vBoardPrint_;

  _checkInitialised_(CN_CARD_PROJECTLIST);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD_PROJECTLIST,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_ProjectList::Card_ProjectList(eCN eClassNo, Stz* szCardNm)
: Card(eClassNo,szCardNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_PROJECTLIST,_FOOTER_);
    
  m_fpDeleteThis = &Card_ProjectList::vDeleteThis_;
  m_fpBoardPrint = &Card_ProjectList::vBoardPrint_;

  _checkInitialised_(CN_CARD_PROJECTLIST);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD_PROJECTLIST,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_ProjectList::~Card_ProjectList(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_PROJECTLIST,_FOOTER_);

  vDeleteAll();
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
void Card_ProjectList::vDeleteThis_(Card* pCard)
{
  _static_
  
  delete (Card_ProjectList*)pCard;
}

/*----------------------------------------------------------------------------*/
/* print                                                                      */
/*----------------------------------------------------------------------------*/
void Card_ProjectList::vBoardPrint_(Card* pCard, Ribbon* pRibbon, i8 iDepth)
{
  _static_
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
