/*============================================================================*/
/* Steps_CrwBuffer.cpp - CrwBuffer steps group                                */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "Flatten.hpp"

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
  vAddStep("CrwBuffer.duplicate"_Stz,     &Step::vCrwBufferDuplicate);
  vAddStep("CrwBuffer.operandDump"_Stz,   &Step::vCrwBufferOperandDump);

  vAddStep("CrwBuffer[].appendClose"_Stz, &Step::vCrwBufferAppendClose);
  vAddStep("CrwBuffer[].flatten"_Stz,     &Step::vCrwBufferFlatten);
  vAddStep("CrwBuffer[]"_Stz,             &Step::vCrwBufferGet);
  vAddStep("CrwBuffer[]\\clear"_Stz,      &Step::vCrwBufferGetAndClear);
  vAddStep("CrwBuffer[].delete"_Stz,      &Step::vCrwBufferDelete);
  vAddStep("CrwBuffer[].get"_Stz,         &Step::vCrwBufferGet);
  vAddStep("CrwBuffer[].inflate"_Stz,     &Step::vCrwBufferInflate);
  vAddStep("CrwBuffer[].insdelIntoTable"_Stz,&Step::vCrwBufferInsdelIntoTable);
  vAddStep("CrwBuffer[].isNotNull"_Stz,   &Step::vCrwBufferIsNotNull);
  vAddStep("CrwBuffer[].isNull"_Stz,      &Step::vCrwBufferIsNull);
  vAddStep("CrwBuffer[].isReport"_Stz,    &Step::vCrwBufferIsReport);
  vAddStep("CrwBuffer[].set"_Stz,         &Step::vCrwBufferSet);
  vAddStep("CrwBuffer[].toCsv"_Stz,       &Step::vCrwBufferToCsv);
  vAddStep("CrwBuffer[].toCsv_OLD"_Stz,   &Step::vCrwBufferToCsv_OLD);
  vAddStep("CrwBuffer[].upsertIntoTable"_Stz,&Step::vCrwBufferUpsertIntoTable);
  vAddStep("CrwBuffer[].writeToFile0"_Stz, &Step::vCrwBufferWriteToFile0);
  
}

