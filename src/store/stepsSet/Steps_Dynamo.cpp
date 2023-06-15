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
  vAddStep("Dynamo[].name@"_Stz,    &Step::vDynamoNameFetch);
  vAddStep("Dynamo[].new"_Stz,      &Step::vDynamoNew);
}

/*----------------------------------------------------------------------------*/
/* Dynamo[].code@ -- fetch the code string                                    */
/*----------------------------------------------------------------------------*/
void Step::vDynamoCodeFetch(void)
{
  ENTRY();

  s512K szCode;

  Dynamo* pDynamo = (* m_pLogicUnit->m_xpDynamo);
  if (pDynamo->szCode())
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

  Stz* szPipelineStmt = m_pJob->szCmdText();
  //vTraceM("%s\n"_Stz,szPipelineStmt);

  Dynamo* pDynamo = (* m_pLogicUnit->m_xpDynamo);
  VertexSpec* pPipeSpec = pDynamo->pCompile(szPipelineStmt);
    
  if (pPipeSpec) {
    if (AM::TRACE::bVerbose(C_VB_PIPESPEC)) {
      SP_PipeSpec spPipeSpec(pPipeSpec);
      vTraceM("%s\n"_Stz,spPipeSpec.sz());
    }

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
  if (pDynamo->szCode())
    m_pDataStack->vPushInt(false);
  else
    m_pDataStack->vPushInt(true);
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Dynamo[].name@ -- fetch the name of the dynamo pipeline                    */
/*----------------------------------------------------------------------------*/
void Step::vDynamoNameFetch(void)
{
  ENTRY();

  s4K szName;

  Dynamo* pDynamo = (* m_pLogicUnit->m_xpDynamo);
  szName = pDynamo->szName();
  m_pDataStack->vPushStr(szName);

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
