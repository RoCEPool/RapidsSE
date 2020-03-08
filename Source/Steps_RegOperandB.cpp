/*============================================================================*/
/* Steps_Operand.cpp -- Operand step group                                    */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_RegOperandB(void)
{
  vAddStep("OperandB[].addIntInfoSet"_Stz,  &Step::vRegOperandBAddIntInfoSet);
  vAddStep("OperandB[].addStrInfoSet"_Stz,  &Step::vRegOperandBAddStrInfoSet);
  vAddStep("OperandB[].appendClose"_Stz,    &Step::vRegOperandBAppendClose);
  vAddStep("OperandB[].delete"_Stz,         &Step::vRegOperandBDelete);
  vAddStep("OperandB[].get"_Stz,            &Step::vRegOperandBGet);
  vAddStep("OperandB[].getIntInfoSet"_Stz,  &Step::vRegOperandBGetIntInfoSet);
  vAddStep("OperandB[].getStrInfoSet"_Stz,  &Step::vRegOperandBGetStrInfoSet);
  vAddStep("OperandB[].isNull"_Stz,         &Step::vRegOperandBIsNull);
  vAddStep("OperandB[].newInfoSet"_Stz,     &Step::vRegOperandBNewInfoSet);
  vAddStep("OperandB[].pipeout"_Stz,        &Step::vRegOperandBPipeOut);
  vAddStep("OperandB[].set"_Stz,            &Step::vRegOperandBSet);
}

/*----------------------------------------------------------------------------*/
/* OperandB[].addIntInfoSet -- add int value to info set                      */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandBAddIntInfoSet(void)
{
  ENTRY(); // ENTRY("vRegOperandBAddIntInfoSet"_Stz);
  
//  InfoSet* pInfoSet = _pInfoSet(*m_pRegSet->m_xpOperandB);
//
//  s256 sName;
//  m_pDataStack->vPopStr(sName, __func__);
//  i8 iValue = m_pDataStack->iPopInt(__func__);
//
//  pInfoSet->vAdd(iValue);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* OperandB[].addStrInfoSet -- add string value to info set                   */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandBAddStrInfoSet(void)
{
  ENTRY(); // ENTRY("vRegOperandBAddStrInfoSet"_Stz);
  
//  InfoSet* pInfoSet = _pInfoSet(*m_pRegSet->m_xpOperandB);
//
//  s256 sName;
//  m_pDataStack->vPopStr(sName, __func__);
//
//  s256 sValue;
//  m_pDataStack->vPopStr(sValue, __func__);
//
//  pInfoSet->vAdd(sValue);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* OperandB[].get -- fetch operand                                             */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandBAppendClose(void)
{
  ENTRY(); // ENTRY("vOperandBAppendClose"_Stz);
  
  CrwBuffer* pCrwBuffer = _pCrwBuffer(*m_pRegSet->m_xpOperandB);
  pCrwBuffer->vAppendClose();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* OperandB[].delete -- operand delete                                        */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandBDelete(void)
{
  ENTRY(); // ENTRY("vRegOperandBDelete"_Stz);
  
  AM::vDeleteOperand(*m_pRegSet->m_xpOperandB);
  
  m_pRegSet->m_xpOperandB = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* OperandB[].get -- fetch operand                                             */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandBGet(void)
{
  ENTRY(); // ENTRY("vOperandBGet"_Stz);
  
  m_pDataStack->vPushPtr(*m_pRegSet->m_xpOperandB);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* OperandB[].getIntInfoSet -- get int value from info set                    */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandBGetIntInfoSet(void)
{
  ENTRY(); // ENTRY("vRegOperandBAddIntInfoSet"_Stz);
  
//  InfoSet* pInfoSet = _pInfoSet(*m_pRegSet->m_xpOperandB);
//
//  s256 sName;
//  m_pDataStack->vPopStr(sName, __func__);
//  i8 iValue = pInfoSet->iValue(sName);
//
//  m_pDataStack->vPushInt(iValue);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* OperandB[].getStrInfoSet -- get string value from info set                 */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandBGetStrInfoSet(void)
{
  ENTRY(); // ENTRY("vRegOperandBGetStrInfoSet"_Stz);
  
//  InfoSet* pInfoSet = _pInfoSet(*m_pRegSet->m_xpOperandB);
//
//  s256 sName;
//  m_pDataStack->vPopStr(sName, __func__);
//  s256 szValue = pInfoSet->szValue(sName);
//
//  m_pDataStack->vPushStr(szValue);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* OperandB[].isNull -- is operandB null ?                                    */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandBIsNull(void)
{
  ENTRY(); // ENTRY("vOperandBIsNull"_Stz);
  
  m_pDataStack->vPushInt(m_pRegSet->m_xpOperandB.bIsNull() ? 1 : 0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* OperandB[].newInfoSet -- new a infoset operand                             */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandBNewInfoSet(void)
{
  ENTRY(); // ENTRY("vRegOperandBNewInfoSet"_Stz);
  
  ASSERT (m_pRegSet->m_xpOperandB.bIsNull());
  
  m_pRegSet->m_xpOperandB = new InfoSet();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* OperandB[].pipeOut -- append operand onto next operator in pipeline         */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandBPipeOut(void)
{
  ENTRY(); // ENTRY("vOperandBPipeOut"_Stz);
  
  ASSERT(*m_pRegSet->m_xpOperandB);
  
  Op* pOp = m_pTask->m_pJob->m_pOp; //  pJob_Universal->m_pOp;
  
  pOp->vPipeOut(*m_pRegSet->m_xpOperandB);
  
  m_pRegSet->m_xpOperandB = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* OperandB[].set -- set operand                                               */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandBSet(void)
{
  ENTRY(); // ENTRY("vOperandBSet"_Stz);
  
  ASSERT(m_pRegSet->m_xpOperandB.bIsNull());
  m_pRegSet->m_xpOperandB = _pOperand(m_pDataStack->pPopPtr(__func__));
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
