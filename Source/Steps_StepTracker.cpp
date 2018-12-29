/*============================================================================*/
/* Steps_StepTracker.cpp - StepTracker step group                             */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_StepTracker(void)
{
  vAddStep("stepTracker::reset"_Stz, &Step::vStepTrackerReset);
  vAddStep("stepTracker::start"_Stz, &Step::vStepTrackerStart);
  vAddStep("stepTracker::write"_Stz, &Step::vStepTrackerWrite);
}

/*----------------------------------------------------------------------------*/
/* stepTracker::reset -- reset steps                                          */
/*----------------------------------------------------------------------------*/
void Step::vStepTrackerReset(MP_Task mpTask)
{
  ENTRY("vStepTrackerReset"_Stz);
  
#ifndef __ENTRY_EXIT
  
  vTrace("WARNING: __ENTRY_EXIT not defined\n"_Stz);
  return;
  
#else
  
  if (gpAttachMap->m_pStepTracker)
    gpAttachMap->m_pStepTracker->vReset();
  
#endif

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* stepTracker::start -- start step tracking                                  */
/*----------------------------------------------------------------------------*/
void Step::vStepTrackerStart(MP_Task mpTask)
{
  ENTRY("vStepTrackerStart"_Stz);
  
#ifndef __ENTRY_EXIT
  
  vTrace("WARNING: __ENTRY_EXIT not defined\n"_Stz);
  return;
  
#else
  
  if (! gpAttachMap->m_pStepTracker)
    gpAttachMap->m_pStepTracker = new StepTracker;
  
#endif

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* stepTracker::write -- write steps                                          */
/*----------------------------------------------------------------------------*/
void Step::vStepTrackerWrite(MP_Task mpTask)
{
  ENTRY("vStepTrackerWritevStepTrackerWrite"_Stz);

#ifndef __ENTRY_EXIT
  
  vTrace("WARNING: __ENTRY_EXIT not defined\n"_Stz);
  return;
  
#else
  
  if (gpAttachMap->m_pStepTracker)
    gpAttachMap->m_pStepTracker->vWrite();
  
#endif
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
