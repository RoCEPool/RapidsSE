/*============================================================================*/
/* Socket.hpp - tcp/ip sockets                                                */
/*============================================================================*/
#pragma once

#include <netdb.h> // sockaddr_in

static Stz* C_LOOPBACK_IP  = "127.0.0.1"_Stz;

#include "Resource.hpp"
#include "Totup.hpp"
#include "eSY.h"

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

    Totup*  m_pTotup;
    Totup*  m_pWhyTot;

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

    bool bAcceptClient(int& pSocketId, SysCall& SysCall);
    void vAddrInit(sockaddr_in* aAddr, int iPort);
    void vAddrInit(sockaddr_in* aAddr, Stz* sAddr, int iPort);
    
    i8   nBytesRcvd(void);
    i8   nBytesSent(void);
  
    bool bClose(SysCall& SysCall);
    bool bConnectServer(Stz* sAddr, int iPort, SysCall& SysCall);
    void vCopy(Socket* pSource);
    bool bCreateServer(int iPort, SysCall& SysCall);
  
    void vDropWrite(void);
  
    void vGrabWrite(void);
  
    bool bIsOpen(void);
  
    Object* pReceivedObject(void);
    void vReceivedObject(Object* pReceivedObject);
    bool bRecvBufferSize(int nBytes, SysCall& SysCall);
    i8   iRecvBufferSize(void);
    Object* pRecvObject(void);
    i8   nRecvs(void);
    void vReset(void);
  
    bool bSendBufferSize(int nBytes, SysCall& SysCall);
    i8   iSendBufferSize(void);
    bool bSendObject(Object* pObject, SysCall& SysCall, eSY eSendWhy=SY_NONE);
    i8   nSends(void);
    bool bSetNonBlocking(SysCall& SysCall);
    bool bSocket(SysCall& SysCall);
    bool bSuccess(void);
    void vSuccess(bool bSuccess);
  
    bool bTestForData(SysCall& SysCall);
    Totup* pTotup(void);
  
    Totup* pWhyTot(void);

  private:
    bool bRecvBytes(void* pBuffer, i8 nBytes, SysCall& SysCall);
    bool bSendBytes(void* pBuffer, i8 nBytes, SysCall& SysCall);
  
    Footer* aFooter(void);
  };
  
  Socket* _pSocket(void* aAddr);

}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
