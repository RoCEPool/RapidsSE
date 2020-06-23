/*============================================================================*/
/* Ref.hpp -- Reference to another buffer item                                */
/*============================================================================*/
#pragma once

namespace Rows
{
  class Ref : public NonProper
  {
  private:
    union {
      struct {
        u8 m_uBuffer  : 40; // which buffer
        u8 m_uItem    : 16; // item in that buffer
        u8 m_uSpare   :  4; // spare
        u8 m_uPayload :  1; // is there is a payload?
        u8 m_uOffsets :  3; // number of neo's that follow
      };             // 64b
      
      i8 m_iAsI8;
    };
  
  public:   
    Ref(void);
    Ref(u8 uBuffer, u8 uItem, u8 uOffsets);
   ~Ref(void) noexcept(false);
    
    void operator =(Ref xwRef); // assignment

    i8    iAsI8(void);
    i8    iBuffer(void);
    void  vBuffer(i8 iBuffer);
    bool  bIsNil(void);
    i8    iItem(void);
    void  vItem(i8 iItem);
    bool  bPayLoad(void);
    void  vPayload(bool bPayLoad);
    void  vReset(void);
    void  vSet(Ref xwRef);
    void  vSetNil(void);
    i8    nSkipLevels(void);
    void  vSkipLevels(i8 nSkipLevels);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
