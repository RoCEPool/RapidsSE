/*============================================================================*/
/* Steps_Socket.cpp - socket step group                                       */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;
extern i8  giClassSize[];

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Socket(void)
{
  vAddStep("socket::open"_Stz,      &Step::vSocketOpen);
  vAddStep("socket::listen"_Stz,    &Step::vSocketListen);
  
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
void Step::vSocketAccept(MP_Task mpTask)
{
  SysCall oSysCall;
  Socket* pSocket;
  int     iSocketId;
  Object* pObject;
  
  ENTRY("vSocketAccept"_Stz);
  
  if ( ! mpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("socket.accept"_Stz);
  
  // get pSocket
  pSocket = reinterpret_cast<Socket*>(pObject);
  
  // accept a connection
  bool bResult = pSocket->bAcceptClient(iSocketId, oSysCall);
  
  if ( ! bResult )
    THROW("Unable to accept connection: Errno %d!"_Stz, oSysCall.iError());
  
  // return the new socket
  Socket* pNewSocket = new Socket(iSocketId);
  
  mpTask->m_pDataStack->vPushPointer(pNewSocket);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* open a socket                                                              */
/*----------------------------------------------------------------------------*/
void Step::vSocketClose(MP_Task mpTask)
{
  SysCall oSysCall;
  Socket* pSocket;
  Object* pObject;
  
  ENTRY("vSocketClose"_Stz);
  
  if ( ! mpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("socketclose"_Stz);
  
  // get pSocket
  pSocket = reinterpret_cast<Socket*>(pObject);
  
  // close the socket
  bool bResult = pSocket->bClose(oSysCall);
  
  if ( ! bResult )
    THROW("Unable to close socket: Errno %d!"_Stz, oSysCall.iError());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* socket.connect: connect to a server                                        */
/*----------------------------------------------------------------------------*/
void Step::vSocketConnect(MP_Task mpTask)
{
  SysCall oSysCall;
  Socket* pSocket;
  Object* pObject;
  GenVar xNodeVar;
  
  i8 iPort;
  
  ENTRY("vSocketConnect"_Stz);
  
  if ( ! mpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("socket.connect"_Stz);
  
  if ( ! mpTask->m_pDataStack->bPop(xNodeVar) )
    THROW_INVALID_DSTACK("socket.connect"_Stz);
  
  if ( ! mpTask->m_pDataStack->bPop(iPort) )
    THROW_INVALID_DSTACK("socket.connect"_Stz);
  
  // get address of node
  Stz* sAddr = reinterpret_cast<Stz*>(_aStrVal(xNodeVar.aNestedValue())->aChar());
  
  // get pSocket
  pSocket = reinterpret_cast<Socket*>(pObject);
  
  bool bResult = false;
  
  // try connecting in a loop if timeout occurs
  // possibly other node not back to accepting connections
  
  i8 iTimedOutCount = 0;
  i8 iConnRefusedCount = 0;
  
  while (!bResult) {
    bResult = pSocket->bConnectServer(sAddr, (int)iPort, oSysCall);
    if ( ! bResult ) {
      i8 iError = oSysCall.iError();
      
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
  mpTask->m_pDataStack->vPushPointer(pPtrSocket);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* socket.dropWrite: drop socket write lock                                   */
/*----------------------------------------------------------------------------*/
void Step::vSocketDropWrite(MP_Task mpTask)
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
void Step::vSocketGrabWrite(MP_Task mpTask)
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
void Step::vSocketListen(MP_Task mpTask)
{
  SysCall oSysCall;
  Socket* pSocket;
  i8      iPort;
  
  ENTRY("vSocketListen"_Stz);
  
  if ( ! mpTask->m_pDataStack->bPop(iPort) )
    THROW_INVALID_DSTACK("socketlisten"_Stz);
  
  pSocket = new Socket;
  bool bResult = pSocket->bCreateServer((int)iPort, oSysCall);
  
  if ( ! bResult ) {
    if (oSysCall.iError() == EADDRINUSE) {
      THROW("Address in use (port: %lld). Check if another instance of RAPIDSSE is running!"_Stz,
      iPort);
    }
    else
      THROW("Unable to listen for connections: Errno %d!"_Stz, oSysCall.iError());
  }
  
  // return new socket
  Object* pPtrSocket = pSocket;
  mpTask->m_pDataStack->vPushPointer(pPtrSocket);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* socket::open: open a socket                                                */
/*----------------------------------------------------------------------------*/
void Step::vSocketOpen(MP_Task mpTask)
{
  SysCall oSysCall;
  Socket* pSocket;
  
  ENTRY("vSocketOpen"_Stz);
  
  pSocket = new Socket;
  bool bResult = pSocket->bSocket(oSysCall);
  
  if ( ! bResult )
    THROW("Unable to create socket: Errno %d!"_Stz, oSysCall.iError());
  
  Object* pPtrSocket = pSocket;
  mpTask->m_pDataStack->vPushPointer(pPtrSocket);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* socket.recvObject: recv object from a socket                               */
/*----------------------------------------------------------------------------*/
void Step::vSocketRecvObject(MP_Task mpTask)
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
  
  char caObjectHeader[sizeof(Object)+C_1K];
  i8 nObjectBytes = sizeof(Object);
  SysCall oSysCall;

  bool bResult = pSocket->bRecvBytes(caObjectHeader,nObjectBytes, oSysCall);
  if (! bResult) {
    mpTask->m_pDataStack->vPushBool(false);
    EXIT();
    return;
  }
  
  if (gpAttachMap->m_bMarkResource[SR_SOCKET] ||
      gpAttachMap->m_bMarkResourceAll) {
    s256 sHeader;
    ((Header*)caObjectHeader)->vSprint(sHeader);
    ActivityMap::vMark(-SR_SOCKET,
                       "Recv header: %s"_Stz,
                       sHeader.aStz());
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the details about the received object
  
  Object* pObject = reinterpret_cast<Object*>(&caObjectHeader);
  eCN eClass = pObject->eClass();
  i8 nObjectSize = giClassSize[eClass];

  RASSERT(_bIsValidJob(eClass) || CN_ENTRYBUFFER == eClass);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // receive rest of job
  
  if ( _bIsValidJob(eClass) ) {
        
    //! temp. to be fixed
    Job* pJob = static_cast<Job*>(HeapMem::pAllocate(eClass));

    MemLib::vCopy(pJob,pObject,sizeof(Object),eClass);
    
    void* aBody = aAdd<void*>(pJob,sizeof(Object));
    i8 nRemainingBytes = nObjectSize - sizeof(Object);

    bResult = pSocket->bRecvBytes(aBody,nRemainingBytes,oSysCall);
    if (! bResult) {
      mpTask->m_pDataStack->vPushBool(false);
      EXIT();
      return;
    }

    //vTrace("vSocketRecvObject: recv %p %lld %s %lld bytes\n"_Stz,
    //       pJob,eClass,aCN[eClass],nObjectSize);
    
    if (gpAttachMap->m_bMarkResource[SR_SOCKET] ||
        gpAttachMap->m_bMarkResourceAll) {
      s256 sFooter;
      Footer* pFooter = aAdd<Footer*>(pJob,nObjectSize-sizeof(Footer));
      pFooter->vSprint(sFooter);
      ActivityMap::vMark(-SR_SOCKET,
                         "Recv footer: %s"_Stz,
                         sFooter.aStz());
    }
    if (pJob->m_bMark)
      ActivityMap::vMark(pJob->m_iJobId,
                         "Recv: %s %p socket %llx"_Stz,
                         aCN[eClass],
                         pJob,
                         pSocket);
                         
    mpTask->m_pDataStack->vPushPointer(pJob);
    mpTask->m_pDataStack->vPushBool(true);
    EXIT();
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // receive rest of working buffer
  
  if (eClass == CN_ENTRYBUFFER) {
    
    EntryBuffer* pEntryBuffer = new EntryBuffer();
    
    MemLib::vCopy(pEntryBuffer,pObject,sizeof(Object),CN_ENTRYBUFFER);

    void* aBody = aAdd<void*>(pEntryBuffer,sizeof(Object));
    i8 nRemainingBytes = giClassSize[CN_ENTRYBUFFER] - sizeof(Object);
    
    bResult = pSocket->bRecvBytes(aBody,nRemainingBytes,oSysCall);
    if (! bResult) {
      mpTask->m_pDataStack->vPushBool(false);
      EXIT();
      return;
    }

    //vTrace("vSocketRecvObject: recv %p %lld %s %lld bytes\n"_Stz,
    //       pEntryBuffer,eClass,aCN[eClass],nObjectSize);
    
    // restore the entryBuffer pointets (m_pHeadEntry etc)
    pEntryBuffer->vRestore();
    
    if (gpAttachMap->m_bMarkResource[SR_SOCKET] ||
        gpAttachMap->m_bMarkResourceAll) {
      ActivityMap::vMark(-SR_SOCKET,
                         "Recv EntryBuffer: %p"_Stz,
                         pEntryBuffer);
    }

    mpTask->m_pDataStack->vPushPointer(pEntryBuffer);
    mpTask->m_pDataStack->vPushBool(true);
    EXIT();
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // unknown object
  
  vTrace("socket.recvObject: unknown object; class %lld\n"_Stz,eClass);
  mpTask->m_pDataStack->vPushBool(false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* socket.recvObject_LowMem: recv object from a socket (under low mem cond)   */
/*----------------------------------------------------------------------------*/
void Step::vSocketRecvObject_LowMem(MP_Task mpTask)
{
  ENTRY("vSocketRecvObject_LowMem"_Stz);
  
  i8 nFreeBuffer = gpBufferPool->nFreeCount();
  
  bool bDiscardEntryBuffer = false;
  
  Object* pSocketPtr;
  if ( ! mpTask->m_pDataStack->bPop(pSocketPtr) )
    THROW_INVALID_DSTACK("socket.recvObject"_Stz);
  Socket* pSocket = _pSocket(pSocketPtr);
  
  void* aLowBuff = pSocket->m_aLowBuff;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // receiver the object header
  
  char caObjectHeader[sizeof(Object)+C_1K];
  i8 nObjectBytes = sizeof(Object);
  SysCall oSysCall;
  
  bool bResult = pSocket->bRecvBytes(caObjectHeader,nObjectBytes, oSysCall);
  if (! bResult) {
    mpTask->m_pDataStack->vPushBool(false);
    EXIT();
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the details about the received object
  
  Object* pObject = reinterpret_cast<Object*>(&caObjectHeader);
  eCN eClass = pObject->eClass();
  i8 nObjectSize = giClassSize[eClass];
  
  RASSERT(_bIsValidJob(eClass) || CN_ENTRYBUFFER == eClass);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // handle the specific case of job_writeBuffer under low memory condition
  
  if (CN_JOB_WRITEBUFFER == eClass) {

    vTrace("Received Job_WriteBuffer under low mem\n"_Stz);
    
    MemLib::vCopy(aLowBuff,pObject,sizeof(Object),eClass);
    
    void* aLowBody = aAdd<void*>(aLowBuff,sizeof(Object));
    i8 nRemainingBytes = sizeof(Job_WriteBuffer) - sizeof(Object);
    
    bResult = pSocket->bRecvBytes(aLowBody,nRemainingBytes,oSysCall);
    if (! bResult) {
      mpTask->m_pDataStack->vPushBool(false);
      EXIT();
      return;
    }
    
    Job_WriteBuffer* pJob_WriteBuffer =
      static_cast<Job_WriteBuffer*>(aLowBuff);

    bDiscardEntryBuffer = pJob_WriteBuffer->m_pCurrentBuffer ? true : false;

    pJob_WriteBuffer->m_bLowMem = true;
    
    Job* pJob = static_cast<Job*>(HeapMem::pAllocate(CN_JOB_WRITEBUFFER));
    MemLib::vCopy(pJob,pJob_WriteBuffer,sizeof(Job_WriteBuffer),CN_JOB_WRITEBUFFER);
    
    pJob_WriteBuffer = static_cast<Job_WriteBuffer*>(pJob);
    
    if (-1 != pJob_WriteBuffer->m_nWriteBehindBuffers) {
      
      pJob_WriteBuffer->m_pCurrentBuffer = nullptr;
    }
    else {
      //pJob_WriteBuffer = nullptr;
      //vTrace("Discarded Job_WriteBuffer under low mem\n"_Stz);
      pJob_WriteBuffer->m_pCurrentBuffer = nullptr;
    }

    if (bDiscardEntryBuffer) {

      bResult = pSocket->bRecvBytes(aLowBuff,sizeof(EntryBuffer),oSysCall);
      vTrace("Discarded EntryBuffer under low mem\n"_Stz);
    }
    
    mpTask->m_pDataStack->vPushPointer(pJob_WriteBuffer);
    mpTask->m_pDataStack->vPushBool(true);
    EXIT();
    return;

  }

  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // receive rest of job
  
  if ( _bIsValidJob(eClass) ) {
    
    //! temp. to be fixed
    Job* pJob = static_cast<Job*>(HeapMem::pAllocate(eClass));
    
    MemLib::vCopy(pJob,pObject,sizeof(Object),eClass);
    
    void* aBody = aAdd<void*>(pJob,sizeof(Object));
    i8 nRemainingBytes = giClassSize[eClass] - sizeof(Object);
    
    bResult = pSocket->bRecvBytes(aBody,nRemainingBytes,oSysCall);
    if (! bResult) {
      mpTask->m_pDataStack->vPushBool(false);
      EXIT();
      return;
    }
    
    //vTrace("vSocketRecvObject: recv %p %lld %s %lld bytes\n"_Stz,
    //       pJob,eClass,aCN[eClass],nObjectSize);
    
    mpTask->m_pDataStack->vPushPointer(pJob);
    mpTask->m_pDataStack->vPushBool(true);
    EXIT();
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // receive rest of working buffer
  
  if (eClass == CN_ENTRYBUFFER) {
    
    EntryBuffer* pEntryBuffer = new EntryBuffer();
    
    MemLib::vCopy(pEntryBuffer,pObject,sizeof(Object),CN_ENTRYBUFFER);
    
    void* aBody = aAdd<void*>(pEntryBuffer,sizeof(Object));
    i8 nRemainingBytes = giClassSize[CN_ENTRYBUFFER] - sizeof(Object);
    
    bResult = pSocket->bRecvBytes(aBody,nRemainingBytes,oSysCall);
    if (! bResult) {
      mpTask->m_pDataStack->vPushBool(false);
      EXIT();
      return;
    }
    
    //vTrace("vSocketRecvObject: recv %p %lld %s %lld bytes\n"_Stz,
    //       pEntryBuffer,eClass,aCN[eClass],nObjectSize);
    
    mpTask->m_pDataStack->vPushPointer(pEntryBuffer);
    mpTask->m_pDataStack->vPushBool(true);
    EXIT();
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // unknown object
  
  vTrace("socket.recvObject: unknown object; class %lld\n"_Stz,eClass);
  mpTask->m_pDataStack->vPushBool(false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* socket.sendObject: send object via a socket                                */
/*----------------------------------------------------------------------------*/
void Step::vSocketSendObject(MP_Task mpTask)
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
      ActivityMap::vMark(pJob->m_iJobId,"Send: %s %p socket %llx"_Stz,
                         aCN[eClass],
                         pJob,
                         pSocket);
  }

  RASSERT(_bIsValidJob(eClass) || CN_ENTRYBUFFER == eClass);
    
  SysCall oSysCall;
  bool bResult = pSocket->bSendTo(pObject, nObjectSize, oSysCall);

  mpTask->m_pDataStack->vPushBool(bResult);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
