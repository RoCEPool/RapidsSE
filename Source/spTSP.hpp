/*============================================================================*/
/* spTSP.hpp - Tme sprint helper                                              */
/*============================================================================*/
#pragma once

namespace Transform
{
  class spTSP : public Sprint
  {
  public:
  
    // must be last
    Footer m_xFooter;
    
  public:
    spTSP(void);
    ~spTSP(void) noexcept(false);
  
    Stz* sz(i8 iTme);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
