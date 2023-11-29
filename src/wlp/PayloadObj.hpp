/*============================================================================*/
/* PayloadObj.hpp - A payload of bytes encapsulated in a proper object        */
/*============================================================================*/
#pragma once

namespace WLP
{  
  class PayloadObj : public Object
  {
  public:
    static const i8 C_MAX_PAYLOAD_BYTES = C_1M;

  private:
    i8   m_nPayloadBytes;
    char m_caPayload[C_MAX_PAYLOAD_BYTES];

    Footer m_xFooter;

   public:

    void* operator new(size_t);
    void  operator delete(void*);
    
    PayloadObj(void);
    ~PayloadObj(void) noexcept(false);
  
    char* pcBuf();
    i8 iPayloadSize();
    void vSetPayloadSize(i8 nBytes);
    void vReset();
    void vMsgType(s256& sBuf);
  
    Footer* aFooter(void);
  };
  
  PayloadObj* _pPayloadObj(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
