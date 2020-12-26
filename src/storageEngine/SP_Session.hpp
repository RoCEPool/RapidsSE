/*============================================================================*/
/* SP_Session.hpp -- Session sprint helper                                    */
/*============================================================================*/
#pragma once

namespace Resources
{
  class SP_Session : public SP
  {
  public:

    // must be last
    Footer m_xFooter;
    
  public:
    SP_Session(void);
   ~SP_Session(void) noexcept(false);
  
    Stz* sz(Session* pSession);
    Stz* sz_log(Session* pSession);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
