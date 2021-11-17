/*============================================================================*/
/* Card_OrdAggList.cpp -- List of Ordered Aggregate                           */
/*============================================================================*/

#include "Card_OrdAggList.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Card_OrdAggList,CN_CARD_ORDAGGLIST)
__aNewFunction(Card_OrdAggList,CN_CARD_ORDAGGLIST)
__vDeleteFunction(Card_OrdAggList,CN_CARD_ORDAGGLIST)
__aFooterFunction(Card_OrdAggList)

__bIsClass(STORE,Card_OrdAggList,CN_CARD_ORDAGGLIST)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_OrdAggList::Card_OrdAggList(void)
: Card_ProjectList(CN_CARD_ORDAGGLIST,"OrdAggList"_Stz)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_ORDAGGLIST,_FOOTER_);
    
  m_fpDeleteThis = &Card_OrdAggList::vDeleteThis_;
  m_fpBoardPrint = &Card_OrdAggList::vBoardPrint_;

  m_eAggLevel = AL_NONE;

  _checkInitialised_(CN_CARD_ORDAGGLIST);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD_ORDAGGLIST,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_OrdAggList::~Card_OrdAggList(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_ORDAGGLIST,_FOOTER_);

  vDeleteAll();
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get agg level                                                              */
/*----------------------------------------------------------------------------*/
eAL Card_OrdAggList::eAggLevel(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_AGGREG,_FOOTER_);
  
  return m_eAggLevel;
}

/*----------------------------------------------------------------------------*/
/* set agg level                                                              */
/*----------------------------------------------------------------------------*/
void Card_OrdAggList::vAggLevel(eAL eAggLevel)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_AGGREG,_FOOTER_);
  
  m_eAggLevel = eAggLevel;
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
void Card_OrdAggList::vDeleteThis_(Card* pCard)
{
  _static_
  
  delete (Card_OrdAggList*)pCard;
}

/*----------------------------------------------------------------------------*/
/* print                                                                      */
/*----------------------------------------------------------------------------*/
void Card_OrdAggList::vBoardPrint_(Card* pCard, Ribbon* pRibbon, i8 iDepth)
{
  _static_
  
  Card_OrdAggList* pCard_OrdAggList = _pCard_OrdAggList(pCard);

  s32 sAggLevel;

  sAggLevel.vSprint("%s"_Stz,aAL[pCard_OrdAggList->m_eAggLevel]);

  pRibbon->vSprint("%15s"_Stz,sAggLevel.szStz());
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
