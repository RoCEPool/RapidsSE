/*============================================================================*/
/* Steps_Vertex.cpp -- Vertex step group                                      */
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
void Step::vAddSteps_Vertex(void)
{
  vAddStep("Vertex[]"_Stz,                &Step::vVertexGet);
  vAddStep("Vertex[].set"_Stz,            &Step::vVertexSet);

  vAddStep("Vertex[].addAccum"_Stz,       &Step::vVertexAddAccum);
  vAddStep("Vertex[].cancelled@"_Stz,     &Step::vVertexCancelledFetch);
  vAddStep("Vertex[].cancelled!"_Stz,     &Step::vVertexCancelledStore);
  vAddStep("Vertex[].create"_Stz,         &Step::vVertexCreate);
  vAddStep("Vertex[].dropDebugLock"_Stz,  &Step::vVertexDropDebugLock);
  vAddStep("Vertex[].dump"_Stz,           &Step::vVertexDump);
  vAddStep("Vertex[].earlyExit@"_Stz,     &Step::vVertexEarlyExitFetch);
  vAddStep("Vertex[].earlyExit!"_Stz,     &Step::vVertexEarlyExitStore);
  vAddStep("Vertex[].enqueueInvoke?"_Stz, &Step::vVertexEnqueueInvoke);
  vAddStep("Vertex[].errored@"_Stz,       &Step::vVertexErroredFetch);
  vAddStep("Vertex[].errored!"_Stz,       &Step::vVertexErroredStore);
  vAddStep("Vertex[].getAccum"_Stz,       &Step::vVertexGetAccum);
  vAddStep("Vertex[].grabDebugLock"_Stz,  &Step::vVertexGrabDebugLock);
  vAddStep("Vertex[].op0@"_Stz,           &Step::vVertexOp0Fetch);
  vAddStep("Vertex[].opsOnNode@"_Stz,     &Step::vVertexOpsOnNodeFetch);
  vAddStep("Vertex[].pulsed@"_Stz,        &Step::vVertexPulsedFetch);
  vAddStep("Vertex[].pulsed!"_Stz,        &Step::vVertexPulsedStore);
  vAddStep("Vertex[].setInvoker"_Stz,     &Step::vVertexSetInvoker);
  vAddStep("Vertex[].spawnOpTasks"_Stz,   &Step::vVertexSpawnOpTasks);
}

