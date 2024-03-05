/*============================================================================*/
/* Steps_Socket.cpp - socket step group                                       */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "eYX.h"

#include "Marker.hpp"

#include "RowCoder.hpp"
#include "HeadLine.hpp"
#include "Crw.hpp"
#include "CrwPacker.hpp"
#include "CrwBuffer.hpp"
#include "Node.hpp"
#include "Job.hpp"
#include "Socket.hpp"
#include "Parcel.hpp"
#include "Node_I.hpp"
#include "LogicUnit.hpp"
#include "OpSpec.hpp"
#include "StageSpec.hpp"

#include "SP_Header.hpp"
#include "SP_Job.hpp"

#include "Node.hpp"
#include "Grid.hpp"
#include <arpa/inet.h>
#include "TagConstants.h"
#include "MY.hpp"
#include "Op.hpp"

#include "PayloadObj.hpp"

extern SizeHeap  gxaSizeHeap[];

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Socket(void)
{

  vAddStep("socket::new"_Stz,        &Step::vSocketNew);
  vAddStep("socket::copy"_Stz,       &Step::vSocketCopy);

  vAddStep("Socket[]"_Stz,           &Step::vSocketGet);
  vAddStep("Socket[].close"_Stz,     &Step::vSocketClose);
  vAddStep("Socket[].connect"_Stz,   &Step::vSocketConnect);
  vAddStep("Socket[].delete"_Stz,    &Step::vSocketDelete);
  vAddStep("Socket[].dropWrite"_Stz, &Step::vSocketDropWrite);
  vAddStep("Socket[].get"_Stz,       &Step::vSocketGet);
  vAddStep("Socket[].grabWrite"_Stz, &Step::vSocketGrabWrite);
  vAddStep("Socket[].isNotNull"_Stz, &Step::vSocketIsNotNull);
  vAddStep("Socket[].isNull"_Stz,    &Step::vSocketIsNull);
  vAddStep("Socket[].object@"_Stz,   &Step::vSocketObjectFetch);
  vAddStep("Socket[].object@\\clear"_Stz,   &Step::vSocketObjectFetchClear);
  vAddStep("Socket[].object.delete"_Stz,    &Step::vSocketObjectDelete);
  vAddStep("Socket[].object.isNull"_Stz,    &Step::vSocketObjectIsNull);
  vAddStep("Socket[].object.isNotNull"_Stz, &Step::vSocketObjectIsNotNull);
  vAddStep("Socket[].recvObject"_Stz,&Step::vSocketRecvObject);
  vAddStep("Socket[].recvWLMsg"_Stz, &Step::vSocketRecvWLMsg);
  vAddStep("Socket[].sendObject"_Stz,&Step::vSocketSendObject);
  vAddStep("Socket[].sendWLMsg"_Stz,&Step::vSocketSendWLMsg);
  vAddStep("Socket[].set"_Stz,       &Step::vSocketSet);
  vAddStep("Socket[].success@"_Stz,  &Step::vSocketSuccessFetch);
  vAddStep("Socket[].success!"_Stz,  &Step::vSocketSuccessStore);
  vAddStep("Socket[].clientAddr@"_Stz,&Step::vSocketClientAddrFetch);

}

