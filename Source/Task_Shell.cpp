/*============================================================================*/
/* Task_Shell.cpp - task shell group                                          */
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

// shell: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Shell(void)
{
  vAddPrimitive("shell.execute",  &vShellExecute,  false);
}

/*----------------------------------------------------------------------------*/
/* shell.execute -- execute given shell command                               */
/*----------------------------------------------------------------------------*/
void Task::vShellExecute(NNP_Task nnpTask)
{
  ENTRY("vShellExecute");
  
  ValWsp xCommand;
  
  if ( ! nnpTask->m_xDataStack.bPop(xCommand) )
    THROW_INVALID_DSTACK("shell.execute");
  
  SysCall xSysCall;
  
  i8 iExitStatus = Shell::iExecute(xCommand.sStr(),xSysCall);
  
  nnpTask->m_xDataStack.vPush(iExitStatus);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
