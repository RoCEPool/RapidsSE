/*============================================================================*/
/* s256.hpp -- a 256-byte string                                              */
/*============================================================================*/
#pragma once

#include "Magic.h"

namespace Magic
{
  class s256 : public s
  {
  private:
    static const i8 C_S_SIZE = 256;
    
    char m_mWorkspace[C_S_SIZE]; // string workspace 
    
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
    void operator = (s& rArg);
    
    void vInit(void);
    
    Footer* pFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
