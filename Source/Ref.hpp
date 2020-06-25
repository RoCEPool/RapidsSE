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
        u8 m_uBuffer  : 32; // which buffer (2^32 * 1MB is 4PB)
        u8 m_uItem    : 16; // item in that buffer (2^16 is max of 64K items)
        u8 m_uLevels  :  3; // number of skip levels in next entry array
        u8 m_uPayload :  1; // is there is a payload? (not used yet)
        u8 m_uKind    :  4; // kind (not used yet)
        u8 m_uSpare   :  8; // spare
      };             // 64b
      
      i8 m_iAsI8;
    };
  
  public:   
    Ref(void);
    Ref(u8 uBuffer, u8 uItem, u8 uLevels);
   ~Ref(void) noexcept(false);
    
    void operator =(Ref xwRef); // assignment

    i8    iAsI8(void);
    i8    iBuffer(void);
    void  vBuffer(i8 iBuffer);
    bool  bIsNil(void);
    i8    iItem(void);
    void  vItem(i8 iItem);
    eKE   eKind(void);
    void  vKind(eKE eKind);
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
