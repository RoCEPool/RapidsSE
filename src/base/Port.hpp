/*============================================================================*/
/* Port.hpp - tcp/ip sockets                                                */
/*============================================================================*/
#pragma once

#include <netdb.h> // sockaddr_in


#include "Resource.hpp"
#include "Totup.hpp"
#include "RejectJob.hpp"
#include "eSY.h"

namespace BASE
{
  class Port : public Resource
  {
  public:
    Lock    m_okLock;

    Object* m_pIsFor;
  
    int     m_iSocketFd; // the o/s socket
    bool    m_bBlocking; // is a blocking socket

    struct sockaddr_in m_xServerAddr;
    struct sockaddr_in m_xClientAddr;
  
    bool    m_bSuccess;  // success?
  
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
    
    Port(Object* pIsFor);
    Port(Object* pIsFor, int iSocketId);
    Port(Object* pIsFor, int iSocketId, Port* pFromPort);
   ~Port(void) noexcept(false);
  
    static void vRepair(Object* pObject);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

    bool bAcceptClient(int& pSocketId, SysCall& SysCall);
    void vAddrInit(sockaddr_in* aAddr, int iPort);
    void vAddrInit(sockaddr_in* aAddr, Stz* sAddr, int iPort);
    
    i8   nBytesRcvd(void);
    i8   nBytesSent(void);
  
    bool bClose(SysCall& SysCall);
    bool bConnectServer(Stz* sAddr, int iPort, SysCall& SysCall);
    void vCopy(Port* pSource);
    bool bCreateFdForSocket(SysCall& SysCall);
    bool bCreateListeningPort(int iPort, SysCall& SysCall);
  
    void vDropWrite(void);
  
    void vGrabWrite(void);
  
    bool bIsOpen(void);
  
    bool bRecvBufferSize(int nBytes, SysCall& SysCall);
    i8   iRecvBufferSize(void);
    i8   nRecvs(void);
    void vReset(void);
  
    bool bSendBufferSize(int nBytes, SysCall& SysCall);
    i8   iSendBufferSize(void);
    i8   nSends(void);
    bool bSetNonBlocking(SysCall& SysCall);
    bool bSuccess(void);
    void vSuccess(bool bSuccess);
  
    bool bTestForData(SysCall& SysCall);
    Totup* pTotup(void);
  
    Totup* pWhyTot(void);

  protected:
    bool bBytesRecv(void* pBuffer, i8 nBytes, SysCall& SysCall);
    bool bBytesSend(void* pBuffer, i8 nBytes, SysCall& SysCall);
  
    Footer* aFooter(void);
  };
  
  Port* _pPort(void* aAddr);

}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
