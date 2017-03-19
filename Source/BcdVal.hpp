/*============================================================================*/
/* BcdVal.hpp - bcd value                                                     */
/*============================================================================*/
#pragma once

namespace Values
{
  class BcdVal : public Value
  {
  private:
    i8 m_iBcd;
    
  public:
    void* operator new(size_t);
    
    BcdVal(void);
    
    void operator =(i8 iVal);
    
    i8 nSize(void) const;
    i8 iBcd(void) const;
    
    void vToString(Str* aWorkspace);
  };
  
  BcdVal* _pBcdVal(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
