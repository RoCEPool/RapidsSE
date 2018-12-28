/*============================================================================*/
/* Steps_JobSink.cpp - job_sink step group                                    */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_JobSink(void)
{
  vAddStep("job_sink.chunk"_Stz,     &Step::vJob_SinkChunk);
}

/*----------------------------------------------------------------------------*/
/* job_sink.chunk -- sink data from pipe                                      */
/*----------------------------------------------------------------------------*/
void Step::vJob_SinkChunk(MP_Task mpTask)
{
  ENTRY("vJob_SinkChunk"_Stz);
  
  Object* pPtrJobSink;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJobSink) )
    THROW_INVALID_DSTACK("job_sink.chunk"_Stz);
  Job_Sink* pJob_Sink = _pJob_Sink(pPtrJobSink);
  
  bool bCallAgain = pJob_Sink->bChunk();
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
