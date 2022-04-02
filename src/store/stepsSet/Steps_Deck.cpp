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
  

//  vAddStep("Deck[].addAccum"_Stz,       &Step::vDeckAddAccum);
//  vAddStep("Deck[].cancelled@"_Stz,     &Step::vDeckCancelledFetch);
//  vAddStep("Deck[].cancelled!"_Stz,     &Step::vDeckCancelledStore);
//  vAddStep("Deck[].create"_Stz,         &Step::vDeckCreate);
//  vAddStep("Deck[].dropDebugLock"_Stz,  &Step::vDeckDropDebugLock);
//  vAddStep("Deck[].dump"_Stz,           &Step::vDeckDump);
//  vAddStep("Deck[].earlyExit@"_Stz,     &Step::vDeckEarlyExitFetch);
//  vAddStep("Deck[].earlyExit!"_Stz,     &Step::vDeckEarlyExitStore);
//  vAddStep("Deck[].errored@"_Stz,       &Step::vDeckErroredFetch);
//  vAddStep("Deck[].errored!"_Stz,       &Step::vDeckErroredStore);
//  vAddStep("Deck[].getAccum"_Stz,       &Step::vDeckGetAccum);
//  vAddStep("Deck[].grabDebugLock"_Stz,  &Step::vDeckGrabDebugLock);
//  vAddStep("Deck[].pulsed@"_Stz,        &Step::vDeckPulsedFetch);
//  vAddStep("Deck[].pulsed!"_Stz,        &Step::vDeckPulsedStore);
//  vAddStep("Deck[].spawnOpTasks"_Stz,   &Step::vDeckSpawnOpTasks);
}

