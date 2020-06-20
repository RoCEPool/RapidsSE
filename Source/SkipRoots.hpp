/*============================================================================*/
/* SkipRoots.hpp -- roots of the skips list in an entry buffer                */
/*============================================================================*/
#pragma once

namespace Indexing
{
static const i8 C_SKIPROOTS_PADDING = 4;

  class SkipRoots : public Ent
  {
  private:
    i8 m_iPadding[C_SKIPROOTS_PADDING];
 
  public:
    SkipRoots(void);
    ~SkipRoots(void) noexcept(false);
  };

  SkipRoots* _aSkipRoots(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
