/*============================================================================*/
/* ItemHeap.hpp -- Item Heap                                                  */
/*============================================================================*/
#pragma once

namespace MiniHeaps
{
  class ItemHeap : public Object
  {
    static const i8 C_WSP_ITEMHEAP = C_SIZE_LARGE_OBJECT - C_1KB;
  
  protected:
    i8 m_nItems;               // number of items in this heap
  
    ItemHeap* m_pNextBuffer;   // next buffer
    ItemHeap* m_pPrevBuffer;   // prev buffer
    ItemHeap* m_pHigherBuffer; // higher level buffer that has entries
                               // that point to this buffer

    FreeItem* m_aFreeEntryList; // head of free entry list

    Wsp_t<C_WSP_ITEMHEAP> m_xWsp;
  
  public:
    ItemHeap(eCN eClassNo);
   ~ItemHeap(void) noexcept(false);
  
    Item*  aAllotItem(i8 nBytes);
    char*  aAllotSpace(i8 nBytes);
    char*  aDataArea(void);
    void   vDecItems(void);
    Item*  aFirstItem(void);
    i8     nFreeBytes(void); //-> nAvailBytes()
    void   vFreeItem(Item* aItem);
    void   vGiveSpace(i8 nBytes);
    i8     nItems(void);
    void   vItems(i8 nItems);
    Item*  aNextItem(Item* aItem);
    void   vResetUsed(void);
    void   vSetUsed(Item* aUptil);
    i4     iToOffset(Item* aItem);
    i8     nUsedBytes(void);
  
  private:
    void   vSetWilderness(void);
  };
  
  ItemHeap* _pItemHeap(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
