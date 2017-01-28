/*============================================================================*/
/* Steps_Cast.cpp - cast step group                                           */
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

// cast: ///////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Cast(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  vAddPrimitive("(int)",           &vCastInt,          false);
  vAddPrimitive("(ptr)",           &vCastPtr,          false);
}

/*----------------------------------------------------------------------------*/
/* (int) -- cast ptr to int                                                   */
/*----------------------------------------------------------------------------*/
void Task::vCastInt(NNP_Task nnpTask)
{
  ENTRY("vCastInt");
  
  Object* pPtr;
  if ( ! nnpTask->m_pDataStack->bPop(pPtr) )
    THROW_INVALID_DSTACK("(int)");
  
  nnpTask->m_pDataStack->vPush((i8)pPtr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* (ptr) -- cast int to ptr                                                   */
/*----------------------------------------------------------------------------*/
void Task::vCastPtr(NNP_Task nnpTask)
{
  ENTRY("vCastPtr");
  
  i8 iInt;
  if ( ! nnpTask->m_pDataStack->bPop(iInt) )
    THROW_INVALID_DSTACK("(ptr)");
  
  nnpTask->m_pDataStack->vPush((Object*)iInt);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
