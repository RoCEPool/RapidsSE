/*============================================================================*/
/* SP_Recurring.hpp - Recurring sprint helper                                 */
/*============================================================================*/
#pragma once

namespace BASE
{
  class SP_Recurring : public SPmedium
  {
  public:

    // must be last
    Footer m_xFooter;
    
  public:
    SP_Recurring(void);
   ~SP_Recurring(void) noexcept(false);
  
    Stz* sz(Task* pTask);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
