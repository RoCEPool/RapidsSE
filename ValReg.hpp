/*============================================================================*/
/* ValReg.hpp -- Workspace for Register Values                                */
/*============================================================================*/
#pragma once

namespace Forth
{
  class ValReg : public Value
  {
    static const i8 C_VALREG_WORKSPACE = C_8KB;
    
  private:
    char m_wWorkspace[C_VALREG_WORKSPACE];
    
  public:
    void* operator new(size_t);
    ValReg(void);
    
    void operator =(i8 iInt);
    void operator =(Object* pPtr);
  };
  
  ValReg* _pValReg(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
