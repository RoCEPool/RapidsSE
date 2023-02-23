/*============================================================================*/
/* MakeWeavable.cpp -- MakeWeavable                                           */
/*============================================================================*/

#include "MakeWeavable.hpp"

#include "Board.hpp"
#include "CardIter.hpp"
#include "Fold.hpp"
#include "Smoosh.hpp"
#include "Smooth.hpp"
#include "Optimiser.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, MakeWeavable, CN_MAKEWEAVABLE)
__aNewFunction(MakeWeavable, CN_MAKEWEAVABLE)
__vDeleteFunction(MakeWeavable, CN_MAKEWEAVABLE)
__aFooterFunction(MakeWeavable)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
MakeWeavable::MakeWeavable(CardIter* pCI)
: Object(CN_MAKEWEAVABLE)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MAKEWEAVABLE, _FOOTER_);

  m_pCI = pCI;

  _checkInitialised_(CN_MAKEWEAVABLE);

  _postcondition_(_ADDRESS_, _MAGIC_, CN_MAKEWEAVABLE, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
MakeWeavable::~MakeWeavable(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MAKEWEAVABLE, _FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/



/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
