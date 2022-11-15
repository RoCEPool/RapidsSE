/*============================================================================*/
/* Steps_AdaptorWL.cpp - Steps for AdaptorWL register in LogicUnit            */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "LogicUnit.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_AdaptorWL(void)
{
  vAddStep("AdaptorWL[].new"_Stz,                 &Step::vAdaptorWLNew);
  vAddStep("AdaptorWL[].delete"_Stz,              &Step::vAdaptorWLDelete);

  vAddStep("AdaptorWL[].decodePayload"_Stz,       &Step::vAdaptorWLDecodePayload);
  vAddStep("AdaptorWL[].hasPayloadToSend@"_Stz,   &Step::vAdaptorWLHasPayloadToSendFetch);
  vAddStep("AdaptorWL[].hasStatement"_Stz,        &Step::vAdaptorWLHasStatement);
  vAddStep("AdaptorWL[].nextPayloadToSend@"_Stz,  &Step::vAdaptorWLNextPayloadToSendFetch);
  vAddStep("AdaptorWL[].processWLMsg"_Stz,        &Step::vAdaptorWLProcessWLMsg);
  vAddStep("AdaptorWL[].protocolError@"_Stz,      &Step::vAdaptorWLProtocolErrorFetch);
  vAddStep("AdaptorWL[].protocolErrorTest"_Stz,   &Step::vAdaptorWLProtocolErrorTest);
  vAddStep("AdaptorWL[].statement@\\clear"_Stz,   &Step::vAdaptorWLStatementFetchClear);
  vAddStep("AdaptorWL[].smtSubmitted"_Stz,        &Step::vAdaptorWLStatementSubmitted);
  vAddStep("AdaptorWL[].smtSubmittedErr"_Stz,     &Step::vAdaptorWLStatementSubmittedError);
}

/*----------------------------------------------------------------------------*/
/* adaptorWL[].new -- create new wire line adaptor                            */
/*----------------------------------------------------------------------------*/
void Step::vAdaptorWLNew(void)
{
  ENTRY();

  m_pLogicUnit->m_xpAdaptorWL = new AdaptorWL;

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
/* adaptorWL[].decodePayload -- Decode a PayloadObj to a wireline message     */
/*----------------------------------------------------------------------------*/
void Step::vAdaptorWLDecodePayload(void)
{
  ENTRY();

  AdaptorWL* pAdaptorWL = _pAdaptorWL(* m_pLogicUnit->m_xpAdaptorWL);

  WLP::PayloadObj* pPayloadObj = WLP::_pPayloadObj(m_pDataStack->pPopPtr(__func__));
  ASSERT(CN_PAYLOADOBJ == pPayloadObj->eGetClassNo());

  // TODO: How do we get any serialised errors and put them on the stack?
  pAdaptorWL->vDecodePayload(pPayloadObj);
  
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
/* adaptorWL[].hasStatement -- Push true if there is a statement to submit    */
/*----------------------------------------------------------------------------*/
void Step::vAdaptorWLHasStatement(void)
{
  ENTRY();

  AdaptorWL* pAdaptorWL = _pAdaptorWL(* m_pLogicUnit->m_xpAdaptorWL);

  m_pDataStack->vPushInt(pAdaptorWL->bHasStatement() ? 1 : 0);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* AdaptorWL[].statement@\clear -- Push & clear the statement to submit       */
/*----------------------------------------------------------------------------*/
void Step::vAdaptorWLStatementFetchClear(void)
{
  ENTRY();

  AdaptorWL* pAdaptorWL = _pAdaptorWL(* m_pLogicUnit->m_xpAdaptorWL);

  m_pDataStack->vPushStz(pAdaptorWL->szStatement());

  pAdaptorWL->vClearStatement();

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

  pAdaptorWL->vStatementSubmitted(eErrCode);

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
