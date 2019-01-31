/*============================================================================*/
/* Item.hpp -- buffer item                                                    */
/*============================================================================*/
#pragma once

namespace MiniHeaps
{
  class Item : public Header
  {
  protected:
    Id  m_xId;       // item id
    i4  m_nSizeItem; // size of this item

  public:
    void* operator new(size_t);
    void* operator new(size_t, Item*);
  
    Item(eCN eClassNo, i8 nSizeItem);
    ~Item(void) noexcept(false);
  
    i8 nSizeItem(void);
  
    char* aAddressAfter(void);
  };
  
  /*----------------------------------------------------------------------------*/
  /* safe cast                                                                  */
  /*----------------------------------------------------------------------------*/
  inline Item* _aItem(void* aAddr)
  {
    Item* aItem = reinterpret_cast<Item*>(aAddr);
    
    return aItem;
  }
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
