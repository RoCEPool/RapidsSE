/*============================================================================*/
/* Steps_Assert.cpp - assert step group                                       */
/*============================================================================*/

#include "Steps.h"

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
void Step::vAssert(void)
{
  ENTRY();
  
  i8 iTrue = m_pDataStack->iPopAsInt(__func__);
  
  RASSERT(iTrue);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* assert::class -- assert class of value on stack                            */
/*----------------------------------------------------------------------------*/
void Step::vAssertClass(void)
{
  ENTRY();
  
  i8 iAssertClassNo = m_pDataStack->iPopAsInt(__func__);
  
  TypedCell* aTypedCell = m_pDataStack->aTopVal();
  
  eCN eAssertClassNo = (eCN)iAssertClassNo;
  eCN eClassNo = aTypedCell->eType();
  
  if (CN_PTRVAL == eClassNo && eAssertClassNo != CN_PTRVAL) {
    //PtrVal* aPtrVal = _aPtrVal(aAnyVal);
    //eClassNo = aPtrVal->pObj()->eClass();
    // TypedCell* aTypedCell = _aTypedCell(aAnyVal);
    eClassNo = aTypedCell->pObj()->eClass();
  }
  
  if ( eClassNo != eAssertClassNo )
    vThrow(FK_CHECK,VL_FATAL,__func__,__FILE__,__LINE__,
           "assert::class in word"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* assert::class_0 -- assert class of value[0] on stack                       */
/*----------------------------------------------------------------------------*/
void Step::vAssertClass_0(void)
{
  ENTRY();
  
  i8 iAssertClassNo = m_pDataStack->iPopAsInt(__func__);
  
  TypedCell* aTypedCell = m_pDataStack->aPeekVal(0);
  
  eCN eAssertClassNo = (eCN)iAssertClassNo;
  eCN eClassNo = aTypedCell->eType();
  
  if (CN_PTRVAL == eClassNo && eAssertClassNo != CN_PTRVAL) {
//    PtrVal* aPtrVal = _aPtrVal(aAnyVal);
//    eClassNo = aPtrVal->pObj()->eClass();
//    TypedCell* aTypedCell = _aTypedCell(aAnyVal);
    eClassNo = aTypedCell->pObj()->eClass();
  }
  
  if ( eClassNo != eAssertClassNo )
    vThrow(FK_CHECK,VL_FATAL,__func__,__FILE__,__LINE__,
           "assert::class_0 in word"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* assert::class_1 -- assert class of value[1] on stack                       */
/*----------------------------------------------------------------------------*/
void Step::vAssertClass_1(void)
{
  ENTRY();
  
  i8 iAssertClassNo = m_pDataStack->iPopAsInt(__func__);
  
  TypedCell* aTypedCell = m_pDataStack->aPeekVal(1);
  
  eCN eAssertClassNo = (eCN)iAssertClassNo;
  eCN eClassNo = aTypedCell->eType();
  
  if (CN_PTRVAL == eClassNo && eAssertClassNo != CN_PTRVAL) {
    //PtrVal* aPtrVal = _aPtrVal(aAnyVal);
    //eClassNo = aPtrVal->pObj()->eClass();
    //TypedCell* aTypedCell = _aTypedCell(aAnyVal);
    eClassNo = aTypedCell->pObj()->eClass();  }
  
  if ( eClassNo != eAssertClassNo )
    vThrow(FK_CHECK,VL_FATAL,__func__,__FILE__,__LINE__,
           "assert::class_1 in word"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* assert::class_2 -- assert class of value[2] on stack                       */
/*----------------------------------------------------------------------------*/
void Step::vAssertClass_2(void)
{
  ENTRY();
  
  i8 iAssertClassNo = m_pDataStack->iPopAsInt(__func__);
  
  TypedCell* aTypedCell = m_pDataStack->aPeekVal(2);
  
  eCN eAssertClassNo = (eCN)iAssertClassNo;
  eCN eClassNo = aTypedCell->eType();
  
  if (CN_PTRVAL == eClassNo && eAssertClassNo != CN_PTRVAL) {
//    PtrVal* aPtrVal = _aPtrVal(aAnyVal);
//    eClassNo = aPtrVal->pObj()->eClass();
//    TypedCell* aTypedCell = _aTypedCell(aAnyVal);
    eClassNo = aTypedCell->pObj()->eClass();
  }
  
  if ( eClassNo != eAssertClassNo )
    vThrow(FK_CHECK,VL_FATAL,__func__,__FILE__,__LINE__,
           "assert::class_2 in word"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* assert::class_3 -- assert class of value[3] on stack                       */
/*----------------------------------------------------------------------------*/
void Step::vAssertClass_3(void)
{
  ENTRY();
  
  i8 iAssertClassNo = m_pDataStack->iPopAsInt(__func__);
  
  TypedCell* aTypedCell = m_pDataStack->aPeekVal(3);
  
  eCN eAssertClassNo = (eCN)iAssertClassNo;
  eCN eClassNo = aTypedCell->eType();
  
  if (CN_PTRVAL == eClassNo && eAssertClassNo != CN_PTRVAL) {
//    PtrVal* aPtrVal = _aPtrVal(aAnyVal);
//    eClassNo = aPtrVal->pObj()->eClass();
//    TypedCell* aTypedCell = _aTypedCell(aAnyVal);
    eClassNo = aTypedCell->pObj()->eClass();
  }
  
  if ( eClassNo != eAssertClassNo )
    vThrow(FK_CHECK,VL_FATAL,__func__,__FILE__,__LINE__,
           "assert::class_3 in word"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* assert::class_4 -- assert class of value[4] on stack                       */
/*----------------------------------------------------------------------------*/
void Step::vAssertClass_4(void)
{
  ENTRY();
  
  i8 iAssertClassNo = m_pDataStack->iPopAsInt(__func__);
  
  TypedCell* aTypedCell = m_pDataStack->aPeekVal(4);
  
  eCN eAssertClassNo = (eCN)iAssertClassNo;
  eCN eClassNo = aTypedCell->eType();
  
  if (CN_PTRVAL == eClassNo && eAssertClassNo != CN_PTRVAL) {
//    PtrVal* aPtrVal = _aPtrVal(aAnyVal);
//    eClassNo = aPtrVal->pObj()->eClass();
//    TypedCell* aTypedCell = _aTypedCell(aAnyVal);
    eClassNo = aTypedCell->pObj()->eClass(); 
  }
  
  if ( eClassNo != eAssertClassNo )
    vThrow(FK_CHECK,VL_FATAL,__func__,__FILE__,__LINE__,
           "assert::class_4 in word"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* assert::stack_0 -- asssert 0 values on stack                               */
/*----------------------------------------------------------------------------*/
void Step::vAssertStack_0(void)
{
  ENTRY();

  i8 iStackValues = m_pDataStack->nValues();
  
  if ( ! (0 == iStackValues)  ) {
    
    m_pTask->m_pForth->vContentDump();

    vThrow(FK_CHECK,VL_FATAL,__func__,__FILE__,__LINE__,
           "assert::stack_0 in word"_Stz);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* assert::stack_0+ -- asssert 0 or more values on stack                      */
/*----------------------------------------------------------------------------*/
void Step::vAssertStack_0more(void)
{
  ENTRY();
  
  i8 iStackValues = m_pDataStack->nValues();
  
  if ( ! (0 <= iStackValues) )
    vThrow(FK_CHECK,VL_FATAL,__func__,__FILE__,__LINE__,
           "assert::stack_0+ in word"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* assert::stack_1 -- asssert 1 values on stack                               */
/*----------------------------------------------------------------------------*/
void Step::vAssertStack_1(void)
{
  ENTRY();
  
  i8 iStackValues = m_pDataStack->nValues();
  
  if ( ! (1 == iStackValues)  )
    vThrow(FK_CHECK,VL_FATAL,
           __func__,__FILE__,__LINE__,"assert::stack_1 in word"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* assert::stack_1+ -- asssert 0 or more values on stack                      */
/*----------------------------------------------------------------------------*/
void Step::vAssertStack_1more(void)
{
  ENTRY();
  
  i8 iStackValues = m_pDataStack->nValues();
  
  if ( ! (1 <= iStackValues) )
    vThrow(FK_CHECK,VL_FATAL,
           __func__,__FILE__,__LINE__,"assert::stack_1+ in word"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* assert::stack_2 -- asssert 2 values on stack                               */
/*----------------------------------------------------------------------------*/
void Step::vAssertStack_2(void)
{
  ENTRY();
  
  i8 iStackValues = m_pDataStack->nValues();
  
  if ( ! (2 == iStackValues)  )
    vThrow(FK_CHECK,VL_FATAL,
           __func__,__FILE__,__LINE__,"assert::stack_2 in word"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* assert::stack_2+ -- asssert 2 or more values on stack                      */
/*----------------------------------------------------------------------------*/
void Step::vAssertStack_2more(void)
{
  ENTRY();
  
  i8 iStackValues = m_pDataStack->nValues();
  
  if ( ! (2 <= iStackValues) )
    vThrow(FK_CHECK,VL_FATAL,
           __func__,__FILE__,__LINE__,"assert::stack_2+ in word"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* assert::stack_3 -- asssert 3 values on stack                               */
/*----------------------------------------------------------------------------*/
void Step::vAssertStack_3(void)
{
  ENTRY();
  
  i8 iStackValues = m_pDataStack->nValues();
  
  if ( ! (3 == iStackValues)  )
    vThrow(FK_CHECK,VL_FATAL,
           __func__,__FILE__,__LINE__,"assert::stack_3 in word"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* assert::stack_3+ -- asssert 3 or more values on stack                      */
/*----------------------------------------------------------------------------*/
void Step::vAssertStack_3more(void)
{
  ENTRY();
  
  i8 iStackValues = m_pDataStack->nValues();
  
  if ( ! (3 <= iStackValues) )
    vThrow(FK_CHECK,VL_FATAL,
           __func__,__FILE__,__LINE__,"assert::stack_3+ in word"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* assert::stack_4 -- asssert 4 values on stack                               */
/*----------------------------------------------------------------------------*/
void Step::vAssertStack_4(void)
{
  ENTRY();
  
  i8 iStackValues = m_pDataStack->nValues();
  
  if ( ! (4 == iStackValues)  )
    vThrow(FK_CHECK,VL_FATAL,
           __func__,__FILE__,__LINE__,"assert::stack_4 in word"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* assert::stack_4+ -- asssert 4 or more values on stack                      */
/*----------------------------------------------------------------------------*/
void Step::vAssertStack_4more(void)
{
  ENTRY();
  
  i8 iStackValues = m_pDataStack->nValues();
  
  if ( ! (4 <= iStackValues) )
    vThrow(FK_CHECK,VL_FATAL,
           __func__,__FILE__,__LINE__,"assert::stack_4+ in word"_Stz);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
