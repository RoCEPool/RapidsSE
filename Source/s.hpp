/*============================================================================*/
/* s.hpp -- an s-string                                                       */
/*============================================================================*/
#pragma once

#include "Magic.h"

namespace Magic
{
  class s : public Header
  {
  private:
    i8   m_iSize;  // fixed size of the workspace (not the length of the string)
    // ..data..
    
  public:
    void* operator new(size_t);
    
    s(eCN eClass, i8 iSize);
   ~s(void);
    
    void operator = (const char* aArg);
    void operator = (char* aArg);
    void operator = (s& rArg);
    
    void  vAppend(const char c);
    void  vAppend(const char* aArg);
    char* aWorkspace(void);
    void  vCopy(const char* aArg);
    bool  bEnds(const char* aSuffix);
    void  vInit(i8 iSize);
    bool  bIsEqual(s& sArg, i8 iCase) ;
    i8    iLength(void);
    i8    iSize(void);
    void  vSize(void);
    char* a(void);
    char* z(void);
    
    Footer* pFooter(void);
  };
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
