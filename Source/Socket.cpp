/*============================================================================*/
/* socket.cpp - tcp/ip sockets                                                */
/*============================================================================*/

#include "Tasks.h"

Atomic gzSocketOpenCount;

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Socket::operator new(size_t iSize)
{
  Socket* pSocket = nullptr;
  
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
void Socket::operator delete(void* pSocketAddress)
{
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Socket::Socket(void)
: Object(CN_SOCKET)
{
  m_iSocketId = -1;
  m_bBlocking = true;
  
  Memory::vCheckInitialised(this,sizeof(Socket));
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Socket::Socket(int iSocketId)
: Object(CN_SOCKET)
{
  m_iSocketId = iSocketId;
  m_bBlocking = true;
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Socket::~Socket()
{
  if (-1 != m_iSocketId)
  {
    vWarn("Socket %d not closed!\n", m_iSocketId);
  }
}

/*----------------------------------------------------------------------------*/
/* initialize a sockaddr_in structure                                         */
/*----------------------------------------------------------------------------*/
void Socket::vAddrInit(sockaddr_in* pAddr, int iPort)
{
  Memory::vZero(pAddr,sizeof(sockaddr_in));
  
  pAddr->sin_family      = AF_INET;
  pAddr->sin_addr.s_addr = htonl(INADDR_ANY);
  pAddr->sin_port        = htons((int)iPort);
}

/*----------------------------------------------------------------------------*/
/* initialize a sockaddr_in structure                                         */
/*----------------------------------------------------------------------------*/
void Socket::vAddrInit(sockaddr_in* pAddr, const char* sAddr, int iPort)
{
  Memory::vZero(pAddr,sizeof(sockaddr_in));
  
  pAddr->sin_family      = AF_INET;
  pAddr->sin_addr.s_addr = inet_addr(sAddr);
  pAddr->sin_port        = htons((int)iPort);
}

/*----------------------------------------------------------------------------*/
/* copy a socket                                                              */
/*----------------------------------------------------------------------------*/
void Socket::vCopy(Socket* pSource)
{
  m_iSocketId    = pSource->m_iSocketId;
  m_bBlocking = pSource->m_bBlocking;
}

/*----------------------------------------------------------------------------*/
/* is socket open?                                                            */
/*----------------------------------------------------------------------------*/
bool Socket::bIsOpen()
{
  if (-1 != m_iSocketId)
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* accept a socket connection request from a client                           */
/*----------------------------------------------------------------------------*/
bool Socket::bAcceptClient(int& iSocketId, SysCall& SysCall)
{
  SysCall.vClear();
  
  sockaddr_in xClientAddr;
  
  socklen_t iSize = sizeof(struct sockaddr_in);
  
  iSocketId = accept(m_iSocketId
                         ,(struct sockaddr*)&xClientAddr
                         ,&iSize
                         );
  if (-1 == iSocketId) {
    SysCall.vError(errno);
    return false;
  }
  
  ++gzSocketOpenCount;
  
  SysCall.vSuccess();
  return true;
}

/*----------------------------------------------------------------------------*/
/* close a socket connection (server or client)                               */
/*----------------------------------------------------------------------------*/
bool Socket::bClose(SysCall& SysCall)
{
  SysCall.vClear();
  
  int iError = shutdown(m_iSocketId, SHUT_RDWR);
  
  if (iError && ENOTCONN != errno) {
    SysCall.vError(errno);
    return false;
  }
  
  iError = close(m_iSocketId);
  if (iError) {
    SysCall.vError(errno);
    return false;
  }
  
  m_iSocketId = -1;
  
  --gzSocketOpenCount;
  
  SysCall.vSuccess();
  return true;
}

/*----------------------------------------------------------------------------*/
/* connect this client with nominated server                                  */
/*----------------------------------------------------------------------------*/
bool Socket::bConnectServer(const char* sAddr, int iPort, SysCall& SysCall)
{
  int iResult;
  int iOpt = 1;
  struct sockaddr_in xServerAddr;
  
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
    return false;
  }
  
  vAddrInit(&xServerAddr,sAddr,iPort);
  
  iResult = connect(m_iSocketId,(sockaddr*)&xServerAddr,sizeof(xServerAddr));
  
  if (iResult != 0) {
    SysCall.vError(errno);
    return false;
  }
  
  ++gzSocketOpenCount;

  SysCall.vSuccess();
  return true;
}

/*----------------------------------------------------------------------------*/
/* create a listening server socket on this server                            */
/*----------------------------------------------------------------------------*/
bool Socket::bCreateServer(int iPort, SysCall& SysCall)
{
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
  
  iResult = listen(m_iSocketId,4);
  if (iResult != 0) {
    SysCall.vError(errno);
    return false;
  }
  
  ++gzSocketOpenCount;

  SysCall.vSuccess();
  return true;
}

/*----------------------------------------------------------------------------*/
/* read string into a char buffer                                             */
/*----------------------------------------------------------------------------*/
bool Socket::bReadVch(char** ppChar, SysCall& SysCall)
{
  char aWorkspace[16_KB];
  
  SysCall.vClear();
  
  size_t nBytes = recv(m_iSocketId,aWorkspace,16_KB-1,0);
  
  if (!nBytes) {
    SysCall.vError(errno);
    return false;
  }
  
  *ppChar = String::wAllocate(aWorkspace);
  
  SysCall.vSuccess();
  return true;
}

/*----------------------------------------------------------------------------*/
/* recv n bytes from this socket into given buffer                            */
/*----------------------------------------------------------------------------*/
bool Socket::bRecvBytes(int nBytes, void* pWS, SysCall& SysCall)
{
  int nSoFar = 0;
  int nMore = nBytes;
  char* pSoFar = (char*)pWS;
  
  SysCall.vClear();
  
  while (nSoFar != nBytes)
  {
    pSoFar = (char*)pWS + nSoFar;
    
    i8 nRecv = recv(m_iSocketId,pSoFar,nMore,0);
    
    if (-1 == nRecv) {
      SysCall.vError(errno);
      return false;
    }
    
    if (0 == nRecv) {
      SysCall.vError(errno);
      return false;
    }
    
    nSoFar += nRecv;
    nMore -= nRecv;
    
  }
  
  SysCall.vSuccess();
  return true;
}

/*----------------------------------------------------------------------------*/
/* write n bytes to this socket from given buffer                             */
/*----------------------------------------------------------------------------*/
bool Socket::bSendBytes(int nBytes, void* pBuffer, SysCall& SysCall)
{
  SysCall.vClear();
  
  i8 nSent = send(m_iSocketId,pBuffer,nBytes,0);
  if (-1 == nSent) {
    SysCall.vError(errno);
    return false;
  }
  
  if (nSent == nBytes) {
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
  ASSERT(m_bBlocking); // logic check                                  //! fix
  
  SysCall.vClear();
  
  char cc;
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
bool Socket::bWriteVch(char* pChar, SysCall& SysCall)
{
  SysCall.vClear();
  
  i8 nBytes = String::iLength(pChar);
  
  i8 nSent = send(m_iSocketId, pChar, nBytes,0);
  
  if (nSent == nBytes) {
    SysCall.vSuccess();
    return true;
  }
  else {
    return false;
  }
}

///////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* create a UDP socket                                                        */
/*----------------------------------------------------------------------------*/
bool Socket::bSocket(SysCall& SysCall)
{
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
  SysCall.vClear();
  
  return bBind(0, SysCall);
}

/*----------------------------------------------------------------------------*/
/* identify (name) a UDP socket                                               */
/*----------------------------------------------------------------------------*/
bool Socket::bBind(int iPort, SysCall& SysCall)
{
  SysCall.vClear();
  
  struct sockaddr_in xAddr;
  
  vAddrInit(&xAddr,iPort);
  
  vTrace("sin_port %d; sin_addr.s_add %d\n"
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
bool Socket::bBind(const char* sAddr, int iPort, SysCall& SysCall)
{
  struct sockaddr_in xAddr;
  
  SysCall.vClear();
  
  vAddrInit(&xAddr,sAddr,iPort);
  
  vTrace("sin_port %d; sin_addr.s_add %d\n"
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
bool Socket::bServer(const char* sAddr, int iPort, SysCall& SysCall)
{
  //  struct hostent *hp;     /* host information */
  
  SysCall.vClear();
  
  vAddrInit(&m_xServAddr,sAddr,iPort);
  
  vTrace("sin_port %d; sin_addr.s_add %d\n"
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
bool Socket::bSendTo(int nBytes, void* pBuffer, SysCall& SysCall)
{
  SysCall.vClear();
  
  i8 iResult = sendto(m_iSocketId
                       ,pBuffer
                       ,nBytes
                       ,0
                       ,(struct sockaddr*)&m_xServAddr
                       ,sizeof(m_xServAddr)
                       );
  
  if (-1 == iResult)
  {
    SysCall.vError(errno);
    return false;
  }
  
  if (0 == iResult)
  {
    SysCall.vError(errno);
    return false;
  }
  
  if (iResult < nBytes)
  {
    vWarn("sendto() less bytes sent; %lld v %lld\n"
      ,iResult
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
bool Socket::bRecvFrom(int nBytes, void* pBuffer, i8* pRecv, SysCall& SysCall)
{
  struct sockaddr_in xRemoteAddr;
  socklen_t iRemoteLen = sizeof(xRemoteAddr);
  
  SysCall.vClear();
  
  i8 iResult = recvfrom(m_iSocketId
                         ,pBuffer
                         ,nBytes
                         ,0
                         ,(struct sockaddr *)&xRemoteAddr
                         ,&iRemoteLen
                         );
  
  if (-1 == iResult)
  {
    SysCall.vError(errno);
    return false;
  }
  
  *pRecv = iResult;
  
  SysCall.vSuccess();
  return true;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Socket::pFooter(void)
{
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/