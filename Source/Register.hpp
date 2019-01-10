/*============================================================================*/
/* Register.hpp -- Forth Register                                             */
/*============================================================================*/
#pragma once

namespace Forth
{
  class Register : public Value
  {
    static const i8 C_REGISTER_WSP = C_8KB;
    
  private:
    WspFxd<C_REGISTER_WSP> m_xWsp; // workspace

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
