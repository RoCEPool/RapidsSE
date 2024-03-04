/*============================================================================*/
/* CorrelatedJoin.cpp -- CorrelatedJoin                                                 */
/*============================================================================*/

#include "CorrelatedJoin.hpp"

#include "CardIter.hpp"
#include "CarryForward.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, CorrelatedJoin, CN_CORRELATEDJOIN)
__aNewFunction(CorrelatedJoin, CN_CORRELATEDJOIN)
__vDeleteFunction(CorrelatedJoin, CN_CORRELATEDJOIN)
__aFooterFunction(CorrelatedJoin)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
CorrelatedJoin::CorrelatedJoin(void)
: Object(CN_CORRELATEDJOIN)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CORRELATEDJOIN, _FOOTER_);

  _postcondition_(_ADDRESS_, _MAGIC_, CN_CORRELATEDJOIN, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
CorrelatedJoin::~CorrelatedJoin(void) noexcept(false)
{
_precondition_(_ADDRESS_, _MAGIC_, CN_CORRELATEDJOIN, _FOOTER_);
}



/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
