/*============================================================================*/
/* SP_Dic.hpp - Dic sprint helper                                             */
/*============================================================================*/
#pragma once

namespace BASE
{
  class SP_Dic : public SP
  {
  public:

    // must be last
    Footer m_xFooter;
    
  public:
    SP_Dic(void);
   ~SP_Dic(void) noexcept(false);
  
    Stz* sz(Dic* pDic);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
