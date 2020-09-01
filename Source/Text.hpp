/*============================================================================*/
/* Text.hpp -- text (can be of varying sizes - tiny, small, medium, large)    */
/*============================================================================*/
#pragma once

namespace Structures
{
  class Text : public Object
  {
  private:
    i8          m_nSizeText;      // size of the text
    TextBuffer* m_pTextBuffer;    // text buffer
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, Text*);
    void  operator delete(void* pText);
    
    Text(void);
    Text(Stz* szArg);
   ~Text(void) noexcept(false);
  
    Stz* szGet(void);
    void vSet(Stz* szArg);
    
    Footer* aFooter(void);
  };
  
  Text* _pText(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
