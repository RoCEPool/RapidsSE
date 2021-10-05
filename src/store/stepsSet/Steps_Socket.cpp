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

#include "RowBuffer.hpp"

#include "InfoSet.hpp"

#include "StreamSet.hpp"

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

extern SizeHeap  gxaSizeHeap[];

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Socket(void)
{
  vAddStep("listen::open"_Stz,       &Step::vSocketListen);
  vAddStep("listen.accept"_Stz,      &Step::vSocketAccept);

  vAddStep("socket::new"_Stz,        &Step::vSocketNew);
  
  vAddStep("Socket[]"_Stz,           &Step::vSocketGet);
  vAddStep("Socket[].close"_Stz,     &Step::vSocketClose);
  vAddStep("Socket[].connect"_Stz,   &Step::vSocketConnect);
  vAddStep("Socket[].dropWrite"_Stz, &Step::vSocketDropWrite);
  vAddStep("Socket[].get"_Stz,       &Step::vSocketGet);
  vAddStep("Socket[].grabWrite"_Stz, &Step::vSocketGrabWrite);
  vAddStep("Socket[].isNotNull"_Stz, &Step::vSocketIsNotNull);
  vAddStep("Socket[].isNull"_Stz,    &Step::vSocketIsNull);
  vAddStep("Socket[].object@"_Stz,   &Step::vSocketObjectFetch);
  vAddStep("Socket[].object@\\clear"_Stz, &Step::vSocketObjectFetchClear);
  vAddStep("Socket[].recvObject"_Stz,&Step::vSocketRecvObject);
  vAddStep("Socket[].sendObject"_Stz,&Step::vSocketSendObject);
  vAddStep("Socket[].set"_Stz,       &Step::vSocketSet);
  vAddStep("Socket[].success@"_Stz,  &Step::vSocketSuccessFetch);
  vAddStep("Socket[].success!"_Stz,  &Step::vSocketSuccessStore);

}

