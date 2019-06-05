/*============================================================================*/
/* snINT.hpp - Int scan helper                                                */
/*============================================================================*/
#pragma once

namespace Sprinting
{
  class snINT : public Scan
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
