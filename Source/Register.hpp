/*============================================================================*/
/* Register.hpp -- Forth Register                                             */
/*============================================================================*/
#pragma once

namespace Forth
{
  class Register : public Object
  {
    static const i8 C_WSP_REGISTER = C_8KB;
    
  private:
    Wsp_t<C_WSP_REGISTER> m_xWsp; // workspace which has a nested value

    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    Register(void);
      
    Value* aNestedValue(void);
  
  private:
    Footer* aFooter(void);
  };
  
  Register* _pRegister(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
