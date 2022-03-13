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
  vAddStep("Dynamo[].delete"_Stz,  &Step::vDynamoDelete);
  vAddStep("Dynamo[].new"_Stz,     &Step::vDynamoNew);

  vAddStep("Dynamo::compileOnly"_Stz,  &Step::vDynamoCompileOnly);
  vAddStep("Dynamo::compileStmt"_Stz,  &Step::vDynamoCompileStmt);
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
/* Dynamo::compileOnly -- compile pipeline statement into a pipelines spec    */
/*----------------------------------------------------------------------------*/
void Step::vDynamoCompileOnly(void)
{
  ENTRY();
    
  s4K sPipelineStmt;
  m_pDataStack->vPopStr(sPipelineStmt,__func__);

  //Dynamo* pDynamo = new Dynamo(m_pJob_task->pJobTags());
  Dynamo* pDynamo = (* m_p_TaskLogicUnit->m_xpDynamo);

  PipelineSpec* pPipelineSpec = pDynamo->pCompile(sPipelineStmt);

  //delete pDynamo;

  if (pPipelineSpec) {
    m_pDataStack->vPushPtr(pPipelineSpec);
    m_pDataStack->vPushInt(EC_NONE);
  }
  else {
    m_pDataStack->vPushInt(EC_INVALID_COMMAND);

  }

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Dynamo::compileStmt -- compile pipeline statement into a pipelines spec    */
/*----------------------------------------------------------------------------*/
void Step::vDynamoCompileStmt(void)
{
  ENTRY();
    
  s4K sPipelineStmt;
  m_pDataStack->vPopStr(sPipelineStmt,__func__);
  
  //LogicUnit* pLogicUnit = m_p_TaskLogicUnit;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // use dynamo to compile the pipeline statement
  
  //Dynamo* pDynamo = new Dynamo(m_pJob_task->pJobTags());
  Dynamo* pDynamo = (* m_p_TaskLogicUnit->m_xpDynamo);

  PipelineSpec* pPipelineSpec = pDynamo->pCompile(sPipelineStmt);
  
  //bool bWasOptimised = pDynamo->bWasOptimised();
  
  
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
      
//      if (bWasOptimised) {
//        vTrace("vDynamoCompileStmt: was optimised\n"_Stz);
//        RASSERT(m_pJob_task->bIsOriginator());
//
//        //m_pJob_task->pJobTags()->vDumpSet(true);
//        //m_pDataStack->vDump(&m_oDicStack,"DataStack:"_Stz);
//        //m_pJob_task->vCmd(pDynamo->szOptPipelineStmt());
//        //m_pDataStack->vDump(&m_oDicStack,"DataStack:"_Stz);
//        //m_pJob_task->pJobTags()->vDumpSet(true);
//      }
      
      m_p_TaskLogicUnit->m_xpPipelineSpec = pPipelineSpec;
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
  
  //delete pDynamo;

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
