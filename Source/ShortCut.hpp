/*============================================================================*/
/* ShortCut.hpp -- ShortCut                                                   */
/*============================================================================*/
#pragma once

namespace Prompt
{
  class ShortCut : public Object
  {
  public:
  
    s32 m_sName;
    s4K m_sCmd;
  
    // must be last
    Footer m_xFooter;
    
  public:
    ShortCut(void);
    ShortCut(Stz* szName);
    ShortCut(Stz* szName, Stz* szCmd);
   ~ShortCut() noexcept(false);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
