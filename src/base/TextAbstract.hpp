/*============================================================================*/
/* TextAbstract.hpp -- Text Abstract Buffer                                   */
/*============================================================================*/
#pragma once

namespace BASE
{
  static const i8 C_TEXT_ABSTRACT = 8 * C_BYTES_PER_WORD;
  static const i8 C_TEXT_TINY     = C_SIZE_TINY_OBJECT   - C_TEXT_ABSTRACT;
  static const i8 C_TEXT_SMALL    = C_SIZE_SMALL_OBJECT  - C_TEXT_ABSTRACT;
  static const i8 C_TEXT_MEDIUM   = C_SIZE_MEDIUM_OBJECT - C_TEXT_ABSTRACT;
  static const i8 C_TEXT_LARGE    = C_SIZE_LARGE_OBJECT  - C_TEXT_ABSTRACT;
  
  class TextAbstract : public Object
  {
  private:
    i8 m_nBufferSize;
    
  public:

    void* operator new(size_t) = delete;
    void  operator delete(void*) = delete;

    TextAbstract(eCN eClass, i8 nBufferSize);
   ~TextAbstract(void) noexcept(false);
    
    i8 nBufferSize(void);
    Stz* szArea(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
