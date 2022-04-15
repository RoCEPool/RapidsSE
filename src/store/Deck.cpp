/*============================================================================*/
/* Deck.cpp -- Deck (of stages)                                               */
/*============================================================================*/

#include "BASE.h"

#include "Deck.hpp"
#include "DeckSpec.hpp"
#include "Vertex.hpp"
#include "Pipeline.hpp"
#include "Stream.hpp"


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

//  i8 nStages = this->nItems();
//  for (i8 i_Stage = 0; i_Stage  < nStages; i_Stage++) {
//    Stage *pStage = pGet(i_Stage);
//
//    delete pStage;
//  }
//
//  vClear();
  
  while (Stage* pStage = pPop())
    delete pStage;
  vCheckEmpty();
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* repair the deck                                                            */
/*----------------------------------------------------------------------------*/
void Deck::vRepair(Object *pObject)
{
  _static_

  Deck *pDeck = _pDeck(pObject);

  delete pDeck;
}



/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get op 0                                                                   */
/*----------------------------------------------------------------------------*/
Op* Deck::pOp0(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DECK,_FOOTER_);
  
  Stage*  pStage0 = pGet(0);
  Op*     pOp0    = pStage0->pOp0();

  return pOp0;
}

/*----------------------------------------------------------------------------*/
/* get number of ops to be excuted on given node                              */
/*----------------------------------------------------------------------------*/
i8 Deck::nOpsExeOnNode(i8 iThisNodeId)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DECK,_FOOTER_);
  
  i8 nOps = 0;
  
  i8 iOriginNodeId = pPipeline()->iOriginNodeId();
  
  i8 nStages = nItems();
  for (i8 i_Stage = 0; i_Stage < nStages; i_Stage++) {
    Stage* pStage = pGet(i_Stage);
    nOps += pStage->nOpsExeOnNode(iThisNodeId,iOriginNodeId);
  }
  
  return nOps;
}

/*----------------------------------------------------------------------------*/
/* get outer pipeline                                                         */
/*----------------------------------------------------------------------------*/
Pipeline* Deck::pPipeline(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DECK,_FOOTER_);
  
  return m_pVertex->pPipeline();
}

/*----------------------------------------------------------------------------*/
/* spawn operaator jobs for the pipeline                                      */
/*----------------------------------------------------------------------------*/
void Deck::vSetInvoker(Task* pInvoker)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DECK,_FOOTER_);
  
  i8 nStages = nItems();
  for (i8 i_Stage = 0; i_Stage < nStages; i_Stage++) {
    
    Stage* pStage = pGet(i_Stage);
    
    for (i8 i_Stream = 0; i_Stream < pStage->nItems(); i_Stream++) {
      Stream* pStream = pStage->pGet(i_Stream);

      pStream->vSetInvoker(pInvoker);
    }
  }
  
}

/*----------------------------------------------------------------------------*/
/* get stage 0                                                                */
/*----------------------------------------------------------------------------*/
Stage* Deck::pStage0(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DECK,_FOOTER_);
  
  Stage*  pStage0 = pGet(0);

  return pStage0;
}


/*----------------------------------------------------------------------------*/
/* get vertex                                                                 */
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_DECK,_FOOTER_);

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
