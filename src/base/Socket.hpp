/*============================================================================*/
/* Socket.hpp - tcp/ip sockets                                                */
/*============================================================================*/
#pragma once

#include <netdb.h> // sockaddr_in


#include "Resource.hpp"
#include "Totup.hpp"
#include "RejectJob.hpp"
#include "eSY.h"
#include "Port.hpp"
#include "WlpHeader.hpp"
#include "PayloadObj.hpp"

namespace BASE
{
  class Socket : public Port
  {
  public:
  
    Object* m_pReceivedObject; // object which was received

    char    m_caObjectHeader[sizeof(Object) + 64];
    Header* m_aHeader;
    Object* m_pObject;
    WLP::WlpHeader* m_pWlpHeader;
  
    RejectJob* m_pRejectJob;
    void*      m_aLowBuff;  // low memory read area

    bool m_bNoClose; // Do not close this socket because its fd is being shared
  
    // must be last
    Footer m_xFooter;
        
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Socket(Object* pIsFor, int iSocketId, Port* pFromPort, bool bNoClose = false);
    Socket(Object* pIsFor, int iSocketId);
   ~Socket(void) noexcept(false);

    static void vRepair(Object* pObject);

    Object* pReceivedObject(void);
    void    vReceivedObject(Object* pReceivedObject);
    Object* pRecvObject(void);
    Object* pRecvWLMsg(eEC& eErrorCode, String* psErrStr);

    bool bSendObject(Object* pObject, SysCall& SysCall, eSY eSendWhy);//=SY_NONE);
    bool bSendWLMsg(WLP::PayloadObj* pPayloadObj, SysCall& SysCall, eSY eSendWhy);//=SY_NONE);

    void vClientAddr(s256& sBuf);

  private:
  
    bool    bHeaderRecv(void);
    bool    bWLHeaderRecv(void);
    void    vJobRecv_lowmem(Job* pJob);
    Marker* pMarkerRecv(void);
    Object* pObjectRecv(eCN eClass);
    Object* pObjectRecv_lowmem(eCN eClass);
    Object* pWLMsgRecv();
    Object* pWLMsgRecv_lowmem();

    Footer* aFooter(void);
  };
  
  Socket* _pSocket(void* aAddr);

}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
