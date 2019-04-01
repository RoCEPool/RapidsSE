/*============================================================================*/
/* ItemHeap.cpp -- Item Buffer                                                */
/*============================================================================*/

#include "MiniHeaps.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
ItemHeap* MiniHeaps::_pItemHeap(void* aAddr)
{
  ItemHeap* pBufferAbstract = reinterpret_cast<ItemHeap*>(aAddr);
  
  return pBufferAbstract;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ItemHeap::ItemHeap(eCN eClassNo)
: Object(eClassNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,eClassNo);
  
  m_iBufferNo     = *nPoolBufferNo();
  m_nItems = 0;

  m_pNextBuffer   = nullptr;
  m_pPrevBuffer   = nullptr;
  m_pHigherBuffer = nullptr;

  m_iPartitionNo  = -1;
  m_iBufferHeight = 0;
  
  m_oWhenNewed.vNow();
  m_bIsLast       = false;
  
  // head of free item chain
  m_aFreeItemChain  = nullptr;

  // entry offsets array
  m_i4aItemOffsets    = nullptr;
  m_i_FreeOffsetChain = C_NO_MORE_ITEMS;
  
  
#ifdef WITH_CHECK_INITIALISED
  m_xWsp.vZero(eClassNo);
#endif
  
  vSetWilderness();
  
  _postcondition_(_ADDRESS_,_MAGIC_,eClassNo);
  
  //  _checkInitialised_(eClassNo);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ItemHeap::~ItemHeap(void) noexcept(false)
{
  //_precondition_(_ADDRESS_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get item which is memory-wise the next item in the item area               */
/*----------------------------------------------------------------------------*/
Item* ItemHeap::aAdjacentItem(Item* aItem)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  i8 nItemBytes = aItem->nSizeItem_WithoutOffsets() + aItem->iNEOsize();
  
  Item* aAdjacent = aAdd<Item*>(aItem,nItemBytes);
  
  if ((char*)aAdjacent >= (char*)m_xWsp.aLatter())
    return nullptr;
  
  return aAdjacent;
}

/*----------------------------------------------------------------------------*/
/* get age of buffer in seconds                                               */
/*----------------------------------------------------------------------------*/
i8 ItemHeap::iAgeSsec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  Since oSince;
  oSince.vNow();
  
  Ivl wDiff;
  
  wDiff = oSince.wSubtract(m_oWhenNewed);
  
  i8 iAge = wDiff.iSecond();
  
  return iAge;
}

