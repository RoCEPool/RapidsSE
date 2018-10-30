/*============================================================================*/
/* Steps_Socket.cpp - socket step group                                       */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap*  gpAttachMap;
extern BufferPool* gpBufferPool;

// socket: /////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Socket(void)
{
  vAddStep("socket::open"_Stz,      &Task::vSocketOpen);
  vAddStep("socket::listen"_Stz,    &Task::vSocketListen);
  
  vAddStep("socket.accept"_Stz,     &Task::vSocketAccept);
  vAddStep("socket.close"_Stz,      &Task::vSocketClose);
  vAddStep("socket.connect"_Stz,    &Task::vSocketConnect);
  vAddStep("socket.dropWrite"_Stz,  &Task::vSocketDropWrite);
  vAddStep("socket.grabWrite"_Stz,  &Task::vSocketGrabWrite);
  vAddStep("socket.recvObject"_Stz, &Task::vSocketRecvObject);
  vAddStep("socket.recvValSet"_Stz, &Task::vSocketRecvValSet);
  vAddStep("socket.sendObject"_Stz, &Task::vSocketSendObject);
  vAddStep("socket.sendValSet"_Stz, &Task::vSocketSendValSet);
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
  
  // return the new socket
  Socket* pNewSocket = new Socket(iSocketId);
  
  mpTask->m_pDataStack->vPushObject(pNewSocket);
  
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
/* socket.dropWrite: drop socket write lock                                   */
/*----------------------------------------------------------------------------*/
void Task::vSocketDropWrite(MP_Task mpTask)
{
  ENTRY("vSocketDropWrite"_Stz);
  
  Object* pSocketPtr;
  if ( ! mpTask->m_pDataStack->bPop(pSocketPtr) )
    THROW_INVALID_DSTACK("socket.dropWrite"_Stz);
  Socket* pSocket = _pSocket(pSocketPtr);
  
  pSocket->vDropWrite();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* socket.grabWrite: grab socket write lock                                   */
/*----------------------------------------------------------------------------*/
void Task::vSocketGrabWrite(MP_Task mpTask)
{
  ENTRY("vSocketGrabWrite"_Stz);
  
  Object* pSocketPtr;
  if ( ! mpTask->m_pDataStack->bPop(pSocketPtr) )
    THROW_INVALID_DSTACK("socket.grabWrite"_Stz);
  Socket* pSocket = _pSocket(pSocketPtr);
  
  pSocket->vGrabWrite();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* socket::listen: listen in for socket connections                           */
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
/* socket::open: open a socket                                                */
/*----------------------------------------------------------------------------*/
void Task::vSocketOpen(MP_Task mpTask)
{
  SysCall xSysCall;
  Socket* pSocket;
  
  ENTRY("vSocketOpen"_Stz);
  
  pSocket = new Socket;
  bool bResult = pSocket->bSocket(xSysCall);
  
  if ( ! bResult )
    THROW("Unable to create socket: Errno %d!"_Stz, xSysCall.iError());
  
  Object* pPtrSocket = pSocket;
  mpTask->m_pDataStack->vPushObject(pPtrSocket);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* socket.recvObject: recv object from a socket                               */
/*----------------------------------------------------------------------------*/
void Task::vSocketRecvObject(MP_Task mpTask)
{
  // ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! !
  // test for low memory condition
  
  bool bLowMem = gpBufferPool->bLowMemory();
  if (bLowMem) {
    vSocketRecvObject_LowMem(mpTask);
    return;
  }
  
  // ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! !

  
  ENTRY("vSocketRecvObject"_Stz);
  
  Object* pSocketPtr;
  if ( ! mpTask->m_pDataStack->bPop(pSocketPtr) )
    THROW_INVALID_DSTACK("socket.recvObject"_Stz);
  Socket* pSocket = _pSocket(pSocketPtr);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // receiver the object header
  
  char xObjectHeader[sizeof(Object)+C_1K];
  i8 nObjectBytes = sizeof(Object);
  SysCall xSysCall;

  bool bResult = pSocket->bRecvBytes(xObjectHeader,nObjectBytes, xSysCall);
  if (! bResult) {
    mpTask->m_pDataStack->vPushInt((i8)0);
    EXIT();
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the details about the received object
  
  Object* pObject = reinterpret_cast<Object*>(&xObjectHeader);
  eCN eClass = pObject->eClass();
  i8 nObjectSize = giClassSize[eClass];

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // receive rest of job
  
  if ( _bIsValidJob(eClass) ) {
        
    //! temp. to be fixed
    Job* pJob = static_cast<Job*>(HeapMem::pAllocate(eClass));

    MemLib::vCopy(pJob,pObject,sizeof(Object),eClass);
    
    void* aBody = pAdd<void*>(pJob,sizeof(Object));
    i8 nRemainingBytes = nObjectSize - sizeof(Object);

    bResult = pSocket->bRecvBytes(aBody,nRemainingBytes,xSysCall);
    if (! bResult) {
      mpTask->m_pDataStack->vPushInt((i8)0);
      EXIT();
      return;
    }

    //vTrace("vSocketRecvObject: recv %p %lld %s %lld bytes\n"_Stz,
    //       pJob,eClass,aCN[eClass],nObjectSize);
    
    if (pJob->m_bMark)
      ActivityMap::vMark(pJob->m_iJobId,"recv %s from socket %llx"_Stz,
                         aCN[eClass],pSocket);
                         
    mpTask->m_pDataStack->vPushObject(pJob);
    mpTask->m_pDataStack->vPushInt((i8)1);
    EXIT();
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // receive rest of working buffer
  
  if (eClass == CN_WORKINGBUFFER) {
    
    WorkingBuffer* pWorkingBuffer = new WorkingBuffer(nullptr, -1);
    
    MemLib::vCopy(pWorkingBuffer,pObject,sizeof(Object),CN_WORKINGBUFFER);

    void* aBody = pAdd<void*>(pWorkingBuffer,sizeof(Object));
    i8 nRemainingBytes = giClassSize[CN_WORKINGBUFFER] - sizeof(Object);
    
    bResult = pSocket->bRecvBytes(aBody,nRemainingBytes,xSysCall);
    if (! bResult) {
      mpTask->m_pDataStack->vPushInt((i8)0);
      EXIT();
      return;
    }

    //vTrace("vSocketRecvObject: recv %p %lld %s %lld bytes\n"_Stz,
    //       pWorkingBuffer,eClass,aCN[eClass],nObjectSize);
    
    mpTask->m_pDataStack->vPushObject(pWorkingBuffer);
    mpTask->m_pDataStack->vPushInt((i8)1);
    EXIT();
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // unknown object
  
  vTrace("socket.recvObject: unknown object; class %lld\n"_Stz,eClass);
  mpTask->m_pDataStack->vPushInt((i8)0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* socket.recvObject_LowMem: recv object from a socket (under low mem cond)   */
/*----------------------------------------------------------------------------*/
void Task::vSocketRecvObject_LowMem(MP_Task mpTask)
{
  ENTRY("vSocketRecvObject_LowMem"_Stz);
  
  i8 nFreeBuffer = gpBufferPool->iFreeCount();
  
  bool bDiscardWorkingBuffer = false;
  
  Object* pSocketPtr;
  if ( ! mpTask->m_pDataStack->bPop(pSocketPtr) )
    THROW_INVALID_DSTACK("socket.recvObject"_Stz);
  Socket* pSocket = _pSocket(pSocketPtr);
  
  void* aLowBuff = pSocket->m_aLowBuff;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // receiver the object header
  
  char xObjectHeader[sizeof(Object)+C_1K];
  i8 nObjectBytes = sizeof(Object);
  SysCall xSysCall;
  
  bool bResult = pSocket->bRecvBytes(xObjectHeader,nObjectBytes, xSysCall);
  if (! bResult) {
    mpTask->m_pDataStack->vPushInt((i8)0);
    EXIT();
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the details about the received object
  
  Object* pObject = reinterpret_cast<Object*>(&xObjectHeader);
  eCN eClass = pObject->eClass();
  i8 nObjectSize = giClassSize[eClass];
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // handle the specific case of job_loadBuffer under low memory condition
  
  if (CN_JOB_LOADBUFFER == eClass) {

    vTrace("Received Job_LoadBuffer under low mem\n"_Stz);
    
    MemLib::vCopy(aLowBuff,pObject,sizeof(Object),eClass);
    
    void* aLowBody = pAdd<void*>(aLowBuff,sizeof(Object));
    i8 nRemainingBytes = sizeof(Job_LoadBuffer) - sizeof(Object);
    
    bResult = pSocket->bRecvBytes(aLowBody,nRemainingBytes,xSysCall);
    if (! bResult) {
      mpTask->m_pDataStack->vPushInt((i8)0);
      EXIT();
      return;
    }
    
    Job_LoadBuffer* pJob_LoadBuffer =
      static_cast<Job_LoadBuffer*>(aLowBuff);

    bDiscardWorkingBuffer = pJob_LoadBuffer->m_pWorkingBuffer ? true : false;

    pJob_LoadBuffer->m_bLowMem = true;
    
    Job* pJob = static_cast<Job*>(HeapMem::pAllocate(CN_JOB_LOADBUFFER));
    MemLib::vCopy(pJob,pJob_LoadBuffer,sizeof(Job_LoadBuffer),CN_JOB_LOADBUFFER);
    
    pJob_LoadBuffer = static_cast<Job_LoadBuffer*>(pJob);
    
    if (-1 != pJob_LoadBuffer->m_nBuffersFromSubLoader) {
      
      pJob_LoadBuffer->m_pWorkingBuffer = nullptr;
    }
    else {
      //pJob_LoadBuffer = nullptr;
      //vTrace("Discarded Job_LoadBuffer under low mem\n"_Stz);
      pJob_LoadBuffer->m_pWorkingBuffer = nullptr;
    }

    if (bDiscardWorkingBuffer) {

      bResult = pSocket->bRecvBytes(aLowBuff,sizeof(WorkingBuffer),xSysCall);
      vTrace("Discarded WorkingBuffer under low mem\n"_Stz);
    }
    
    mpTask->m_pDataStack->vPushObject(pJob_LoadBuffer);
    mpTask->m_pDataStack->vPushInt((i8)1);
    EXIT();
    return;

  }

  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // receive rest of job
  
  if ( _bIsValidJob(eClass) ) {
    
    //! temp. to be fixed
    Job* pJob = static_cast<Job*>(HeapMem::pAllocate(eClass));
    
    MemLib::vCopy(pJob,pObject,sizeof(Object),eClass);
    
    void* aBody = pAdd<void*>(pJob,sizeof(Object));
    i8 nRemainingBytes = giClassSize[eClass] - sizeof(Object);
    
    bResult = pSocket->bRecvBytes(aBody,nRemainingBytes,xSysCall);
    if (! bResult) {
      mpTask->m_pDataStack->vPushInt((i8)0);
      EXIT();
      return;
    }
    
    //vTrace("vSocketRecvObject: recv %p %lld %s %lld bytes\n"_Stz,
    //       pJob,eClass,aCN[eClass],nObjectSize);
    
    mpTask->m_pDataStack->vPushObject(pJob);
    mpTask->m_pDataStack->vPushInt((i8)1);
    EXIT();
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // receive rest of working buffer
  
  if (eClass == CN_WORKINGBUFFER) {
    
    WorkingBuffer* pWorkingBuffer = new WorkingBuffer(nullptr, -1);
    
    MemLib::vCopy(pWorkingBuffer,pObject,sizeof(Object),CN_WORKINGBUFFER);
    
    void* aBody = pAdd<void*>(pWorkingBuffer,sizeof(Object));
    i8 nRemainingBytes = giClassSize[CN_WORKINGBUFFER] - sizeof(Object);
    
    bResult = pSocket->bRecvBytes(aBody,nRemainingBytes,xSysCall);
    if (! bResult) {
      mpTask->m_pDataStack->vPushInt((i8)0);
      EXIT();
      return;
    }
    
    //vTrace("vSocketRecvObject: recv %p %lld %s %lld bytes\n"_Stz,
    //       pWorkingBuffer,eClass,aCN[eClass],nObjectSize);
    
    mpTask->m_pDataStack->vPushObject(pWorkingBuffer);
    mpTask->m_pDataStack->vPushInt((i8)1);
    EXIT();
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // unknown object
  
  vTrace("socket.recvObject: unknown object; class %lld\n"_Stz,eClass);
  mpTask->m_pDataStack->vPushInt((i8)0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* socket.recvValSet: recv from a socket                                      */
/*----------------------------------------------------------------------------*/
void Task::vSocketRecvValSet(MP_Task mpTask)
{
  ENTRY("vSocketRecvValSet"_Stz);
  
  SysCall xSysCall;
  Socket* pSocket;
  ValSet xValSet;
  Object* pObject;
  i8 iBytes;
  
  if ( ! mpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("socketrecv"_Stz);
  
  pSocket = reinterpret_cast<Socket*>(pObject);
  
  if (gpAttachMap->m_bTracepoint)
    vTrace("[N%lld:%8s\\%lld ] receiving on %lld\n"_Stz,
           gpAttachMap->m_pGrid->iThisNode(),
           mpTask->aJobName(),
           mpTask->m_xAcksRemaining.iPrintOnlyValue(),
           
           pSocket->m_iSocketId);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // receive the size of valset
  bool bResult = pSocket->bRecvBytes(reinterpret_cast<void*>(&iBytes), sizeof(iBytes), xSysCall);
  
  if ( ! bResult )
    THROW("Unable to recvFrom socket: Errno %d!"_Stz, xSysCall.iError());
  
  if (iBytes == 0) {
    vTrace("Valset size 0 to be received\n"_Stz);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // receive the valset
  bResult = pSocket->bRecvBytes(reinterpret_cast<void*>(&xValSet), iBytes, xSysCall);
  
  if ( ! bResult )
    THROW("Unable to recvFrom socket: Errno %d!"_Stz, xSysCall.iError());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // check if valset has a entry buffer
  bool bHasEntryBuffer = xValSet.bHasRowBuffer();
  
  if (bHasEntryBuffer) {
    
    // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
    // receive entryBuffer size
    bResult = pSocket->bRecvBytes(reinterpret_cast<void*>(&iBytes), sizeof(iBytes), xSysCall);
    
    if ( ! bResult )
      THROW("Unable to recvFrom socket: Errno %d!"_Stz, xSysCall.iError());
    
    if (iBytes == 0) {
      vTrace("EntryBuffer size 0 to be received\n"_Stz);
    }
             
    // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
    // receive entryBuffer
    EntryBuffer* pEntryBuffer = (EntryBuffer*)new EntryBuffer();
    pEntryBuffer->aAllotSpace(iBytes);
    
    //vTrace("Recvd EntryBuffer: %p\n"_Stz,pEntryBuffer);
    bResult = pSocket->bRecvBytes(reinterpret_cast<void*>(pEntryBuffer->aDataArea()), iBytes, xSysCall);
    
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
    
    // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
    // set entryBuffer in valset
    bool bSet = xValSet.bSetRowBuffer(pEntryBuffer);
    ASSERT(bSet);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // check if valset has a working buffer
  bool bHasWorkingBuffer = xValSet.bHasWorkingBuffer();
  
  if (bHasWorkingBuffer) {
    
    // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
    // receive workingBuffer size
    bResult = pSocket->bRecvBytes(reinterpret_cast<void*>(&iBytes), sizeof(iBytes), xSysCall);
    
    if ( ! bResult )
      THROW("Unable to recvFrom socket: Errno %d!"_Stz, xSysCall.iError());
    
    // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
    // receive workingBuffer
    WorkingBuffer* pWorkingBuffer = (WorkingBuffer*)new WorkingBuffer(nullptr, -1);
    
    if (iBytes) {
      pWorkingBuffer->pAllotSpace(iBytes);
      
      //vTrace("Recvd EntryBuffer: %p\n"_Stz,pEntryBuffer);
      bResult = pSocket->bRecvBytes(reinterpret_cast<void*>(pWorkingBuffer->aDataArea()), iBytes, xSysCall);
      
      if ( ! bResult )
        THROW("Unable to recvFrom socket: Errno %d!"_Stz, xSysCall.iError());
      
      pWorkingBuffer->vRestore();
      
      //pEntryBuffer->vShow();
    }
    
    // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
    // set workingBuffer in valset
    bool bSet = xValSet.bSetWorkingBuffer(pWorkingBuffer);
    ASSERT(bSet);
  }
  
  mpTask->m_pDataStack->vPushValSet(xValSet);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* socket.sendObject: send object via a socket                                */
/*----------------------------------------------------------------------------*/
void Task::vSocketSendObject(MP_Task mpTask)
{
  ENTRY("vSocketSendObject"_Stz);
  
  Object* pSocketPtr;
  if ( ! mpTask->m_pDataStack->bPop(pSocketPtr) )
    THROW_INVALID_DSTACK("socket.sendObject"_Stz);
  Socket* pSocket = _pSocket(pSocketPtr);
  
  Object* pObject;
  if ( ! mpTask->m_pDataStack->bPop(pObject) )
  THROW_INVALID_DSTACK("socket.sendObject"_Stz);

  eCN eClass = pObject->eClass();
  i8 nObjectSize = giClassSize[eClass];
  
  //vTrace("vSocketSendObject: send %p %lld %s %lld bytes\n"_Stz,
  //       pObject,eClass,aCN[eClass],nObjectSize);
  
  if (_bIsValidJob(eClass)) {
    Job* pJob = _pJob(pObject);
    if (pJob->m_bMark)
      ActivityMap::vMark(pJob->m_iJobId,"send %s to socket %llx"_Stz,
                         aCN[eClass],pSocket);
  }

  SysCall xSysCall;
  bool bResult = pSocket->bSendTo(pObject, nObjectSize, xSysCall);

  mpTask->m_pDataStack->vPushInt((i8)bResult);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* socket.sendValSet: send to a socket ( valset socket -- 1 )                 */
/*----------------------------------------------------------------------------*/
void Task::vSocketSendValSet(MP_Task mpTask)
{
  ENTRY("vSocketSendValSet"_Stz);
  
  SysCall xSysCall;
  Socket* pSocket;
  Object* pObject;
  ValSet  xValSet;
  bool    bResult;
  
  if ( ! mpTask->m_pDataStack->bPop(pObject) )
  THROW_INVALID_DSTACK("socket.sendValSet"_Stz);
  
  if ( ! mpTask->m_pDataStack->bPop(xValSet) )
  THROW_INVALID_DSTACK("socket.sendValSet"_Stz);
  
  pSocket = reinterpret_cast<Socket*>(pObject);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // send the size of valset
  i8 iBytes = xValSet.nSize();
  if (iBytes == 0)
  vTrace("xValset of size 0 being sent\n"_Stz);
  
  bResult = pSocket->bSendTo(&iBytes, sizeof(iBytes), xSysCall);
  
  if ( ! bResult ) {
    //THROW("Unable to sendTo socket: Errno %d!", xSysCall.iError());
    mpTask->m_pDataStack->vPushInt((i8)0);
    
    //goto exit;
    EXIT();
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // now send the actual value set
  bResult = pSocket->bSendTo(&xValSet, iBytes, xSysCall);
  
  if ( ! bResult ) {
    //THROW("Unable to sendTo socket: Errno %d!", xSysCall.iError());
    mpTask->m_pDataStack->vPushInt((i8)0);
    
    //goto exit;
    EXIT();
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get entryBuffer if any in valset
  EntryBuffer* pEntryBuffer = xValSet.pGetRowBuffer();
  
  if (pEntryBuffer) {
    
    //pEntryBuffer->vShow();
    
    //vTrace("Sending EntryBuffer: %p\n"_Stz,pEntryBuffer);
    
    // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
    // send entryBuffer size
    i8 iBytes = pEntryBuffer->nUsedBytes();
    //vTrace("sending %lld bytes\n"_Stz, iBytes);
    bResult = pSocket->bSendTo(&iBytes, sizeof(iBytes), xSysCall);
    
    if ( ! bResult ) {
      
      //THROW("Unable to sendTo socket: Errno %d!", xSysCall.iError());
      mpTask->m_pDataStack->vPushInt((i8)0);
      
      //goto exit;
      EXIT();
    }
    
    // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
    // send entryBuffer itself
    bResult = pSocket->bSendTo(pEntryBuffer->aDataArea(), iBytes, xSysCall);
    
    // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
    // test send result
    if ( ! bResult ) {
      
      //THROW("Unable to sendTo socket: Errno %d!", xSysCall.iError());
      mpTask->m_pDataStack->vPushInt((i8)0);
      
      //goto exit;
      EXIT();
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get workingBuffer if any in valset
  WorkingBuffer* pWorkingBuffer = xValSet.pGetWorkingBuffer();
  
  if (pWorkingBuffer) {
    
    //pWorkingBuffer->vShow();
    
    //vTrace("Sending WorkingBuffer: %p\n"_Stz,pWorkingBuffer);
    
    // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
    // send entryBuffer size
    i8 iBytes = pWorkingBuffer->nUsedBytes();
    //vTrace("sending %lld bytes\n"_Stz, iBytes);
    
    bResult = pSocket->bSendTo(&iBytes, sizeof(iBytes), xSysCall);
    
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
      bResult = pSocket->bSendTo(pWorkingBuffer->aDataArea(), iBytes, xSysCall);
      
      // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
      // test send result
      if ( ! bResult ) {
        
        //THROW("Unable to sendTo socket: Errno %d!", xSysCall.iError());
        mpTask->m_pDataStack->vPushInt((i8)0);
        
        //goto exit;
        EXIT();
      }
    }
    
    // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
    // delete the working buffer
    delete pWorkingBuffer;
  }
  
  mpTask->m_pDataStack->vPushInt((i8)1);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
