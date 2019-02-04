/*============================================================================*/
/* ItemHeap.hpp -- Item Heap                                                  */
/*============================================================================*/
#pragma once

namespace MiniHeaps
{
  static const i8 C_MAX_ITEMS = C_8KB;
  static const i4 C_NO_MORE_ITEMS = C_16KB;

  class ItemHeap : public Object
  {
    static const i8 C_WSP_ITEMHEAP = C_SIZE_LARGE_OBJECT - C_1KB;
  
  protected:
    // RDP Java layout ---------------------------------------------------------
    i8 m_iBufferNo; // this EntryBuffer's buffer number
    i8 m_nItems;    // number of items in this heap
    ItemHeap* m_pNextBuffer; // next buffer
    i4* m_i4aItemOffsets;    // array of offsets (i4) to items in m_xWsp
    Wsp_t<C_WSP_ITEMHEAP> m_xWsp; // workspace
    // End RDP Java layout -----------------------------------------------------

    ItemHeap* m_pPrevBuffer;   // prev buffer
    ItemHeap* m_pHigherBuffer; // higher level buffer that has entries
                               // that point to this buffer

    i2     m_iPartitionNo; // partition no this buffer belongs to
    i2     m_iLevelNo;     // level no in tree
    
    Since  m_oWhenNewed;   // when was this buffer created?
    bool   m_bIsLast;      // is this the last buffer in sequence
  
    FreeItem* m_aFreeItemChain; // head of free item chain

    i4  m_i_FreeOffsetChain; // head of free item offset chain
  
  public:
    ItemHeap(eCN eClassNo);
   ~ItemHeap(void) noexcept(false);
  
    i8     iAgeSsec(void);
    Item*  aAllotItem(eCN eClassNo, i8 nBytes);
    NEO*   aAllotNEO(i8 nBytes);
    char*  aAllotSpace(i8 nBytes);
    i8     iDecile(void);
    void   vDecItems(void);
    Item*  aFirstItem(void);
    i8     nFreeBytes(void); //-> nAvailBytes()
    void   vFreeItem(Item* aItem);
    i4     iGetItemNo(void);
    void   vGiveSpace(i8 nBytes);
    bool   bHasFreeItem(void);
    bool   bHasSpace(i8 nBytes);
    void   vInitFreeChain(void);
    void   vIsLast(bool bIsLast);
    bool   bIsLast(void);
    Item*  aItemArea(void);
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
