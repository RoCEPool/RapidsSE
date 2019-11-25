/*============================================================================*/
/* Steps_Olist.cpp -- Originator List step group                              */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Olist(void)
{
  vAddStep("olist::findJob"_Stz,   &Step::vOlistFindJob);
}

/*----------------------------------------------------------------------------*/
/* olist::findJob -- find job in originator list                              */
/*----------------------------------------------------------------------------*/
void Step::vOlistFindJob(void)
{
  ENTRY(); // ENTRY("vOlistFindJob"_Stz);
  
  i8 iJobId;
  if ( ! m_pDataStack->bPopAsInt(iJobId) )
    THROW_DSTACK();
  
  Job* pJobFound = Scheduler::pFindOriginatingJob(iJobId);
  RASSERT(pJobFound);
  
  
  //! borrow the job ref [ this needs to be done under lock ]
  pJobFound->vBorrowRefBy(&m_pRegSet->m_xpJobRef);
  
  
  m_pDataStack->vPushPtr(pJobFound);
  
  EXIT();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
