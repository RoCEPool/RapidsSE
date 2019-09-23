/*============================================================================*/
/* EnumSS.hpp -- enum string set                                              */
/*============================================================================*/
#pragma once

namespace Gadgets
{
  static const i8 C_ENM_PAIRS = 64;
  
  class EnumSS : public Object
  {
  public:
  
    struct {
      i8 m_iName;
      i8 m_iString;
    } m_xEnnPair[C_ENM_PAIRS];

    b4K  m_bStrings;
  
    // must be last
    Footer m_xFooter;
    
  public:
    EnumSS(void);
  
   ~EnumSS() noexcept(false);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
