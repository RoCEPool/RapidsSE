/*============================================================================*/
/* Task_Logic.cpp - task logic group                                          */
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

// logic: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Logic(void)
{
  vAddPrimitive("and",             &vLogicAnd,         false);
  vAddPrimitive("not",             &vLogicNot,         false);
  vAddPrimitive("or",              &vLogicOr,          false);
}

/*----------------------------------------------------------------------------*/
/* and -- logical and of top two values                                       */
/*----------------------------------------------------------------------------*/
void Task::vLogicAnd(NNP_Task nnpTask)
{
  ENTRY("vLogicAnd");
  
  i8 iArgB;
  if ( ! nnpTask->m_pDataStack->bPopParamInt(iArgB) )
    THROW_INVALID_DSTACK("and");
  
  i8 iArgA;
  if ( ! nnpTask->m_pDataStack->bPopParamInt(iArgA) )
    THROW_INVALID_DSTACK("and");
  
  i8 iArgR = (iArgA && iArgB) ? 1 : 0;
  
  nnpTask->m_pDataStack->vPush(iArgR);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* not -- not of top value                                                    */
/*----------------------------------------------------------------------------*/
void Task::vLogicNot(NNP_Task nnpTask)
{
  ENTRY("vLogicNot");
  
  i8 iValA;
  if ( ! nnpTask->m_pDataStack->bPopParamInt(iValA) )
    THROW_INVALID_DSTACK("not");
  
  i8 iNotA = iValA ? 0 : 1;
  
  nnpTask->m_pDataStack->vPush(iNotA);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* or -- logical or of top two values                                         */
/*----------------------------------------------------------------------------*/
void Task::vLogicOr(NNP_Task nnpTask)
{
  ENTRY("vLogicOr");
  
  i8 iArgB;
  if ( ! nnpTask->m_pDataStack->bPopParamInt(iArgB) )
    THROW_INVALID_DSTACK("or");
  
  i8 iArgA;
  if ( ! nnpTask->m_pDataStack->bPopParamInt(iArgA) )
    THROW_INVALID_DSTACK("or");
  
  i8 iArgR = (iArgA || iArgB) ? 1 : 0;
  
  nnpTask->m_pDataStack->vPush(iArgR);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
