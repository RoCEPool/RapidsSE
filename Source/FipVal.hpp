/*============================================================================*/
/* FipVal.hpp - Forth Instruction Pointer StkVal                          */
/*============================================================================*/
#pragma once

namespace SenVM
{
  class FipVal : public StkVal
  {
  private:
    Cell* m_aFip;
        
  public:
    void* operator new(size_t);
    
    FipVal(void);
    
    void  operator =(Cell* aCell);
    Cell* aFipPtr(void);
  
    i8    nStackBytes(void);
    void  vToString(Stz* aWorkspace);
  };
  
  FipVal* _aFipVal(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
