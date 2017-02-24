/*============================================================================*/
/* s256.hpp -- a 256-byte string                                              */
/*============================================================================*/
#pragma once

#include "Values.h"

namespace Values
{
  class s256 : public StrVal
  {
  private:
    static const i8 C_S256_DATAAREA = 256;
    
    char m_mDataArea[C_S256_DATAAREA]; // string data area
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t, s256* pS256);
    
    s256(void);
    s256(s256& rArg);
    s256(const char* aArg);
    s256(const char* aArg, i8 nChars);
   ~s256(void);

    void operator = (const char* aArg);
    void operator = (char* aArg);
    void operator = (StrVal& rArg);
    
    void vInit(void);
    
    Footer* pFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
