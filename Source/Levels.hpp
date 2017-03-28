/*============================================================================*/
/* Levels.hpp -- array of level pointers/references/offsets                   */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class Levels
  {
  public:
  u8  m_uRight[1];  //
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, Levels*);
  
    Levels(void);
    ~Levels(void);

    u8 uRight(i8 iLevel);
  
    static i8 nBytesRequired(i8 nLevels);
  };
  
  Levels* _aLevels(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
