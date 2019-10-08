/*============================================================================*/
/* SP_Article.hpp - Article sprint helper                       (on c stack)  */
/*============================================================================*/
#pragma once

namespace Gadgets
{
  class SP_Article : public SP
  {
  public:

    // must be last
    Footer m_xFooter;
    
  public:
    SP_Article(void);
   ~SP_Article(void) noexcept(false);
  
    Stz* sz(Article* pArticle, i8 iDepth);

  private:
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
