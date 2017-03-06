/*============================================================================*/
/* Steps_Exit.cpp - exit step group                                           */
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

// exit: ///////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Exit(void)
{
  vAddPrimitive("exit"_Str,            &vExit,             false);
}

/*----------------------------------------------------------------------------*/
/* exit -- exit from routine                                                  */
/*----------------------------------------------------------------------------*/
void Task::vExit(NNP_Task nnpTask)
{
  ENTRY("vExit");
  
  //vTrace("Exit: return stack %lld\n", nnpTask->m_pReturnStack->iValues());
  
  if ( (nnpTask->m_iTracing & C_TRACE_ROUTINES) )
    nnpTask->vTracePoint("<");
  
  if (nnpTask->m_pLocalsFrame &&
      nnpTask->m_pLocalsFrame->pStackPtr() ==
      (void*)nnpTask->m_pReturnStack->pTopValue())
    nnpTask->vLocalsPop();
  
  Cell* pCell;
  if ( ! nnpTask->m_pReturnStack->bPop(pCell) )
    THROW_INVALID_RSTACK("exit");
  
  nnpTask->IP.m_pCell = pCell;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
