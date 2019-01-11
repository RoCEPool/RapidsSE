/*============================================================================*/
/* FltVal.hpp - float value                                                    */
/*============================================================================*/
#pragma once

namespace Values
{
  class FltVal : public Value,
                 public Flt
  {
  private:

  public:
    void* operator new(size_t);
    
    FltVal(void);
    
    void operator =(d8 dVal);
    
    i8 nSize(void) const;
    d8 dFlt(void) const;
    d8* aFlt(void);
  
    void vToString(Stz* aWorkspace);
  
    void vMaxValue(void);
    void vMidValue(void);
    void vMinValue(void);
    void vNullValue(void);
  };
  
  FltVal* _aFltVal(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
