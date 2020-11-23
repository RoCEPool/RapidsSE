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
    void  operator delete(void* pText);
    
    Text(void);
    Text(i8 nStringSize);
    Text(Stz* szArg);
   ~Text(void) noexcept(false);
  
    Stz* szGet(void);
    void vSet(Stz* szArg);
    void vSizeText(void);

  private:
    void vDeleteBuffer(void);
    void vNewBuffer(i8 nStringSize);
    
    Footer* aFooter(void);
  };
  
  Text* _pText(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
