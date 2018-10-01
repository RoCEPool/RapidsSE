/*============================================================================*/
/* Steps_Trace.cpp - trace step group                                         */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap*      gpAttachMap;

// trace: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddSteps_Trace(void)
{
  vAddStep("traceOff"_Stz,         &Task::vTraceOff,         false);
  vAddStep("traceOn"_Stz,          &Task::vTraceOn,          false);
  vAddStep("tracePoint"_Stz,       &Task::vTracePoint,       false);
  vAddStep("trace!"_Stz,           &Task::vTraceSet,         false);

  vAddStep("performancePoint"_Stz, &Task::vPerformancePoint, false);
  vAddStep("performancePrint"_Stz, &Task::vPerformancePrint, false);
}

/*----------------------------------------------------------------------------*/
/* turn tracing off                                                           */
/*----------------------------------------------------------------------------*/
void Task::vTraceOff(MP_Task mpTask)
{
  ENTRY("vTraceOff"_Stz);
  
  mpTask->m_iTracing = 0;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* turn tracing on                                                            */
/*----------------------------------------------------------------------------*/
void Task::vTraceOn(MP_Task mpTask)
{
  ENTRY("vTraceOn"_Stz);
  
  mpTask->m_iTracing = C_TRACE_STEPS;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* trace point                                                                */
/*----------------------------------------------------------------------------*/
void Task::vTracePoint(MP_Task mpTask)
{
  ENTRY("vTracePoint"_Stz);
  
  s256 sPointName;
  
  if ( ! mpTask->m_pDataStack->bPop(sPointName) )
    THROW_INVALID_DSTACK("tracePoint"_Stz);
  
  if (gpAttachMap->m_bTracepoint)
    mpTask->vTracePoint(sPointName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* set tracing                                                                */
/*----------------------------------------------------------------------------*/
void Task::vTraceSet(MP_Task mpTask)
{
  ENTRY("vTraceSet"_Stz);
  
  i8 iTrace;
  
  if ( ! mpTask->m_pDataStack->bPop(iTrace) )
    THROW_INVALID_DSTACK("trace!"_Stz);
  
  mpTask->m_iTracing = iTrace;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* performance point                                                          */
/*----------------------------------------------------------------------------*/
void Task::vPerformancePoint(MP_Task mpTask)
{
  ENTRY("vPerformancePoint"_Stz);
  
  s256 sWordName;
  if ( ! mpTask->m_pDataStack->bPop(sWordName) )
    THROW_INVALID_DSTACK("performancePoint"_Stz);
  
  s256 sPointNote;
  if ( ! mpTask->m_pDataStack->bPop(sPointNote) )
    THROW_INVALID_DSTACK("performancePoint"_Stz);
    
  //if (gpAttachMap->m_bPerformancepoint)
    mpTask->vPerformancePoint(sWordName,sPointNote);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* performance print                                                          */
/*----------------------------------------------------------------------------*/
void Task::vPerformancePrint(MP_Task mpTask)
{
  ENTRY("vPerformancePrint"_Stz);
  
  mpTask->vPerformancePrint();
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
