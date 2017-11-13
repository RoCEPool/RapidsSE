/*============================================================================*/
/* Steps_Socket.cpp - socket step group                                       */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap*      gpAttachMap;
extern BPA*            gpBPA;
extern Dic*            gpMainDic;
extern Word*           gpWord;

extern Grid*           gpGrid;
extern StepArray*      gpStepArray;
extern ThreadList*     gpThreadList;
extern TaskList*       gpTaskList;

extern NP_Task         gnpBootTask;
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;

extern bool            gbTracepoint;

// socket: /////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Socket(void)
{
  vAddPrimitive("socket.accept"_Stz,   &vSocketAccept,     false);
  vAddPrimitive("socket.close"_Stz,    &vSocketClose,      false);
  vAddPrimitive("socket.connect"_Stz,  &vSocketConnect,    false);
  vAddPrimitive("socket.listen"_Stz,   &vSocketListen,     false);
  vAddPrimitive("socket.open"_Stz,     &vSocketOpen,       false);
  vAddPrimitive("socket.recv"_Stz,     &vSocketRecv,       false);
  vAddPrimitive("socket.send"_Stz,     &vSocketSend,       false);
}

/*----------------------------------------------------------------------------*/
/* socket.accept: accept a socket connection                                  */
/*----------------------------------------------------------------------------*/
void Task::vSocketAccept(MP_Task mpTask)
{
  SysCall xSysCall;
  Socket* pSocket;
  int     iSocketId;
  Object* pObject;
  
  ENTRY("vSocketAccept"_Stz);
  
  if ( ! mpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("socket.accept"_Stz);
  
  // get pSocket
  pSocket = reinterpret_cast<Socket*>(pObject);
  
  // accept a connection
  bool bResult = pSocket->bAcceptClient(iSocketId, xSysCall);
  
  if ( ! bResult )
    THROW("Unable to accept connection: Errno %d!"_Stz, xSysCall.iError());
  
  Task* pTask = (Task*)0x208f500c8;
  
  // return the new socket
  mpTask->m_pDataStack->vPushObject(new Socket(iSocketId));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* open a socket                                                              */
/*----------------------------------------------------------------------------*/
void Task::vSocketClose(MP_Task mpTask)
{
  SysCall xSysCall;
  Socket* pSocket;
  Object* pObject;
  
  ENTRY("vSocketClose"_Stz);
  
  if ( ! mpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("socketclose"_Stz);
  
  // get pSocket
  pSocket = reinterpret_cast<Socket*>(pObject);
  
  // close the socket
  bool bResult = pSocket->bClose(xSysCall);
  
  if ( ! bResult )
    THROW("Unable to close socket: Errno %d!"_Stz, xSysCall.iError());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* socket.connect: connect to a server                                        */
/*----------------------------------------------------------------------------*/
void Task::vSocketConnect(MP_Task mpTask)
{
  SysCall xSysCall;
  Socket* pSocket;
  Object* pObject;
  ValWrk xNode;
  
  i8 iPort;
  
  ENTRY("vSocketConnect"_Stz);
  
  if ( ! mpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("socket.connect"_Stz);
  
  if ( ! mpTask->m_pDataStack->bPop(xNode) )
    THROW_INVALID_DSTACK("socket.connect"_Stz);
  
  if ( ! mpTask->m_pDataStack->bPop(iPort) )
    THROW_INVALID_DSTACK("socket.connect"_Stz);
  
  // get address of node
  Stz* sAddr = reinterpret_cast<Stz*>(_pStrVal(xNode.pNestedValueSelf())->aChar());
  
  // get pSocket
  pSocket = reinterpret_cast<Socket*>(pObject);
  
  bool bResult = false;
  
  // try connecting in a loop if timeout occurs
  // possibly other node not back to accepting connections
  
  i8 iTimedOutCount = 0;
  i8 iConnRefusedCount = 0;
  
  while (!bResult) {
    bResult = pSocket->bConnectServer(sAddr, (int)iPort, xSysCall);
    if ( ! bResult ) {
      i8 iError = xSysCall.iError();
      
      if (iError == ETIMEDOUT) {
        if (0 == iTimedOutCount++)
          vTrace("Connect timedout... retrying...\n"_Stz);
        continue;
      }
      
      if (iError == ECONNREFUSED) {
        if (0 == iConnRefusedCount++)
          vTrace("Connect refused... retrying...\n"_Stz);
        continue;
      }
      
      THROW("Unable to connect: Errno %d!"_Stz, iError);
    }
  }
  
  // return the socket object
  Object* pPtrSocket = pSocket;
  mpTask->m_pDataStack->vPushObject(pPtrSocket);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* socket.listen: listen in for socket connections                            */
/*----------------------------------------------------------------------------*/
void Task::vSocketListen(MP_Task mpTask)
{
  SysCall xSysCall;
  Socket* pSocket;
  i8      iPort;
  
  ENTRY("vSocketListen"_Stz);
  
  if ( ! mpTask->m_pDataStack->bPop(iPort) )
    THROW_INVALID_DSTACK("socketlisten"_Stz);
  
  pSocket = new Socket;
  bool bResult = pSocket->bCreateServer((int)iPort, xSysCall);
  
  if ( ! bResult ) {
    if (xSysCall.iError() == EADDRINUSE) {
      THROW("Address in use (port: %lld). Check if another instance of RAPIDSSE is running!"_Stz,
      iPort);
    }
    else
      THROW("Unable to listen for connections: Errno %d!"_Stz, xSysCall.iError());
  }
  
  // return new socket
  Object* pPtrSocket = pSocket;
  mpTask->m_pDataStack->vPushObject(pPtrSocket);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* socket.open: open a socket                                                 */
/*----------------------------------------------------------------------------*/
void Task::vSocketOpen(MP_Task mpTask)
{
  SysCall xSysCall;
  Socket* pSocket;
  
  ENTRY("vSocketOpen"_Stz);
  
  pSocket = new Socket;
  bool bResult = pSocket->bSocket(xSysCall);
  
  if ( ! bResult )
    THROW("Unable to create socker: Errno %d!"_Stz, xSysCall.iError());
  
  // return pSocket
  Object* pPtrSocket = pSocket;
  mpTask->m_pDataStack->vPushObject(pPtrSocket);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* socket.send: send to a socket ( valset socket -- 1 )                       */
/*----------------------------------------------------------------------------*/
void Task::vSocketSend(MP_Task mpTask)
{
  ENTRY("vSocketSend"_Stz);
  
  SysCall xSysCall;
  Socket* pSocket;
  Object* pObject;
  ValSet  xValSet;
  bool    bResult;
  
  if ( ! mpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("socket.send"_Stz);
  
  if ( ! mpTask->m_pDataStack->bPop(xValSet) )
    THROW_INVALID_DSTACK("socket.send"_Stz);
  
  pSocket = reinterpret_cast<Socket*>(pObject);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // send the size of valset
  i8 iBytes = xValSet.nSize();
  bResult = pSocket->bSendTo(sizeof(iBytes), &iBytes, xSysCall);
  
  if ( ! bResult ) {
    //THROW("Unable to sendTo socket: Errno %d!", xSysCall.iError());
    mpTask->m_pDataStack->vPushInt((i8)0);
    
    //goto exit;
    EXIT();
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // now send the actual value set
  bResult = pSocket->bSendTo(iBytes, &xValSet, xSysCall);
  
  if ( ! bResult ) {
    //THROW("Unable to sendTo socket: Errno %d!", xSysCall.iError());
    mpTask->m_pDataStack->vPushInt((i8)0);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get rowBuffer if any in valset
  EntryBuffer* pRowBuffer = xValSet.pGetRowBuffer();
  
  if (pRowBuffer) {
    
    // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
    // send rowBuffer size
    i8 iBytes = pRowBuffer->nUsedBytes();
    bResult = pSocket->bSendTo(sizeof(iBytes), &iBytes, xSysCall);
    
    if ( ! bResult ) {
      
      // delete the rowbuffer if its temporary
      if (BD_TEMPORARY == pRowBuffer->eDurability())
        delete pRowBuffer;
      
      //THROW("Unable to sendTo socket: Errno %d!", xSysCall.iError());
      mpTask->m_pDataStack->vPushInt((i8)0);
      
      //goto exit;
      EXIT();
    }
    
    // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
    // send rowBuffer itself
    bResult = pSocket->bSendTo(iBytes, pRowBuffer->aDataArea(), xSysCall);
    
    // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
    // delete the rowbuffer if its temporary
    if (BD_TEMPORARY == pRowBuffer->eDurability())
      delete pRowBuffer;
    
    // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
    // test send result
    if ( ! bResult ) {
      
      //THROW("Unable to sendTo socket: Errno %d!", xSysCall.iError());
      mpTask->m_pDataStack->vPushInt((i8)0);
      
      //goto exit;
      EXIT();
    }
  }
  
  mpTask->m_pDataStack->vPushInt((i8)1);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* socket.recv: recv from a socket                                            */
/*----------------------------------------------------------------------------*/
void Task::vSocketRecv(MP_Task mpTask)
{
  ENTRY("vSocketRecv"_Stz);
  
  SysCall xSysCall;
  Socket* pSocket;
  ValSet xValSet;
  //s48K   sWorkspace;
  Object* pObject;
  i8 iBytes;
  
  if ( ! mpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("socketrecv"_Stz);
  
  pSocket = reinterpret_cast<Socket*>(pObject);
  
  if (gbTracepoint)
    vTrace("[N%lld:%8s\\%lld %-11s] receiving on %lld\n"_Stz,
           gpGrid->iThisNode(),
           mpTask->m_sName.aChar(),
           *mpTask->m_zAcks,
           mpTask->m_sComment.aChar(),
           
           pSocket->m_iSocketId);
  
  //******************************************************************
  // Receive the size of valset
  //******************************************************************
  bool bResult = pSocket->bRecvBytes(sizeof(iBytes), reinterpret_cast<void*>(&iBytes), xSysCall);
  
  if ( ! bResult )
    THROW("Unable to recvFrom socket: Errno %d!"_Stz, xSysCall.iError());
  
  //******************************************************************
  // Receive the valset
  //******************************************************************
  bResult = pSocket->bRecvBytes(iBytes, reinterpret_cast<void*>(&xValSet), xSysCall);
  
  if ( ! bResult )
    THROW("Unable to recvFrom socket: Errno %d!"_Stz, xSysCall.iError());
  
  //******************************************************************
  // Check if valset has a row buffer
  //******************************************************************
  bool bHasRowBuffer = xValSet.bHasRowBuffer();
  
  if (bHasRowBuffer) {
    
    //******************************************************************
    // receive rowBuffer size
    //******************************************************************
    bResult = pSocket->bRecvBytes(sizeof(iBytes), reinterpret_cast<void*>(&iBytes), xSysCall);
    
    if ( ! bResult )
      THROW("Unable to recvFrom socket: Errno %d!"_Stz, xSysCall.iError());
    
    //******************************************************************
    // receive rowBuffer
    //******************************************************************
    EntryBuffer* pRowBuffer = (EntryBuffer*)new EntryBuffer(iBytes);
    bResult = pSocket->bRecvBytes(iBytes, reinterpret_cast<void*>(pRowBuffer->aDataArea()), xSysCall);
    
    if ( ! bResult )
      THROW("Unable to recvFrom socket: Errno %d!"_Stz, xSysCall.iError());
    
    pRowBuffer->vRowCount();
    //vTrace("%lld \n"_Stz, *(pRowBuffer->nBufferNo()));
    
    // set the new row buffer in each entry of the row buffer
    Id xId;
    xId.vBuffer(*(pRowBuffer->nBufferNo()));
    for (Entry* pEntry = pRowBuffer->pNextEntry(pRowBuffer->pHead()); pEntry != nullptr;
         pEntry = pRowBuffer->pNextEntry(pEntry)) {
      xId.vRow(pEntry->xId().iRow());
               
      pEntry->vId(xId);
      
    }
    //******************************************************************
    // set rowBuffer in valset
    //******************************************************************
    bool bSet = xValSet.bSetRowBuffer(pRowBuffer);
    ASSERT(bSet);
  }
  
  mpTask->m_pDataStack->vPushValSet(xValSet);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
