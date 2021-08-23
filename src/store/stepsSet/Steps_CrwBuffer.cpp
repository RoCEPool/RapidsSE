/*============================================================================*/
/* Steps_CrwBuffer.cpp - CrwBuffer steps group                                */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "LogicUnit.hpp"

#include "CrwBuffer.hpp"
#include "SkipRoots.hpp"
#include "EntBuffer.hpp"
#include "IndexTree.hpp"
#include "TablePtn.hpp"
#include "Table.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_CrwBuffer(void)
{
  vAddStep("CrwBuffer::new"_Stz,          &Step::vCrwBufferNew);
  
  vAddStep("CrwBuffer.delete"_Stz,        &Step::vCrwBufferDeleteOld);
  vAddStep("CrwBuffer.operandDump"_Stz,   &Step::vCrwBufferOperandDump);

  vAddStep("CrwBuffer[].appendClose"_Stz, &Step::vCrwBufferAppendClose);
  vAddStep("CrwBuffer[]"_Stz,             &Step::vCrwBufferGet);
  vAddStep("CrwBuffer[]\\clear"_Stz,      &Step::vCrwBufferGetAndClear);
  vAddStep("CrwBuffer[].delete"_Stz,      &Step::vCrwBufferDelete);
  vAddStep("CrwBuffer[].get"_Stz,         &Step::vCrwBufferGet);
  vAddStep("CrwBuffer[].isNotNull"_Stz,   &Step::vCrwBufferIsNotNull);
  vAddStep("CrwBuffer[].isNull"_Stz,      &Step::vCrwBufferIsNull);
  vAddStep("CrwBuffer[].set"_Stz,         &Step::vCrwBufferSet);
  
}

/*----------------------------------------------------------------------------*/
/* crwBuffer.delete -- delete a crw buffer                                    */
/*----------------------------------------------------------------------------*/
void Step::vCrwBufferDeleteOld(void)
{
  ENTRY();

  CrwBuffer* pCrwBuffer = _pCrwBuffer(m_pDataStack->pPopPtr(__func__));

  delete pCrwBuffer;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* crwBuffer::new -- new a crw buffer                                         */
/*----------------------------------------------------------------------------*/
void Step::vCrwBufferNew(void)
{
  ENTRY();

  CrwBuffer* pCrwBuffer = new CrwBuffer;

  m_pDataStack->vPushPtr(pCrwBuffer);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* crwBuffer.operandDump -- dump information about crw buffer                 */
/*----------------------------------------------------------------------------*/
void Step::vCrwBufferOperandDump(void)
{
  ENTRY();

  CrwBuffer* pCrwBuffer = _pCrwBuffer(m_pDataStack->pPopPtr(__func__));

  i8 iDetail = m_pDataStack->iPopInt(__func__);
  
  pCrwBuffer->vOperandDump(iDetail);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* CrwBuffer[].appendClose -- append close                                     */
/*----------------------------------------------------------------------------*/
void Step::vCrwBufferAppendClose(void)
{
  ENTRY();

  CrwBuffer* pCrwBuffer = _pCrwBuffer(*m_p_TaskLogicUnit->m_xpCrwBuffer);
  pCrwBuffer->vAppendClose();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBuffer[].delete -- operand delete                                       */
/*----------------------------------------------------------------------------*/
void Step::vCrwBufferDelete(void)
{
  ENTRY();

  AM::vDeleteOperand(*m_p_TaskLogicUnit->m_xpCrwBuffer);

  m_p_TaskLogicUnit->m_xpCrwBuffer = nullptr;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBuffer[].get -- fetch operand                                           */
/*----------------------------------------------------------------------------*/
void Step::vCrwBufferGet(void)
{
  ENTRY();

  m_pDataStack->vPushPtr(*m_p_TaskLogicUnit->m_xpCrwBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBuffer[].getAndClear -- fetch operand and set to nullptr                */
/*----------------------------------------------------------------------------*/
void Step::vCrwBufferGetAndClear(void)
{
  ENTRY();

  m_pDataStack->vPushPtr(*m_p_TaskLogicUnit->m_xpCrwBuffer);

  m_p_TaskLogicUnit->m_xpCrwBuffer = nullptr;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBuffer[].isNotNull -- is RowBuffer not null ?                           */
/*----------------------------------------------------------------------------*/
void Step::vCrwBufferIsNotNull(void)
{
  ENTRY();

  m_pDataStack->vPushInt(m_p_TaskLogicUnit->m_xpCrwBuffer.bIsNull() ? 0 : 1);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBuffer[].isNull -- is RowBuffer null ?                                  */
/*----------------------------------------------------------------------------*/
void Step::vCrwBufferIsNull(void)
{
  ENTRY();

  m_pDataStack->vPushInt(m_p_TaskLogicUnit->m_xpCrwBuffer.bIsNull() ? 1 : 0);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBuffer[].set -- set operand                                             */
/*----------------------------------------------------------------------------*/
void Step::vCrwBufferSet(void)
{
  ENTRY();

  Operand* pOperand = _pOperand(m_pDataStack->pPopPtr(__func__));

  ASSERT(! pOperand || m_p_TaskLogicUnit->m_xpCrwBuffer.bIsNull());

  m_p_TaskLogicUnit->m_xpCrwBuffer = _pCrwBuffer(pOperand);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
