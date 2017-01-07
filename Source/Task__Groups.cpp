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

// zero: ///////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Zero(void)
{
  vAddPrimitive("0<",              &vZeroLess,         false);
  vAddPrimitive("0<=",             &vZeroLessEqual,    false);
  vAddPrimitive("0==",             &vZeroEqual,        false);
  vAddPrimitive("0!=",             &vZeroNotEqual,     false);
  vAddPrimitive("0>=",             &vZeroGreaterEqual, false);
  vAddPrimitive("0>",              &vZeroGreater,      false);
}

/*----------------------------------------------------------------------------*/
/* 0< -- test if top value is less than zero                                  */
/*----------------------------------------------------------------------------*/
void Task::vZeroLess(NNP_Task nnpTask)
{
  ENTRY("vZeroLess");
  
  i8 iZero;
  if ( ! nnpTask->m_xDataStack.bPopParamInt(iZero) )
    THROW_INVALID_DSTACK("0<");
  
  if (iZero < 0)
    nnpTask->m_xDataStack.vPush((i8)1);
  else
    nnpTask->m_xDataStack.vPush((i8)0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* 0<= -- test if top value is less than or equal to zero                     */
/*----------------------------------------------------------------------------*/
void Task::vZeroLessEqual(NNP_Task nnpTask)
{
  ENTRY("vZeroLessEqual");
  
  i8 iZero;
  if ( ! nnpTask->m_xDataStack.bPopParamInt(iZero) )
    THROW_INVALID_DSTACK("0<=");
  
  if (iZero <= 0)
    nnpTask->m_xDataStack.vPush((i8)1);
  else
    nnpTask->m_xDataStack.vPush((i8)0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* 0== -- test if top value is equal to zero / nullptr                        */
/*----------------------------------------------------------------------------*/
void Task::vZeroEqual(NNP_Task nnpTask)
{
  ENTRY("vZeroEqual");
  
  i8 iZero;
  if ( ! nnpTask->m_xDataStack.bPopParamInt(iZero) )
    THROW_INVALID_DSTACK("0==");
  
  if (iZero == 0)
    nnpTask->m_xDataStack.vPush((i8)1);
  else
    nnpTask->m_xDataStack.vPush((i8)0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* 0!= -- test if top value is not equal to zero                              */
/*----------------------------------------------------------------------------*/
void Task::vZeroNotEqual(NNP_Task nnpTask)
{
  ENTRY("vZeroNotEqual");
  
  i8 iZero;
  if ( ! nnpTask->m_xDataStack.bPopParamInt(iZero) )
    THROW_INVALID_DSTACK("0!=");
  
  if (iZero != 0)
    nnpTask->m_xDataStack.vPush((i8)1);
  else
    nnpTask->m_xDataStack.vPush((i8)0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* zeroGreaterEqual -- test if top value is greater than or equal to zero     */
/*----------------------------------------------------------------------------*/
void Task::vZeroGreaterEqual(NNP_Task nnpTask)
{
  ENTRY("vZeroGreaterEqual");
  
  i8 iZero;
  if ( ! nnpTask->m_xDataStack.bPopParamInt(iZero) )
    THROW_INVALID_DSTACK("0>=");
  
  if (iZero >= 0)
    nnpTask->m_xDataStack.vPush((i8)1);
  else
    nnpTask->m_xDataStack.vPush((i8)0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* 0> -- test if top value is greater than zero                              */
/*----------------------------------------------------------------------------*/
void Task::vZeroGreater(NNP_Task nnpTask)
{
  ENTRY("vZeroGreater");
  
  i8 iZero;
  if ( ! nnpTask->m_xDataStack.bPopParamInt(iZero) )
    THROW_INVALID_DSTACK("0>");
  
  if (iZero > 0)
    nnpTask->m_xDataStack.vPush((i8)1);
  else
    nnpTask->m_xDataStack.vPush((i8)0);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
