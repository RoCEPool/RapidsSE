/*============================================================================*/
/* SP_Thread.hpp - Thread sprint helper                                       */
/*============================================================================*/
#pragma once

namespace Threads
{
  class SP_Thread : public SP
  {
  public:

    // must be last
    Footer m_xFooter;
    
  public:
    SP_Thread(void);
   ~SP_Thread(void) noexcept(false);
  
    Stz* sz(Task* pTask);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
