/*============================================================================*/
/* Steps_ThreadList.cpp - thread list step group                              */
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

extern NP_Task         gnpBootTask;
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;

// threadlist: //////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_ThreadList(void)
{
  vAddPrimitive("threadList.dump"_Stz, &vThreadListDump,   false);
  vAddPrimitive("threadList.init"_Stz, &vThreadListInit,   false);
}

/*----------------------------------------------------------------------------*/
/* threadlist.dump -- dump the threadList on data stack                       */
/*----------------------------------------------------------------------------*/
void Task::vThreadListDump(MP_Task mpTask)
{
ENTRY("vThreadListDump"_Stz);
  
  Object* pPtrThreadList;
  if ( ! mpTask->m_pDataStack->bPop(pPtrThreadList) )
    THROW_INVALID_DSTACK("threadlist.dump"_Stz);
  
  ThreadList* pThreadList = _pThreadList(pPtrThreadList);
  ASSERT(CN_THREADLIST == pThreadList->eClass());

  pThreadList->vContentDump("ThreadList:"_Stz, aSingleTab());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* threadlist.init -- init of threadList on data stack                        */
/*----------------------------------------------------------------------------*/
void Task::vThreadListInit(MP_Task mpTask)
{
ENTRY("vThreadListInit"_Stz);
  
  Object* pPtrThreadList;
  if ( ! mpTask->m_pDataStack->bPop(pPtrThreadList) )
    THROW_INVALID_DSTACK("threadlist.init"_Stz);
  
  ThreadList* pThreadList = _pThreadList(pPtrThreadList);
  ASSERT(CN_THREADLIST == pThreadList->eClass());
  
  new(pThreadList) ThreadList;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
