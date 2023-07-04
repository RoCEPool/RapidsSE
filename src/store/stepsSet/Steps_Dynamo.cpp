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
void Step::vAddSteps_Dynamo(void)
{
  vAddStep("Dynamo[]"_Stz,          &Step::vDynamoGet);
  vAddStep("Dynamo[].code@"_Stz,    &Step::vDynamoCodeFetch);
  vAddStep("Dynamo[].compile"_Stz,  &Step::vDynamoCompile);
  vAddStep("Dynamo[].delete"_Stz,   &Step::vDynamoDelete);
  vAddStep("Dynamo[].get"_Stz,      &Step::vDynamoGet);
  vAddStep("Dynamo[].hasCode"_Stz,  &Step::vDynamoHasCode);
  vAddStep("Dynamo[].new"_Stz,      &Step::vDynamoNew);
}

/*----------------------------------------------------------------------------*/
/* Dynamo[].code@ -- fetch the code string                                    */
/*----------------------------------------------------------------------------*/
void Step::vDynamoCodeFetch(void)
{
  ENTRY();

  s48K szCode;

  Dynamo* pDynamo = (* m_pLogicUnit->m_xpDynamo);
  szCode = pDynamo->szCode();
  m_pDataStack->vPushStr(szCode);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Dynamo[].compile -- compile pipeline statement into a pipeline spec        */
/*----------------------------------------------------------------------------*/
void Step::vDynamoCompile(void)
{
  ENTRY();
    
  s48K sPipelineStmt;
  m_pDataStack->vPopStr(sPipelineStmt,__func__);
      
  Dynamo* pDynamo = (* m_pLogicUnit->m_xpDynamo);
  VertexSpec* pPipeSpec = pDynamo->pCompile(sPipelineStmt);
    
  if (pPipeSpec) {
    if (AM::TRACE::bVerbose(C_VB_PIPESPEC)) {
      SP_PipeSpec spPipeSpec(pPipeSpec);
      vTraceM("%s\n"_Stz,spPipeSpec.sz());
    }

//    if ( ! pDynamo->szCode()->bSame(""_Stz)) {
//      TinyC* pTinyC = new TinyC;
//      m_pLogicUnit->m_xpTinyC = pTinyC;
//      pTinyC->vBegin();
//      pTinyC->vAcquire(pTinyC,TinyC::vTinyError);
//      bool bClean = pTinyC->bCompile((char*)pDynamo->szCode());
//      RASSERT(bClean);
//    }

    m_pDataStack->vPushPtr(pPipeSpec);
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

  m_pLogicUnit->m_xpDynamo.vDeleteComponent();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Dynamo[].get -- get dynamo                                                 */
/*----------------------------------------------------------------------------*/
void Step::vDynamoGet(void)
{
  ENTRY();

  if (m_pLogicUnit->m_xpDynamo.bIsNotNull())
    m_pDataStack->vPushPtr(* m_pLogicUnit->m_xpDynamo);
  else
    m_pDataStack->vPushPtr(nullptr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Dynamo[].hasCode -- does the dynamo have code ?                            */
/*----------------------------------------------------------------------------*/
void Step::vDynamoHasCode(void)
{
  ENTRY();

  Dynamo* pDynamo = (* m_pLogicUnit->m_xpDynamo);
  if (pDynamo->szCode()->bSame(""_Stz))
    m_pDataStack->vPushInt(false);
  else
    m_pDataStack->vPushInt(true);
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Dynamo[].new -- new dynamo                                                 */
/*----------------------------------------------------------------------------*/
void Step::vDynamoNew(void)
{
  ENTRY();

  Job* pJob = MY::pJob();

  m_pLogicUnit->m_xpDynamo = new Dynamo(pJob->pJobTags());

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
