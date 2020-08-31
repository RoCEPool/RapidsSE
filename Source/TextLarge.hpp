/*============================================================================*/
/* TextLarge.hpp -- large text                                                */
/*============================================================================*/
#pragma once

namespace Structures
{
  class TextLarge : public TextBuffer
  {
  private:
    
    // must be last
    //Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, Text*);
    void  operator delete(void* pText);
    
    TextLarge(void);
   ~TextLarge(void) noexcept(false);
    
    //Footer* aFooter(void);
  };
  
  TextLarge* _pTextLarge(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
