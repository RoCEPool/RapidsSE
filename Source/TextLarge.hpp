/*============================================================================*/
/* TextLarge.hpp -- large text                                                */
/*============================================================================*/
#pragma once

namespace Structures
{
  class TextLarge : public TextBuffer
  {
  private:
    
    char  m_caText[C_SIZE_LARGE_TEXT];  // text
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void* pText);
    
    TextLarge(void);
   ~TextLarge(void) noexcept(false);
    
    Stz* aGetText(void);
    
    Footer* aFooter(void);
  };
  
  TextLarge* _pTextLarge(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