/*----------------------------------------------------------------------------*/
/* allot an item of nBytes in this buffer                                     */
/*----------------------------------------------------------------------------*/
Item* ItemHeap::aAllotItem(eCN eClassNo, i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
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
NEO* ItemHeap::aAllotNEO(i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  NEO* aNEO = reinterpret_cast<NEO*>(aAllotSpace(nBytes));
  
  return aNEO;
}

/*----------------------------------------------------------------------------*/
/* allot space for nBytes in this buffer                                      */
/*----------------------------------------------------------------------------*/
char* ItemHeap::aAllotSpace(i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
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
i8 ItemHeap::iDecile(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  i8 iDecile;
  
  if (gpAttachMap->m_nIndexEntries == 0)
    iDecile = IntLib::iDecile(m_xWsp.nUsed(), C_SIZE_POOL_BUFFER);
  else
    iDecile = IntLib::iDecile(m_nItems, gpAttachMap->m_nIndexEntries);
  
  return iDecile;
}

/*----------------------------------------------------------------------------*/
/* decrement number of items in buffer                                        */
/*----------------------------------------------------------------------------*/
void ItemHeap::vDecItems(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  m_nItems--;
}

/*----------------------------------------------------------------------------*/
/* dump items in heap                                                         */
/*----------------------------------------------------------------------------*/
void ItemHeap::vDumpItems(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  i8 iItem = 0;

  Item* aItem = aFirstItem();
  if (m_i4aItemOffsets)
    aItem = aAdd<Item*>(aItem,sizeof(i4) * C_MAX_ITEMS);

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
Item* ItemHeap::aFirstItem(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  if ( m_nItems == 0 )
    return nullptr;
  
  Item* aFirst = reinterpret_cast<Item*>(m_xWsp.aArea());
  
  return aFirst;
}

/*----------------------------------------------------------------------------*/
/* how much free space (in bytes) left in this buffer?                        */
/*----------------------------------------------------------------------------*/
i8 ItemHeap::nFreeBytes(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  i8 nBytes = m_xWsp.nFree();
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* add given entry to free chain                                              */
/*----------------------------------------------------------------------------*/
void ItemHeap::vFreeItem(Item* aItem)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // add item to head of free chain
  
  FreeItem* aFreeItem = FreeItem::aMorphFreeItem(aItem);
  
  //if (m_aFreeItemChain)
  //  aFreeItem->vNextFree(m_aFreeItemChain);
  
  //m_aFreeItemChain = aFreeItem;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // decement the number of items in buffer
  vDecItems();
}

/*----------------------------------------------------------------------------*/
/* give space back to this buffer                                             */
/*----------------------------------------------------------------------------*/
void ItemHeap::vGiveSpace(i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_xWsp.vGive(nBytes);
}

/*----------------------------------------------------------------------------*/
/* does buffer have a free item no in free chain ?                            */
/*----------------------------------------------------------------------------*/
bool ItemHeap::bHasFreeItem(void)
{
  if (C_NO_MORE_ITEMS == m_i_FreeOffsetChain)
    return false;
  
  //RASSERT(nItems() < C_MAX_ITEMS - 16);

  return true;
}

/*----------------------------------------------------------------------------*/
/* does buffer have space for nBytes ?                                        */
/*----------------------------------------------------------------------------*/
bool ItemHeap::bHasSpace(i8 nBytes)
{
  i8 nFree = m_xWsp.nFree() - sizeof(Wilderness);
  
  // we allow for only C_MAX_ITEMS - 16 entries
//  if (nItems() >= C_MAX_ITEMS - 16) {
//    return false;
//  }
  
  if (gpAttachMap->m_nIndexEntries > 0) {
    // has space calculation based on no of entries
    if (m_nItems >= gpAttachMap->m_nIndexEntries)
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
/* set up the initial entry offset free chain                                 */
/*----------------------------------------------------------------------------*/
void ItemHeap::vInitFreeChain(void)
{
  m_i4aItemOffsets  =
    reinterpret_cast<i4*>(aAllotSpace(sizeof(i4) * C_MAX_ITEMS));
  
  m_i_FreeOffsetChain = C_NO_MORE_ITEMS;
  
  i8 iMaxItems =
    gpAttachMap->m_nIndexEntries ? gpAttachMap->m_nIndexEntries : C_MAX_ITEMS;
  
  for (i4 i_Item = 0; i_Item < (iMaxItems - 1); i_Item++) {
    RASSERT(i_Item >= 0 && i_Item < C_MAX_ITEMS);
    m_i4aItemOffsets[i_Item] = - (i_Item + 1);
  }
  
  RASSERT((iMaxItems - 1) >= 0 && (iMaxItems - 1) < C_MAX_ITEMS);
  m_i4aItemOffsets[iMaxItems - 1] = - C_NO_MORE_ITEMS;
  
  m_i_FreeOffsetChain = 0;
}

/*----------------------------------------------------------------------------*/
/* set buffer is last                                                         */
/*----------------------------------------------------------------------------*/
void ItemHeap::vIsLast(bool bIsLast)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_bIsLast = bIsLast;
}

/*----------------------------------------------------------------------------*/
/* get if buffer is last                                                      */
/*----------------------------------------------------------------------------*/
bool ItemHeap::bIsLast(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  return m_bIsLast;
}

/*----------------------------------------------------------------------------*/
/* get address of data area                                                   */
/*----------------------------------------------------------------------------*/
Item* ItemHeap::aItemArea(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  Item* aItemArea = (Item*)m_xWsp.aArea();
  
  return aItemArea;
}

/*----------------------------------------------------------------------------*/
/* get number of items in the buffer                                          */
/*----------------------------------------------------------------------------*/
i8 ItemHeap::nItems(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_nItems;
}

/*----------------------------------------------------------------------------*/
/* set number of items in the buffer                                          */
/*----------------------------------------------------------------------------*/
void ItemHeap::vItems(i8 nItems)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_nItems = nItems;
}

/*----------------------------------------------------------------------------*/
/* set buffer level                                                           */
/*----------------------------------------------------------------------------*/
void ItemHeap::vBufferHeight(i8 iBufferHeight)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_iBufferHeight = iBufferHeight;
}

/*----------------------------------------------------------------------------*/
/* get the buffer height                                                       */
/*----------------------------------------------------------------------------*/
i8 ItemHeap::iBufferHeight(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_iBufferHeight;
}

/*----------------------------------------------------------------------------*/
/* set partition no                                                           */
/*----------------------------------------------------------------------------*/
void ItemHeap::vPartitionNo(i8 iPartitionNo)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  m_iPartitionNo = iPartitionNo;
}

/*----------------------------------------------------------------------------*/
/* get partition no                                                           */
/*----------------------------------------------------------------------------*/
i8 ItemHeap::iPartitionNo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  return m_iPartitionNo;
}

/*----------------------------------------------------------------------------*/
/* reset used space                                                           */
/*----------------------------------------------------------------------------*/
void ItemHeap::vResetUsed(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_xWsp.vUsed(0);
  
  m_nItems = 0;
  
  vSetWilderness();
}

/*----------------------------------------------------------------------------*/
/* set used space (typically after a compaction)                              */
/*----------------------------------------------------------------------------*/
void ItemHeap::vSetUsed(Item* aUptil)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  i8 nUsedBytes = nSub(aUptil,aItemArea());
  
  m_xWsp.vUsed(nUsedBytes);
  
  vSetWilderness();
}

/*----------------------------------------------------------------------------*/
/* calculate the absolute offset within workspace for given item              */
/*----------------------------------------------------------------------------*/
i4 ItemHeap::iToOffset(Item* aItem)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  i4 iOffset = static_cast<i4>(nSub(aItem,m_xWsp.aArea()));
  
  ASSERT(iOffset >= 0 && iOffset < m_xWsp.nSize());
  
  return iOffset;
}

/*----------------------------------------------------------------------------*/
/* get bytes used                                                             */
/*----------------------------------------------------------------------------*/
i8 ItemHeap::nUsedBytes(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  i8 nBytes = m_xWsp.nUsed();
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* when was this buffer newed?                                                */
/*----------------------------------------------------------------------------*/
Since& ItemHeap::oWhenNewed(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_oWhenNewed;
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* set wilderness marker                                                      */
/*----------------------------------------------------------------------------*/
void ItemHeap::vSetWilderness(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  if (nFreeBytes() < (i8)sizeof(Wilderness))
    return;
  
  Wilderness* aWilderness = reinterpret_cast<Wilderness*>(m_xWsp.aLatter());
  
  new(aWilderness) Wilderness;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
