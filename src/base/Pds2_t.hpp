/*============================================================================*/
/* Pds2_t.hpp -- Primary Data Structure (PDS) Template                          */
/*============================================================================*/
#pragma once

#include "Pds2.hpp"
#include "SkipListNode.hpp"
#include "SkipListBlock.hpp"

namespace BASE
{
  template <class ITEM>
  class Pds2_t : public Pds2
  {
  public:

    // must be last
    Footer m_xFooter;

  public:
//    Pds2_t(void);
    Pds2_t(CompareFunc fpCompare);
    ~Pds2_t(void) noexcept(false);


    ITEM* aAllotEntry(void);

  public:
    Footer* aFooter(void);
  };
  static i8 iDoubleKeyCompare(void* aItem_l, void* aItem_r);
}

using namespace BASE;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
//template <class ITEM>
//Pds2_t<ITEM>::Pds2_t(void)
//    : Pds2(sizeof(ITEM))
//{
//}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <class ITEM>
Pds2_t<ITEM>::Pds2_t(CompareFunc fpCompare)
    : Pds2(sizeof(ITEM), fpCompare)
{
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
template <class ITEM>
Pds2_t<ITEM>::~Pds2_t(void) noexcept(false)
{
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Allot an entry in this Skip List                                           */
/*----------------------------------------------------------------------------*/
template <class ITEM>
ITEM* Pds2_t<ITEM>::aAllotEntry(void)
{
  return (ITEM*) Pds2::aAllotSkipListBlock();
}

/*----------------------------------------------------------------------------*/
/* compare two nodes                                          */
/*----------------------------------------------------------------------------*/
i8 BASE::iDoubleKeyCompare(void* aItem_l, void* aItem_r)
{
  _static_
  struct xTest {
    i8 iKey_1;
    i8 iKey_2;
  }typedef DoubleKey;

  DoubleKey* pKey_l = (DoubleKey*) aItem_l;
  DoubleKey* pKey_r = (DoubleKey*) aItem_r;

  // do the node names match?
  if (pKey_l->iKey_1 == pKey_r->iKey_1) {
    if (pKey_l->iKey_2 == pKey_r->iKey_2) {
      return 0;
    }
    else {
      if (pKey_l->iKey_2 >= pKey_r->iKey_2)
        return 1;
      else
        return -1;
    }
  } else {
    if (pKey_l->iKey_1 >= pKey_r->iKey_1)
      return 1;
    else
      return -1;
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
