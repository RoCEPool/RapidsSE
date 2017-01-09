/*============================================================================*/
/* Task_Assert.cpp - task assert group                                        */
/*============================================================================*/

#include "StorageEngine.h"
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

// assert: /////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Assert(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  vAddPrimitive("assert",          &vAssert,            false);
  vAddPrimitive("assert.stack_0",  &vAssertStack_0,     false);
  vAddPrimitive("assert.stack_0+", &vAssertStack_0more, false);
  vAddPrimitive("assert.stack_1",  &vAssertStack_1,     false);
  vAddPrimitive("assert.stack_1+", &vAssertStack_1more, false);
  vAddPrimitive("assert.stack_2",  &vAssertStack_2,     false);
  vAddPrimitive("assert.stack_2+", &vAssertStack_2more, false);
  vAddPrimitive("assert.stack_3",  &vAssertStack_3,     false);
  vAddPrimitive("assert.stack_3+", &vAssertStack_3more, false);
  vAddPrimitive("assert.stack_4",  &vAssertStack_4,     false);
  vAddPrimitive("assert.stack_5+", &vAssertStack_4more, false);
}

/*----------------------------------------------------------------------------*/
/* assert -- assert top value on stack is true                                */
/*----------------------------------------------------------------------------*/
void Task::vAssert(NNP_Task nnpTask)
{
  ENTRY("vAssert");
  
  i8 iTrue;
  if ( ! nnpTask->m_pDataStack->bPopParamInt(iTrue) )
    THROW_INVALID_DSTACK("assert");
  
  if ( ! iTrue )
    THROW("assert");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* assert.stack_0 -- asssert 0 values on stack                                */
/*----------------------------------------------------------------------------*/
void Task::vAssertStack_0(NNP_Task nnpTask)
{
ENTRY("vAssertStack_0");

  i8 iStackValues = nnpTask->m_pDataStack->iValues();
  
  if ( ! (0 == iStackValues)  )
    THROW("assert.stack_0");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* assert.stack_0+ -- asssert 0 or more values on stack                       */
/*----------------------------------------------------------------------------*/
void Task::vAssertStack_0more(NNP_Task nnpTask)
{
ENTRY("vAssertStack_0more");
  
  i8 iStackValues = nnpTask->m_pDataStack->iValues();
  
  if ( ! (0 <= iStackValues) )
    THROW("assert.stack0+");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* assert.stack_1 -- asssert 1 values on stack                                */
/*----------------------------------------------------------------------------*/
void Task::vAssertStack_1(NNP_Task nnpTask)
{
ENTRY("vAssertStack_1");
  
  i8 iStackValues = nnpTask->m_pDataStack->iValues();
  
  if ( ! (1 == iStackValues)  )
    THROW("assert.stack_1");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* assert.stack_1+ -- asssert 0 or more values on stack                       */
/*----------------------------------------------------------------------------*/
void Task::vAssertStack_1more(NNP_Task nnpTask)
{
ENTRY("vAssertStack_1more");
  
  i8 iStackValues = nnpTask->m_pDataStack->iValues();
  
  if ( ! (1 <= iStackValues) )
    THROW("assert.stack1+");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* assert.stack_2 -- asssert 2 values on stack                                */
/*----------------------------------------------------------------------------*/
void Task::vAssertStack_2(NNP_Task nnpTask)
{
ENTRY("vAssertStack_2");
  
  i8 iStackValues = nnpTask->m_pDataStack->iValues();
  
  if ( ! (2 == iStackValues)  )
    THROW("assert.stack_2");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* assert.stack_2+ -- asssert 2 or more values on stack                       */
/*----------------------------------------------------------------------------*/
void Task::vAssertStack_2more(NNP_Task nnpTask)
{
ENTRY("vAssertStack_2more");
  
  i8 iStackValues = nnpTask->m_pDataStack->iValues();
  
  if ( ! (2 <= iStackValues) )
    THROW("assert.stack0+");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* assert.stack_3 -- asssert 3 values on stack                                */
/*----------------------------------------------------------------------------*/
void Task::vAssertStack_3(NNP_Task nnpTask)
{
ENTRY("vAssertStack_3");
  
  i8 iStackValues = nnpTask->m_pDataStack->iValues();
  
  if ( ! (3 == iStackValues)  )
    THROW("assert.stack_3");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* assert.stack_3+ -- asssert 3 or more values on stack                       */
/*----------------------------------------------------------------------------*/
void Task::vAssertStack_3more(NNP_Task nnpTask)
{
ENTRY("vAssertStack_3more");
  
  i8 iStackValues = nnpTask->m_pDataStack->iValues();
  
  if ( ! (0 <= iStackValues) )
    THROW("assert.stack3+");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* assert.stack_4 -- asssert 4 values on stack                                */
/*----------------------------------------------------------------------------*/
void Task::vAssertStack_4(NNP_Task nnpTask)
{
ENTRY("vAssertStack_4");
  
  i8 iStackValues = nnpTask->m_pDataStack->iValues();
  
  if ( ! (4 == iStackValues)  )
    THROW("assert.stack_4");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* assert.stack_4+ -- asssert 4 or more values on stack                       */
/*----------------------------------------------------------------------------*/
void Task::vAssertStack_4more(NNP_Task nnpTask)
{
ENTRY("vAssertStack_4more");
  
  i8 iStackValues = nnpTask->m_pDataStack->iValues();
  
  if ( ! (0 <= iStackValues) )
    THROW("assert.stack4+");
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
