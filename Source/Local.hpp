/*============================================================================*/
/* Local.hpp -- Forth Local                                                   */
/*============================================================================*/
#pragma once

namespace Forth
{
  class Local : public Object
  {
    static const i8 C_WSP_LOCAL = C_8KB;
    
  private:
    Wsp_t<C_WSP_LOCAL> m_xWsp; // workspace which has a nested value

    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    Local(void);
      
    ForthValue* aLocalValue(void);
  
  private:
    Footer* aFooter(void);
  };
  
  Local* _pLocal(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
