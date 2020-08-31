/*============================================================================*/
/* TextBuffer.hpp -- textBuffer                                               */
/*============================================================================*/
#pragma once

namespace Structures
{
  class TextBuffer : public Object
  {
  private:
    i8    m_nBufferSize;    // buffer size
    char  m_caText[0];      // text
    
  public:
    
    TextBuffer(eCN eClass, i8 nBufferSize);
   ~TextBuffer(void) noexcept(false);
  
    i8   nBufferSize(void);
    Stz* aGetText(void);
  };
  
  TextBuffer* _pTextBuffer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
