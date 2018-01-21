/*============================================================================*/
/* ItemBuffer.hpp -- Item Buffer                                              */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class ItemBuffer // : public Object
  {
  protected:
  i8    m_nItems;    // number of items in this buffer

  WspFxd<C_SIZE_LARGE_OBJECT - C_1KB*4> m_xWsp;
  
  public:
  //ItemBuffer(eCN eClassNo);
    ItemBuffer(void);
   ~ItemBuffer(void);
  
    Item*  pAllotItem(i8 nBytes);
    char*  aDataArea(void);
    Item*  pFirstItem(void);
    i8     nFreeBytes(void);
    i8     nItems(void);
    Item*  pNextItem(Item* pCurrentItem);
    i8     nUsedBytes(void);
  };
  
  ItemBuffer* _pBufferAbstract(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
