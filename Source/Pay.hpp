/*============================================================================*/
/* Pay.hpp -- optional payload part of an index entry                         */
/*============================================================================*/
#pragma once

namespace Entries
{
  class Pay
  {
  private:
    i8   m_nPayBytes; // size of element sequence (in bytes)
    byte m_yaByte[];  // payload bytes

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
    inline byte* ayaBytes(void)
    {
      return m_yaByte;
    }
  };
  
  Pay* _aPay(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
