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
    WspFxd<C_VALREG_WORKSPACE> m_xWsp; // workspace

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
