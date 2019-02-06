/*============================================================================*/
/* socket.cpp - tcp/ip sockets                                                */
/*============================================================================*/

#include "System.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Socket* System::_pSocket(void* aAddr)
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
Socket::Socket(void)
: Object(CN_SOCKET)
{
  _precondition_(_MAGIC_,CN_SOCKET,_FOOTER_);
  
  m_iSocketId = -1;
  m_bBlocking = true;
  m_okLock.vInitLock(LK_SOCKET);
  
  m_aLowBuff = MemLib::aMallocBlock(C_SIZE_POOL_BUFFER);
  
  vAddrInit(&m_xServAddr,nullptr,0);
  
  _checkInitialised_(CN_SOCKET);
  
  _postcondition_(_MAGIC_,CN_SOCKET,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Socket::Socket(int iSocketId)
: Object(CN_SOCKET)
{
  _precondition_(_MAGIC_,CN_SOCKET,_FOOTER_);
  
  m_iSocketId = iSocketId;
  m_bBlocking = true;
  m_okLock.vInitLock(LK_SOCKET);
  
  m_aLowBuff = MemLib::aMallocBlock(C_SIZE_POOL_BUFFER);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Socket::~Socket() noexcept(false)
{
  _precondition_(_MAGIC_,CN_SOCKET,_FOOTER_);
  
  if (-1 != m_iSocketId) {
    vWarn("Socket %d not closed!\n"_Stz, m_iSocketId);
  }
  
  MemLib::vFreeBlock(m_aLowBuff,C_SIZE_POOL_BUFFER);
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
         pSocket->m_iSocketId,
         pSocket->m_bBlocking ? "blocking"_Stz : "non-blocking"_Stz);
}

/*----------------------------------------------------------------------------*/
/* monitor profile                                                            */
/*----------------------------------------------------------------------------*/
void Socket::vMonitorPortrait(s48K& sPortrait)
{
  Statistics* pStat = &gpAttachMap->m_oStatistics;
  
  s32 sRecvMag; Magnitudes::vByteMag(pStat->Socket.m_nRecvBytes, sRecvMag);
  s32 sSendMag; Magnitudes::vByteMag(pStat->Socket.m_nSendBytes, sSendMag);

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
                      sRecvMag.aStz(),
                    pStat->Socket.m_nSends,
                    pStat->Socket.m_nSendTos,
                      sSendMag.aStz()
                    );
}

/*----------------------------------------------------------------------------*/
/* statistics                                                                 */
/*----------------------------------------------------------------------------*/
void Socket::vStatistics(void)
{
  s32 sByteMag;
  
  vTrace("  nSocketAccepts    %lld\n"_Stz,gpAttachMap->m_oStatistics.Socket.m_nAccepts);
  vTrace("  nSocketCloses     %lld\n"_Stz,gpAttachMap->m_oStatistics.Socket.m_nCloses);
  vTrace("  nSocketConnects   %lld\n"_Stz,gpAttachMap->m_oStatistics.Socket.m_nConnects);
  vTrace("  nSocketListens    %lld\n"_Stz,gpAttachMap->m_oStatistics.Socket.m_nListens);
  vTrace("  nSocketPeeks      %lld\n"_Stz,gpAttachMap->m_oStatistics.Socket.m_nPeeks);
  vTrace("  nSocketRecvs      %lld\n"_Stz,gpAttachMap->m_oStatistics.Socket.m_nRecvs);
  vTrace("  nSocketRecvFroms  %lld\n"_Stz,gpAttachMap->m_oStatistics.Socket.m_nRecvFroms);
  vTrace("  nSocketSends      %lld\n"_Stz,gpAttachMap->m_oStatistics.Socket.m_nSends);
  vTrace("  nSocketSendTos    %lld\n"_Stz,gpAttachMap->m_oStatistics.Socket.m_nSendTos);

  Magnitudes::vByteMag(gpAttachMap->m_oStatistics.Socket.m_nRecvBytes,  sByteMag);
  vTrace("  nSocketRecvBytes  %s\n"_Stz,sByteMag.aStz());
  Magnitudes::vByteMag(gpAttachMap->m_oStatistics.Socket.m_nSendBytes,  sByteMag);
  vTrace("  nSocketSendBytes  %s\n"_Stz,sByteMag.aStz());
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
  aAddr->sin_addr.s_addr = sAddr->aChar() == nullptr ? -1 : inet_addr(sAddr->aChar());
  aAddr->sin_port        = htons((int)iPort);
}

/*----------------------------------------------------------------------------*/
/* copy a socket                                                              */
/*----------------------------------------------------------------------------*/
void Socket::vCopy(Socket* pSource)
{
  _precondition_(_MAGIC_,CN_SOCKET,_FOOTER_);
  
  m_iSocketId = pSource->m_iSocketId;
  m_bBlocking = pSource->m_bBlocking;
}

/*----------------------------------------------------------------------------*/
/* is socket open?                                                            */
/*----------------------------------------------------------------------------*/
bool Socket::bIsOpen()
{
  _precondition_(_MAGIC_,CN_SOCKET,_FOOTER_);
  
  if (-1 != m_iSocketId)
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
  
  gpAttachMap->m_oStatistics.Socket.m_nAccepts++;
  iSocketId = accept(m_iSocketId
                         ,(struct sockaddr*)&xClientAddr
                         ,&iSize
                         );
  if (-1 == iSocketId) {
    SysCall.vError(errno);
    return false;
  }
  
  if (gpAttachMap) gpAttachMap->m_oResourceTracker.vIncSocketOpen();
  if (gpAttachMap->m_bMarkResourceAll || gpAttachMap->m_bMarkResource[SR_SOCKET])
    ActivityMap::vMarkRes(SR_SOCKET,"SysRes: Socket accepted cient"_Stz);

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
  
  int iError = shutdown(m_iSocketId, SHUT_RDWR);
  
  if (iError && ENOTCONN != errno) {
    SysCall.vError(errno);
    return false;
  }
  
  gpAttachMap->m_oStatistics.Socket.m_nCloses++;
  iError = close(m_iSocketId);
  if (iError) {
    SysCall.vError(errno);
    return false;
  }
  
  m_iSocketId = -1;
  
  if (gpAttachMap) gpAttachMap->m_oResourceTracker.vDecSocketOpen();
  if (gpAttachMap->m_bMarkResourceAll || gpAttachMap->m_bMarkResource[SR_SOCKET])
    ActivityMap::vMarkRes(SR_SOCKET,"SysRes: Socket closed"_Stz);

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
  
  m_iSocketId = socket(AF_INET,SOCK_STREAM,0);
  
  if (m_iSocketId < 0) {
    SysCall.vError(errno);
    return false;
  }
  
  iResult = setsockopt(m_iSocketId
                       ,SOL_SOCKET
                       ,SO_REUSEADDR
                       ,(void*)&iOpt
                       ,sizeof(iOpt)
                       );
  if (-1 == iResult) {
    SysCall.vError(errno);
    bClose(SysCall);
    return false;
  }
  
  struct addrinfo  xAddrInfo;
  struct addrinfo* pAddrInfo = NULL;
  
  MemLib::vZero(&xAddrInfo,sizeof(xAddrInfo),CN_SOCKET);
  
  xAddrInfo.ai_family   = AF_INET;
  xAddrInfo.ai_socktype = SOCK_STREAM;
  xAddrInfo.ai_protocol = IPPROTO_TCP;
  xAddrInfo.ai_flags    = AI_PASSIVE;
  
  s256 sPort;
  StrLib::vSprint(sPort.aStz(), "%lld"_Stz, iPort);
  
  i8 iReturn = getaddrinfo(sAddr->aChar(), sPort.aChar(), &xAddrInfo, &pAddrInfo);
  if (iReturn != 0) {
    vTrace("getaddrinfo: error %lld"_Stz, iReturn);
  }

  gpAttachMap->m_oStatistics.Socket.m_nConnects++;
  iResult = connect(m_iSocketId,(struct sockaddr*)pAddrInfo->ai_addr,sizeof(struct sockaddr));
  
  if (iResult != 0) {
    SysCall.vError(errno);
    return false;
  }
  
  if (gpAttachMap) gpAttachMap->m_oResourceTracker.vIncSocketOpen();
  if (gpAttachMap->m_bMarkResourceAll || gpAttachMap->m_bMarkResource[SR_SOCKET])
    ActivityMap::vMarkRes(SR_SOCKET,"SysRes: Socket connected to server %s %d"_Stz,
                       sAddr,iPort);

  SysCall.vSuccess();
  return true;
}

/*----------------------------------------------------------------------------*/
/* create a listening server socket on this server                            */
/*----------------------------------------------------------------------------*/
bool Socket::bCreateServer(int iPort, SysCall& SysCall)
{
  _precondition_(_MAGIC_,CN_SOCKET,_FOOTER_);
  
  int iResult;
  int iOpt = 1;
  sockaddr_in xServerAddr;
  
  SysCall.vClear();
  
  vAddrInit(&xServerAddr,iPort);
  
  m_iSocketId = socket(AF_INET
                    ,SOCK_STREAM
                    ,0
                    );
  if (-1 == m_iSocketId) {
    SysCall.vError(errno);
    return false;
  }
  
  iResult = setsockopt(m_iSocketId
                       ,SOL_SOCKET
                       ,SO_REUSEADDR
                       ,(void*)&iOpt
                       ,sizeof(iOpt)
                       );
  if (-1 == iResult) {
    SysCall.vError(errno);
    return false;
  }
  
  iResult = bind(m_iSocketId
                 ,(struct sockaddr*)&xServerAddr
                 ,sizeof(xServerAddr)
                 );
  
  if (iResult != 0) {
    SysCall.vError(errno);
    return false;
  }
  
  gpAttachMap->m_oStatistics.Socket.m_nListens++;
  iResult = listen(m_iSocketId,4);
  if (iResult != 0) {
    SysCall.vError(errno);
    return false;
  }
  
  if (gpAttachMap) gpAttachMap->m_oResourceTracker.vIncSocketOpen();
  if (gpAttachMap->m_bMarkResourceAll || gpAttachMap->m_bMarkResource[SR_SOCKET])
    ActivityMap::vMarkRes(SR_SOCKET,"SysRes: Socket created listening socket : %d"_Stz,
                       iPort);

  SysCall.vSuccess();
  return true;
}

/*----------------------------------------------------------------------------*/
/* grab lock for write                                                        */
/*----------------------------------------------------------------------------*/
void Socket::vGrabWrite(void)
{
  _precondition_(_MAGIC_,CN_SOCKET,_FOOTER_);
  
  m_okLock.vGrabWrite();
}

/*----------------------------------------------------------------------------*/
/* drop lock for write                                                        */
/*----------------------------------------------------------------------------*/
void Socket::vDropWrite(void)
{
  _precondition_(_MAGIC_,CN_SOCKET,_FOOTER_);
  
  m_okLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* recv n bytes from this socket into given buffer                            */
/*----------------------------------------------------------------------------*/
bool Socket::bRecvBytes(void* pWS, i8 nBytes, SysCall& SysCall)
{
  _precondition_(_MAGIC_,CN_SOCKET,_FOOTER_);
  
  int nSoFar = 0;
  i8 nMore = nBytes;
  char* pSoFar = (char*)pWS;
  
  SysCall.vClear();
  
  if (nBytes == 0) {
    vTrace("bRecvBytes() asked to recv 0 bytes\n"_Stz);
    RASSERT(nBytes == 0);
  }
  
  while (nSoFar != nBytes)
  {
    pSoFar = (char*)pWS + nSoFar;
    
    gpAttachMap->m_oStatistics.Socket.m_nRecvs++;
    i8 nRecvBytes = recv(m_iSocketId,pSoFar,nMore,0);
    
    if (-1 == nRecvBytes) {
      SysCall.vError(errno);
      return false;
    }
    
    if (0 == nRecvBytes) {
      vTrace("Received 0 bytes; errno %lld\n"_Stz, errno);
      SysCall.vError(errno);
      return false;
    }
    
    nSoFar += nRecvBytes;
    nMore -= nRecvBytes;
    gpAttachMap->m_oStatistics.Socket.m_nRecvBytes += nRecvBytes;
    if (gpAttachMap->m_bMarkResourceAll || gpAttachMap->m_bMarkResource[SR_SOCKET])
      ActivityMap::vMarkRes(SR_SOCKET,"SysRes: Socket received %lld bytes"_Stz,
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
  
  gpAttachMap->m_oStatistics.Socket.m_nSends++;
  i8 nSentBytes = send(m_iSocketId,pBuffer,nBytes,0);
  if (-1 == nSentBytes) {
    SysCall.vError(errno);
    return false;
  }
  
  gpAttachMap->m_oStatistics.Socket.m_nSendBytes += nSentBytes;
  if (gpAttachMap->m_bMarkResourceAll || gpAttachMap->m_bMarkResource[SR_SOCKET])
    ActivityMap::vMarkRes(SR_SOCKET,"SysRes: Socket set %lld bytes"_Stz,
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
  int iFlags = fcntl(m_iSocketId,F_GETFL,0);
  if (-1 == iFlags) {
    SysCall.vError(errno);
    return false;

  }
  
  // set non-blocking
  int iResult = fcntl(m_iSocketId,F_SETFL,iFlags | O_NONBLOCK);
  if (-1 == iResult) {
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
  gpAttachMap->m_oStatistics.Socket.m_nPeeks++;
  i8 nRecv = recv(m_iSocketId,&cc,1,MSG_PEEK);
  
  if (-1 == nRecv && EAGAIN == errno) {
    return false;
  }
  
  if (-1 == nRecv) {
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
  
  i8 nBytes = aChar->iLength();
  
  gpAttachMap->m_oStatistics.Socket.m_nSends++;
  i8 nSent = send(m_iSocketId, aChar->aChar(), nBytes,0);
  
  if (nSent != nBytes)
    return false;
  
  gpAttachMap->m_oStatistics.Socket.m_nSendBytes += nSent;
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
  
  m_iSocketId = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
  
  if (m_iSocketId < 0) {
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
  
  int iResult = bind(m_iSocketId
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
  
  int iResult = bind(m_iSocketId
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
  
  iResult = setsockopt(m_iSocketId
                       ,SOL_SOCKET
                       ,SO_RCVBUF
                       ,(void*)&iOpt
                       ,sizeof(iOpt)
                       );
  if (-1 == iResult)
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
bool Socket::bSendTo(void* pBuffer, i8 nBytes, SysCall& SysCall)
{
  _precondition_(_MAGIC_,CN_SOCKET,_FOOTER_);
  
  SysCall.vClear();
  
  if (nBytes == 0) {
    vTrace("sendto() sending 0 bytes\n"_Stz);
    RASSERT(nBytes == 0);
  }
  
  gpAttachMap->m_oStatistics.Socket.m_nSendTos++;
  i8 nBytesSent = sendto(m_iSocketId
                       ,pBuffer
                       ,nBytes
                       ,0
                       ,(struct sockaddr*)&m_xServAddr
                       ,sizeof(m_xServAddr)
                       );
  
  if (-1 == nBytesSent)
  {
    SysCall.vError(errno);
    return false;
  }
  
  if (0 == nBytesSent)
  {
    SysCall.vError(errno);
    return false;
  }
  
  gpAttachMap->m_oStatistics.Socket.m_nSendBytes += nBytesSent;
  
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
  
  gpAttachMap->m_oStatistics.Socket.m_nRecvFroms++;
  i8 nBytesRecv = recvfrom(m_iSocketId
                         ,pBuffer
                         ,nBytes
                         ,0
                         ,(struct sockaddr *)&xRemoteAddr
                         ,&iRemoteLen
                         );
  
  if (-1 == nBytesRecv)
  {
    SysCall.vError(errno);
    return false;
  }
  
  gpAttachMap->m_oStatistics.Socket.m_nRecvBytes += nBytesRecv;
  
  *pRecv = nBytesRecv;
  
  SysCall.vSuccess();
  return true;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Socket::aFooter(void)
{
  _precondition_(_MAGIC_,CN_SOCKET);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
