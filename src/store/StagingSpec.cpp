/*============================================================================*/
/* StageSpecSet.cpp -- List of Stage Specifications                             */
/*============================================================================*/

#include "BASE.h"
#include "StagingSpec.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, StagingSpec, CN_DECKSPEC)
__aNewFunction(StagingSpec, CN_DECKSPEC)
__vDeleteFunction(StagingSpec, CN_DECKSPEC)
__aFooterFunction(StagingSpec)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
StagingSpec::StagingSpec(void)
    
: ListNL_t<StageSpec>(CN_DECKSPEC)

{
  _precondition_(_ADDRESS_, _MAGIC_, CN_DECKSPEC, _FOOTER_);


  _checkInitialised_(CN_DECKSPEC);

  _postcondition_(_ADDRESS_, _MAGIC_, CN_DECKSPEC, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
StagingSpec::~StagingSpec(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_DECKSPEC, _FOOTER_);

  while (StageSpec* pStageSpec = pPop())
    delete pStageSpec;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
