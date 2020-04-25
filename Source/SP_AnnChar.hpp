/*============================================================================*/
/* SP_AnnChar.hpp - AnnChar sprint helper                                     */
/*============================================================================*/
#pragma once

namespace LexicalAnalysis
{
  class SP_AnnChar : public SP
  {
  public:

    // must be last
    Footer m_xFooter;
    
  public:
    SP_AnnChar(void);
   ~SP_AnnChar(void) noexcept(false);
  
    Stz* sz(AnnChar* aAnnChar);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