/*----------------------------------------------------------------------------*/
/* socket.close : close a socket                                              */
/*----------------------------------------------------------------------------*/
void Step::vSocketClose(void)
{
  ENTRY();
  
  SysCall oSysCall;

  Socket* pSocket = *m_pLogicUnit->m_xpSocket;

  // close the socket
  bool bResult = pSocket->bClose(oSysCall);
  
  if ( ! bResult )
    vThrow(FK_CHECK,__func__,__FILE__,__LINE__,
           "Unable to close socket: Errno %d!"_Stz, oSysCall.iError());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* socket.connect: connect to a server                                        */
/*----------------------------------------------------------------------------*/
void Step::vSocketConnect(void)
{
  ENTRY();
  
  SysCall oSysCall;
  SenCell xNodeVar;
  
  Socket* pSocket = *m_pLogicUnit->m_xpSocket;

  m_pDataStack->vPopSenCell(xNodeVar,__func__);
  
  i8 iPort = m_pDataStack->iPopInt(__func__);
  
  // get address of node
  Stz* sAddr = xNodeVar.szStz();
  
  if (AM::TRACE::bVerbose(C_VB_SOCKET))
    vTrace("SOCKET ADDR: %s\n"_Stz,sAddr);
  
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  bool bResult = pSocket->bConnectServer(sAddr, (int)iPort, oSysCall);

  if (bResult) {
    // return the socket object
    //m_pDataStack->vPushPtr(pSocket);
    EXIT();
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  i8 iError = oSysCall.iError();
  
  if (iError == ETIMEDOUT) {
    vTrace("Connect timedout... retrying...\n"_Stz);
  }
  else
  if (iError == ECONNREFUSED) {
    vTrace("Connect refused... retrying...\n"_Stz);
  }
  else {
    vTrace("Connect error: errno %d %s\n"_Stz,
           iError, strerror((int)iError));
  }
  
  //m_pDataStack->vPushPtr(nullptr);
  m_pLogicUnit->m_xpSocket = nullptr;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* socket.delete: delete and clear the socket object                          */
/*----------------------------------------------------------------------------*/
void Step::vSocketDelete(void)
{
  ENTRY();
  
  Socket* pSocket = *m_pLogicUnit->m_xpSocket;
  delete pSocket;
  m_pLogicUnit->m_xpSocket = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* socket.dropWrite: drop socket write lock                                   */
/*----------------------------------------------------------------------------*/
void Step::vSocketDropWrite(void)
{
  ENTRY();

  Socket* pSocket = *m_pLogicUnit->m_xpSocket;

  pSocket->vDropWrite();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* socket.get: fetch socket                                                   */
/*----------------------------------------------------------------------------*/
void Step::vSocketGet(void)
{
  ENTRY();
  
  m_pDataStack->vPushPtr(*m_pLogicUnit->m_xpSocket);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* socket.grabWrite: grab socket write lock                                   */
/*----------------------------------------------------------------------------*/
void Step::vSocketGrabWrite(void)
{
  ENTRY();
  
  Socket* pSocket = *m_pLogicUnit->m_xpSocket;

  pSocket->vGrabWrite();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Socket[].isNotNull -- is socket not null ?                                 */
/*----------------------------------------------------------------------------*/
void Step::vSocketIsNotNull(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(m_pLogicUnit->m_xpSocket.bIsNull() ? 0 : 1);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Socket[].isNull -- is Socket null ?                                        */
/*----------------------------------------------------------------------------*/
void Step::vSocketIsNull(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(m_pLogicUnit->m_xpSocket.bIsNull() ? 1 : 0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* socket::new: open a socket                                                */
/*----------------------------------------------------------------------------*/
void Step::vSocketNew(void)
{
  ENTRY();
  
  SysCall oSysCall;
  Socket* pSocket;
  
  Node* pNode = _pNode(m_pDataStack->pPopPtr(__func__));

  pSocket = new Socket(pNode,C_UNKNOWN);
  bool bResult = pSocket->bCreateFdForSocket(oSysCall);
  
  if ( ! bResult )
    vThrow(FK_CHECK,__func__,__FILE__,__LINE__,
           "Unable to create socket: Errno %d"_Stz, oSysCall.iError());
  
  m_pDataStack->vPushPtr(pSocket);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* socket::copy: duplicate a socket, keeping the same file descriptor         */
/*----------------------------------------------------------------------------*/
void Step::vSocketCopy(void)
{
  ENTRY();

  SysCall oSysCall;
  Socket* pSocket;

  Socket* pOther = _pSocket(m_pDataStack->pPopPtr(__func__));

  pSocket = new Socket(pOther->m_pIsFor,pOther->m_iSocketFd, pOther, true);

  m_pDataStack->vPushPtr(pSocket);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Socket[].object@: fetch object                                             */
/*----------------------------------------------------------------------------*/
void Step::vSocketObjectFetch(void)
{
  ENTRY();
  
  Socket* pSocket = *m_pLogicUnit->m_xpSocket;

  Object* pObject = pSocket->pReceivedObject();

  m_pDataStack->vPushPtr(pObject);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Socket[].object@\clear: fetch object (and clear)                           */
/*----------------------------------------------------------------------------*/
void Step::vSocketObjectFetchClear(void)
{
  ENTRY();
  
  Socket* pSocket = *m_pLogicUnit->m_xpSocket;

  Object* pObject = pSocket->pReceivedObject();
  pSocket->vReceivedObject(nullptr);
  
  m_pDataStack->vPushPtr(pObject);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Socket[].object.delete: delete the socket object                           */
/*----------------------------------------------------------------------------*/
void Step::vSocketObjectDelete(void)
{
  ENTRY();

  Socket* pSocket = *m_pLogicUnit->m_xpSocket;

  Object* pObject = pSocket->pReceivedObject();
  pSocket->vReceivedObject(nullptr);
  delete pObject;


  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Socket[].object.isNull: test if the socket object is null                  */
/*----------------------------------------------------------------------------*/
void Step::vSocketObjectIsNull(void)
{
  ENTRY();

  Socket* pSocket = *m_pLogicUnit->m_xpSocket;

  Object* pObject = pSocket->pReceivedObject();

  m_pDataStack->vPushInt(pObject == nullptr ? 1 : 0);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Socket[].object.isNotNull: test if the socket object is not null           */
/*----------------------------------------------------------------------------*/
void Step::vSocketObjectIsNotNull(void)
{
  ENTRY();

  Socket* pSocket = *m_pLogicUnit->m_xpSocket;

  Object* pObject = pSocket->pReceivedObject();

  m_pDataStack->vPushInt(pObject != nullptr ? 1 : 0);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Socket[].recvObject: recv object from a socket                             */
/*----------------------------------------------------------------------------*/
void Step::vSocketRecvObject(void)
{
  ENTRY();

  Socket* pSocket = *m_pLogicUnit->m_xpSocket;

  RASSERT(! pSocket->pReceivedObject());

  Object* pReceivedObject = pSocket->pRecvObject();
  
  if (pReceivedObject && CN_PARCEL == pReceivedObject->eGetClassNo()) {
    AM::PARCEL::vRecvContents((Parcel*)pReceivedObject,pSocket);

		if (AM::TRACE::bVerbose(C_VB_OP))
      MY::pOp()->vOpTrace((Parcel*)pReceivedObject,(Stz*)__func__,false);
  }
  
  pSocket->vReceivedObject(pReceivedObject);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Socket[].recvWLMsg: recv wireline message from a socket                    */
/* Sets ErrorCode[] and ErrorStr[] registers.                                 */
/*----------------------------------------------------------------------------*/
void Step::vSocketRecvWLMsg(void)
{
  ENTRY();

  Socket* pSocket = *m_pLogicUnit->m_xpSocket;

  RASSERT(! pSocket->pReceivedObject());

  eEC eErrCode = EC_NONE;
  s256 sErrStr;

  Object* pReceivedObject = pSocket->pRecvWLMsg(eErrCode, sErrStr.pString());

  pSocket->vReceivedObject(pReceivedObject);
  m_pLogicUnit->m_eErrorCode_reg = eErrCode;
  m_pLogicUnit->m_sErrorStr_reg.vCopy(sErrStr);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Socket[].sendObject: send object via a socket                              */
/*----------------------------------------------------------------------------*/
void Step::vSocketSendObject(void)
{
  ENTRY();
  
  Object* pObject = m_pDataStack->pPopPtr(__func__);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  eCN eClass = pObject->eGetClassNo();

  RASSERT(CN_JOB == eClass ||
          CN_REJECTJOB == eClass ||
          _bIsValidOperand(eClass));
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  SysCall oSysCall;
  
  Socket* pSocket = *m_pLogicUnit->m_xpSocket;

  bool bResult = pSocket->bSendObject(pObject,oSysCall,SY_NONE);

  m_pDataStack->vPushInt(bResult);

  EXIT();
}


/*----------------------------------------------------------------------------*/
/* Socket[].sendWLMsg: send wireline message via a socket                     */
/*----------------------------------------------------------------------------*/
void Step::vSocketSendWLMsg(void)
{
  ENTRY();

  WLP::PayloadObj* pPayload = WLP::_pPayloadObj(m_pDataStack->pPopPtr(__func__));
  ASSERT(CN_PAYLOADOBJ == pPayload->eGetClassNo());
  ASSERT(pPayload->iPayloadSize() > 0);

  SysCall oSysCall;

  Socket* pSocket = *m_pLogicUnit->m_xpSocket;

  bool bResult = pSocket->bSendWLMsg(pPayload,oSysCall,SY_NONE);

  m_pDataStack->vPushInt(bResult);

  EXIT();
}


/*----------------------------------------------------------------------------*/
/* Socket.set -- set socket                                                   */
/*----------------------------------------------------------------------------*/
void Step::vSocketSet(void)
{
  ENTRY();

  Socket* pSocket = _pSocket(m_pDataStack->pPopPtr(__func__));

//  if ( ! pSocket)
//    i8 i = 1;

  ASSERT(! pSocket || m_pLogicUnit->m_xpSocket.bIsNull());
  m_pLogicUnit->m_xpSocket = _pSocket(pSocket);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Socket[].success@ -- fetch success flag                                    */
/*----------------------------------------------------------------------------*/
void Step::vSocketSuccessFetch(void)
{
  ENTRY();
  
  Socket* pSocket = *m_pLogicUnit->m_xpSocket;

  bool bSuccess = pSocket->bSuccess();

  m_pDataStack->vPushInt(bSuccess);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Socket[].success! -- store success flag                                    */
/*----------------------------------------------------------------------------*/
void Step::vSocketSuccessStore(void)
{
  ENTRY();
  
  bool bSuccess = (bool)m_pDataStack->iPopInt(__func__);

  Socket* pSocket = *m_pLogicUnit->m_xpSocket;

  pSocket->vSuccess(bSuccess);

  EXIT();
}


/*----------------------------------------------------------------------------*/
/* Socket[].clientAddr@ -- fetch client address of socket                     */
/*----------------------------------------------------------------------------*/
void Step::vSocketClientAddrFetch(void)
{
  ENTRY();

  Socket* pSocket = *m_pLogicUnit->m_xpSocket;

  s256 sBuf;
  pSocket->vClientAddr(sBuf);

  m_pDataStack->vPushStz(sBuf.szStz());

  EXIT();
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
