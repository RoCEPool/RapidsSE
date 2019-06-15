/*============================================================================*/
/* TmeVal.hpp - time value                                                    */
/*============================================================================*/
#pragma once

namespace Values
{
  class TmeVal : public Value,
                 public Dtm
  {
  private:
    
  public:
    void* operator new(size_t);
    
    TmeVal(void);
    
    void operator =(i8 iVal);
    void operator =(Tme xTme);
  
    i8 nSize(void);
    i8* aInt(void);
    i8 iTme(void);
    
    void vToString(Stz* aWorkspace);
  
    void vMaxValue(void);
    void vMidValue(void);
    void vMinValue(void);
    void vNullValue(void);
  };
  
  TmeVal* _aTmeVal(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
