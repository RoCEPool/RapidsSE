/*============================================================================*/
/* StrTpl.hpp -- fixed-length string template                                 */
/*============================================================================*/
#pragma once

#include "Values.h"
#include "Footer.hpp"

namespace Values
{
  template <class CLASS, int SIZE>
  class StrTpl : public StrVal
  {
  private:
    char m_mDataArea[SIZE]; // string data area
    
    // must be last
    Footer m_xFooter;
    
  public:    
    StrTpl(void);
    StrTpl(StrVal& xArg);
    StrTpl(const char* aArg);
    StrTpl(const char* aArg, i8 nChars);
   ~StrTpl(void);
    
    void operator = (const char* aArg);
    void operator = (char* aArg);
    void operator = (StrVal& xArg);
    
    void vInit(void);
    
    Footer* pFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
