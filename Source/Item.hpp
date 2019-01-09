/*============================================================================*/
/* Item.hpp -- buffer item                                                    */
/*============================================================================*/
#pragma once

namespace Structures
{
  class Item : public Header
  {
  protected:
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
  inline Item* _pItem(void* aAddr)
  {
    Item* pItem = reinterpret_cast<Item*>(aAddr);
    
    return pItem;
  }
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
