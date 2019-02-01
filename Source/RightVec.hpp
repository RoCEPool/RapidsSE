/*============================================================================*/
/* RightVec.hpp -- Right vector (array of offsets)                            */
/*============================================================================*/
#pragma once

namespace Structures
{
  class RightVec
  {
  public:
    i8  m_nOffset[1];  // offset[0] is right for level 1
                       // offset[1] is right for level 2
                       // level 0 is implied to mean next entry
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, RightVec*);
  
    RightVec(void);
    ~RightVec(void) noexcept(false);

    i8   iOffset(i8 i_OffsetNo);
    void vOffset(i8 i_OffsetNo, i8 iOffset);
  };
  
  RightVec* _aRightVec(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
