/*============================================================================*/
/* Optimisations.cpp -- Optimisations                                           */
/*============================================================================*/

#include "Optimisations.hpp"

#include "Board.hpp"
#include "CardIter.hpp"
#include "Fold.hpp"
#include "Smoosh.hpp"
#include "Smooth.hpp"
#include "Optimiser.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, Optimisations, CN_OPTIMISATIONS)
__aNewFunction(Optimisations, CN_OPTIMISATIONS)
__vDeleteFunction(Optimisations, CN_OPTIMISATIONS)
__aFooterFunction(Optimisations)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Optimisations::Optimisations(CardIter* pCI)
: Object(CN_OPTIMISATIONS)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_OPTIMISATIONS, _FOOTER_);

  m_pCI = pCI;

  _checkInitialised_(CN_OPTIMISATIONS);

  _postcondition_(_ADDRESS_, _MAGIC_, CN_OPTIMISATIONS, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Optimisations::~Optimisations(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_OPTIMISATIONS, _FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/



/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
