/*============================================================================*/
/* Steps_DataBuffer.cpp - DataBuffer steps group                              */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "LogicUnit.hpp"

#include "DataBuffer.hpp"
#include "SkipRoots.hpp"
#include "EntBuffer.hpp"
#include "IndexTree.hpp"
#include "TablePtn.hpp"
#include "Table.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_DataBuffer(void)
{
  vAddStep("DataBuffer[]"_Stz,             &Step::vDataBufferGet);
  vAddStep("DataBuffer[]\\clear"_Stz,      &Step::vDataBufferGetAndClear);
  vAddStep("DataBuffer[].delete"_Stz,      &Step::vDataBufferDelete);
  vAddStep("DataBuffer[].get"_Stz,         &Step::vDataBufferGet);
  vAddStep("DataBuffer[].isNotNull"_Stz,   &Step::vDataBufferIsNotNull);
  vAddStep("DataBuffer[].isNull"_Stz,      &Step::vDataBufferIsNull);
  vAddStep("DataBuffer[].set"_Stz,         &Step::vDataBufferSet);
  
}

/*----------------------------------------------------------------------------*/
/* DataBuffer[].delete -- operand delete                                       */
/*----------------------------------------------------------------------------*/
void Step::vDataBufferDelete(void)
{
  ENTRY();

  AM::vDeleteOperand(*m_pLogicUnit->m_xpDataBuffer);

  m_pLogicUnit->m_xpDataBuffer = nullptr;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* DataBuffer[].get -- fetch operand                                           */
/*----------------------------------------------------------------------------*/
void Step::vDataBufferGet(void)
{
  ENTRY();

  m_pDataStack->vPushPtr(*m_pLogicUnit->m_xpDataBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* DataBuffer[].getAndClear -- fetch operand and set to nullptr                */
/*----------------------------------------------------------------------------*/
void Step::vDataBufferGetAndClear(void)
{
  ENTRY();

  m_pDataStack->vPushPtr(*m_pLogicUnit->m_xpDataBuffer);

  m_pLogicUnit->m_xpDataBuffer = nullptr;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* DataBuffer[].isNotNull -- is RowBuffer not null ?                           */
/*----------------------------------------------------------------------------*/
void Step::vDataBufferIsNotNull(void)
{
  ENTRY();

  m_pDataStack->vPushInt(m_pLogicUnit->m_xpDataBuffer.bIsNull() ? 0 : 1);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* DataBuffer[].isNull -- is RowBuffer null ?                                  */
/*----------------------------------------------------------------------------*/
void Step::vDataBufferIsNull(void)
{
  ENTRY();

  m_pDataStack->vPushInt(m_pLogicUnit->m_xpDataBuffer.bIsNull() ? 1 : 0);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* DataBuffer[].set -- set operand                                             */
/*----------------------------------------------------------------------------*/
void Step::vDataBufferSet(void)
{
  ENTRY();

  Operand* pOperand = _pOperand(m_pDataStack->pPopPtr(__func__));

  ASSERT(! pOperand || m_p_TaskLogicUnit->m_xpDataBuffer.bIsNull());

  m_pLogicUnit->m_xpDataBuffer = _pDataBuffer(pOperand);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
