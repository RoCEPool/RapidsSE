/*============================================================================*/
/* UT_SkipList.hpp - SkipList Unit Test                                       */
/*============================================================================*/
#pragma once

#include "UT.hpp"

namespace BASE
{
  class UT_SkipList : public UT
  {
  public:

    // must be last
    Footer m_xFooter;
    
  public:
    UT_SkipList(void);
   ~UT_SkipList(void) noexcept(false);
 
    static bool bTest(void);
  
  private:
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
