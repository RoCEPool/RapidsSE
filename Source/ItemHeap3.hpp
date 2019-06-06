/*============================================================================*/
/* ItemHeap3.hpp -- Item Heap                                                 */
/*============================================================================*/
#pragma once

namespace MiniHeaps
{
  class ItemHeap3
  {
    static const i8 C_WSP_ITEMHEAP = C_SIZE_LARGE_OBJECT - C_1KB;
  
  protected:
    
    i8 m_nItems; // number of items in this heap
    Wsp_t<C_WSP_ITEMHEAP> m_xWsp; // workspace
  
  public:
    ItemHeap3(void);
   ~ItemHeap3(void) noexcept(false);
  
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
    void   vResetUsed(void);
    void   vSetUsed(Item* aUptil);
    i4     iToOffset(Item* aItem);
    i8     nUsedBytes(void);

  private:
    void   vSetWilderness(void);
  };
  
  ItemHeap3* _pItemHeap3(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
