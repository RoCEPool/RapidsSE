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
  ENTRY(); // ENTRY("vPlistFindJob"_Stz);
  
  i8 iJobId;
  if ( ! m_pDataStack->bPopAsInt(iJobId) )
    THROW_DSTACK();
  
  Job* pJobFound = Scheduler::pFindParticipatingJob(iJobId);
  RASSERT(pJobFound);
  
  
  //! borrow the job ref [ this needs to be done under lock ]
  pJobFound->vBorrowRefBy(&m_pRegSet->m_xpJobRef);
  
  
  m_pDataStack->vPushPtr(pJobFound);
  
  EXIT();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
