/*============================================================================*/
/* socket.cpp - tcp/ip sockets                                                */
/*============================================================================*/

#include "BASE.h"

#include "Socket.hpp"
#include "Resources.hpp"

//#include <pcap/pcap.h>
#include <arpa/inet.h>
//#include <netinet/in.h>

Atomic gixSocket;

extern SizeHeap  gxaSizeHeap[];

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(BASE,Socket,CN_SOCKET)
__aNewFunction(Socket,CN_SOCKET)
__vDeleteFunction(Socket,CN_SOCKET)
__aFooterFunction(Socket)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Socket::Socket(Object* pIsFor)
: Resource(CN_SOCKET,pIsFor)
, m_okLock(this)
{
  _precondition_(_MAGIC_,CN_SOCKET,_FOOTER_);
  
  m_pIsFor    = pIsFor;
  m_iSocketFd = C_UNKNOWN;
  m_bBlocking = true;
  
  m_aLowBuff  = MemLib::aMallocBlock(C_SIZE_POOL_BUFFER);
  
  MemLib::vZero(m__Padding,sizeof(m__Padding),CN_SOCKET);
  
  vAddrInit(&m_xServAddr,nullptr,0);
  
  m_bSuccess = false;
  m_pReceivedObject = nullptr;
  
  _checkInitialised_(CN_SOCKET);
  
  ++ gixSocket;

  _postcondition_(_MAGIC_,CN_SOCKET,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Socket::Socket(Object* pIsFor, int iSocketId)
: Resource(CN_SOCKET,pIsFor)
, m_okLock(this)
{
  _precondition_(_MAGIC_,CN_SOCKET,_FOOTER_);
  
  m_pIsFor    = pIsFor;
  m_iSocketFd = iSocketId;
  m_bBlocking = true;
  
  m_aLowBuff  = MemLib::aMallocBlock(C_SIZE_POOL_BUFFER);
  
  MemLib::vZero(m__Padding,sizeof(m__Padding),CN_SOCKET);
  
  vAddrInit(&m_xServAddr,nullptr,0);
  
  m_bSuccess = false;
  m_pReceivedObject = nullptr;

  _checkInitialised_(CN_SOCKET);
  
  ++ gixSocket;
  
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
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Socket::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  UNUSED(eClass);
  UNUSED(aTab);

  Socket* pSocket = _pSocket(pObject);

  vTrace("id %lld %s\n"_Stz,
         pSocket->m_iSocketFd,
         pSocket->m_bBlocking ? "blocking"_Stz : "non-blocking"_Stz);
}

/*----------------------------------------------------------------------------*/
/* statistics                                                                 */
/*----------------------------------------------------------------------------*/
void Socket::vStatistics(void)
{
  s32 sByteMag;
  
  Statistics* pStat = AM::pStatistics();

  vTrace("  nSocketAccepts    %lld\n"_Stz,pStat->Socket_.m_nAccepts);
  vTrace("  nSocketCloses     %lld\n"_Stz,pStat->Socket_.m_nCloses);
  vTrace("  nSocketConnects   %lld\n"_Stz,pStat->Socket_.m_nConnects);
  vTrace("  nSocketListens    %lld\n"_Stz,pStat->Socket_.m_nListens);
  vTrace("  nSocketPeeks      %lld\n"_Stz,pStat->Socket_.m_nPeeks);
  vTrace("  nSocketRecvs      %lld\n"_Stz,pStat->Socket_.m_nRecvs);
  vTrace("  nSocketRecvFroms  %lld\n"_Stz,pStat->Socket_.m_nRecvFroms);
  vTrace("  nSocketSends      %lld\n"_Stz,pStat->Socket_.m_nSends);
  vTrace("  nSocketSendTos    %lld\n"_Stz,pStat->Socket_.m_nSendTos);

  MagLib::vByteMag(pStat->Socket_.m_nRecvBytes,  sByteMag);
  vTrace("  nSocketRecvBytes  %s\n"_Stz,sByteMag.szStz());
  MagLib::vByteMag(pStat->Socket_.m_nSentBytes,  sByteMag);
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
/* get success flag                                                           */
/*----------------------------------------------------------------------------*/
bool Socket::bSuccess(void)
{
  _precondition_(_MAGIC_,CN_SOCKET,_FOOTER_);
  
  return m_bSuccess;
}

/*----------------------------------------------------------------------------*/
/* set success flag                                                           */
/*----------------------------------------------------------------------------*/
void Socket::vSuccess(bool bSuccess)
{
  _precondition_(_MAGIC_,CN_SOCKET,_FOOTER_);
  
  m_bSuccess = bSuccess;
}

/*----------------------------------------------------------------------------*/
/* recv next object from this socket                                          */
/*----------------------------------------------------------------------------*/
Object* Socket::pRecvObject(void)
{
  _precondition_(_MAGIC_,CN_SOCKET,_FOOTER_);
  
  RASSERT(! pReceivedObject());

  bool bLowMem = gpBufferPool->bLowMemory();

  char caObjectHeader[sizeof(Object) + C_1K];
  Header* aHeader = (Header*) & caObjectHeader[0];
  Object* pObject = reinterpret_cast<Object*>(&caObjectHeader);

  pObject->vSetWasReceived(true);
  pObject->vSetPostRecvRun(false);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // receive the header of the next object

  SysCall oSysCall;
  bool bResult = bRecvBytes(aHeader,sizeof(Object),oSysCall);
  if (! bResult) {
    //m_pDataStack->vPushInt(false);
    //vReceivedObject(nullptr);
    vTrace("Socket::pRecvObject (header) : bRecvBytes returned false\n"_Stz);
    vSuccess(false);
    return nullptr;
  }

//  SP_Header spHeader;
//  vTrace("%lld: vSocketRecvObject: Header %s\n"_Stz,
//         AM::iThisNodeNo(),
//         spHeader.sz(aHeader));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // receive the body of the object (enough memory condition)

  eCN eClass = pObject->eGetClassNo();
  if (! bLowMem || ! gxaSizeHeap[eClass].m_bLowMemoryDiscard) {
    //vTrace("class:%s\n"_Stz, aCN[eClass]);
    Object* pReceivedObject = Heaps::pAllocate(eClass);
    MemLib::vCopy(pReceivedObject,pObject,sizeof(Object),eClass);

    void* aBody = aAdd<void*>(pReceivedObject,sizeof(Object));
    i8 nRemainingBytes = gxaSizeHeap[eClass].m_nClassSize - sizeof(Object);

    bResult = bRecvBytes(aBody,nRemainingBytes,oSysCall);
    if (! bResult) {
     //m_pDataStack->vPushInt(false);
     //vReceivedObject(nullptr);
     vTrace("Socket::pRecvObject (body) : bRecvBytes returned false\n"_Stz);
     vSuccess(false);
     return nullptr;
    }

    pReceivedObject->vRunPostRecv();

    //vReceivedObject(pReceivedObject);
    vSuccess(true);
    return pReceivedObject;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // receive and discard the body of the object (low memory condition)
  if (AM::SHOW::bLowMem_Recv())
    vTrace("Socket::pRecvObject : LOW MEMORY\n"_Stz);

  void* aLowBuff = m_aLowBuff;

  // copy the object header into the LowBuffer
  MemLib::vCopy(aLowBuff,pObject,sizeof(Object),eClass);

  // recv rest of the buffer into LowBuffer
  void* aLowBody = aAdd<void*>(aLowBuff,sizeof(Object));
  i8 nRemainingBytes = gxaSizeHeap[eClass].m_nClassSize - sizeof(Object);

  bResult = bRecvBytes(aLowBody,nRemainingBytes,oSysCall);

  //vReceivedObject(nullptr);
  vSuccess(true);
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get received object                                                        */
/*----------------------------------------------------------------------------*/
Object* Socket::pReceivedObject(void)
{
  _precondition_(_MAGIC_,CN_SOCKET,_FOOTER_);
  
  return m_pReceivedObject;
}

/*----------------------------------------------------------------------------*/
/* set received object                                                        */
/*----------------------------------------------------------------------------*/
void Socket::vReceivedObject(Object* pReceivedObject)
{
  _precondition_(_MAGIC_,CN_SOCKET,_FOOTER_);
  
  m_pReceivedObject = pReceivedObject;
}

/*----------------------------------------------------------------------------*/
/* outline                                                                    */
/*----------------------------------------------------------------------------*/
void Socket::vOutline(s4K& sOutline)
{
  _precondition_(_MAGIC_,CN_SOCKET,_FOOTER_);
  
  sOutline.vSprint("<%s focal>"_Stz,aCN[eGetClassNo()]);
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
  
  if (AM::bAttached()) AM::pResources()->vIncSocketOpen();
  
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
  
  if (AM::bAttached()) AM::pResources()->vDecSocketOpen();
  
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
    
  iResult = connect(m_iSocketFd,(struct sockaddr*)pAddrInfo->ai_addr,sizeof(struct sockaddr));
  
  if (iResult != 0) {
    SysCall.vError(errno);
    close(m_iSocketFd);
    m_iSocketFd = C_UNKNOWN;
    return false;
  }
  
  if (AM::bAttached()) AM::pResources()->vIncSocketOpen();
  
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
  
  if (AM::bAttached()) AM::pResources()->vIncSocketOpen();
  
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
      SysCall.vError(errno); // don't die after this, node can come and goes
      //vTrace("ERROR2: %lld\n"_Stz, errno);
      return false;
    }

    nSoFar += nRecvBytes;
    nMore -= nRecvBytes;
    
#ifdef WITH_STATISTICS_COUNTERS
    gpAttachMap->m_oStatistics.Socket.m_nRecvBytes += nRecvBytes;
#endif
    
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
  
  NEVER_REACH;
  
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
  gpAttachMap->m_oStatistics.Socket.m_nSentBytes += nSentBytes;
#endif
  
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
bool Socket::bSetRecvBufferSize(int nBytes, SysCall& SysCall)
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
    
    i8 nSentBytes = sendto(m_iSocketFd
                           ,nMore
                           ,0
                           ,(struct sockaddr*)&m_xServAddr
                           ,sizeof(m_xServAddr)
                           );
    
    if (C_UNKNOWN == nSentBytes)
    {
      SysCall.vError(errno);
      return false;
    }
    
    if (0 == nSentBytes)
    {
      SysCall.vError(errno);
      return false;
    }
    
    if (nSentBytes < nBytes)
    {
      vWarn("sendto() less bytes sent; %lld v %lld\n"_Stz
            ,nSentBytes
            ,nBytes
            );
    }
    nSoFar += nSentBytes;
    nMore -= nSentBytes;
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
    
  i8 nSentBytes = sendto(m_iSocketFd
                         ,aBytes
                         ,nBytes
                         ,0
                         ,(struct sockaddr*)&m_xServAddr
                         ,sizeof(m_xServAddr)
                         );
  
  if (C_UNKNOWN == nSentBytes)
  {
    SysCall.vError(errno);
    return false;
  }
  
  if (0 == nSentBytes)
  {
    SysCall.vError(errno);
    return false;
  }

#ifdef WITH_STATISTICS_COUNTERS
  gpAttachMap->m_oStatistics.Socket.m_nSentBytes += nSentBytes;
#endif
    
  if (nSentBytes < nBytes)
  {
    vWarn("sendto() less bytes sent; %lld v %lld\n"_Stz
      ,nSentBytes
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
  
  NEVER_REACH;
  
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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
