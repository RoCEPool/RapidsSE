/*============================================================================*/
/* SizedHeader.hpp -- Sized Header                                            */
/*============================================================================*/
#pragma once

namespace Resources
{
  class SizedHeader : public Header
  {
  protected:
    i8  m_nSizeBytes;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, SizedHeader*);
  
    SizedHeader(eCN eClassNo, i8 nSizeBytes);
    ~SizedHeader(void) noexcept(false);
  
    i8 nSizeBytes(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
