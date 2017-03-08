/*============================================================================*/
/* Steps_Trace.cpp - trace step group                                         */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern Arguments       gxArguments;

extern AttachMap*      gpAttachMap;
extern BufferPool*     gpBufferPool;
extern Heap*           gpMediumHeap;
extern BPA*            gpBPA;
extern Dic*            gpMainDic;
extern Word*           gpWord;

extern Grid*           gpGrid;
extern StepArray*      gpStepArray;
extern ThreadList*     gpThreadList;
extern TaskList*       gpTaskList;
extern TxCollection*   gpTxCollection;

extern NP_Task         gnpRootTask;
extern StepBoard*      gpStepBoard;
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;

// trace: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Trace(void)
{
  vAddPrimitive("traceOff"_Str,        &vTraceOff,         false);
  vAddPrimitive("traceOn"_Str,         &vTraceOn,          false);
  vAddPrimitive("tracePoint"_Str,      &vTracePoint,       false);
  vAddPrimitive("trace!"_Str,          &vTraceSet,         false);
}

/*----------------------------------------------------------------------------*/
/* turn tracing off                                                           */
/*----------------------------------------------------------------------------*/
void Task::vTraceOff(NNP_Task nnpTask)
{
  ENTRY("vTraceOff"_Str);
  
  nnpTask->m_iTracing = 0;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* turn tracing on                                                            */
/*----------------------------------------------------------------------------*/
void Task::vTraceOn(NNP_Task nnpTask)
{
  ENTRY("vTraceOn"_Str);
  
  nnpTask->m_iTracing = C_TRACE_STEPS;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* trace point                                                                */
/*----------------------------------------------------------------------------*/
void Task::vTracePoint(NNP_Task nnpTask)
{
  ENTRY("vTracePoint"_Str);
  
  s256 sPointName;
  
  if ( ! nnpTask->m_pDataStack->bPop(sPointName) )
    THROW_INVALID_DSTACK("tracePoint"_Str);
  
  if (gxArguments.m_bTracepoint)
    nnpTask->vTracePoint(sPointName.aStr());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* set tracing                                                                */
/*----------------------------------------------------------------------------*/
void Task::vTraceSet(NNP_Task nnpTask)
{
  ENTRY("vTraceSet"_Str);
  
  i8 iTrace;
  
  if ( ! nnpTask->m_pDataStack->bPop(iTrace) )
    THROW_INVALID_DSTACK("trace!"_Str);
  
  nnpTask->m_iTracing = iTrace;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
