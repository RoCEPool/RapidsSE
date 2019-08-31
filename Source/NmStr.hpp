/*============================================================================*/
/* NmStr.hpp -- name - str value pair                                         */
/*============================================================================*/
#pragma once

namespace Gadgets
{
  class NmStr : public Object
  {
  public:
  
    s32 m_sName;
    s4K m_sValue;
  
    // must be last
    Footer m_xFooter;
    
  public:
    NmStr(void);
    NmStr(Stz* szName);
    NmStr(Stz* szName, Stz* szValue);
   ~NmStr() noexcept(false);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
