/*============================================================================*/
/* Steps_Socket.cpp - socket step group                                       */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "ePX.h"

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
#include "XmitOperand.hpp"
#include "Node_I.hpp"
#include "LogicUnit.hpp"
#include "OpSpec.hpp"
#include "StageSpec.hpp"

#include "SP_Header.hpp"
#include "SP_Job.hpp"

extern SizeHeap  gxaSizeHeap[];

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Socket(void)
{
  vAddStep("socket::open"_Stz,      &Step::vSocketOpen);
  vAddStep("socket::listen"_Stz,    &Step::vSocketListen);
  
  vAddStep("Socket[]"_Stz,          &Step::vSocketGet);
  vAddStep("Socket[].get"_Stz,      &Step::vSocketGet);
  vAddStep("Socket[].isNotNull"_Stz,&Step::vSocketIsNotNull);
  vAddStep("Socket[].isNull"_Stz,   &Step::vSocketIsNull);
  vAddStep("Socket[].set"_Stz,      &Step::vSocketSet);

  vAddStep("socket.accept"_Stz,     &Step::vSocketAccept);
  vAddStep("socket.close"_Stz,      &Step::vSocketClose);
  vAddStep("socket.connect"_Stz,    &Step::vSocketConnect);
  vAddStep("socket.dropWrite"_Stz,  &Step::vSocketDropWrite);
  vAddStep("socket.grabWrite"_Stz,  &Step::vSocketGrabWrite);
  vAddStep("socket.recvObject"_Stz, &Step::vSocketRecvObject);
  vAddStep("socket.sendObject"_Stz, &Step::vSocketSendObject);
}

