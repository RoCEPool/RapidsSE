/*============================================================================*/
/* SP_Charser.hpp - Charser sprint helper                                     */
/*============================================================================*/
#pragma once

namespace LexicalAnalysis
{
  class SP_Charser : public SP
  {
  public:

    // must be last
    Footer m_xFooter;
    
  public:
    SP_Charser(void);
   ~SP_Charser(void) noexcept(false);
  
    Stz* sz(Charser* pCharser);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
