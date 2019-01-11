/*============================================================================*/
/* Steps_Assert.cpp - assert step group                                       */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Assert(void)
{
  vAddStep("assert"_Stz,           &Step::vAssert);
  vAddStep("assert::class"_Stz,    &Step::vAssertClass);
  vAddStep("assert::class_0"_Stz,  &Step::vAssertClass_0);
  vAddStep("assert::class_1"_Stz,  &Step::vAssertClass_1);
  vAddStep("assert::class_2"_Stz,  &Step::vAssertClass_2);
  vAddStep("assert::class_3"_Stz,  &Step::vAssertClass_3);
  vAddStep("assert::class_4"_Stz,  &Step::vAssertClass_4);
  vAddStep("assert::stack_0"_Stz,  &Step::vAssertStack_0);
  vAddStep("assert::stack_0+"_Stz, &Step::vAssertStack_0more);
  vAddStep("assert::stack_1"_Stz,  &Step::vAssertStack_1);
  vAddStep("assert::stack_1+"_Stz, &Step::vAssertStack_1more);
  vAddStep("assert::stack_2"_Stz,  &Step::vAssertStack_2);
  vAddStep("assert::stack_2+"_Stz, &Step::vAssertStack_2more);
  vAddStep("assert::stack_3"_Stz,  &Step::vAssertStack_3);
  vAddStep("assert::stack_3+"_Stz, &Step::vAssertStack_3more);
  vAddStep("assert::stack_4"_Stz,  &Step::vAssertStack_4);
  vAddStep("assert::stack_5+"_Stz, &Step::vAssertStack_4more);
}

/*----------------------------------------------------------------------------*/
/* assert -- assert top value on stack is true                                */
/*----------------------------------------------------------------------------*/
void Step::vAssert(MP_Task mpTask)
{
  ENTRY("vAssert"_Stz);
  
  i8 iTrue;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iTrue) )
    THROW_INVALID_DSTACK("assert"_Stz);
  
  if ( ! iTrue )
    THROW("assert in word %s"_Stz, "NULL");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* assert::class -- assert class of value on stack                            */
/*----------------------------------------------------------------------------*/
void Step::vAssertClass(MP_Task mpTask)
{
  ENTRY("vAssertClass"_Stz);
  
  i8 iAssertClassNo;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iAssertClassNo) )
    THROW_INVALID_DSTACK("assert::class"_Stz);
  
  Value* aValue = mpTask->m_pDataStack->aPeekValue();
  
  eCN eAssertClassNo = (eCN)iAssertClassNo;
  eCN eClassNo = aValue->eClass();
  
  if (CN_PTRVAL == eClassNo && eAssertClassNo != CN_PTRVAL) {
    PtrVal* pPtrVal = _aPtrVal(aValue);
    eClassNo = pPtrVal->pObj()->eClass();
  }
  
  if ( eClassNo != eAssertClassNo )
    THROW("assert::class in word %s"_Stz, "NULL");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* assert::class_0 -- assert class of value[0] on stack                       */
/*----------------------------------------------------------------------------*/
void Step::vAssertClass_0(MP_Task mpTask)
{
  ENTRY("vAssertClass_0"_Stz);
  
  i8 iAssertClassNo;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iAssertClassNo) )
    THROW_INVALID_DSTACK("assert::class_0"_Stz);
  
  Value* aValue = mpTask->m_pDataStack->aPeekValue(0);
  
  eCN eAssertClassNo = (eCN)iAssertClassNo;
  eCN eClassNo = aValue->eClass();
  
  if (CN_PTRVAL == eClassNo && eAssertClassNo != CN_PTRVAL) {
    PtrVal* pPtrVal = _aPtrVal(aValue);
    eClassNo = pPtrVal->pObj()->eClass();
  }
  
  if ( eClassNo != eAssertClassNo )
    THROW("assert::class_0 in word %s"_Stz, "NULL");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* assert::class_1 -- assert class of value[1] on stack                       */
