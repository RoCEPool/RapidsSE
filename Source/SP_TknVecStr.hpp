/*============================================================================*/
/* SP_TknVecStr.hpp - Token VEctor to String sprint helper       (on c stack) */
/*============================================================================*/
#pragma once

namespace Commands
{
  class SP_TknVecStr : public Object
  {
  public:
    char m_caResult[4096];

    // must be last
    Footer m_xFooter;
    
  public:
    SP_TknVecStr(void);
   ~SP_TknVecStr(void) noexcept(false);
  
    Stz* sz(TokenVec* pTknVec);

  private:
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
