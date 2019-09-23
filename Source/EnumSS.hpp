/*============================================================================*/
/* EnumSS.hpp -- enum string set                                              */
/*============================================================================*/
#pragma once

namespace Gadgets
{
  class EnumSS : public Object
  {
  public:
  
    e_EG m_eEnumGroup;
  
    b4K  m_bStrings;
  
    // must be last
    Footer m_xFooter;
    
  public:
    EnumSS(e_EG eEnumGroup);
  
   ~EnumSS() noexcept(false);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
