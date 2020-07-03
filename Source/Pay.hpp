/*============================================================================*/
/* Pay.hpp -- optional payload part of an index entry                         */
/*============================================================================*/
#pragma once

namespace Entries
{
  class Pay
  {
  private:
    i8   m_nPayBytes; // size of payload (in bytes)
    // .. payload bytes
  
  public:
    void* operator new(size_t);
  
    Pay(void);
   ~Pay(void) noexcept(false);
  
    void vIncPayBytes(i8 nDeltaBytes);
    i8   nPayBytes(void);
    void vPayBytes(i8 nBytes);
    void vReset(void);

    /*------------------------------------------------------------------------*/
    /* get the payload bytes                                                  */
    /*------------------------------------------------------------------------*/
    inline byte* ayaPayBytes(void)
    {
      return aAdd<byte*>(this,sizeof(m_nPayBytes));
    }
  };
  
  Pay* _aPay(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
