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
DeckSpec::DeckSpec(VertexSpec* pVertexSpec)
    
: ListNL_t<StageSpec>(CN_DECKSPEC)

{
  _precondition_(_ADDRESS_, _MAGIC_, CN_DECKSPEC, _FOOTER_);
  
  m_pVertexSpec = pVertexSpec;

  m_pParamsSpec = nullptr;

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

  delete m_pParamsSpec;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get parameter list spec                                                    */
/*----------------------------------------------------------------------------*/
ParamsSpec* DeckSpec::pParamsSpec(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_DECKSPEC, _FOOTER_);

  return m_pParamsSpec;
}

/*----------------------------------------------------------------------------*/
/* set parameter list spec                                                    */
/*----------------------------------------------------------------------------*/
void DeckSpec::vParamsSpec(ParamsSpec* pParamsSpec)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_DECKSPEC, _FOOTER_);

  m_pParamsSpec = pParamsSpec;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
