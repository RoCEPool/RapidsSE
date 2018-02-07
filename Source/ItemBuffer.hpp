/*============================================================================*/
/* ItemBuffer.hpp -- Item Buffer                                              */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class ItemBuffer : public Object
  {
  protected:
    i8 m_nItems; // number of items in this buffer

    WspFxd<C_SIZE_LARGE_OBJECT - C_1KB> m_xWsp;
  
  public:
    ItemBuffer(eCN eClassNo);
   ~ItemBuffer(void);
  
    Item*  pAllotItem(i8 nBytes);
    char*  aAllotSpace(i8 nBytes);
    char*  aDataArea(void);
    Item*  pFirstItem(void);
    i8     nFreeBytes(void);
    void   vGiveSpace(i8 nBytes);
    i8     nItems(void);
    Item*  pNextItem(Item* pCurrentItem);
    i8     iToOffset(Item* pItem);
    i8     nUsedBytes(void);
  
  //private:
    void   vSetWilderness(void);
  };
  
  ItemBuffer* _pItemBuffer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