/*----------------------------------------------------------------------------*/
/* crwBuffer.crwDuplicate -- duplicate crw buffer operand                     */
/*----------------------------------------------------------------------------*/
void Step::vCrwBufferDuplicate(void)
{
  ENTRY();

  CrwBuffer* pCrwBuffer = _pCrwBuffer(m_pDataStack->pPopPtr(__func__));

  CrwBuffer* pNewCrwBuffer = new CrwBuffer;
  pNewCrwBuffer->vCopy(pCrwBuffer);

  m_pDataStack->vPushPtr(pNewCrwBuffer);

  EXIT();
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
/* CrwBuffer[].delete -- operand delete                                       */
/*----------------------------------------------------------------------------*/
void Step::vCrwBufferDelete(void)
{
  ENTRY();

  AM::vDeleteOperand(*m_p_TaskLogicUnit->m_xpCrwBuffer);

  m_p_TaskLogicUnit->m_xpCrwBuffer = nullptr;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* CrwBuffer[].get -- fetch operand                                           */
/*----------------------------------------------------------------------------*/
void Step::vCrwBufferGet(void)
{
  ENTRY();

  m_pDataStack->vPushPtr(*m_p_TaskLogicUnit->m_xpCrwBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* CrwBuffer[].getAndClear -- fetch operand and set to nullptr                */
/*----------------------------------------------------------------------------*/
void Step::vCrwBufferGetAndClear(void)
{
  ENTRY();

  m_pDataStack->vPushPtr(*m_p_TaskLogicUnit->m_xpCrwBuffer);

  m_p_TaskLogicUnit->m_xpCrwBuffer = nullptr;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* CrwBuffer[].flatten -- execute flattened query against this buffer         */
/*----------------------------------------------------------------------------*/
void Step::vCrwBufferFlatten(void)
{
  ENTRY();

  CrwBuffer* pCrwBuffer = *m_p_TaskLogicUnit->m_xpCrwBuffer;

  Flatten* pFlatten = new Flatten;
  pFlatten->vTestFlatten(pCrwBuffer);

  delete pFlatten;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* CrwBuffer[].inflate -- inflate the crwbuffer                               */
/*----------------------------------------------------------------------------*/
void Step::vCrwBufferInflate(void)
{
  ENTRY();

  StreamDef* pStreamDef = _pStreamDef(m_pDataStack->pPopPtr(__func__));

  CrwBuffer* pCrwBuffer = *m_p_TaskLogicUnit->m_xpCrwBuffer;
  pCrwBuffer->vInflateRows(pStreamDef);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* CrwBuffer[].insdelIntoTable -- insert-delete crwBuffer into the table      */
/*----------------------------------------------------------------------------*/
void Step::vCrwBufferInsdelIntoTable(void)
{
  ENTRY();

//  ASSERT(*m_p_TaskLogicUnit->m_xpCrwBuffer);
//
//  Op* pOp = m_pJob->m_pOp;
//  PtnId tPtnId = pOp->tPtnId();
//  i8 i_TablePtn = Table::iPtnNoToTablePtn(tPtnId);
//
//  CrwBuffer* pCrwBuffer = _pCrwBuffer(*m_p_TaskLogicUnit->m_xpCrwBuffer);
//  // will delete this buffer
//  pCrwBuffer->vIsPermanent(false);
//  m_p_TaskLogicUnit->m_xpTable->vInsdelBuffer(C_UNKNOWN, i_TablePtn, pCrwBuffer);
//

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* CrwBuffer[].isNotNull -- is RowBuffer not null ?                           */
/*----------------------------------------------------------------------------*/
void Step::vCrwBufferIsNotNull(void)
{
  ENTRY();

  m_pDataStack->vPushInt(m_p_TaskLogicUnit->m_xpCrwBuffer.bIsNull() ? 0 : 1);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* CrwBuffer[].isNull -- is RowBuffer null ?                                  */
/*----------------------------------------------------------------------------*/
void Step::vCrwBufferIsNull(void)
{
  ENTRY();

  m_pDataStack->vPushInt(m_p_TaskLogicUnit->m_xpCrwBuffer.bIsNull() ? 1 : 0);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* CrwBuffer[].isReport -- is crwBuffer for report processing ?               */
/*----------------------------------------------------------------------------*/
void Step::vCrwBufferIsReport(void)
{
  ENTRY();

  Operand* pOperand = *m_p_TaskLogicUnit->m_xpCrwBuffer;
  m_pDataStack->vPushInt(pOperand->eTailProcessing() == PT_REPORT ? true : false);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* CrwBuffer[].set -- set operand                                             */
/*----------------------------------------------------------------------------*/
void Step::vCrwBufferSet(void)
{
  ENTRY();

  Operand* pOperand = _pOperand(m_pDataStack->pPopPtr(__func__));

  ASSERT(! pOperand || m_p_TaskLogicUnit->m_xpCrwBuffer.bIsNull());

  m_p_TaskLogicUnit->m_xpCrwBuffer = _pCrwBuffer(pOperand);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* CrwBuffer[].toCsv -- convert CrwBuffer to Csv                              */
/*----------------------------------------------------------------------------*/
void Step::vCrwBufferToCsv(void)
{
  ENTRY();

  CrwBuffer* pCrwBuffer = _pCrwBuffer(*m_p_TaskLogicUnit->m_xpCrwBuffer);
 
  Tub* pTub = _pTub(m_pDataStack->pPopPtr(__func__));
  
  pCrwBuffer->vToCsv(pTub);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* CrwBuffer[].toCsv -- convert CrwBuffer to Csv                              */
/*----------------------------------------------------------------------------*/
void Step::vCrwBufferToCsv_OLD(void)
{
  ENTRY();

  CrwBuffer* pCrwBuffer = _pCrwBuffer(*m_p_TaskLogicUnit->m_xpCrwBuffer);
  m_pDataStack->vPushPtr(pCrwBuffer->pToCsvBuffer());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* CrwBuffer[].upsertIntoTable -- insert crwBuffer into the table             */
/*----------------------------------------------------------------------------*/
void Step::vCrwBufferUpsertIntoTable(void)
{
  ENTRY();

//  ASSERT(*m_p_TaskLogicUnit->m_xpCrwBuffer);
//  CrwBuffer* pCrwBuffer = _pCrwBuffer(*m_p_TaskLogicUnit->m_xpCrwBuffer);
//
//  Op* pOp = m_pJob->m_pOp;
//  PtnId tPtnId = pOp->tPtnId();
//  i8 i_TablePtn = Table:iPtnNoToTablePtn(tPtnId);
//
//  m_p_TaskLogicUnit->m_xpTable->vUpsertBuffer(C_UNKNOWN, i_TablePtn, pCrwBuffer);
//
//  // will delete this buffer
//  pCrwBuffer->vIsPermanent(false);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* CrwBuffer[].writeToFile0 -- write to file0[]                               */
/*----------------------------------------------------------------------------*/
void Step::vCrwBufferWriteToFile0(void)
{
  ENTRY();

  ASSERT(*m_p_TaskLogicUnit->m_xpCrwBuffer);
  ASSERT(*m_p_TaskLogicUnit->m_xpFile0);
  ASSERT(m_p_TaskLogicUnit->m_xpFile0->bIsOpen());

  CrwBuffer* pCrwBuffer = _pCrwBuffer(*m_p_TaskLogicUnit->m_xpCrwBuffer);

  byte* aCrwProper = aAdd<byte*>(pCrwBuffer,sizeof(DataBuffer));
  i8 nSizeCrwProper = sizeof(CrwBuffer) - sizeof(DataBuffer);

  bool b = m_p_TaskLogicUnit->m_xpFile0->bWriteBytes(aCrwProper,nSizeCrwProper);
  ASSERT(b);
  UNUSED(b);

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
