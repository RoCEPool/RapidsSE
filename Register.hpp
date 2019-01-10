/*============================================================================*/
/* Register.hpp -- Forth Register                                             */
/*============================================================================*/
#pragma once

namespace Forth
{
  class Register : public Value
  {
    static const i8 C_VALREG_WORKSPACE = C_8KB;
    
  private:
    char m_wWorkspace[C_VALREG_WORKSPACE];
    
  public:
    void* operator new(size_t);
    Register(void);
    
    void operator =(i8 iInt);
    void operator =(Object* pPtr);
  };
  
  Register* _pRegister(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
