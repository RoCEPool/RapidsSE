/*============================================================================*/
/* IvlVal.hpp - interval value                                                */
/*============================================================================*/
#pragma once

namespace Values
{
  class IvlVal : public Value
  {
  private:
    i8 m_iIvl;
    
  public:
    void* operator new(size_t);
    
    IvlVal(void);
    
    void operator =(i8 iVal);
    void operator =(Ivl xIvl);
  
    i8 nSize(void) const;
    i8* aInt(void);
    i8 iIvl(void) const;
    
    void vToString(Str* aWorkspace);
  };
  
  IvlVal* _pIvlVal(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
