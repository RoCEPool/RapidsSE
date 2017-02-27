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
    i8   m_nAreaSize; // size of the data area 
    char m_cArea[8];  // should be ..data..
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, BinVal* pBinVal);
    
    BinVal(void);
    BinVal(i8 iSize);
    
    void operator =(const Object* pObject);
    
    i8   iBytes(void) const;
    char* pData(void);
    eCN  eObjectClass(void);
    void vReset(void);
    i8   nSize(void) const;
    void vSize(void);
    void vSize(i8 iSize);
    void vToString(char* aWorkspace);
        
    Object* pNewObject(void);
  
    static i8 nBytesRequired(i8 iLength);
  };
  
  BinVal* _pBinVal(void* pAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
