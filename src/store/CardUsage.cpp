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

    case CK_o_AGGREG:
    case CK_o_CROSSJOIN:
    case CK_o_FILTER:
    case CK_o_GBYAGG:
    case CK_o_HAVING:
    case CK_o_INDEXJOIN:
    case CK_o_INDEXSCAN:
    case CK_o_KEYEXISTS:
    case CK_o_KEYNOTEXISTS:
    case CK_o_LIMIT:
    case CK_o_ORDAGG:
    case CK_o_PROJECT:
    case CK_o_WHERE:
    case CK_o_SCAN:
    case CK_o_SORT:
    case CK_p_TABLE:
    case CK_o_TOP:
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

    case CK_o_AGGREG:
    case CK_o_CROSSJOIN:
    case CK_o_FILTER:
    case CK_o_GBYAGG:
    case CK_o_HAVING:
    case CK_o_INDEXJOIN:
    case CK_o_INDEXSCAN:
    case CK_o_KEYEXISTS:
    case CK_o_KEYNOTEXISTS:
    case CK_o_LIMIT:
    case CK_o_ORDAGG:
    case CK_o_PROJECT:
    case CK_o_WHERE:
    case CK_o_SCAN:
    case CK_o_SORT:
    case CK_p_TABLE:
    case CK_o_TOP:
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

    case CK_o_AGGREG:
    case CK_o_CROSSJOIN:
    case CK_o_FILTER:
    case CK_o_GBYAGG:
    case CK_o_HAVING:
    case CK_o_INDEXJOIN:
    case CK_o_INDEXSCAN:
    case CK_o_KEYEXISTS:
    case CK_o_KEYNOTEXISTS:
    case CK_o_LIMIT:
    case CK_o_ORDAGG:
    case CK_o_PROJECT:
    case CK_o_WHERE:
    case CK_o_SCAN:
    case CK_o_SORT:
    case CK_p_TABLE:
    case CK_o_TOP:
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