/*----------------------------------------------------------------------------*/
void Step::vAssertClass_1(MP_Task mpTask)
{
  ENTRY("vAssertClass_1"_Stz);
  
  i8 iAssertClassNo;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iAssertClassNo) )
    THROW_INVALID_DSTACK("assert::class_1"_Stz);
  
  Value* aValue = mpTask->m_pDataStack->aPeekValue(1);
  
  eCN eAssertClassNo = (eCN)iAssertClassNo;
  eCN eClassNo = aValue->eClass();
  
  if (CN_PTRVAL == eClassNo && eAssertClassNo != CN_PTRVAL) {
    PtrVal* pPtrVal = _aPtrVal(aValue);
    eClassNo = pPtrVal->pObj()->eClass();
  }
  
  if ( eClassNo != eAssertClassNo )
    THROW("assert::class_1 in word %s"_Stz, "NULL");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* assert::class_2 -- assert class of value[2] on stack                       */
/*----------------------------------------------------------------------------*/
void Step::vAssertClass_2(MP_Task mpTask)
{
  ENTRY("vAssertClass_2"_Stz);
  
  i8 iAssertClassNo;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iAssertClassNo) )
    THROW_INVALID_DSTACK("assert::class_2"_Stz);
  
  Value* aValue = mpTask->m_pDataStack->aPeekValue(2);
  
  eCN eAssertClassNo = (eCN)iAssertClassNo;
  eCN eClassNo = aValue->eClass();
  
  if (CN_PTRVAL == eClassNo && eAssertClassNo != CN_PTRVAL) {
    PtrVal* pPtrVal = _aPtrVal(aValue);
    eClassNo = pPtrVal->pObj()->eClass();
  }
  
  if ( eClassNo != eAssertClassNo )
    THROW("assert::class_2 in word %s"_Stz, "NULL");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* assert::class_3 -- assert class of value[3] on stack                       */
/*----------------------------------------------------------------------------*/
void Step::vAssertClass_3(MP_Task mpTask)
{
  ENTRY("vAssertClass_3"_Stz);
  
  i8 iAssertClassNo;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iAssertClassNo) )
    THROW_INVALID_DSTACK("assert::class_3"_Stz);
  
  Value* aValue = mpTask->m_pDataStack->aPeekValue(3);
  
  eCN eAssertClassNo = (eCN)iAssertClassNo;
  eCN eClassNo = aValue->eClass();
  
  if (CN_PTRVAL == eClassNo && eAssertClassNo != CN_PTRVAL) {
    PtrVal* pPtrVal = _aPtrVal(aValue);
    eClassNo = pPtrVal->pObj()->eClass();
  }
  
  if ( eClassNo != eAssertClassNo )
    THROW("assert::class_3 in word %s"_Stz, "NULL");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* assert::class_4 -- assert class of value[4] on stack                       */
/*----------------------------------------------------------------------------*/
void Step::vAssertClass_4(MP_Task mpTask)
{
  ENTRY("vAssertClass_4"_Stz);
  
  i8 iAssertClassNo;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iAssertClassNo) )
    THROW_INVALID_DSTACK("assert::class_4"_Stz);
  
  Value* aValue = mpTask->m_pDataStack->aPeekValue(4);
  
  eCN eAssertClassNo = (eCN)iAssertClassNo;
  eCN eClassNo = aValue->eClass();
  
  if (CN_PTRVAL == eClassNo && eAssertClassNo != CN_PTRVAL) {
    PtrVal* pPtrVal = _aPtrVal(aValue);
    eClassNo = pPtrVal->pObj()->eClass();
  }
  
  if ( eClassNo != eAssertClassNo )
    THROW("assert::class_4 in word %s"_Stz, "NULL");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* assert::stack_0 -- asssert 0 values on stack                               */
