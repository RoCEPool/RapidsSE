/*============================================================================*/
/* BolVal.hpp - boolean value                                                 */
/*============================================================================*/
#pragma once

namespace Values
{
  class BolVal : public Value,
                 public Dtm
  {
  private:
  
  public:
    void* operator new(size_t);
    
    BolVal(void);
    
    void operator =(Bol xBol);
    void operator =(i8 iVal);
    
    i8 nSize(void) const;
    i8* aInt(void);
    i8 iBol(void) const;
    
    void vToString(Stz* aWorkspace);
  
    void vMaxValue(void);
    void vMidValue(void);
    void vMinValue(void);
    void vNullValue(void);
  };
  
  BolVal* _aBolVal(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
