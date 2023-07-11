/*============================================================================*/
/* SawOpCards.cpp -- SawOpCards                                           */
/*============================================================================*/

#include "SawOpCards.hpp"

#include "Board.hpp"
#include "CardIter.hpp"
#include "Fold.hpp"
#include "XF_Smoosh.hpp"
#include "XF_Smooth.hpp"
#include "Optimiser.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, SawOpCards, CN_SAWOPCARDS)
__aNewFunction(SawOpCards, CN_SAWOPCARDS)
__vDeleteFunction(SawOpCards, CN_SAWOPCARDS)
__aFooterFunction(SawOpCards)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SawOpCards::SawOpCards(void)
: Object(CN_SAWOPCARDS)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_SAWOPCARDS, _FOOTER_);

  vReset(AL_PARTITION);

  _checkInitialised_(CN_SAWOPCARDS);

  _postcondition_(_ADDRESS_, _MAGIC_, CN_SAWOPCARDS, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SawOpCards::~SawOpCards(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_SAWOPCARDS, _FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* reset                                                                      */
/*----------------------------------------------------------------------------*/
void SawOpCards::vReset(eAL eAggLevel)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_SAWOPCARDS, _FOOTER_);

  if (AL_NONE != eAggLevel)
    m_eAggLevel = eAggLevel;

  m_p_aggreg      = nullptr;
  m_p_filter      = nullptr;
  m_p_gbyAgg      = nullptr;
  m_p_having      = nullptr;
  m_p_indexJoin   = nullptr;
  m_p_indexScan   = nullptr;
  m_p_project     = nullptr;
  m_p_scan        = nullptr;
  m_p_sort        = nullptr;
  m_p_where       = nullptr;
}

/*----------------------------------------------------------------------------*/
/* update what has been seen                                                  */
/*----------------------------------------------------------------------------*/
void SawOpCards::vSaw(Card_OP* p_card_OP)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_SAWOPCARDS, _FOOTER_);

  eCK e_card_OP = p_card_OP->eCard();

  switch (e_card_OP) {
    case CK_AGGREG:    m_p_aggreg    = _pCard_Aggreg(p_card_OP);     break;
    case CK_FILTER:    m_p_filter    = _pCard_Filter(p_card_OP);     break;
    case CK_GBYAGG:    m_p_gbyAgg    = _pCard_GbyAgg(p_card_OP);     break;
    case CK_HAVING:    m_p_having    = _pCard_Having(p_card_OP);     break;
    case CK_INDEXJOIN: m_p_indexJoin = _pCard_IndexJoin(p_card_OP);  break;
    case CK_INDEXSCAN: m_p_indexScan = _pCard_IndexScan(p_card_OP);  break;
    case CK_PROJECT:   m_p_project   = _pCard_Project(p_card_OP);    break;
    case CK_SCAN:      m_p_scan      = _pCard_Scan(p_card_OP);       break;
    case CK_SORT:      m_p_sort      = _pCard_Sort(p_card_OP);       break;
    case CK_WHERE:     m_p_where     = _pCard_Where(p_card_OP);      break;
    default:
      NEVER_REACH
  }

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
