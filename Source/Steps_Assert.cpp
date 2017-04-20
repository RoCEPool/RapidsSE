/*============================================================================*/
/* Steps_Assert.cpp - assert step group                                       */
/*============================================================================*/

#include "StorageEngine.h"
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

// assert: /////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Assert(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  vAddPrimitive("assert"_Str,          &vAssert,            false);
  vAddPrimitive("assert.class"_Str,    &vAssertClass,       false);
  vAddPrimitive("assert.class_0"_Str,  &vAssertClass_0,     false);
  vAddPrimitive("assert.class_1"_Str,  &vAssertClass_1,     false);
  vAddPrimitive("assert.class_2"_Str,  &vAssertClass_2,     false);
  vAddPrimitive("assert.class_3"_Str,  &vAssertClass_3,     false);
  vAddPrimitive("assert.class_4"_Str,  &vAssertClass_4,     false);
  vAddPrimitive("assert.stack_0"_Str,  &vAssertStack_0,     false);
  vAddPrimitive("assert.stack_0+"_Str, &vAssertStack_0more, false);
  vAddPrimitive("assert.stack_1"_Str,  &vAssertStack_1,     false);
  vAddPrimitive("assert.stack_1+"_Str, &vAssertStack_1more, false);
  vAddPrimitive("assert.stack_2"_Str,  &vAssertStack_2,     false);
  vAddPrimitive("assert.stack_2+"_Str, &vAssertStack_2more, false);
  vAddPrimitive("assert.stack_3"_Str,  &vAssertStack_3,     false);
  vAddPrimitive("assert.stack_3+"_Str, &vAssertStack_3more, false);
  vAddPrimitive("assert.stack_4"_Str,  &vAssertStack_4,     false);
  vAddPrimitive("assert.stack_5+"_Str, &vAssertStack_4more, false);
}

