/*============================================================================*/
/* TextBuffer.hpp -- textBuffer                                               */
/*============================================================================*/
#pragma once

namespace Structures
{
  static const i8 C_SIZE_BASE        = 8*C_BYTES_PER_WORD;
  static const i8 C_SIZE_TINY_TEXT   = C_SIZE_TINY_OBJECT   - C_SIZE_BASE;
  static const i8 C_SIZE_SMALL_TEXT  = C_SIZE_SMALL_OBJECT  - C_SIZE_BASE;
  static const i8 C_SIZE_MEDIUM_TEXT = C_SIZE_MEDIUM_OBJECT - C_SIZE_BASE;
  static const i8 C_SIZE_LARGE_TEXT  = C_SIZE_LARGE_OBJECT  - C_SIZE_BASE;
  
  class TextBuffer : public Object
  {
  private:
    i8 m_nBufferSize;
    
  public:
    
    TextBuffer(eCN eClass, i8 nBufferSize);
    virtual ~TextBuffer(void) noexcept(false);
    
    i8 nBufferSize(void);
    virtual Stz* aGetText(void) = 0;
  };
  
  TextBuffer* _pTextBuffer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
