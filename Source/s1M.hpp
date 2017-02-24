/*============================================================================*/
/* s1M.hpp -- an 1M-byte string                                               */
/*============================================================================*/
#pragma once

#include "Values.h"

namespace Values
{
  class s1M : public sRefactor
  {
  private:
    static const i8 C_S1M_DATAAREA = C_1M;
    
    char m_mDataArea[C_S1M_DATAAREA]; // string workspace
    
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
    void operator = (sRefactor& rArg);
    
    Footer* pFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
