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
    i8 m_nItems;                 // number of items in this buffer
  
    ItemBuffer* m_pNextBuffer;   // next buffer
    ItemBuffer* m_pPrevBuffer;   // prev buffer
    ItemBuffer* m_pHigherBuffer; // higher level buffer that has entries
                                 // that point to this buffer

    Wsp_t<C_WSP_ITEMBUFFER> m_xWsp;
  
  public:
    ItemBuffer(eCN eClassNo);
   ~ItemBuffer(void) noexcept(false);
  
    Item*  aAllotItem(i8 nBytes);
    char*  aAllotSpace(i8 nBytes);
    char*  aDataArea(void);
    void   vDecItems(void);
    Item*  aFirstItem(void);
    i8     nFreeBytes(void);
    void   vGiveSpace(i8 nBytes);
    i8     nItems(void);
    void   vItems(i8 nItems);
    Item*  aNextItem(Item* aCurrentItem);
    void   vResetUsed(void);
    void   vResetUsed(Item* aUptil);
    void   vSetWilderness(void);
    i4     iToOffset(Item* aItem);
    i8     nUsedBytes(void);
  };
  
  ItemBuffer* _pItemBuffer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
