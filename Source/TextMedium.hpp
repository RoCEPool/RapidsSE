/*============================================================================*/
/* TextMedium.hpp -- medium text                                              */
/*============================================================================*/
#pragma once

namespace Structures
{
  class TextMedium : public TextBuffer
  {
  private:
    
    // must be last
    //Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, Text*);
    void  operator delete(void* pText);
    
    TextMedium(void);
   ~TextMedium(void) noexcept(false);
    
    //Footer* aFooter(void);
  };
  
  TextMedium* _pTextMedium(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
