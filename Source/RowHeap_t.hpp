/*============================================================================*/
/* RowHeap_t.hpp -- Row Heap                                                  */
/*============================================================================*/
#pragma once

namespace Structures
{
  template <class ITEM>
  class RowHeap_t
  {
    static const i8 C_WSP_ROWHEAP = C_SIZE_LARGE_OBJECT - C_1KB;
    
  protected:
    
    i8    m_nItems;                           // number of items in this row-heap
    WspConverge_t<C_WSP_ROWHEAP>  m_xWsp;     // workspace
    
  public:
    RowHeap_t(void);
    ~RowHeap_t(void) noexcept(false);
  
    ITEM*  aAdjacentItem(ITEM* aItem, i8 nSizeBytes);
    ITEM*  ayAllotHead(i8 nBytes);
    Stz*   ayAllotTail(i8 nBytes);
    i8     iDecile(void);
    void   vDecItems(void);
    ITEM*  aFirstItem(void);
    i8     nFreeBytes(void);
    void   vFreeItem(ITEM* aItem, i8 nBytes);
    bool   bHasSpace(i8 nBytes);
    void   vIncItems(void);
    ITEM*  aItemArea(void);
    i8     nItems(void);
    void   vItems(i8 nItems);
    void   vResetUsed(void);
    i8     nUsedBytes(void);

  private:
    void   vSetWilderness(void);
  };
  
  template <class ITEM>
  RowHeap_t<ITEM>* _pRowHeap_t(void* aAddr);
}

////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
template <class ITEM>
RowHeap_t<ITEM>* Structures::_pRowHeap_t(void* aAddr)
{
  RowHeap_t<ITEM>* pBufferAbstract =
    reinterpret_cast<RowHeap_t<ITEM>*>(aAddr);
  
  return pBufferAbstract;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <class ITEM>
RowHeap_t<ITEM>::RowHeap_t(void)
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
template <class ITEM>
RowHeap_t<ITEM>::~RowHeap_t(void) noexcept(false)
{
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get item which is memory-wise the next item in the item area               */
/*----------------------------------------------------------------------------*/
template <class ITEM>
ITEM* RowHeap_t<ITEM>::aAdjacentItem(ITEM* aItem, i8 nSizeItem)
{
  ITEM* aAdjacent = nullptr;
  
//  i8 nItemBytes = (CN_FREEITEM == aItem->eClass())
//  ? ((FreeItem*)aItem)->nSizeBytes()
//  : nSizeItem;
  
  aAdjacent = aAdd<ITEM*>(aItem,nSizeItem);
  
  if ((char*)aAdjacent >= (char*)m_xWsp.ayLatterHead())
    return nullptr;
  
  return aAdjacent;
}

/*----------------------------------------------------------------------------*/
/* allot space for nBytes in this buffer                                      */
/*----------------------------------------------------------------------------*/
template <class ITEM>
ITEM* RowHeap_t<ITEM>::ayAllotHead(i8 nBytes)
{
  // test sufficient free space
  if (nFreeBytes() < nBytes)
    return nullptr;
  
  ITEM* aItem = (ITEM*)m_xWsp.ayAllotHead(nBytes);
  
  vSetWilderness();
  
  return aItem;
}

/*----------------------------------------------------------------------------*/
/* allot space for nBytes in this buffer                                      */
/*----------------------------------------------------------------------------*/
template <class ITEM>
Stz* RowHeap_t<ITEM>::ayAllotTail(i8 nBytes)
{
  // test sufficient free space
  if (nFreeBytes() < nBytes)
    return nullptr;
  
  Stz* ayStz = (Stz*)m_xWsp.ayAllotTail(nBytes);
  
  vSetWilderness();
  
  return ayStz;
}

/*----------------------------------------------------------------------------*/
/* get decile full of the buffer                                              */
/*----------------------------------------------------------------------------*/
template <class ITEM>
i8 RowHeap_t<ITEM>::iDecile(void)
{
  i8 iDecile;
  
  iDecile = IntLib::iDecile(m_xWsp.nUsed(), C_SIZE_POOL_BUFFER);

  return iDecile;
}

/*----------------------------------------------------------------------------*/
/* decrement number of items in buffer                                        */
/*----------------------------------------------------------------------------*/
template <class ITEM>
void RowHeap_t<ITEM>::vDecItems(void)
{
  m_nItems--;
}

/*----------------------------------------------------------------------------*/
/* first item                                                                 */
/*----------------------------------------------------------------------------*/
template <class ITEM>
ITEM* RowHeap_t<ITEM>::aFirstItem(void)
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
i8 RowHeap_t<ITEM>::nFreeBytes(void)
{
  i8 nBytes = m_xWsp.nFree();
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* add given entry to free chain                                              */
/*----------------------------------------------------------------------------*/
template <class ITEM>
void RowHeap_t<ITEM>::vFreeItem(ITEM* aItem, i8 nBytes)
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // add item to head of free chain
  
  FreeItem* aFreeItem = FreeItem::aMorphFreeItem(aItem,nBytes);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // decement the number of items in buffer
  vDecItems();
}

/*----------------------------------------------------------------------------*/
/* does buffer have space for nBytes ?                                        */
/*----------------------------------------------------------------------------*/
template <class ITEM>
bool RowHeap_t<ITEM>::bHasSpace(i8 nBytes)
{
  i8 nFree = m_xWsp.nFree() - sizeof(Wilderness);
  
  // has space calculation based on free space
  if (nFree < nBytes)
    return false;
  else
    return true;
}

/*----------------------------------------------------------------------------*/
/* increment number of items in buffer                                        */
/*----------------------------------------------------------------------------*/
template <class ITEM>
void RowHeap_t<ITEM>::vIncItems(void)
{
  m_nItems++;
}

/*----------------------------------------------------------------------------*/
/* get address of data area                                                   */
/*----------------------------------------------------------------------------*/
template <class ITEM>
ITEM* RowHeap_t<ITEM>::aItemArea(void)
{
  ITEM* aItemArea = (ITEM*)m_xWsp.ayArea();
  
  return aItemArea;
}

/*----------------------------------------------------------------------------*/
/* get number of items in the buffer                                          */
/*----------------------------------------------------------------------------*/
template <class ITEM>
i8 RowHeap_t<ITEM>::nItems(void)
{
  return m_nItems;
}

/*----------------------------------------------------------------------------*/
/* set number of items in the buffer                                          */
/*----------------------------------------------------------------------------*/
template <class ITEM>
void RowHeap_t<ITEM>::vItems(i8 nItems)
{
  m_nItems = nItems;
}

/*----------------------------------------------------------------------------*/
/* reset used space                                                           */
/*----------------------------------------------------------------------------*/
template <class ITEM>
void RowHeap_t<ITEM>::vResetUsed(void)
{
  m_xWsp.vUsedHead(0);
  m_xWsp.vUsedTail(0);
  
  m_nItems = 0;
  
  vSetWilderness();
}

/*----------------------------------------------------------------------------*/
/* get bytes used                                                             */
/*----------------------------------------------------------------------------*/
template <class ITEM>
i8 RowHeap_t<ITEM>::nUsedBytes(void)
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
void RowHeap_t<ITEM>::vSetWilderness(void)
{
  if (nFreeBytes() < (i8)sizeof(Wilderness))
    return;
  
  Wilderness* aWilderness = reinterpret_cast<Wilderness*>(m_xWsp.ayLatterHead());
  
  new(aWilderness) Wilderness;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
