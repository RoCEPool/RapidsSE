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
  vAddPrimitive("traceOff",        &vTraceOff,         false);
  vAddPrimitive("traceOn",         &vTraceOn,          false);
  vAddPrimitive("tracePoint",      &vTracePoint,       false);
  vAddPrimitive("trace!",          &vTraceSet,         false);
}

/*----------------------------------------------------------------------------*/
/* turn tracing off                                                           */
/*----------------------------------------------------------------------------*/
void Task::vTraceOff(NNP_Task nnpTask)
{
  ENTRY("vTraceOff");
  
  nnpTask->m_iTracing = 0;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* turn tracing on                                                            */
/*----------------------------------------------------------------------------*/
void Task::vTraceOn(NNP_Task nnpTask)
{
  ENTRY("vTraceOn");
  
  nnpTask->m_iTracing = C_TRACE_STEPS;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* trace point                                                                */
/*----------------------------------------------------------------------------*/
void Task::vTracePoint(NNP_Task nnpTask)
{
  ENTRY("vTracePoint");
  
  s256 sPointName;
  
  if ( ! nnpTask->m_pDataStack->bPop(sPointName) )
    THROW_INVALID_DSTACK("tracePoint");
  
  if (gxArguments.m_bTracepoint)
    nnpTask->vTracePoint(sPointName.a());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* set tracing                                                                */
/*----------------------------------------------------------------------------*/
void Task::vTraceSet(NNP_Task nnpTask)
{
  ENTRY("vTraceSet");
  
  i8 iTrace;
  
  if ( ! nnpTask->m_pDataStack->bPop(iTrace) )
    THROW_INVALID_DSTACK("trace!");
  
  nnpTask->m_iTracing = iTrace;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
