/*============================================================================*/
/* Id.hpp -- the id of an item in a mini-heap                                 */
/*============================================================================*/
#pragma once

namespace MiniHeaps
{
  class Id
  {
  private:
    struct {
      u8 m_uBuffer : 45; // which buffer
      u8 m_uItem   : 16; // item in buffer
      u8 m_uOffsets:  3; // number of offsets
    };            // 64
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, Id*);
    
    Id(void);
    Id(Id& xId);
    Id(u8 uBuffer, u8 uItem, u8 uOffsets);
    ~Id(void) noexcept(false);
    
    void operator =(Id& xId); // assignment
  
    i8    iAsI8(void);
    void  vAssign(Id& xId);
    void  vAssign(i8 iBuffer, i8 iItem);
    i8    iBuffer(void);
    void  vBuffer(i8 iBuffer);
    i8    iItem(void);
    void  vItem(i8 iItem);
    i8    nOffsets(void);
    void  vOffsets(i8 nOffsets);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
