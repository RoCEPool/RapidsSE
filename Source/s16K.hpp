/*============================================================================*/
/* s16K.hpp -- a 16K-byte string                                              */
/*============================================================================*/
#pragma once

#include "Common.h"

namespace Common
{
  class s16K : public s
  {
  private:
    static const i8 C_S_SIZE = 16*C_1KB;
    
    char m_mWorkspace[C_S_SIZE]; // string workspace
    
  public:
    s16K(void);
    s16K(s16K& rArg);
    s16K(const char* aArg);
    s16K(const char* aArg, i8 nChars);
   ~s16K(void);

    void operator = (const char* aArg);
    void operator = (char* aArg);
    void operator = (s& rArg);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
