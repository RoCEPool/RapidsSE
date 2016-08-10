/*============================================================================*/
/* s32.hpp -- a 32 byte s-string                                              */
/*============================================================================*/
#pragma once

#include "Common.h"

namespace Common
{
  class s32 : public s
  {
  private:
    static const i8 C_S_SIZE = 32;
    
    char m_mWorkspace[C_S_SIZE]; // string workspace 
    
  public:
    s32(void);
    s32(s32& rArg);
    s32(const char* aArg);
    s32(const char* aArg, i8 nChars);
   ~s32(void);
    
    void operator = (const char* aArg);
    void operator = (char* aArg);
    void operator = (s& rArg);
    
    void vInit(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
