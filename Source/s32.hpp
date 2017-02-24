/*============================================================================*/
/* s32.hpp -- a 32 byte s-string                                              */
/*============================================================================*/
#pragma once

#include "Values.h"
#include "Footer.hpp"

namespace Values
{
  class s32 : public sRefactor
  {
  private:
    static const i8 C_S32_DATAAREA = 32;
    
    char m_mDataArea[C_S32_DATAAREA]; // string data area
    
    // must be last
    Footer m_xFooter;
    
  public:    
    s32(void);
    s32(s32& rArg);
    s32(const char* aArg);
    s32(const char* aArg, i8 nChars);
   ~s32(void);
    
    void operator = (const char* aArg);
    void operator = (char* aArg);
    void operator = (sRefactor& rArg);
    
    void vInit(void);
    
    Footer* pFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
