/*============================================================================*/
/* Steps_DeckOfOp.cpp -- Deck (of op) step group                              */
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
void Step::vAddSteps_DeckOfOp(void)
{
  vAddStep("[deck]"_Stz,                &Step::vDeckOfOpGet);
  vAddStep("[deck].set"_Stz,            &Step::vDeckOfOpSet);

  vAddStep("[deck].addAccum"_Stz,       &Step::vDeckOfOpAddAccum);
  vAddStep("[deck].cancelled@"_Stz,     &Step::vDeckOfOpCancelledFetch);
  vAddStep("[deck].cancelled!"_Stz,     &Step::vDeckOfOpCancelledStore);
  vAddStep("[deck].create"_Stz,         &Step::vDeckOfOpCreate);
  vAddStep("[deck].dropDebugLock"_Stz,  &Step::vDeckOfOpDropDebugLock);
  vAddStep("[deck].dump"_Stz,           &Step::vDeckOfOpDump);
  vAddStep("[deck].earlyExit@"_Stz,     &Step::vDeckOfOpEarlyExitFetch);
  vAddStep("[deck].earlyExit!"_Stz,     &Step::vDeckOfOpEarlyExitStore);
  vAddStep("[deck].enqueueInvoke?"_Stz, &Step::vDeckOfOpEnqueueInvoke);
  vAddStep("[deck].errored@"_Stz,       &Step::vDeckOfOpErroredFetch);
  vAddStep("[deck].errored!"_Stz,       &Step::vDeckOfOpErroredStore);
  vAddStep("[deck].getAccum"_Stz,       &Step::vDeckOfOpGetAccum);
  vAddStep("[deck].grabDebugLock"_Stz,  &Step::vDeckOfOpGrabDebugLock);
  vAddStep("[deck].op0@"_Stz,           &Step::vDeckOfOpOp0Fetch);
  vAddStep("[deck].opsOnNode@"_Stz,     &Step::vDeckOfOpOpsOnNodeFetch);
  vAddStep("[deck].pulsed@"_Stz,        &Step::vDeckOfOpPulsedFetch);
  vAddStep("[deck].pulsed!"_Stz,        &Step::vDeckOfOpPulsedStore);
  vAddStep("[deck].setInvoker"_Stz,     &Step::vDeckOfOpSetInvoker);
  vAddStep("[deck].spawnOpTasks"_Stz,   &Step::vDeckOfOpSpawnOpTasks);
}

