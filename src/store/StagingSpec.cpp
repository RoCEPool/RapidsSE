/*============================================================================*/
/* StageSpecSet.cpp -- List of Stage Specifications                             */
/*============================================================================*/

#include "BASE.h"
#include "StagingSpec.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, StagingSpec, CN_STAGINGSPEC)
__aNewFunction(StagingSpec, CN_STAGINGSPEC)
__vDeleteFunction(StagingSpec, CN_STAGINGSPEC)
__aFooterFunction(StagingSpec)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
StagingSpec::StagingSpec(void)
    
: ListNL_t<StageSpec>(CN_STAGINGSPEC)

{
  _precondition_(_ADDRESS_, _MAGIC_, CN_STAGINGSPEC, _FOOTER_);


  _checkInitialised_(CN_STAGINGSPEC);

  _postcondition_(_ADDRESS_, _MAGIC_, CN_STAGINGSPEC, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
StagingSpec::~StagingSpec(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_STAGINGSPEC, _FOOTER_);

  while (StageSpec* pStageSpec = pPop())
    delete pStageSpec;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
