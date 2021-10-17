/*============================================================================*/
/* TextMedium.hpp -- medium text                                              */
/*============================================================================*/
#pragma once

namespace BASE
{
  class TextMedium : public TextBuffer
  {
  private:
    
    char  m_caText[C_SIZE_MEDIUM_TEXT];  // text
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void* pText);
    
    TextMedium(void);
    TextMedium(Stz* szText);
    virtual ~TextMedium(void) noexcept(false);
    
    virtual Stz* aGetText(void);
    
    Footer* aFooter(void);
  };
  
  TextMedium* _pTextMedium(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
