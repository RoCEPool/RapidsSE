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
  vAddStep("job_repeater.chunk"_Stz,     &Step::vJob_RepeaterChunk);
}

/*----------------------------------------------------------------------------*/
/* job_repeater.chunk -- indefinietly copy file into pipe                    */
/*----------------------------------------------------------------------------*/
void Step::vJob_RepeaterChunk(MP_Task mpTask)
{
  ENTRY("vJob_RepeaterChunk"_Stz);
  
  Object* pPtrJobRepeater;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJobRepeater) )
    THROW_INVALID_DSTACK("job_repeater.chunk"_Stz);
  Job_Repeater* pJob_Repeater = _pJob_Repeater(pPtrJobRepeater);
  
  bool bCallAgain = pJob_Repeater->bChunk();
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
