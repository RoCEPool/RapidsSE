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
void Step::vJob_RepeaterBegin(void)
{
  ENTRY("vJob_RepeaterBegin"_Stz);
  
  Object* pPtrJob_Repeater;
  if ( ! m_pDataStack->bPopObject(pPtrJob_Repeater) )
    THROW_DSTACK();
  Job_Repeater* pJob_Repeater = _pJob_Repeater(pPtrJob_Repeater);
  
  bool bBegun = pJob_Repeater->bBegin();
  
  m_pDataStack->vPushBool(bBegun);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_repeater.chunk -- indefinietly copy file into pipe                     */
/*----------------------------------------------------------------------------*/
void Step::vJob_RepeaterChunk(void)
{
  ENTRY("vJob_RepeaterChunk"_Stz);
  
  Object* pPtrJob_Repeater;
  if ( ! m_pDataStack->bPopObject(pPtrJob_Repeater) )
    THROW_DSTACK();
  Job_Repeater* pJob_Repeater = _pJob_Repeater(pPtrJob_Repeater);
  
  bool bCallAgain = pJob_Repeater->bChunk();
  
  m_pDataStack->vPushBool(bCallAgain);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_repeater.end -- end repeater                                           */
/*----------------------------------------------------------------------------*/
void Step::vJob_RepeaterEnd(void)
{
  ENTRY("vJob_RepeaterEnd"_Stz);
  
  Object* pPtrJob_Repeater;
  if ( ! m_pDataStack->bPopObject(pPtrJob_Repeater) )
    THROW_DSTACK();
  Job_Repeater* pJob_Repeater = _pJob_Repeater(pPtrJob_Repeater);
  
  pJob_Repeater->vEnd();
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
