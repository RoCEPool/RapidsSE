/*============================================================================*/
/* spNOW.hpp - Now sprint helper                                              */
/*============================================================================*/
#pragma once

namespace Sprinting
{
  class spNOW : public Sprint
  {
  public:
    Tsp m_oTsp;

    // must be last
    Footer m_xFooter;
    
  public:
    spNOW(void);
    ~spNOW(void) noexcept(false);
  
    Stz* sz(void);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
