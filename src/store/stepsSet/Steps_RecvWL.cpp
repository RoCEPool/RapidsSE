/*============================================================================*/
/* Steps_Object.cpp - object step group                                       */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"
#include "LogicUnit.hpp"
#include "thrift/protocol/TCompactProtocol.h"
#include "thrift/transport/TBufferTransports.h"

#include "wlp/thrift/generated/ErrorResponse_types.h"
#include "wlp/thrift/generated/MessageTypes_types.h"
#include "wlp/thrift/supports/BufferTransport.hpp"

extern SizeHeap gxaSizeHeap[];

using namespace com::rapidsdata::wirelineprotocol::messages::v5;
using namespace apache::thrift::protocol;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_RecvWL(void)
{
  vAddStep("recvWL[]"_Stz,               &Step::vRecvWLGet);
  vAddStep("recvWL[].get"_Stz,           &Step::vRecvWLGet);
  vAddStep("recvWL[].isNotNull"_Stz,     &Step::vRecvWLIsNotNull);
  vAddStep("recvWL[].isNull"_Stz,        &Step::vRecvWLIsNull);
  vAddStep("recvWL[].set"_Stz,           &Step::vRecvWLSet);
  vAddStep("recvWL[].delete"_Stz,        &Step::vRecvWLDelete);
  vAddStep("recvWL[].serialiseError"_Stz,&Step::vRecvWLSerialiseError);
}

/*----------------------------------------------------------------------------*/
/* recvWL[].get -- get the value in recveived object                     */
/*----------------------------------------------------------------------------*/
void Step::vRecvWLGet(void)
{
  ENTRY();

  m_pDataStack->vPushPtr(*m_pLogicUnit->m_xpRecvWL);
}

/*----------------------------------------------------------------------------*/
/* recvWL[].isNotNull -- is recvWL not null ?                       */
/*----------------------------------------------------------------------------*/
void Step::vRecvWLIsNotNull(void)
{
  ENTRY();

  m_pDataStack->vPushInt(m_pLogicUnit->m_xpRecvWL.bIsNull() ? 0 : 1);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* recvWL[].isNull -- is recvWL null ?                              */
/*----------------------------------------------------------------------------*/
void Step::vRecvWLIsNull(void)
{
  ENTRY();

  m_pDataStack->vPushInt(m_pLogicUnit->m_xpRecvWL.bIsNull() ? 1 : 0);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* recvWL[].set -- set the value in recveived object                     */
/*----------------------------------------------------------------------------*/
void Step::vRecvWLSet(void)
{
  ENTRY();

  m_pLogicUnit->m_xpRecvWL = (RecvWL*)m_pDataStack->pPopPtr(__func__);
}

/*----------------------------------------------------------------------------*/
/* recvWL[].delete -- delete the received object and clear                    */
/*----------------------------------------------------------------------------*/
void Step::vRecvWLDelete(void)
{
  ENTRY();

  RecvWL* pRecvWL = * m_pLogicUnit->m_xpRecvWL;
  delete pRecvWL;
  m_pLogicUnit->m_xpRecvWL = nullptr;
}

/*----------------------------------------------------------------------------*/
/* recvWL[].serialiseError -- Take an error code and error string and push a  */
/* payload containing a serialised wireline ErrorResponse.                    */
/*----------------------------------------------------------------------------*/
void Step::vRecvWLSerialiseError(void)
{
  ENTRY();

  eEC eErrorCode = (eEC) m_pDataStack->iPopInt(__func__);
  RASSERT(eErrorCode > EC_NONE);
  RASSERT(eErrorCode < EC_ENUMS);

  s256 sErrorStr;
  m_pDataStack->vPopStr(sErrorStr, __func__);

  Stz* szFmt = "Error: %s: %s"_Stz;
  s16KB sBuf;
  sBuf.vSprint(szFmt, aErrorMsg[eErrorCode], sErrorStr.szStz());

  ErrorResponse oErrorResponse;
  oErrorResponse.__set_errorString(sBuf.azChar());
  oErrorResponse.__set_errorCode(eErrorCode);

  // This payload is not to be processed by the AdaptorWL. It should
  // then just be passed on to the sendWL to be sent to the client.
  WLP::PayloadObj* pPayload = new WLP::PayloadObj(true);

  // Serialise the error message to a payload
  auto pTransport = std::make_shared<WLP::BufferTransport>();
  auto pProtocol  = std::make_shared<apache::thrift::protocol::TCompactProtocol>(pTransport);

  pTransport->vUseBuffer(pPayload->pcBuf(), WLP::PayloadObj::C_MAX_PAYLOAD_BYTES);
  WLP::WlpHeader* pHeader = pTransport->pAllocHeader(MessageTypes::ErrorResponse);

  try {
    oErrorResponse.write(pProtocol.get());

  } catch (WLP::BufferFullException& e) {
    // We should never get here. Push a null object in its place.
    vTrace("Error: Couldn't serialise a RecvWL error to an ErrorResponse msg because "
           "the buffer is too full.\n"_Stz);
    m_pDataStack->vPushPtr(nullptr);
    return;
  }

  // Message size needs to be written into the header of the message
  // as well as the payload object.
  i8 iMsgSize = pTransport->iBytesWritten();
  pHeader->vMsgSize(iMsgSize);
  pPayload->vSetPayloadSize(iMsgSize);
  pTransport->vRemoveBuffer();

  m_pDataStack->vPushPtr(pPayload);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
