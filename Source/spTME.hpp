/*============================================================================*/
/* spTME.hpp - Tme sprint helper                                              */
/*============================================================================*/
#pragma once

namespace Transform
{
  class spTME : public Sprint
  {
  public:
  
    // must be last
    Footer m_xFooter;
    
  public:
    spTME(void);
    ~spTME(void) noexcept(false);
  
    Stz* sz(i8 iTme);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
