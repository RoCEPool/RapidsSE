/*============================================================================*/
/* SkipRoots.hpp -- roots of the skips list in an entry buffer                */
/*============================================================================*/
#pragma once

namespace Data
{
  class SkipRoots : public Item
  {
    friend class EntryBuffer;
  
  private:
    i4 m_iaNextOffset[3];
 
  public:
    SkipRoots(void);
    ~SkipRoots(void) noexcept(false);
  };
  
  SkipRoots* _aSkipRoots(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
