/*============================================================================*/
/* Steps_AdaptorWL.cpp - Steps for AdaptorWL register in LogicUnit            */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "LogicUnit.hpp"
#include "Socket.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_AdaptorWL(void)
{
  vAddStep("AdaptorWL[].new"_Stz,                 &Step::vAdaptorWLNew);
  vAddStep("AdaptorWL[].delete"_Stz,              &Step::vAdaptorWLDelete);

  vAddStep("AdaptorWL[].childJobExited"_Stz,      &Step::vAdaptorWLChildJobExited);
  vAddStep("AdaptorWL[].compilerError"_Stz,       &Step::vAdaptorWLCompilerError);
  vAddStep("AdaptorWL[].crwBuffer"_Stz,           &Step::vAdaptorWLCrwBuffer);
  vAddStep("AdaptorWL[].decodePayload"_Stz,       &Step::vAdaptorWLDecodePayload);
  vAddStep("AdaptorWL[].executionError"_Stz,      &Step::vAdaptorWLExecutionError);
  vAddStep("AdaptorWL[].executionWarning"_Stz,    &Step::vAdaptorWLExecutionWarning);
  vAddStep("adaptorWL[].affectedRowCount"_Stz,    &Step::vAdaptorWLAffectedRowCount);
  vAddStep("AdaptorWL[].hasPayloadToSend@"_Stz,   &Step::vAdaptorWLHasPayloadToSendFetch);
  vAddStep("AdaptorWL[].hasQuery"_Stz,            &Step::vAdaptorWLHasQuery);
  vAddStep("AdaptorWL[].isAbortEarly"_Stz,        &Step::vAdaptorWLIsAbortEarly);
  vAddStep("AdaptorWL[].isChildJobActive"_Stz,    &Step::vAdaptorWLIsChildJobActive);
  vAddStep("AdaptorWL[].nextPayloadToSend@"_Stz,  &Step::vAdaptorWLNextPayloadToSendFetch);
  vAddStep("AdaptorWL[].payloadCreateTime@"_Stz,  &Step::vAdaptorWLPayloadCreateTime);
  vAddStep("AdaptorWL[].pds"_Stz,                 &Step::vAdaptorWLPdsRowBuffer);
  vAddStep("AdaptorWL[].processWLMsg"_Stz,        &Step::vAdaptorWLProcessWLMsg);
  vAddStep("AdaptorWL[].protocolError@"_Stz,      &Step::vAdaptorWLProtocolErrorFetch);
  vAddStep("AdaptorWL[].protocolErrorTest"_Stz,   &Step::vAdaptorWLProtocolErrorTest);
  vAddStep("AdaptorWL[].queryComplete"_Stz,       &Step::vAdaptorWLQueryComplete);
  vAddStep("AdaptorWL[].queryReceipt"_Stz,        &Step::vAdaptorWLQueryReceipt);
  vAddStep("AdaptorWL[].queryStr@"_Stz,           &Step::vAdaptorWLQueryStrPush);
  vAddStep("AdaptorWL[].resultMetadata"_Stz,      &Step::vAdaptorWLResultMetadata);
  vAddStep("AdaptorWL[].rowBuffer"_Stz,           &Step::vAdaptorWLRowBuffer);
  vAddStep("AdaptorWL[].statementToSubmit"_Stz,   &Step::vAdaptorWLStatementToSubmit);
  vAddStep("AdaptorWL[].stmtParsingErr"_Stz,      &Step::vAdaptorWLStatementParsingError);
  vAddStep("AdaptorWL[].stmtSubmitted"_Stz,       &Step::vAdaptorWLStatementSubmitted);
  vAddStep("AdaptorWL[].stmtSubmittedErr"_Stz,    &Step::vAdaptorWLStatementSubmittedError);
}

