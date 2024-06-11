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
  vAddStep("Deck[].opsExeOnNode@"_Stz,  &Step::vDeckOpsExeOnNodeFetch);
  vAddStep("Deck[].setJobTask"_Stz,     &Step::vDeckSetJobTask);
}

/*----------------------------------------------------------------------------*/
/* Deck[].get -- get subpipeline                                              */
/*----------------------------------------------------------------------------*/
void Step::vDeckGet(void)
{
  ENTRY();
   
  if (m_pLogicUnit->m_xpDeck.bIsNull())
    m_oDataStack.vPushPtr(nullptr);
  else
    m_oDataStack.vPushPtr(* m_pLogicUnit->m_xpDeck);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Deck[].set -- set subpipeline                                              */
/*----------------------------------------------------------------------------*/
void Step::vDeckSet(void)
{
  ENTRY();
  
  m_pLogicUnit->m_xpDeck = _pDeck(m_oDataStack.pPopPtr(__func__));

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
    m_oDataStack.vPushInt(true);
  }
  else {
    m_oDataStack.vPushInt(false);
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
    
  m_oDataStack.vPushPtr(pOp0);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Deck[].opsExeOnNode@ -- get no of ops that will execute on this node       */
/*----------------------------------------------------------------------------*/
void Step::vDeckOpsExeOnNodeFetch(void)
{
  ENTRY();
    
  i8 iThisNodeId = AM::pGrid()->iThisNodeId();

  i8 nOps = m_pLogicUnit->m_xpDeck->nOpsExeOnNode(iThisNodeId);

  m_oDataStack.vPushInt(nOps);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Deck[].setJobTask -- set jobTask for all ops in sub-pipeline               */
/*----------------------------------------------------------------------------*/
void Step::vDeckSetJobTask(void)
{
  ENTRY();

  Task* pJobTask = _pTask(m_oDataStack.pPopPtr(__func__));
  
  m_pLogicUnit->m_xpDeck->vSetJobTask(pJobTask);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
