/*============================================================================*/
/* Steps_Process.cpp - process step group                                     */
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

// process: ////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Process(void)
{
  vAddPrimitive("process.getPid"_Str,  &vProcessGetPid,    false);
  vAddPrimitive("process.quit"_Str,    &vProcessQuit,      false);
  vAddPrimitive("process.testPid"_Str, &vProcessTestPid,   false);
}

/*----------------------------------------------------------------------------*/
/* process.getpid -- get pid of this process                                  */
/*----------------------------------------------------------------------------*/
void Task::vProcessGetPid(NNP_Task nnpTask)
{
ENTRY("vProcessGetPid");
  
  i8 iPid = Process::iGetPid();
  
  nnpTask->m_pDataStack->vPush(iPid);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* process.quit -- process exits early                                        */
/*----------------------------------------------------------------------------*/
void Task::vProcessQuit(NNP_Task nnpTask)
{
  ENTRY("vProcessQuit");
  
  exit(EXIT_FAILURE);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* process.testpid -- test pid on stack                                       */
/*----------------------------------------------------------------------------*/
void Task::vProcessTestPid(NNP_Task nnpTask)
{
ENTRY("vProcessTestPid");

  i8 iPid;
  if ( ! nnpTask->m_pDataStack->bPop(iPid) )
    THROW_INVALID_DSTACK("process.testpid");
  
  i8 iExists = Process::bTestPid(iPid);
  
  nnpTask->m_pDataStack->vPush(iExists);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
