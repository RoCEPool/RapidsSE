/*============================================================================*/
/* Birth.hpp - birth timestamp                                                */
/*============================================================================*/
#pragma once

namespace System
{
  class Birth : public Object
  {
  public:
    Dtm m_wTsp; // birth as tsp
    Dtm m_wIvl; // birth as Ivl since epoch
  
    // must be last
    Footer m_xFooter;
    
  public:
    Birth();
    ~Birth() noexcept(false);
    
    void vClear(void);
    void vNow(void);
    i8   iElapsedSsec(void);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
