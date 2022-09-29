/*============================================================================*/
/* PermGen.hpp -- permutation generator                                       */
/*============================================================================*/
#pragma once

#include "BASE.h"

namespace STORE
{
  class PermGen : public Object
  {
  private:
    static const i8 C_PERM_LEVELS = 0;

    i8 m_nLevels;

    i8 m_i_CurrLevel;
    i8 m_iaAction[C_PERM_LEVELS];
    i8 m_iaSubscript[C_PERM_LEVELS];

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    PermGen(i8 nLevels);
   ~PermGen(void) noexcept(false);

    i8 i_CurrLevel(void);
    bool bNextLevel(void);
    bool bPrevLevel(void);

    i8 iFirstSubscript_nextLevel(void);
    i8 iNextSubscript_currLevel(void);

    void vReset(void);

  private:
    Footer* aFooter(void);
  };

  PermGen* _pPermGen(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
