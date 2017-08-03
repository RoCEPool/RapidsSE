/*============================================================================*/
/* BinVal.hpp - binary value                                                  */
/*============================================================================*/
#pragma once

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
    
    void operator =(Object* pObject);
    
    i8   iBytes(void) const;
    char* pData(void);
    eCN  eObjectClass(void);
    void vReset(void);
    i8   nSize(void) const;
    void vSize(void);
    void vSize(i8 iSize);
    void vToString(Stz* aWorkspace);
        
    Object* pNewObject(void);
  
    static i8 nBytesRequired(i8 iLength);
  };
  
  BinVal* _pBinVal(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
