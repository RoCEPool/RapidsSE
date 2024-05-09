/*============================================================================*/
/* Port.cpp - tcp/ip sockets                                                  */
/*============================================================================*/

#include "BASE.h"

#include "Port.hpp"
#include "Resources.hpp"
#include "Marker.hpp"
#include "AM.hpp"

//#include <pcap/pcap.h>
#include <arpa/inet.h>
//#include <netinet/in.h>

Stz* C_LOOPBACK_IP  = "127.0.0.1"_Stz;


extern Atomic gixSocket;

extern SizeHeap  gxaSizeHeap[];

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(BASE,Port,CN_PORT)
__aNewFunction(Port,CN_PORT)
__vDeleteFunction(Port,CN_PORT)
__aFooterFunction(Port)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Port::Port(Object* pIsFor)
: Resource(CN_PORT,pIsFor)
, m_okLock(this)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  m_pIsFor    = pIsFor;
  m_iSocketFd = C_UNKNOWN;
  m_bBlocking = true;
  
  vAddrInit(&m_xServerAddr, nullptr, 0);
  vAddrInit(&m_xClientAddr,nullptr,0);

  m_bSuccess = false;
  
  m_nSends = 0;
  m_nBytesSent = 0;
  
  m_nRecvs = 0;
  m_nBytesRcvd = 0;
 
  m_pTotup  = new Totup;
  m_pWhyTot = new Totup;

  _checkInitialised_(CN_PORT);
  
  ++ gixSocket;

  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Port::Port(Object* pIsFor, int iSocketId)
