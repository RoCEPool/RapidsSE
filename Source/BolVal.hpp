/*============================================================================*/
/* BolVal.hpp - boolean value                                                 */
/*============================================================================*/
#pragma once

namespace Values
{
  class BolVal : public Value
  {
  private:
    i8 m_iBol;
    
  public:
    void* operator new(size_t);
    
    BolVal(void);
    
    void operator =(i8 iVal);
    
    i8 nSize(void) const;
    i8 iBol(void) const;
    
    void vToString(Str* aWorkspace);
  };
  
  BolVal* _pBolVal(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
