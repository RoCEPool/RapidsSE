/*============================================================================*/
/* Steps_Socket.cpp - socket step group                                       */
/*============================================================================*/

#include "Steps.h"

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
void Step::vSocketAccept(void)
{
  ENTRY(); // ENTRY("vSocketAccept"_Stz);
  
  SysCall oSysCall;
  Socket* pSocket;
  int     iSocketId;
  Object* pObject;
  
  
  if ( ! m_pDataStack->bPopPtr(pObject) )
    THROW_DSTACK();
  
  // get pSocket
  pSocket = reinterpret_cast<Socket*>(pObject);
  
  // accept a connection
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
  ENTRY(); // ENTRY("vSocketClose"_Stz);
  
  SysCall oSysCall;
  Socket* pSocket;
  Object* pObject;
  
  
  if ( ! m_pDataStack->bPopPtr(pObject) )
    THROW_DSTACK();
  
  // get pSocket
  pSocket = reinterpret_cast<Socket*>(pObject);
  
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
  ENTRY(); // ENTRY("vSocketConnect"_Stz);
  
  SysCall oSysCall;
  Socket* pSocket;
  Object* pObject;
  GenVar xNodeVar;
  
  i8 iPort;
  
  
  if ( ! m_pDataStack->bPopPtr(pObject) )
    THROW_DSTACK();
  
  if ( ! m_pDataStack->bPopGenVar(xNodeVar) )
    THROW_DSTACK();
  
  if ( ! m_pDataStack->bPopInt(iPort) )
    THROW_DSTACK();
  
  // get address of node
  Stz* sAddr = xNodeVar.szStz();
  
  if (AM::bVerbose())
    vTrace("SOCKET ADDR: %s\n"_Stz,sAddr);
  
  // get pSocket
  pSocket = reinterpret_cast<Socket*>(pObject);
  
  
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
//void Step::vSocketConnect(void)
//{
//  SysCall oSysCall;
//  Socket* pSocket;
//  Object* pObject;
//  GenVar xNodeVar;
//
//  i8 iPort;
//
//  ENTRY(); // ENTRY("vSocketConnect"_Stz);
//
//  if ( ! m_pDataStack->bPopPtr(pObject) )
//    THROW_DSTACK();
//
//  if ( ! m_pDataStack->bPopGenVar(xNodeVar) )
//    THROW_DSTACK();
//
//  if ( ! m_pDataStack->bPopInt(iPort) )
//    THROW_DSTACK();
//
//  // get address of node
//  Stz* sAddr = xNodeVar.szStz();
//
//  if (AM::bVerbose())
//    vTrace("SOCKET ADDR: %s\n"_Stz,sAddr);
//
//  // get pSocket
//  pSocket = reinterpret_cast<Socket*>(pObject);
//
//  bool bResult = false;
//
//  // try connecting in a loop if timeout occurs
//  // possibly other node not back to accepting connections
//
//  i8 iTimedOutCount = 0;
//  i8 iConnRefusedCount = 0;
//
//  while (!bResult) {
//
//    bResult = pSocket->bConnectServer(sAddr, (int)iPort, oSysCall);
//
//    if ( ! bResult ) {
//      i8 iError = oSysCall.iError();
//
//      if (iError == ETIMEDOUT) {
//        if (0 == iTimedOutCount++)
//          vTrace("Connect timedout... retrying...\n"_Stz);
//        continue;
//      }
//
//      if (iError == ECONNREFUSED) {
//        if (0 == iConnRefusedCount++)
//          vTrace("Connect refused... retrying...\n"_Stz);
//        continue;
//      }
//
//      vTrace("Unable to connect: Errno %d %s\n"_Stz,
//             iError, strerror((int)iError));
//      AM::vNap(1000, CN_SOCKET);
//      //vThrow(FK_CHECK,__func__,__FILE__,__LINE__,"Unable to connect: Errno %d"_Stz, iError);
//    }
//  }
//
//  // return the socket object
//  Object* pPtrSocket = pSocket;
//  m_pDataStack->vPushPointer(pPtrSocket);
//
//  EXIT();
//}

/*----------------------------------------------------------------------------*/
/* socket.dropWrite: drop socket write lock                                   */
/*----------------------------------------------------------------------------*/
void Step::vSocketDropWrite(void)
{
  ENTRY(); // ENTRY("vSocketDropWrite"_Stz);
  
  Object* pSocketPtr;
  if ( ! m_pDataStack->bPopPtr(pSocketPtr) )
    THROW_DSTACK();
  Socket* pSocket = _pSocket(pSocketPtr);
  
  pSocket->vDropWrite();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* socket.grabWrite: grab socket write lock                                   */
/*----------------------------------------------------------------------------*/
void Step::vSocketGrabWrite(void)
{
  ENTRY(); // ENTRY("vSocketGrabWrite"_Stz);
  
  Object* pSocketPtr;
  if ( ! m_pDataStack->bPopPtr(pSocketPtr) )
    THROW_DSTACK();
  Socket* pSocket = _pSocket(pSocketPtr);
  
  pSocket->vGrabWrite();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* socket::listen: listen in for socket connections                           */
/*----------------------------------------------------------------------------*/
void Step::vSocketListen(void)
{
  ENTRY(); // ENTRY("vSocketListen"_Stz);
  
  SysCall oSysCall;
  Socket* pSocket;
  i8      iPort;
  
  
  if ( ! m_pDataStack->bPopInt(iPort) )
    THROW_DSTACK();
  
  pSocket = new Socket(this);
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
  ENTRY(); // ENTRY("vSocketOpen"_Stz);
  
  SysCall oSysCall;
  Socket* pSocket;
  
  Object* pNodePtr;
  if ( ! m_pDataStack->bPopPtr(pNodePtr) )
    THROW_DSTACK();
  Node* pNode = _pNode(pNodePtr);

  pSocket = new Socket(pNodePtr);
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
  // ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! !
  // test for low memory condition
  
  bool bLowMem = gpBufferPool->bLowMemory();
  if (bLowMem) {
    vSocketRecvObject_LowMem();
    return;
  }
  
  // ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! !
  
  ENTRY(); // ENTRY("vSocketRecvObject"_Stz);
  
  Object* pSocketPtr;
  if ( ! m_pDataStack->bPopPtr(pSocketPtr) )
    THROW_DSTACK();
  Socket* pSocket = _pSocket(pSocketPtr);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // receiver the object header
  
  char caObjectHeader[sizeof(Object) + C_1K];
  
  Header* aHeader = (Header*) & caObjectHeader[0];
  
  i8 nObjectBytes = sizeof(Object);
  SysCall oSysCall;

  bool bResult = pSocket->bRecvBytes(aHeader,nObjectBytes,oSysCall);
  if (! bResult) {
    m_pDataStack->vPushInt(false);
    EXIT();
    return;
  }
  
  SP_Header spHeader;
  
  //vTrace("Socket received: Header %s"_Stz,
  //       spHeader.sz(aHeader));
  
  AM::vLogRes(SL_INF,pSocket,RA_SKT_RECEIVED,
              "Socket received: Header %s"_Stz, //!format-LogRes
              spHeader.sz(aHeader));
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the details about the received object
  
  Object* pObject = reinterpret_cast<Object*>(&caObjectHeader);
  eCN eClass = pObject->eClass();
  i8 nObjectSize = giClassSize[eClass];

  RASSERT(_bIsValidJob(eClass) ||
          _bIsValidOperand(eClass));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // receive rest of a job
  
  if ( _bIsValidJob(eClass) ) {
        
    //! temp. to be fixed
    Job* pJob = static_cast<Job*>(HeapMem::pAllocate(eClass));

    MemLib::vCopy(pJob,pObject,sizeof(Object),eClass);
    
    void* aBody = aAdd<void*>(pJob,sizeof(Object));
    i8 nRemainingBytes = nObjectSize - sizeof(Object);

    bResult = pSocket->bRecvBytes(aBody,nRemainingBytes,oSysCall);
    if (! bResult) {
      m_pDataStack->vPushInt(false);
      EXIT();
      return;
    }

    //vTrace("vSocketRecvObject: recv %p %lld %s %lld bytes\n"_Stz,
    //       pJob,eClass,aCN[eClass],nObjectSize);
    
    SP_Job spJob;
    AM::vLogRes(SL_INF,pSocket,RA_SKT_RECEIVED,
                "Socket received Job: %s"_Stz, //!format-LogRes
                 spJob.sz_recv(pJob));

    if (pJob->m_xReqTuple.m_iDetail) {
      JobLog::vNote(pJob,
                         "Recv: %s %p socket %llx"_Stz,
                         aCN[eClass],
                         pJob,
                         pSocket);
      pJob->vMarkJob();
    }
    
    //ActLog::vAdd(pJob,AN_RECV_JOB,m_pTask->pJob(),nObjectSize);

    pJob->m_i_Focal = C_UNKNOWN;
                         
    m_pDataStack->vPushPtr(pJob);
    m_pDataStack->vPushInt(true);
    EXIT();
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // receive rest of a row buffer
  
  void* aBody;
  i8 nRemainingBytes;
  
  switch(eClass) {
    
    case CN_CANCELMARKER: {
      CancelMarker* pCancelMarker = new CancelMarker();
      
      MemLib::vCopy(pCancelMarker,pObject,sizeof(Object),CN_CANCELMARKER);
      
      aBody = aAdd<void*>(pCancelMarker,sizeof(Object));
      nRemainingBytes = giClassSize[CN_CANCELMARKER] - sizeof(Object);
      
      bResult = pSocket->bRecvBytes(aBody,nRemainingBytes,oSysCall);
      if (! bResult) {
        m_pDataStack->vPushInt(false);
        EXIT();
        return;
      }
      
      //vTrace("vSocketRecvObject: recv %p %lld %s %lld bytes\n"_Stz,
      //       pEntBuffer,eClass,aCN[eClass],nObjectSize);
      
      m_pDataStack->vPushPtr(pCancelMarker);
      m_pDataStack->vPushInt(true);
      EXIT();
      return;
    }
      
    case CN_CRWBUFFER: {
      
      CrwBuffer* pCrwBuffer = new CrwBuffer();
      
      MemLib::vCopy(pCrwBuffer,pObject,sizeof(Object),CN_CRWBUFFER);
      
      aBody = aAdd<void*>(pCrwBuffer,sizeof(Object));
      nRemainingBytes = giClassSize[CN_CRWBUFFER] - sizeof(Object);
      
      bResult = pSocket->bRecvBytes(aBody,nRemainingBytes,oSysCall);
      if (! bResult) {
        m_pDataStack->vPushInt(false);
        EXIT();
        return;
      }
      
      //vTrace("vSocketRecvObject: recv %p %lld %s %lld bytes\n"_Stz,
      //       pEntBuffer,eClass,aCN[eClass],nObjectSize);
      
      // restore the nub of the crwbuffer
      pCrwBuffer->vRestoreNub();
      
      m_pDataStack->vPushPtr(pCrwBuffer);
      m_pDataStack->vPushInt(true);
      EXIT();
      return;
    }
      
    case CN_ENDMARKER: {
      EndMarker* pEndMarker = new EndMarker();
      
      MemLib::vCopy(pEndMarker,pObject,sizeof(Object),CN_ENDMARKER);
      
      aBody = aAdd<void*>(pEndMarker,sizeof(Object));
      nRemainingBytes = giClassSize[CN_ENDMARKER] - sizeof(Object);
      
      bResult = pSocket->bRecvBytes(aBody,nRemainingBytes,oSysCall);
      if (! bResult) {
        m_pDataStack->vPushInt(false);
        EXIT();
        return;
      }
      
      //vTrace("vSocketRecvObject: recv %p %lld %s %lld bytes\n"_Stz,
      //       pEntBuffer,eClass,aCN[eClass],nObjectSize);
      
      m_pDataStack->vPushPtr(pEndMarker);
      m_pDataStack->vPushInt(true);
      EXIT();
      return;
    }
    
    case CN_IRWBUFFER: {
      
      IrwBuffer* pIrwBuffer = new IrwBuffer();
      
      MemLib::vCopy(pIrwBuffer,pObject,sizeof(Object),CN_IRWBUFFER);
      
      aBody = aAdd<void*>(pIrwBuffer,sizeof(Object));
      nRemainingBytes = giClassSize[CN_IRWBUFFER] - sizeof(Object);
      
      bResult = pSocket->bRecvBytes(aBody,nRemainingBytes,oSysCall);
      if (! bResult) {
        m_pDataStack->vPushInt(false);
        EXIT();
        return;
      }
      
      //vTrace("vSocketRecvObject: recv %p %lld %s %lld bytes\n"_Stz,
      //       pEntBuffer,eClass,aCN[eClass],nObjectSize);
      
      // restore the nub of the crwbuffer
      //pIrwBuffer->vRestoreNub();
      
      m_pDataStack->vPushPtr(pIrwBuffer);
      m_pDataStack->vPushInt(true);
      EXIT();
      return;
    }
     
    case CN_CSVBUFFER: {
      
      CsvBuffer* pCsvBuffer = new CsvBuffer();
      
      MemLib::vCopy(pCsvBuffer,pObject,sizeof(Object),CN_CSVBUFFER);
      
      aBody = aAdd<void*>(pCsvBuffer,sizeof(Object));
      nRemainingBytes = giClassSize[CN_CSVBUFFER] - sizeof(Object);
      
      bResult = pSocket->bRecvBytes(aBody,nRemainingBytes,oSysCall);
      if (! bResult) {
        m_pDataStack->vPushInt(false);
        EXIT();
        return;
      }
      
      //vTrace("vSocketRecvObject: recv %p %lld %s %lld bytes\n"_Stz,
      //       pEntBuffer,eClass,aCN[eClass],nObjectSize);
      
      // restore the nub of the crwbuffer
      //pIrwBuffer->vRestoreNub();
      
      m_pDataStack->vPushPtr(pCsvBuffer);
      m_pDataStack->vPushInt(true);
      EXIT();
      return;
    }
      
    case CN_STREAMDEF: {
      StreamDef* pStreamDef = new StreamDef();
      
      MemLib::vCopy(pStreamDef,pObject,sizeof(Object),CN_STREAMDEF);
      
      aBody = aAdd<void*>(pStreamDef,sizeof(Object));
      nRemainingBytes = giClassSize[CN_STREAMDEF] - sizeof(Object);
      
      bResult = pSocket->bRecvBytes(aBody,nRemainingBytes,oSysCall);
      if (! bResult) {
        m_pDataStack->vPushInt(false);
        EXIT();
        return;
      }
      
      //vTrace("vSocketRecvObject: recv %p %lld %s %lld bytes\n"_Stz,
      //       pEntBuffer,eClass,aCN[eClass],nObjectSize);
      
      m_pDataStack->vPushPtr(pStreamDef);
      m_pDataStack->vPushInt(true);
      EXIT();
      return;
    }
      
    default:
      RASSERT(0);
  }
  
  
  //////////////////////////////////////////////////////////////////////////////
  

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // unknown object
  
  vTrace("socket.recvObject: unknown object; class %lld\n"_Stz,eClass);
  m_pDataStack->vPushInt(false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* socket.recvObject_LowMem: recv object from a socket (under low mem cond)   */
/*----------------------------------------------------------------------------*/
void Step::vSocketRecvObject_LowMem(void)
{
  ENTRY(); // ENTRY("vSocketRecvObject_LowMem"_Stz);
  
  i8 nFreeBuffer = gpBufferPool->nFreeCount();
  
  bool bDiscardOperand = false;
  
  Object* pSocketPtr;
  if ( ! m_pDataStack->bPopPtr(pSocketPtr) )
    THROW_DSTACK();
  Socket* pSocket = _pSocket(pSocketPtr);
  
  void* aLowBuff = pSocket->m_aLowBuff;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // receiver the object header
  
  char caObjectHeader[sizeof(Object)+C_1K];
  i8 nObjectBytes = sizeof(Object);
  SysCall oSysCall;
  
  bool bResult = pSocket->bRecvBytes(caObjectHeader,nObjectBytes, oSysCall);
  if (! bResult) {
    m_pDataStack->vPushInt(false);
    EXIT();
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the details about the received object
  
  Object* pObject = reinterpret_cast<Object*>(&caObjectHeader);
  eCN eClass = pObject->eClass();
  i8 nObjectSize = giClassSize[eClass];
  
  RASSERT(_bIsValidJob(eClass) || _bIsValidOperand(eClass));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // handle the specific case of job_writeBuffer under low memory condition
  
  if (CN_JOB_WRITEBUFFER == eClass) {

    vTrace("Received Job_WriteBuffer under low mem\n"_Stz);
    
    MemLib::vCopy(aLowBuff,pObject,sizeof(Object),eClass);
    
    void* aLowBody = aAdd<void*>(aLowBuff,sizeof(Object));
    i8 nRemainingBytes = sizeof(Job_WriteBuffer) - sizeof(Object);
    
    bResult = pSocket->bRecvBytes(aLowBody,nRemainingBytes,oSysCall);
    if (! bResult) {
      m_pDataStack->vPushInt(false);
      EXIT();
      return;
    }
    
    Job_WriteBuffer* pJob_WriteBuffer =
      static_cast<Job_WriteBuffer*>(aLowBuff);

    pJob_WriteBuffer->m_i_Focal = C_UNKNOWN;

    bDiscardOperand = pJob_WriteBuffer->m_pOperand ? true : false;

    pJob_WriteBuffer->m_bLowMem = true;
    
    Job* pJob = static_cast<Job*>(HeapMem::pAllocate(CN_JOB_WRITEBUFFER));
    MemLib::vCopy(pJob,pJob_WriteBuffer,sizeof(Job_WriteBuffer),CN_JOB_WRITEBUFFER);
    
    pJob_WriteBuffer = static_cast<Job_WriteBuffer*>(pJob);
    
    if (-1 != pJob_WriteBuffer->m_nWriteBehindBuffers) {
      
      pJob_WriteBuffer->m_pOperand = nullptr;
    }
    else {
      //pJob_WriteBuffer = nullptr;
      //vTrace("Discarded Job_WriteBuffer under low mem\n"_Stz);
      pJob_WriteBuffer->m_pOperand = nullptr;
    }

    if (bDiscardOperand) {

      bResult = pSocket->bRecvBytes(aLowBuff,sizeof(EntBuffer),oSysCall);
      vTrace("Discarded EntBuffer under low mem\n"_Stz);
    }
    
    m_pDataStack->vPushPtr(pJob_WriteBuffer);
    m_pDataStack->vPushInt(true);
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
      m_pDataStack->vPushInt(false);
      EXIT();
      return;
    }
    
    //vTrace("vSocketRecvObject: recv %p %lld %s %lld bytes\n"_Stz,
    //       pJob,eClass,aCN[eClass],nObjectSize);
    
    //ActLog::vAdd(pJob,AN_RECV_JOB,m_pTask->pJob(),nObjectSize);
    //ActLog::vAdd(pJob,AN_RECV_JOB_LOW_MEMORY);
    
    pJob->m_i_Focal = C_UNKNOWN;

    m_pDataStack->vPushPtr(pJob);
    m_pDataStack->vPushInt(true);
    EXIT();
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // receive rest of row buffer
  void* aBody;
  i8 nRemainingBytes;
  CrwBuffer* pCrwBuffer;
  EndMarker* pEndMarker;
  StreamDef* pStreamDef;
  
  switch(eClass) {
      
    case CN_CRWBUFFER:
    
      pCrwBuffer = new CrwBuffer();
      
      MemLib::vCopy(pCrwBuffer,pObject,sizeof(Object),CN_CRWBUFFER);
      
      aBody = aAdd<void*>(pCrwBuffer,sizeof(Object));
      nRemainingBytes = giClassSize[CN_CRWBUFFER] - sizeof(Object);
      
      bResult = pSocket->bRecvBytes(aBody,nRemainingBytes,oSysCall);
      if (! bResult) {
        m_pDataStack->vPushInt(false);
        EXIT();
        return;
      }
      
      //vTrace("vSocketRecvObject: recv %p %lld %s %lld bytes\n"_Stz,
      //       pEntBuffer,eClass,aCN[eClass],nObjectSize);
      
      // restore the nub of the crwbuffer
      pCrwBuffer->vRestoreNub();
      
      m_pDataStack->vPushPtr(pCrwBuffer);
      m_pDataStack->vPushInt(true);
      EXIT();
      return;
     
    case CN_ENDMARKER:
      pEndMarker = new EndMarker();
      
      MemLib::vCopy(pEndMarker,pObject,sizeof(Object),CN_ENDMARKER);
      
      aBody = aAdd<void*>(pEndMarker,sizeof(Object));
      nRemainingBytes = giClassSize[CN_ENDMARKER] - sizeof(Object);
      
      bResult = pSocket->bRecvBytes(aBody,nRemainingBytes,oSysCall);
      if (! bResult) {
        m_pDataStack->vPushInt(false);
        EXIT();
        return;
      }
      
      //vTrace("vSocketRecvObject: recv %p %lld %s %lld bytes\n"_Stz,
      //       pEntBuffer,eClass,aCN[eClass],nObjectSize);
      
      m_pDataStack->vPushPtr(pEndMarker);
      m_pDataStack->vPushInt(true);
      EXIT();
      return;
  
    case CN_IRWBUFFER: {
      
      IrwBuffer* pIrwBuffer = new IrwBuffer();
      
      MemLib::vCopy(pIrwBuffer,pObject,sizeof(Object),CN_IRWBUFFER);
      
      aBody = aAdd<void*>(pIrwBuffer,sizeof(Object));
      nRemainingBytes = giClassSize[CN_IRWBUFFER] - sizeof(Object);
      
      bResult = pSocket->bRecvBytes(aBody,nRemainingBytes,oSysCall);
      if (! bResult) {
        m_pDataStack->vPushInt(false);
        EXIT();
        return;
      }
      
      //vTrace("vSocketRecvObject: recv %p %lld %s %lld bytes\n"_Stz,
      //       pEntBuffer,eClass,aCN[eClass],nObjectSize);
      
      // restore the nub of the crwbuffer
      //pIrwBuffer->vRestoreNub();
      
      m_pDataStack->vPushPtr(pIrwBuffer);
      m_pDataStack->vPushInt(true);
      EXIT();
      return;
    }
      
    case CN_STREAMDEF:
      pStreamDef = new StreamDef();
      
      MemLib::vCopy(pStreamDef,pObject,sizeof(Object),CN_STREAMDEF);
      
      aBody = aAdd<void*>(pStreamDef,sizeof(Object));
      nRemainingBytes = giClassSize[CN_STREAMDEF] - sizeof(Object);
      
      bResult = pSocket->bRecvBytes(aBody,nRemainingBytes,oSysCall);
      if (! bResult) {
        m_pDataStack->vPushInt(false);
        EXIT();
        return;
      }
      
      //vTrace("vSocketRecvObject: recv %p %lld %s %lld bytes\n"_Stz,
      //       pEntBuffer,eClass,aCN[eClass],nObjectSize);
      
      m_pDataStack->vPushPtr(pStreamDef);
      m_pDataStack->vPushInt(true);
      EXIT();
      return;
      
    default:
      RASSERT(0);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // unknown object
  
  vTrace("socket.recvObject: unknown object; class %lld\n"_Stz,eClass);
  m_pDataStack->vPushInt(false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* socket.sendObject: send object via a socket                                */
/*----------------------------------------------------------------------------*/
void Step::vSocketSendObject(void)
{
  ENTRY(); // ENTRY("vSocketSendObject"_Stz);
  
  Object* pSocketPtr;
  if ( ! m_pDataStack->bPopPtr(pSocketPtr) )
    THROW_DSTACK();
  Socket* pSocket = _pSocket(pSocketPtr);
  
  Object* pObject;
  if ( ! m_pDataStack->bPopPtr(pObject) )
  THROW_DSTACK();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  eCN eClass = pObject->eClass();
  i8 nObjectSize = giClassSize[eClass];
  
  //vTrace("vSocketSendObject: send %p %lld %s %lld bytes\n"_Stz,
  //       pObject,eClass,aCN[eClass],nObjectSize);
  
  
  
  if (_bIsValidJob(eClass)) {
    Job* pJob = _pJob(pObject);
    
    // ray added 25 Aug 2019
    ASSERT(pJob->m_aJobClass->m_fpSend && pJob->m_aJobClass->m_fpRecv);
    
    if (pJob->m_xReqTuple.m_iDetail)
      JobLog::vNote(pJob,"Send: %s %p socket %llx"_Stz,
                         aCN[eClass],
                         pJob,
                         pSocket);
    
    //ActLog::vAdd(pJob,AN_SEND_JOB,m_pTask->pJob(),nObjectSize);
  }

  RASSERT(_bIsValidJob(eClass) ||
          _bIsValidOperand(eClass));
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
//  if (eClass == CN_CRWBUFFER) {
//    CrwBuffer* pCrwBuffer = _pCrwBuffer(pObject);
//    pCrwBuffer->vShow();
//    vDebugPoint();
//  }
  AM::vLogRes(SL_INF,pSocket,RA_SKT_SEND,
              "Object %s %lldB to node %s"_Stz, //!format-LogRes
              aCN[eClass],
              nObjectSize,
              m_pRegSet->m_pSendNode ?
                m_pRegSet->m_pSendNode->szNodeNm() : "<unknown>"_Stz,
              C_ENDARGS);

    
  SysCall oSysCall;
  
  bool bResult = pSocket->bSendTo(pObject, nObjectSize, oSysCall);
  
  if (bResult)
    AM::vLogRes(SL_INF,pSocket,RA_SKT_SENT,
                "Object %s %lldB"_Stz, //!format-LogRes
                aCN[eClass],
                nObjectSize,
                C_ENDARGS);
  else
    AM::vLogRes(SL_INF,pSocket,RA_SKT_ERROR,
                "Error while sending object %s %lldB"_Stz, //!format-LogRes
                aCN[eClass],
                nObjectSize,
                C_ENDARGS);

  m_pDataStack->vPushInt(bResult);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
