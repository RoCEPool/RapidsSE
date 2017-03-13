/*============================================================================*/
/* IpVal.hpp - instruction pointer value                                      */
/*============================================================================*/
#pragma once

namespace Dictionaries
{
  class Cell;
}

namespace Values
{
  class IpVal : public Value
  {
  private:
    Dictionaries::Cell* m_pPtr;
        
  public:
    void* operator new(size_t);
    
    IpVal(void);
    
    void operator =(Dictionaries::Cell* pPtr);
    
    i8      nSize(void) const;
    Dictionaries::Cell* pIpPtr(void) const;
    
    void vToString(Str* aWorkspace);
  };
  
  IpVal* _pIpVal(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
