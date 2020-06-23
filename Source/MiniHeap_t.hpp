/*============================================================================*/
/* MiniHeap_t.hpp -- Mini Heap                                                */
/*============================================================================*/
#pragma once

namespace Structures
{
  template <class ITEM>
  class MiniHeap_t
  {
    static const i8 C_WSP_MINIHEAP = C_SIZE_LARGE_OBJECT - C_1KB;
  
  protected:
    
    i8 m_nItems;    // number of items in this mini-heap
    i8 m_nMaxItems; // max items allowed
  
    SingleWord m__PadToCacheLine[6]; // padding to cache line

    Wsp_t<C_WSP_MINIHEAP> m_xWsp; // workspace
  
  public:
    MiniHeap_t(void);
   ~MiniHeap_t(void) noexcept(false);
  
    ITEM*  aAdjacentItem(ITEM* aItem);
    ITEM*  aAdjacentItem(ITEM* aItem, i8 nSizeBytes);
    byte*  ayAllotBytes(i8 nBytes);
    ITEM*  aAllotItem(eCN eClassNo, i8 nBytes);
    i8     iDecile(void);
    void   vDecItems(void);
    void   vDumpItems(void);
    ITEM*  aFirstItem(void);
    i8     nFreeBytes(void);
    void   vFreeItem(ITEM* aItem, i8 nBytes);
    ITEM*  aGetItem(i8 iOffset);
    void   vGiveSpace(i8 nBytes);
    bool   bHasSpace(i8 nBytes);
    void   vIncItems(void);
    ITEM*  aItemArea(void);
    i8     nItems(void);
    void   vItems(i8 nItems);
    i8     nMaxItems(void);
    void   vMaxItems(i8 nMaxItems);
    void   vResetUsed(void);
    void   vSetUsed(ITEM* aUptil);
    i4     iToOffset_i4(ITEM* aItem);
    i8     nUsedBytes(void);

  private:
    void   vSetWilderness(void);
  };
  
  template <class ITEM>
  MiniHeap_t<ITEM>* _pMiniHeap_t(void* aAddr);
}

