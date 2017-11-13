/*============================================================================*/
/* Steps_Assert.cpp - assert step group                                       */
/*============================================================================*/

#include "StorageEngine.h"
#include "SePrompt.hpp"

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

// assert: /////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Assert(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  vAddPrimitive("assert"_Stz,           &vAssert,            false);
  vAddPrimitive("assert::class"_Stz,    &vAssertClass,       false);
  vAddPrimitive("assert::class_0"_Stz,  &vAssertClass_0,     false);
  vAddPrimitive("assert::class_1"_Stz,  &vAssertClass_1,     false);
  vAddPrimitive("assert::class_2"_Stz,  &vAssertClass_2,     false);
  vAddPrimitive("assert::class_3"_Stz,  &vAssertClass_3,     false);
  vAddPrimitive("assert::class_4"_Stz,  &vAssertClass_4,     false);
  vAddPrimitive("assert::stack_0"_Stz,  &vAssertStack_0,     false);
  vAddPrimitive("assert::stack_0+"_Stz, &vAssertStack_0more, false);
  vAddPrimitive("assert::stack_1"_Stz,  &vAssertStack_1,     false);
  vAddPrimitive("assert::stack_1+"_Stz, &vAssertStack_1more, false);
  vAddPrimitive("assert::stack_2"_Stz,  &vAssertStack_2,     false);
  vAddPrimitive("assert::stack_2+"_Stz, &vAssertStack_2more, false);
  vAddPrimitive("assert::stack_3"_Stz,  &vAssertStack_3,     false);
  vAddPrimitive("assert::stack_3+"_Stz, &vAssertStack_3more, false);
  vAddPrimitive("assert::stack_4"_Stz,  &vAssertStack_4,     false);
  vAddPrimitive("assert::stack_5+"_Stz, &vAssertStack_4more, false);
}

/*----------------------------------------------------------------------------*/
/* assert -- assert top value on stack is true                                */
/*----------------------------------------------------------------------------*/
void Task::vAssert(MP_Task mpTask)
{
  ENTRY("vAssert"_Stz);
  
  i8 iTrue;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iTrue) )
    THROW_INVALID_DSTACK("assert"_Stz);
  
  if ( ! iTrue )
    THROW("assert in word %s"_Stz, mpTask->m_pWord ? mpTask->m_pWord->sName().aChar() : "NULL");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* assert::class -- assert class of value on stack                            */
/*----------------------------------------------------------------------------*/
void Task::vAssertClass(MP_Task mpTask)
{
  ENTRY("vAssertClass"_Stz);
  
  i8 iAssertClassNo;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iAssertClassNo) )
    THROW_INVALID_DSTACK("assert::class"_Stz);
  
  Value* pValue = mpTask->m_pDataStack->pPeekValue();
  
  eCN eAssertClassNo = (eCN)iAssertClassNo;
  eCN eClassNo = pValue->eClass();
  
  if (CN_PTRVAL == eClassNo && eAssertClassNo != CN_PTRVAL) {
    PtrVal* pPtrVal = _pPtrVal(pValue);
    eClassNo = pPtrVal->pPtr()->eClass();
  }
  
  if ( eClassNo != eAssertClassNo )
    THROW("assert::class in word %s"_Stz, mpTask->m_pWord ? mpTask->m_pWord->sName().aChar() : "NULL");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* assert::class_0 -- assert class of value[0] on stack                       */
/*----------------------------------------------------------------------------*/
void Task::vAssertClass_0(MP_Task mpTask)
{
  ENTRY("vAssertClass_0"_Stz);
  
  i8 iAssertClassNo;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iAssertClassNo) )
    THROW_INVALID_DSTACK("assert::class_0"_Stz);
  
  Value* pValue = mpTask->m_pDataStack->pPeekValue(0);
  
  eCN eAssertClassNo = (eCN)iAssertClassNo;
  eCN eClassNo = pValue->eClass();
  
  if (CN_PTRVAL == eClassNo && eAssertClassNo != CN_PTRVAL) {
    PtrVal* pPtrVal = _pPtrVal(pValue);
    eClassNo = pPtrVal->pPtr()->eClass();
  }
  
  if ( eClassNo != eAssertClassNo )
    THROW("assert::class_0 in word %s"_Stz, mpTask->m_pWord ? mpTask->m_pWord->sName().aChar() : "NULL");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* assert::class_1 -- assert class of value[1] on stack                       */
