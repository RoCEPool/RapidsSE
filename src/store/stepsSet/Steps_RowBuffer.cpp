/*============================================================================*/
/* Steps_RowBuffer.cpp - RowBuffer step group                                 */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "LogicUnit.hpp"
#include "RowBuffer.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_RowBuffer(void)
{
  vAddStep("rowBuffer.operandDump"_Stz,      &Step::vRowBufferOperandDump);
  vAddStep("RowBuffer[]"_Stz,                &Step::vRowBufferGet);
  vAddStep("RowBuffer[]\\clear"_Stz,         &Step::vRowBufferGetAndClear);
  vAddStep("RowBuffer[].copy"_Stz,           &Step::vRowBufferCopy);
  vAddStep("RowBuffer[].copyIntoSame"_Stz,   &Step::vRowBufferCopyIntoSame);
  vAddStep("RowBuffer[].delete"_Stz,         &Step::vRowBufferDelete);
  vAddStep("RowBuffer[].get"_Stz,            &Step::vRowBufferGet);
  vAddStep("RowBuffer[].getRows"_Stz,        &Step::vRowBufferGetRows);
  vAddStep("RowBuffer[].insdelIntoTable"_Stz,&Step::vRowBufferInsdelIntoTable);
  vAddStep("RowBuffer[].isNotNull"_Stz,      &Step::vRowBufferIsNotNull);
  vAddStep("RowBuffer[].isNull"_Stz,         &Step::vRowBufferIsNull);
  vAddStep("RowBuffer[].iterate"_Stz,        &Step::vRowBufferIterate);
  vAddStep("RowBuffer[].set"_Stz,            &Step::vRowBufferSet);
  vAddStep("RowBuffer[].toCsv"_Stz,          &Step::vRowBufferToCsv);
  vAddStep("RowBuffer[].toCsv_OLD"_Stz,      &Step::vRowBufferToCsv_OLD);
}