////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
template <class ITEM>
MiniHeap_t<ITEM>* Structures::_pMiniHeap_t(void* aAddr)
{
  MiniHeap_t<ITEM>* pBufferAbstract =
    reinterpret_cast<MiniHeap_t<ITEM>*>(aAddr);
  
  return pBufferAbstract;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <class ITEM>
MiniHeap_t<ITEM>::MiniHeap_t(void)
{
  ASSERT(CacheLine::bMultipleCacheLines(C_WSP_MINIHEAP));
  ASSERT(CacheLine::bOnCacheLineBoundary(this));
  
  
  m_nItems = 0;
  m_nMaxItems = 0;
  
  MemLib::vZero(m__PadToCacheLine,sizeof(m__PadToCacheLine),CN_NMLSTR);
  
#ifdef WITH_CHECK_INITIALISED
  m_xWsp.vZero(CN_ROWTRAY);
#endif
  
  vSetWilderness();
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
template <class ITEM>
MiniHeap_t<ITEM>::~MiniHeap_t(void) noexcept(false)
{
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get item which is memory-wise the next item in the item area               */
/*----------------------------------------------------------------------------*/
template <class ITEM>
ITEM* MiniHeap_t<ITEM>::aAdjacentItem(ITEM* aItem)
{
  ITEM* aAdjacent = nullptr;
  
  i8 nItemBytes = (CN_FREEITEM == aItem->eClass())
                    ? ((FreeItem*)aItem)->nSizeBytes()
                    : aItem->nSizeBytes();
  
  aAdjacent = aAdd<ITEM*>(aItem,nItemBytes);
  
  if ((char*)aAdjacent >= (char*)m_xWsp.ayLatter())
    return nullptr;
  
  return aAdjacent;
}

/*----------------------------------------------------------------------------*/
/* get item which is memory-wise the next item in the item area               */
/*----------------------------------------------------------------------------*/
template <class ITEM>
ITEM* MiniHeap_t<ITEM>::aAdjacentItem(ITEM* aItem, i8 nSizeItem)
{
  ITEM* aAdjacent = nullptr;
  
  i8 nItemBytes = (CN_FREEITEM == aItem->eClass())
  ? ((FreeItem*)aItem)->nSizeBytes()
  : nSizeItem;
  
  aAdjacent = aAdd<ITEM*>(aItem,nItemBytes);
  
  if ((char*)aAdjacent >= (char*)m_xWsp.ayLatter())
    return nullptr;
  
  return aAdjacent;
}

/*----------------------------------------------------------------------------*/
/* allot space for nBytes in this buffer                                      */
/*----------------------------------------------------------------------------*/
template <class ITEM>
byte* MiniHeap_t<ITEM>::ayAllotBytes(i8 nBytes)
{
  // test sufficient free space
  if (nFreeBytes() < nBytes)
    return nullptr;
  
  byte* ayBytes = m_xWsp.ayAllot(nBytes);
  
  vSetWilderness();
  
  return ayBytes;
}

/*----------------------------------------------------------------------------*/
/* allot an item of nBytes in this buffer                                     */
/*----------------------------------------------------------------------------*/
template <class ITEM>
ITEM* MiniHeap_t<ITEM>::aAllotItem(eCN eClassNo, i8 nBytes)
{
  ITEM* aItem = reinterpret_cast<ITEM*>(ayAllotBytes(nBytes));
  if (! aItem)
    return nullptr;
  
  new(aItem) ITEM(eClassNo,nBytes);
  
  m_nItems++;
  
  return aItem;
}

/*----------------------------------------------------------------------------*/
/* get decile full of the buffer                                              */
/*----------------------------------------------------------------------------*/
template <class ITEM>
i8 MiniHeap_t<ITEM>::iDecile(void)
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
template <class ITEM>
void MiniHeap_t<ITEM>::vDecItems(void)
{
  m_nItems--;
}

/*----------------------------------------------------------------------------*/
/* dump items in heap                                                         */
/*----------------------------------------------------------------------------*/
template <class ITEM>
void MiniHeap_t<ITEM>::vDumpItems(void)
{
  i8 iItem = 0;
  
  ITEM* aItem = aFirstItem();
  //  if (m_i4aItemOffsets)
  //    aItem = aAdd<ITEM*>(aItem,sizeof(i4) * C_MAX_ITEMS);
  
  for (;
       aItem;
       aItem = aAdjacentItem(aItem)) {
    
    i8 iOffset = iToOffset(aItem);
    eCN eClass = aItem->eClass();
    switch (eClass) {
        
      case CN_ENT: {
        ITEM* aEntry = _aItem(aItem);
        vTrace("[%04lld:%06lld] CN_ENT: %lldB\n"_Stz,
               iItem,iOffset,aEntry->nSizeData());
        break;
      }
        
      case CN_FREEITEM: {
        FreeItem* aFreeItem = _aFreeItem(aItem);
        vTrace("[%04lld:%06lld] CN_FREEITEM: %lldB\n"_Stz,
               iItem,iOffset,aFreeItem->nSizeBytes());
        break;
      }
        
      case CN_WILDERNESS: {
        Wilderness* aWilderness = _aWilderness(aItem);
        vTrace("[%04lld:%06lld] CN_WILDERNESS: %lldB\n"_Stz,
               iItem,iOffset,aWilderness->nSizeBytes());
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
template <class ITEM>
ITEM* MiniHeap_t<ITEM>::aFirstItem(void)
{
  if ( m_nItems == 0 )
    return nullptr;
  
  ITEM* aFirst = reinterpret_cast<ITEM*>(m_xWsp.ayArea());
  
  return aFirst;
}

/*----------------------------------------------------------------------------*/
/* how much free space (in bytes) left in this buffer?                        */
/*----------------------------------------------------------------------------*/
template <class ITEM>
i8 MiniHeap_t<ITEM>::nFreeBytes(void)
{
  i8 nBytes = m_xWsp.nFree();
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* add given entry to free chain                                              */
/*----------------------------------------------------------------------------*/
template <class ITEM>
void MiniHeap_t<ITEM>::vFreeItem(ITEM* aItem, i8 nBytes)
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // add item to head of free chain
  
  FreeItem* aFreeItem = FreeItem::aMorphFreeItem(aItem,nBytes);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // decement the number of items in buffer
  vDecItems();
}

/*----------------------------------------------------------------------------*/
/* get item at given offset                                                   */
/*----------------------------------------------------------------------------*/
template <class ITEM>
ITEM* MiniHeap_t<ITEM>::aGetItem(i8 iOffset)
{
  ITEM* aItem = aAdd<ITEM*>(aItemArea(),iOffset);
  
  return aItem;
}

/*----------------------------------------------------------------------------*/
/* give space back to this buffer                                             */
/*----------------------------------------------------------------------------*/
template <class ITEM>
void MiniHeap_t<ITEM>::vGiveSpace(i8 nBytes)
{
  m_xWsp.vGive(nBytes);
}

/*----------------------------------------------------------------------------*/
/* does buffer have space for nBytes ?                                        */
/*----------------------------------------------------------------------------*/
template <class ITEM>
bool MiniHeap_t<ITEM>::bHasSpace(i8 nBytes)
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
/* increment number of items in buffer                                        */
/*----------------------------------------------------------------------------*/
template <class ITEM>
void MiniHeap_t<ITEM>::vIncItems(void)
{
  m_nItems++;
}

/*----------------------------------------------------------------------------*/
/* get address of data area                                                   */
/*----------------------------------------------------------------------------*/
template <class ITEM>
ITEM* MiniHeap_t<ITEM>::aItemArea(void)
{
  ITEM* aItemArea = (ITEM*)m_xWsp.ayArea();
  
  return aItemArea;
}

/*----------------------------------------------------------------------------*/
/* get number of items in the buffer                                          */
/*----------------------------------------------------------------------------*/
template <class ITEM>
i8 MiniHeap_t<ITEM>::nItems(void)
{
  return m_nItems;
}

/*----------------------------------------------------------------------------*/
/* set number of items in the buffer                                          */
/*----------------------------------------------------------------------------*/
template <class ITEM>
void MiniHeap_t<ITEM>::vItems(i8 nItems)
{
  m_nItems = nItems;
}

/*----------------------------------------------------------------------------*/
/* get number of items in the buffer                                          */
/*----------------------------------------------------------------------------*/
template <class ITEM>
i8 MiniHeap_t<ITEM>::nMaxItems(void)
{
  return m_nMaxItems;
}

/*----------------------------------------------------------------------------*/
/* set number of items in the buffer                                          */
/*----------------------------------------------------------------------------*/
template <class ITEM>
void MiniHeap_t<ITEM>::vMaxItems(i8 nMaxItems)
{
  m_nMaxItems = nMaxItems;
}

/*----------------------------------------------------------------------------*/
/* reset used space                                                           */
/*----------------------------------------------------------------------------*/
template <class ITEM>
void MiniHeap_t<ITEM>::vResetUsed(void)
{
  m_xWsp.vUsed(0);
  
  m_nItems = 0;
  
  vSetWilderness();
}

/*----------------------------------------------------------------------------*/
/* set used space (typically after a compaction)                              */
/*----------------------------------------------------------------------------*/
template <class ITEM>
void MiniHeap_t<ITEM>::vSetUsed(ITEM* aUptil)
{
  i8 nUsedBytes = nDifference(aUptil,aItemArea());
  
  m_xWsp.vUsed(nUsedBytes);
  
  vSetWilderness();
}

/*----------------------------------------------------------------------------*/
/* calculate the absolute offset within workspace for given item              */
/*----------------------------------------------------------------------------*/
template <class ITEM>
i4 MiniHeap_t<ITEM>::iToOffset_i4(ITEM* aItem)
{
  i4 iOffset = static_cast<i4>(nDifference(aItem,m_xWsp.ayArea()));
  
  ASSERT(iOffset >= 0 && iOffset < m_xWsp.nSize());
  
  return iOffset;
}

/*----------------------------------------------------------------------------*/
/* get bytes used                                                             */
/*----------------------------------------------------------------------------*/
template <class ITEM>
i8 MiniHeap_t<ITEM>::nUsedBytes(void)
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
template <class ITEM>
void MiniHeap_t<ITEM>::vSetWilderness(void)
{
  if (nFreeBytes() < (i8)sizeof(Wilderness))
    return;
  
  Wilderness* aWilderness = reinterpret_cast<Wilderness*>(m_xWsp.ayLatter());
  
  new(aWilderness) Wilderness;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
