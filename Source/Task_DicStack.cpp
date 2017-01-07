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

// dicstack: ///////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_DicStack(void)
{
  vAddPrimitive("dicStack.push",   &vDicStackPush,     false);
}

/*----------------------------------------------------------------------------*/
/* dicStack.push -- push a dictionary from stack onto task dictionary stack   */
/*----------------------------------------------------------------------------*/
void Task::vDicStackPush(NNP_Task nnpTask)
{
  ENTRY("vDicStackPush");
  
  Object* pPtrDic;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrDic) )
    THROW_INVALID_DSTACK("dicStack.push");
  
  Dic* pDic = _pDic(pPtrDic);
  
  nnpTask->m_xDicStack.vPush(pDic);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
