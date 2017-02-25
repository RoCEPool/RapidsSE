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
    void* operator new(size_t, StrVal* pThis);
  
    StrVal(eCN eClassNo);
    StrVal(StrVal* pThis);
    StrVal(StrVal* pThis, i8 iChars);
    StrVal(eCN eClass, i8 iSize);
  
    ~StrVal(void);
  
    void operator = (const char* aArg);
    void operator = (char* aArg);
    void operator = (StrVal& rArg);
  
  
    char* a(void);
    char* z(void);

  
  
    // from s::
    void  vAppend(const char c);
    void  vAppend(const char* aArg);
    void  vCopy(const char* aArg);
    void  vCopyString(char* aStr, i8 iLength);
    void  vStrValAssign(const char* aStr);
    void  vStrValAssign(const char* aStr, i8 iLength);
    bool  bEnds(const char* aSuffix);
    bool  bIsEqual(StrVal& sArg, i8 iCase) ;
    i8    iLength(void);
  
  
  
    // from sFactor::
    i8   iChars(void) const;
    i8   iSize(void) const;
    void vSize(void);
    char* aStr(void);
    
    void vToCommand(char* aWorkspace);
    void vToString(char* aWorkspace);
    
  
    static i8 nBytesRequired(i8 iLength);
  

    i8    nDataBytes(void);

  private:
    char* pDataArea(void);
    void  vDataBytes(void);
  
    Footer* pFooter(void);
  };

  StrVal* _pStrVal(void* pAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
