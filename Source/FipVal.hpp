/*============================================================================*/
/* FipVal.hpp -- Layout of a Forth Instruction Pointer when on Stack          */
/*============================================================================*/
#pragma once

namespace SenVM
{
  class FipVal : public AnyVal
  {
  private:
        
  public:
    void* operator new(size_t);
    
    void  operator =(Cell* aFip);
    Cell* aFip(void);
  
    i8    nStackUsage(void);
    void  vToString(Stz* szDst);
  };
  
  FipVal* _aFipVal(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
