/*============================================================================*/
/* TextLarge.hpp -- large text                                                */
/*============================================================================*/
#pragma once

namespace BASE
{
  class TextLarge : public TextAbstract
  {
  private:
    
    char  m_caText[C_TEXT_LARGE];  // text
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void* pText);
    
    TextLarge(void);
    virtual ~TextLarge(void) noexcept(false);
    
    virtual Stz* aGetText(void);
    
    Footer* aFooter(void);
  };
  
  TextLarge* _pTextLarge(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
