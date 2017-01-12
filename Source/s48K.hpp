/*============================================================================*/
/* s48K.hpp -- an 8K-byte string                                               */
/*============================================================================*/
#pragma once

#include "Magic.h"

namespace Magic
{
  class s48K : public s
  {
  private:
    static const i8 C_S_SIZE = C_48K;
    
    char m_mWorkspace[C_S_SIZE]; // string workspace
    
    // must be last
    Footer m_xFooter;
    
  public:
    s48K(void);
    s48K(s48K& rArg);
    s48K(const char* aArg);
    s48K(const char* aArg, i8 nChars);
   ~s48K(void);

    void operator = (const char* aArg);
    void operator = (char* aArg);
    void operator = (s& rArg);
    
    Footer* pFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
