/*============================================================================*/
/* SP_TagSet.hpp - TagSet sprint helper                         (on c stack)  */
/*============================================================================*/
#pragma once

namespace Gadgets
{
  class SP_TagSet : public SPmedium
  {
  public:

    // must be last
    Footer m_xFooter;
    
  public:
    SP_TagSet(void);
   ~SP_TagSet(void) noexcept(false);
  
    Stz* sz(TagSet* pTagSet);

  private:
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
