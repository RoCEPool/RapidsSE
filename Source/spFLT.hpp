/*============================================================================*/
/* spFLT.hpp - Flt sprint helper                                              */
/*============================================================================*/
#pragma once

namespace Transform
{
  class spFLT : public Sprint
  {
  public:
  
    // must be last
    Footer m_xFooter;
    
  public:
    spFLT(void);
    ~spFLT(void) noexcept(false);
  
    Stz* sz(d8 dFlt);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
