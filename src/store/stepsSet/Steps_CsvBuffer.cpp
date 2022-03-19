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
  vAddStep("csvBuffer::new"_Stz,        &Step::vCsvBufferNewOld);

  vAddStep("csvBuffer.operandDump"_Stz, &Step::vCsvBufferOperandDump);
  vAddStep("csvBuffer.print"_Stz,       &Step::vCsvBufferPrint);

  vAddStep("csvBuffer[].csvAppend"_Stz,   &Step::vCsvBufferCsvAppend);
  vAddStep("csvBuffer[].csvData"_Stz,     &Step::vCsvBufferCsvData);
  vAddStep("csvBuffer[].csvToRow"_Stz,    &Step::vCsvBufferCsvToRow);

  vAddStep("csvBuffer[]"_Stz,             &Step::vCsvBufferGet);
  vAddStep("csvBuffer[]\\clear"_Stz,      &Step::vCsvBufferGetAndClear);
  vAddStep("csvBuffer[].delete"_Stz,      &Step::vCsvBufferDelete);
  vAddStep("csvBuffer[].get"_Stz,         &Step::vCsvBufferGet);
  vAddStep("csvBuffer[].isNotNull"_Stz,   &Step::vCsvBufferIsNotNull);
  vAddStep("csvBuffer[].isNull"_Stz,      &Step::vCsvBufferIsNull);
  vAddStep("csvBuffer[].isReport"_Stz,    &Step::vCsvBufferIsReport);
  vAddStep("csvBuffer[].new"_Stz,         &Step::vCsvBufferNew);
  vAddStep("csvBuffer[].report"_Stz,      &Step::vCsvBufferReport);
  vAddStep("csvBuffer[].set"_Stz,         &Step::vCsvBufferSet);
  vAddStep("csvBuffer[].writeToFile0"_Stz, &Step::vCsvBufferWriteToFile0);
}

/*----------------------------------------------------------------------------*/
/* csvBuffer::new -- create a new csv buffer                                  */
/*----------------------------------------------------------------------------*/
void Step::vCsvBufferNewOld(void)
{
  ENTRY();
  
  CsvBuffer* pCsvBuffer = new CsvBuffer;
    
  m_pDataStack->vPushPtr(pCsvBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

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
/* CsvBuffer[].csvAppend -- append a csv row to the csv buffer                */
/*----------------------------------------------------------------------------*/
void Step::vCsvBufferCsvAppend(void)
{
  ENTRY();

  s256 sCsvRow;
  m_pDataStack->vPopStr(sCsvRow,__func__);

  CsvBuffer* pCsvBuffer = _pCsvBuffer(*m_pLogicUnit->m_xpCsvBuffer);
  i8 nBytes = sCsvRow.nSize();
  if (pCsvBuffer->nFreeBytes() < nBytes + 1)
      ASSERT(0);

  Stz* szRecord = pCsvBuffer->szAllotRecord(nBytes - 1);
  StrLib::vCopyLen(szRecord->azChar(),sCsvRow.azChar(),nBytes - 1);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* CsvBuffer[].csvData -- return csvBuffer data                               */
/*----------------------------------------------------------------------------*/
void Step::vCsvBufferCsvData(void)
{
  ENTRY();

  CsvBuffer* pCsvBuffer = _pCsvBuffer(*m_pLogicUnit->m_xpCsvBuffer);
  char* data = (char*)pCsvBuffer->ayDataArea();
  m_pDataStack->vPushStz((Stz*)data);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* CsvBuffer[].csvToRow -- convert csv buffer to row buffer                   */
/*----------------------------------------------------------------------------*/
void Step::vCsvBufferCsvToRow(void)
{
  ENTRY();

  CsvBuffer* pCsvBuffer =
      _pCsvBuffer(*m_pLogicUnit->m_xpCsvBuffer);

  ToRow* pToRow = *m_pLogicUnit->m_xpToRow;

  bool b = pToRow->bConvertCsv(pCsvBuffer);
  UNUSED(b);

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

  AM::vDeleteOperand(*m_pLogicUnit->m_xpCsvBuffer);

  m_pLogicUnit->m_xpCsvBuffer = nullptr;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* CsvBuffer[].get -- fetch operand                                           */
/*----------------------------------------------------------------------------*/
void Step::vCsvBufferGet(void)
{
  ENTRY();

  m_pDataStack->vPushPtr(*m_pLogicUnit->m_xpCsvBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* CsvBuffer[].getAndClear -- fetch operand and set to nullptr                */
/*----------------------------------------------------------------------------*/
void Step::vCsvBufferGetAndClear(void)
{
  ENTRY();

  m_pDataStack->vPushPtr(*m_pLogicUnit->m_xpCsvBuffer);

  m_pLogicUnit->m_xpCsvBuffer = nullptr;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* CsvBuffer[].isNotNull -- is RowBuffer not null ?                           */
/*----------------------------------------------------------------------------*/
void Step::vCsvBufferIsNotNull(void)
{
  ENTRY();

  m_pDataStack->vPushInt(m_pLogicUnit->m_xpCsvBuffer.bIsNull() ? 0 : 1);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* CsvBuffer[].isNull -- is RowBuffer null ?                                  */
/*----------------------------------------------------------------------------*/
void Step::vCsvBufferIsNull(void)
{
  ENTRY();

  m_pDataStack->vPushInt(m_pLogicUnit->m_xpCsvBuffer.bIsNull() ? 1 : 0);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* CsvBuffer[].isReport -- is csvBuffer for report processing ?               */
/*----------------------------------------------------------------------------*/
void Step::vCsvBufferIsReport(void)
{
  ENTRY();

  Operand* pOperand = *m_pLogicUnit->m_xpCsvBuffer;
  m_pDataStack->vPushInt(pOperand->eTailProcessing() == PT_REPORT ? true : false);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* CsvBuffer[].new -- new a csv buffer                                        */
/*----------------------------------------------------------------------------*/
void Step::vCsvBufferNew(void)
{
  ENTRY();

  ASSERT (m_p_TaskLogicUnit->m_xpCsvBuffer.bIsNull());

  m_pLogicUnit->m_xpCsvBuffer = new CsvBuffer;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* CsvBuffer[].report -- mark csvBuffer for report processing ?               */
/*----------------------------------------------------------------------------*/
void Step::vCsvBufferReport(void)
{
  ENTRY();

  Operand* pOperand = *m_pLogicUnit->m_xpCsvBuffer;
  pOperand->vTailProcessing(PT_REPORT);

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

  m_pLogicUnit->m_xpCsvBuffer = _pCsvBuffer(pOperand);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* CsvBuffer[].writeToFile0 -- write to file0[]                               */
/*----------------------------------------------------------------------------*/
void Step::vCsvBufferWriteToFile0(void)
{
  ENTRY();

  ASSERT(*m_p_TaskLogicUnit->m_xpCsvBuffer);
  ASSERT(*m_p_TaskLogicUnit->m_xpFile0);
  ASSERT(m_p_TaskLogicUnit->m_xpFile0->bIsOpen());

  CsvBuffer* pCsvBuffer = _pCsvBuffer(*m_pLogicUnit->m_xpCsvBuffer);
  char* aData = (char*)pCsvBuffer->ayDataArea();

  m_pLogicUnit->m_xpFile0->vPrint("%s"_Stz,aData);
  //m_p_TaskLogicUnit->m_xpFile0->vFlush();


}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
