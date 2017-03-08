/*============================================================================*/
/* Steps_Zero.cpp - zero step group                                           */
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
  vAddPrimitive("0<"_Str,              &vZeroLess,         false);
  vAddPrimitive("0<="_Str,             &vZeroLessEqual,    false);
  vAddPrimitive("0=="_Str,             &vZeroEqual,        false);
  vAddPrimitive("0!="_Str,             &vZeroNotEqual,     false);
  vAddPrimitive("0>="_Str,             &vZeroGreaterEqual, false);
  vAddPrimitive("0>"_Str,              &vZeroGreater,      false);
}

/*----------------------------------------------------------------------------*/
/* 0< -- test if top value is less than zero                                  */
/*----------------------------------------------------------------------------*/
void Task::vZeroLess(NNP_Task nnpTask)
{
  ENTRY("vZeroLess"_Str);
  
  i8 iZero;
  if ( ! nnpTask->m_pDataStack->bPopParamInt(iZero) )
    THROW_INVALID_DSTACK("0<"_Str);
  
  if (iZero < 0)
    nnpTask->m_pDataStack->vPush((i8)1);
  else
    nnpTask->m_pDataStack->vPush((i8)0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* 0<= -- test if top value is less than or equal to zero                     */
/*----------------------------------------------------------------------------*/
void Task::vZeroLessEqual(NNP_Task nnpTask)
{
  ENTRY("vZeroLessEqual"_Str);
  
  i8 iZero;
  if ( ! nnpTask->m_pDataStack->bPopParamInt(iZero) )
    THROW_INVALID_DSTACK("0<="_Str);
  
  if (iZero <= 0)
    nnpTask->m_pDataStack->vPush((i8)1);
  else
    nnpTask->m_pDataStack->vPush((i8)0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* 0== -- test if top value is equal to zero / nullptr                        */
/*----------------------------------------------------------------------------*/
void Task::vZeroEqual(NNP_Task nnpTask)
{
  ENTRY("vZeroEqual"_Str);
  
  i8 iZero;
  if ( ! nnpTask->m_pDataStack->bPopParamInt(iZero) )
    THROW_INVALID_DSTACK("0=="_Str);
  
  if (iZero == 0)
    nnpTask->m_pDataStack->vPush((i8)1);
  else
    nnpTask->m_pDataStack->vPush((i8)0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* 0!= -- test if top value is not equal to zero                              */
/*----------------------------------------------------------------------------*/
void Task::vZeroNotEqual(NNP_Task nnpTask)
{
  ENTRY("vZeroNotEqual"_Str);
  
  i8 iZero;
  if ( ! nnpTask->m_pDataStack->bPopParamInt(iZero) )
    THROW_INVALID_DSTACK("0!="_Str);
  
  if (iZero != 0)
    nnpTask->m_pDataStack->vPush((i8)1);
  else
    nnpTask->m_pDataStack->vPush((i8)0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* zeroGreaterEqual -- test if top value is greater than or equal to zero     */
/*----------------------------------------------------------------------------*/
void Task::vZeroGreaterEqual(NNP_Task nnpTask)
{
  ENTRY("vZeroGreaterEqual"_Str);
  
  i8 iZero;
  if ( ! nnpTask->m_pDataStack->bPopParamInt(iZero) )
    THROW_INVALID_DSTACK("0>="_Str);
  
  if (iZero >= 0)
    nnpTask->m_pDataStack->vPush((i8)1);
  else
    nnpTask->m_pDataStack->vPush((i8)0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* 0> -- test if top value is greater than zero                              */
/*----------------------------------------------------------------------------*/
void Task::vZeroGreater(NNP_Task nnpTask)
{
  ENTRY("vZeroGreater"_Str);
  
  i8 iZero;
  if ( ! nnpTask->m_pDataStack->bPopParamInt(iZero) )
    THROW_INVALID_DSTACK("0>"_Str);
  
  if (iZero > 0)
    nnpTask->m_pDataStack->vPush((i8)1);
  else
    nnpTask->m_pDataStack->vPush((i8)0);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
