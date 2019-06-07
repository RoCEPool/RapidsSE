/*============================================================================*/
/* Item_tbd.hpp -- buffer item                                                */
/*============================================================================*/
#pragma once

namespace MiniHeaps
{
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Item_tbd : public Header
  {
  protected:
    Id  m_xId;       // item id
    i4  m_nSizeItem_WithoutOffsets; // size of this item (not including offsets)
    // ..data bytes..
    // ..neo..
  
  public:
#ifdef WITH_DEBUG_VERSION
  i8  m_iSkipNo;    // verify of order of skip items
  //i8  m_iIntKey;    // key debug hack
#endif

  public:
    void* operator new(size_t);
    void* operator new(size_t, Item_tbd*);
  
    Item_tbd(eCN eClassNo, i8 nSizeItem);
    ~Item_tbd(void) noexcept(false);
  
    i8 nSizeData(void);
    i8 nSizeItem_WithoutOffsets(void);
  };
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
