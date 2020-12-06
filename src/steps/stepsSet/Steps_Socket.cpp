/*============================================================================*/
/* Steps_Socket.cpp - socket step group                                       */
/*============================================================================*/

#include "Steps.h"

extern SizeHeap  gxaSizeHeap[];

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
  Datum xNodeVar;
  
  
  Object* pObject = m_pDataStack->pPopPtr(__func__);
  // get pSocket
  pSocket = reinterpret_cast<Socket*>(pObject);

  m_pDataStack->vPopVal(xNodeVar,__func__);
  
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
  // ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! !
  // test for low memory condition
  
  bool bLowMem = gpBufferPool->bLowMemory();
  if (bLowMem) {
    //AM::vPoolAnalyze();
    vSocketRecvObject_LowMem();
    return;
  }
  
  // ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! !
  
  ENTRY();
  
  Socket* pSocket = _pSocket(m_pDataStack->pPopPtr(__func__));
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // receive the object header
  
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
  i8 nObjectSize = gxaSizeHeap[eClass].m_nClassSize;

  RASSERT(_bIsValidJob(eClass) ||
          _bIsValidOperand(eClass) ||
          eClass == CN_XMITOPERAND);

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
    pJob->vOriginResponseVec(nullptr);
    pJob->vBufferQueue(nullptr);
    
    m_pDataStack->vPushPtr(pJob);
    m_pDataStack->vPushInt(true);
    EXIT();
    return;
  }
  
  if (eClass == CN_XMITOPERAND) {
    XmitOperand* pXmitOperand = static_cast<XmitOperand*>(HeapMem::pAllocate(eClass));
    
    MemLib::vCopy(pXmitOperand,pObject,sizeof(Object),eClass);
    
    void* aBody = aAdd<void*>(pXmitOperand,sizeof(Object));
    i8 nRemainingBytes = nObjectSize - sizeof(Object);
    
    bResult = pSocket->bRecvBytes(aBody,nRemainingBytes,oSysCall);
    if (! bResult) {
      m_pDataStack->vPushInt(false);
      EXIT();
      return;
    }
    
    //vTrace("vSocketRecvObject: recv %p %lld %s %lld bytes\n"_Stz,
    //       pJob,eClass,aCN[eClass],nObjectSize);
    
//    SP_Job spJob;
//    AM::vLogRes(SL_INF,pSocket,RA_SKT_RECEIVED,
//                "Socket received XmitOperand: %s"_Stz, //!format-LogRes
//                spJob.sz_recv(pXmitOperand));
    
    m_pDataStack->vPushPtr(pXmitOperand);
    m_pDataStack->vPushInt(true);
    EXIT();
    return;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // receive an operand
  
  void* aBody;
  i8 nRemainingBytes;
  
  switch(eClass) {
      
    case CN_CRWBUFFER: {
      
      CrwBuffer* pCrwBuffer = new CrwBuffer;
      
      MemLib::vCopy(pCrwBuffer,pObject,sizeof(Object),CN_CRWBUFFER);
      
      aBody = aAdd<void*>(pCrwBuffer,sizeof(Object));
      nRemainingBytes = gxaSizeHeap[CN_CRWBUFFER].m_nClassSize - sizeof(Object);
      
      bResult = pSocket->bRecvBytes(aBody,nRemainingBytes,oSysCall);
      if (! bResult) {
        m_pDataStack->vPushInt(false);
        EXIT();
        return;
      }
      pCrwBuffer->vIsPermanent(false);
      
      //vTrace("vSocketRecvObject: recv %p %lld %s %lld bytes\n"_Stz,
      //       pEntBuffer,eClass,aCN[eClass],nObjectSize);
      
      // restore the nub of the crwbuffer
      pCrwBuffer->vRestoreNub();
      
      m_pDataStack->vPushPtr(pCrwBuffer);
      m_pDataStack->vPushInt(true);
      EXIT();
      return;
    }
     
    case CN_CSVBUFFER: {
      
      CsvBuffer* pCsvBuffer = new CsvBuffer();
      
      MemLib::vCopy(pCsvBuffer,pObject,sizeof(Object),CN_CSVBUFFER);
      
      aBody = aAdd<void*>(pCsvBuffer,sizeof(Object));
      nRemainingBytes = gxaSizeHeap[CN_CSVBUFFER].m_nClassSize - sizeof(Object);
      
      bResult = pSocket->bRecvBytes(aBody,nRemainingBytes,oSysCall);
      if (! bResult) {
        m_pDataStack->vPushInt(false);
        EXIT();
        return;
      }
      
      //vTrace("vSocketRecvObject: recv %p %lld %s %lld bytes\n"_Stz,
      //       pEntBuffer,eClass,aCN[eClass],nObjectSize);
            
      m_pDataStack->vPushPtr(pCsvBuffer);
      m_pDataStack->vPushInt(true);
      EXIT();
      return;
    }
     
    case CN_INDEXDEF: {
      IndexDef* pIndexDef = new IndexDef();
      
      MemLib::vCopy(pIndexDef,pObject,sizeof(Object),CN_INDEXDEF);
      
      aBody = aAdd<void*>(pIndexDef,sizeof(Object));
      nRemainingBytes = gxaSizeHeap[CN_INDEXDEF].m_nClassSize - sizeof(Object);
      
      bResult = pSocket->bRecvBytes(aBody,nRemainingBytes,oSysCall);
      if (! bResult) {
        m_pDataStack->vPushInt(false);
        EXIT();
        return;
      }
      
      //vTrace("vSocketRecvObject: recv %p %lld %s %lld bytes\n"_Stz,
      //       pEntBuffer,eClass,aCN[eClass],nObjectSize);
      
      m_pDataStack->vPushPtr(pIndexDef);
      m_pDataStack->vPushInt(true);
      EXIT();
      return;
    }
      
    case CN_INFOSET: {
      InfoSet* pInfoSet = new InfoSet;
      
      MemLib::vCopy(pInfoSet,pObject,sizeof(Object),CN_INFOSET);
      
      aBody = aAdd<void*>(pInfoSet,sizeof(Object));
      nRemainingBytes = gxaSizeHeap[CN_INFOSET].m_nClassSize - sizeof(Object);
      
      bResult = pSocket->bRecvBytes(aBody,nRemainingBytes,oSysCall);
      if (! bResult) {
        m_pDataStack->vPushInt(false);
        EXIT();
        return;
      }
      
      m_pDataStack->vPushPtr(pInfoSet);
      m_pDataStack->vPushInt(true);
      EXIT();
      return;
    }
      
    case CN_ROWBUFFER: {
      RowBuffer* pRowBuffer = new RowBuffer;
      
      MemLib::vCopy(pRowBuffer,pObject,sizeof(Object),CN_ROWBUFFER);
      
      aBody = aAdd<void*>(pRowBuffer,sizeof(Object));
      nRemainingBytes = gxaSizeHeap[CN_ROWBUFFER].m_nClassSize - sizeof(Object);
      
      bResult = pSocket->bRecvBytes(aBody,nRemainingBytes,oSysCall);
      if (! bResult) {
        m_pDataStack->vPushInt(false);
        EXIT();
        return;
      }
      
      m_pDataStack->vPushPtr(pRowBuffer);
      m_pDataStack->vPushInt(true);
      EXIT();
      return;
    }
      
    case CN_STREAMSET: {
      StreamSet* pStreamSet = new StreamSet(pSocket);
      
      MemLib::vCopy(pStreamSet,pObject,sizeof(Object),CN_STREAMSET);
      
      aBody = aAdd<void*>(pStreamSet,sizeof(Object));
      nRemainingBytes = gxaSizeHeap[CN_STREAMSET].m_nClassSize - sizeof(Object);
      
      bResult = pSocket->bRecvBytes(aBody,nRemainingBytes,oSysCall);
      if (! bResult) {
        m_pDataStack->vPushInt(false);
        EXIT();
        return;
      }
      
      m_pDataStack->vPushPtr(pStreamSet);
      m_pDataStack->vPushInt(true);
      EXIT();
      return;
    }
      
    case CN_MARKER: {
      Marker* pMarker = new Marker();
      
      MemLib::vCopy(pMarker,pObject,sizeof(Object),CN_MARKER);
      
      aBody = aAdd<void*>(pMarker,sizeof(Object));
      nRemainingBytes = gxaSizeHeap[CN_MARKER].m_nClassSize - sizeof(Object);
      
      bResult = pSocket->bRecvBytes(aBody,nRemainingBytes,oSysCall);
      if (! bResult) {
        m_pDataStack->vPushInt(false);
        EXIT();
        return;
      }
      
      m_pDataStack->vPushPtr(pMarker);
      m_pDataStack->vPushInt(true);
      EXIT();
      return;
    }
     
    case CN_PROFILE: {
      Profile* pProfile = new Profile(""_Stz);
      
      MemLib::vCopy(pProfile,pObject,sizeof(Object),CN_PROFILE);
      
      aBody = aAdd<void*>(pProfile,sizeof(Object));
      nRemainingBytes = gxaSizeHeap[CN_PROFILE].m_nClassSize - sizeof(Object);
      
      bResult = pSocket->bRecvBytes(aBody,nRemainingBytes,oSysCall);
      if (! bResult) {
        m_pDataStack->vPushInt(false);
        EXIT();
        return;
      }
      
      m_pDataStack->vPushPtr(pProfile);
      m_pDataStack->vPushInt(true);
      EXIT();
      return;
    }
      
    case CN_STREAMDEF: {
      StreamDef* pStreamDef = new StreamDef();
      
      MemLib::vCopy(pStreamDef,pObject,sizeof(Object),CN_STREAMDEF);
      
      aBody = aAdd<void*>(pStreamDef,sizeof(Object));
      nRemainingBytes = gxaSizeHeap[CN_STREAMDEF].m_nClassSize - sizeof(Object);
      
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
      vTrace("socket.recvObject: unknown object; class %lld\n"_Stz,eClass);
      RASSERT(0);
  }
  
  //////////////////////////////////////////////////////////////////////////////
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* socket.recvObject_LowMem: recv object from a socket (under low mem cond)   */
/*----------------------------------------------------------------------------*/
void Step::vSocketRecvObject_LowMem(void)
{
  ENTRY();
  
  Socket* pSocket = _pSocket(m_pDataStack->pPopPtr(__func__));
  
  void* aLowBuff = pSocket->m_aLowBuff;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // receive the object header
  
  char caObjectHeader[sizeof(Object)+C_1K];
  Header* aHeader = (Header*) & caObjectHeader[0];
  i8 nObjectBytes = sizeof(Object);
  SysCall oSysCall;
  
  bool bResult = pSocket->bRecvBytes(aHeader,nObjectBytes, oSysCall);
  if (! bResult) {
    m_pDataStack->vPushInt(false);
    EXIT();
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the details about the received object
  
  Object* pObject = reinterpret_cast<Object*>(&caObjectHeader);
  eCN eClass = pObject->eClass();
  i8 nObjectSize = gxaSizeHeap[eClass].m_nClassSize;
  
  RASSERT(_bIsValidJob(eClass) ||
          _bIsValidOperand(eClass) ||
          eClass == CN_XMITOPERAND);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // receive rest of job
  
  if ( _bIsValidJob(eClass) ) {
    
    //! temp. to be fixed
    Job* pJob = static_cast<Job*>(HeapMem::pAllocate(eClass));
    
    MemLib::vCopy(pJob,pObject,sizeof(Object),eClass);
    
    void* aBody = aAdd<void*>(pJob,sizeof(Object));
    i8 nRemainingBytes = gxaSizeHeap[eClass].m_nClassSize - sizeof(Object);
    
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
    pJob->vOriginResponseVec(nullptr);
    pJob->vBufferQueue(nullptr);
    
    m_pDataStack->vPushPtr(pJob);
    m_pDataStack->vPushInt(true);
    EXIT();
    return;
  }
  
  if (eClass == CN_XMITOPERAND) {
    
    //vTrace("Received XmitOperand under low mem\n"_Stz);
    
    XmitOperand* pXmitOperand = static_cast<XmitOperand*>(HeapMem::pAllocate(eClass));
    
    MemLib::vCopy(pXmitOperand,pObject,sizeof(Object),eClass);
    
    void* aBody = aAdd<void*>(pXmitOperand,sizeof(Object));
    i8 nRemainingBytes = nObjectSize - sizeof(Object);
    
    bResult = pSocket->bRecvBytes(aBody,nRemainingBytes,oSysCall);
    if (! bResult) {
      m_pDataStack->vPushInt(false);
      EXIT();
      return;
    }
    
    m_pDataStack->vPushPtr(pXmitOperand);
    m_pDataStack->vPushInt(true);
    EXIT();
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // receive an operand
  
  void* aBody;
  i8 nRemainingBytes;
  
  switch(eClass) {
      
    case CN_CRWBUFFER: {
      
      vTrace("Received CRWBUFFER under low mem\n"_Stz);
      
      // copy the object header into the LowBuffer
      MemLib::vCopy(aLowBuff,pObject,sizeof(Object),CN_CRWBUFFER);
      
      // recv rest of the buffer into LowBuffer
      void* aLowBody = aAdd<void*>(aLowBuff,sizeof(Object));
      nRemainingBytes = gxaSizeHeap[CN_CRWBUFFER].m_nClassSize - sizeof(Object);
      
      bResult = pSocket->bRecvBytes(aLowBody,nRemainingBytes,oSysCall);
      
      // whether able to recv or not, return false
      m_pDataStack->vPushInt(false);
      EXIT();
      return;
    }
      
    case CN_CSVBUFFER: {
      
      vTrace("Received CSVBUFFER under low mem\n"_Stz);
      
      // copy the object header into the LowBuffer
      MemLib::vCopy(aLowBuff,pObject,sizeof(Object),CN_CSVBUFFER);
      
      // recv rest of the buffer into LowBuffer
      void* aLowBody = aAdd<void*>(aLowBuff,sizeof(Object));
      nRemainingBytes = gxaSizeHeap[CN_CSVBUFFER].m_nClassSize - sizeof(Object);
      
      bResult = pSocket->bRecvBytes(aLowBody,nRemainingBytes,oSysCall);
      if (! bResult) {
        m_pDataStack->vPushInt(false);
        EXIT();
        return;
      }
      
      m_pDataStack->vPushInt(false);
      EXIT();
      return;
    }
      
    case CN_INDEXDEF: {
      IndexDef* pIndexDef = new IndexDef();
      
      MemLib::vCopy(pIndexDef,pObject,sizeof(Object),CN_INDEXDEF);
      
      aBody = aAdd<void*>(pIndexDef,sizeof(Object));
      nRemainingBytes = gxaSizeHeap[CN_INDEXDEF].m_nClassSize - sizeof(Object);
      
      bResult = pSocket->bRecvBytes(aBody,nRemainingBytes,oSysCall);
      if (! bResult) {
        m_pDataStack->vPushInt(false);
        EXIT();
        return;
      }
      
      //vTrace("vSocketRecvObject: recv %p %lld %s %lld bytes\n"_Stz,
      //       pEntBuffer,eClass,aCN[eClass],nObjectSize);
      
      m_pDataStack->vPushPtr(pIndexDef);
      m_pDataStack->vPushInt(true);
      EXIT();
      return;
    }
      
    case CN_INFOSET: {
      
      InfoSet* pInfoSet = new InfoSet;
      
      MemLib::vCopy(pInfoSet,pObject,sizeof(Object),CN_INFOSET);
      
      aBody = aAdd<void*>(pInfoSet,sizeof(Object));
      nRemainingBytes = gxaSizeHeap[CN_INFOSET].m_nClassSize - sizeof(Object);
      
      bResult = pSocket->bRecvBytes(aBody,nRemainingBytes,oSysCall);
      if (! bResult) {
        m_pDataStack->vPushInt(false);
        EXIT();
        return;
      }
      
      m_pDataStack->vPushPtr(pInfoSet);
      m_pDataStack->vPushInt(true);
      EXIT();
      return;
    }
      
    case CN_ROWBUFFER: {
      
      vTrace("Received RSBBUFFER under low mem\n"_Stz);
      
      // copy the object header into the LowBuffer
      MemLib::vCopy(aLowBuff,pObject,sizeof(Object),CN_ROWBUFFER);
      
      // recv rest of the buffer into LowBuffer
      void* aLowBody = aAdd<void*>(aLowBuff,sizeof(Object));
      nRemainingBytes = gxaSizeHeap[CN_ROWBUFFER].m_nClassSize - sizeof(Object);
      
      bResult = pSocket->bRecvBytes(aLowBody,nRemainingBytes,oSysCall);

      m_pDataStack->vPushInt(false);
      EXIT();
      return;
    }
      
    case CN_STREAMSET: {
      StreamSet* pStreamSet = new StreamSet(pSocket);
      
      MemLib::vCopy(pStreamSet,pObject,sizeof(Object),CN_STREAMSET);
      
      aBody = aAdd<void*>(pStreamSet,sizeof(Object));
      nRemainingBytes = gxaSizeHeap[CN_STREAMSET].m_nClassSize - sizeof(Object);
      
      bResult = pSocket->bRecvBytes(aBody,nRemainingBytes,oSysCall);
      if (! bResult) {
        m_pDataStack->vPushInt(false);
        EXIT();
        return;
      }
      
      m_pDataStack->vPushPtr(pStreamSet);
      m_pDataStack->vPushInt(true);
      EXIT();
      return;
    }
      
    case CN_MARKER: {
      Marker* pMarker = new Marker();
      
      MemLib::vCopy(pMarker,pObject,sizeof(Object),CN_MARKER);
      
      aBody = aAdd<void*>(pMarker,sizeof(Object));
      nRemainingBytes = gxaSizeHeap[CN_MARKER].m_nClassSize - sizeof(Object);
      
      bResult = pSocket->bRecvBytes(aBody,nRemainingBytes,oSysCall);
      if (! bResult) {
        m_pDataStack->vPushInt(false);
        EXIT();
        return;
      }
      
      m_pDataStack->vPushPtr(pMarker);
      m_pDataStack->vPushInt(true);
      EXIT();
      return;
    }
      
    case CN_PROFILE: {
      vTrace("Received RSBBUFFER under low mem\n"_Stz);
      
      // copy the object header into the LowBuffer
      MemLib::vCopy(aLowBuff,pObject,sizeof(Object),CN_PROFILE);
      
      void* aLowBody = aAdd<void*>(aLowBuff,sizeof(Object));
      nRemainingBytes = gxaSizeHeap[CN_PROFILE].m_nClassSize - sizeof(Object);
      
      // recv rest of the object into LowBuffer
      bResult = pSocket->bRecvBytes(aLowBody,nRemainingBytes,oSysCall);
     
      m_pDataStack->vPushInt(false);
      EXIT();
      return;
    }
      
    case CN_STREAMDEF: {
      StreamDef* pStreamDef = new StreamDef();
      
      MemLib::vCopy(pStreamDef,pObject,sizeof(Object),CN_STREAMDEF);
      
      aBody = aAdd<void*>(pStreamDef,sizeof(Object));
      nRemainingBytes = gxaSizeHeap[CN_STREAMDEF].m_nClassSize - sizeof(Object);
      
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
      vTrace("socket.recvObject: unknown object; class %lld\n"_Stz,eClass);
      RASSERT(0);
  }
  
  //////////////////////////////////////////////////////////////////////////////
  
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
  
  eCN eClass = pObject->eClass();
  i8 nObjectSize = gxaSizeHeap[eClass].m_nClassSize;
  
  //vTrace("vSocketSendObject: send %p %lld %s %lld bytes\n"_Stz,
  //       pObject,eClass,aCN[eClass],nObjectSize);
  
  
  
  if (_bIsValidJob(eClass)) {
    Job* pJob = _pJob(pObject);
    
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
              m_pLogicUnit->m_pSendNode ?
                m_pLogicUnit->m_pSendNode->szNodeNm() : "<unknown>"_Stz,
              C_VARGS_END);

    
  SysCall oSysCall;
  
  bool bResult = pSocket->bSendTo(pObject, nObjectSize, oSysCall);
  
  if (bResult)
    AM::vLogRes(SL_INF,pSocket,RA_SKT_SENT,
                "Object %s %lldB"_Stz, //!format-LogRes
                aCN[eClass],
                nObjectSize,
                C_VARGS_END);
  else
    AM::vLogRes(SL_INF,pSocket,RA_SKT_ERROR,
                "Error while sending object %s %lldB"_Stz, //!format-LogRes
                aCN[eClass],
                nObjectSize,
                C_VARGS_END);

  m_pDataStack->vPushInt(bResult);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
