/*============================================================================*/
/* BcdVal.hpp - bcd value                                                     */
/*============================================================================*/
#pragma once

namespace Values
{
  class BcdVal : public Value,
                 public Dtm
  {
  private:
    
  public:
    void* operator new(size_t);
    
    BcdVal(void);
    
    void operator =(Bcd xBcd);
    void operator =(i8 iVal);
    
    i8 nSize(void) const;
    i8* aInt(void);
    i8 iBcd(void) const;
    
    void vToString(Stz* aWorkspace);
  
    void vMaxValue(void);
    void vMidValue(void);
    void vMinValue(void);
    void vNullValue(void);
  };
  
  BcdVal* _aBcdVal(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
