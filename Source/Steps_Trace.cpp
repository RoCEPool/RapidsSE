/*============================================================================*/
/* Steps_Trace.cpp - trace step group                                         */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap*      gpAttachMap;
extern BPA*            gpBPA;
extern Dic*            gpMainDic;
extern Word*           gpWord;

extern Grid*           gpGrid;
extern StepArray*      gpStepArray;
extern ThreadList*     gpThreadList;
extern TaskList*       gpTaskList;

extern NP_Task         gnpBootTask;
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;
extern bool            gbTracepoint;

// trace: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Trace(void)
{
  vAddPrimitive("traceOff"_Stz,        &vTraceOff,         false);
  vAddPrimitive("traceOn"_Stz,         &vTraceOn,          false);
  vAddPrimitive("tracePoint"_Stz,      &vTracePoint,       false);
  vAddPrimitive("trace!"_Stz,          &vTraceSet,         false);
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
  
  if (gbTracepoint)
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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
