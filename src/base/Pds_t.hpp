/*============================================================================*/
/* Pds_t.hpp -- Primary Data Structure (PDS) Template                          */
/*============================================================================*/
#pragma once

//#include "Pds.hpp"
//#include "SkipListNode.hpp"
//#include "SkipListBlock.hpp"

namespace BASE
{
  template <class ITEM>
  class Pds_t : public Pds
  {
  public:

    // must be last
    Footer m_xFooter;

  public:
    Pds_t(CompareFunc fpCompare);
    Pds_t(CompareFunc fpCompare, bool bAddToHashMap);
    ~Pds_t(void) noexcept(false);


//    ITEM* aAllotEntry(void);

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
template <class ITEM>
Pds_t<ITEM>::Pds_t(CompareFunc fpCompare)
    : Pds(sizeof(ITEM), fpCompare)
{
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <class ITEM>
Pds_t<ITEM>::Pds_t(CompareFunc fpCompare, bool bAddToHashMap)
    : Pds(sizeof(ITEM), fpCompare, bAddToHashMap)
{
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
template <class ITEM>
Pds_t<ITEM>::~Pds_t(void) noexcept(false)
{
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Allot an entry in this Skip List                                           */
/*----------------------------------------------------------------------------*/
//template <class ITEM>
//ITEM* Pds_t<ITEM>::aAllotEntry(void)
//{
//  return (ITEM*) Pds::aAllotSkipListBlock();
//}

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
