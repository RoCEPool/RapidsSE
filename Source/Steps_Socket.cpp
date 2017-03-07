/*============================================================================*/
/* Steps_Socket.cpp - socket step group                                       */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern Arguments       gxArguments;

extern AttachMap*      gpAttachMap;
extern BufferPool*     gpBufferPool;
extern Heap*           gpMediumHeap;
extern BPA*            gpBPA;
extern Dic*            gpMainDic;
extern Word*           gpWord;

extern Grid*           gpGrid;
extern StepArray*      gpStepArray;
extern ThreadList*     gpThreadList;
extern TaskList*       gpTaskList;
extern TxCollection*   gpTxCollection;

extern NP_Task         gnpRootTask;
extern StepBoard*      gpStepBoard;
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;

// socket: /////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Socket(void)
{
  vAddPrimitive("socket.accept"_Str,   &vSocketAccept,     false);
  vAddPrimitive("socket.close"_Str,    &vSocketClose,      false);
  vAddPrimitive("socket.connect"_Str,  &vSocketConnect,    false);
  vAddPrimitive("socket.listen"_Str,   &vSocketListen,     false);
  vAddPrimitive("socket.open"_Str,     &vSocketOpen,       false);
  vAddPrimitive("socket.recv"_Str,     &vSocketRecv,       false);
  vAddPrimitive("socket.send"_Str,     &vSocketSend,       false);
}

