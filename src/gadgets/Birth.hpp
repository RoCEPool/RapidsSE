/*============================================================================*/
/* Birth.hpp - birth timestamp                                                */
/*============================================================================*/
#pragma once

namespace Resources
{
  class Birth : public Object
  {
  public:
    Dtm m_wTsp; // birth as tsp
    Ivl m_wIvl; // birth as Ivl since epoch
  
    // must be last
    Footer m_xFooter;
    
  public:
    Birth(void);
   ~Birth(void) noexcept(false);
    
    void vClear(void);
    void vNow(void);
    i8   iElapsedSsec(void);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
