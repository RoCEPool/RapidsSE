/*============================================================================*/
/* Levels.hpp -- array of level pointers/references/offsets                   */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class RightVec
  {
  public:
  u8  m_uRight[1];  //
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, RightVec*);
  
    RightVec(void);
    ~RightVec(void);

    u8 uRight(i8 iLevel);
    void vRight(i8 iLevel, u8 uOffset);
  
    static i8 nBytesRequired(i8 nLevels);
  };
  
  RightVec* _aRightVec(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
