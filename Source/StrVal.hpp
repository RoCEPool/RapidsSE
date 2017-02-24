/*============================================================================*/
/* StrVal.hpp -- string value                                                 */
/*============================================================================*/

#pragma once

#include "Values.h"

namespace Values
{
  class StrVal : public Value
  {
  protected:
    i8   m_nDataBytes; // size of the data area (may not be length of the string)
    // ..dataArea..
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, StrVal* pRefactor);
  
    StrVal(void);
    StrVal(eCN eClass);
    StrVal(i8 iChars);
    StrVal(eCN eClass, i8 iSize);
  
    ~StrVal(void);
  
    void operator = (const char* aArg);
    void operator = (char* aArg);
    void operator = (StrVal& rArg);
  
    void vStrValAssign(const char* sStr);
  
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
    bool  bIsEqual(StrVal& sArg, i8 iCase) ;
    i8    iLength(void);
  
  
  
    // from sFactor::
    i8   iChars(void) const;
    void vReset(void);
    i8   iSize(void) const;
    void vSize(void);
    void vSize(i8 iSize);
    char* sStr(void);
    
    void vToCommand(char* aWorkspace);
    void vToString(char* aWorkspace);
    
    void  vCopyString(char* aStr, i8 iLength);
    
    static i8 nBytesRequired(i8 iLength);
  
  
  
    Footer* pFooter(void);
  
  };

  StrVal* _psRefactor(void* pAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
