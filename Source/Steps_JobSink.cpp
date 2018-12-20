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
  vAddStep("job_sink.sink"_Stz,     &Step::vJob_SinkSink);
}

/*----------------------------------------------------------------------------*/
/* job_sink.sink -- sink data from pipe                                       */
/*----------------------------------------------------------------------------*/
void Step::vJob_SinkSink(MP_Task mpTask)
{
  ENTRY("vJob_SinkSink"_Stz);
  
  Object* pPtrJobSink;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJobSink) )
    THROW_INVALID_DSTACK("job_sink.sink"_Stz);
  Job_Sink* pJob_Sink = _pJob_Sink(pPtrJobSink);
  
  pJob_Sink->vSink();
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
