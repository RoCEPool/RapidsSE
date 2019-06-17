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
    Forth::Cell* m_aFip;
        
  public:
    void* operator new(size_t);
    
    FipVal(void);
    
    void operator =(Forth::Cell* aCell);
    
    Forth::Cell* aFipPtr(void);
    i8   nSize(void);
    void vToString(Stz* aWorkspace);
  };
  
  FipVal* _aFipVal(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
