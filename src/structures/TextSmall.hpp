/*============================================================================*/
/* TextSmall.hpp -- small text                                                */
/*============================================================================*/
#pragma once

namespace Structures
{
  class TextSmall : public TextBuffer
  {
  private:
    
    char  m_caText[C_SIZE_SMALL_TEXT];  // text
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void* pText);
    
    TextSmall(void);
    virtual ~TextSmall(void) noexcept(false);
    
    virtual Stz* aGetText(void);
    
    Footer* aFooter(void);
  };
  
  TextSmall* _pTextSmall(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