: Resource(CN_SOCKET,pIsFor)
, m_okLock(this)
{
  _precondition_(_MAGIC_,CN_SOCKET,_FOOTER_);
  
  m_pIsFor    = pIsFor;
  m_iSocketFd = iSocketId;
  m_bBlocking = true;

  vAddrInit(&m_xServerAddr, nullptr, 0);
  vAddrInit(&m_xClientAddr,nullptr,0);
  
  m_bSuccess = false;

  m_nSends = 0;
  m_nBytesSent = 0;
  
  m_nRecvs = 0;
  m_nBytesRcvd = 0;
  
  m_pTotup = new Totup;
  m_pWhyTot = new Totup;

  _checkInitialised_(CN_PORT);
  
  ++ gixSocket;
  
  _postcondition_(_MAGIC_,CN_SOCKET,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Port::Port(Object* pIsFor, int iSocketId, Port* pFromPort)
    : Resource(CN_SOCKET,pIsFor)
    , m_okLock(this)
{
  _precondition_(_MAGIC_,CN_SOCKET,_FOOTER_);

  m_pIsFor    = pIsFor;
  m_iSocketFd = iSocketId;
  m_bBlocking = true;

  MemLib::vCopy(&m_xServerAddr, &(pFromPort->m_xServerAddr), sizeof(m_xServerAddr), CN_PORT);
  MemLib::vCopy(&m_xClientAddr, &(pFromPort->m_xClientAddr), sizeof(m_xClientAddr), CN_PORT);

  m_bSuccess = false;

  m_nSends = 0;
  m_nBytesSent = 0;

  m_nRecvs = 0;
  m_nBytesRcvd = 0;

  m_pTotup = new Totup;
  m_pWhyTot = new Totup;

  _checkInitialised_(CN_PORT);

  ++ gixSocket;

  _postcondition_(_MAGIC_,CN_SOCKET,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Port::~Port(void) noexcept(false)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  delete m_pTotup;
  delete m_pWhyTot;

  if (C_UNKNOWN != m_iSocketFd) {
    vTrace("Port %d not closed!\n"_Stz, m_iSocketFd);
  }
    
  -- gixSocket;
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Port::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  UNUSED(eClass);
  UNUSED(aTab);

  Port* pPort = _pPort(pObject);

  vTrace("id %lld %s\n"_Stz,
         pPort->m_iSocketFd,
         pPort->m_bBlocking ? "blocking"_Stz : "non-blocking"_Stz);
}

/*----------------------------------------------------------------------------*/
/* repair the port                                                            */
/*----------------------------------------------------------------------------*/
void Port::vRepair(Object* pObject)
{
  _static_

  SysCall oSysCall;
  
  Port* pPort = _pPort(pObject);

  if (pPort->bIsOpen())
    pPort->bClose(oSysCall);
  
  // delete the rowOnce
  delete pPort;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* accept a socket connection request from a client                           */
/*----------------------------------------------------------------------------*/
bool Port::bAcceptClient(int& iSocketId, SysCall& roSysCall)
{
  _precondition_(_MAGIC_,_FOOTER_);
  roSysCall.vClear();

  socklen_t iSize = sizeof(struct sockaddr_in);
  
#ifdef WITH_STATISTICS_COUNTERS
  gpAttachMap->m_oStatistics.Port.m_nAccepts++;
#endif
  
  iSocketId = accept(m_iSocketFd
                         ,(struct sockaddr*)&m_xClientAddr
                         ,&iSize
                         );
  if (C_UNKNOWN == iSocketId) {
    roSysCall.vError(errno);
    return false;
  }

  s256 sServerAddr;
  s256 sClientAddr;

  if (nullptr == inet_ntop(this->m_xServerAddr.sin_family,
                           &(this->m_xServerAddr.sin_addr),
                           sServerAddr.azChar(),
                           sServerAddr.nAreaBytes())) {
    roSysCall.vError(errno);
    close(iSocketId);
    iSocketId = C_UNKNOWN;
    return false;
  }

  if (nullptr == inet_ntop(this->m_xClientAddr.sin_family,
                           &(this->m_xClientAddr.sin_addr),
                           sClientAddr.azChar(),
                           sClientAddr.nAreaBytes())) {
    roSysCall.vError(errno);
    close(iSocketId);
    iSocketId = C_UNKNOWN;
    return false;
  }

  vTrace("Client connected on %s:%d from %s:%d\n"_Stz,
         sServerAddr.szStz(),
         ntohs(this->m_xServerAddr.sin_port),
         sClientAddr.szStz(),
         ntohs(this->m_xClientAddr.sin_port));
  
  if (AM::bAttached()) AM::pResources()->vIncSocketOpen();

//  // Turn off socket address reuse on the accepted socket
//  int iOpt = 0;
//  int iResult = setsockopt(m_iSocketFd
//      ,SOL_SOCKET     // setting socket-level option
//      ,SO_REUSEADDR
//      ,(void*)&iOpt
//      ,sizeof(iOpt)
//  );
//
//  if (C_UNKNOWN == iResult) {
//    roSysCall.vError(errno);
//    close(iSocketId);
//    iSocketId = C_UNKNOWN;
//    return false;
//  }
  
  roSysCall.vSuccess();
  return true;
}

/*----------------------------------------------------------------------------*/
/* initialize a sockaddr_in structure                                         */
/*----------------------------------------------------------------------------*/
void Port::vAddrInit(sockaddr_in* aAddr, int iPort)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  MemLib::vZero(aAddr,sizeof(sockaddr_in),CN_PORT);
  
  aAddr->sin_family      = AF_INET;
  aAddr->sin_addr.s_addr = htonl(INADDR_ANY);
  aAddr->sin_port        = htons((int)iPort);
}

/*----------------------------------------------------------------------------*/
/* initialize a sockaddr_in structure                                         */
/*----------------------------------------------------------------------------*/
void Port::vAddrInit(sockaddr_in* aAddr, Stz* sAddr, int iPort)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  MemLib::vZero(aAddr,sizeof(sockaddr_in),CN_PORT);
  
  aAddr->sin_family      = AF_INET;
  aAddr->sin_addr.s_addr = sAddr == nullptr ? C_UNKNOWN : inet_addr(sAddr->azChar());
  aAddr->sin_port        = htons((int)iPort);
}

/*----------------------------------------------------------------------------*/
/* get nBytesRcvd                                                             */
/*----------------------------------------------------------------------------*/
i8 Port::nBytesRcvd(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return m_nBytesRcvd;
}

/*----------------------------------------------------------------------------*/
/* get nBytesSent                                                             */
/*----------------------------------------------------------------------------*/
i8 Port::nBytesSent(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return m_nBytesSent;
}

/*----------------------------------------------------------------------------*/
/* close a socket connection (server or client)                               */
/*----------------------------------------------------------------------------*/
bool Port::bClose(SysCall& roSysCall)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  roSysCall.vClear();
  
  int iError = shutdown(m_iSocketFd, SHUT_RDWR);
  
  if (iError && ENOTCONN != errno && EBADF != errno) {
    roSysCall.vError(errno);
    return false;
  }

#ifdef WITH_STATISTICS_COUNTERS
  gpAttachMap->m_oStatistics.Port.m_nCloses++;
#endif
  
  iError = close(m_iSocketFd);
  if (iError && EBADF != errno) {   // cater for closing a previously closed fd
    roSysCall.vError(errno);
    return false;
  }
  
  m_iSocketFd = C_UNKNOWN;
  
  if (AM::bAttached()) AM::pResources()->vDecSocketOpen();
  
  roSysCall.vSuccess();
  return true;
}

/*----------------------------------------------------------------------------*/
/* connect this client with nominated server                                  */
/*----------------------------------------------------------------------------*/
bool Port::bConnectServer(Stz* sAddr, int iPort, SysCall& roSysCall)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  int iResult;
  int iOpt = 1;
  
  roSysCall.vClear();
  
  if (C_UNKNOWN == m_iSocketFd) {
    m_iSocketFd = socket(AF_INET,SOCK_STREAM,0);

    if (m_iSocketFd < 0) {
      roSysCall.vError(errno);
      return false;
    }
    
    iResult = setsockopt(m_iSocketFd
                         ,SOL_SOCKET
                         ,SO_REUSEADDR
                         ,(void*)&iOpt
                         ,sizeof(iOpt)
                         );
    if (C_UNKNOWN == iResult) {
      roSysCall.vError(errno);
      bClose(roSysCall);
      return false;
    }
  }
  
  struct addrinfo  xAddrInfo;
  struct addrinfo* pAddrInfo = NULL;
  
  MemLib::vZero(&xAddrInfo,sizeof(xAddrInfo),CN_PORT);
  
  xAddrInfo.ai_family   = AF_INET;
  xAddrInfo.ai_socktype = SOCK_STREAM;
  xAddrInfo.ai_protocol = IPPROTO_TCP;
  xAddrInfo.ai_flags    = AI_PASSIVE;
  
  s256 sPort;
  sPort.vSprint("%lld"_Stz, iPort);
  
  i8 iReturn = getaddrinfo(sAddr->azChar(), sPort.azChar(), &xAddrInfo, &pAddrInfo);
  if (iReturn != 0) {
    vTrace("getaddrinfo: error %lld"_Stz, iReturn);
  }

#ifdef WITH_STATISTICS_COUNTERS
  gpAttachMap->m_oStatistics.Port.m_nConnects++;
#endif
    
  iResult = connect(m_iSocketFd,(struct sockaddr*)pAddrInfo->ai_addr,sizeof(struct sockaddr));
  
  if (iResult != 0) {
    roSysCall.vError(errno);
    close(m_iSocketFd);
    m_iSocketFd = C_UNKNOWN;
    return false;
  }
  
  if (AM::bAttached()) AM::pResources()->vIncSocketOpen();
  
  roSysCall.vSuccess();
  return true;
}

