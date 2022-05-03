/*============================================================================*/
/* Steps_JobGeneric.cpp - generic job step group                              */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "Grid.hpp"
#include "Job.hpp"
#include "TagConstants.h"

#include "Api.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_JobGeneric(void)
{
  vAddStep("api::newGenericJob"_Stz,        &Step::vJob_BegunNew);
}

/*----------------------------------------------------------------------------*/
/* job_begun::new -- new a job_begun for this distributed 2pc job             */
/*----------------------------------------------------------------------------*/
void Step::vJob_BegunNew(void)
{
  ENTRY();
  
  s256 sEntryNm;
  m_pDataStack->vPopStr(sEntryNm,__func__);
    
  i8 iDistJobId = m_pDataStack->iPopInt(__func__);
  JobId tDistJobId;
  tDistJobId = iDistJobId;
  Job* pJobBegun = Api::pNewGenericJob(sEntryNm,tDistJobId);
  
  m_pDataStack->vPushPtr(pJobBegun);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
