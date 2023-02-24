/*============================================================================*/
/* SawOpCards.cpp -- SawOpCards                                           */
/*============================================================================*/

#include "SawOpCards.hpp"

#include "Board.hpp"
#include "CardIter.hpp"
#include "Fold.hpp"
#include "Smoosh.hpp"
#include "Smooth.hpp"
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
SawOpCards::SawOpCards(CardIter* pCI)
: Object(CN_SAWOPCARDS)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_SAWOPCARDS, _FOOTER_);

  vReset();

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
void SawOpCards::vReset(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_SAWOPCARDS, _FOOTER_);

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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
