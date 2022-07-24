/*============================================================================*/
/* TextSmall.hpp -- small text                                                */
/*============================================================================*/
#pragma once

namespace BASE
{
  class TextSmall : public TextAbstract
  {
  private:
    
    char  m_caText[C_TEXT_SMALL];  // text
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void* pText);
    
    TextSmall(void);
   ~TextSmall(void) noexcept(false);
    
    Stz* szArea(void);
    
    Footer* aFooter(void);
  };
  
  TextSmall* _pTextSmall(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
