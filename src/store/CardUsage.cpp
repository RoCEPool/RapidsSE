/*============================================================================*/
/* CardUsage.cpp -- Card Usage                                                */
/*============================================================================*/

#include "CardUsage.hpp"
#include "Card.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,CardUsage,CN_CARDUSAGE)
__aNewFunction(CardUsage,CN_CARDUSAGE)
__vDeleteFunction(CardUsage,CN_CARDUSAGE)
__aFooterFunction(CardUsage)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
CardUsage::CardUsage(void)
: Object(CN_CARDUSAGE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDUSAGE,_FOOTER_);

  vClearAll();

  _checkInitialised_(CN_CARDUSAGE);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARDUSAGE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
CardUsage::~CardUsage(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDUSAGE,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool CardUsage::bAddCombined(eCK eCard)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARDUSAGE, _FOOTER_);

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool CardUsage::bAddNext(eCK eCard)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARDUSAGE, _FOOTER_);

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool CardUsage::bAddStage(eCK eCard)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARDUSAGE, _FOOTER_);

  return false;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardUsage::vClear(eCK eCard)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARDUSAGE, _FOOTER_);

  m_baSawCard[eCard] = false;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardUsage::vClearAll(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARDUSAGE, _FOOTER_);

  for (i8 i_Card = 0; i_Card < CK_ENUMS; i_Card++)
    m_baSawCard[i_Card] = false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool CardUsage::bSaw(eCK eCard)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARDUSAGE, _FOOTER_);

  return m_baSawCard[eCard];
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
i8 CardUsage::nSaw(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARDUSAGE, _FOOTER_);

  i8 nUsed = 0;
  for (i8 i_Card = 0; i_Card < CK_ENUMS; i_Card++)
    if (m_baSawCard[i_Card])
      nUsed++;

  return nUsed;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardUsage::vSaw(eCK eCard, bool bUsed)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARDUSAGE, _FOOTER_);

  m_baSawCard[eCard] = bUsed;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