/*----------------------------------------------------------------------------*/
/* socket.accept: accept a socket connection                                  */
/*----------------------------------------------------------------------------*/
void Step::vSocketAccept(void)
{
  ENTRY();
  
  SysCall oSysCall;
  
  Socket* pSocket = _pSocket(m_pDataStack->pPopPtr(__func__));
  
  // accept a connection
  int iSocketId;
  bool bResult = pSocket->bAcceptClient(iSocketId, oSysCall);
  
  if ( ! bResult )
    vThrow(FK_CHECK,VL_FATAL,__func__,__FILE__,__LINE__,
           "Unable to accept connection: Errno %d"_Stz, oSysCall.iError());
  
  // return the socket
  Socket* pNewSocket = new Socket(this,iSocketId);
  
  m_pDataStack->vPushPtr(pNewSocket);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* open a socket                                                              */
/*----------------------------------------------------------------------------*/
void Step::vSocketClose(void)
{
  ENTRY();
  
  SysCall oSysCall;

  Socket* pSocket = _pSocket(m_pDataStack->pPopPtr(__func__));

  // close the socket
  bool bResult = pSocket->bClose(oSysCall);
  
  if ( ! bResult )
    vThrow(FK_CHECK,VL_FATAL,__func__,__FILE__,__LINE__,
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
  Socket* pSocket;
  CellSingle xNodeVar;
  
  
  Object* pObject = m_pDataStack->pPopPtr(__func__);
  // get pSocket
  pSocket = reinterpret_cast<Socket*>(pObject);

  m_pDataStack->vPopItem(xNodeVar,__func__);
  
  i8 iPort = m_pDataStack->iPopInt(__func__);
  
  // get address of node
  Stz* sAddr = xNodeVar.szStz();
  
  if (AM::bVerbose())
    vTrace("SOCKET ADDR: %s\n"_Stz,sAddr);
  
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  bool bResult = pSocket->bConnectServer(sAddr, (int)iPort, oSysCall);

  if (bResult) {
    // return the socket object
    Object* pPtrSocket = pSocket;
    m_pDataStack->vPushPtr(pPtrSocket);
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
  
  m_pDataStack->vPushPtr(nullptr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* socket.dropWrite: drop socket write lock                                   */
/*----------------------------------------------------------------------------*/
void Step::vSocketDropWrite(void)
{
  ENTRY();
  
  Socket* pSocket = _pSocket(m_pDataStack->pPopPtr(__func__));
  
  pSocket->vDropWrite();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* socket.get: fetch socket                                                   */
/*----------------------------------------------------------------------------*/
void Step::vSocketGet(void)
{
  ENTRY();
  
  m_pDataStack->vPushPtr(*m_pJob->pLogicUnit_job()->m_xpSocket);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* socket.grabWrite: grab socket write lock                                   */
/*----------------------------------------------------------------------------*/
void Step::vSocketGrabWrite(void)
{
  ENTRY();
  
  Socket* pSocket = _pSocket(m_pDataStack->pPopPtr(__func__));
  
  pSocket->vGrabWrite();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Socket[].isNotNull -- is socket not null ?                                 */
/*----------------------------------------------------------------------------*/
void Step::vSocketIsNotNull(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(m_pJob->pLogicUnit_job()->m_xpSocket.bIsNull() ? 0 : 1);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Socket[].isNull -- is Socket null ?                                        */
/*----------------------------------------------------------------------------*/
void Step::vSocketIsNull(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(m_pJob->pLogicUnit_job()->m_xpSocket.bIsNull() ? 1 : 0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* socket::listen: listen in for socket connections                           */
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
      vThrow(FK_CHECK,VL_FATAL,__func__,__FILE__,__LINE__,
             "Address in use (port: %lld). "
             "Check if another instance of RAPIDSSE is running!"_Stz,
             iPort);
    }
    else
      vThrow(FK_CHECK,VL_FATAL,__func__,__FILE__,__LINE__,
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
//      vThrow(FK_CHECK,VL_FATAL,__func__,__FILE__,__LINE__,
//             "Address in use (port: %lld). "
//             "Check if another instance of RAPIDSSE is running!"_Stz,
//             iPort);
//    }
//    else
//      vThrow(FK_CHECK,VL_FATAL,__func__,__FILE__,__LINE__,
//             "Unable to listen for connections: Errno %d"_Stz, oSysCall.iError());
//  }

  // return socket
  Object* pPtrSocket = pSocket;
  m_pDataStack->vPushPtr(pPtrSocket);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* socket::open: open a socket                                                */
/*----------------------------------------------------------------------------*/
void Step::vSocketOpen(void)
{
  ENTRY();
  
  SysCall oSysCall;
  Socket* pSocket;
  
  Node* pNode = _pNode(m_pDataStack->pPopPtr(__func__));

  pSocket = new Socket(pNode);
  bool bResult = pSocket->bSocket(oSysCall);
  
  if ( ! bResult )
    vThrow(FK_CHECK,VL_FATAL,__func__,__FILE__,__LINE__,
           "Unable to create socket: Errno %d"_Stz, oSysCall.iError());
  
  Object* pPtrSocket = pSocket;
  m_pDataStack->vPushPtr(pPtrSocket);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* socket.recvObject: recv object from a socket                               */
/*----------------------------------------------------------------------------*/
void Step::vSocketRecvObject(void)
{
  ENTRY();
  
  bool bLowMem = gpBufferPool->bLowMemory();
  
  char caObjectHeader[sizeof(Object) + C_1K];
  Header* aHeader = (Header*) & caObjectHeader[0];
  Object* pObject = reinterpret_cast<Object*>(&caObjectHeader);
  
  pObject->vSetWasReceived(true);
  pObject->vSetPostRecvRun(false);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // receive the header of the next object

  Socket* pSocket = _pSocket(m_pDataStack->pPopPtr(__func__));
  
  SysCall oSysCall;
  bool bResult = pSocket->bRecvBytes(aHeader,sizeof(Object),oSysCall);
  if (! bResult) {
    m_pDataStack->vPushInt(false);
    EXIT();
    return;
  }
  
//  SP_Header spHeader;
//  vTrace("%lld: vSocketRecvObject: Header %s\n"_Stz,
//         AM::iThisNodeNo(),
//         spHeader.sz(aHeader));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // receive the body of the obect (enough memory condition)

  eCN eClass = pObject->eGetClassNo();
  
  if (! bLowMem || ! gxaSizeHeap[eClass].m_bLowMemoryDiscard) {

    Object* pReceivedObject = Heaps::pAllocate(eClass);
    MemLib::vCopy(pReceivedObject,pObject,sizeof(Object),eClass);

    void* aBody = aAdd<void*>(pReceivedObject,sizeof(Object));
    i8 nRemainingBytes = gxaSizeHeap[eClass].m_nClassSize - sizeof(Object);

    bResult = pSocket->bRecvBytes(aBody,nRemainingBytes,oSysCall);
    if (! bResult) {
     m_pDataStack->vPushInt(false);
     EXIT();
     return;
    }

    //pReceivedObject->vSetPostRecvRun(false);
    
    pReceivedObject->vRunPostRecv();

    m_pDataStack->vPushPtr(pReceivedObject);
    m_pDataStack->vPushInt(true);
    EXIT();
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // receive and discard the body of the obect (low memory condition)

  void* aLowBuff = pSocket->m_aLowBuff;

  // copy the object header into the LowBuffer
  MemLib::vCopy(aLowBuff,pObject,sizeof(Object),eClass);
  
  // recv rest of the buffer into LowBuffer
  void* aLowBody = aAdd<void*>(aLowBuff,sizeof(Object));
  i8 nRemainingBytes = gxaSizeHeap[eClass].m_nClassSize - sizeof(Object);
  
  bResult = pSocket->bRecvBytes(aLowBody,nRemainingBytes,oSysCall);
  
  // whether able to recv or not, return false
  m_pDataStack->vPushPtr(nullptr);
  m_pDataStack->vPushInt(true);
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* socket.sendObject: send object via a socket                                */
/*----------------------------------------------------------------------------*/
void Step::vSocketSendObject(void)
{
  ENTRY();
  
  Socket* pSocket = _pSocket(m_pDataStack->pPopPtr(__func__));
  
  Object* pObject = m_pDataStack->pPopPtr(__func__);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  eCN eClass = pObject->eGetClassNo();
  i8 nObjectSize = gxaSizeHeap[eClass].m_nClassSize;
  
  //vTrace("vSocketSendObject: send %p %lld %s %lld bytes\n"_Stz,
  //       pObject,eClass,aCN[eClass],nObjectSize);
  
  
  
  if (_bIsValidJob(eClass)) {
    Job* pJob = _pJob(pObject);
        
    SP_Job spJob;

//    vTrace("%lld %p: vSocketSendObject: Job %s\n"_Stz,
//           AM::iThisNodeNo(),
//           pSocket,
//           spJob.sz_detail(pJob));

  }

  RASSERT(_bIsValidJob(eClass) ||
          _bIsValidOperand(eClass));
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
    
  SysCall oSysCall;
  
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
  ASSERT(! pSocket || m_pJob->pLogicUnit_job()->m_xpSocket.bIsNull());
  m_pJob->pLogicUnit_job()->m_xpSocket = _pSocket(pSocket);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
