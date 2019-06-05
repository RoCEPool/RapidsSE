/*============================================================================*/
/* snINT.hpp - Int sprint helper                                              */
/*============================================================================*/
#pragma once

namespace Sprinting
{
  class snINT : public Sprint
  {
  public:
  
    // must be last
    Footer m_xFooter;
    
  public:
    snINT(void);
    ~snINT(void) noexcept(false);
  
    Stz* sz(i8 iInt);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
