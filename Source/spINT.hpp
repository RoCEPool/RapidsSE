/*============================================================================*/
/* spINT.hpp - Int sprint helper                                              */
/*============================================================================*/
#pragma once

namespace Transform
{
  class spINT : public Sprint
  {
  public:
  
    // must be last
    Footer m_xFooter;
    
  public:
    spINT(void);
    ~spINT(void) noexcept(false);
  
    Stz* sz(i8 iInt);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