/*----------------------------------------------------------------------------*/
void Step::vAssertStack_0(MP_Task mpTask)
{
ENTRY("vAssertStack_0"_Stz);

  i8 iStackValues = mpTask->m_pDataStack->nValues();
  
  if ( ! (0 == iStackValues)  )
    THROW("assert::stack_0 in word %s"_Stz, "NULL");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* assert::stack_0+ -- asssert 0 or more values on stack                      */
/*----------------------------------------------------------------------------*/
void Step::vAssertStack_0more(MP_Task mpTask)
{
ENTRY("vAssertStack_0more"_Stz);
  
  i8 iStackValues = mpTask->m_pDataStack->nValues();
  
  if ( ! (0 <= iStackValues) )
    THROW("assert::stack_0+ in word %s"_Stz, "NULL");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* assert::stack_1 -- asssert 1 values on stack                               */
/*----------------------------------------------------------------------------*/
void Step::vAssertStack_1(MP_Task mpTask)
{
ENTRY("vAssertStack_1"_Stz);
  
  i8 iStackValues = mpTask->m_pDataStack->nValues();
  
  if ( ! (1 == iStackValues)  )
    THROW("assert::stack_1 in word %s"_Stz, "NULL");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* assert::stack_1+ -- asssert 0 or more values on stack                      */
/*----------------------------------------------------------------------------*/
void Step::vAssertStack_1more(MP_Task mpTask)
{
ENTRY("vAssertStack_1more"_Stz);
  
  i8 iStackValues = mpTask->m_pDataStack->nValues();
  
  if ( ! (1 <= iStackValues) )
    THROW("assert::stack_1+ in word %s"_Stz, "NULL");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* assert::stack_2 -- asssert 2 values on stack                               */
/*----------------------------------------------------------------------------*/
void Step::vAssertStack_2(MP_Task mpTask)
{
ENTRY("vAssertStack_2"_Stz);
  
  i8 iStackValues = mpTask->m_pDataStack->nValues();
  
  if ( ! (2 == iStackValues)  )
    THROW("assert::stack_2 in word %s"_Stz, "NULL");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* assert::stack_2+ -- asssert 2 or more values on stack                      */
/*----------------------------------------------------------------------------*/
void Step::vAssertStack_2more(MP_Task mpTask)
{
ENTRY("vAssertStack_2more"_Stz);
  
  i8 iStackValues = mpTask->m_pDataStack->nValues();
  
  if ( ! (2 <= iStackValues) )
    THROW("assert::stack_2+ in word %s"_Stz, "NULL");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* assert::stack_3 -- asssert 3 values on stack                               */
/*----------------------------------------------------------------------------*/
void Step::vAssertStack_3(MP_Task mpTask)
{
ENTRY("vAssertStack_3"_Stz);
  
  i8 iStackValues = mpTask->m_pDataStack->nValues();
  
  if ( ! (3 == iStackValues)  )
    THROW("assert::stack_3 in word %s"_Stz, "NULL");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* assert::stack_3+ -- asssert 3 or more values on stack                      */
/*----------------------------------------------------------------------------*/
void Step::vAssertStack_3more(MP_Task mpTask)
{
ENTRY("vAssertStack_3more"_Stz);
  
  i8 iStackValues = mpTask->m_pDataStack->nValues();
  
  if ( ! (3 <= iStackValues) )
    THROW("assert::stack_3+ in word %s"_Stz, "NULL");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* assert::stack_4 -- asssert 4 values on stack                               */
/*----------------------------------------------------------------------------*/
void Step::vAssertStack_4(MP_Task mpTask)
{
ENTRY("vAssertStack_4"_Stz);
  
  i8 iStackValues = mpTask->m_pDataStack->nValues();
  
  if ( ! (4 == iStackValues)  )
    THROW("assert::stack_4 in word %s"_Stz, "NULL");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* assert::stack_4+ -- asssert 4 or more values on stack                      */
/*----------------------------------------------------------------------------*/
void Step::vAssertStack_4more(MP_Task mpTask)
{
ENTRY("vAssertStack_4more"_Stz);
  
  i8 iStackValues = mpTask->m_pDataStack->nValues();
  
  if ( ! (4 <= iStackValues) )
    THROW("assert::stack_4+ in word %s"_Stz, "NULL");
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
