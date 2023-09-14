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
/* add combined op (given card) ?                                             */
/*----------------------------------------------------------------------------*/
bool CardUsage::bAddCombined(Card_OP* p_OP)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARDUSAGE, _FOOTER_);

  eCK e_OP = p_OP->eCard();

  switch (e_OP) {

    case CK_AGGREG:
    case CK_CROSSJOIN:
    case CK_FILTER:
    case CK_GBYAGG:
    case CK_HAVING:
    case CK_INDEXJOIN:
    case CK_INDEXSCAN:
    case CK_KEYEXISTS:
    case CK_KEYNOTEXISTS:
    case CK_LIMIT:
    case CK_ORDAGG:
    case CK_PROJECT:
    case CK_WHERE:
    case CK_SCAN:
    case CK_SORT:
    case CK_TABLE:
    case CK_TOP:
      break;

    default:
      NEVER_REACH
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* add next op (given card) ?                                                 */
/*----------------------------------------------------------------------------*/
bool CardUsage::bAddNext(Card_OP* p_OP)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARDUSAGE, _FOOTER_);

  eCK e_OP = p_OP->eCard();

  switch (e_OP) {

    case CK_AGGREG:
    case CK_CROSSJOIN:
    case CK_FILTER:
    case CK_GBYAGG:
    case CK_HAVING:
    case CK_INDEXJOIN:
    case CK_INDEXSCAN:
    case CK_KEYEXISTS:
    case CK_KEYNOTEXISTS:
    case CK_LIMIT:
    case CK_ORDAGG:
    case CK_PROJECT:
    case CK_WHERE:
    case CK_SCAN:
    case CK_SORT:
    case CK_TABLE:
    case CK_TOP:
      break;

    default:
      NEVER_REACH
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* add stage (given card) ?                                                   */
/*----------------------------------------------------------------------------*/
bool CardUsage::bAddStage(Card_OP* p_OP)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARDUSAGE, _FOOTER_);

  eCK e_OP = p_OP->eCard();

  switch (e_OP) {

    case CK_AGGREG:
    case CK_CROSSJOIN:
    case CK_FILTER:
    case CK_GBYAGG:
    case CK_HAVING:
    case CK_INDEXJOIN:
    case CK_INDEXSCAN:
    case CK_KEYEXISTS:
    case CK_KEYNOTEXISTS:
    case CK_LIMIT:
    case CK_ORDAGG:
    case CK_PROJECT:
    case CK_WHERE:
    case CK_SCAN:
    case CK_SORT:
    case CK_TABLE:
    case CK_TOP:
      break;

    default:
      NEVER_REACH
  }

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

  m_eLevel = Card::e_stageTopo_To_aggLevel(YG_ALL_PARTITIONS);

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
void CardUsage::vSaw(Card_OP* p_OP)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARDUSAGE, _FOOTER_);

  eCK e_OP = p_OP->eCard();

  m_baSawCard[e_OP] = true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