/*----------------------------------------------------------------------------*/
/* copy a socket                                                              */
/*----------------------------------------------------------------------------*/
void Port::vCopy(Port* pSource)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  m_iSocketFd = pSource->m_iSocketFd;
  m_bBlocking = pSource->m_bBlocking;
}


/*----------------------------------------------------------------------------*/
/* create a UDP socket endpoint                                               */
/*----------------------------------------------------------------------------*/
bool Port::bCreateFdForSocket(SysCall& roSysCall)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  roSysCall.vClear();
  
  //m_iSocketFd = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
  m_iSocketFd = socket(AF_INET,SOCK_STREAM,0);

  if (m_iSocketFd < 0) {
    roSysCall.vError(errno);
    return false;
  }
  
  roSysCall.vSuccess();
  return true;
}

/*----------------------------------------------------------------------------*/
/* create a listening socket/port on this server                              */
/*----------------------------------------------------------------------------*/
bool Port::bCreateListeningPort(int i4PortNo, SysCall& roSysCall)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  int iResult;
  int iOpt = 1;

  roSysCall.vClear();
  
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
  
  vAddrInit(&m_xServerAddr, i4PortNo);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create a socket end-point
  
  m_iSocketFd = socket(AF_INET
                      ,SOCK_STREAM
                      ,6  // tcp
                      );
  
  if (C_UNKNOWN == m_iSocketFd) {
    roSysCall.vError(errno);
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
    roSysCall.vError(errno);
    return false;
  }

  // 2b) bind a name-address to this (listening) socket
  //     this prohibits the same actual socket being used by two processes
  //
  // refer: https://hea-www.harvard.edu/~fine/Tech/addrinuse.html

  iResult = bind(m_iSocketFd
                 ,(struct sockaddr*)&(m_xServerAddr)
                 ,sizeof(m_xServerAddr)
                 );

  if (iResult != 0) {
    roSysCall.vError(errno);
    return false;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 3) allow socket to listen for new connections
  
  iResult = listen(m_iSocketFd
                   ,SOMAXCONN
                   );
  
  if (iResult != 0) {
    roSysCall.vError(errno);
    return false;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
#ifdef WITH_STATISTICS_COUNTERS
  gpAttachMap->m_oStatistics.Port.m_nListens++;
#endif
  
  if (AM::bAttached()) AM::pResources()->vIncSocketOpen();
  
  roSysCall.vSuccess();
  return true;
}

