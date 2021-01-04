/*============================================================================*/
/* Steps_Plist.cpp -- Participant List step group                             */
/*============================================================================*/

#include "BASE.h"
#include "STORE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "StepTiming.hpp"

#include "Result.hpp"
#include "Job.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Plist(void)
{
  vAddStep("plist::findJob"_Stz,   &Step::vPlistFindJob);
}

/*----------------------------------------------------------------------------*/
/* olist::findJob -- find job in originator list                              */
/*----------------------------------------------------------------------------*/
void Step::vPlistFindJob(void)
{
  ENTRY();
  
  i8 iJobId = m_pDataStack->iPopAsInt(__func__);
  
  Job* pJobFound = Scheduler::pJob_FindParticipating(iJobId);
  RASSERT(pJobFound);
  
  
  //! borrow the job ref [ this needs to be done under lock ]
  pJobFound->vBorrowRefBy(&( m_pJob->pLogicUnit_job() )->m_xpJobRef);
  
  
  m_pDataStack->vPushPtr(pJobFound);
  
  EXIT();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
