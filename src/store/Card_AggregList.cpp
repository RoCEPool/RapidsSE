/*============================================================================*/
/* Card_AggregList.cpp -- List of Aggregate Cards                               */
/*============================================================================*/

#include "Card_AggregList.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Card_AggregList,CN_CARD_AGGREGLIST)
__aNewFunction(Card_AggregList,CN_CARD_AGGREGLIST)
__vDeleteFunction(Card_AggregList,CN_CARD_AGGREGLIST)
__aFooterFunction(Card_AggregList)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_AggregList::Card_AggregList(void)
: Card(CN_CARD_AGGREGLIST,"AggregList"_Stz)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_AGGREGLIST,_FOOTER_);
    
  m_fpDeleteThis = &Card_AggregList::vDeleteThis_;
  m_fpBoardPrint = &Card_AggregList::vBoardPrint_;

  m_eAggLevel = AL_NONE;

  _checkInitialised_(CN_CARD_AGGREGLIST);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD_AGGREGLIST,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_AggregList::~Card_AggregList(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_AGGREGLIST,_FOOTER_);

  vDeleteAll();
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get agg level                                                              */
/*----------------------------------------------------------------------------*/
eAL Card_AggregList::eAggLevel(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_AGGREG,_FOOTER_);
  
  return m_eAggLevel;
}

/*----------------------------------------------------------------------------*/
/* set agg level                                                              */
/*----------------------------------------------------------------------------*/
void Card_AggregList::vAggLevel(eAL eAggLevel)
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
void Card_AggregList::vDeleteThis_(Card* pCard)
{
  _static_
  
  delete (Card_AggregList*)pCard;
}

/*----------------------------------------------------------------------------*/
/* print                                                                      */
/*----------------------------------------------------------------------------*/
void Card_AggregList::vBoardPrint_(Card* pCard, Ribbon* pRibbon, i8 iDepth)
{
  _static_
  
  Card_AggregList* pCard_AggregList = _pCard_AggregList(pCard);

  s32 sAggLevel;

  sAggLevel.vSprint("%s"_Stz,aAL[pCard_AggregList->m_eAggLevel]);

  pRibbon->vSprint("%15s"_Stz,sAggLevel.szStz());
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
