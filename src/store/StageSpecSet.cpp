/*============================================================================*/
/* StageSpecSet.cpp -- List of Stage Specifications                             */
/*============================================================================*/

#include "BASE.h"
#include "StageSpecSet.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, StageSpecSet, CN_STAGESPECSET)
__aNewFunction(StageSpecSet, CN_STAGESPECSET)
__vDeleteFunction(StageSpecSet, CN_STAGESPECSET)
__aFooterFunction(StageSpecSet)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
StageSpecSet::StageSpecSet(void)
    
: ListNL_t<StageSpec>(CN_STAGESPECSET)

{
  _precondition_(_ADDRESS_, _MAGIC_, CN_STAGESPECSET, _FOOTER_);


  _checkInitialised_(CN_STAGESPECSET);

  _postcondition_(_ADDRESS_, _MAGIC_, CN_STAGESPECSET, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
StageSpecSet::~StageSpecSet(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_STAGESPECSET, _FOOTER_);

  while (StageSpec* pStageSpec = pPop())
    delete pStageSpec;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
