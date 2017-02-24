/*============================================================================*/
/* sRefactor.hpp -- temporary class for refactoring s                         */
/*============================================================================*/

#pragma once

#include "Values.h"

namespace Values
{
  class sRefactor : public Value
  {
  protected:
    i8   m_nDataBytes; // size of the data area (may not be length of the string)
    // ..dataArea..
  
  public:
    sRefactor(eCN eClass);
  
    char* pDataArea(void);
    i8    nDataBytes(void);
    void  vDataBytes(void);
  
    char* a(void);
    char* z(void);

  
  
    // from s::
    void  vAppend(const char c);
    void  vAppend(const char* aArg);
    void  vCopy(const char* aArg);
    bool  bEnds(const char* aSuffix);
    void  vInit(i8 iSize);
    bool  bIsEqual(sRefactor& sArg, i8 iCase) ;
    i8    iLength(void);
  
  
  
    Footer* pFooter(void);
  
  };

  StrVal* _pStrVal(void* pAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
