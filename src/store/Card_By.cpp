/*============================================================================*/
/* Card_By.cpp -- By Card                                                     */
/*============================================================================*/

#include "Card_By.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Card_By,CN_CARD_BY)
__aNewFunction(Card_By,CN_CARD_BY)
__vDeleteFunction(Card_By,CN_CARD_BY)
__aFooterFunction(Card_By)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_By::Card_By(void)
: Card(CK_NONE,CN_CARD_BY,"By"_Stz)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_BY,_FOOTER_);
      
  m_fpDeleteThis = &Card_By::vDeleteThis_;
  m_fpBoardPrint = &Card_By::vBoardPrint_;

  m_pColumn = nullptr;
  m_pTable  = nullptr;
  
  m_eAggLevel = AL_NONE;

  _checkInitialised_(CN_CARD_BY);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD_BY,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_By::Card_By(Column* pColumn, Table* pTable)
: Card(CK_NONE,CN_CARD_BY,"By"_Stz)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_BY,_FOOTER_);
      
  m_fpDeleteThis = &Card_By::vDeleteThis_;
  m_fpBoardPrint = &Card_By::vBoardPrint_;

  m_pColumn = pColumn;
  m_pTable  = pTable;
  
  m_eAggLevel = AL_NONE;

  _checkInitialised_(CN_CARD_BY);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD_BY,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_By::~Card_By(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_BY,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get aggregate level                                                        */
/*----------------------------------------------------------------------------*/
eAL Card_By::eAggLevel(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_BY,_FOOTER_);
  
  return m_eAggLevel;
}

/*----------------------------------------------------------------------------*/
/* set aggregate level                                                        */
/*----------------------------------------------------------------------------*/
void Card_By::vAggLevel(eAL eAggLevel)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_BY,_FOOTER_);
  
  m_eAggLevel = eAggLevel;
}

/*----------------------------------------------------------------------------*/
/* return column                                                              */
/*----------------------------------------------------------------------------*/
Column* Card_By::pColumn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_BY,_FOOTER_);
  
  return m_pColumn;
}

/*----------------------------------------------------------------------------*/
/* return table                                                               */
/*----------------------------------------------------------------------------*/
Table* Card_By::pTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_BY,_FOOTER_);
  
  return m_pTable;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* print hashcast                                                             */
/*----------------------------------------------------------------------------*/
void Card_By::vDeleteThis_(Card* pCard)
{
  _static_
  
  delete (Card_By*)pCard;
}

/*----------------------------------------------------------------------------*/
/* print by piece                                                             */
/*----------------------------------------------------------------------------*/
void Card_By::vBoardPrint_(Card* pCard, Ribbon* pRibbon, i8 iDepth)
{
  _static_

//  Card_By* pCardBy = _pCard_By(pCard);
//  
//  s32 sBy;
//
//  sBy.vSprint("{%s}"_Stz,pCardBy->pColumn()->szColNm());
//
//  pRibbon->vSprint("%15s"_Stz,sBy.szStz());
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
