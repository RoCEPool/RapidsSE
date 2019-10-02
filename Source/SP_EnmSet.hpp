/*============================================================================*/
/* SP_EnmSet.hpp - EnmSet sprint helper                         (on c stack)  */
/*============================================================================*/
#pragma once

namespace Gadgets
{
  class SP_EnmSet : public SP
  {
  public:

    // must be last
    Footer m_xFooter;
    
  public:
    SP_EnmSet(void);
   ~SP_EnmSet(void) noexcept(false);
  
    Stz* sz(EnmSet* pEnmSet);

  private:
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
