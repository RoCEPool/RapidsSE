/*============================================================================*/
/* Socket.hpp - tcp/ip sockets                                                */
/*============================================================================*/
#pragma once

#include <netdb.h> // sockaddr_in

static Stz* C_LOOPBACK_IP  = "127.0.0.1"_Stz;

#include "Resource.hpp"

namespace BASE
{
  class Socket : public Resource
  {
  public:
    Lock    m_okLock;

    Object* m_pIsFor;
  
    int     m_iSocketFd; // the o/s socket
    bool    m_bBlocking; // is a blocking socket
    void*   m_aLowBuff;  // low memory read area

    union {
      struct sockaddr_in m_xServAddr;
      i8 m__Padding[16];
    };
  
    bool    m_bSuccess;  // success?
    Object* m_pReceivedObject; // object which was received

    i8      m_nSends;
    i8      m_nBytesSent;

    i8      m_nRecvs;
    i8      m_nBytesRcvd;

    // must be last
    Footer m_xFooter;
        
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Socket(Object* pIsFor);
    Socket(Object* pIsFor, int iSocketId);
   ~Socket(void) noexcept(false);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    static void vStatistics(void);

    void vAddrInit(sockaddr_in* aAddr, int iPort);
    void vAddrInit(sockaddr_in* aAddr, Stz* sAddr, int iPort);
    
    void vCopy(Socket* pSource);
    bool bIsOpen();

    Object* pRecvObject(void);
  
    Object* pReceivedObject(void);
    void vReceivedObject(Object* pReceivedObject);
  
    bool bSuccess(void);
    void vSuccess(bool bSuccess);

    bool bAcceptClient(int& pSocketId, SysCall& SysCall);
    bool bClose(SysCall& SysCall);
    bool bConnectServer(Stz* sAddr, int iPort, SysCall& SysCall);
    bool bCreateServer(int iPort, SysCall& SysCall);
    void vDropWrite(void);
    void vOutline(s4K& sOutline);
    void vGrabWrite(void);
    bool bRecvBytes(void* pBuffer, i8 nBytes, SysCall& SysCall);
    bool bSendBytes(void* pBuffer, i8 nBytes, SysCall& SysCall);
    bool bSetRecvBufferSize(int nBytes, SysCall& SysCall);
    bool bSetNonBlocking(SysCall& SysCall);
    bool bTestForData(SysCall& SysCall);
    
    bool bSocket(SysCall& SysCall);
    bool bBind(SysCall& SysCall);
    bool bBind(int iPort, SysCall& SysCall);
    bool bBind(Stz* aAddr, int iPort, SysCall& SysCall);
    bool bServer(Stz* sAddr, int iPort, SysCall& SysCall);
    bool bSendTo(void* pBuffer, i8 nBytes, SysCall& SysCall);
    bool bRecvFrom(void* pBuffer, i8 nBytes, i8* pRecv, SysCall& SysCall);

    i8   nSends(void);
    i8   nBytesSent(void);
    i8   nRecvs(void);
    i8   nBytesRcvd(void);

    Footer* aFooter(void);
  };
  
  Socket* _pSocket(void* aAddr);

}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
