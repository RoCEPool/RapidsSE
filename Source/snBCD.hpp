/*============================================================================*/
/* snBCD.hpp - Bcd scan helper                                                */
/*============================================================================*/
#pragma once

namespace Sprinting
{
  class snBCD : public Sprint
  {
  public:
  
    // must be last
    Footer m_xFooter;
    
  public:
    snBCD(void);
    ~snBCD(void) noexcept(false);
  
    Stz* sz(i8 iBcd);
    Stz* sz(i8 iBcd, i8 iScale);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
