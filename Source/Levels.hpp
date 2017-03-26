/*============================================================================*/
/* Levels.hpp -- array of level pointers/references/offsets                   */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class Levels
  {
  public:
  i8  m_iRight[1];  //
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, Levels*);
  
    Levels(void);
    ~Levels(void);

    static i8 nBytesRequired(i8 nLevels);
  };
  
  Levels* _aLevels(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
