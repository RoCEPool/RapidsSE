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
  vAddStep("Dynamo[].compile"_Stz,  &Step::vDynamoCompile);
  vAddStep("Dynamo[].delete"_Stz,   &Step::vDynamoDelete);
	vAddStep("Dynamo[].fileName@"_Stz,&Step::vDynamoFileNameFetch);
  vAddStep("Dynamo[].get"_Stz,      &Step::vDynamoGet);
  vAddStep("Dynamo[].name@"_Stz,    &Step::vDynamoNameFetch);
  vAddStep("Dynamo[].new"_Stz,      &Step::vDynamoNew);
}

/*----------------------------------------------------------------------------*/
/* Dynamo[].compile -- compile pipeline statement into a pipeline spec        */
/*----------------------------------------------------------------------------*/
void Step::vDynamoCompile(void)
{
  ENTRY();

  s1M sCmdText;
  b1 bResult = m_pJob->pCmdText()->bRetrieve(sCmdText);
  ASSERT(bResult);
  UNUSED(bResult);

  Stz* szPipelineStmt = sCmdText.szStz();
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
/* Dynamo[].fileName@ -- fetch the file name of the dynamo pipeline           */
/*----------------------------------------------------------------------------*/
void Step::vDynamoFileNameFetch(void)
{
	ENTRY();

	s4K sFileNm;

	Dynamo* pDynamo = (* m_pLogicUnit->m_xpDynamo);
	sFileNm.vSprint("%s_%s_%lld"_Stz,
	  pDynamo->szName(), AM::GRID::pGetThisNode()->szNodeNm(), AM::FILE::iNextFileNo());
	m_pDataStack->vPushStr(sFileNm);

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
