/*============================================================================*/
/* EnumSS.hpp -- enum string set                                              */
/*============================================================================*/
#pragma once

namespace Gadgets
{
  static const i8 C_ENO_PAIRS = 64;
  
  class EnumSS : public Object
  {
  public:
  
    e_EG m_eEnumGroup;
  
    struct {
      i8 m_iEnum;
      i8 m_iString;
    } m_xEnoPair[C_ENO_PAIRS];

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
