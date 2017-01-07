/*============================================================================*/
/* Task_Docol.cpp - task docol group                                          */
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

// docol: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Docol(void)
{
  vAddPrimitive("]",                     &vDocolCompiling,      false);
  vAddPrimitive("[",                     &vDocolInterpreting,   true );
  vAddPrimitive("docol.isCompiling",     &vDocolIsCompiling,    false);
  vAddPrimitive("docol.isInterpreting",  &vDocolIsInterpreting, false);
}

/*----------------------------------------------------------------------------*/
/* ] -- enter compiler state                                                  */
/*----------------------------------------------------------------------------*/
void Task::vDocolCompiling(NNP_Task nnpTask)
{
  ENTRY("vDocolCompiling");
  
  nnpTask->m_bCompiling = true;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [ -- enter interpreter state                                               */
/*----------------------------------------------------------------------------*/
void Task::vDocolInterpreting(NNP_Task nnpTask)
{
  ENTRY("vDocolInterpreting");
  
  nnpTask->m_bCompiling = false;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* docol.isCompiling -- currently in compiling mode?                          */
/*----------------------------------------------------------------------------*/
void Task::vDocolIsCompiling(NNP_Task nnpTask)
{
  ENTRY("vDocolIsCompiling");
  
  i8 iIsCompiling = nnpTask->m_bCompiling ? 1 : 0;
  
  nnpTask->m_xDataStack.vPush(iIsCompiling);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* docol.isInterpreting -- currently in interpreting mode?                    */
/*----------------------------------------------------------------------------*/
void Task::vDocolIsInterpreting(NNP_Task nnpTask)
{
  ENTRY("vDocolIsInterpreting");
  
  i8 iIsInterpreting = nnpTask->m_bCompiling ? 0 : 1;
  
  nnpTask->m_xDataStack.vPush(iIsInterpreting);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
