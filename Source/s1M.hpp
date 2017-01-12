/*============================================================================*/
/* s1M.hpp -- an 1M-byte string                                               */
/*============================================================================*/
#pragma once

#include "Magic.h"

namespace Magic
{
  class s1M : public s
  {
  private:
    static const i8 C_S_SIZE = C_1M;
    
    char m_mWorkspace[C_S_SIZE]; // string workspace
    
    // must be last
    Footer m_xFooter;
    
  public:
    s1M(void);
    s1M(s1M& rArg);
    s1M(const char* aArg);
    s1M(const char* aArg, i8 nChars);
   ~s1M(void);

    void operator = (const char* aArg);
    void operator = (char* aArg);
    void operator = (s& rArg);
    
    Footer* pFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
