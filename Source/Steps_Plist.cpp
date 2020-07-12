/*============================================================================*/
/* Steps_Plist.cpp -- Participant List step group                             */
/*============================================================================*/

#include "Steps.h"

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
  pJobFound->vBorrowRefBy(&m_pLogicUnit->m_xpJobRef);
  
  
  m_pDataStack->vPushPtr(pJobFound);
  
  EXIT();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
