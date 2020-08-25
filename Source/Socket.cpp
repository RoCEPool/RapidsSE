/*============================================================================*/
/* socket.cpp - tcp/ip sockets                                                */
/*============================================================================*/

#include "Resources.h"


Atomic gixSocket;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Socket* Resources::_pSocket(void* aAddr)
{
  Socket* pSocket = reinterpret_cast<Socket*>(aAddr);
  
  RASSERT_TYPE(pSocket,CN_SOCKET);
  
  return pSocket;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Socket::operator new(size_t)
{
  void* pSocket = HeapMem::pAllocate(CN_SOCKET);
  
  return pSocket;
}

/*----------------------------------------------------------------------------*/
/* new operator                                                               */
/*----------------------------------------------------------------------------*/
void* Socket::operator new(size_t, Socket* pSocket)
{
  return pSocket;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void Socket::operator delete(void*)
{
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Socket::Socket(Object* pIsFor)
: Resource(CN_SOCKET)
, m_okLock(LK_SOCKET,this)
{
  _precondition_(_MAGIC_,CN_SOCKET,_FOOTER_);
  
  m_pIsFor    = pIsFor;
  m_iSocketFd = C_UNKNOWN;
  m_bBlocking = true;
  
  m_aLowBuff  = MemLib::aMallocBlock(C_SIZE_POOL_BUFFER);
  
  MemLib::vZero(m__Padding,sizeof(m__Padding),CN_SOCKET);
  
  vAddrInit(&m_xServAddr,nullptr,0);
  
  _checkInitialised_(CN_SOCKET);
  
  ++ gixSocket;
  AM::vLogRes(SL_INF,this,RA_SKT_CONSTRUCT,
              "Socket construct |gixSocket=%lld|"_Stz, //!format-LogRes
              * gixSocket);

  _postcondition_(_MAGIC_,CN_SOCKET,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Socket::Socket(Object* pIsFor, int iSocketId)
: Resource(CN_SOCKET)
, m_okLock(LK_SOCKET,this)
{
  _precondition_(_MAGIC_,CN_SOCKET,_FOOTER_);
  
  m_pIsFor    = pIsFor;
  m_iSocketFd = iSocketId;
  m_bBlocking = true;
  
  m_aLowBuff  = MemLib::aMallocBlock(C_SIZE_POOL_BUFFER);
  
  MemLib::vZero(m__Padding,sizeof(m__Padding),CN_SOCKET);
  
  vAddrInit(&m_xServAddr,nullptr,0);
  
  _checkInitialised_(CN_SOCKET);
  
  ++ gixSocket;
  AM::vLogRes(SL_INF,this,RA_SKT_CONSTRUCT,
              "Socket construct |gixSocket=%lld|"_Stz, //!format-LogRes
              * gixSocket);
  
  _postcondition_(_MAGIC_,CN_SOCKET,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Socket::~Socket(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_SOCKET,_FOOTER_);
  
  if (C_UNKNOWN != m_iSocketFd) {
    vWarn("Socket %d not closed!\n"_Stz, m_iSocketFd);
  }
  
  MemLib::vFreeBlock(m_aLowBuff,C_SIZE_POOL_BUFFER);
  
  -- gixSocket;
  AM::vLogRes(SL_INF,this,RA_SKT_DESTRUCT,
              "Socket destruct : %p |gixSocket=%lld|"_Stz, //!format-LogRes
              this,* gixSocket);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Socket::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Socket* pSocket = _pSocket(pObject);

  vTrace("id %lld %s\n"_Stz,
         pSocket->m_iSocketFd,
         pSocket->m_bBlocking ? "blocking"_Stz : "non-blocking"_Stz);
}

/*----------------------------------------------------------------------------*/
/* monitor profile                                                            */
/*----------------------------------------------------------------------------*/
void Socket::vMonitorPortrait(s48K& sPortrait)
{
  Statistics* pStat = AM::pStatistics();
  
  s32 sRecvMag; MagLib::vByteMag(pStat->Socket.m_nRecvBytes, sRecvMag);
  s32 sSendMag; MagLib::vByteMag(pStat->Socket.m_nSendBytes, sSendMag);

  sPortrait.vSprint("Socket: "
                    "accepts %lld "
                    "closes %lld "
                    "connects %lld "
                    "listens %lld "
                    "peeks %lld\n"
                    "        "
                    "recvs %lld+"
                    "recvFroms %lld (%s) "
                    "sends %lld+"
                    "sendTos %lld (%s)"
                    "\n"_Stz,
                    
                    pStat->Socket.m_nAccepts,
                    pStat->Socket.m_nCloses,
                    pStat->Socket.m_nConnects,
                    pStat->Socket.m_nListens,
                    pStat->Socket.m_nPeeks,
                    
                    pStat->Socket.m_nRecvs,
                    pStat->Socket.m_nRecvFroms,
                      sRecvMag.szStz(),
                    pStat->Socket.m_nSends,
                    pStat->Socket.m_nSendTos,
                      sSendMag.szStz()
                    );
}

/*----------------------------------------------------------------------------*/
/* statistics                                                                 */
/*----------------------------------------------------------------------------*/
void Socket::vStatistics(void)
{
  s32 sByteMag;
  
  Statistics* pStat = AM::pStatistics();

  vTrace("  nSocketAccepts    %lld\n"_Stz,pStat->Socket.m_nAccepts);
  vTrace("  nSocketCloses     %lld\n"_Stz,pStat->Socket.m_nCloses);
  vTrace("  nSocketConnects   %lld\n"_Stz,pStat->Socket.m_nConnects);
  vTrace("  nSocketListens    %lld\n"_Stz,pStat->Socket.m_nListens);
  vTrace("  nSocketPeeks      %lld\n"_Stz,pStat->Socket.m_nPeeks);
  vTrace("  nSocketRecvs      %lld\n"_Stz,pStat->Socket.m_nRecvs);
  vTrace("  nSocketRecvFroms  %lld\n"_Stz,pStat->Socket.m_nRecvFroms);
  vTrace("  nSocketSends      %lld\n"_Stz,pStat->Socket.m_nSends);
  vTrace("  nSocketSendTos    %lld\n"_Stz,pStat->Socket.m_nSendTos);

  MagLib::vByteMag(pStat->Socket.m_nRecvBytes,  sByteMag);
  vTrace("  nSocketRecvBytes  %s\n"_Stz,sByteMag.szStz());
  MagLib::vByteMag(pStat->Socket.m_nSendBytes,  sByteMag);
  vTrace("  nSocketSendBytes  %s\n"_Stz,sByteMag.szStz());
}

///////////////////////////////////////////////////////////////////////////////
//                              tcp/ip support                               //
///////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* initialize a sockaddr_in structure                                         */
/*----------------------------------------------------------------------------*/
void Socket::vAddrInit(sockaddr_in* aAddr, int iPort)
{
  _precondition_(_MAGIC_,CN_SOCKET,_FOOTER_);
  
  MemLib::vZero(aAddr,sizeof(sockaddr_in),CN_SOCKET);
  
  aAddr->sin_family      = AF_INET;
  aAddr->sin_addr.s_addr = htonl(INADDR_ANY);
  aAddr->sin_port        = htons((int)iPort);
}

/*----------------------------------------------------------------------------*/
/* initialize a sockaddr_in structure                                         */
/*----------------------------------------------------------------------------*/
void Socket::vAddrInit(sockaddr_in* aAddr, Stz* sAddr, int iPort)
{
  _precondition_(_MAGIC_,CN_SOCKET,_FOOTER_);
  
  MemLib::vZero(aAddr,sizeof(sockaddr_in),CN_SOCKET);
  
  aAddr->sin_family      = AF_INET;
  aAddr->sin_addr.s_addr = sAddr->azChar() == nullptr ? C_UNKNOWN : inet_addr(sAddr->azChar());
  aAddr->sin_port        = htons((int)iPort);
}

/*----------------------------------------------------------------------------*/
/* copy a socket                                                              */
/*----------------------------------------------------------------------------*/
void Socket::vCopy(Socket* pSource)
{
  _precondition_(_MAGIC_,CN_SOCKET,_FOOTER_);
  
  m_iSocketFd = pSource->m_iSocketFd;
  m_bBlocking = pSource->m_bBlocking;
}

/*----------------------------------------------------------------------------*/
/* focal outline                                                              */
/*----------------------------------------------------------------------------*/
void Socket::vFocalOutline(s4K& sOutline)
{
  _precondition_(_MAGIC_,CN_SOCKET,_FOOTER_);
  
  sOutline.vSprint("<%s focal>"_Stz,aCN[eClass()]);
}

/*----------------------------------------------------------------------------*/
/* is socket open?                                                            */
/*----------------------------------------------------------------------------*/
bool Socket::bIsOpen()
{
  _precondition_(_MAGIC_,CN_SOCKET,_FOOTER_);
  
  if (C_UNKNOWN != m_iSocketFd)
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* accept a socket connection request from a client                           */
/*----------------------------------------------------------------------------*/
bool Socket::bAcceptClient(int& iSocketId, SysCall& SysCall)
{
  _precondition_(_MAGIC_,CN_SOCKET,_FOOTER_);
  
  SysCall.vClear();
  
  sockaddr_in xClientAddr;
  
  socklen_t iSize = sizeof(struct sockaddr_in);
  
#ifdef WITH_STATISTICS_COUNTERS
  gpAttachMap->m_oStatistics.Socket.m_nAccepts++;
#endif
  
  iSocketId = accept(m_iSocketFd
                         ,(struct sockaddr*)&xClientAddr
                         ,&iSize
                         );
  if (C_UNKNOWN == iSocketId) {
    SysCall.vError(errno);
    return false;
  }
  
  if (AM::bAttached()) AM::pResourceTracker()->vIncSocketOpen();
  
  AM::vLogRes(SL_INF,this,RA_SKT_ACCEPTED,
              "this server has accepted new client"_Stz); //!format-LogRes

  SysCall.vSuccess();
  return true;
}

/*----------------------------------------------------------------------------*/
/* close a socket connection (server or client)                               */
/*----------------------------------------------------------------------------*/
bool Socket::bClose(SysCall& SysCall)
{
  _precondition_(_MAGIC_,CN_SOCKET,_FOOTER_);
  
  SysCall.vClear();
  
  int iError = shutdown(m_iSocketFd, SHUT_RDWR);
  
  if (iError && ENOTCONN != errno) {
    SysCall.vError(errno);
    return false;
  }

#ifdef WITH_STATISTICS_COUNTERS
  gpAttachMap->m_oStatistics.Socket.m_nCloses++;
#endif
  
  iError = close(m_iSocketFd);
  if (iError) {
    SysCall.vError(errno);
    return false;
  }
  
  m_iSocketFd = C_UNKNOWN;
  
  if (AM::bAttached()) AM::pResourceTracker()->vDecSocketOpen();
  
  AM::vLogRes(SL_INF,this,RA_SKT_CLOSED,
              "Socket closed : "_Stz); //!format-LogRes

  SysCall.vSuccess();
  return true;
}

/*----------------------------------------------------------------------------*/
/* connect this client with nominated server                                  */
/*----------------------------------------------------------------------------*/
bool Socket::bConnectServer(Stz* sAddr, int iPort, SysCall& SysCall)
{
  _precondition_(_MAGIC_,CN_SOCKET,_FOOTER_);
  
  int iResult;
  int iOpt = 1;
  
  SysCall.vClear();
  
  if (C_UNKNOWN == m_iSocketFd) {
    m_iSocketFd = socket(AF_INET,SOCK_STREAM,0);

    if (m_iSocketFd < 0) {
      SysCall.vError(errno);
      return false;
    }
    
    iResult = setsockopt(m_iSocketFd
                         ,SOL_SOCKET
                         ,SO_REUSEADDR
                         ,(void*)&iOpt
                         ,sizeof(iOpt)
                         );
    if (C_UNKNOWN == iResult) {
      SysCall.vError(errno);
      bClose(SysCall);
      return false;
    }
  }
  
  struct addrinfo  xAddrInfo;
  struct addrinfo* pAddrInfo = NULL;
  
  MemLib::vZero(&xAddrInfo,sizeof(xAddrInfo),CN_SOCKET);
  
  xAddrInfo.ai_family   = AF_INET;
  xAddrInfo.ai_socktype = SOCK_STREAM;
  xAddrInfo.ai_protocol = IPPROTO_TCP;
  xAddrInfo.ai_flags    = AI_PASSIVE;
  
  s256 sPort;
  StrLib::vSprint(sPort.szStz(), "%lld"_Stz, iPort);
  
  i8 iReturn = getaddrinfo(sAddr->azChar(), sPort.azChar(), &xAddrInfo, &pAddrInfo);
  if (iReturn != 0) {
    vTrace("getaddrinfo: error %lld"_Stz, iReturn);
  }

#ifdef WITH_STATISTICS_COUNTERS
  gpAttachMap->m_oStatistics.Socket.m_nConnects++;
#endif
  
  AM::vLogRes(SL_INF,this,RA_SKT_CONNECT,
              "this client attempting connection to server %s %d"_Stz, //!format-LogRes
              sAddr,iPort);
  
  iResult = connect(m_iSocketFd,(struct sockaddr*)pAddrInfo->ai_addr,sizeof(struct sockaddr));
  
  if (iResult != 0) {
    SysCall.vError(errno);
    close(m_iSocketFd);
    m_iSocketFd = C_UNKNOWN;
    return false;
  }
  
  if (AM::bAttached()) AM::pResourceTracker()->vIncSocketOpen();
  
  AM::vLogRes(SL_INF,this,RA_SKT_CONNECTED,
              "this client now connected to server %s %d"_Stz, //!format-LogRes
              sAddr,iPort);

  SysCall.vSuccess();
  return true;
}

/*----------------------------------------------------------------------------*/
/* create a listening server socket on this server                            */
/*----------------------------------------------------------------------------*/
bool Socket::bCreateServer(int iPortNo, SysCall& SysCall)
{
  _precondition_(_MAGIC_,CN_SOCKET,_FOOTER_);
  
  int iResult;
  int iOpt = 1;
  int iBacklogOf4 = 4;
  sockaddr_in xThisServerAddr;
  
  SysCall.vClear();
  
  /*
    refer: $ man 2 listen
   
    DESCRIPTION
    Creation of socket-based connections requires several operations.  First,
    a socket is created with socket(2).  Next, a willingness to accept incoming
    connections and a  queue limit for incoming connections are specified with
    listen().  Finally, the connections are accepted with accept(2).  The
    listen() call applies only to sockets of type SOCK_STREAM.
    
    The backlog parameter defines the maximum length for the queue of pending
    connections.  If a connection request arrives with the queue full, the
    client may receive an error with an indication of ECONNREFUSED.
    Alternatively, if the underlying protocol supports retransmission, the
    request may be ignored so that retries may succeed.
  */
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 1) initialize the server sockaddr_in structure
  
  vAddrInit(&xThisServerAddr,iPortNo);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create a socket end-point
  
  m_iSocketFd = socket(AF_INET
                      ,SOCK_STREAM
                      ,0
                      );
  
  if (C_UNKNOWN == m_iSocketFd) {
    SysCall.vError(errno);
    return false;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 2) allow quick socket reuse
  
  //! CAUTION: need both setsockopt(SO_REUSEADDR) and bind(sockaddr)
  //!          to avoid accidental use of port by two processes
  
  // 2a) set the socket to allow quick reuse of port-address
  
  iResult = setsockopt(m_iSocketFd
                       ,SOL_SOCKET     // setting socket-level option
                       ,SO_REUSEADDR
                       ,(void*)&iOpt
                       ,sizeof(iOpt)
                       );

  if (C_UNKNOWN == iResult) {
    SysCall.vError(errno);
    return false;
  }

  // 2b) bind a name-address to this (listening) socket
  //     this prohibits the same actual socket being used by two processes
  //
  // refer: https://hea-www.harvard.edu/~fine/Tech/addrinuse.html

  iResult = bind(m_iSocketFd
                 ,(struct sockaddr*)&xThisServerAddr
                 ,sizeof(xThisServerAddr)
                 );

  if (iResult != 0) {
    SysCall.vError(errno);
    return false;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 3) allow socket to listen for new connections
  
  iResult = listen(m_iSocketFd
                   ,iBacklogOf4                                      //! Why 4??
                   );
  
  if (iResult != 0) {
    SysCall.vError(errno);
    return false;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
#ifdef WITH_STATISTICS_COUNTERS
  gpAttachMap->m_oStatistics.Socket.m_nListens++;
#endif
  
  if (AM::bAttached()) AM::pResourceTracker()->vIncSocketOpen();
  
  AM::vLogRes(SL_INF,this,RA_SKT_LISTENER,
              "listening socket created port=%d"_Stz, //!format-LogRes
              iPortNo);

  SysCall.vSuccess();
  return true;
}

/*----------------------------------------------------------------------------*/
/* grab lock for write                                                        */
/*----------------------------------------------------------------------------*/
void Socket::vGrabWrite(void)
{
  _precondition_(_MAGIC_,CN_SOCKET,_FOOTER_);
  
  AM::vLogRes(SL_INF,this,RA_SKT_GRAB,
              "attempt"_Stz); //!format-LogRes
 
  m_okLock.vGrabWrite();
  
  AM::vLogRes(SL_INF,this,RA_SKT_GRABBED,
              "successful"_Stz); //!format-LogRes
}

/*----------------------------------------------------------------------------*/
/* drop lock for write                                                        */
/*----------------------------------------------------------------------------*/
void Socket::vDropWrite(void)
{
  _precondition_(_MAGIC_,CN_SOCKET,_FOOTER_);
  
  m_okLock.vDropWrite();
  
  AM::vLogRes(SL_INF,this,RA_SKT_DROPPED,
              "successful"_Stz); //!format-LogRes
}

/*----------------------------------------------------------------------------*/
/* recv n bytes from this socket into given buffer                            */
/*----------------------------------------------------------------------------*/
bool Socket::bRecvBytes(void* aBytes, i8 nBytes, SysCall& SysCall)
{
  _precondition_(_MAGIC_,CN_SOCKET,_FOOTER_);
  
  int nSoFar = 0;
  i8 nMore = nBytes;
  char* pSoFar = (char*)aBytes;
  
  SysCall.vClear();
  
  if (nBytes == 0) {
    vTrace("bRecvBytes() asked to recv 0 bytes\n"_Stz);
    RASSERT(nBytes == 0);
  }
  
  while (nSoFar != nBytes)
  {
    pSoFar = (char*)aBytes + nSoFar;
 
#ifdef WITH_STATISTICS_COUNTERS
    gpAttachMap->m_oStatistics.Socket.m_nRecvs++;
#endif
    
    i8 nRecvBytes = recv(m_iSocketFd,pSoFar,nMore,0);
    
    if (C_UNKNOWN == nRecvBytes) {
      SysCall.vError(errno);
      //vTrace("ERROR1: %lld\n"_Stz, errno);
      return false;
    }
    
    if (0 == nRecvBytes) {
      vTrace("Received 0 bytes; errno %lld\n"_Stz, errno);
      SysCall.vError(errno);
      //vTrace("ERROR2: %lld\n"_Stz, errno);
      return false;
    }
    
    nSoFar += nRecvBytes;
    nMore -= nRecvBytes;
    
#ifdef WITH_STATISTICS_COUNTERS
    gpAttachMap->m_oStatistics.Socket.m_nRecvBytes += nRecvBytes;
#endif
    
  AM::vLogRes(SL_INF,this,RA_SKT_RECEIVED,
              "Socket received %lldB"_Stz, //!format-LogRes
              nRecvBytes);

    //vTrace("Bytes received: (%lld) %lld\n"_Stz, nBytes, nSoFar);
  }
  
  SysCall.vSuccess();
  return true;
}

/*----------------------------------------------------------------------------*/
/* write n bytes to this socket from given buffer                             */
/*----------------------------------------------------------------------------*/
bool Socket::bSendBytes(void* pBuffer, i8 nBytes, SysCall& SysCall)
{
  _precondition_(_MAGIC_,CN_SOCKET,_FOOTER_);
  
  SysCall.vClear();
  
#ifdef WITH_STATISTICS_COUNTERS
  gpAttachMap->m_oStatistics.Socket.m_nSends++;
#endif
  
  i8 nSentBytes = send(m_iSocketFd,pBuffer,nBytes,0);
  if (C_UNKNOWN == nSentBytes) {
    SysCall.vError(errno);
    return false;
  }
  
#ifdef WITH_STATISTICS_COUNTERS
  gpAttachMap->m_oStatistics.Socket.m_nSendBytes += nSentBytes;
#endif
  
  AM::vLogRes(SL_INF,this,RA_SKT_SENT,
              "Socket sent %lldB"_Stz, //!format-LogRes
              nSentBytes);

  if (nSentBytes == nBytes) {
    SysCall.vSuccess();
    return true;
  }
  else {
    return false;
  }
}

/*----------------------------------------------------------------------------*/
/* set socket to be non-blocking                                              */
/*----------------------------------------------------------------------------*/
bool Socket::bSetNonBlocking(SysCall& SysCall)
{
  _precondition_(_MAGIC_,CN_SOCKET,_FOOTER_);
  
  SysCall.vClear();
  
  // get current socket flags
  int iFlags = fcntl(m_iSocketFd,F_GETFL,0);
  if (C_UNKNOWN == iFlags) {
    SysCall.vError(errno);
    return false;

  }
  
  // set non-blocking
  int iResult = fcntl(m_iSocketFd,F_SETFL,iFlags | O_NONBLOCK);
  if (C_UNKNOWN == iResult) {
    SysCall.vError(errno);
    return false;

  }
  
  // record socket as non-blocking
  m_bBlocking = true;                                                  //! fix
  
  SysCall.vSuccess();
  return true;
}

/*----------------------------------------------------------------------------*/
/* test if there is data available?                                           */
/*----------------------------------------------------------------------------*/
bool Socket::bTestForData(SysCall& SysCall)
{
  _precondition_(_MAGIC_,CN_SOCKET,_FOOTER_);
  
  ASSERT(m_bBlocking); // logic check                                  //! fix
  
  SysCall.vClear();
  
  char cc;
  
#ifdef WITH_STATISTICS_COUNTERS
  gpAttachMap->m_oStatistics.Socket.m_nPeeks++;
#endif
  
  i8 nRecv = recv(m_iSocketFd,&cc,1,MSG_PEEK);
  
  if (C_UNKNOWN == nRecv && EAGAIN == errno) {
    return false;
  }
  
  if (C_UNKNOWN == nRecv) {
    SysCall.vError(errno);
    return false;
  }
  
  //! what should nRecv be at this point?
  
  SysCall.vSuccess();
  return true;
}

/*----------------------------------------------------------------------------*/
/* write n bytes to this socket from given buffer                             */
/*----------------------------------------------------------------------------*/
bool Socket::bWriteVch(Stz* aChar, SysCall& SysCall)
{
  _precondition_(_MAGIC_,CN_SOCKET,_FOOTER_);
  
  SysCall.vClear();
  
  i8 nBytes = aChar->nLength();
 
#ifdef WITH_STATISTICS_COUNTERS
  gpAttachMap->m_oStatistics.Socket.m_nSends++;
#endif
  
  i8 nSent = send(m_iSocketFd, aChar->azChar(), nBytes,0);
  
  if (nSent != nBytes)
    return false;

#ifdef WITH_STATISTICS_COUNTERS
  gpAttachMap->m_oStatistics.Socket.m_nSendBytes += nSent;
#endif
  
  SysCall.vSuccess();
  return true;
}

///////////////////////////////////////////////////////////////////////////////
//                                udp support                                //
///////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* create a UDP socket                                                        */
/*----------------------------------------------------------------------------*/
bool Socket::bSocket(SysCall& SysCall)
{
  _precondition_(_MAGIC_,CN_SOCKET,_FOOTER_);
  
  SysCall.vClear();
  
  //m_iSocketFd = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
  m_iSocketFd = socket(AF_INET,SOCK_STREAM,0);

  if (m_iSocketFd < 0) {
    SysCall.vError(errno);
    return false;
  }
  
  SysCall.vSuccess();
  return true;
}

/*----------------------------------------------------------------------------*/
/* identify (name) a UDP socket                                               */
/*----------------------------------------------------------------------------*/
bool Socket::bBind(SysCall& SysCall)
{
  _precondition_(_MAGIC_,CN_SOCKET,_FOOTER_);
  
  SysCall.vClear();
  
  return bBind(0, SysCall);
}

/*----------------------------------------------------------------------------*/
/* identify (name) a UDP socket                                               */
/*----------------------------------------------------------------------------*/
bool Socket::bBind(int iPort, SysCall& SysCall)
{
  _precondition_(_MAGIC_,CN_SOCKET,_FOOTER_);
  
  SysCall.vClear();
  
  struct sockaddr_in xAddr;
  
  vAddrInit(&xAddr,iPort);
  
  vTrace("sin_port %d; sin_addr.s_add %d\n"_Stz
    ,xAddr.sin_port
    ,xAddr.sin_addr.s_addr
    );
  
  int iResult = bind(m_iSocketFd
                     ,(struct sockaddr *)&xAddr
                     ,sizeof(xAddr)
                     );
  
  if (iResult != 0) {
    SysCall.vError(errno);
    return false;
  }
  
  SysCall.vSuccess();
  return true;
}

/*----------------------------------------------------------------------------*/
/* identify (name) a UDP socket                                               */
/*----------------------------------------------------------------------------*/
bool Socket::bBind(Stz* sAddr, int iPort, SysCall& SysCall)
{
  _precondition_(_MAGIC_,CN_SOCKET,_FOOTER_);
  
  struct sockaddr_in xAddr;
  
  SysCall.vClear();
  
  vAddrInit(&xAddr,sAddr,iPort);
  
  vTrace("sin_port %d; sin_addr.s_add %d\n"_Stz
    ,xAddr.sin_port
    ,xAddr.sin_addr.s_addr
    );
  
  int iResult = bind(m_iSocketFd
                     ,(struct sockaddr *)&xAddr
                     ,sizeof(xAddr)
                     );
  
  if (iResult != 0) {
    SysCall.vError(errno);
    return false;

  }
  
  SysCall.vSuccess();
  return true;
}

/*----------------------------------------------------------------------------*/
/* set receive buffer size                                                    */
/*----------------------------------------------------------------------------*/
bool Socket::bRecvBuffer(int nBytes, SysCall& SysCall)
{
  _precondition_(_MAGIC_,CN_SOCKET,_FOOTER_);
  
  int iResult;
  int iOpt = nBytes;
  
  SysCall.vClear();
  
  iResult = setsockopt(m_iSocketFd
                       ,SOL_SOCKET
                       ,SO_RCVBUF
                       ,(void*)&iOpt
                       ,sizeof(iOpt)
                       );
  if (C_UNKNOWN == iResult)
  {
    SysCall.vError(errno);
    return false;
  }
  
  SysCall.vSuccess();
  return true;
}

/*----------------------------------------------------------------------------*/
/* set up for sendto server                                                   */
/*----------------------------------------------------------------------------*/
bool Socket::bServer(Stz* sAddr, int iPort, SysCall& SysCall)
{
  _precondition_(_MAGIC_,CN_SOCKET,_FOOTER_);
  
  //  struct hostent *hp;     /* host information */
  
  SysCall.vClear();
  
  vAddrInit(&m_xServAddr,sAddr,iPort);
  
  vTrace("sin_port %d; sin_addr.s_add %d\n"_Stz
    ,m_xServAddr.sin_port
    ,m_xServAddr.sin_addr.s_addr
    );
  
  //  /* look up the address of the server given its name */
  //  hp = gethostbyname(sAddr);
  //  if ( ! hp )
  //  {
  //    ++gSystemError;
  //    L(LSysCall
  //      ,"gethostbyname() error; %s; errno=%d\n"
  //      ,sAddr
  //      ,errno
  //      );
  //    return R_ERROR;
  //  }
  //
  //  /* put the host's address into the server address structure */
  //  memcpy((void *)&m_xServAddr.sin_addr, hp->h_addr_list[0], hp->h_length);
  
  SysCall.vSuccess();
  return true;
}

/*----------------------------------------------------------------------------*/
/* send a message via a UDP socket                                            */
/*----------------------------------------------------------------------------*/
/*
bool Socket::bSendTo(void* aBytes, i8 nBytes, SysCall& SysCall)
{
  _precondition_(_MAGIC_,CN_SOCKET,_FOOTER_);
  
  int nSoFar = 0;
  i8 nMore = nBytes;
  char* pSoFar = (char*)aBytes;
  
  SysCall.vClear();
  
  if (nBytes == 0) {
    vTrace("sendto() sending 0 bytes\n"_Stz);
    RASSERT(nBytes == 0);
  }
  
  while (nSoFar != nBytes)
  {
    pSoFar = (char*)aBytes + nSoFar;
    
    i8 nBytesSent = sendto(m_iSocketFd
                           ,nMore
                           ,0
                           ,(struct sockaddr*)&m_xServAddr
                           ,sizeof(m_xServAddr)
                           );
    
    if (C_UNKNOWN == nBytesSent)
    {
      SysCall.vError(errno);
      return false;
    }
    
    if (0 == nBytesSent)
    {
      SysCall.vError(errno);
      return false;
    }
    
    if (nBytesSent < nBytes)
    {
      vWarn("sendto() less bytes sent; %lld v %lld\n"_Stz
            ,nBytesSent
            ,nBytes
            );
    }
    nSoFar += nBytesSent;
    nMore -= nBytesSent;
  }
  
  SysCall.vSuccess();
  return true;
}
*/
/*----------------------------------------------------------------------------*/
/* send a message via a UDP socket                                            */
/*----------------------------------------------------------------------------*/
bool Socket::bSendTo(void* aBytes, i8 nBytes, SysCall& SysCall)
{
  _precondition_(_MAGIC_,CN_SOCKET,_FOOTER_);
  
  SysCall.vClear();
  
  if (nBytes == 0) {
    vTrace("sendto() sending 0 bytes\n"_Stz);
    RASSERT(nBytes == 0);
  }

#ifdef WITH_STATISTICS_COUNTERS
  gpAttachMap->m_oStatistics.Socket.m_nSendTos++;
#endif
  
//  AM::vLogRes(SL_INF,this,RA_SKT_SEND,
//              "Socket send %lldB"_Stz, //!format-LogRes
//              nBytes);
  
  i8 nBytesSent = sendto(m_iSocketFd
                         ,aBytes
                         ,nBytes
                         ,0
                         ,(struct sockaddr*)&m_xServAddr
                         ,sizeof(m_xServAddr)
                         );
  
  if (C_UNKNOWN == nBytesSent)
  {
    SysCall.vError(errno);
    return false;
  }
  
  if (0 == nBytesSent)
  {
    SysCall.vError(errno);
    return false;
  }

#ifdef WITH_STATISTICS_COUNTERS
  gpAttachMap->m_oStatistics.Socket.m_nSendBytes += nBytesSent;
#endif
  
  if (nBytesSent < nBytes)
  {
    vWarn("sendto() less bytes sent; %lld v %lld\n"_Stz
      ,nBytesSent
      ,nBytes
      );
    SysCall.vError(errno);
    return false;
  }
  
  SysCall.vSuccess();
  return true;
}

/*----------------------------------------------------------------------------*/
/* receive a message via a UDP socket                                         */
/*----------------------------------------------------------------------------*/
bool Socket::bRecvFrom(void* pBuffer, i8 nBytes, i8* pRecv, SysCall& SysCall)
{
  _precondition_(_MAGIC_,CN_SOCKET,_FOOTER_);
  
  struct sockaddr_in xRemoteAddr;
  socklen_t iRemoteLen = sizeof(xRemoteAddr);
  
  SysCall.vClear();
 
#ifdef WITH_STATISTICS_COUNTERS
  gpAttachMap->m_oStatistics.Socket.m_nRecvFroms++;
#endif
  
  i8 nBytesRecv = recvfrom(m_iSocketFd
                         ,pBuffer
                         ,nBytes
                         ,0
                         ,(struct sockaddr *)&xRemoteAddr
                         ,&iRemoteLen
                         );
  
  if (C_UNKNOWN == nBytesRecv)
  {
    SysCall.vError(errno);
    return false;
  }

#ifdef WITH_STATISTICS_COUNTERS
  gpAttachMap->m_oStatistics.Socket.m_nRecvBytes += nBytesRecv;
#endif
  
  *pRecv = nBytesRecv;
  
  SysCall.vSuccess();
  return true;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(Socket)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
