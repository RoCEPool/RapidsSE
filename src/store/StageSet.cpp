/*============================================================================*/
/* StageSet.cpp -- List of StageSet                                               */
/*============================================================================*/

#include "BASE.h"
#include "StageSet.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, StageSet, CN_STAGESET)
__aNewFunction(StageSet, CN_STAGESET)
__vDeleteFunction(StageSet, CN_STAGESET)
__aFooterFunction(StageSet)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
StageSet::StageSet(void)
: ListNL_t<Stage>(CN_STAGESET)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_STAGESET, _FOOTER_);

  _checkInitialised_(CN_STAGESET);

  _postcondition_(_ADDRESS_, _MAGIC_, CN_STAGESET, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
StageSet::~StageSet(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_STAGESET, _FOOTER_);

  while (Stage* pStage = pPop())
    delete pStage;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