/*----------------------------------------------------------------------------*/
/* drop lock for write                                                        */
/*----------------------------------------------------------------------------*/
void Port::vDropWrite(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  m_okLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* grab lock for write                                                        */
/*----------------------------------------------------------------------------*/
void Port::vGrabWrite(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
   
  m_okLock.vGrabWrite();
}

/*----------------------------------------------------------------------------*/
/* is socket open?                                                            */
/*----------------------------------------------------------------------------*/
bool Port::bIsOpen(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  if (C_UNKNOWN != m_iSocketFd)
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* set receive buffer size                                                    */
/*----------------------------------------------------------------------------*/
bool Port::bRecvBufferSize(int nBytes, SysCall& roSysCall)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  int iResult;
  int iOpt = nBytes;
  
  roSysCall.vClear();
  
  iResult = setsockopt(m_iSocketFd
                       ,SOL_SOCKET
                       ,SO_RCVBUF
                       ,(void*)&iOpt
                       ,sizeof(iOpt)
                       );
  
  if (C_UNKNOWN == iResult) {
    roSysCall.vError(errno);
    return false;
  }
  
  roSysCall.vSuccess();
  return true;
}

/*----------------------------------------------------------------------------*/
/* get receive buffer size                                                    */
/*----------------------------------------------------------------------------*/
i8 Port::iRecvBufferSize(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  int iResult;
  int iOptValue;
  int iOptSize = sizeof(iOptValue);

  iResult = getsockopt(m_iSocketFd
                       ,SOL_SOCKET
                       ,SO_RCVBUF
                       ,(void*)&iOptValue
                       ,(socklen_t *)&iOptSize
                       );
  UNUSED(iResult);

  return iOptValue;
}

/*----------------------------------------------------------------------------*/
/* get nRecvs                                                                 */
/*----------------------------------------------------------------------------*/
i8 Port::nRecvs(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return m_nRecvs;
}

/*----------------------------------------------------------------------------*/
/* reset                                                                      */
/*----------------------------------------------------------------------------*/
void Port::vReset(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  m_nSends = 0;
  m_nBytesSent = 0;
  
  m_nRecvs = 0;
  m_nBytesRcvd = 0;
}

/*----------------------------------------------------------------------------*/
/* set send buffer size                                                       */
/*----------------------------------------------------------------------------*/
bool Port::bSendBufferSize(int nBytes, SysCall& roSysCall)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  int iResult;
  int iOpt = nBytes;
  
  roSysCall.vClear();
  
  iResult = setsockopt(m_iSocketFd
                       ,SOL_SOCKET
                       ,SO_SNDBUF
                       ,(void*)&iOpt
                       ,sizeof(iOpt)
                       );
  
  if (C_UNKNOWN == iResult) {
    roSysCall.vError(errno);
    return false;
  }
  
  roSysCall.vSuccess();
  return true;
}

