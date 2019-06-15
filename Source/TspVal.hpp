/*============================================================================*/
/* TspVal.hpp - integer value                                                 */
/*============================================================================*/
#pragma once

namespace Values
{
  class TspVal : public Value,
                 public Dtm
  {
  private:
    
  public:
    void* operator new(size_t);
    
    TspVal(void);
    
    void operator =(i8 iVal);
    void operator =(Tsp xTsp);
  
    i8 nSize(void);
    i8* aInt(void);
    i8 iTsp(void);
    
    void vToString(Stz* aWorkspace);
  
    void vMaxValue(void);
    void vMidValue(void);
    void vMinValue(void);
    void vNullValue(void);
  };
  
  TspVal* _aTspVal(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
