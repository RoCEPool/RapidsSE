/*============================================================================*/
/* Steps_JobRepeater.cpp - job_repeater step group                            */
/*============================================================================*/

#include "Steps.h"

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
void Step::vJob_RepeaterBegin(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vJob_RepeaterBegin"_Stz);
  
  Object* pPtrJob_Repeater;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrJob_Repeater) )
    vThrow(__FILE__,__LINE__,FK_DSTACK,__STEP__);
  Job_Repeater* pJob_Repeater = _pJob_Repeater(pPtrJob_Repeater);
  
  bool bBegun = pJob_Repeater->bBegin();
  
  mpTask->m_pForth->m_pDataStack->vPushBool(bBegun);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_repeater.chunk -- indefinietly copy file into pipe                     */
/*----------------------------------------------------------------------------*/
void Step::vJob_RepeaterChunk(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vJob_RepeaterChunk"_Stz);
  
  Object* pPtrJob_Repeater;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrJob_Repeater) )
    vThrow(__FILE__,__LINE__,FK_DSTACK,__STEP__);
  Job_Repeater* pJob_Repeater = _pJob_Repeater(pPtrJob_Repeater);
  
  bool bCallAgain = pJob_Repeater->bChunk();
  
  mpTask->m_pForth->m_pDataStack->vPushBool(bCallAgain);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_repeater.end -- end repeater                                           */
/*----------------------------------------------------------------------------*/
void Step::vJob_RepeaterEnd(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vJob_RepeaterEnd"_Stz);
  
  Object* pPtrJob_Repeater;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrJob_Repeater) )
    vThrow(__FILE__,__LINE__,FK_DSTACK,__STEP__);
  Job_Repeater* pJob_Repeater = _pJob_Repeater(pPtrJob_Repeater);
  
  pJob_Repeater->vEnd();
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
