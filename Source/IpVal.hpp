/*============================================================================*/
/* IpVal.hpp - instruction pointer value                                      */
/*============================================================================*/
#pragma once

namespace Forth
{
  class Cell;
}

namespace Values
{
  class IpVal : public Value
  {
  private:
    Forth::Cell* m_pPtr;
        
  public:
    void* operator new(size_t);
    
    IpVal(void);
    
    void operator =(Forth::Cell* pPtr);
    
    i8      nSize(void) const;
    Forth::Cell* pIpPtr(void) const;
    
    void vToString(Stz* aWorkspace);
  };
  
  IpVal* _pIpVal(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
