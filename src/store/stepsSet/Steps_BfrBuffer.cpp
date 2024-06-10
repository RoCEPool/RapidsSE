/*============================================================================*/
/* Steps_CsvBuffer.cpp - BfrBuffer step group                                 */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "LogicUnit.hpp"
#include "BfrBuffer.hpp"


/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_BfrBuffer(void)
{

  vAddStep("bfrBuffer.operandDump"_Stz, &Step::vBfrBufferOperandDump);
  //vAddStep("bfrBuffer.print"_Stz,       &Step::vBfrBufferPrint);

  vAddStep("bfrBuffer[].csvAppend"_Stz,   &Step::vBfrBufferCsvAppend);
  vAddStep("bfrBuffer[].csvData"_Stz,     &Step::vBfrBufferCsvData);
  vAddStep("bfrBuffer[]"_Stz,             &Step::vBfrBufferGet);
  vAddStep("bfrBuffer[]\\clear"_Stz,      &Step::vBfrBufferGetAndClear);
  vAddStep("bfrBuffer[].delete"_Stz,      &Step::vBfrBufferDelete);
  vAddStep("bfrBuffer[].get"_Stz,         &Step::vBfrBufferGet);
  vAddStep("bfrBuffer[].isNotNull"_Stz,   &Step::vBfrBufferIsNotNull);
  vAddStep("bfrBuffer[].isNull"_Stz,      &Step::vBfrBufferIsNull);
  vAddStep("bfrBuffer[].new"_Stz,         &Step::vBfrBufferNew);
  vAddStep("bfrBuffer[].set"_Stz,         &Step::vBfrBufferSet);
}

/*----------------------------------------------------------------------------*/
/* bfrBuffer::new -- create a new csv buffer                                  */
/*----------------------------------------------------------------------------*/
//void Step::vBfrBufferNewOld(void)
//{
//  ENTRY();
//
//  BfrBuffer* pBfrBuffer = new BfrBuffer;
//
//  m_pDataStack->vPushPtr(pBfrBuffer);
//
//  EXIT();
//}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* bfrBuffer.operandDump -- operand dump of bfrBuffer                         */
/*----------------------------------------------------------------------------*/
void Step::vBfrBufferOperandDump(void)
{
  ENTRY();
//
//  BfrBuffer* pBfrBuffer = _pBfrBuffer(m_pDataStack->pPopPtr(__func__));
//
//  i8 iDetail = m_pDataStack->iPopInt(__func__);
//
//  if (iDetail)
//    pBfrBuffer->vPrintRecords();
//
//  vTrace("BfrBuffer: %lld(B)\n"_Stz,pBfrBuffer->nDataBytes());
//
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* BfrBuffer[].csvAppend -- append a csv row to the csv buffer                */
/*----------------------------------------------------------------------------*/
void Step::vBfrBufferCsvAppend(void)
{
  ENTRY();

//  s256 sCsvRow;
//  m_pDataStack->vPopStr(sCsvRow,__func__);
//
//  BfrBuffer* pBfrBuffer = _pBfrBuffer(*m_pLogicUnit->m_xpCsvBuffer);
//  i8 nBytes = sCsvRow.nSize();
//  if (pBfrBuffer->nFreeBytes() < nBytes + 1) {
//    ASSERT(0);
//  }
//
//  Stz* szRecord = pBfrBuffer->szAllotRecord(nBytes - 1);
//  StrLib::vCopy(szRecord->azChar(), nBytes, sCsvRow.azChar());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* BfrBuffer[].csvData -- return bfrBuffer data                               */
/*----------------------------------------------------------------------------*/
void Step::vBfrBufferCsvData(void)
{
  ENTRY();

//  BfrBuffer* pBfrBuffer = _pBfrBuffer(*m_pLogicUnit->m_xpCsvBuffer);
//  char* data = (char*)pBfrBuffer->ayDataArea();
//  m_pDataStack->vPushStz((Stz*)data);

  EXIT();
}


/*----------------------------------------------------------------------------*/
/* BfrBuffer[].delete -- operand delete                                       */
/*----------------------------------------------------------------------------*/
void Step::vBfrBufferDelete(void)
{
  ENTRY();

//  AM::vDeleteOperand(*m_pLogicUnit->m_xpBfrBuffer,nullptr);
//
//  m_pLogicUnit->m_xpBfrBuffer = nullptr;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* BfrBuffer[].get -- fetch operand                                           */
/*----------------------------------------------------------------------------*/
void Step::vBfrBufferGet(void)
{
  ENTRY();

  m_pDataStack->vPushPtr(*m_pLogicUnit->m_xpBfrBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* BfrBuffer[].getAndClear -- fetch operand and set to nullptr                */
/*----------------------------------------------------------------------------*/
void Step::vBfrBufferGetAndClear(void)
{
  ENTRY();

  m_pDataStack->vPushPtr(*m_pLogicUnit->m_xpBfrBuffer);

  m_pLogicUnit->m_xpBfrBuffer = nullptr;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* BfrBuffer[].isNotNull -- is RowBuffer not null ?                           */
/*----------------------------------------------------------------------------*/
void Step::vBfrBufferIsNotNull(void)
{
  ENTRY();

  m_pDataStack->vPushInt(m_pLogicUnit->m_xpBfrBuffer.bIsNull() ? 0 : 1);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* BfrBuffer[].isNull -- is RowBuffer null ?                                  */
/*----------------------------------------------------------------------------*/
void Step::vBfrBufferIsNull(void)
{
  ENTRY();

  m_pDataStack->vPushInt(m_pLogicUnit->m_xpBfrBuffer.bIsNull() ? 1 : 0);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* BfrBuffer[].new -- new a csv buffer                                        */
/*----------------------------------------------------------------------------*/
void Step::vBfrBufferNew(void)
{
  ENTRY();

  ASSERT (m_pLogicUnit->m_xpBfrBuffer.bIsNull());

  m_pLogicUnit->m_xpBfrBuffer = new BfrBuffer;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* BfrBuffer[].set -- set operand                                             */
/*----------------------------------------------------------------------------*/
void Step::vBfrBufferSet(void)
{
  ENTRY();

  Operand* pOperand = _pOperand(m_pDataStack->pPopPtr(__func__));

  ASSERT(! pOperand || m_pLogicUnit->m_xpBfrBuffer.bIsNull());

  m_pLogicUnit->m_xpBfrBuffer = _pBfrBuffer(pOperand);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
