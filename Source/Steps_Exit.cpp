/*============================================================================*/
/* Steps_Exit.cpp - exit step group                                           */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

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
void Task::vExit(MP_Task mpTask)
{
  ENTRY("vExit"_Str);
  
  //vTrace("Exit: return stack %lld\n"_Str, mpTask->m_pReturnStack->iValues());
  
  if ( (mpTask->m_iTracing & C_TRACE_ROUTINES) )
    mpTask->vTracePoint("<"_Str);
  
  if (mpTask->m_pLocalsFrame &&
      mpTask->m_pLocalsFrame->pStackPtr() ==
      (void*)mpTask->m_pReturnStack->pTopValue())
    mpTask->vLocalsPop();
  
  Cell* pCell;
  if ( ! mpTask->m_pReturnStack->bPop(pCell) )
    THROW_INVALID_RSTACK("exit"_Str);
  
  mpTask->IP.m_pCell = pCell;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
