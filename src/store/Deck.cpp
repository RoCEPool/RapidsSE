/*============================================================================*/
/* Deck.cpp -- Deck (of stages)                                               */
/*============================================================================*/

#include "BASE.h"

#include "Deck.hpp"
#include "DeckSpec.hpp"
#include "Vertex.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, Deck, CN_DECK)
__aNewFunction(Deck, CN_DECK)
__vDeleteFunction(Deck, CN_DECK)
__aFooterFunction(Deck)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Deck::Deck(Vertex* pVertex, DeckSpec* pDeckSpec)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_DECK, _FOOTER_);

  m_pVertex     = pVertex;
  m_pDeckSpec   = pDeckSpec;

  _checkInitialised_(CN_DECK);

  vCreateStages();
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_DECK, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Deck::~Deck(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_DECK, _FOOTER_);

  //while (Stage* pStage = pPop())
  //  delete pStage;

  i8 nStages = this->nItems();
  for (i8 i_Stage = 0; i_Stage  < nStages; i_Stage++) {
    Stage *pStage = pGet(i_Stage);

    delete pStage;
  }

  vClear();

}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get outer pipeline                                                         */
/*----------------------------------------------------------------------------*/
Pipeline* Deck::pPipeline(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DECK,_FOOTER_);
  
  return m_pVertex->pPipeline();
}

/*----------------------------------------------------------------------------*/
/* get pipework                                                               */
/*----------------------------------------------------------------------------*/
Vertex* Deck::pVertex(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DECK,_FOOTER_);
  
  return m_pVertex;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* create the stages for this deck                                            */
/*----------------------------------------------------------------------------*/
void Deck::vCreateStages(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VERTEX,_FOOTER_);

  i8 nStages = m_pDeckSpec->nItems();
  for (i8 i_StageSpec = 0; i_StageSpec < nStages; i_StageSpec++ ) {

    // get the stageSpec
    StageSpec* pStageSpec = m_pDeckSpec->pGet(i_StageSpec);

    // add a stage from the stageSpec to the stageVec
    Stage* pStage = new Stage(this,pStageSpec);
    vAppend(pStage);
  }
}



/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
