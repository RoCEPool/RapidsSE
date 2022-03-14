/*============================================================================*/
/* Steps_Dynamo.cpp -- Dynamo step group                                      */
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
#include "Pipeline.hpp"
#include "PipelineSpec.hpp"
#include "Job.hpp"
#include "Node_I.hpp"
#include "LogicUnit.hpp"

#include "Marker.hpp"

#include "SP_Pipeline.hpp"
#include "SP_PipelineSpec.hpp"

#include "Dynamo.hpp"
#include "TagConstants.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Dynamo(void)
{
  vAddStep("Dynamo[]"_Stz,          &Step::vDynamoGet);
  vAddStep("Dynamo[].compile"_Stz,  &Step::vDynamoCompile);
  vAddStep("Dynamo[].delete"_Stz,   &Step::vDynamoDelete);
  vAddStep("Dynamo[].get"_Stz,      &Step::vDynamoGet);
  vAddStep("Dynamo[].new"_Stz,      &Step::vDynamoNew);
}


/*----------------------------------------------------------------------------*/
/* Dynamo[].compile -- compile pipeline statement into a pipeline spec        */
/*----------------------------------------------------------------------------*/
void Step::vDynamoCompile(void)
{
  ENTRY();
    
  s4K sPipelineStmt;
  m_pDataStack->vPopStr(sPipelineStmt,__func__);
      
  Dynamo* pDynamo = (* m_p_TaskLogicUnit->m_xpDynamo);
  PipelineSpec* pPipelineSpec = pDynamo->pCompile(sPipelineStmt);
    
  if (pPipelineSpec) {
    if (AM::TRACE::bVerbose(C_VB_PIPELINESPEC)) {
      SP_PipelineSpec spPipelineSpec(pPipelineSpec);
      vTraceM("%s\n"_Stz,spPipelineSpec.sz());
    }
    
    m_pDataStack->vPushPtr(pPipelineSpec);
    m_pDataStack->vPushInt(EC_NONE);
  }
  else {
    m_pDataStack->vPushInt(EC_INVALID_COMMAND);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Dynamo[].delete -- delete dynamo                                           */
/*----------------------------------------------------------------------------*/
void Step::vDynamoDelete(void)
{
  ENTRY();

  m_p_TaskLogicUnit->m_xpDynamo.vDeleteComponent();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Dynamo[].get -- get dynamo                                                 */
/*----------------------------------------------------------------------------*/
void Step::vDynamoGet(void)
{
  ENTRY();

  if (m_p_TaskLogicUnit->m_xpDynamo.bIsNotNull())
    m_pDataStack->vPushPtr(* m_p_TaskLogicUnit->m_xpDynamo);
  else
    m_pDataStack->vPushPtr(nullptr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Dynamo[].new -- new dynamo                                                 */
/*----------------------------------------------------------------------------*/
void Step::vDynamoNew(void)
{
  ENTRY();
  
  m_p_TaskLogicUnit->m_xpDynamo = new Dynamo(m_pJob_task->pJobTags());

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