/*----------------------------------------------------------------------------*/
/* socket.accept: accept a socket connection                                  */
/*----------------------------------------------------------------------------*/
void Task::vSocketAccept(NNP_Task nnpTask)
{
  SysCall xSysCall;
  Socket* pSocket;
  int     iSocketId;
  Object* pObject;
  
  ENTRY("vSocketAccept");
  
  if ( ! nnpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("socket.accept");
  
  // get pSocket
  pSocket = reinterpret_cast<Socket*>(pObject);
  
  // accept a connection
  bool bResult = pSocket->bAcceptClient(iSocketId, xSysCall);
  
  if ( ! bResult )
    THROW("Unable to accept connection: Errno %d!", xSysCall.iError());
  
  Task* pTask = (Task*)0x208f500c8;
  
  // return the new socket
  nnpTask->m_pDataStack->vPush(new Socket(iSocketId));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* open a socket                                                              */
/*----------------------------------------------------------------------------*/
void Task::vSocketClose(NNP_Task nnpTask)
{
  SysCall xSysCall;
  Socket* pSocket;
  Object* pObject;
  
  ENTRY("vSocketClose");
  
  if ( ! nnpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("socketclose");
  
  // get pSocket
  pSocket = reinterpret_cast<Socket*>(pObject);
  
  // close the socket
  bool bResult = pSocket->bClose(xSysCall);
  
  if ( ! bResult )
    THROW("Unable to close socket: Errno %d!", xSysCall.iError());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* socket.connect: connect to a server                                        */
/*----------------------------------------------------------------------------*/
void Task::vSocketConnect(NNP_Task nnpTask)
{
  SysCall xSysCall;
  Socket* pSocket;
  Object* pObject;
  ValWsp xNode;
  
  i8 iPort;
  
  ENTRY("vSocketConnect");
  
  if ( ! nnpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("socket.connect");
  
  if ( ! nnpTask->m_pDataStack->bPop(xNode) )
    THROW_INVALID_DSTACK("socket.connect");
  
  if ( ! nnpTask->m_pDataStack->bPop(iPort) )
    THROW_INVALID_DSTACK("socket.connect");
  
  // get address of node
  const char* sAddr = reinterpret_cast<char*>(_pStrVal(&xNode)->aStr());
  
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
          vTrace("Connect timedout... retrying...\n");
        continue;
      }
      
      if (iError == ECONNREFUSED) {
        if (0 == iConnRefusedCount++)
          vTrace("Connect refused... retrying...\n");
        continue;
      }
      
      THROW("Unable to connect: Errno %d!", iError);
    }
  }
  
  // return the socket object
  Object* pPtrSocket = pSocket;
  nnpTask->m_pDataStack->vPush(pPtrSocket);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* socket.listen: listen in for socket connections                            */
/*----------------------------------------------------------------------------*/
void Task::vSocketListen(NNP_Task nnpTask)
{
  SysCall xSysCall;
  Socket* pSocket;
  i8      iPort;
  
  ENTRY("vSocketListen");
  
  if ( ! nnpTask->m_pDataStack->bPop(iPort) )
    THROW_INVALID_DSTACK("socketlisten");
  
  pSocket = new Socket;
  bool bResult = pSocket->bCreateServer((int)iPort, xSysCall);
  
  if ( ! bResult )
    THROW("Unable to listen for connections: Errno %d!", xSysCall.iError());
  
  // return new socket
  Object* pPtrSocket = pSocket;
  nnpTask->m_pDataStack->vPush(pPtrSocket);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* socket.open: open a socket                                                 */
/*----------------------------------------------------------------------------*/
void Task::vSocketOpen(NNP_Task nnpTask)
{
  SysCall xSysCall;
  Socket* pSocket;
  
  ENTRY("vSocketOpen");
  
  pSocket = new Socket;
  bool bResult = pSocket->bSocket(xSysCall);
  
  if ( ! bResult )
    THROW("Unable to create socker: Errno %d!", xSysCall.iError());
  
  // return pSocket
  Object* pPtrSocket = pSocket;
  nnpTask->m_pDataStack->vPush(pPtrSocket);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* socket.send: send to a socket ( valset socket -- 1 )                       */
/*----------------------------------------------------------------------------*/
void Task::vSocketSend(NNP_Task nnpTask)
{
  ENTRY("vSocketSend");
  
  SysCall xSysCall;
  Socket* pSocket;
  Object* pObject;
  ValSet  xValSet;
  bool    bResult;
  
  if ( ! nnpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("socket.send");
  
  if ( ! nnpTask->m_pDataStack->bPop(xValSet) )
    THROW_INVALID_DSTACK("socket.send");
  
  pSocket = reinterpret_cast<Socket*>(pObject);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // send the size of valset
  i8 iBytes = xValSet.nSize();
  bResult = pSocket->bSendTo(sizeof(iBytes), &iBytes, xSysCall);
  
  if ( ! bResult ) {
    //THROW("Unable to sendTo socket: Errno %d!", xSysCall.iError());
    nnpTask->m_pDataStack->vPush((i8)0);
    
    //goto exit;
    EXIT();
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // now send the actual value set
  bResult = pSocket->bSendTo(iBytes, &xValSet, xSysCall);
  
  if ( ! bResult ) {
    //THROW("Unable to sendTo socket: Errno %d!", xSysCall.iError());
    nnpTask->m_pDataStack->vPush((i8)0);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get rowBuffer if any in valset
  RowBuffer* pRowBuffer = xValSet.pGetRowBuffer();
  
  if (pRowBuffer) {
    
    // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
    // send rowBuffer size
    i8 iBytes = pRowBuffer->iBytesUsed();
    bResult = pSocket->bSendTo(sizeof(iBytes), &iBytes, xSysCall);
    
    if ( ! bResult ) {
      
      // delete the rowbuffer if its temporary
      if (RD_TEMPORARY == pRowBuffer->eDurability())
        delete pRowBuffer;
      
      //THROW("Unable to sendTo socket: Errno %d!", xSysCall.iError());
      nnpTask->m_pDataStack->vPush((i8)0);
      
      //goto exit;
      EXIT();
    }
    
    // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
    // send rowBuffer itself
    bResult = pSocket->bSendTo(iBytes, pRowBuffer->pDataArea(), xSysCall);
    
    // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
    // delete the rowbuffer if its temporary
    if (RD_TEMPORARY == pRowBuffer->eDurability())
      delete pRowBuffer;
    
    // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
    // test send result
    if ( ! bResult ) {
      
      //THROW("Unable to sendTo socket: Errno %d!", xSysCall.iError());
      nnpTask->m_pDataStack->vPush((i8)0);
      
      //goto exit;
      EXIT();
    }
  }
  
  nnpTask->m_pDataStack->vPush((i8)1);
  
exit:
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* socket.recv: recv from a socket                                            */
/*----------------------------------------------------------------------------*/
void Task::vSocketRecv(NNP_Task nnpTask)
{
  ENTRY("vSocketRecv");
  
  SysCall xSysCall;
  Socket* pSocket;
  ValSet xValSet;
  //s48K   sWorkspace;
  Object* pObject;
  i8 iBytes;
  
  if ( ! nnpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("socketrecv");
  
  pSocket = reinterpret_cast<Socket*>(pObject);
  
  if (gxArguments.m_bTracepoint)
    vTrace("[N%lld:%8s\\%lld %-11s] receiving on %lld\n"_Str,
           gpGrid->iThisNode(),
           nnpTask->m_sName.aStr(),
           *nnpTask->m_zAcks,
           nnpTask->m_sComment.aStr(),
           
           pSocket->m_iSocketId);
  
  //******************************************************************
  // Receive the size of valset
  //******************************************************************
  bool bResult = pSocket->bRecvBytes(sizeof(iBytes), reinterpret_cast<void*>(&iBytes), xSysCall);
  
  if ( ! bResult )
    THROW("Unable to recvFrom socket: Errno %d!", xSysCall.iError());
  
  //******************************************************************
  // Receive the valset
  //******************************************************************
  bResult = pSocket->bRecvBytes(iBytes, reinterpret_cast<void*>(&xValSet), xSysCall);
  
  if ( ! bResult )
    THROW("Unable to recvFrom socket: Errno %d!", xSysCall.iError());
  
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
      THROW("Unable to recvFrom socket: Errno %d!", xSysCall.iError());
    
    //******************************************************************
    // receive rowBuffer
    //******************************************************************
    RowBuffer* pRowBuffer = new RowBuffer(iBytes);
    bResult = pSocket->bRecvBytes(iBytes, reinterpret_cast<void*>(pRowBuffer->pDataArea()), xSysCall);
    
    if ( ! bResult )
      THROW("Unable to recvFrom socket: Errno %d!", xSysCall.iError());
    
    pRowBuffer->vRowCount();
    
    //******************************************************************
    // set rowBuffer in valset
    //******************************************************************
    bool bSet = xValSet.bSetRowBuffer(pRowBuffer);
    ASSERT(bSet);
  }
  
  nnpTask->m_pDataStack->vPush(xValSet);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
