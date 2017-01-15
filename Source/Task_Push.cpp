/*============================================================================*/
/* Task_Push.cpp - task push group                                            */
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

// push: ///////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Push(void)
{
  vAddPrimitive("i",               &vPushI,            false);
  vAddPrimitive("j",               &vPushJ,            false);
  vAddPrimitive("nullPtr",         &vPushNullPtr,      false);
  vAddPrimitive("pushValue",       &vPushValue,        false);
}

/*----------------------------------------------------------------------------*/
/* i -- copy loop index from return stack onto data stack                     */
/*----------------------------------------------------------------------------*/
void Task::vPushI(NNP_Task nnpTask)
{
  ENTRY("vPushI");
  
  Value* pI = nnpTask->m_pReturnStack->pPeek();
  
  nnpTask->m_pDataStack->vPush(pI);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* j -- copy 2nd loop index from return stack onto data stack                 */
/*----------------------------------------------------------------------------*/
void Task::vPushJ(NNP_Task nnpTask)
{
  ENTRY("vPushJ");
  
  Value* pI = nnpTask->m_pReturnStack->pPeek(3);
  
  nnpTask->m_pDataStack->vPush(pI);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* nullPtr -- push a nullptr                                                  */
/*----------------------------------------------------------------------------*/
void Task::vPushNullPtr(NNP_Task nnpTask)
{
  ENTRY("vPushNullPtr");
  
  PtrVal xPtrVal;
  xPtrVal = (Object*)nullptr;
  
  nnpTask->m_pDataStack->vPush(xPtrVal);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* pushValue -- push value arg onto stack                                     */
/*----------------------------------------------------------------------------*/
void Task::vPushValue(NNP_Task nnpTask)
{
  ENTRY("vPushValue");
  
  if (nnpTask->m_iTracing) {
    ASSERT(nnpTask->m_iTracing);
  }
  
  Value* pValue = nnpTask->IP.m_pValue;
  
  nnpTask->m_pDataStack->vPush(pValue);
  
  nnpTask->vAdjustNext(pValue->iSize());
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
