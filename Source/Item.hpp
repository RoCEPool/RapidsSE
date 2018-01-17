/*============================================================================*/
/* Item.hpp -- buffer item                                                    */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class Item : public Object
  {
  protected:
    i8  m_nThisBytes; // size of this item

  public:
    void* operator new(size_t);
    void* operator new(size_t, Item*);
  
    Item(eCN eClassNo, i8 nThisBytes);
    ~Item(void);
  
    i8 nSizeItem(void);
  };
  
  Item* _pItem(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
