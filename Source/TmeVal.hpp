/*============================================================================*/
/* TmeVal.hpp - time value                                                    */
/*============================================================================*/
#pragma once

namespace Values
{
  class TmeVal : public Value,
                 public Tme
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
    
    void vToString(Str* aWorkspace);
  };
  
  TmeVal* _pTmeVal(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
