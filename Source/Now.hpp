/*============================================================================*/
/* Now.hpp - Now sprint helper                                                */
/*============================================================================*/
#pragma once

namespace Transform
{
  class Now : public Sprint
  {
  public:
    Tsp m_oTsp;

    // must be last
    Footer m_xFooter;
    
  public:
    Now(void);
    ~Now(void) noexcept(false);
  
    Stz* sz(void);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
