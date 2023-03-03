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
    static const i8 C_PERMGEN_ITEMS = 1024;

  private:

    i8 m_nItems; // number of items
    i8 m_nPermutations; // number of permutations so far
    i8 m_iaItem[C_PERMGEN_ITEMS]; // sequencing array of items
    i8 m_iaRotate[C_PERMGEN_ITEMS]; // rotation pivots of items
    i8 m_i_Rotate; // current rotate point

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    PermGen(i8 nItems);
   ~PermGen(void) noexcept(false);

    static void vTest(i8 nItems);
    static void vTest_Rotation(i8 nItems);

    void vInitRotations(void);
    i8   iItem(i8 i_Position);
    i8   nItems(void);
    bool bNextPermutation(void);
    bool bNextRotation(void);
    i8   nPermutations(void);
    void vPrint(Stz* szLabel);

  private:

    bool bIsValidPermutation(void);

    Footer* aFooter(void);
  };

  PermGen* _pPermGen(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
