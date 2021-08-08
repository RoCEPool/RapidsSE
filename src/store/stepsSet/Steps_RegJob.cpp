/*============================================================================*/
/* Steps_RegJob.cpp - job[] step group                                        */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "Job.hpp"
#include "Op.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_RegJob(void)
{
  vAddStep("job[]"_Stz,      &Step::vRegJobGet);
}

/*----------------------------------------------------------------------------*/
/* job[].get -- get task                                                      */
/*----------------------------------------------------------------------------*/
void Step::vRegJobGet(void)
{
  ENTRY();
  
#ifdef WITH_NO_JOB_FOR_OP

  Job* pJob = nullptr;
  
  if (m_pJob.bIsNotNull()) {
    //RASSERT(! *m_pOp);
    pJob = *m_pJob;
  }
  else
  if (m_pOp.bIsNotNull()) {
    RASSERT(m_pJob.bIsNull());
    pJob = m_pOp->pPipelineJob();
  }
  else {
    NEVER_REACH;
  }
  
  m_pDataStack->vPushPtr(pJob);

#else

#endif
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
