/*============================================================================*/
/* MiniHeap_t.hpp -- Mini Heap                                                */
/*============================================================================*/
#pragma once

namespace MiniHeaps
{
  template <eCN CN, class ITEM>
  class MiniHeap_t
  {
    static const i8 C_WSP_ITEMHEAP = C_SIZE_LARGE_OBJECT - C_1KB;
  
  protected:
    
    i8 m_nItems;    // number of items in this mini-heap
    i8 m_nMaxItems; // max items allowed
    Wsp_t<C_WSP_ITEMHEAP> m_xWsp; // workspace
  
  public:
    MiniHeap_t(i8 nMaxItems);
   ~MiniHeap_t(void) noexcept(false);
  
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
  
  template <eCN CN, class ITEM>
  MiniHeap_t<CN,ITEM>* _pMiniHeap_t(void* aAddr);
}

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
template <eCN CN, class ITEM>
MiniHeap_t<CN,ITEM>* MiniHeaps::_pMiniHeap_t(void* aAddr)
{
  MiniHeap_t<CN,ITEM>* pBufferAbstract =
    reinterpret_cast<MiniHeap_t<CN,ITEM>*>(aAddr);
  
  return pBufferAbstract;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <eCN CN, class ITEM>
MiniHeap_t<CN,ITEM>::MiniHeap_t(i8 nMaxItems)
{
  m_nItems = 0;
  m_nMaxItems = nMaxItems;
  
#ifdef WITH_CHECK_INITIALISED
  m_xWsp.vZero(CN_ROWTRAY);
#endif
  
  vSetWilderness();
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
template <eCN CN, class ITEM>
MiniHeap_t<CN,ITEM>::~MiniHeap_t(void) noexcept(false)
{
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get item which is memory-wise the next item in the item area               */
/*----------------------------------------------------------------------------*/
template <eCN CN, class ITEM>
Item* MiniHeap_t<CN,ITEM>::aAdjacentItem(Item* aItem)
{
  i8 nItemBytes = aItem->nSizeItem_WithoutOffsets() + aItem->iNEOsize();
  
  Item* aAdjacent = aAdd<Item*>(aItem,nItemBytes);
  
  if ((char*)aAdjacent >= (char*)m_xWsp.aLatter())
    return nullptr;
  
  return aAdjacent;
}

/*----------------------------------------------------------------------------*/
/* allot an item of nBytes in this buffer                                     */
/*----------------------------------------------------------------------------*/
template <eCN CN, class ITEM>
Item* MiniHeap_t<CN,ITEM>::aAllotItem(eCN eClassNo, i8 nBytes)
{
  Item* aItem = reinterpret_cast<Item*>(aAllotSpace(nBytes));
  if (! aItem)
    return nullptr;
  
  new(aItem) Item(eClassNo,nBytes);
  
  m_nItems++;
  
  return aItem;
}

/*----------------------------------------------------------------------------*/
/* allot an offset array in this buffer                                       */
/*----------------------------------------------------------------------------*/
template <eCN CN, class ITEM>
NEO* MiniHeap_t<CN,ITEM>::aAllotNEO(i8 nBytes)
{
  NEO* aNEO = reinterpret_cast<NEO*>(aAllotSpace(nBytes));
  
  return aNEO;
}

/*----------------------------------------------------------------------------*/
/* allot space for nBytes in this buffer                                      */
/*----------------------------------------------------------------------------*/
template <eCN CN, class ITEM>
char* MiniHeap_t<CN,ITEM>::aAllotSpace(i8 nBytes)
{
  // test  sufficient freespace
  if (nFreeBytes() < nBytes)
    return nullptr;
  
  char* aSpace = m_xWsp.aAllot(nBytes);
  
  vSetWilderness();
  
  return aSpace;
}

/*----------------------------------------------------------------------------*/
/* get decile full of the buffer                                              */
/*----------------------------------------------------------------------------*/
template <eCN CN, class ITEM>
i8 MiniHeap_t<CN,ITEM>::iDecile(void)
{
  i8 iDecile;
  
  if (m_nMaxItems == 0)
    iDecile = IntLib::iDecile(m_xWsp.nUsed(), C_SIZE_POOL_BUFFER);
  else
    iDecile = IntLib::iDecile(m_nItems, m_nMaxItems);
  
  return iDecile;
}

/*----------------------------------------------------------------------------*/
/* decrement number of items in buffer                                        */
/*----------------------------------------------------------------------------*/
template <eCN CN, class ITEM>
void MiniHeap_t<CN,ITEM>::vDecItems(void)
{
  m_nItems--;
}

/*----------------------------------------------------------------------------*/
/* dump items in heap                                                         */
/*----------------------------------------------------------------------------*/
template <eCN CN, class ITEM>
void MiniHeap_t<CN,ITEM>::vDumpItems(void)
{
  i8 iItem = 0;
  
  Item* aItem = aFirstItem();
  //  if (m_i4aItemOffsets)
  //    aItem = aAdd<Item*>(aItem,sizeof(i4) * C_MAX_ITEMS);
  
  for (;
       aItem;
       aItem = aAdjacentItem(aItem)) {
    
    i8 iOffset = iToOffset(aItem);
    eCN eClass = aItem->eClass();
    switch (eClass) {
        
      case CN_ENTRY: {
        Item* aEntry = _aItem(aItem);
        vTrace("[%04lld:%06lld] CN_ENTRY: %lldB\n"_Stz,
               iItem,iOffset,aEntry->nSizeData());
        break;
      }
        
      case CN_FREEENTRY: {
        FreeItem* aFreeEntry = _aFreeItem(aItem);
        vTrace("[%04lld:%06lld] CN_FREEENTRY: %lldB\n"_Stz,
               iItem,iOffset,aFreeEntry->nSizeData());
        break;
      }
        
      case CN_WILDERNESS: {
        Wilderness* aWilderness = _aWilderness(aItem);
        vTrace("[%04lld:%06lld] CN_WILDERNESS: %lldB\n"_Stz,
               iItem,iOffset,aWilderness->nSizeData());
        break;
      }
      default:
        vTrace("Unknown class in item heap\n"_Stz);
        vTrace("Unknown class %lld\n"_Stz,eClass);
    }
    
    iItem++;
  }
  
}

/*----------------------------------------------------------------------------*/
/* first item                                                                 */
/*----------------------------------------------------------------------------*/
template <eCN CN, class ITEM>
Item* MiniHeap_t<CN,ITEM>::aFirstItem(void)
{
  if ( m_nItems == 0 )
    return nullptr;
  
  Item* aFirst = reinterpret_cast<Item*>(m_xWsp.aArea());
  
  return aFirst;
}

/*----------------------------------------------------------------------------*/
/* how much free space (in bytes) left in this buffer?                        */
/*----------------------------------------------------------------------------*/
template <eCN CN, class ITEM>
i8 MiniHeap_t<CN,ITEM>::nFreeBytes(void)
{
  i8 nBytes = m_xWsp.nFree();
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* add given entry to free chain                                              */
/*----------------------------------------------------------------------------*/
template <eCN CN, class ITEM>
void MiniHeap_t<CN,ITEM>::vFreeItem(Item* aItem)
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // add item to head of free chain
  
  FreeItem* aFreeItem = FreeItem::aMorphFreeItem(aItem);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // decement the number of items in buffer
  vDecItems();
}

/*----------------------------------------------------------------------------*/
/* give space back to this buffer                                             */
/*----------------------------------------------------------------------------*/
template <eCN CN, class ITEM>
void MiniHeap_t<CN,ITEM>::vGiveSpace(i8 nBytes)
{
  m_xWsp.vGive(nBytes);
}

/*----------------------------------------------------------------------------*/
/* does buffer have space for nBytes ?                                        */
/*----------------------------------------------------------------------------*/
template <eCN CN, class ITEM>
bool MiniHeap_t<CN,ITEM>::bHasSpace(i8 nBytes)
{
  i8 nFree = m_xWsp.nFree() - sizeof(Wilderness);
  
  if (m_nMaxItems > 0) {
    // has space calculation based on no of entries
    if (m_nItems >= m_nMaxItems)
      return false;
    else
      return true;
  }
  else {
    // has space calculation based on free space
    if (nFree < nBytes)
      return false;
    else
      return true;
  }
}

/*----------------------------------------------------------------------------*/
/* get address of data area                                                   */
/*----------------------------------------------------------------------------*/
template <eCN CN, class ITEM>
Item* MiniHeap_t<CN,ITEM>::aItemArea(void)
{
  Item* aItemArea = (Item*)m_xWsp.aArea();
  
  return aItemArea;
}

/*----------------------------------------------------------------------------*/
/* get number of items in the buffer                                          */
/*----------------------------------------------------------------------------*/
template <eCN CN, class ITEM>
i8 MiniHeap_t<CN,ITEM>::nItems(void)
{
  return m_nItems;
}

/*----------------------------------------------------------------------------*/
/* set number of items in the buffer                                          */
/*----------------------------------------------------------------------------*/
template <eCN CN, class ITEM>
void MiniHeap_t<CN,ITEM>::vItems(i8 nItems)
{
  m_nItems = nItems;
}

/*----------------------------------------------------------------------------*/
/* reset used space                                                           */
/*----------------------------------------------------------------------------*/
template <eCN CN, class ITEM>
void MiniHeap_t<CN,ITEM>::vResetUsed(void)
{
  m_xWsp.vUsed(0);
  
  m_nItems = 0;
  
  vSetWilderness();
}

/*----------------------------------------------------------------------------*/
/* set used space (typically after a compaction)                              */
/*----------------------------------------------------------------------------*/
template <eCN CN, class ITEM>
void MiniHeap_t<CN,ITEM>::vSetUsed(Item* aUptil)
{
  i8 nUsedBytes = nSub(aUptil,aItemArea());
  
  m_xWsp.vUsed(nUsedBytes);
  
  vSetWilderness();
}

/*----------------------------------------------------------------------------*/
/* calculate the absolute offset within workspace for given item              */
/*----------------------------------------------------------------------------*/
template <eCN CN, class ITEM>
i4 MiniHeap_t<CN,ITEM>::iToOffset(Item* aItem)
{
  i4 iOffset = static_cast<i4>(nSub(aItem,m_xWsp.aArea()));
  
  ASSERT(iOffset >= 0 && iOffset < m_xWsp.nSize());
  
  return iOffset;
}

/*----------------------------------------------------------------------------*/
/* get bytes used                                                             */
/*----------------------------------------------------------------------------*/
template <eCN CN, class ITEM>
i8 MiniHeap_t<CN,ITEM>::nUsedBytes(void)
{
  i8 nBytes = m_xWsp.nUsed();
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* set wilderness marker                                                      */
/*----------------------------------------------------------------------------*/
template <eCN CN, class ITEM>
void MiniHeap_t<CN,ITEM>::vSetWilderness(void)
{
  if (nFreeBytes() < (i8)sizeof(Wilderness))
    return;
  
  Wilderness* aWilderness = reinterpret_cast<Wilderness*>(m_xWsp.aLatter());
  
  new(aWilderness) Wilderness;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
