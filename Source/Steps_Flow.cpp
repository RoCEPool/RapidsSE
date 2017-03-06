/*============================================================================*/
/* Steps_Flow.cpp - flow step group                                           */
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
void Task::vFlowBegin_(NNP_Task nnpTask)
{
  ENTRY("vBegin_");
  
  nnpTask->m_pBeginIP = nnpTask->IP.m_pCell;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* execute -- execute word on data stack                                      */
/*----------------------------------------------------------------------------*/
void Task::vFlowExecute(NNP_Task nnpTask)
{
  ENTRY("vFlowExecute");
  
  Object* pWordObject;
  if ( ! nnpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("execute");
  Word* pWord = _pWord(pWordObject);
  
  if (WT_PRIMITIVE == pWord->eWordType()) {
    TaskStep pTaskStep = pWord->pEntryStep();
    (pTaskStep)(nnpTask);
  }
  else if (WT_OBJECT == pWord->eWordType()) {
    nnpTask->m_pReturnStack->vPush(nnpTask->IP.m_pCell);
    nnpTask->IP.m_pCell = pWord->pEntryCell();
    
    Object* pObject = (Object*)pWord->pFirstCell();
    nnpTask->m_pDataStack->vPush(pObject);
  }
  else if (WT_ROUTINE == pWord->eWordType()){
    nnpTask->m_pReturnStack->vPush(nnpTask->IP.m_pCell);
    nnpTask->IP.m_pCell = pWord->pEntryCell();
  }
  else {
    ASSERT(0);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* hazard -- should never be called                                           */
/*----------------------------------------------------------------------------*/
void Task::vFlowHazard(NNP_Task nnpTask)
{
  ENTRY("vFlowHazard");
  
  THROW("Entered hazard");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jump -- jump to location                                                   */
/*----------------------------------------------------------------------------*/
void Task::vFlowJump(NNP_Task nnpTask)
{
  ENTRY("vJump");
  
  nnpTask->IP.m_pCell = nnpTask->IP.m_pCell->m_pJumpTo;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jumpBegin_ -- jump to begin_ (used only in : docol)              immediate */
/*----------------------------------------------------------------------------*/
void Task::vFlowJumpBegin_(NNP_Task nnpTask)
{
  ENTRY("vFlowJumpBegin_");
  
  nnpTask->IP.m_pCell = nnpTask->m_pBeginIP;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jumpFalse -- jump on false to location                                     */
/*----------------------------------------------------------------------------*/
void Task::vFlowJumpFalse(NNP_Task nnpTask)
{
  ENTRY("vJumpFalse");
  
  i8 iVal;
  if ( ! nnpTask->m_pDataStack->bPopParamInt(iVal) )
    THROW_INVALID_DSTACK("jumpFalse");
  
  // n.b. the IP points to the jump arg
  
  if (iVal)
    // do not jump, skip over argument
    nnpTask->IP.m_pCell++;
  else
    // jump to location
    nnpTask->IP.m_pCell = nnpTask->IP.m_pCell->m_pJumpTo;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jumpTrue -- jump on true to location                                       */
/*----------------------------------------------------------------------------*/
void Task::vFlowJumpTrue(NNP_Task nnpTask)
{
  ENTRY("vJumpTrue");
  
  i8 iVal;
  if ( ! nnpTask->m_pDataStack->bPopParamInt(iVal) )
    THROW_INVALID_DSTACK("jumpTrue");
  
  // n.b. the IP points to the jump arg
  
  if (iVal)
    // jump to location
    nnpTask->IP.m_pCell = nnpTask->IP.m_pCell->m_pJumpTo;
  else
    // do not jump, skip over argument
    nnpTask->IP.m_pCell++;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* nop -- no op                                                               */
/*----------------------------------------------------------------------------*/
void Task::vFlowNop(NNP_Task nnpTask)
{
  ENTRY("vFlowNop");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* retryPoint -- mark retry point                                             */
/*----------------------------------------------------------------------------*/
void Task::vFlowRetryPoint(NNP_Task nnpTask)
{
  ENTRY("vFlowRetryPoint");
  
  nnpTask->m_pResetIP = nnpTask->IP.m_pCell;
  
  nnpTask->m_pDataStack->vRetryPoint();
  
  nnpTask->m_pReturnStack->vRetryPoint();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* test -- print test                                                         */
/*----------------------------------------------------------------------------*/
void Task::vFlowTest(NNP_Task nnpTask)
{
  ENTRY("vFlowTest");
  
  vTrace("[%s] Test\n",nnpTask->m_sName.aStr());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* throw -- throw c++ exception (for testing purposes)                        */
/*----------------------------------------------------------------------------*/
void Task::vFlowThrow(NNP_Task nnpTask)
{
  ENTRY("vFlowThrow");
  
  char wBuffer[256];
  
  StrLib::vSprint(wBuffer,"THROW TEST");
  
  throw wBuffer;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
