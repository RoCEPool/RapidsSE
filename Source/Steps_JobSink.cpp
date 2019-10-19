/*============================================================================*/
/* Steps_JobSink.cpp - job_sink step group                                    */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_JobSink(void)
{
  vAddStep("job_sink.begin"_Stz,     &Step::vJob_SinkBegin);
  vAddStep("job_sink.chunk"_Stz,     &Step::vJob_SinkChunk);
  vAddStep("job_sink.end"_Stz,       &Step::vJob_SinkEnd);
}

/*----------------------------------------------------------------------------*/
/* Job_Sink.begin -- begin sink                                               */
/*----------------------------------------------------------------------------*/
void Step::vJob_SinkBegin(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vJob_SinkBegin"_Stz);
  
  Object* pPtrJob_Sink;
  if ( ! pForth->m_pDataStack->bPop(pPtrJob_Sink) )
    THROW_DSTACK();
  Job_Sink* pJob_Sink = _pJob_Sink(pPtrJob_Sink);
  
  bool bBegun = pJob_Sink->bBegin();
  
  pForth->m_pDataStack->vPushBool(bBegun);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_sink.chunk -- sink data from pipe                                      */
/*----------------------------------------------------------------------------*/
void Step::vJob_SinkChunk(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vJob_SinkChunk"_Stz);
  
  Object* pPtrJob_Sink;
  if ( ! pForth->m_pDataStack->bPop(pPtrJob_Sink) )
    THROW_DSTACK();
  Job_Sink* pJob_Sink = _pJob_Sink(pPtrJob_Sink);
  
  bool bCallAgain = pJob_Sink->bChunk();
  
  pForth->m_pDataStack->vPushBool(bCallAgain);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_sink.end -- end sink                                                   */
/*----------------------------------------------------------------------------*/
void Step::vJob_SinkEnd(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vJob_SinkEnd"_Stz);
  
  Object* pPtrJob_Sink;
  if ( ! pForth->m_pDataStack->bPop(pPtrJob_Sink) )
    THROW_DSTACK();
  Job_Sink* pJob_Sink = _pJob_Sink(pPtrJob_Sink);
  
  pJob_Sink->vEnd();
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
