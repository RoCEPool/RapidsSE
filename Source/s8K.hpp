/*============================================================================*/
/* s8K.hpp -- an 8K-byte string                                               */
/*============================================================================*/
#pragma once

#include "Magic.h"

namespace Magic
{
  class s8K : public s
  {
  private:
    static const i8 C_S_SIZE = C_8KB;
    
    char m_mWorkspace[C_S_SIZE]; // string workspace
    
    // must be last
    Footer m_xFooter;
    
  public:
    s8K(void);
    s8K(s8K& rArg);
    s8K(const char* aArg);
    s8K(const char* aArg, i8 nChars);
   ~s8K(void);

    void operator = (const char* aArg);
    void operator = (char* aArg);
    void operator = (s& rArg);
    
    Footer* pFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
