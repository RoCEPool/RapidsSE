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
    
    i8       m_iBufferNo;        // this EntryBuffer's buffer number
    i8       m_nItems;           // number of items in this heap
    i2       m_iPartitionNo;     // partition no this buffer belongs to
    i2       m_iBufferHeight;    // buffer height in the tree
    
    ItemHeap* m_pNextBuffer;      // next buffer
    ItemHeap* m_pPrevBuffer;      // prev buffer
    ItemHeap* m_pHigherBuffer;    // higher level buffer that has entries
                                  // that point to this buffer
    Wsp_t<C_WSP_ITEMHEAP> m_xWsp; // workspace
  
  public:
    ItemHeap(eCN eClassNo);
   ~ItemHeap(void) noexcept(false);
  
    Item*  aAdjacentItem(Item* aItem);
    Item*  aAllotItem(eCN eClassNo, i8 nBytes);
    NEO*   aAllotNEO(i8 nBytes);
    char*  aAllotSpace(i8 nBytes);
    i8     iDecile(void);
    void   vDecItems(void);
    void   vDumpItems(void);
    Item*  aFirstItem(void);
    i8     nFreeBytes(void);
    void   vFreeItem(Item* aItem);
    void   vGiveSpace(i8 nBytes);
    bool   bHasSpace(i8 nBytes);
    Item*  aItemArea(void);
    i8     nItems(void);
    void   vItems(i8 nItems);
    void   vBufferHeight(i8 iBufferHeight);
    i8     iBufferHeight(void);
    void   vPartitionNo(i8 iPartitionNo);
    i8     iPartitionNo(void);
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
