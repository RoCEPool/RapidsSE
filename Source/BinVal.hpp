/*============================================================================*/
/* BinVal.hpp - binary value                                                  */
/*============================================================================*/

#pragma once

#include "Values.h"

namespace Values
{
  class BinVal : public Value
  {
  private:
    i8   m_iBytes;
    char m_cArray[8];  // should be ..data..
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, BinVal* pBinVal);
    
    BinVal(void);
    BinVal(i8 iSize);
    
    void operator =(const Object* pObject);
    
    i8   iBytes(void) const;
    void vReset(void);
    i8   iSize(void) const;
    void vSize(void);
    void vSize(i8 iSize);
    Object* pObject(void);
    
    char* pData(void);
    
    void vCopyObject(Object* pObject, i8 iLength);
    
    static i8 nBytesRequired(i8 iLength);
  };
  
  BinVal* _pBinVal(void* pAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
