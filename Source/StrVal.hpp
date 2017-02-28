/*============================================================================*/
/* StrVal.hpp -- string value                                                 */
/*============================================================================*/

#pragma once

#include "Values.h"

namespace Values
{
  class StrVal : public Value,
                 public Vsp
  {
  private:
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, StrVal* pThis);
  
    StrVal(eCN eClassNo);
    StrVal(StrVal* pThis);
    StrVal(StrVal* pThis, i8 nBytes);
    StrVal(eCN eClass, i8 nBytes);
  
    ~StrVal(void);
  
    static i8 nAreaRequired(i8 iLength);
  
    void operator =(const char* aArg);
    void operator =(char* aArg);
    void operator =(StrVal& rArg);
  
    void  vAppend(const char cChar);
    void  vAppend(const char* aStr);
    char* aArea(void);
    i8    nAreaSize(void);
    void  vAreaSize(void);
    void  vAssign(const char* aStr);
    void  vAssign(const char* aStr, i8 iLength);
    void  vCopy(const char* aStr);
    void  vCopy(const char* aStr, i8 iLength);
    bool  bEnds(const char* aSuffix);
    bool  bIsEqual(StrVal& sArg, i8 iCase);
    i8    iLength(void);
    i8    nSize(void);
    char* aStr(void);
    void  vToCommand(char* aStr);
    void  vToString(char* aStr);
  
  private:
  
    Footer* pFooter(void);
  };

  StrVal* _pStrVal(void* pAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
