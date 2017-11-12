/*============================================================================*/
/* Steps_Push.cpp - push step group                                           */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap*      gpAttachMap;
extern BufferPool*     gpBufferPool;
extern BPA*            gpBPA;
extern Dic*            gpMainDic;
extern Word*           gpWord;

extern Grid*           gpGrid;
extern StepArray*      gpStepArray;
extern ThreadList*     gpThreadList;
extern TaskList*       gpTaskList;

extern NP_Task         gnpBootTask;
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;

// push: ///////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Push(void)
{
  vAddPrimitive("i"_Stz,               &vPushI,            false);
  vAddPrimitive("j"_Stz,               &vPushJ,            false);
  vAddPrimitive("nullPtr"_Stz,         &vPushNullPtr,      false);
  vAddPrimitive("pushValue"_Stz,       &vPushValue,        false);
}

/*----------------------------------------------------------------------------*/
/* i -- copy loop index from return stack onto data stack                     */
/*----------------------------------------------------------------------------*/
void Task::vPushI(MP_Task mpTask)
{
  ENTRY("vPushI"_Stz);
  
  Value* pI = mpTask->m_pReturnStack->pPeekValue();
  
  mpTask->m_pDataStack->vPushValue(pI);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* j -- copy 2nd loop index from return stack onto data stack                 */
/*----------------------------------------------------------------------------*/
void Task::vPushJ(MP_Task mpTask)
{
  ENTRY("vPushJ"_Stz);
  
  Value* pI = mpTask->m_pReturnStack->pPeekValue(3);
  
  mpTask->m_pDataStack->vPushValue(pI);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* nullPtr -- push a nullptr                                                  */
/*----------------------------------------------------------------------------*/
void Task::vPushNullPtr(MP_Task mpTask)
{
  ENTRY("vPushNullPtr"_Stz);
  
  PtrVal xPtrVal;
  xPtrVal = (Object*)nullptr;
  
  mpTask->m_pDataStack->vPushValue(xPtrVal);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* pushValue -- push value arg onto stack                                     */
/*----------------------------------------------------------------------------*/
void Task::vPushValue(MP_Task mpTask)
{
  ENTRY("vPushValue"_Stz);
  
  if (mpTask->m_iTracing) {
    ASSERT(mpTask->m_iTracing);
  }
  
  Value* pValue = mpTask->IP.m_pValue;
  
  mpTask->m_pDataStack->vPushValue(pValue);
  
  mpTask->vAdjustNext(ValLib::iSize(pValue));
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
