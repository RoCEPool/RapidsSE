/*============================================================================*/
/* IvlVal.hpp - interval value                                                */
/*============================================================================*/
#pragma once

namespace Values
{
  class IvlVal : public Value,
                 public Dtm
  {
  private:
  
  public:
    void* operator new(size_t);
    
    IvlVal(void);
    
    void operator =(i8 iVal);
    void operator =(Ivl xIvl);
  
    i8 nSize(void);
    i8* aInt(void);
    i8 iIvl(void);
    
    void vToString(Stz* aWorkspace);
  
    void vMaxValue(void);
    void vMidValue(void);
    void vMinValue(void);
    void vNullValue(void);
  };
  
  IvlVal* _aIvlVal(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
