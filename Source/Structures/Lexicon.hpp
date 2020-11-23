/*============================================================================*/
/* Lexicon.hpp -- System Lexicon                                              */
/*============================================================================*/
#pragma once

namespace Structures
{
  class Lexicon : public Object
  {
  private:
    Article*  m_pArticle;  // the lexicon article
  
    s32       m_sName;     // name of this lexicon
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Lexicon(Stz* szName);
   ~Lexicon(void) noexcept(false);
    
    Stz*  szName(void);
    void  vName(Stz* szName);
  
    Article* pArticle(void);
    
    Footer* aFooter(void);
  };
  
  Lexicon* _pLexicon(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
