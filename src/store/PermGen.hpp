/*============================================================================*/
/* PermGen.hpp -- permutation generator                                       */
/*============================================================================*/
#pragma once

#include "BASE.h"

namespace STORE
{
  class PermGen : public Object
  {
  public:
    static const i8 C_PERMGEN_ELEMENTS = 16;

  private:

    i8 m_nElements; // number of items
    i8 m_iaElements[C_PERMGEN_ELEMENTS]; // sequenced array of elements

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    PermGen(i8 nItems);
   ~PermGen(void) noexcept(false);

    static void vTest_Permutation_v2(i8 nItems);

    void vCalcPermutation(i8 i_Permutation);
    i8   iElement(i8 i_Position);
    i8   nElements(void);
    i8   nPermutations(i8 nElements);

  private:
    void vCalcPermutation_showWorking(i8 i_Permutation);

    Footer* aFooter(void);
  };

  PermGen* _pPermGen(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