/*----------------------------------------------------------------------------*/
/* Vertex[].addAccum -- add to an accumulator in this pipeline                */
/*----------------------------------------------------------------------------*/
void Step::vVertexAddAccum(void)
{
  ENTRY();
    
  i8 i_Accum = m_pDataStack->iPopInt(__func__);
  
  i8 iValue  = m_pDataStack->iPopInt(__func__);
  
  // m_pLogicUnit->m_xpVertex->vAddAccum(i_Accum,iValue);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Vertex[].cancelled@ -- has this pipeline been cancelled?                   */
/*----------------------------------------------------------------------------*/
void Step::vVertexCancelledFetch(void)
{
  ENTRY();
  
  // return if pipeline has been cancelled
  // m_pDataStack->vPushInt(m_pLogicUnit->m_xpVertex->bCancelled());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Vertex[].cancelled! -- fetch this pipeline's cancelled indicator           */
/*----------------------------------------------------------------------------*/
void Step::vVertexCancelledStore(void)
{
  ENTRY();
  
  // m_pLogicUnit->m_xpVertex->vCancelled(m_pDataStack->iPopInt(__func__));
  
  EXIT();
}


/*----------------------------------------------------------------------------*/
/* Vertex[].create -- create the pipeline, stages, streams and ops            */
/*----------------------------------------------------------------------------*/
void Step::vVertexCreate(void)
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
/* Vertex[].dropDebugLock -- drop this pipeline's debug lock                  */
/*----------------------------------------------------------------------------*/
void Step::vVertexDropDebugLock(void)
{
  ENTRY();
    
  // m_pLogicUnit->m_xpVertex->vDropDebugLock();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Vertex[].dump -- dump this subpipeline only                                */
/*----------------------------------------------------------------------------*/
void Step::vVertexDump(void)
{
  ENTRY();
      
  SP_Pipeline spSubPipe;
  
  // vTrace("%s\n"_Stz,spSubPipe.sz(* m_pLogicUnit->m_xpVertex,true));

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Vertex[].earlyExit@ -- fetch this pipelines earlyExit indicator            */
/*----------------------------------------------------------------------------*/
void Step::vVertexEarlyExitFetch(void)
{
  ENTRY();

  // return if pipeline has been errored
  // m_pDataStack->vPushInt(m_pLogicUnit->m_xpVertex->bEarlyExit());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Vertex[].earlyExit! -- store this pipeline's earlyExit indicator           */
/*----------------------------------------------------------------------------*/
void Step::vVertexEarlyExitStore(void)
{
  ENTRY();

  // m_pLogicUnit->m_xpVertex->vEarlyExit(m_pDataStack->iPopIntBol(__func__));

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Vertex[].enqueueInvoke? -- test for enquing an invoke marker               */
/*----------------------------------------------------------------------------*/
void Step::vVertexEnqueueInvoke(void)
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
/* Vertex[].errored@ -- fetch this pipeline errored indicator                 */
/*----------------------------------------------------------------------------*/
void Step::vVertexErroredFetch(void)
{
  ENTRY();
    
  // return if pipeline has been errored
  // m_pDataStack->vPushInt(m_pLogicUnit->m_xpVertex->bErrored());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Vertex[].errored! -- store this pipeline errored indicator                 */
/*----------------------------------------------------------------------------*/
void Step::vVertexErroredStore(void)
{
  ENTRY();
  
  //m_pLogicUnit->m_xpVertex->vErrored(m_pDataStack->iPopBol(__func__));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Vertex[].get -- get subpipeline                                            */
/*----------------------------------------------------------------------------*/
void Step::vVertexGet(void)
{
  ENTRY();
   
  if (m_pLogicUnit->m_xpVertex.bIsNull())
    m_pDataStack->vPushPtr(nullptr);
  else
    m_pDataStack->vPushPtr(* m_pLogicUnit->m_xpVertex);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Vertex[].getAccum -- get an accumulator from this pipeline                 */
/*----------------------------------------------------------------------------*/
void Step::vVertexGetAccum(void)
{
  ENTRY();
   
  i8 i_Accum = m_pDataStack->iPopInt(__func__);
  
  i8 iValue = 0; //m_pLogicUnit->m_xpVertex->iGetAccum(i_Accum);
  
  m_pDataStack->vPushInt(iValue);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Vertex[].grabDebugLock -- grab this pipeline's debug lock                  */
/*----------------------------------------------------------------------------*/
void Step::vVertexGrabDebugLock(void)
{
  ENTRY();
    
  // m_pLogicUnit->m_xpVertex->vGrabDebugLock();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Vertex[].pulsed@ -- fetch this pipeline's pulsed indicator                 */
/*----------------------------------------------------------------------------*/
void Step::vVertexPulsedFetch(void)
{
  ENTRY();
    
  // return if pipeline has been pulsed
  // m_pDataStack->vPushInt(m_pLogicUnit->m_xpVertex->bPulsed());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Vertex[].pulsed! -- store this pipeline's pulsed indicator                */
/*----------------------------------------------------------------------------*/
void Step::vVertexPulsedStore(void)
{
  ENTRY();
    
  // m_pLogicUnit->m_xpVertex->vPulsed(m_pDataStack->iPopBol(__func__));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Vertex[].op0@ -- fetch op0 of this pipeline                                */
/*----------------------------------------------------------------------------*/
void Step::vVertexOp0Fetch(void)
{
  ENTRY();
    
  Op* pOp0 = m_pLogicUnit->m_xpVertex->pOp0();
    
  m_pDataStack->vPushPtr(pOp0);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Vertex[].opsOnNode@ -- get no of ops executing on this node                */
/*----------------------------------------------------------------------------*/
void Step::vVertexOpsOnNodeFetch(void)
{
  ENTRY();
    
  i8 iThisNode = AM::pGrid()->iThisNodeNo();

  i8 nOps = m_pLogicUnit->m_xpVertex->nOpsOnNode(iThisNode);

  m_pDataStack->vPushInt(nOps);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Vertex[].set -- set subpipeline                                            */
/*----------------------------------------------------------------------------*/
void Step::vVertexSet(void)
{
  ENTRY();
  
  m_pLogicUnit->m_xpVertex =
    _pVertex(m_pDataStack->pPopPtr(__func__));

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Vertex[].setInvoker -- set invoker for all tasks in sub-pipeline           */
/*----------------------------------------------------------------------------*/
void Step::vVertexSetInvoker(void)
{
  ENTRY();

  Task* pInvoker = _pTask(m_pDataStack->pPopPtr(__func__));
  
  m_pLogicUnit->m_xpVertex->vSetInvoker(pInvoker);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Vertex[].spawnOpTasks -- spawn tasks for all ops of pipeline for this node */
/*----------------------------------------------------------------------------*/
void Step::vVertexSpawnOpTasks(void)
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
