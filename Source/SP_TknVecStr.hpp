/*============================================================================*/
/* SP_TknVecStr.hpp - Token VEctor to String sprint helper       (on c stack) */
/*============================================================================*/
#pragma once

namespace LexicalAnalysis
{
  class SP_TknVecStr : public SPmedium
  {
  public:

    // must be last
    Footer m_xFooter;
    
  public:
    SP_TknVecStr(void);
   ~SP_TknVecStr(void) noexcept(false);
  
    Stz* sz(TokenVec* pTknVec);
    Stz* sz(TokenVec* pTknVec, i8 i_From, i8 i_Till=C_UNKNOWN);

  private:
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
