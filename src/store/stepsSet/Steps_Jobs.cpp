/*============================================================================*/
/* Steps_Job.cpp - job step group                                             */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "ePX.h"

#include "Grid.hpp"
#include "Api.hpp"
#include "Session.hpp"
#include "Op.hpp"
#include "Job.hpp"
#include "Node_I.hpp"
#include "LogicUnit.hpp"

#include "JobList.hpp"
#include "Sessions.hpp"

#include "TagConstants.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Jobs(void)
{
  vAddStep("jobs::seeDlist"_Stz,       &Step::vJobsSeeDlist);
  vAddStep("jobs::seeLlist"_Stz,       &Step::vJobsSeeLlist);
  vAddStep("jobs::seeOlist"_Stz,       &Step::vJobsSeeOlist);
  vAddStep("jobs::seePlist"_Stz,       &Step::vJobsSeePlist);
  vAddStep("jobs::seeRlist"_Stz,       &Step::vJobsSeeRlist);
  vAddStep("jobs::seeSqueue"_Stz,      &Step::vJobsSeeSqueue);
}

/*----------------------------------------------------------------------------*/
/* jobs::seeDlist -- see contents of m_pDemonJobs                             */
/*----------------------------------------------------------------------------*/
void Step::vJobsSeeDlist(void)
{
  ENTRY();
  
  AM::pDemonJobs()->vListJobs();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jobs::seeLlist -- see contents of m_pLocalJobs                             */
/*----------------------------------------------------------------------------*/
void Step::vJobsSeeLlist(void)
{
  ENTRY();
  
  AM::pLocalJobs()->vListJobs();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jobs::seeOlist -- see contents of m_pOriginatingJobs                       */
/*----------------------------------------------------------------------------*/
void Step::vJobsSeeOlist(void)
{
  ENTRY();
  
  AM::pOriginatingJobs()->vListJobs();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jobs::seePlist -- see contents of m_pParticipatingJobs                     */
/*----------------------------------------------------------------------------*/
void Step::vJobsSeePlist(void)
{
  ENTRY();
  
  AM::pParticipatingJobs()->vListJobs();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jobs::seeRlist -- see contents of m_pRetiredJobs                           */
/*----------------------------------------------------------------------------*/
void Step::vJobsSeeRlist(void)
{
  ENTRY();
  
  AM::pRetiredJobs()->vListJobs();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jobs::seeSqueue -- see contents of m_pSleepingQueue                        */
/*----------------------------------------------------------------------------*/
void Step::vJobsSeeSqueue(void)
{
  ENTRY();
  
  AM::pSchedule()->vDumpBidingSet();
  AM::pSchedule()->vDumpExecutingSet();
  AM::pSchedule()->vDumpReadyQueue();
  //AM::pSchedule()->vDumpReadyQueueLP();
  AM::pSchedule()->vDumpSleepingQueue();
  AM::pSchedule()->vDumpWaitingSet();

  AM::pSchedule()->vThreadsReport();

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

