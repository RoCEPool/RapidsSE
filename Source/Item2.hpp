/*============================================================================*/
/* Item2.hpp -- lightweight buffer item                                       */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class Item2
  {
  protected:
    i8  m_nSizeItem; // size of this item

  public:
    void* operator new(size_t);
    void* operator new(size_t, Item2*);
  
    Item2(eCN eClassNo, i8 nSizeItem);
    ~Item2(void);
  
    i8 nSizeItem(void);
  
    char* aAddressAfter(void);
  };
  
  //Item* _pItem(void* aAddr);
  
  /*----------------------------------------------------------------------------*/
  /* safe cast                                                                  */
  /*----------------------------------------------------------------------------*/
  inline Item2* _pItem2(void* aAddr)
  {
  Item2* pItem2 = reinterpret_cast<Item2*>(aAddr);
  
  return pItem2;
  }
  
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
