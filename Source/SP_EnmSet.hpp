/*============================================================================*/
/* SP_EnmSet.hpp - TagSet sprint helper                         (on c stack)  */
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
  
    Stz* sz(TagSet* pTagSet);

  private:
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
