/*============================================================================*/
/* IntVal.hpp - integer value                                                 */
/*============================================================================*/
#pragma once

namespace Values
{
  class IntVal : public Value,
                 public Int
  {
  private:
    
  public:
    void* operator new(size_t);
    
    IntVal(void);
    
    void operator =(i8 iVal);
    
    i8 nSize(void) const;
    i8* aInt(void);
    i8 iInt(void) const;
    
    void vToString(Str* aWorkspace);
  };
  
  IntVal* _pIntVal(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
