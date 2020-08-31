/*============================================================================*/
/* TextSmall.hpp -- small text                                                */
/*============================================================================*/
#pragma once

namespace Structures
{
  class TextSmall : public TextBuffer
  {
  private:
    
    // must be last
    //Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, Text*);
    void  operator delete(void* pText);
    
    TextSmall(void);
   ~TextSmall(void) noexcept(false);
    
    //Footer* aFooter(void);
  };
  
  TextSmall* _pTextSmall(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