/*----------------------------------------------------------------------------*/
/* Deck[].get -- get subpipeline                                            */
/*----------------------------------------------------------------------------*/
void Step::vDeckGet(void)
{
  ENTRY();
   
  if (m_pLogicUnit->m_xpVertex.bIsNull())
    m_pDataStack->vPushPtr(nullptr);
  else
    m_pDataStack->vPushPtr(* m_pLogicUnit->m_xpVertex);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Deck[].set -- set subpipeline                                            */
/*----------------------------------------------------------------------------*/
void Step::vDeckSet(void)
{
  ENTRY();
  
  m_pLogicUnit->m_xpVertex =
    _pVertex(m_pDataStack->pPopPtr(__func__));

  EXIT();
}



/*----------------------------------------------------------------------------*/
/* Deck[].enqueueInvoke? -- test for enquing an invoke marker               */
/*----------------------------------------------------------------------------*/
void Step::vDeckEnqueueInvoke(void)
{
  ENTRY();
  
  Vertex* pVortex = * m_pLogicUnit->m_xpVertex;

  Stage*    pStage0   = pVortex->pStage0();

  if (pStage0->bEnqueueInvoke()) {
    m_pDataStack->vPushInt(true);
  }
  else {
    m_pDataStack->vPushInt(false);
  }

  EXIT();
}


/*----------------------------------------------------------------------------*/
/* Deck[].op0@ -- fetch op0 of this pipeline                                */
/*----------------------------------------------------------------------------*/
void Step::vDeckOp0Fetch(void)
{
  ENTRY();
    
  Op* pOp0 = m_pLogicUnit->m_xpVertex->pOp0();
    
  m_pDataStack->vPushPtr(pOp0);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Deck[].opsOnNode@ -- get no of ops executing on this node                */
/*----------------------------------------------------------------------------*/
void Step::vDeckOpsOnNodeFetch(void)
{
  ENTRY();
    
  i8 iThisNode = AM::pGrid()->iThisNodeNo();

  i8 nOps = m_pLogicUnit->m_xpVertex->nOpsOnNode(iThisNode);

  m_pDataStack->vPushInt(nOps);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Deck[].setInvoker -- set invoker for all tasks in sub-pipeline           */
/*----------------------------------------------------------------------------*/
void Step::vDeckSetInvoker(void)
{
  ENTRY();

  Task* pInvoker = _pTask(m_pDataStack->pPopPtr(__func__));
  
  m_pLogicUnit->m_xpVertex->vSetInvoker(pInvoker);
  
  EXIT();
}








#if 0
/*----------------------------------------------------------------------------*/
/* Deck[].addAccum -- add to an accumulator in this pipeline                */
/*----------------------------------------------------------------------------*/
void Step::vDeckAddAccum(void)
{
  ENTRY();
    
  i8 i_Accum = m_pDataStack->iPopInt(__func__);
  
  i8 iValue  = m_pDataStack->iPopInt(__func__);
  
  // m_pLogicUnit->m_xpVertex->vAddAccum(i_Accum,iValue);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Deck[].cancelled@ -- has this pipeline been cancelled?                   */
/*----------------------------------------------------------------------------*/
void Step::vDeckCancelledFetch(void)
{
  ENTRY();
  
  // return if pipeline has been cancelled
  // m_pDataStack->vPushInt(m_pLogicUnit->m_xpVertex->bCancelled());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Deck[].cancelled! -- fetch this pipeline's cancelled indicator           */
/*----------------------------------------------------------------------------*/
void Step::vDeckCancelledStore(void)
{
  ENTRY();
  
  // m_pLogicUnit->m_xpVertex->vCancelled(m_pDataStack->iPopInt(__func__));
  
  EXIT();
}


/*----------------------------------------------------------------------------*/
/* Deck[].create -- create the pipeline, stages, streams and ops            */
/*----------------------------------------------------------------------------*/
void Step::vDeckCreate(void)
{
  ENTRY();
    
  i8 iThisNode = AM::pGrid()->iThisNodeNo();

  // get pipeline spec
  
  VertexSpec* pPipeSpec = _pVertexSpec(m_pDataStack->pPopPtr(__func__));
  
  // create subpipeline
  
  Pipeline* pSubPipeline =
    new Pipeline(m_pJob,pPipeSpec,m_pJob->iOriginatorNodeNo());
  
  // m_pLogicUnit->m_xpVertex = pSubPipeline;
    
  // dump pipeline
  
  if (AM::TRACE::bVerbose(C_VB_PIPELINE)) {
    if (iThisNode == 0) {
      SP_Pipeline spPipeline(pSubPipeline,true);
      vTraceM("%s\n"_Stz,spPipeline.sz());
      vDebugPoint();
    }
  }
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Deck[].dropDebugLock -- drop this pipeline's debug lock                  */
/*----------------------------------------------------------------------------*/
void Step::vDeckDropDebugLock(void)
{
  ENTRY();
    
  // m_pLogicUnit->m_xpVertex->vDropDebugLock();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Deck[].dump -- dump this subpipeline only                                */
/*----------------------------------------------------------------------------*/
void Step::vDeckDump(void)
{
  ENTRY();
      
  SP_Pipeline spSubPipe;
  
  // vTrace("%s\n"_Stz,spSubPipe.sz(* m_pLogicUnit->m_xpVertex,true));

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Deck[].earlyExit@ -- fetch this pipelines earlyExit indicator            */
/*----------------------------------------------------------------------------*/
void Step::vDeckEarlyExitFetch(void)
{
  ENTRY();

  // return if pipeline has been errored
  // m_pDataStack->vPushInt(m_pLogicUnit->m_xpVertex->bEarlyExit());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Deck[].earlyExit! -- store this pipeline's earlyExit indicator           */
/*----------------------------------------------------------------------------*/
void Step::vDeckEarlyExitStore(void)
{
  ENTRY();

  // m_pLogicUnit->m_xpVertex->vEarlyExit(m_pDataStack->iPopIntBol(__func__));

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Deck[].errored@ -- fetch this pipeline errored indicator                 */
/*----------------------------------------------------------------------------*/
void Step::vDeckErroredFetch(void)
{
  ENTRY();
    
  // return if pipeline has been errored
  // m_pDataStack->vPushInt(m_pLogicUnit->m_xpVertex->bErrored());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Deck[].errored! -- store this pipeline errored indicator                 */
/*----------------------------------------------------------------------------*/
void Step::vDeckErroredStore(void)
{
  ENTRY();
  
  //m_pLogicUnit->m_xpVertex->vErrored(m_pDataStack->iPopBol(__func__));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Deck[].getAccum -- get an accumulator from this pipeline                 */
/*----------------------------------------------------------------------------*/
void Step::vDeckGetAccum(void)
{
  ENTRY();
   
  i8 i_Accum = m_pDataStack->iPopInt(__func__);
  
  i8 iValue = 0; //m_pLogicUnit->m_xpVertex->iGetAccum(i_Accum);
  
  m_pDataStack->vPushInt(iValue);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Deck[].grabDebugLock -- grab this pipeline's debug lock                  */
/*----------------------------------------------------------------------------*/
void Step::vDeckGrabDebugLock(void)
{
  ENTRY();
    
  // m_pLogicUnit->m_xpVertex->vGrabDebugLock();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Deck[].pulsed@ -- fetch this pipeline's pulsed indicator                 */
/*----------------------------------------------------------------------------*/
void Step::vDeckPulsedFetch(void)
{
  ENTRY();
    
  // return if pipeline has been pulsed
  // m_pDataStack->vPushInt(m_pLogicUnit->m_xpVertex->bPulsed());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Deck[].pulsed! -- store this pipeline's pulsed indicator                */
/*----------------------------------------------------------------------------*/
void Step::vDeckPulsedStore(void)
{
  ENTRY();
    
  // m_pLogicUnit->m_xpVertex->vPulsed(m_pDataStack->iPopBol(__func__));
  
  EXIT();
}


/*----------------------------------------------------------------------------*/
/* Deck[].spawnOpTasks -- spawn tasks for all ops of pipeline for this node */
/*----------------------------------------------------------------------------*/
void Step::vDeckSpawnOpTasks(void)
{
  ENTRY();

  if (AM::TRACE::bVerbose(C_VB_PIPELINE)) {
    i8 iThisNodeNo = MY::iNodeNo();
    vTraceM("Spawning operator jobs for this node %lld\n"_Stz,iThisNodeNo);
  }
    
  m_pLogicUnit->m_xpVertex->vSpawnOpTasks();
  
  EXIT();
}

#endif

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
