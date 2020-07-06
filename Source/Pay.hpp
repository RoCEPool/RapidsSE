/*============================================================================*/
/* Pay.hpp -- optional payload part of an index entry                         */
/*============================================================================*/
#pragma once

namespace Entries
{
  class Pay
  {
  private:
    i8   m_nPaySize; // size of payload (in bytes)
    // .. payload bytes
  
  public:
    void* operator new(size_t);
  
    Pay(void);
   ~Pay(void) noexcept(false);
  
    void vIncPaySize(i8 nDelta);
    i8   nPaySize(void);
    void vPaySize(i8 nSize);
    void vReset(void);

    /*------------------------------------------------------------------------*/
    /* get the payload bytes                                                  */
    /*------------------------------------------------------------------------*/
    inline byte* ayaPayload(void)
    {
      return aAdd<byte*>(this,sizeof(m_nPaySize));
    }
  };
  
  Pay* _aPay(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
