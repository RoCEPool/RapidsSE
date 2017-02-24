/*============================================================================*/
/* s.hpp -- an s-string                                                       */
/*============================================================================*/
#pragma once

#include "Values.h"

namespace Values
{
  class s : public sRefactor
  {
  private:
    i8   m_nDataBytes; // size of the data area (may not be length of the string)
    // ..dataArea..
    
  public:
    void* operator new(size_t);
    
    s(eCN eClass, i8 iSize);
   ~s(void);
    
    void operator = (const char* aArg);
    void operator = (char* aArg);
    void operator = (s& rArg);
    
    void  vAppend(const char c);
    void  vAppend(const char* aArg);
    void  vCopy(const char* aArg);
    char* pDataArea(void);
    i8    nDataBytes(void);
    void  vDataBytes(void);
    bool  bEnds(const char* aSuffix);
    void  vInit(i8 iSize);
    bool  bIsEqual(s& sArg, i8 iCase) ;
    i8    iLength(void);
    char* a(void);
    char* z(void);
    
    Footer* pFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
