/*============================================================================*/
/* SP_Mutex.hpp -- Mutex sprint helper                                        */
/*============================================================================*/
#pragma once

namespace Resources
{
  class SP_Mutex : public SP
  {
  public:

    // must be last
    Footer m_xFooter;
    
  public:
    SP_Mutex(void);
   ~SP_Mutex(void) noexcept(false);
  
    Stz* sz(Mutex* pMutex);
    Stz* sz_log(Mutex* pMutex);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
