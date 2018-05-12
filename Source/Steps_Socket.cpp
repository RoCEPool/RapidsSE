/*============================================================================*/
/* Steps_Socket.cpp - socket step group                                       */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap*      gpAttachMap;

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
  if (iBytes == 0)
    vTrace("xValset of size 0 being sent\n"_Stz);
  
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
    
    //goto exit;
    EXIT();
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get entryBuffer if any in valset
  EntryBuffer* pEntryBuffer = xValSet.pGetRowBuffer();
  
  if (pEntryBuffer) {
    
    //pEntryBuffer->vShow();
    
    //vTrace("Sending EntryBuffer: %p\n"_Stz,pEntryBuffer);
    
    // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
    // send entryBuffer size
    i8 iBytes = pEntryBuffer->nUsedBytes();
    //vTrace("sending %lld bytes\n"_Stz, iBytes);
    bResult = pSocket->bSendTo(sizeof(iBytes), &iBytes, xSysCall);
    
    if ( ! bResult ) {
      
      //THROW("Unable to sendTo socket: Errno %d!", xSysCall.iError());
      mpTask->m_pDataStack->vPushInt((i8)0);
      
      //goto exit;
      EXIT();
    }
    
    // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
    // send entryBuffer itself
    bResult = pSocket->bSendTo(iBytes, pEntryBuffer->aDataArea(), xSysCall);
    
    // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
    // test send result
    if ( ! bResult ) {
      
      //THROW("Unable to sendTo socket: Errno %d!", xSysCall.iError());
      mpTask->m_pDataStack->vPushInt((i8)0);
      
      //goto exit;
      EXIT();
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get workingBuffer if any in valset
  WorkingBuffer* pWorkingBuffer = xValSet.pGetWorkingBuffer();
  
  if (pWorkingBuffer) {
    
    //pWorkingBuffer->vShow();
    
    //vTrace("Sending WorkingBuffer: %p\n"_Stz,pWorkingBuffer);
    
    // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
    // send entryBuffer size
    i8 iBytes = pWorkingBuffer->nUsedBytes();
    //vTrace("sending %lld bytes\n"_Stz, iBytes);

    bResult = pSocket->bSendTo(sizeof(iBytes), &iBytes, xSysCall);
    
    if ( ! bResult ) {
      
      // delete the working buffer
      delete pWorkingBuffer;
      
      //THROW("Unable to sendTo socket: Errno %d!", xSysCall.iError());
      mpTask->m_pDataStack->vPushInt((i8)0);
      
      //goto exit;
      EXIT();
    }
    
    if (iBytes) {
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      // send entryBuffer itself
      bResult = pSocket->bSendTo(iBytes, pWorkingBuffer->aDataArea(), xSysCall);
      
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      // test send result
      if ( ! bResult ) {
        
        //THROW("Unable to sendTo socket: Errno %d!", xSysCall.iError());
        mpTask->m_pDataStack->vPushInt((i8)0);
        
        //goto exit;
        EXIT();
      }
    }
    
    // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
    // delete the working buffer
    delete pWorkingBuffer;
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
  
  if (gpAttachMap->m_bTracepoint)
    vTrace("[N%lld:%8s\\%lld %-11s] receiving on %lld\n"_Stz,
           gpAttachMap->m_pGrid->iThisNode(),
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
  // Check if valset has a entry buffer
  //******************************************************************
  bool bHasEntryBuffer = xValSet.bHasRowBuffer();
  
  if (bHasEntryBuffer) {
    
    //******************************************************************
    // receive entryBuffer size
    //******************************************************************
    bResult = pSocket->bRecvBytes(sizeof(iBytes), reinterpret_cast<void*>(&iBytes), xSysCall);
    
    if ( ! bResult )
      THROW("Unable to recvFrom socket: Errno %d!"_Stz, xSysCall.iError());
    
    if (iBytes == 0) {
      vTrace("EntryBuffer size 0 to be received\n"_Stz);
    }
             
    //******************************************************************
    // receive entryBuffer
    //******************************************************************
    EntryBuffer* pEntryBuffer = (EntryBuffer*)new EntryBuffer();
    pEntryBuffer->aAllotSpace(iBytes);
    
    //vTrace("Recvd EntryBuffer: %p\n"_Stz,pEntryBuffer);
    bResult = pSocket->bRecvBytes(iBytes, reinterpret_cast<void*>(pEntryBuffer->aDataArea()), xSysCall);
    
    if ( ! bResult )
      THROW("Unable to recvFrom socket: Errno %d!"_Stz, xSysCall.iError());
    
    pEntryBuffer->vRestore();
    
    //pEntryBuffer->vShow();
    
    //vTrace("%lld %lld %lld\n"_Stz, iBytes, *(pEntryBuffer->nBufferNo()), pEntryBuffer->nItems());
    
    // set the new entry buffer in each entry of the entry buffer
    Id xId;
    xId.vBuffer(*(pEntryBuffer->nBufferNo()));
    for (Entry* pEntry = pEntryBuffer->pFirstEntry(); pEntry != nullptr;
         pEntry = pEntryBuffer->pNextEntry(pEntry, 0)) {
      xId.vRow(pEntry->xId().iRow());
               
      pEntry->vId(xId);
    }
    
    //******************************************************************
    // set entryBuffer in valset
    //******************************************************************
    bool bSet = xValSet.bSetRowBuffer(pEntryBuffer);
    ASSERT(bSet);
  }

  //******************************************************************
  // Check if valset has a working buffer
  //******************************************************************
  bool bHasWorkingBuffer = xValSet.bHasWorkingBuffer();
  
  if (bHasWorkingBuffer) {
    
    //******************************************************************
    // receive workingBuffer size
    //******************************************************************
    bResult = pSocket->bRecvBytes(sizeof(iBytes), reinterpret_cast<void*>(&iBytes), xSysCall);
    
    if ( ! bResult )
      THROW("Unable to recvFrom socket: Errno %d!"_Stz, xSysCall.iError());
    
    //******************************************************************
    // receive workingBuffer
    //******************************************************************
    WorkingBuffer* pWorkingBuffer = (WorkingBuffer*)new WorkingBuffer(nullptr, -1);
    
    if (iBytes) {
      pWorkingBuffer->pAllotSpace(iBytes);
      
      //vTrace("Recvd EntryBuffer: %p\n"_Stz,pEntryBuffer);
      bResult = pSocket->bRecvBytes(iBytes, reinterpret_cast<void*>(pWorkingBuffer->aDataArea()), xSysCall);
      
      if ( ! bResult )
        THROW("Unable to recvFrom socket: Errno %d!"_Stz, xSysCall.iError());
      
      pWorkingBuffer->vRestore();
      
      //pEntryBuffer->vShow();
    }
    
    //******************************************************************
    // set workingBuffer in valset
    //******************************************************************
    bool bSet = xValSet.bSetWorkingBuffer(pWorkingBuffer);
    ASSERT(bSet);
  }
  mpTask->m_pDataStack->vPushValSet(xValSet);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
