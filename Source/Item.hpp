/*============================================================================*/
/* Item.hpp -- buffer item                                                    */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class Item : public Object
  {
  protected:
    i8  m_nSizeItem; // size of this item

  public:
    void* operator new(size_t);
    void* operator new(size_t, Item*);
  
    Item(eCN eClassNo, i8 nSizeItem);
    ~Item(void);
  
    i8 nSizeItem(void);
  
    char* aAddressAfter(void);
  };
  
  //Item* _pItem(void* aAddr);
  
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
