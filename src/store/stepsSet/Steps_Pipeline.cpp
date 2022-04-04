/*============================================================================*/
/* Steps_Pipeline.cpp -- Pipeline step group                                  */
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
#include "Deck.hpp"
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
void Step::vAddSteps_Pipeline(void)
{
  vAddStep("Pipeline[]"_Stz,                &Step::vPipelineGet);
  vAddStep("Pipeline[].set"_Stz,            &Step::vPipelineSet);

  vAddStep("Pipeline[].create"_Stz,         &Step::vPipelineCreate);
  vAddStep("Pipeline[].dump"_Stz,           &Step::vPipelineDump);
  vAddStep("Pipeline[].rootDeck@"_Stz,      &Step::vPipelineRootDeckFetch);
  vAddStep("Pipeline[].spawnOpTasks"_Stz,   &Step::vPipelineSpawnOpTasks);
}

/*----------------------------------------------------------------------------*/
/* Pipeline[].create -- create the pipeline, stages, streams and ops          */
/*----------------------------------------------------------------------------*/
void Step::vPipelineCreate(void)
{
  ENTRY();
    
  i8 iThisNode = AM::pGrid()->iThisNodeId();

  // get pipeline spec
  
  VertexSpec* pPipeSpec = _pVertexSpec(m_pDataStack->pPopPtr(__func__));
  
  // create subpipeline
  
  Pipeline* pPipeline =
    new Pipeline(m_pJob,pPipeSpec,m_pJob->iOriginatorNodeId());
  
  m_pLogicUnit->m_xpPipeline = pPipeline;
    
  // dump pipeline
  
  if (AM::TRACE::bVerbose(C_VB_PIPELINE)) {
    if (iThisNode == 0) {
      SP_Pipeline spPipeline(pPipeline,true);
      vTraceM("%s\n"_Stz,spPipeline.sz());
      vDebugPoint();
    }
  }
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Pipeline[].dump -- dump this pipelines and its subpipelines                */
/*----------------------------------------------------------------------------*/
void Step::vPipelineDump(void)
{
  ENTRY();
      
  SP_Pipeline spPipeline;
  
  vTrace("%s\n"_Stz,spPipeline.sz(* m_pLogicUnit->m_xpPipeline,true));
         
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Pipeline[] -- get pipeline                                                 */
/*----------------------------------------------------------------------------*/
void Step::vPipelineGet(void)
{
  ENTRY();
   
  if (m_pLogicUnit->m_xpPipeline.bIsNull())
    m_pDataStack->vPushPtr(nullptr);
  else
    m_pDataStack->vPushPtr(* m_pLogicUnit->m_xpPipeline);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Pipeline[].rootDeck@ -- fetch root deck of pipeline                        */
/*----------------------------------------------------------------------------*/
void Step::vPipelineRootDeckFetch(void)
{
  ENTRY();
    
  Deck* pDeck = m_pLogicUnit->m_xpPipeline->pVertexRoot()->pDeck();
    
  m_pDataStack->vPushPtr(pDeck);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Pipeline[].set -- set pipeline                                             */
/*----------------------------------------------------------------------------*/
void Step::vPipelineSet(void)
{
  ENTRY();
  
  m_pLogicUnit->m_xpPipeline =
    _pPipeline(m_pDataStack->pPopPtr(__func__));

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Pipeline[].spawnOpTasks -- spawn tasks for all ops of pipeline for this node*/
/*----------------------------------------------------------------------------*/
void Step::vPipelineSpawnOpTasks(void)
{
  ENTRY();

  if (AM::TRACE::bVerbose(C_VB_PIPELINE)) {
    i8 iThisNodeId = MY::iNodeId();
    vTraceM("Spawning operator jobs for this node %lld\n"_Stz,iThisNodeId);
  }
    
  m_pLogicUnit->m_xpPipeline->vSpawnOpTasks();

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
