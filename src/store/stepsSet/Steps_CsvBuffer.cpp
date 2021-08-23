/*============================================================================*/
/* Steps_CsvBuffer.cpp - CsvBuffer step group                                 */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "LogicUnit.hpp"
#include "CsvBuffer.hpp"


/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_CsvBuffer(void)
{
  vAddStep("csvBuffer::new"_Stz,        &Step::vCsvBufferNew);
  
  vAddStep("csvBuffer.delete"_Stz,      &Step::vCsvBufferDeleteOld);
  vAddStep("csvBuffer.operandDump"_Stz, &Step::vCsvBufferOperandDump);
  vAddStep("csvBuffer.print"_Stz,       &Step::vCsvBufferPrint);

  vAddStep("csvBuffer[]"_Stz,             &Step::vCsvBufferGet);
  vAddStep("csvBuffer[]\\clear"_Stz,      &Step::vCsvBufferGetAndClear);
  vAddStep("csvBuffer[].delete"_Stz,      &Step::vCsvBufferDelete);
  vAddStep("csvBuffer[].get"_Stz,         &Step::vCsvBufferGet);
  vAddStep("csvBuffer[].isNotNull"_Stz,   &Step::vCsvBufferIsNotNull);
  vAddStep("csvBuffer[].isNull"_Stz,      &Step::vCsvBufferIsNull);
  vAddStep("csvBuffer[].set"_Stz,         &Step::vCsvBufferSet);
}

/*----------------------------------------------------------------------------*/
/* csvBuffer::new -- create a new csv buffer                                  */
/*----------------------------------------------------------------------------*/
void Step::vCsvBufferNew(void)
{
  ENTRY();
  
  CsvBuffer* pCsvBuffer = new CsvBuffer;
    
  m_pDataStack->vPushPtr(pCsvBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* csvBuffer.delete -- delete                                                 */
/*----------------------------------------------------------------------------*/
void Step::vCsvBufferDeleteOld(void)
{
  ENTRY();
  
  CsvBuffer* pCsvBuffer = _pCsvBuffer(m_pDataStack->pPopPtr(__func__));

  delete pCsvBuffer;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* csvBuffer.operandDump -- operand dump of csvBuffer                         */
/*----------------------------------------------------------------------------*/
void Step::vCsvBufferOperandDump(void)
{
  ENTRY();
  
  CsvBuffer* pCsvBuffer = _pCsvBuffer(m_pDataStack->pPopPtr(__func__));

  i8 iDetail = m_pDataStack->iPopInt(__func__);

  if (iDetail)
    pCsvBuffer->vPrintRecords();
  
  vTrace("CsvBuffer: %lld(B)\n"_Stz,pCsvBuffer->nDataBytes());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* csvBuffer.print -- print                                                   */
/*----------------------------------------------------------------------------*/
void Step::vCsvBufferPrint(void)
{
  ENTRY();
  
  CsvBuffer* pCsvBuffer = _pCsvBuffer(m_pDataStack->pPopPtr(__func__));

  pCsvBuffer->vPrintRecords();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* CsvBuffer[].delete -- operand delete                                       */
/*----------------------------------------------------------------------------*/
void Step::vCsvBufferDelete(void)
{
  ENTRY();

  AM::vDeleteOperand(*m_p_TaskLogicUnit->m_xpCsvBuffer);

  m_p_TaskLogicUnit->m_xpCsvBuffer = nullptr;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* CsvBuffer[].get -- fetch operand                                           */
/*----------------------------------------------------------------------------*/
void Step::vCsvBufferGet(void)
{
  ENTRY();

  m_pDataStack->vPushPtr(*m_p_TaskLogicUnit->m_xpCsvBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* CsvBuffer[].getAndClear -- fetch operand and set to nullptr                */
/*----------------------------------------------------------------------------*/
void Step::vCsvBufferGetAndClear(void)
{
  ENTRY();

  m_pDataStack->vPushPtr(*m_p_TaskLogicUnit->m_xpCsvBuffer);

  m_p_TaskLogicUnit->m_xpCsvBuffer = nullptr;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* CsvBuffer[].isNotNull -- is RowBuffer not null ?                           */
/*----------------------------------------------------------------------------*/
void Step::vCsvBufferIsNotNull(void)
{
  ENTRY();

  m_pDataStack->vPushInt(m_p_TaskLogicUnit->m_xpCsvBuffer.bIsNull() ? 0 : 1);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* CsvBuffer[].isNull -- is RowBuffer null ?                                  */
/*----------------------------------------------------------------------------*/
void Step::vCsvBufferIsNull(void)
{
  ENTRY();

  m_pDataStack->vPushInt(m_p_TaskLogicUnit->m_xpCsvBuffer.bIsNull() ? 1 : 0);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* CsvBuffer[].set -- set operand                                             */
/*----------------------------------------------------------------------------*/
void Step::vCsvBufferSet(void)
{
  ENTRY();

  Operand* pOperand = _pOperand(m_pDataStack->pPopPtr(__func__));

  ASSERT(! pOperand || m_p_TaskLogicUnit->m_xpCsvBuffer.bIsNull());

  m_p_TaskLogicUnit->m_xpCsvBuffer = _pCsvBuffer(pOperand);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
