/*============================================================================*/
/* NmVal.hpp - name-value pair                                                */
/*============================================================================*/
#pragma once

namespace System
{
  class NmVal : public Object
  {
  public:
  
    s32 m_sName;
    i8  m_iValue;
  
    // must be last
    Footer m_xFooter;
    
  public:
    NmVal(void);
    NmVal(Stz* szName);
    NmVal(Stz* szName, i8 iValue);
    ~NmVal() noexcept(false);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