/*----------------------------------------------------------------------------*/
/* rowBuffer.operandDump -- operand dump of rowBuffer                         */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferOperandDump(void)
{
  ENTRY();
  
  RowBuffer* pRowBuffer = _pRowBuffer(m_pDataStack->pPopPtr(__func__));

  i8 iDetail = m_pDataStack->iPopInt(__func__);
  
  if (iDetail)
    pRowBuffer->vDumpAllRows();
  
  pRowBuffer->vSlabReport();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBuffer[].copy-- copy the row buffer                                     */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferCopy(void)
{
  ENTRY();

  RowBuffer* pRowBuffer = *m_p_TaskLogicUnit->m_xpRowBuffer;
  RowBuffer* pNewRowBuffer = new RowBuffer;

  RowPix* pPix = pRowBuffer->pRowPix();
  pNewRowBuffer->vCopyRowPix(pPix);

  i8 nRows = pRowBuffer->nRows();

  for (i8 i_Row = 0; i_Row < nRows; i_Row++) {
    Row* pRow = pRowBuffer->pRowAddress(i_Row);
    Row* pNewRow = pNewRowBuffer->pAllotRow(pRow);
    RASSERT(pNewRow);
  }

  m_pDataStack->vPushPtr(pNewRowBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBuffer[].copyIntoSame -- copy the row buffer into same row              */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferCopyIntoSame(void)
{
  ENTRY();

  RowBuffer* pRowBuffer = *m_p_TaskLogicUnit->m_xpRowBuffer;
  RowBuffer* pNewRowBuffer = new RowBuffer;

  RowPix* pPix = pRowBuffer->pRowPix();
  pNewRowBuffer->vCopyRowPix(pPix);

  // get no of rows
  i8 nRows = pRowBuffer->nRows();

  // allot a row in the new rowBuffer
  Row* pNewRow = pNewRowBuffer->pAllotRow();

  for (i8 i_Row = 0; i_Row < nRows; i_Row++) {
    Row* pRow = pRowBuffer->pRowAddress(i_Row);
    Row* pSameRow = pNewRowBuffer->pCopyIntoRow(pRow, pNewRow);
    RASSERT(pNewRow == pSameRow);
  }

  m_pDataStack->vPushPtr(pNewRowBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBuffer[].delete -- operand delete                                       */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferDelete(void)
{
  ENTRY();

  AM::vDeleteOperand(*m_p_TaskLogicUnit->m_xpRowBuffer);

  m_p_TaskLogicUnit->m_xpRowBuffer = nullptr;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBuffer[].get -- fetch operand                                           */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferGet(void)
{
  ENTRY();

  m_pDataStack->vPushPtr(*m_p_TaskLogicUnit->m_xpRowBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBuffer[].getAndClear -- fetch operand and set to nullptr                */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferGetAndClear(void)
{
  ENTRY();

  m_pDataStack->vPushPtr(*m_p_TaskLogicUnit->m_xpRowBuffer);

  m_p_TaskLogicUnit->m_xpRowBuffer = nullptr;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBuffer[].getRows -- getRows the row buffer                              */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferGetRows(void)
{
  ENTRY();

  RowBuffer* pRowBuffer = *m_p_TaskLogicUnit->m_xpRowBuffer;

  i8 nRows = pRowBuffer->nRows();
  RowPix* pPix = pRowBuffer->pRowPix();

  // get rows the buffer
  for (i8 i_Row = 0; i_Row < nRows; i_Row++) {
    Row* pRow = pRowBuffer->pRowAddress(i_Row);

    for (i8 i_Col = 0; i_Col < pPix->m_nColumns; i_Col++) {

      ColPix xColPix = pPix->m_xaColPix[i_Col];
      if (DM_VCH == xColPix.m_uDatum) {
        IGNORE_RETURN(pRow->szColVal_ofs(i_Col));
      }
      else {
        IGNORE_RETURN(pRow->iColVal_int(i_Col));
      }

    }

  }

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBuffer[].insdelIntoTable -- insert-delete rowBuffer into the table      */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferInsdelIntoTable(void)
{
  ENTRY();

//  ASSERT(*m_p_TaskLogicUnit->m_xpRowBuffer);
//
//  Op* pOp = m_pJob->m_pOp;
//  PtnId tPtnId = pOp->tPtnId();
//  i8 i_TablePtn = Table::iPtnNoToTablePtn(tPtnId);
//

//  // will delete this buffer
//  RowBuffer* pRowBuffer = _pRowBuffer(*m_p_TaskLogicUnit->m_xpRowBuffer);
//  pRowBuffer->vIsPermanent(false);
//  m_p_TaskLogicUnit->m_xpTable->vInsdelBuffer(C_UNKNOWN, i_TablePtn, pRowBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBuffer[].isNotNull -- is RowBuffer not null ?                           */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferIsNotNull(void)
{
  ENTRY();

  m_pDataStack->vPushInt(m_p_TaskLogicUnit->m_xpRowBuffer.bIsNull() ? 0 : 1);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBuffer[].isNull -- is RowBuffer null ?                                  */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferIsNull(void)
{
  ENTRY();

  m_pDataStack->vPushInt(m_p_TaskLogicUnit->m_xpRowBuffer.bIsNull() ? 1 : 0);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBuffer[].iterate -- iterate the row buffer                              */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferIterate(void)
{
  ENTRY();

  RowBuffer* pRowBuffer = *m_p_TaskLogicUnit->m_xpRowBuffer;

  i8 nRows = pRowBuffer->nRows();

  // iterate the buffer
  for (i8 i_Row = 0; i_Row < nRows; i_Row++) {
    IGNORE_RETURN(pRowBuffer->pRowAddress(i_Row));
  }

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBuffer[].set -- set operand                                             */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferSet(void)
{
  ENTRY();

  Operand* pOperand = _pOperand(m_pDataStack->pPopPtr(__func__));

  ASSERT(! pOperand || m_p_TaskLogicUnit->m_xpRowBuffer.bIsNull());

  m_p_TaskLogicUnit->m_xpRowBuffer = _pRowBuffer(pOperand);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBuffer[].toCsv -- convert rowBuffer operand to csvBuffer                */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferToCsv(void)
{
  ENTRY();

  RowBuffer* pRowBuffer = _pRowBuffer(*m_p_TaskLogicUnit->m_xpRowBuffer);
  
  Tub* pTub = _pTub(m_pDataStack->pPopPtr(__func__));
  
  pRowBuffer->vToCsv(pTub);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBuffer[].toCsv -- convert rowBuffer operand to csvBuffer                */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferToCsv_OLD(void)
{
  ENTRY();

  RowBuffer* pRowBuffer = _pRowBuffer(*m_p_TaskLogicUnit->m_xpRowBuffer);
  m_pDataStack->vPushPtr(pRowBuffer->pToCsvBuffer());

  delete pRowBuffer;

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