/*----------------------------------------------------------------------------*/
/* get send buffer size                                                       */
/*----------------------------------------------------------------------------*/
i8 Port::iSendBufferSize(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  int iResult;
  int iOptValue;
  int iOptSize = sizeof(iOptValue);

  iResult = getsockopt(m_iSocketFd
                       ,SOL_SOCKET
                       ,SO_SNDBUF
                       ,(void*)&iOptValue
                       ,(socklen_t *)&iOptSize
                       );
  UNUSED(iResult);

  return iOptValue;
}

/*----------------------------------------------------------------------------*/
/* get nSends                                                                 */
/*----------------------------------------------------------------------------*/
i8 Port::nSends(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return m_nSends;
}

/*----------------------------------------------------------------------------*/
/* set socket to be non-blocking                                              */
/*----------------------------------------------------------------------------*/
bool Port::bSetNonBlocking(SysCall& roSysCall)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  roSysCall.vClear();
  
  // get current socket flags
  int iFlags = fcntl(m_iSocketFd,F_GETFL,0);
  if (C_UNKNOWN == iFlags) {
    roSysCall.vError(errno);
    return false;

  }
  
  // set non-blocking
  int iResult = fcntl(m_iSocketFd,F_SETFL,iFlags | O_NONBLOCK);
  if (C_UNKNOWN == iResult) {
    roSysCall.vError(errno);
    return false;

  }
  
  // record socket as non-blocking
  m_bBlocking = true;                                                  //! fix
  
  roSysCall.vSuccess();
  return true;
}


/*----------------------------------------------------------------------------*/
/* get success flag                                                           */
/*----------------------------------------------------------------------------*/
bool Port::bSuccess(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return m_bSuccess;
}

/*----------------------------------------------------------------------------*/
/* set success flag                                                           */
/*----------------------------------------------------------------------------*/
void Port::vSuccess(bool bSuccess)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  m_bSuccess = bSuccess;
}

/*----------------------------------------------------------------------------*/
/* test if there is data available?                                           */
/*----------------------------------------------------------------------------*/
bool Port::bTestForData(SysCall& roSysCall)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  ASSERT(m_bBlocking); // logic check                                  //! fix
  
  roSysCall.vClear();
  
  char cc;
  
#ifdef WITH_STATISTICS_COUNTERS
  gpAttachMap->m_oStatistics.Port.m_nPeeks++;
#endif
  
  i8 nRecv = recv(m_iSocketFd,&cc,1,MSG_PEEK);
  
  if (C_UNKNOWN == nRecv && EAGAIN == errno) {
    return false;
  }
  
  if (C_UNKNOWN == nRecv) {
    roSysCall.vError(errno);
    return false;
  }
  
  //! what should nRecv be at this point?
  
  roSysCall.vSuccess();
  return true;
}

