/*============================================================================*/
/* SP_Token.hpp - Token sprint helper                                         */
/*============================================================================*/
#pragma once

namespace LexicalAnalysis
{
  class SP_Token : public SP
  {
  public:

    // must be last
    Footer m_xFooter;
    
  public:
    SP_Token(void);
   ~SP_Token(void) noexcept(false);
  
    Stz* sz(Token* pToken);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
