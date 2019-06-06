/*============================================================================*/
/* MiniHeap_t.cpp -- Mini Heap                                                */
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
MiniHeap_t* MiniHeaps::_pMiniHeap_t(void* aAddr)
{
  MiniHeap_t* pBufferAbstract = reinterpret_cast<MiniHeap_t*>(aAddr);
  
  return pBufferAbstract;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
MiniHeap_t::MiniHeap_t(void)
{
  m_nItems = 0;
  
#ifdef WITH_CHECK_INITIALISED
  m_xWsp.vZero(CN_ROWTRAY);
#endif
  
  vSetWilderness();
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
MiniHeap_t::~MiniHeap_t(void) noexcept(false)
{
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get item which is memory-wise the next item in the item area               */
/*----------------------------------------------------------------------------*/
Item* MiniHeap_t::aAdjacentItem(Item* aItem)
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
Item* MiniHeap_t::aAllotItem(eCN eClassNo, i8 nBytes)
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
NEO* MiniHeap_t::aAllotNEO(i8 nBytes)
{
  NEO* aNEO = reinterpret_cast<NEO*>(aAllotSpace(nBytes));
  
  return aNEO;
}

/*----------------------------------------------------------------------------*/
/* allot space for nBytes in this buffer                                      */
/*----------------------------------------------------------------------------*/
char* MiniHeap_t::aAllotSpace(i8 nBytes)
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
i8 MiniHeap_t::iDecile(void)
{
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
void MiniHeap_t::vDecItems(void)
{
  m_nItems--;
}

/*----------------------------------------------------------------------------*/
/* dump items in heap                                                         */
/*----------------------------------------------------------------------------*/
void MiniHeap_t::vDumpItems(void)
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
Item* MiniHeap_t::aFirstItem(void)
{
  if ( m_nItems == 0 )
    return nullptr;
  
  Item* aFirst = reinterpret_cast<Item*>(m_xWsp.aArea());
  
  return aFirst;
}

/*----------------------------------------------------------------------------*/
/* how much free space (in bytes) left in this buffer?                        */
/*----------------------------------------------------------------------------*/
i8 MiniHeap_t::nFreeBytes(void)
{
  i8 nBytes = m_xWsp.nFree();
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* add given entry to free chain                                              */
/*----------------------------------------------------------------------------*/
void MiniHeap_t::vFreeItem(Item* aItem)
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
void MiniHeap_t::vGiveSpace(i8 nBytes)
{
  m_xWsp.vGive(nBytes);
}

/*----------------------------------------------------------------------------*/
/* does buffer have space for nBytes ?                                        */
/*----------------------------------------------------------------------------*/
bool MiniHeap_t::bHasSpace(i8 nBytes)
{
  i8 nFree = m_xWsp.nFree() - sizeof(Wilderness);
  
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
/* get address of data area                                                   */
/*----------------------------------------------------------------------------*/
Item* MiniHeap_t::aItemArea(void)
{
  Item* aItemArea = (Item*)m_xWsp.aArea();
  
  return aItemArea;
}

/*----------------------------------------------------------------------------*/
/* get number of items in the buffer                                          */
/*----------------------------------------------------------------------------*/
i8 MiniHeap_t::nItems(void)
{
  return m_nItems;
}

/*----------------------------------------------------------------------------*/
/* set number of items in the buffer                                          */
/*----------------------------------------------------------------------------*/
void MiniHeap_t::vItems(i8 nItems)
{
  m_nItems = nItems;
}

/*----------------------------------------------------------------------------*/
/* reset used space                                                           */
/*----------------------------------------------------------------------------*/
void MiniHeap_t::vResetUsed(void)
{
  m_xWsp.vUsed(0);
  
  m_nItems = 0;
  
  vSetWilderness();
}

/*----------------------------------------------------------------------------*/
/* set used space (typically after a compaction)                              */
/*----------------------------------------------------------------------------*/
void MiniHeap_t::vSetUsed(Item* aUptil)
{
  i8 nUsedBytes = nSub(aUptil,aItemArea());
  
  m_xWsp.vUsed(nUsedBytes);
  
  vSetWilderness();
}

/*----------------------------------------------------------------------------*/
/* calculate the absolute offset within workspace for given item              */
/*----------------------------------------------------------------------------*/
i4 MiniHeap_t::iToOffset(Item* aItem)
{
  i4 iOffset = static_cast<i4>(nSub(aItem,m_xWsp.aArea()));
  
  ASSERT(iOffset >= 0 && iOffset < m_xWsp.nSize());
  
  return iOffset;
}

/*----------------------------------------------------------------------------*/
/* get bytes used                                                             */
/*----------------------------------------------------------------------------*/
i8 MiniHeap_t::nUsedBytes(void)
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
void MiniHeap_t::vSetWilderness(void)
{
  if (nFreeBytes() < (i8)sizeof(Wilderness))
    return;
  
  Wilderness* aWilderness = reinterpret_cast<Wilderness*>(m_xWsp.aLatter());
  
  new(aWilderness) Wilderness;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
