/*============================================================================*/
/* EnumSS.hpp -- enum string set                                              */
/*============================================================================*/
#pragma once

namespace Gadgets
{
  class EnumSS : public Object
  {
  public:
  
    s32 m_sName;
    i8  m_iValue;
  
    // must be last
    Footer m_xFooter;
    
  public:
    EnumSS(void);
    EnumSS(Stz* szName);
    EnumSS(Stz* szName, i8 iValue);
   ~EnumSS() noexcept(false);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
