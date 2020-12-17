/*============================================================================*/
/* SP_Thread.hpp - Thread sprint helper                                       */
/*============================================================================*/
#pragma once

namespace Tasking
{
  class SP_Thread : public SP
  {
  public:

    // must be last
    Footer m_xFooter;
    
  public:
    SP_Thread(void);
   ~SP_Thread(void) noexcept(false);
  
    Stz* sz(Thread* pThread);
    Stz* sz_log(Thread* pThread);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
