/*============================================================================*/
/* Steps_ThreadList.cpp - thread list step group                              */
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
  vAddPrimitive("threadList.dump"_Str, &vThreadListDump,   false);
  vAddPrimitive("threadList.init"_Str, &vThreadListInit,   false);
}

/*----------------------------------------------------------------------------*/
/* threadlist.dump -- dump the threadList on data stack                       */
/*----------------------------------------------------------------------------*/
void Task::vThreadListDump(NNP_Task nnpTask)
{
ENTRY("vThreadListDump"_Str);
  
  Object* pPtrThreadList;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrThreadList) )
    THROW_INVALID_DSTACK("threadlist.dump");
  
  ThreadList* pThreadList = _pThreadList(pPtrThreadList);
  ASSERT(CN_THREADLIST == pThreadList->eClass());

  pThreadList->vContentDump("ThreadList:"_Str, aSingleTab());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* threadlist.init -- init of threadList on data stack                        */
/*----------------------------------------------------------------------------*/
void Task::vThreadListInit(NNP_Task nnpTask)
{
ENTRY("vThreadListInit"_Str);
  
  Object* pPtrThreadList;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrThreadList) )
    THROW_INVALID_DSTACK("threadlist.init");
  
  ThreadList* pThreadList = _pThreadList(pPtrThreadList);
  ASSERT(CN_THREADLIST == pThreadList->eClass());
  
  new(pThreadList) ThreadList;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
