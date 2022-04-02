/*============================================================================*/
/* Steps_Deck.cpp -- Deck step group                                          */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "eYX.h"

#include "Grid.hpp"
#include "Op.hpp"
#include "Stream.hpp"
#include "Stage.hpp"
#include "Vertex.hpp"
#include "Pipeline.hpp"
#include "VertexSpec.hpp"
#include "Job.hpp"
#include "Node_I.hpp"
#include "LogicUnit.hpp"

#include "Marker.hpp"

#include "SP_Pipeline.hpp"
#include "SP_PipeSpec.hpp"

#include "Dynamo.hpp"
#include "TagConstants.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Deck(void)
{
  vAddStep("Deck[]"_Stz,                &Step::vDeckGet);
  vAddStep("Deck[].set"_Stz,            &Step::vDeckSet);
  
  vAddStep("Deck[].enqueueInvoke?"_Stz, &Step::vDeckEnqueueInvoke);
  vAddStep("Deck[].op0@"_Stz,           &Step::vDeckOp0Fetch);
  vAddStep("Deck[].opsOnNode@"_Stz,     &Step::vDeckOpsOnNodeFetch);
  vAddStep("Deck[].setInvoker"_Stz,     &Step::vDeckSetInvoker);
}

/*----------------------------------------------------------------------------*/
/* Deck[].get -- get subpipeline                                              */
/*----------------------------------------------------------------------------*/
void Step::vDeckGet(void)
{
  ENTRY();
   
  if (m_pLogicUnit->m_xpDeck.bIsNull())
    m_pDataStack->vPushPtr(nullptr);
  else
    m_pDataStack->vPushPtr(* m_pLogicUnit->m_xpDeck);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Deck[].set -- set subpipeline                                              */
/*----------------------------------------------------------------------------*/
void Step::vDeckSet(void)
{
  ENTRY();
  
  m_pLogicUnit->m_xpDeck = _pDeck(m_pDataStack->pPopPtr(__func__));

  EXIT();
}



/*----------------------------------------------------------------------------*/
/* Deck[].enqueueInvoke? -- test for enquing an invoke marker                 */
/*----------------------------------------------------------------------------*/
void Step::vDeckEnqueueInvoke(void)
{
  ENTRY();
  
  Deck* pDeck= * m_pLogicUnit->m_xpDeck;

  Stage*  pStage0   = pDeck->pStage0();

  if (pStage0->bEnqueueInvoke()) {
    m_pDataStack->vPushInt(true);
  }
  else {
    m_pDataStack->vPushInt(false);
  }

  EXIT();
}


/*----------------------------------------------------------------------------*/
/* Deck[].op0@ -- fetch op0 of this pipeline                                  */
/*----------------------------------------------------------------------------*/
void Step::vDeckOp0Fetch(void)
{
  ENTRY();
    
  Op* pOp0 = m_pLogicUnit->m_xpDeck->pOp0();
    
  m_pDataStack->vPushPtr(pOp0);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Deck[].opsOnNode@ -- get no of ops executing on this node                  */
/*----------------------------------------------------------------------------*/
void Step::vDeckOpsOnNodeFetch(void)
{
  ENTRY();
    
  i8 iThisNode = AM::pGrid()->iThisNodeNo();

  i8 nOps = m_pLogicUnit->m_xpDeck->nOpsOnNode(iThisNode);

  m_pDataStack->vPushInt(nOps);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Deck[].setInvoker -- set invoker for all tasks in sub-pipeline             */
/*----------------------------------------------------------------------------*/
void Step::vDeckSetInvoker(void)
{
  ENTRY();

  Task* pInvoker = _pTask(m_pDataStack->pPopPtr(__func__));
  
  m_pLogicUnit->m_xpDeck->vSetInvoker(pInvoker);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
