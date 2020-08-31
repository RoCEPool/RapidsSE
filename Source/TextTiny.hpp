/*============================================================================*/
/* TextTiny.hpp -- tiny text                                                  */
/*============================================================================*/
#pragma once

namespace Structures
{
  class TextTiny : public TextBuffer
  {
  private:
    
    // must be last
    //Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, Text*);
    void  operator delete(void* pText);
    
    TextTiny(void);
   ~TextTiny(void) noexcept(false);
    
    //Footer* aFooter(void);
  };
  
  TextTiny* _pTextTiny(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
