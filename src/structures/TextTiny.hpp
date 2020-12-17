/*============================================================================*/
/* TextTiny.hpp -- tiny text                                                  */
/*============================================================================*/
#pragma once

namespace Resources
{
  
  class TextTiny : public TextBuffer
  {
  private:
    
    char  m_caText[C_SIZE_TINY_TEXT];  // text
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void* pText);
    
    TextTiny(void);
    virtual ~TextTiny(void) noexcept(false);
    
    virtual Stz* aGetText(void);
    
    Footer* aFooter(void);
  };
  
  TextTiny* _pTextTiny(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
