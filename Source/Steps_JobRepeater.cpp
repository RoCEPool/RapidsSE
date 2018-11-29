/*============================================================================*/
/* Steps_JobRepeater.cpp - job_repeater step group                            */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_JobRepeater(void)
{
  vAddStep("job_repeater.repeat"_Stz,     &Step::vJob_RepeaterRepeat);
}

/*----------------------------------------------------------------------------*/
/* job_repeater.repeat -- indefinietly copy file into pipe                    */
/*----------------------------------------------------------------------------*/
void Step::vJob_RepeaterRepeat(MP_Task mpTask)
{
  ENTRY("vJob_RepeaterRepeat"_Stz);
  
  Object* pPtrJobRepeater;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJobRepeater) )
    THROW_INVALID_DSTACK("job_repeater.repeat"_Stz);
  Job_Repeater* pJob_Repeater = _pJob_Repeater(pPtrJobRepeater);
  
  pJob_Repeater->vRepeat();
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
