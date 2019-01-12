/*============================================================================*/
/* Register.hpp -- Forth Register                                             */
/*============================================================================*/
#pragma once

namespace Forth
{
  class Register : public Value
  {
    static const i8 C_WSP_REGISTER = C_8KB;
    
  private:
    Wsp_t<C_WSP_REGISTER> m_xWsp; // workspace

    // must be last
    //Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    Register(void);
    
    void operator =(i8 iInt);
    void operator =(Object* pPtr);
  
  private:
    //Footer* aFooter(void);
  };
  
  Register* _pRegister(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