/*----------------------------------------------------------------------------*/
/* [deck].addAccum -- add to an accumulator in this pipeline                */
/*----------------------------------------------------------------------------*/
void Step::vDeckOfOpAddAccum(void)
{
  ENTRY();
    
  i8 i_Accum = m_pDataStack->iPopInt(__func__);
  
  i8 iValue  = m_pDataStack->iPopInt(__func__);
  
  // m_pLogicUnit->m_xpVertex->vAddAccum(i_Accum,iValue);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [deck].cancelled@ -- has this pipeline been cancelled?                   */
/*----------------------------------------------------------------------------*/
void Step::vDeckOfOpCancelledFetch(void)
{
  ENTRY();
  
  // return if pipeline has been cancelled
  // m_pDataStack->vPushInt(m_pLogicUnit->m_xpVertex->bCancelled());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [deck].cancelled! -- fetch this pipeline's cancelled indicator           */
/*----------------------------------------------------------------------------*/
void Step::vDeckOfOpCancelledStore(void)
{
  ENTRY();
  
  // m_pLogicUnit->m_xpVertex->vCancelled(m_pDataStack->iPopInt(__func__));
  
  EXIT();
}


/*----------------------------------------------------------------------------*/
/* [deck].create -- create the pipeline, stages, streams and ops            */
/*----------------------------------------------------------------------------*/
void Step::vDeckOfOpCreate(void)
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
/* [deck].dropDebugLock -- drop this pipeline's debug lock                  */
/*----------------------------------------------------------------------------*/
void Step::vDeckOfOpDropDebugLock(void)
{
  ENTRY();
    
  // m_pLogicUnit->m_xpVertex->vDropDebugLock();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [deck].dump -- dump this subpipeline only                                */
/*----------------------------------------------------------------------------*/
void Step::vDeckOfOpDump(void)
{
  ENTRY();
      
  SP_Pipeline spSubPipe;
  
  // vTrace("%s\n"_Stz,spSubPipe.sz(* m_pLogicUnit->m_xpVertex,true));

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [deck].earlyExit@ -- fetch this pipelines earlyExit indicator            */
/*----------------------------------------------------------------------------*/
void Step::vDeckOfOpEarlyExitFetch(void)
{
  ENTRY();

  // return if pipeline has been errored
  // m_pDataStack->vPushInt(m_pLogicUnit->m_xpVertex->bEarlyExit());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [deck].earlyExit! -- store this pipeline's earlyExit indicator           */
/*----------------------------------------------------------------------------*/
void Step::vDeckOfOpEarlyExitStore(void)
{
  ENTRY();

  // m_pLogicUnit->m_xpVertex->vEarlyExit(m_pDataStack->iPopIntBol(__func__));

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [deck].enqueueInvoke? -- test for enquing an invoke marker               */
/*----------------------------------------------------------------------------*/
void Step::vDeckOfOpEnqueueInvoke(void)
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
/* [deck].errored@ -- fetch this pipeline errored indicator                 */
/*----------------------------------------------------------------------------*/
void Step::vDeckOfOpErroredFetch(void)
{
  ENTRY();
    
  // return if pipeline has been errored
  // m_pDataStack->vPushInt(m_pLogicUnit->m_xpVertex->bErrored());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [deck].errored! -- store this pipeline errored indicator                 */
/*----------------------------------------------------------------------------*/
void Step::vDeckOfOpErroredStore(void)
{
  ENTRY();
  
  //m_pLogicUnit->m_xpVertex->vErrored(m_pDataStack->iPopBol(__func__));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [deck].get -- get subpipeline                                            */
/*----------------------------------------------------------------------------*/
void Step::vDeckOfOpGet(void)
{
  ENTRY();
   
  if (m_pLogicUnit->m_xpVertex.bIsNull())
    m_pDataStack->vPushPtr(nullptr);
  else
    m_pDataStack->vPushPtr(* m_pLogicUnit->m_xpVertex);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [deck].getAccum -- get an accumulator from this pipeline                 */
/*----------------------------------------------------------------------------*/
void Step::vDeckOfOpGetAccum(void)
{
  ENTRY();
   
  i8 i_Accum = m_pDataStack->iPopInt(__func__);
  
  i8 iValue = 0; //m_pLogicUnit->m_xpVertex->iGetAccum(i_Accum);
  
  m_pDataStack->vPushInt(iValue);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [deck].grabDebugLock -- grab this pipeline's debug lock                  */
/*----------------------------------------------------------------------------*/
void Step::vDeckOfOpGrabDebugLock(void)
{
  ENTRY();
    
  // m_pLogicUnit->m_xpVertex->vGrabDebugLock();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [deck].pulsed@ -- fetch this pipeline's pulsed indicator                 */
/*----------------------------------------------------------------------------*/
void Step::vDeckOfOpPulsedFetch(void)
{
  ENTRY();
    
  // return if pipeline has been pulsed
  // m_pDataStack->vPushInt(m_pLogicUnit->m_xpVertex->bPulsed());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [deck].pulsed! -- store this pipeline's pulsed indicator                */
/*----------------------------------------------------------------------------*/
void Step::vDeckOfOpPulsedStore(void)
{
  ENTRY();
    
  // m_pLogicUnit->m_xpVertex->vPulsed(m_pDataStack->iPopBol(__func__));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [deck].op0@ -- fetch op0 of this pipeline                                */
/*----------------------------------------------------------------------------*/
void Step::vDeckOfOpOp0Fetch(void)
{
  ENTRY();
    
  Op* pOp0 = m_pLogicUnit->m_xpVertex->pOp0();
    
  m_pDataStack->vPushPtr(pOp0);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [deck].opsOnNode@ -- get no of ops executing on this node                */
/*----------------------------------------------------------------------------*/
void Step::vDeckOfOpOpsOnNodeFetch(void)
{
  ENTRY();
    
  i8 iThisNode = AM::pGrid()->iThisNodeNo();

  i8 nOps = m_pLogicUnit->m_xpVertex->nOpsOnNode(iThisNode);

  m_pDataStack->vPushInt(nOps);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [deck].set -- set subpipeline                                            */
/*----------------------------------------------------------------------------*/
void Step::vDeckOfOpSet(void)
{
  ENTRY();
  
  m_pLogicUnit->m_xpVertex =
    _pVertex(m_pDataStack->pPopPtr(__func__));

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [deck].setInvoker -- set invoker for all tasks in sub-pipeline           */
/*----------------------------------------------------------------------------*/
void Step::vDeckOfOpSetInvoker(void)
{
  ENTRY();

  Task* pInvoker = _pTask(m_pDataStack->pPopPtr(__func__));
  
  m_pLogicUnit->m_xpVertex->vSetInvoker(pInvoker);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [deck].spawnOpTasks -- spawn tasks for all ops of pipeline for this node */
/*----------------------------------------------------------------------------*/
void Step::vDeckOfOpSpawnOpTasks(void)
{
  ENTRY();

  if (AM::TRACE::bVerbose(C_VB_PIPELINE)) {
    i8 iThisNodeNo = MY::iNodeNo();
    vTraceM("Spawning operator jobs for this node %lld\n"_Stz,iThisNodeNo);
  }
    
  m_pLogicUnit->m_xpVertex->vSpawnOpTasks();
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
