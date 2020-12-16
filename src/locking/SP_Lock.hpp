/*============================================================================*/
/* SP_Lock.hpp -- Lock sprint helper                                          */
/*============================================================================*/
#pragma once

namespace Resources
{
  class SP_Lock : public SP
  {
  public:

    // must be last
    Footer m_xFooter;
    
  public:
    SP_Lock(void);
   ~SP_Lock(void) noexcept(false);
  
    Stz* sz(Lock* pLock);
    Stz* sz_log(Lock* pLock);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
