/*============================================================================*/
/* Levels.hpp -- array of level pointers/references/offsets                   */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class RightVec
  {
  public:
    on  m_nOffset[1];  // offset[0] is right for level 1
                      // offset[1] is right for level 2
                      // level 0 is implied to mean next entry
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, RightVec*);
  
    RightVec(void);
    ~RightVec(void);

    on nOffset(u8 uOffset);
    void vOffset(u8 uOffset, on nOffset);
  
    static i8 nBytesRequired(i8 nLevels);
  };
  
  RightVec* _aRightVec(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
