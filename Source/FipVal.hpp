/*============================================================================*/
/* FipVal.hpp - Forth Instruction Pointer Value                               */
/*============================================================================*/
#pragma once

namespace Forth
{
  class Cell;
}

namespace Values
{
  class FipVal : public Value
  {
  private:
    Forth::Cell* m_aCell;
        
  public:
    void* operator new(size_t);
    
    FipVal(void);
    
    void operator =(Forth::Cell* aCell);
    
    i8      nSize(void) const;
    Forth::Cell* aIpPtr(void) const;
    
    void vToString(Stz* aWorkspace);
  };
  
  FipVal* _aFipVal(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
