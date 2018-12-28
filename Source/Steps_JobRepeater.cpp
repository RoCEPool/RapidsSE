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
  vAddStep("job_repeater.begin"_Stz,   &Step::vJob_RepeaterBegin);
  vAddStep("job_repeater.chunk"_Stz,   &Step::vJob_RepeaterChunk);
  vAddStep("job_repeater.end"_Stz,     &Step::vJob_RepeaterEnd);
}

/*----------------------------------------------------------------------------*/
/* Job_Repeater.begin -- begin repeating                                      */
/*----------------------------------------------------------------------------*/
void Step::vJob_RepeaterBegin(MP_Task mpTask)
{
  ENTRY("vJob_RepeaterBegin"_Stz);
  
  Object* pPtrJob_Repeater;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJob_Repeater) )
    THROW_INVALID_DSTACK("Job_Repeater.begin"_Stz);
  Job_Repeater* pJob_Repeater = _pJob_Repeater(pPtrJob_Repeater);
  
  bool bBegun = pJob_Repeater->bBegin();
  
  mpTask->m_pDataStack->vPushInt((i8)bBegun);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_repeater.chunk -- indefinietly copy file into pipe                     */
/*----------------------------------------------------------------------------*/
void Step::vJob_RepeaterChunk(MP_Task mpTask)
{
  ENTRY("vJob_RepeaterChunk"_Stz);
  
  Object* pPtrJob_Repeater;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJob_Repeater) )
    THROW_INVALID_DSTACK("job_repeater.chunk"_Stz);
  Job_Repeater* pJob_Repeater = _pJob_Repeater(pPtrJob_Repeater);
  
  bool bCallAgain = pJob_Repeater->bChunk();
  
  mpTask->m_pDataStack->vPushInt((i8)bCallAgain);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_repeater.end -- end repeater                                           */
/*----------------------------------------------------------------------------*/
void Step::vJob_RepeaterEnd(MP_Task mpTask)
{
  ENTRY("vJob_RepeaterEnd"_Stz);
  
  Object* pPtrJob_Repeater;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJob_Repeater) )
    THROW_INVALID_DSTACK("job_repeater.end"_Stz);
  Job_Repeater* pJob_Repeater = _pJob_Repeater(pPtrJob_Repeater);
  
  pJob_Repeater->vEnd();
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
