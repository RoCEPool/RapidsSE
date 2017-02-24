/*============================================================================*/
/* StrVal.hpp - string value                                                  */
/*============================================================================*/

#pragma once

#include "Values.h"

namespace Values
{
  class StrVal : public Value
  {
  private:
    i8   m_nDataBytes;
    char m_sStr[8];  // should be ..data..
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, StrVal* pStrVal);
    
    StrVal(void);
    StrVal(i8 iChars);
    
    void operator =(const char* sStr);
    
    i8   iChars(void) const;
    void vReset(void);
    i8   iSize(void) const;
    void vSize(void);
    void vSize(i8 iSize);
    char* sStr(void);
    
    char* pDataArea(void);

    void vToCommand(char* aWorkspace);
    void vToString(char* aWorkspace);
    
    void  vCopyString(char* aStr, i8 iLength);
    
    static i8 nBytesRequired(i8 iLength);
  };
  
  StrVal* _pStrVal(void* pAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
