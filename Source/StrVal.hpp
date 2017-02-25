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
  
    void vStrValAssign(const char* sStr);
    void vStrValAssign(const char* sStr, i8 iLength);
  
    char* a(void);
    char* z(void);

  
  
    // from s::
    void  vAppend(const char c);
    void  vAppend(const char* aArg);
    void  vCopy(const char* aArg);
    bool  bEnds(const char* aSuffix);
    bool  bIsEqual(StrVal& sArg, i8 iCase) ;
    i8    iLength(void);
  
  
  
    // from sFactor::
    i8   iChars(void) const;
    void vReset(void);
    i8   iSize(void) const;
    void vSize(void);
    char* sStr(void);
    
    void vToCommand(char* aWorkspace);
    void vToString(char* aWorkspace);
    
    void  vCopyString(char* aStr, i8 iLength);
    
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
