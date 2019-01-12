/*============================================================================*/
/* ItemBuffer.hpp -- Item Buffer                                              */
/*============================================================================*/
#pragma once

namespace Structures
{
  class ItemBuffer : public Object
  {
    static const i8 C_WSP_ITEMBUFFER = C_SIZE_LARGE_OBJECT - C_1KB;
  
  protected:
    i8 m_nItems; // number of items in this buffer

    Wsp_t<C_WSP_ITEMBUFFER> m_xWsp;
  
  public:
    ItemBuffer(eCN eClassNo);
   ~ItemBuffer(void) noexcept(false);
  
    Item*  pAllotItem(i8 nBytes);
    char*  aAllotSpace(i8 nBytes);
    char*  aDataArea(void);
    void   vDecItems(void);
    Item*  pFirstItem(void);
    i8     nFreeBytes(void);
    void   vGiveSpace(i8 nBytes);
    i8     nItems(void);
    void   vItems(i8 nItems);
    Item*  pNextItem(Item* pCurrentItem);
    void   vResetUsed(void);
    void   vResetUsed(Item* pUptil);
    i4     iToOffset(Item* pItem);
    i8     nUsedBytes(void);
  
  //private:
    void   vSetWilderness(void);
  };
  
  ItemBuffer* _pItemBuffer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
