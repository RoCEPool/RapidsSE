/*============================================================================*/
/* Socket.hpp - tcp/ip sockets                                                */
/*============================================================================*/
#pragma once

static Stz* C_LOOPBACK_IP  = "127.0.0.1"_Stz;

namespace Resources
{
  class Socket : public Resource
  {
  public:
  
    Object* m_pIsFor;
  
    int     m_iSocketFd;
    bool    m_bBlocking;
    Lock    m_okLock;
  
    void*   m_aLowBuff; // low memory read area

    union {
      struct sockaddr_in m_xServAddr;
      i8 m__Padding[16];
    };
  
    // must be last
    Footer m_xFooter;
        
  public:
    void* operator new(size_t);
    void* operator new(size_t, Socket* pSocket);
    void  operator delete(void*);
    
    Socket(Object* pIsFor);
    Socket(Object* pIsFor, int iSocketId);
   ~Socket(void) noexcept(false);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    static void vMonitorPortrait(s48K& sPortrait);
    static void vStatistics(void);

    void vAddrInit(sockaddr_in* aAddr, int iPort);
    void vAddrInit(sockaddr_in* aAddr, Stz* sAddr, int iPort);
    
    void vCopy(Socket* pSource);
    bool bIsOpen();

    bool bAcceptClient(int& pSocketId, SysCall& SysCall);
    bool bClose(SysCall& SysCall);
    bool bConnectServer(Stz* sAddr, int iPort, SysCall& SysCall);
    bool bCreateServer(int iPort, SysCall& SysCall);
    void vDropWrite(void);
    void vFocalOutline(s4K& sOutline);
    void vGrabWrite(void);
    bool bRecvBuffer(int nBytes, SysCall& SysCall);
    bool bRecvBytes(void* pBuffer, i8 nBytes, SysCall& SysCall);
    bool bSendBytes(void* pBuffer, i8 nBytes, SysCall& SysCall);
    bool bSetNonBlocking(SysCall& SysCall);
    bool bTestForData(SysCall& SysCall);
    bool bWriteVch(Stz* pVch, SysCall& SysCall);
    
    bool bSocket(SysCall& SysCall);
    bool bBind(SysCall& SysCall);
    bool bBind(int iPort, SysCall& SysCall);
    bool bBind(Stz* aAddr, int iPort, SysCall& SysCall);
    bool bServer(Stz* sAddr, int iPort, SysCall& SysCall);
    bool bSendTo(void* pBuffer, i8 nBytes, SysCall& SysCall);
    bool bRecvFrom(void* pBuffer, i8 nBytes, i8* pRecv, SysCall& SysCall);

    Footer* aFooter(void);
  };
  
  Socket* _pSocket(void* aAddr);

}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
