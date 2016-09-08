/*============================================================================*/
/* s16K.hpp -- a 16K-byte string                                              */
/*============================================================================*/
#pragma once

#include "Magic.h"

namespace Magic
{
  class s16K : public s
  {
  private:
    static const i8 C_S_SIZE = 15*C_1KB;
    
    char m_mWorkspace[C_S_SIZE]; // string workspace
    
    // must be last
    Footer m_xFooter;
    
  public:
    s16K(void);
    s16K(s16K& rArg);
    s16K(const char* aArg);
    s16K(const char* aArg, i8 nChars);
   ~s16K(void);

    void operator = (const char* aArg);
    void operator = (char* aArg);
    void operator = (s& rArg);
    
    Footer* pFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
