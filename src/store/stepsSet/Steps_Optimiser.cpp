/*============================================================================*/
/* Steps_Optimiser.cpp -- Optimiser step group                                */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "Op.hpp"
#include "Job.hpp"

#include "LogicUnit.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Optimiser(void)
{
  vAddStep("Optimiser[].delete"_Stz ,        &Step::vOptimiserDelete);
  vAddStep("Optimiser[].new"_Stz,            &Step::vOptimiserNew);
  vAddStep("Optimiser[].optimise"_Stz,       &Step::vOptimiserOptimise);
  vAddStep("Optimiser[].pipelineStmt!"_Stz,  &Step::vOptimiserPipelineStmtStore);
  vAddStep("Optimiser[].weaved@"_Stz,        &Step::vOptimiserWeavedFetch);
}


/*----------------------------------------------------------------------------*/
/* Optimiser[].delete -- delete optimiser                                     */
/*----------------------------------------------------------------------------*/
void Step::vOptimiserDelete(void)
{
  ENTRY();
    

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Optimiser[].new -- new optimiser                                           */
/*----------------------------------------------------------------------------*/
void Step::vOptimiserNew(void)
{
  ENTRY();
    
  Job* pJob = nullptr;

  if (m_pJob_task.bIsNotNull()) {
    //RASSERT(! *m_pOp);
    pJob = *m_pJob_task;
  }
  else
  if (m_pOp.bIsNotNull()) {
    RASSERT(m_pJob_task.bIsNull());
    pJob = m_pOp->pPipelineJob();
  }
  else {
    NEVER_REACH;
  }

  m_p_TaskLogicUnit->m_xpOptimiser = new Optimiser(pJob->pJobTags());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Optimiser[].optimise -- optimise a pipeline statement                      */
/*----------------------------------------------------------------------------*/
void Step::vOptimiserOptimise(void)
{
  ENTRY();
    
  m_p_TaskLogicUnit->m_xpOptimiser.vDeleteComponent();


  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Optimiser[].pipelineStmtStore -- store unoptimised pipeline statement      */
/*----------------------------------------------------------------------------*/
void Step::vOptimiserPipelineStmtStore(void)
{
  ENTRY();
    

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Optimiser[].weavedFetch -- fetch optimised pipeline statement              */
/*----------------------------------------------------------------------------*/
void Step::vOptimiserWeavedFetch(void)
{
  ENTRY();
    

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
