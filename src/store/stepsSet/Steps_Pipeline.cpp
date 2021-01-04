/*============================================================================*/
/* Steps_Pipeline.cpp -- Pipeline step group                                  */
/*============================================================================*/

#include "BASE.h"
#include "STORE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "StepTiming.hpp"

#include "Pipeline.hpp"
#include "Response.hpp"
#include "Result.hpp"
#include "Job.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Pipeline(void)
{
  vAddStep("pipeline.pipeout"_Stz,  &Step::vPipelinePipeout);
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

  pOp0->vPipeOut_thisOp(pOperand,this);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
