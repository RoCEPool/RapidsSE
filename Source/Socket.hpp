/*============================================================================*/
/* Socket.hpp - tcp/ip sockets                                                */
/*============================================================================*/

#pragma once

#include "Tasks.h"

namespace Common
{
  class Socket : public Object
  {
  public:
    int  m_iSocketId;
    bool m_bBlocking;
    
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
    
    void vAddrInit(sockaddr_in* pAddr, int iPort);
    void vAddrInit(sockaddr_in* pAddr, const char* sAddr, int iPort);
    
    void vCopy(Socket* pSource);
    bool bIsOpen();

    bool bAcceptClient(int& pSocketId, SysCall& SysCall);
    bool bClose(SysCall& SysCall);
    
    bool bConnectServer(const char* sAddr, int iPort, SysCall& SysCall);
    bool bCreateServer(int iPort, SysCall& SysCall);
    bool bReadVch(char** ppChar, SysCall& SysCall);
    bool bRecvBuffer(int nBytes, SysCall& SysCall);
    bool bRecvBytes(int nBytes, void* pBuffer, SysCall& SysCall);
    bool bSendBytes(int nBytes, void* pBuffer, SysCall& SysCall);
    bool bSetNonBlocking(SysCall& SysCall);
    bool bTestForData(SysCall& SysCall);
    bool bWriteVch(char* pVch, SysCall& SysCall);
    
    bool bSocket(SysCall& SysCall);
    bool bBind(SysCall& SysCall);
    bool bBind(int iPort, SysCall& SysCall);
    bool bBind(const char* pAddr, int iPort, SysCall& SysCall);
    bool bServer(const char* sAddr, int iPort, SysCall& SysCall);
    bool bSendTo(int nBytes, void* pBuffer, SysCall& SysCall);
    bool bRecvFrom(int nBytes, void* pBuffer, i8* pRecv, SysCall& SysCall);

    Footer* pFooter(void);
  };
  
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/