/*----------------------------------------------------------------------------*/
/* get this sockets totup                                                     */
/*----------------------------------------------------------------------------*/
Totup* Port::pTotup(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return m_pTotup;
}

/*----------------------------------------------------------------------------*/
/* get this sockets why totup                                                 */
/*----------------------------------------------------------------------------*/
Totup* Port::pWhyTot(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return m_pWhyTot;
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* recv n bytes from this socket into given buffer                            */
/*----------------------------------------------------------------------------*/
bool Port::bBytesRecv(void* aBytes, i8 nBytes, SysCall& roSysCall)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  int nSoFar = 0;
  i8 nMore = nBytes;
  char* pSoFar = (char*)aBytes;
  
  roSysCall.vClear();
  
  if (nBytes == 0) {
    vTrace("bRecvBytes() asked to recv 0 bytes\n"_Stz);
    RASSERT(nBytes == 0);
  }
  
  while (nSoFar != nBytes)
  {
    pSoFar = (char*)aBytes + nSoFar;
 
#ifdef WITH_STATISTICS_COUNTERS
    gpAttachMap->m_oStatistics.Port.m_nRecvs++;
#endif
    
    i8 nRecvBytes = recv(m_iSocketFd,pSoFar,nMore,0);
    
    if (C_UNKNOWN == nRecvBytes) {
      roSysCall.vError(errno);
      //vTrace("ERROR1: %lld\n"_Stz, errno);
      return false;
    }
    
    if (0 == nRecvBytes) {
      // vTrace("Received 0 bytes; errno %lld\n"_Stz, errno);
      roSysCall.vError(errno); // don't die after this, node can come and goes
      //vTrace("ERROR2: %lld\n"_Stz, errno);
      return false;
    }

    nSoFar += nRecvBytes;
    nMore -= nRecvBytes;
    
#ifdef WITH_STATISTICS_COUNTERS
    gpAttachMap->m_oStatistics.Port.m_nRecvBytes += nRecvBytes;
#endif
    
    //vTrace("Bytes received: (%lld) %lld\n"_Stz, nBytes, nSoFar);
  }
  
  m_nRecvs++;
  m_nBytesRcvd += nBytes;

  roSysCall.vSuccess();
  return true;
}

/*----------------------------------------------------------------------------*/
/* send n bytes via this socket                                               */
/*----------------------------------------------------------------------------*/
bool Port::bBytesSend(void* aBytes, i8 nBytes, SysCall& roSysCall)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  roSysCall.vClear();
  
  if (nBytes == 0) {
    vTrace("sendto() sending 0 bytes\n"_Stz);
    RASSERT(nBytes == 0);
  }

#ifdef WITH_STATISTICS_COUNTERS
  gpAttachMap->m_oStatistics.Port.m_nSendTos++;
#endif
    
  i8 nSentBytes = sendto(m_iSocketFd
                         ,aBytes
                         ,nBytes
                         ,0
                         ,(struct sockaddr*)&m_xServerAddr
                         ,sizeof(m_xServerAddr)
                         );
  
  if (C_UNKNOWN == nSentBytes)
  {
    roSysCall.vError(errno);
    return false;
  }
  
  if (0 == nSentBytes)
  {
    roSysCall.vError(errno);
    return false;
  }

#ifdef WITH_STATISTICS_COUNTERS
  gpAttachMap->m_oStatistics.Port.m_nSentBytes += nSentBytes;
#endif
    
  if (nSentBytes < nBytes)
  {
    vTrace("sendto() less bytes sent; %lld v %lld\n"_Stz
      ,nSentBytes
      ,nBytes
      );
    roSysCall.vError(errno);
    return false;
  }
  
  m_nSends++;
  m_nBytesSent += nBytes;

  roSysCall.vSuccess();
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
