/*============================================================================*/
/* HeadsUp.hpp -- Heads-Up helper                                             */
/*============================================================================*/
#pragma once

namespace BASE
{
  class HeadsUp : public SPmedium
  {
  public:

    // must be last
    Footer m_xFooter;
    
  public:
    HeadsUp(void);
    HeadsUp(Stz* szFunc);
   ~HeadsUp(void) noexcept(false);
  
    Stz* sz();
  
    void vShow();

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
