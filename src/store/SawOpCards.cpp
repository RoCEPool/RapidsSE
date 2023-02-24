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

__pCastFunction(STORE, SawOpCards, CN_APPLYOPTIMISATIONS)
__aNewFunction(SawOpCards, CN_APPLYOPTIMISATIONS)
__vDeleteFunction(SawOpCards, CN_APPLYOPTIMISATIONS)
__aFooterFunction(SawOpCards)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SawOpCards::SawOpCards(CardIter* pCI)
: Object(CN_APPLYOPTIMISATIONS)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_APPLYOPTIMISATIONS, _FOOTER_);


  _checkInitialised_(CN_APPLYOPTIMISATIONS);

  _postcondition_(_ADDRESS_, _MAGIC_, CN_APPLYOPTIMISATIONS, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SawOpCards::~SawOpCards(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_APPLYOPTIMISATIONS, _FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- SawOpCards  ----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
