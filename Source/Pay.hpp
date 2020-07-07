/*============================================================================*/
/* Pay.hpp -- optional payload part of an index entry                         */
/*============================================================================*/
#pragma once

namespace Entries
{
  class Pay
  {
  private:
    i8   m_nPayloadBytes; // size of payload (in bytes)
    // .. payload
  
  public:
    void* operator new(size_t);
  
    Pay(void);
   ~Pay(void) noexcept(false);
  
    i8   nPayloadBytes(void);
    void vPayloadBytes(i8 nPayloadBytes);
    i8   nPaySize(void);
    void vReset(void);

    /*------------------------------------------------------------------------*/
    /* get the payload                                                        */
    /*------------------------------------------------------------------------*/
    inline byte* ayaPayload(void)
    {
      return aAdd<byte*>(this,sizeof(m_nPayloadBytes));
    }
  };
  
  Pay* _aPay(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
