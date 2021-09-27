/*============================================================================*/
/* ListNL_t__.hpp -- List of Items (non-locking)                              */
/*============================================================================*/
#pragma once

namespace BASE
{
  template <class ITEM> 
  class ListNL_t__ 
  {
  protected:
    i8    m_nItems;  // number of items
    ITEM* m_pHead;   // head item
    ITEM* m_pTail;   // tail item

    // must be last
    Footer m_xFooter;
    
  public:
    ListNL_t__(void);
    
    void  vAdd(ITEM* pItem);
    void  vAppend(ITEM* pItem);
    void  vClear(void);
    bool  bContains(ITEM* pItem);
    i8    iFind(ITEM* pItem);
    ITEM* pGet(i8 i_Item);
    ITEM* pHead(void);
    void  vInsertAfter(ITEM* pItem, ITEM* pAfter);
    void  vInsertBefore(ITEM* pItem, ITEM* pBefore);
    i8    nItems(void);
    ITEM* pNext(ITEM* pItem);
    ITEM* pPop(void);
    ITEM* pPrev(ITEM* pItem);
    void  vPush(ITEM* pItem);
    void  vRemove(ITEM* pItem);
    ITEM* pTail(void);
    
    Footer* aFooter(void);
  };
}

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <class ITEM>
ListNL_t__<ITEM>::ListNL_t__(void)
{
  m_nItems = 0;
  m_pHead  = nullptr;
  m_pTail  = nullptr;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add item to head of list                                                   */
/*----------------------------------------------------------------------------*/
template <class ITEM>
void ListNL_t__<ITEM>::vAdd(ITEM* pItem)
{
  vPush(pItem);
}

/*----------------------------------------------------------------------------*/
/* append item to end of list                                                 */
/*----------------------------------------------------------------------------*/
template <class ITEM>
void ListNL_t__<ITEM>::vAppend(ITEM* pItem)
{
  ASSERT(NULL == pItem->pPrevItem());
  ASSERT(NULL == pItem->pNextItem());

  if (m_pTail)
    m_pTail->vNextItem(pItem);
  
  pItem->vPrevItem(m_pTail);
  pItem->vNextItem(nullptr);
  
  m_pTail = pItem;
  
  if (! m_pHead)
    m_pHead = pItem;
  
  m_nItems++;
}

/*----------------------------------------------------------------------------*/
/* clear out list                                                             */
/*----------------------------------------------------------------------------*/
template <class ITEM>
void ListNL_t__<ITEM>::vClear(void)
{
  m_nItems = 0;
  m_pHead  = nullptr;
  m_pTail  = nullptr;
}

/*----------------------------------------------------------------------------*/
/* test if item in list                                                       */
/*----------------------------------------------------------------------------*/
template <class ITEM>
bool ListNL_t__<ITEM>::bContains(ITEM* pItem)
{
  if (C_UNKNOWN == iFind(pItem))
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* find item in list                                                          */
/*----------------------------------------------------------------------------*/
template <class ITEM>
i8 ListNL_t__<ITEM>::iFind(ITEM* pItem)
{
  i8 iItemNo = 0;
  for (ITEM* pScan = m_pHead;
       pScan;
       pScan = pScan->pNextItem(), iItemNo++)
    if (pScan == pItem)
      return iItemNo;

  return C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/* get nth item of list                                                       */
/*----------------------------------------------------------------------------*/
template <class ITEM>
ITEM* ListNL_t__<ITEM>::pGet(i8 i_Item)
{
  ITEM* pScan = m_pHead;
  for (i8 iItemNo = 0; pScan && iItemNo < i_Item; iItemNo++)
    pScan = pScan->pNextItem();

  return pScan;
}

/*----------------------------------------------------------------------------*/
/* get head item in list                                                      */
/*----------------------------------------------------------------------------*/
template <class ITEM>
ITEM* ListNL_t__<ITEM>::pHead(void)
{
  return m_pHead;
}

/*----------------------------------------------------------------------------*/
/* insert item after given item which is already in list                      */
/*----------------------------------------------------------------------------*/
template <class ITEM>
void ListNL_t__<ITEM>::vInsertAfter(ITEM* pInsertItem, ITEM* pAfter)
{
  if (pAfter == m_pTail) {
    pInsertItem->vPrevItem(pAfter);
    pAfter->vNextItem(pInsertItem);

    m_pTail = pInsertItem;
    m_nItems++;
    return;
  }

  // Verify that pAfter is actually in this list
  ITEM* pTest = m_pHead;
  while (pTest) {

    if (pTest == pAfter) {
      pInsertItem->vNextItem(pAfter->pNextItem());
      pAfter->pNextItem()->vPrevItem(pInsertItem);

      pAfter->vNextItem(pInsertItem);
      pInsertItem->vPrevItem(pAfter);

      m_nItems++;
      return;
    }

    pTest = pTest->pNextItem();
  }

  NEVER_REACH
}

/*----------------------------------------------------------------------------*/
/* insert item before tem which is already in list                            */
/*----------------------------------------------------------------------------*/
template <class ITEM>
void ListNL_t__<ITEM>::vInsertBefore(ITEM* pInsertItem, ITEM* pBefore)
{
  if (pBefore == m_pHead) {
    pInsertItem->vNextItem(pBefore);
    pBefore->vPrevItem(pInsertItem);

    m_pHead = pInsertItem;
    m_nItems++;
    return;
  }

  // Verify that pBefore is actually in this list
  ITEM* pTest = m_pHead;
  while (pTest) {

    if (pTest == pBefore) {
      pInsertItem->vPrevItem(pBefore->pPrevItem());
      pBefore->pPrevItem()->vNextItem(pInsertItem);

      pBefore->vPrevItem(pInsertItem);
      pInsertItem->vNextItem(pBefore);

      m_nItems++;
      return;
    }

    pTest = pTest->pNextItem();
  }

  NEVER_REACH
}

/*----------------------------------------------------------------------------*/
/* get number of items in list                                                */
/*----------------------------------------------------------------------------*/
template <class ITEM>
i8 ListNL_t__<ITEM>::nItems(void)
{
  return m_nItems;
}

/*----------------------------------------------------------------------------*/
/* get next item after given item                                             */
/*----------------------------------------------------------------------------*/
template <class ITEM>
ITEM* ListNL_t__<ITEM>::pNext(ITEM* pItem)
{
  ITEM* pNext = pItem->pNextItem();
  
  return pNext;
}

/*----------------------------------------------------------------------------*/
/* pop item from head of list                                                 */
/*----------------------------------------------------------------------------*/
template <class ITEM>
ITEM* ListNL_t__<ITEM>::pPop(void)
{
  if ( ! m_pHead ) {
    ASSERT( ! m_nItems );
    return nullptr;
  }

  ITEM* pItem = m_pHead;

  m_pHead = m_pHead->pNextItem();
  if (m_pHead)
    m_pHead->vPrevItem(nullptr);
  else
    m_pTail = nullptr;
  
  m_nItems--;
  if (m_nItems) {
    ASSERT(m_pHead);
    ASSERT(m_pTail);
  }
  else {
    ASSERT( ! m_pHead );
    ASSERT( ! m_pTail )
  }
  
  pItem->vPrevItem(nullptr);
  pItem->vNextItem(nullptr);
  
  return pItem;
}

/*----------------------------------------------------------------------------*/
/* get next item after given item                                             */
/*----------------------------------------------------------------------------*/
template <class ITEM>
ITEM* ListNL_t__<ITEM>::pPrev(ITEM* pItem)
{
  ITEM* pPrev = pItem->pPrevItem();
  
  return pPrev;
}

/*----------------------------------------------------------------------------*/
/* push item on top of list                                                   */
/*----------------------------------------------------------------------------*/
template <class ITEM>
void ListNL_t__<ITEM>::vPush(ITEM* pItem)
{
  ASSERT(NULL == pItem->pPrevItem());
  ASSERT(NULL == pItem->pNextItem());
  
  pItem->vPrevItem(nullptr);
  pItem->vNextItem(m_pHead);
  
  if (m_pHead)
    m_pHead->vPrevItem(pItem);
  else
    m_pTail = pItem;
  
  m_pHead = pItem;
  
  m_nItems++;
  ASSERT(m_pHead);
  ASSERT(m_pTail)
}

/*----------------------------------------------------------------------------*/
/* remove item from list                                                      */
/*----------------------------------------------------------------------------*/
template <class ITEM>
void ListNL_t__<ITEM>::vRemove(ITEM* pRemoveItem)
{
  ITEM* pPrev = pRemoveItem->pPrevItem();
  ITEM* pNext = pRemoveItem->pNextItem();

  if (pPrev)
    pPrev->vNextItem(pNext);

  if (pNext)
    pNext->vPrevItem(pPrev);
  
  if (pRemoveItem == m_pHead)
    m_pHead = pNext;
  
  if (pRemoveItem == m_pTail)
    m_pTail = pPrev;
  
  m_nItems--;
  
  pRemoveItem->vPrevItem(nullptr);
  pRemoveItem->vNextItem(nullptr);
}

/*----------------------------------------------------------------------------*/
/* get last item in list                                                      */
/*----------------------------------------------------------------------------*/
template <class ITEM>
ITEM* ListNL_t__<ITEM>::pTail(void)
{
  return m_pTail;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