/*----------------------------------------------------------------------------*/
void Task::vAssertClass_1(MP_Task mpTask)
{
  ENTRY("vAssertClass_1"_Stz);
  
  i8 iAssertClassNo;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iAssertClassNo) )
    THROW_INVALID_DSTACK("assert::class_1"_Stz);
  
  Value* pValue = mpTask->m_pDataStack->pPeekValue(1);
  
  eCN eAssertClassNo = (eCN)iAssertClassNo;
  eCN eClassNo = pValue->eClass();
  
  if (CN_PTRVAL == eClassNo && eAssertClassNo != CN_PTRVAL) {
    PtrVal* pPtrVal = _pPtrVal(pValue);
    eClassNo = pPtrVal->pPtr()->eClass();
  }
  
  if ( eClassNo != eAssertClassNo )
    THROW("assert::class_1 in word %s"_Stz, mpTask->m_pWord ? mpTask->m_pWord->sName().aChar() : "NULL");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* assert::class_2 -- assert class of value[2] on stack                       */
/*----------------------------------------------------------------------------*/
void Task::vAssertClass_2(MP_Task mpTask)
{
  ENTRY("vAssertClass_2"_Stz);
  
  i8 iAssertClassNo;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iAssertClassNo) )
    THROW_INVALID_DSTACK("assert::class_2"_Stz);
  
  Value* pValue = mpTask->m_pDataStack->pPeekValue(2);
  
  eCN eAssertClassNo = (eCN)iAssertClassNo;
  eCN eClassNo = pValue->eClass();
  
  if (CN_PTRVAL == eClassNo && eAssertClassNo != CN_PTRVAL) {
    PtrVal* pPtrVal = _pPtrVal(pValue);
    eClassNo = pPtrVal->pPtr()->eClass();
  }
  
  if ( eClassNo != eAssertClassNo )
    THROW("assert::class_2 in word %s"_Stz, mpTask->m_pWord ? mpTask->m_pWord->sName().aChar() : "NULL");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* assert::class_3 -- assert class of value[3] on stack                       */
/*----------------------------------------------------------------------------*/
void Task::vAssertClass_3(MP_Task mpTask)
{
  ENTRY("vAssertClass_3"_Stz);
  
  i8 iAssertClassNo;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iAssertClassNo) )
    THROW_INVALID_DSTACK("assert::class_3"_Stz);
  
  Value* pValue = mpTask->m_pDataStack->pPeekValue(3);
  
  eCN eAssertClassNo = (eCN)iAssertClassNo;
  eCN eClassNo = pValue->eClass();
  
  if (CN_PTRVAL == eClassNo && eAssertClassNo != CN_PTRVAL) {
    PtrVal* pPtrVal = _pPtrVal(pValue);
    eClassNo = pPtrVal->pPtr()->eClass();
  }
  
  if ( eClassNo != eAssertClassNo )
    THROW("assert::class_3 in word %s"_Stz, mpTask->m_pWord ? mpTask->m_pWord->sName().aChar() : "NULL");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* assert::class_4 -- assert class of value[4] on stack                       */
/*----------------------------------------------------------------------------*/
void Task::vAssertClass_4(MP_Task mpTask)
{
  ENTRY("vAssertClass_4"_Stz);
  
  i8 iAssertClassNo;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iAssertClassNo) )
    THROW_INVALID_DSTACK("assert::class_4"_Stz);
  
  Value* pValue = mpTask->m_pDataStack->pPeekValue(4);
  
  eCN eAssertClassNo = (eCN)iAssertClassNo;
  eCN eClassNo = pValue->eClass();
  
  if (CN_PTRVAL == eClassNo && eAssertClassNo != CN_PTRVAL) {
    PtrVal* pPtrVal = _pPtrVal(pValue);
    eClassNo = pPtrVal->pPtr()->eClass();
  }
  
  if ( eClassNo != eAssertClassNo )
    THROW("assert::class_4 in word %s"_Stz, mpTask->m_pWord ? mpTask->m_pWord->sName().aChar() : "NULL");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* assert::stack_0 -- asssert 0 values on stack                               */
