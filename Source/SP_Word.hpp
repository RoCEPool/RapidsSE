/*============================================================================*/
/* SP_Word.hpp - Word sprint helper                                           */
/*============================================================================*/
#pragma once

namespace Forth
{
  class SP_Word : public SP
  {
  public:

    // must be last
    Footer m_xFooter;
    
  public:
    SP_Word(void);
   ~SP_Word(void) noexcept(false);
  
    Stz* sz(Word* pWord);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
