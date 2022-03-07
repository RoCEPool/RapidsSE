/*============================================================================*/
/* CardRegs.cpp -- Card Registers                                             */
/*============================================================================*/

 #include "CardRegs.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,CardRegs,CN_CARDREGS)
__aNewFunction(CardRegs,CN_CARDREGS)
__vDeleteFunction(CardRegs,CN_CARDREGS)
__aFooterFunction(CardRegs)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
CardRegs::CardRegs(void)
: Object(CN_CARDREGS)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDREGS,_FOOTER_);
        
  MemLib::vZero(m_paCardReg,sizeof(m_paCardReg),CN_CARDREGS);

  _checkInitialised_(CN_CARDREGS);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARDREGS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
CardRegs::~CardRegs(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDREGS,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardRegs::pCardReg(eCK eCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDREGS,_FOOTER_);

  Card* pCard = m_paCardReg[eCard];
  
  return pCard;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardRegs::vCardReg(Card* pCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDREGS,_FOOTER_);

  eCK eCard = pCard->eCard();

  m_paCardReg[eCard] = pCard;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardRegs::vClearCardRegs(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDREGS,_FOOTER_);

  MemLib::vZero(m_paCardReg,sizeof(m_paCardReg),CN_CARDREGS);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool CardRegs::bInCardReg(Card* pCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDREGS,_FOOTER_);

  eCK eCard = pCard->eCard();
  
  if (pCard == m_paCardReg[eCard])
    return true;
  
  return false;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
