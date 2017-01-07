/*============================================================================*/
/* Task_groups.cpp - task groups                                              */
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

// threadlist: //////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_ThreadList(void)
{
  vAddPrimitive("threadList.dump", &vThreadListDump,   false);
  vAddPrimitive("threadList.init", &vThreadListInit,   false);
}

/*----------------------------------------------------------------------------*/
/* threadlist.dump -- dump the threadList on param stack                      */
/*----------------------------------------------------------------------------*/
void Task::vThreadListDump(NNP_Task nnpTask)
{
ENTRY("vThreadListDump");
  
  Object* pPtrThreadList;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrThreadList) )
    THROW_INVALID_DSTACK("threadlist.dump");
  
  ThreadList* pThreadList = _pThreadList(pPtrThreadList);
  ASSERT(CN_THREADLIST == pThreadList->eClass());

  pThreadList->vDump("ThreadList:", pSingleTab());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* threadlist.init -- init of threadList on param stack                       */
/*----------------------------------------------------------------------------*/
void Task::vThreadListInit(NNP_Task nnpTask)
{
ENTRY("vThreadListInit");
  
  Object* pPtrThreadList;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrThreadList) )
    THROW_INVALID_DSTACK("threadlist.init");
  
  ThreadList* pThreadList = _pThreadList(pPtrThreadList);
  ASSERT(CN_THREADLIST == pThreadList->eClass());
  
  new(pThreadList) ThreadList;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
