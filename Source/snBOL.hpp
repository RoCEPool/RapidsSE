/*============================================================================*/
/* snBOL.hpp - Bol scan helper                                                */
/*============================================================================*/
#pragma once

namespace Sprinting
{
  class snBOL : public Sprint
  {
  public:
  
    // must be last
    Footer m_xFooter;
    
  public:
    snBOL(void);
    ~snBOL(void) noexcept(false);
  
    Stz* sz(i8 iBol);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