/*----------------------------------------------------------------------------*/
/* listen.accept: accept a socket connection                                  */
/*----------------------------------------------------------------------------*/
void Step::vSocketAccept(void)
{
  ENTRY();
  
  SysCall oSysCall;
  
  Socket* pListeningSocket = _pSocket(m_pDataStack->pPopPtr(__func__));
  
  // accept a connection
  int iSocketId;
  bool bResult = pListeningSocket->bAcceptClient(iSocketId, oSysCall);
  
  if ( ! bResult )
    vThrow(FK_CHECK,__func__,__FILE__,__LINE__,
           "Unable to accept connection: Errno %d"_Stz, oSysCall.iError());
  
  // return the socket
  Socket* pNewSocket = new Socket(this,iSocketId);
  
  m_pDataStack->vPushPtr(pNewSocket);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* socket.close : close a socket                                              */
/*----------------------------------------------------------------------------*/
void Step::vSocketClose(void)
{
  ENTRY();
  
  SysCall oSysCall;

  Socket* pSocket = *m_p_TaskLogicUnit->m_xpSocket;

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
  CellSingle xNodeVar;
  
  Socket* pSocket = *m_p_TaskLogicUnit->m_xpSocket;

  m_pDataStack->vPopItem(xNodeVar,__func__);
  
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
  m_p_TaskLogicUnit->m_xpSocket = nullptr;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* socket.dropWrite: drop socket write lock                                   */
/*----------------------------------------------------------------------------*/
void Step::vSocketDropWrite(void)
{
  ENTRY();
  
  Socket* pSocket = *m_p_TaskLogicUnit->m_xpSocket;

  pSocket->vDropWrite();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* socket.get: fetch socket                                                   */
/*----------------------------------------------------------------------------*/
void Step::vSocketGet(void)
{
  ENTRY();
  
  m_pDataStack->vPushPtr(*m_p_TaskLogicUnit->m_xpSocket);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* socket.grabWrite: grab socket write lock                                   */
/*----------------------------------------------------------------------------*/
void Step::vSocketGrabWrite(void)
{
  ENTRY();
  
  Socket* pSocket = *m_p_TaskLogicUnit->m_xpSocket;

  pSocket->vGrabWrite();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Socket[].isNotNull -- is socket not null ?                                 */
/*----------------------------------------------------------------------------*/
void Step::vSocketIsNotNull(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(m_p_TaskLogicUnit->m_xpSocket.bIsNull() ? 0 : 1);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Socket[].isNull -- is Socket null ?                                        */
/*----------------------------------------------------------------------------*/
void Step::vSocketIsNull(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(m_p_TaskLogicUnit->m_xpSocket.bIsNull() ? 1 : 0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* listen::open: listen in for socket connections                           */
/*----------------------------------------------------------------------------*/
void Step::vSocketListen(void)
{
  ENTRY();
  
  SysCall oSysCall;
  
  i8 iPort = m_pDataStack->iPopInt(__func__);
  
  //- - - - - - - - - - - - - - - - - - -  - - - - - - - - - - - - - - - - - - -
  // attempt to create server-side listening socket
  
  Socket* pSocket = new Socket(this);
  bool bResult = pSocket->bCreateServer((int)iPort, oSysCall);
  
  if ( ! bResult ) {
    if (oSysCall.iError() == EADDRINUSE) {
      vThrow(FK_CHECK,__func__,__FILE__,__LINE__,
             "Address in use (port: %lld). "
             "Check if another instance of RAPIDSSE is running!"_Stz,
             iPort);
    }
    else
      vThrow(FK_CHECK,__func__,__FILE__,__LINE__,
             "Unable to listen for connections: Errno %d"_Stz, oSysCall.iError());
  }
  
  //- - - - - - - - - - - - - - - - - - -  - - - - - - - - - - - - - - - - - - -
  // test to see what happens when we attempt to create
  // server-side listening socket when the socket/port is already in use
  
//  Socket* pSocket2 = new Socket(this);
//  bool bResult2 = pSocket2->bCreateServer((int)iPort, oSysCall);
//
//  if ( ! bResult2 ) {
//    if (oSysCall.iError() == EADDRINUSE) {
//      vThrow(FK_CHECK,__func__,__FILE__,__LINE__,
//             "Address in use (port: %lld). "
//             "Check if another instance of RAPIDSSE is running!"_Stz,
//             iPort);
//    }
//    else
//      vThrow(FK_CHECK,__func__,__FILE__,__LINE__,
//             "Unable to listen for connections: Errno %d"_Stz, oSysCall.iError());
//  }

  // return socket
  m_pDataStack->vPushPtr(pSocket);
  
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

  pSocket = new Socket(pNode);
  bool bResult = pSocket->bSocket(oSysCall);
  
  if ( ! bResult )
    vThrow(FK_CHECK,__func__,__FILE__,__LINE__,
           "Unable to create socket: Errno %d"_Stz, oSysCall.iError());
  
  m_pDataStack->vPushPtr(pSocket);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Socket[].object@: fetch object                                             */
/*----------------------------------------------------------------------------*/
void Step::vSocketObjectFetch(void)
{
  ENTRY();
  
  Socket* pSocket = *m_p_TaskLogicUnit->m_xpSocket;

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
  
  Socket* pSocket = *m_p_TaskLogicUnit->m_xpSocket;

  Object* pObject = pSocket->pReceivedObject();
  pSocket->vReceivedObject(nullptr);
  
  m_pDataStack->vPushPtr(pObject);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Socket[].recvObject: recv object from a socket                             */
/*----------------------------------------------------------------------------*/
void Step::vSocketRecvObject(void)
{
  ENTRY();

  Socket* pSocket = *m_p_TaskLogicUnit->m_xpSocket;

  RASSERT(! pSocket->pReceivedObject());

  Object* pReceivedObject = pSocket->pRecvObject();
  
  if (pReceivedObject && CN_PARCEL == pReceivedObject->eGetClassNo()) {
    AM::PARCEL::vRecvContents((Parcel*)pReceivedObject,pSocket);

    MY::pOp()->vOpTrace((Parcel*)pReceivedObject,(Stz*)__func__,false);
  }
  
  pSocket->vReceivedObject(pReceivedObject);
  
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
  i8 nObjectSize = gxaSizeHeap[eClass].m_nClassSize;
  
  RASSERT(CN_JOB == eClass || _bIsValidOperand(eClass));
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  SysCall oSysCall;
  
  Socket* pSocket = *m_p_TaskLogicUnit->m_xpSocket;

  bool bResult = pSocket->bSendTo(pObject, nObjectSize, oSysCall);

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
  ASSERT(! pSocket || m_p_TaskLogicUnit->m_xpSocket.bIsNull());
  m_p_TaskLogicUnit->m_xpSocket = _pSocket(pSocket);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Socket[].success@ -- fetch success flag                                    */
/*----------------------------------------------------------------------------*/
void Step::vSocketSuccessFetch(void)
{
  ENTRY();
  
  Socket* pSocket = *m_p_TaskLogicUnit->m_xpSocket;

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

  Socket* pSocket = *m_p_TaskLogicUnit->m_xpSocket;

  pSocket->vSuccess(bSuccess);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
