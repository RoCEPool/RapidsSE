/*============================================================================*/
/* Ref.hpp -- Reference to another buffer item                                */
/*============================================================================*/
#pragma once

namespace Rows
{
  class Ref
  {
  private:
    struct {
      u8 m_uBuffer  : 45; // which buffer
      u8 m_uItem    : 16; // item in that buffer
      u8 m_uOffsets :  3; // number of neo's that follow
    };             // 64
  
  public:
    void* operator new(size_t);
    
    Ref(void);
    Ref(u8 uBuffer, u8 uItem, u8 uOffsets);
   ~Ref(void) noexcept(false);
    
    void operator =(Ref xwRef); // assignment

    i8    iAsI8(void);
    i8    iBuffer(void);
    void  vBuffer(i8 iBuffer);
    i8    iItem(void);
    void  vItem(i8 iItem);
    i8    nOffsets(void);
    void  vOffsets(i8 nOffsets);
    void  vSet(Ref xwRef);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
