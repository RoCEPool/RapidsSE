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

namespace BASE
{
  class Socket : public Port
  {
  public:
  
    Object* m_pReceivedObject; // object which was received

    char    m_caObjectHeader[sizeof(Object) + 64];
    Header* m_aHeader;
    Object* m_pObject;
  
    RejectJob* m_pRejectJob;
    void*      m_aLowBuff;  // low memory read area
  
    // must be last
    Footer m_xFooter;
        
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Socket(Object* pIsFor);
    Socket(Object* pIsFor, int iSocketId);
   ~Socket(void) noexcept(false);
      
    Object* pReceivedObject(void);
    void    vReceivedObject(Object* pReceivedObject);
    Object* pRecvObject(void);

    bool bSendObject(Object* pObject, SysCall& SysCall, eSY eSendWhy);//=SY_NONE);

  private:
  
    bool    bHeaderRecv(void);
    Marker* pMarkerRecv(void);
    Object* pObjectRecv(eCN eClass);
    Object* pObjectRecv_lowmem(eCN eClass);

    Footer* aFooter(void);
  };
  
  Socket* _pSocket(void* aAddr);

}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
