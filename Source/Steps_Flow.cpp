/*============================================================================*/
/* Steps_Flow.cpp - flow step group                                           */
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

extern NP_Task         gnpRootTask;
extern StepBoard*      gpStepBoard;
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;

// flow: ///////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Flow(void)
{
  vAddPrimitive("begin_"_Str,          &vFlowBegin_,       false);
  vAddPrimitive("execute"_Str,         &vFlowExecute,      false);
  vAddPrimitive("hazard"_Str,          &vFlowHazard,       false);
  vAddPrimitive("jump"_Str,            &vFlowJump,         false);
  vAddPrimitive("jumpBegin_"_Str,      &vFlowJumpBegin_,   false);
  vAddPrimitive("jumpFalse"_Str,       &vFlowJumpFalse,    false);
  vAddPrimitive("jumpTrue"_Str,        &vFlowJumpTrue,     false);
  vAddPrimitive("nop"_Str,             &vFlowNop,          false);
  vAddPrimitive("retryPoint"_Str,      &vFlowRetryPoint,   false);
  vAddPrimitive("test"_Str,            &vFlowTest,         false);
  vAddPrimitive("throw"_Str,           &vFlowThrow,        false);
}

/*----------------------------------------------------------------------------*/
/* begin_ -- begin of the docol loop : docol (used only in docol)   immediate */
/*----------------------------------------------------------------------------*/
void Task::vFlowBegin_(MP_Task mpTask)
{
  ENTRY("vBegin_"_Str);
  
  mpTask->m_pBeginIP = mpTask->IP.m_pCell;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* execute -- execute word on data stack                                      */
/*----------------------------------------------------------------------------*/
void Task::vFlowExecute(MP_Task mpTask)
{
  ENTRY("vFlowExecute"_Str);
  
  Object* pWordObject;
  if ( ! mpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("execute"_Str);
  Word* pWord = _pWord(pWordObject);
  
  if (WT_PRIMITIVE == pWord->eWordType()) {
    TaskStep pTaskStep = pWord->pEntryStep();
    (pTaskStep)(mpTask);
  }
  else if (WT_OBJECT == pWord->eWordType()) {
    mpTask->m_pReturnStack->vPushCell(mpTask->IP.m_pCell);
    mpTask->IP.m_pCell = pWord->pEntryCell();
    
    Object* pObject = (Object*)pWord->pFirstCell();
    mpTask->m_pDataStack->vPushObject(pObject);
  }
  else if (WT_ROUTINE == pWord->eWordType()){
    mpTask->m_pReturnStack->vPushCell(mpTask->IP.m_pCell);
    mpTask->IP.m_pCell = pWord->pEntryCell();
  }
  else {
    ASSERT(0);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* hazard -- should never be called                                           */
/*----------------------------------------------------------------------------*/
void Task::vFlowHazard(MP_Task mpTask)
{
  ENTRY("vFlowHazard"_Str);
  
  THROW("Entered hazard"_Str);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jump -- jump to location                                                   */
/*----------------------------------------------------------------------------*/
void Task::vFlowJump(MP_Task mpTask)
{
  ENTRY("vJump"_Str);
  
  mpTask->IP.m_pCell = mpTask->IP.m_pCell->m_pJumpTo;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jumpBegin_ -- jump to begin_ (used only in : docol)              immediate */
/*----------------------------------------------------------------------------*/
void Task::vFlowJumpBegin_(MP_Task mpTask)
{
  ENTRY("vFlowJumpBegin_"_Str);
  
  mpTask->IP.m_pCell = mpTask->m_pBeginIP;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jumpFalse -- jump on false to location                                     */
/*----------------------------------------------------------------------------*/
void Task::vFlowJumpFalse(MP_Task mpTask)
{
  ENTRY("vJumpFalse"_Str);
  
  i8 iVal;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iVal) )
    THROW_INVALID_DSTACK("jumpFalse"_Str);
  
  // n.b. the IP points to the jump arg
  
  if (iVal)
    // do not jump, skip over argument
    mpTask->IP.m_pCell++;
  else
    // jump to location
    mpTask->IP.m_pCell = mpTask->IP.m_pCell->m_pJumpTo;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jumpTrue -- jump on true to location                                       */
/*----------------------------------------------------------------------------*/
void Task::vFlowJumpTrue(MP_Task mpTask)
{
  ENTRY("vJumpTrue"_Str);
  
  i8 iVal;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iVal) )
    THROW_INVALID_DSTACK("jumpTrue"_Str);
  
  // n.b. the IP points to the jump arg
  
  if (iVal)
    // jump to location
    mpTask->IP.m_pCell = mpTask->IP.m_pCell->m_pJumpTo;
  else
    // do not jump, skip over argument
    mpTask->IP.m_pCell++;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* nop -- no op                                                               */
/*----------------------------------------------------------------------------*/
void Task::vFlowNop(MP_Task mpTask)
{
  ENTRY("vFlowNop"_Str);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* retryPoint -- mark retry point                                             */
/*----------------------------------------------------------------------------*/
void Task::vFlowRetryPoint(MP_Task mpTask)
{
  ENTRY("vFlowRetryPoint"_Str);
  
  mpTask->m_pResetIP = mpTask->IP.m_pCell;
  
  mpTask->m_pDataStack->vRetryPoint();
  
  mpTask->m_pReturnStack->vRetryPoint();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* test -- print test                                                         */
/*----------------------------------------------------------------------------*/
void Task::vFlowTest(MP_Task mpTask)
{
  ENTRY("vFlowTest"_Str);
  
  vTrace("[%s] Test\n"_Str,mpTask->m_sName.s());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* throw -- throw c++ exception (for testing purposes)                        */
/*----------------------------------------------------------------------------*/
void Task::vFlowThrow(MP_Task mpTask)
{
  ENTRY("vFlowThrow"_Str);
  
  char wBuffer[256];
  
  StrLib::vSprint(wBuffer,"THROW TEST"_Str);
  
  throw wBuffer;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
