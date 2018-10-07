/*============================================================================*/
/* Socket.hpp - tcp/ip sockets                                                */
/*============================================================================*/
#pragma once

namespace System
{
  class Socket : public Object
  {
  public:
    int   m_iSocketId;
    bool  m_bBlocking;
    Lock  m_xLock;
  
    void* m_aLowBuff; // low memory read area

    struct sockaddr_in m_xServAddr;
    
    // must be last
    Footer m_xFooter;
        
  public:
    void* operator new(size_t);
    void* operator new(size_t, Socket* pSocket);
    void  operator delete(void*);
    
    Socket();
    Socket(int iSocketId);
    ~Socket();
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

    void vAddrInit(sockaddr_in* aAddr, int iPort);
    void vAddrInit(sockaddr_in* aAddr, Stz* sAddr, int iPort);
    
    void vCopy(Socket* pSource);
    bool bIsOpen();

    bool bAcceptClient(int& pSocketId, SysCall& SysCall);
    bool bClose(SysCall& SysCall);
    
    bool bConnectServer(Stz* sAddr, int iPort, SysCall& SysCall);
    bool bCreateServer(int iPort, SysCall& SysCall);
    void vDropWrite(void);
    void vGrabWrite(void);
    bool bRecvBuffer(int nBytes, SysCall& SysCall);
    bool bRecvBytes(i8 nBytes, void* pBuffer, SysCall& SysCall);
    bool bSendBytes(int nBytes, void* pBuffer, SysCall& SysCall);
    bool bSetNonBlocking(SysCall& SysCall);
    bool bTestForData(SysCall& SysCall);
    bool bWriteVch(Stz* pVch, SysCall& SysCall);
    
    bool bSocket(SysCall& SysCall);
    bool bBind(SysCall& SysCall);
    bool bBind(int iPort, SysCall& SysCall);
    bool bBind(Stz* aAddr, int iPort, SysCall& SysCall);
    bool bServer(Stz* sAddr, int iPort, SysCall& SysCall);
    bool bSendTo(i8 nBytes, void* pBuffer, SysCall& SysCall);
    bool bRecvFrom(int nBytes, void* pBuffer, i8* pRecv, SysCall& SysCall);

    Footer* aFooter(void);
  };
  
  Socket* _pSocket(void* aAddr);

}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