/*----------------------------------------------------------------------------*/
/* adaptorWL[].new -- create new wire line adaptor                            */
/*----------------------------------------------------------------------------*/
void Step::vAdaptorWLNew(void)
{
  ENTRY();

  Socket* pSocket = _pSocket(m_pDataStack->pPopPtr(__func__));
  ASSERT(pSocket);
  ASSERT(pSocket->eGetClassNo() == CN_SOCKET);

  s256 sClientAddr;
  pSocket->vClientAddr(sClientAddr);

  m_pLogicUnit->m_xpAdaptorWL = new AdaptorWL(sClientAddr.szStz());

//  vTrace("Spawning wireline adaptor for client at %s\n"_Stz, sClientAddr.szStz());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* adaptorWL[].delete -- delete wire line adaptor                             */
/*----------------------------------------------------------------------------*/
void Step::vAdaptorWLDelete(void)
{
  ENTRY();

  m_pLogicUnit->m_xpAdaptorWL.vDeleteComponent();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* adaptorWL[].childJobExited -- update state after the child job has exited  */
/*----------------------------------------------------------------------------*/
void Step::vAdaptorWLChildJobExited(void)
{
  ENTRY();

  AdaptorWL* pAdaptorWL = _pAdaptorWL(* m_pLogicUnit->m_xpAdaptorWL);
  pAdaptorWL->vChildJobExited();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* adaptorWL[].compilerError -- Received an error from the SQL compiler       */
/*----------------------------------------------------------------------------*/
void Step::vAdaptorWLCompilerError(void)
{
  ENTRY();

  AdaptorWL* pAdaptorWL = _pAdaptorWL(* m_pLogicUnit->m_xpAdaptorWL);
	RowBuffer* pRowBuffer = * m_pLogicUnit->m_xpRowBufferErr;
  ASSERT(pRowBuffer);
  ASSERT(pRowBuffer->eBufferType() == BT_COMPILEERRORS);

  pAdaptorWL->vCompilerError(pRowBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* adaptorWL[].crwBuffer -- Received an compact row buffer from the EE        */
/*----------------------------------------------------------------------------*/
void Step::vAdaptorWLCrwBuffer(void)
{
  ENTRY();

  AdaptorWL* pAdaptorWL = _pAdaptorWL(* m_pLogicUnit->m_xpAdaptorWL);
  CrwBuffer* pCrwBuffer = _pCrwBuffer(m_pDataStack->pPopPtr(__func__));
  ASSERT(pCrwBuffer);
  ASSERT(pCrwBuffer->eGetClassNo() == CN_CRWBUFFER);

  pAdaptorWL->vRowData(pCrwBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* adaptorWL[].decodePayload -- Decode a PayloadObj to a wireline message     */
/*----------------------------------------------------------------------------*/
void Step::vAdaptorWLDecodePayload(void)
{
  ENTRY();

  AdaptorWL* pAdaptorWL = _pAdaptorWL(* m_pLogicUnit->m_xpAdaptorWL);

  WLP::PayloadObj* pPayloadObj = WLP::_pPayloadObj(m_pDataStack->pPopPtr(__func__));
  ASSERT(CN_PAYLOADOBJ == pPayloadObj->eGetClassNo());

  pAdaptorWL->vDecodePayload(pPayloadObj);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* adaptorWL[].executionError -- Received an error from the execution engine  */
/*----------------------------------------------------------------------------*/
void Step::vAdaptorWLExecutionError(void)
{
  ENTRY();

  AdaptorWL* pAdaptorWL = _pAdaptorWL(* m_pLogicUnit->m_xpAdaptorWL);
  RowBuffer* pRowBuffer = * m_pLogicUnit->m_xpRowBufferErr;
  ASSERT(pRowBuffer);
  ASSERT(pRowBuffer->eBufferType() == BT_EXECERRORS);

  pAdaptorWL->vExecutionError(pRowBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* adaptorWL[].executionWarning -- Received a warning from the execution engine*/
/*----------------------------------------------------------------------------*/
void Step::vAdaptorWLExecutionWarning(void)
{
  ENTRY();

  AdaptorWL* pAdaptorWL = _pAdaptorWL(* m_pLogicUnit->m_xpAdaptorWL);
	RowBuffer* pRowBuffer = * m_pLogicUnit->m_xpRowBufferWarn;
  ASSERT(pRowBuffer);
  ASSERT(pRowBuffer->eBufferType() == BT_WARNINGS);

  pAdaptorWL->vExecutionWarning(pRowBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* adaptorWL[].affectedRowCount -- Accumulate affected row counts             */
/*----------------------------------------------------------------------------*/
void Step::vAdaptorWLAffectedRowCount(void)
{
  ENTRY();

  AdaptorWL* pAdaptorWL = _pAdaptorWL(* m_pLogicUnit->m_xpAdaptorWL);
  RowBuffer* pRowBuffer = * m_pLogicUnit->m_xpRowBuffer;
  ASSERT(pRowBuffer);
  ASSERT(pRowBuffer->eBufferType() == BT_AFFECTEDROWS);

  pAdaptorWL->vAffectedRowCount(pRowBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* adaptorWL[].processWLMessage -- Process a decoded wireline message         */
/*----------------------------------------------------------------------------*/
void Step::vAdaptorWLProcessWLMsg(void)
{
  ENTRY();

  AdaptorWL* pAdaptorWL = _pAdaptorWL(* m_pLogicUnit->m_xpAdaptorWL);
  pAdaptorWL->vProcessWLMsg();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* adaptorWL[].payloadCreateTime -- Get creation time of last payload received*/
/*----------------------------------------------------------------------------*/
void Step::vAdaptorWLPayloadCreateTime(void)
{
  ENTRY();

  AdaptorWL* pAdaptorWL = _pAdaptorWL(* m_pLogicUnit->m_xpAdaptorWL);
  u8 uPayloadCreateTime = pAdaptorWL->uPayloadCreationTime();

  m_pDataStack->vPushInt((i8) uPayloadCreateTime);
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* adaptorWL[].hasPayloadToSend@ -- True if there is a PayloadObj on the stack*/
/*----------------------------------------------------------------------------*/
void Step::vAdaptorWLHasPayloadToSendFetch(void)
{
  ENTRY();

  AdaptorWL* pAdaptorWL = _pAdaptorWL(* m_pLogicUnit->m_xpAdaptorWL);
  m_pDataStack->vPushBol(pAdaptorWL->bHasPayloadToSend() ? 1 : 0);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* adaptorWL[].nextPayloadToSend@ -- Puts next payload to send on the stack   */
/*----------------------------------------------------------------------------*/
void Step::vAdaptorWLNextPayloadToSendFetch(void)
{
  ENTRY();

  AdaptorWL* pAdaptorWL = _pAdaptorWL(* m_pLogicUnit->m_xpAdaptorWL);
  ASSERT(pAdaptorWL->bHasPayloadToSend());

  WLP::PayloadObj* pPayload = pAdaptorWL->pNextPayloadToSend();
  ASSERT(pPayload);
  ASSERT(CN_PAYLOADOBJ == pPayload->eGetClassNo());

  m_pDataStack->vPushPtr(pPayload);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* adaptorWL[].protocolErrorTest -- Pushes true if the protocol is out of sync*/
/*----------------------------------------------------------------------------*/
void Step::vAdaptorWLProtocolErrorTest(void)
{
  ENTRY();

  AdaptorWL* pAdaptorWL = _pAdaptorWL(* m_pLogicUnit->m_xpAdaptorWL);

  m_pDataStack->vPushInt(pAdaptorWL->eProtocolError() == EC_NONE ? 0 : 1);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* adaptorWL[].queryComplete -- Perform actions when the query is completed   */
/*----------------------------------------------------------------------------*/
void Step::vAdaptorWLQueryComplete(void)
{
  ENTRY();

  AdaptorWL* pAdaptorWL = _pAdaptorWL(* m_pLogicUnit->m_xpAdaptorWL);

  pAdaptorWL->vQueryComplete();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* adaptorWL[].queryReceipt -- Send a query receipt for the parsed tasks      */
/*----------------------------------------------------------------------------*/
void Step::vAdaptorWLQueryReceipt(void)
{
  ENTRY();

  AdaptorWL* pAdaptorWL = _pAdaptorWL(* m_pLogicUnit->m_xpAdaptorWL);
  TaskList* pTaskList = _pTaskList(* m_pLogicUnit->m_xpTaskList);

  ASSERT(pAdaptorWL);
  ASSERT(pTaskList);

  pAdaptorWL->vQueryReceipt(pTaskList);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* adaptorWL[].queryStr@ -- Push the query string onto the data stack         */
/*----------------------------------------------------------------------------*/
void Step::vAdaptorWLQueryStrPush(void)
{
  ENTRY();

  AdaptorWL* pAdaptorWL = _pAdaptorWL(* m_pLogicUnit->m_xpAdaptorWL);

  m_pDataStack->vPushStz(pAdaptorWL->szQuery());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* adaptorWL[].resultMetadata -- Resultset metadata has been received         */
/*----------------------------------------------------------------------------*/
void Step::vAdaptorWLResultMetadata(void)
{
  ENTRY();

  AdaptorWL* pAdaptorWL = _pAdaptorWL(* m_pLogicUnit->m_xpAdaptorWL);

  StreamDef* pStreamDef = _pStreamDef(m_pDataStack->pPopPtr(__func__ ));
  ASSERT(pStreamDef);
  ASSERT(CN_STREAMDEF == pStreamDef->eGetClassNo());

  pAdaptorWL->vResultMetadata(pStreamDef);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* adaptorWL[].protocolError@ -- Push the protocol error code                 */
/*----------------------------------------------------------------------------*/
void Step::vAdaptorWLProtocolErrorFetch(void)
{
  ENTRY();

  AdaptorWL* pAdaptorWL = _pAdaptorWL(* m_pLogicUnit->m_xpAdaptorWL);

  m_pDataStack->vPushInt(pAdaptorWL->eProtocolError());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* adaptorWL[].hasQuery -- Push true if there is a query to be parsed         */
/*----------------------------------------------------------------------------*/
void Step::vAdaptorWLHasQuery(void)
{
  ENTRY();

  AdaptorWL* pAdaptorWL = _pAdaptorWL(* m_pLogicUnit->m_xpAdaptorWL);

  m_pDataStack->vPushInt(pAdaptorWL->bHasQuery() ? 1 : 0);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* adaptorWL[].isAbortEarly -- Push true if execution of the query is to abort*/
/*----------------------------------------------------------------------------*/
void Step::vAdaptorWLIsAbortEarly(void)
{
  ENTRY();

  AdaptorWL* pAdaptorWL = _pAdaptorWL(* m_pLogicUnit->m_xpAdaptorWL);

  m_pDataStack->vPushInt(pAdaptorWL->bAbortEarly() ? 1 : 0);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* adaptorWL[].isChildJobActive -- Push true if there is a statement executing*/
/*----------------------------------------------------------------------------*/
void Step::vAdaptorWLIsChildJobActive(void)
{
  ENTRY();

  AdaptorWL* pAdaptorWL = _pAdaptorWL(* m_pLogicUnit->m_xpAdaptorWL);

  m_pDataStack->vPushInt(pAdaptorWL->bIsChildJobActive() ? 1 : 0);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* adaptorWL[].rowBuffer -- Received a row buffer from the execution engine   */
/*----------------------------------------------------------------------------*/
void Step::vAdaptorWLRowBuffer(void)
{
  ENTRY();

  AdaptorWL* pAdaptorWL = _pAdaptorWL(* m_pLogicUnit->m_xpAdaptorWL);
  RowBuffer* pRowBuffer = _pRowBuffer(m_pDataStack->pPopPtr(__func__));
  ASSERT(pRowBuffer);
  ASSERT(pRowBuffer->eGetClassNo() == CN_ROWBUFFER);

  pAdaptorWL->vRowData(pRowBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* adaptorWL[].pds -- Received a pds row buffer from the execution engine     */
/*----------------------------------------------------------------------------*/
void Step::vAdaptorWLPdsRowBuffer(void)
{
  ENTRY();

  AdaptorWL* pAdaptorWL = _pAdaptorWL(* m_pLogicUnit->m_xpAdaptorWL);
  Pds* pPdsRowBuffer = _pPds(m_pDataStack->pPopPtr(__func__));
  ASSERT(pPdsRowBuffer);
  ASSERT(pPdsRowBuffer->eGetClassNo() == CN_PDS);

  pAdaptorWL->vPdsRowData(pPdsRowBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* AdaptorWL[].statementToSubmit -- There is a next statement to be executed  */
/*----------------------------------------------------------------------------*/
void Step::vAdaptorWLStatementToSubmit(void)
{
  ENTRY();

  Task* pNextTask = _pTask(m_pDataStack->pPopPtr(__func__));

  AdaptorWL* pAdaptorWL = _pAdaptorWL(* m_pLogicUnit->m_xpAdaptorWL);

  pAdaptorWL->vStatementToSubmit(pNextTask);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* AdaptorWL[].stmtParsingErr -- The statement parsing failed.                */
/* An error code and error string must be on the stack.                       */
/*----------------------------------------------------------------------------*/
void Step::vAdaptorWLStatementParsingError(void)
{
  ENTRY();

  AdaptorWL* pAdaptorWL = _pAdaptorWL(* m_pLogicUnit->m_xpAdaptorWL);

  s2K sErrMsg;
  m_pDataStack->vPopStr(sErrMsg, __func__);
  ASSERT(sErrMsg.nLength() > 0);

  eEC eErrCode = (eEC) m_pDataStack->iPopInt(__func__);
  ASSERT(eErrCode != EC_NONE);

  pAdaptorWL->vStatementParsingFailed(eErrCode, sErrMsg.szStz());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* AdaptorWL[].statementSubmittedError -- The statement submission failed.    */
/* An error code must be on the stack.                                        */
/*----------------------------------------------------------------------------*/
void Step::vAdaptorWLStatementSubmittedError(void)
{
  ENTRY();

  AdaptorWL* pAdaptorWL = _pAdaptorWL(* m_pLogicUnit->m_xpAdaptorWL);

  eEC eErrCode = (eEC) m_pDataStack->iPopInt(__func__);

  pAdaptorWL->vStatementSubmissionFailed(eErrCode);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* AdaptorWL[].statementSubmitted -- The statement submission succeeded.      */
/*----------------------------------------------------------------------------*/
void Step::vAdaptorWLStatementSubmitted(void)
{
  ENTRY();

  AdaptorWL* pAdaptorWL = _pAdaptorWL(* m_pLogicUnit->m_xpAdaptorWL);

  pAdaptorWL->vStatementSubmitted();

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