/*----------------------------------------------------------------------------*/
/* assert -- assert top value on stack is true                                */
/*----------------------------------------------------------------------------*/
void Task::vAssert(NNP_Task nnpTask)
{
  ENTRY("vAssert"_Str);
  
  i8 iTrue;
  if ( ! nnpTask->m_pDataStack->bPopParamInt(iTrue) )
    THROW_INVALID_DSTACK("assert"_Str);
  
  if ( ! iTrue )
    THROW("assert"_Str);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* assert.class -- assert class of value on stack                             */
/*----------------------------------------------------------------------------*/
void Task::vAssertClass(NNP_Task nnpTask)
{
  ENTRY("vAssertClass"_Str);
  
  i8 iAssertClassNo;
  if ( ! nnpTask->m_pDataStack->bPopParamInt(iAssertClassNo) )
    THROW_INVALID_DSTACK("assert.class"_Str);
  
  Value* pValue = nnpTask->m_pDataStack->pPeek();
  
  eCN eAssertClassNo = (eCN)iAssertClassNo;
  eCN eClassNo = pValue->eClass();
  
  if (CN_PTRVAL == eClassNo && eAssertClassNo != CN_PTRVAL) {
    PtrVal* pPtrVal = _pPtrVal(pValue);
    eClassNo = pPtrVal->pPtr()->eClass();
  }
  
  if ( eClassNo != eAssertClassNo )
    THROW("assert.class"_Str);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* assert.class_0 -- assert class of value[0] on stack                        */
/*----------------------------------------------------------------------------*/
void Task::vAssertClass_0(NNP_Task nnpTask)
{
  ENTRY("vAssertClass_0"_Str);
  
  i8 iAssertClassNo;
  if ( ! nnpTask->m_pDataStack->bPopParamInt(iAssertClassNo) )
    THROW_INVALID_DSTACK("assert.class_0"_Str);
  
  Value* pValue = nnpTask->m_pDataStack->pPeek(0);
  
  eCN eAssertClassNo = (eCN)iAssertClassNo;
  eCN eClassNo = pValue->eClass();
  
  if (CN_PTRVAL == eClassNo && eAssertClassNo != CN_PTRVAL) {
    PtrVal* pPtrVal = _pPtrVal(pValue);
    eClassNo = pPtrVal->pPtr()->eClass();
  }
  
  if ( eClassNo != eAssertClassNo )
    THROW("assert.class_0"_Str);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* assert.class_1 -- assert class of value[1] on stack                        */
/*----------------------------------------------------------------------------*/
void Task::vAssertClass_1(NNP_Task nnpTask)
{
  ENTRY("vAssertClass_1"_Str);
  
  i8 iAssertClassNo;
  if ( ! nnpTask->m_pDataStack->bPopParamInt(iAssertClassNo) )
    THROW_INVALID_DSTACK("assert.class_1"_Str);
  
  Value* pValue = nnpTask->m_pDataStack->pPeek(1);
  
  eCN eAssertClassNo = (eCN)iAssertClassNo;
  eCN eClassNo = pValue->eClass();
  
  if (CN_PTRVAL == eClassNo && eAssertClassNo != CN_PTRVAL) {
    PtrVal* pPtrVal = _pPtrVal(pValue);
    eClassNo = pPtrVal->pPtr()->eClass();
  }
  
  if ( eClassNo != eAssertClassNo )
    THROW("assert.class_1"_Str);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* assert.class_2 -- assert class of value[2] on stack                        */
/*----------------------------------------------------------------------------*/
void Task::vAssertClass_2(NNP_Task nnpTask)
{
  ENTRY("vAssertClass_2"_Str);
  
  i8 iAssertClassNo;
  if ( ! nnpTask->m_pDataStack->bPopParamInt(iAssertClassNo) )
    THROW_INVALID_DSTACK("assert.class_2"_Str);
  
  Value* pValue = nnpTask->m_pDataStack->pPeek(2);
  
  eCN eAssertClassNo = (eCN)iAssertClassNo;
  eCN eClassNo = pValue->eClass();
  
  if (CN_PTRVAL == eClassNo && eAssertClassNo != CN_PTRVAL) {
    PtrVal* pPtrVal = _pPtrVal(pValue);
    eClassNo = pPtrVal->pPtr()->eClass();
  }
  
  if ( eClassNo != eAssertClassNo )
    THROW("assert.class_2"_Str);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* assert.class_3 -- assert class of value[3] on stack                        */
/*----------------------------------------------------------------------------*/
void Task::vAssertClass_3(NNP_Task nnpTask)
{
  ENTRY("vAssertClass_3"_Str);
  
  i8 iAssertClassNo;
  if ( ! nnpTask->m_pDataStack->bPopParamInt(iAssertClassNo) )
    THROW_INVALID_DSTACK("assert.class_3"_Str);
  
  Value* pValue = nnpTask->m_pDataStack->pPeek(3);
  
  eCN eAssertClassNo = (eCN)iAssertClassNo;
  eCN eClassNo = pValue->eClass();
  
  if (CN_PTRVAL == eClassNo && eAssertClassNo != CN_PTRVAL) {
    PtrVal* pPtrVal = _pPtrVal(pValue);
    eClassNo = pPtrVal->pPtr()->eClass();
  }
  
  if ( eClassNo != eAssertClassNo )
    THROW("assert.class_3"_Str);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* assert.class_4 -- assert class of value[4] on stack                        */
/*----------------------------------------------------------------------------*/
void Task::vAssertClass_4(NNP_Task nnpTask)
{
  ENTRY("vAssertClass_4"_Str);
  
  i8 iAssertClassNo;
  if ( ! nnpTask->m_pDataStack->bPopParamInt(iAssertClassNo) )
    THROW_INVALID_DSTACK("assert.class_4"_Str);
  
  Value* pValue = nnpTask->m_pDataStack->pPeek(4);
  
  eCN eAssertClassNo = (eCN)iAssertClassNo;
  eCN eClassNo = pValue->eClass();
  
  if (CN_PTRVAL == eClassNo && eAssertClassNo != CN_PTRVAL) {
    PtrVal* pPtrVal = _pPtrVal(pValue);
    eClassNo = pPtrVal->pPtr()->eClass();
  }
  
  if ( eClassNo != eAssertClassNo )
    THROW("assert.class_4"_Str);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* assert.stack_0 -- asssert 0 values on stack                                */
/*----------------------------------------------------------------------------*/
void Task::vAssertStack_0(NNP_Task nnpTask)
{
ENTRY("vAssertStack_0"_Str);

  i8 iStackValues = nnpTask->m_pDataStack->iValues();
  
  if ( ! (0 == iStackValues)  )
    THROW("assert.stack_0"_Str);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* assert.stack_0+ -- asssert 0 or more values on stack                       */
/*----------------------------------------------------------------------------*/
void Task::vAssertStack_0more(NNP_Task nnpTask)
{
ENTRY("vAssertStack_0more"_Str);
  
  i8 iStackValues = nnpTask->m_pDataStack->iValues();
  
  if ( ! (0 <= iStackValues) )
    THROW("assert.stack0+"_Str);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* assert.stack_1 -- asssert 1 values on stack                                */
/*----------------------------------------------------------------------------*/
void Task::vAssertStack_1(NNP_Task nnpTask)
{
ENTRY("vAssertStack_1"_Str);
  
  i8 iStackValues = nnpTask->m_pDataStack->iValues();
  
  if ( ! (1 == iStackValues)  )
    THROW("assert.stack_1"_Str);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* assert.stack_1+ -- asssert 0 or more values on stack                       */
/*----------------------------------------------------------------------------*/
void Task::vAssertStack_1more(NNP_Task nnpTask)
{
ENTRY("vAssertStack_1more"_Str);
  
  i8 iStackValues = nnpTask->m_pDataStack->iValues();
  
  if ( ! (1 <= iStackValues) )
    THROW("assert.stack1+"_Str);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* assert.stack_2 -- asssert 2 values on stack                                */
/*----------------------------------------------------------------------------*/
void Task::vAssertStack_2(NNP_Task nnpTask)
{
ENTRY("vAssertStack_2"_Str);
  
  i8 iStackValues = nnpTask->m_pDataStack->iValues();
  
  if ( ! (2 == iStackValues)  )
    THROW("assert.stack_2"_Str);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* assert.stack_2+ -- asssert 2 or more values on stack                       */
/*----------------------------------------------------------------------------*/
void Task::vAssertStack_2more(NNP_Task nnpTask)
{
ENTRY("vAssertStack_2more"_Str);
  
  i8 iStackValues = nnpTask->m_pDataStack->iValues();
  
  if ( ! (2 <= iStackValues) )
    THROW("assert.stack0+"_Str);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* assert.stack_3 -- asssert 3 values on stack                                */
/*----------------------------------------------------------------------------*/
void Task::vAssertStack_3(NNP_Task nnpTask)
{
ENTRY("vAssertStack_3"_Str);
  
  i8 iStackValues = nnpTask->m_pDataStack->iValues();
  
  if ( ! (3 == iStackValues)  )
    THROW("assert.stack_3"_Str);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* assert.stack_3+ -- asssert 3 or more values on stack                       */
/*----------------------------------------------------------------------------*/
void Task::vAssertStack_3more(NNP_Task nnpTask)
{
ENTRY("vAssertStack_3more"_Str);
  
  i8 iStackValues = nnpTask->m_pDataStack->iValues();
  
  if ( ! (0 <= iStackValues) )
    THROW("assert.stack3+"_Str);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* assert.stack_4 -- asssert 4 values on stack                                */
/*----------------------------------------------------------------------------*/
void Task::vAssertStack_4(NNP_Task nnpTask)
{
ENTRY("vAssertStack_4"_Str);
  
  i8 iStackValues = nnpTask->m_pDataStack->iValues();
  
  if ( ! (4 == iStackValues)  )
    THROW("assert.stack_4"_Str);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* assert.stack_4+ -- asssert 4 or more values on stack                       */
/*----------------------------------------------------------------------------*/
void Task::vAssertStack_4more(NNP_Task nnpTask)
{
ENTRY("vAssertStack_4more"_Str);
  
  i8 iStackValues = nnpTask->m_pDataStack->iValues();
  
  if ( ! (0 <= iStackValues) )
    THROW("assert.stack4+"_Str);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
