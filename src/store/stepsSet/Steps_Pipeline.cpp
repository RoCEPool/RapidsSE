/*============================================================================*/
/* Steps_Pipeline.cpp -- Pipeline step group                                  */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "ePX.h"

#include "Op.hpp"
#include "Stream.hpp"
#include "Stage.hpp"
#include "Pipeline.hpp"
#include "Job.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Pipeline(void)
{
  vAddStep("pipeline::call"_Stz,    &Step::vPipelineCall);
  vAddStep("pipeline::spawn"_Stz,   &Step::vPipelineSpawn);

  vAddStep("pipeline.pipeout"_Stz,  &Step::vPipelinePipeout);
}

/*----------------------------------------------------------------------------*/
/* pipeline::call -- call nested pipeline                                     */
/*----------------------------------------------------------------------------*/
void Step::vPipelineCall(void)
{
  ENTRY();
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* pipeline::spawn -- spawn nested pipeline                                   */
/*----------------------------------------------------------------------------*/
void Step::vPipelineSpawn(void)
{
  ENTRY();
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* pipeline.pipeout -- append operand onto next operator in pipeline          */
/*----------------------------------------------------------------------------*/
void Step::vPipelinePipeout(void)
{
  ENTRY();

  Job* pPipelineJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  Operand* pOperand = _pOperand(m_pDataStack->pPopPtr(__func__));
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Pipeline* pPipeline = pPipelineJob->m_pPipeline;
  Stage*    pStage0   = pPipeline->pGet(0);
  Stream*   pStand0   = pStage0->pGet(0);
  Op*       pOp0      = pStand0->pGet(0);

  pOp0->vAppendLeft(pOperand,this);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
