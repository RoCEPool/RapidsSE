/*============================================================================*/
/* StageSpecSet.cpp -- List of Stage Specifications                             */
/*============================================================================*/

#include "BASE.h"
#include "DeckSpec.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, DeckSpec, CN_DECKSPEC)
__aNewFunction(DeckSpec, CN_DECKSPEC)
__vDeleteFunction(DeckSpec, CN_DECKSPEC)
__aFooterFunction(DeckSpec)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
DeckSpec::DeckSpec(void)
    
: ListNL_t<StageSpec>(CN_DECKSPEC)

{
  _precondition_(_ADDRESS_, _MAGIC_, CN_DECKSPEC, _FOOTER_);


  _checkInitialised_(CN_DECKSPEC);

  _postcondition_(_ADDRESS_, _MAGIC_, CN_DECKSPEC, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
DeckSpec::~DeckSpec(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_DECKSPEC, _FOOTER_);

  while (StageSpec* pStageSpec = pPop())
    delete pStageSpec;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
