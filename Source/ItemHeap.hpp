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

    FreeItem* m_aFreeItemChain; // head of free item chain

    i2     m_iPartitionNo; // partition no this buffer belongs to
    i2     m_iLevelNo;     // level no in tree
    i8     m_iBufferNo;    // this EntryBuffer's buffer number
    Since  m_oWhenNewed;   // when was this buffer created?
    bool   m_bIsLast;      // is this the last buffer in sequence

    Wsp_t<C_WSP_ITEMHEAP> m_xWsp;
  
  public:
    ItemHeap(eCN eClassNo);
   ~ItemHeap(void) noexcept(false);
  
    i8     iAgeSsec(void);
    Item*  aAllotItem(i8 nBytes);
    char*  aAllotSpace(i8 nBytes);
    char*  aDataArea(void);
    i8     iDecile(void);
    void   vDecItems(void);
    Item*  aFirstItem(void);
    i8     nFreeBytes(void); //-> nAvailBytes()
    void   vFreeItem(Item* aItem);
    void   vGiveSpace(i8 nBytes);
    bool   bHasSpace(i8 nBytes);
    void   vIsLast(bool bIsLast);
    bool   bIsLast(void);
    i8     nItems(void);
    void   vItems(i8 nItems);
    void   vLevelNo(i8 iLevelNo);
    i8     iLevelNo(void);
    Item*  aNextItem(Item* aItem);
    void   vPartitionNo(i8 iPartitionNo);
    i8     iPartitionNo(void);
    void   vResetUsed(void);
    void   vSetUsed(Item* aUptil);
    i4     iToOffset(Item* aItem);
    i8     nUsedBytes(void);
    Since& oWhenNewed(void);

  private:
    void   vSetWilderness(void);
  };
  
  ItemHeap* _pItemHeap(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
