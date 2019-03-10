/*============================================================================*/
/* SkipRoots.hpp -- roots of the skips list in an entry buffer                */
/*============================================================================*/
#pragma once

namespace Data
{
  class SkipRoots : public Item
  {
  private:
    i8 m_iPadding[4];
 
  public:
    SkipRoots(void);
    ~SkipRoots(void) noexcept(false);
  };

  SkipRoots* _aSkipRoots(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
