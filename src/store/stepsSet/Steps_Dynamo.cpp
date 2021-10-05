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
  vAddStep("Dynamo::compileStmt"_Stz,  &Step::vDynamoCompileStmt);
}


/*----------------------------------------------------------------------------*/
/* Dynamo::compileStmt -- compile pipeline statement into a pipelines spec    */
/*----------------------------------------------------------------------------*/
void Step::vDynamoCompileStmt(void)
{
  ENTRY();
    
  sSTMT sPipelineStmt;
  m_pDataStack->vPopStr(sPipelineStmt,__func__);
  
  LogicUnit* pLogicUnit = m_p_TaskLogicUnit;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // use dynamo to compile the pipeline statement
  
  Dynamo* pDynamo = new Dynamo(sPipelineStmt,m_pJob_task->pJobTags());
    
  PipelineSpec* pPipelineSpec = pDynamo->pCompile();
  
  delete pDynamo;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // test compilation result
  
  if (pPipelineSpec) {
    
    eEC eError = AM::JOB::ePreExecuteTest(m_pJob_task);
    
    if (EC_NONE == eError) {
      
      // a) clean compile, passes pre-execution requirments
      
      if (AM::TRACE::bVerbose(C_VB_PIPELINESPEC)) {
        
        SP_PipelineSpec spPipelineSpec(pPipelineSpec);
        vTraceM("%s\n"_Stz,spPipelineSpec.sz());
      }
      
      pLogicUnit->m_xpPipelineSpec = pPipelineSpec;
      m_pDataStack->vPushInt(EC_NONE);
      
    }
    else {
      
      // b) clean compile, but does not pass pre-execution requirments

      //delete pPipelineSpec;
      pPipelineSpec->vGuardedDelete(CN_PIPELINESPEC,&pPipelineSpec,
                                    CN_DYNAMO,this,(Stz*)__func__);
      m_pDataStack->vPushInt(eError);
      
    }

  }
  else {
    
    // c) failed to compile
    
    m_pDataStack->vPushInt(EC_INVALID_COMMAND);

  }
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