/*----------------------------------------------------------------------------*/
void Task::vAssertStack_0(MP_Task mpTask)
{
ENTRY("vAssertStack_0"_Stz);

  i8 iStackValues = mpTask->m_pDataStack->iValues();
  
  if ( ! (0 == iStackValues)  )
    THROW("assert::stack_0 in word %s"_Stz, mpTask->m_pWord ? mpTask->m_pWord->sName().aChar() : "NULL");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* assert::stack_0+ -- asssert 0 or more values on stack                      */
/*----------------------------------------------------------------------------*/
void Task::vAssertStack_0more(MP_Task mpTask)
{
ENTRY("vAssertStack_0more"_Stz);
  
  i8 iStackValues = mpTask->m_pDataStack->iValues();
  
  if ( ! (0 <= iStackValues) )
    THROW("assert::stack_0+ in word %s"_Stz, mpTask->m_pWord ? mpTask->m_pWord->sName().aChar() : "NULL");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* assert::stack_1 -- asssert 1 values on stack                               */
/*----------------------------------------------------------------------------*/
void Task::vAssertStack_1(MP_Task mpTask)
{
ENTRY("vAssertStack_1"_Stz);
  
  i8 iStackValues = mpTask->m_pDataStack->iValues();
  
  if ( ! (1 == iStackValues)  )
    THROW("assert::stack_1 in word %s"_Stz, mpTask->m_pWord ? mpTask->m_pWord->sName().aChar() : "NULL");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* assert::stack_1+ -- asssert 0 or more values on stack                      */
/*----------------------------------------------------------------------------*/
void Task::vAssertStack_1more(MP_Task mpTask)
{
ENTRY("vAssertStack_1more"_Stz);
  
  i8 iStackValues = mpTask->m_pDataStack->iValues();
  
  if ( ! (1 <= iStackValues) )
    THROW("assert::stack_1+ in word %s"_Stz, mpTask->m_pWord ? mpTask->m_pWord->sName().aChar() : "NULL");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* assert::stack_2 -- asssert 2 values on stack                               */
/*----------------------------------------------------------------------------*/
void Task::vAssertStack_2(MP_Task mpTask)
{
ENTRY("vAssertStack_2"_Stz);
  
  i8 iStackValues = mpTask->m_pDataStack->iValues();
  
  if ( ! (2 == iStackValues)  )
    THROW("assert::stack_2 in word %s"_Stz, mpTask->m_pWord ? mpTask->m_pWord->sName().aChar() : "NULL");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* assert::stack_2+ -- asssert 2 or more values on stack                      */
/*----------------------------------------------------------------------------*/
void Task::vAssertStack_2more(MP_Task mpTask)
{
ENTRY("vAssertStack_2more"_Stz);
  
  i8 iStackValues = mpTask->m_pDataStack->iValues();
  
  if ( ! (2 <= iStackValues) )
    THROW("assert::stack_2+ in word %s"_Stz, mpTask->m_pWord ? mpTask->m_pWord->sName().aChar() : "NULL");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* assert::stack_3 -- asssert 3 values on stack                               */
/*----------------------------------------------------------------------------*/
void Task::vAssertStack_3(MP_Task mpTask)
{
ENTRY("vAssertStack_3"_Stz);
  
  i8 iStackValues = mpTask->m_pDataStack->iValues();
  
  if ( ! (3 == iStackValues)  )
    THROW("assert::stack_3 in word %s"_Stz, mpTask->m_pWord ? mpTask->m_pWord->sName().aChar() : "NULL");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* assert::stack_3+ -- asssert 3 or more values on stack                      */
/*----------------------------------------------------------------------------*/
void Task::vAssertStack_3more(MP_Task mpTask)
{
ENTRY("vAssertStack_3more"_Stz);
  
  i8 iStackValues = mpTask->m_pDataStack->iValues();
  
  if ( ! (3 <= iStackValues) )
    THROW("assert::stack_3+ in word %s"_Stz, mpTask->m_pWord ? mpTask->m_pWord->sName().aChar() : "NULL");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* assert::stack_4 -- asssert 4 values on stack                               */
/*----------------------------------------------------------------------------*/
void Task::vAssertStack_4(MP_Task mpTask)
{
ENTRY("vAssertStack_4"_Stz);
  
  i8 iStackValues = mpTask->m_pDataStack->iValues();
  
  if ( ! (4 == iStackValues)  )
    THROW("assert::stack_4 in word %s"_Stz, mpTask->m_pWord ? mpTask->m_pWord->sName().aChar() : "NULL");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* assert::stack_4+ -- asssert 4 or more values on stack                      */
/*----------------------------------------------------------------------------*/
void Task::vAssertStack_4more(MP_Task mpTask)
{
ENTRY("vAssertStack_4more"_Stz);
  
  i8 iStackValues = mpTask->m_pDataStack->iValues();
  
  if ( ! (4 <= iStackValues) )
    THROW("assert::stack_4+ in word %s"_Stz, mpTask->m_pWord ? mpTask->m_pWord->sName().aChar() : "NULL");
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
